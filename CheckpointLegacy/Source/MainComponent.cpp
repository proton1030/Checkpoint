//
//  MainComponent.cpp
//  recordingtest
//
//  Created by Hanyu Liu on 10/3/17.
//

#include "../JuceLibraryCode/JuceHeader.h"
#include "AudioLiveScrollingDisplay.h"
#include "AmplitudeExtractor.h"
#include "SpectralExtractor.h"
#include <iostream>
#include <vector>


//===============================================================================================================
class RecordingThumbnail  : public Component,
                            private ChangeListener
{
public:
    String thumbnailText;
    bool standbyFlag;
    
    RecordingThumbnail(): thumbnailCache (10), thumbnail (512, formatManager, thumbnailCache), displayFullThumb (false)
    {
        thumbnailText = "Standby";
        standbyFlag = 1;
        formatManager.registerBasicFormats();
        thumbnail.addChangeListener (this);
    }

    ~RecordingThumbnail()
    {
        thumbnail.removeChangeListener (this);
    }
    
    AudioThumbnail& getAudioThumbnail()     { return thumbnail; }
    bool& getDisplayfull() { return displayFull; }

    void setDisplayFullThumbnail (bool display)
    {
        displayFullThumb = display;
        repaint();
    }

    void paint (Graphics& g) override
    {
        g.fillAll (Colour(0xff222222));
        g.setFont(Font("Courier New", "Regular", 20.0f));
        g.setColour (Colours::lightgrey);
        g.drawRect(-1.0f, 0.0f, (float)proportionOfWidth(1.0f)+2.0f, (float)proportionOfHeight(1.0f), 0.3f);
        setDisplayFullThumbnail(displayFull);
        if (thumbnail.getTotalLength() > 0.0 && !standbyFlag)
        {
            const double endTime = displayFullThumb ? thumbnail.getTotalLength()
                                                    : jmax (30.0, thumbnail.getTotalLength());

            Rectangle<int> thumbArea (getLocalBounds());
            thumbnail.drawChannel (g, thumbArea.reduced (2), 0.0, endTime,0 , 1.0f);
            
        }
        else
        {
            g.setFont (14.0f);
            g.drawFittedText (thumbnailText, getLocalBounds(), Justification::centred, 2);
        }
    }

private:
    AudioFormatManager formatManager;
    AudioThumbnailCache thumbnailCache;
    AudioThumbnail thumbnail;
    bool displayFull;
    bool displayFullThumb;
    

    void changeListenerCallback (ChangeBroadcaster* source) override
    {
        if (source == &thumbnail)
            repaint();
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RecordingThumbnail)
};


//===============================================================================================================


class AudioRecorder  : public Component, public AudioIODeviceCallback, public ChangeBroadcaster
{
public:
    AudioRecorder (AudioThumbnail& thumbnailToUpdate, bool& displayFullToUpdate) : thumbnail (thumbnailToUpdate), displayFull (displayFullToUpdate),
    sampleRate (0), nextSampleNum (0)
    {
        // backgroundThread.startThread();
        trainingOrNot = true;
        sampleCnt = 0;
    }

    ~AudioRecorder()
    {
        stop();
    }
    

    // const File& file
    void startRecording ()
    {
        stop();

        if (sampleRate > 0)
        {
            // Reset our recording thumbnail
            thumbnail.reset (numChannels, sampleRate);
            nextSampleNum = 0;
            displayFull = false;
        }
    }

    void stop()
    {
        displayFull = true;
    }

    bool isRecording()
    {
        return signalStatus;
    }

    void triggerThumbnail(int numSamples, AudioSampleBuffer wavFileBuffer)
    {
        if (signalStatus == 1 && lastSignalStatus == 0)
            startRecording();
        else if (signalStatus == 0 && lastSignalStatus == 1)
            stop();

        else if (signalStatus == 1 && lastSignalStatus == 1)
        {
            thumbnail.addBlock (nextSampleNum, wavFileBuffer, 0, numSamples);
            nextSampleNum += numSamples;
        }
        lastSignalStatus = signalStatus;
    }

    void audioDeviceAboutToStart (AudioIODevice* device) override
    {
        numChannels = 2;
        sampleRate = device->getCurrentSampleRate();
        systemBufferSize = device->getCurrentBufferSizeSamples();
        amplitudeExtractors.clear();
        spectralExtractors.clear();
        for (int channel = 0;channel < numChannels;channel++) {
            AmplitudeExtractor* ampExt = new AmplitudeExtractor(systemBufferSize, sampleRate);
            SpectralExtractor* specExt = new SpectralExtractor(systemBufferSize, sampleRate);
            amplitudeExtractors.push_back(ampExt);
            spectralExtractors.push_back(specExt);
        }

        signalStatus = 0;
        lastSignalStatus = 0;
        
        std::cout << "Sample Rate: "<< sampleRate << std::endl << "System Buffer Size: "<< systemBufferSize << std::endl;
    }

    void audioDeviceStopped() override
    {
        sampleRate = 0;
    }

    void audioDeviceIOCallback (const float** inputChannelData, int /*numInputChannels*/,
                                float** outputChannelData, int numOutputChannels,
                                int numSamples) override
    {
        AudioSampleBuffer wavFileBuffer (const_cast<float**> (inputChannelData), numOutputChannels, numSamples);
        AudioSampleBuffer readBuffer (const_cast<float**> (inputChannelData), numOutputChannels, numSamples);
        
//        std::cout<< trainingOrNot << std::endl;
        if (trainingOrNot)
        {
            for (int channel = 0; channel < numOutputChannels-1; channel++) {
                signalStatus = amplitudeExtractors[channel]->process(readBuffer.getReadPointer(channel));
                spectralExtractors[channel]->process(readBuffer.getReadPointer(channel), signalStatus);
            }
            if (signalStatus == 1)
                sampleCnt ++;
            else if (signalStatus == 0 && sampleCnt > sampleThres)
            {
                sendChangeMessage();
                sampleCnt = 0;
            }

            triggerThumbnail(numSamples, wavFileBuffer);
        }
        
        for (int i = 0; i < numOutputChannels; ++i)
        {
            if (outputChannelData[i] != nullptr)
                FloatVectorOperations::clear (outputChannelData[i], numSamples);
        }
    }
    
    std::vector<float>& getADSRValues()
    {
        return amplitudeExtractors[0]->getFinalADSR();
    }
    
    ScopedPointer<AmplitudeExtractor>& getAmpExtModule()
    {
        return amplitudeExtractors[0];
    }
    
    ScopedPointer<SpectralExtractor>& getSpecExtModule()
    {
        return spectralExtractors[0];
    }
    
    
    void setExtractorWorking(bool workingOrNot)
    {
        trainingOrNot = workingOrNot;
    }
    
    
private:
    AudioThumbnail& thumbnail;
    bool& displayFull;
    AudioSampleBuffer wavFileBuffer, readBuffer;
    std::vector<ScopedPointer<AmplitudeExtractor>> amplitudeExtractors;
    std::vector<ScopedPointer<SpectralExtractor>> spectralExtractors;
    
    double sampleRate;
    int numChannels;
    int systemBufferSize;
    int signalStatus;
    int lastSignalStatus;
    int64 nextSampleNum;
    bool trainingOrNot;
    int sampleCnt;
    int sampleThres = 20;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioRecorder)
};
