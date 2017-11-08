//
//  MainComponent.cpp
//  recordingtest
//
//  Created by Hanyu Liu on 10/3/17.
//

#include "../JuceLibraryCode/JuceHeader.h"
#include "AudioLiveScrollingDisplay.h"
#include "AmplitudeExtractor.h"
#include <iostream>
#include <vector>


//===============================================================================================================
class RecordingThumbnail  : public Component,
                            private ChangeListener
{
public:

    RecordingThumbnail(): thumbnailCache (10), thumbnail (512, formatManager, thumbnailCache), displayFullThumb (false)
    {
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
        g.fillAll (Colours::darkgrey);
        g.setColour (Colours::lightgrey);
        setDisplayFullThumbnail(displayFull);
        if (thumbnail.getTotalLength() > 0.0)
        {
            const double endTime = displayFullThumb ? thumbnail.getTotalLength()
                                                    : jmax (30.0, thumbnail.getTotalLength());

            Rectangle<int> thumbArea (getLocalBounds());
            thumbnail.drawChannel (g, thumbArea.reduced (2), 0.0, endTime,0 , 1.0f);
            
        }
        else
        {
            g.setFont (14.0f);
            g.drawFittedText ("No signal detected", getLocalBounds(), Justification::centred, 2);
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


class AudioRecorder  : public Component, public AudioIODeviceCallback
{
public:
    AudioRecorder (AudioThumbnail& thumbnailToUpdate, bool& displayFullToUpdate) : thumbnail (thumbnailToUpdate), displayFull (displayFullToUpdate),
    sampleRate (0), nextSampleNum (0)
    {
        // backgroundThread.startThread();

    }

    ~AudioRecorder()
    {
        stop();
    }
    
    void initializing ()
    {
        
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
        std::cout << signalStatus << std::endl;
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
        for (int channel = 0;channel < numChannels;channel++) {
            AmplitudeExtractor* ampExt = new AmplitudeExtractor(systemBufferSize, sampleRate);
//            ampExt->addChangeListener(this);
            amplitudeExtractors.push_back(ampExt);
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
        AudioSampleBuffer wavFileBuffer (const_cast<float**> (inputChannelData), thumbnail.getNumChannels(), numSamples);
        AudioSampleBuffer readBuffer (const_cast<float**> (inputChannelData), numOutputChannels, numSamples);
        
        for (int channel = 0; channel < numOutputChannels-1; channel++) {
            signalStatus = amplitudeExtractors[channel]->process(readBuffer.getReadPointer(channel));
            ADSRValues = amplitudeExtractors[channel]->getAverageADSRCache();
        }

        triggerThumbnail(numSamples, wavFileBuffer);

        
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
    
    
private:
    AudioThumbnail& thumbnail;
    bool& displayFull;
    AudioSampleBuffer wavFileBuffer, readBuffer;
    std::vector<ScopedPointer<AmplitudeExtractor>> amplitudeExtractors;
    
    double sampleRate;
    int numChannels;
    int systemBufferSize;
    int signalStatus;
    int lastSignalStatus;
    int64 nextSampleNum;
    std::vector<float> ADSRValues;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioRecorder)
};
