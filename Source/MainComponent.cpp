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

        if (thumbnail.getTotalLength() > 0.0)
        {
            const double endTime = displayFullThumb ? thumbnail.getTotalLength()
                                                    : jmax (30.0, thumbnail.getTotalLength());

            Rectangle<int> thumbArea (getLocalBounds());
            thumbnail.drawChannel (g, thumbArea.reduced (2), 0.0, endTime,0 , 1.0f);
            setDisplayFullThumbnail(displayFull);
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
    // backgroundThread ("Audio Recorder Thread"), activeWriter (nullptr),
    sampleRate (0), nextSampleNum (0)
    {
        // backgroundThread.startThread();
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
            displayFull = 0;


            // // Create an OutputStream to write to our destination file...
            // file.deleteFile();
            // ScopedPointer<FileOutputStream> fileStream (file.createOutputStream());

            // if (fileStream != nullptr)
            // {
            //     // Now create a WAV writer object that writes to our output stream...
            //     WavAudioFormat wavFormat;
            //     AudioFormatWriter* writer = wavFormat.createWriterFor (fileStream, sampleRate, 1, 16, StringPairArray(), 0);

            //     if (writer != nullptr)
            //     {
            //         fileStream.release(); // (passes responsibility for deleting the stream to the writer object that is now using it)

            //         // Now we'll create one of these helper objects which will act as a FIFO buffer, and will
            //         // write the data to disk on our background thread.
            //         threadedWriter = new AudioFormatWriter::ThreadedWriter (writer, backgroundThread, 32768);

            //         // Reset our recording thumbnail
            //         thumbnail.reset (writer->getNumChannels(), writer->getSampleRate());
            //         nextSampleNum = 0;

            //         // And now, swap over our active writer pointer so that the audio callback will start using it..
            //         const ScopedLock sl (writerLock);
            //         activeWriter = threadedWriter;
            //     }
            // }
        }
    }

    void stop()
    {
        displayFull = true;
        // // First, clear this pointer to stop the audio callback from using our writer object..
        // {
        //     const ScopedLock sl (writerLock);
        //     activeWriter = nullptr;
        // }
        // // Now we can delete the writer object. It's done in this order because the deletion could
        // // take a little time while remaining data gets flushed to disk, so it's best to avoid blocking
        // // the audio callback while this happens.
        // threadedWriter = nullptr;
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
            amplitudeExtractors.push_back(new AmplitudeExtractor(systemBufferSize, sampleRate));
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
            // std::cout << signalStatus << std::endl;

        }

        triggerThumbnail(numSamples, wavFileBuffer);

    //    const ScopedLock sl (writerLock);
    //    if (signalStatus == 1)
    //    {
    //     //    activeWriter->write (inputChannelData, numSamples);
    //        thumbnail.addBlock (nextSampleNum, wavFileBuffer, 0, numSamples);
    //        nextSampleNum += numSamples;
    //    }
        
        for (int i = 0; i < numOutputChannels; ++i)
        {
            if (outputChannelData[i] != nullptr)
                FloatVectorOperations::clear (outputChannelData[i], numSamples);
        }
    }

private:
    AudioThumbnail& thumbnail;
    bool& displayFull;
    // TimeSliceThread backgroundThread; // the thread that will write our audio data to disk
    // ScopedPointer<AudioFormatWriter::ThreadedWriter> threadedWriter; // the FIFO used to buffer the incoming data
    AudioSampleBuffer wavFileBuffer, readBuffer;
    std::vector<ScopedPointer<AmplitudeExtractor>> amplitudeExtractors;
    
    double sampleRate;
    int numChannels;
    int systemBufferSize;
    int signalStatus;
    int lastSignalStatus;
    int64 nextSampleNum;
    

    CriticalSection writerLock;
    AudioFormatWriter::ThreadedWriter* volatile activeWriter;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioRecorder)
};


//===============================================================================================================
class AudioRecordingDemo  : public Component,
                            private Button::Listener
{
public:
    AudioRecordingDemo()
    :
    deviceManager (),
    recorder (recordingThumbnail.getAudioThumbnail(),recordingThumbnail.getDisplayfull())
    {
        setOpaque (true);
        addAndMakeVisible (liveAudioScroller);

        // addAndMakeVisible (explanationLabel);
        // explanationLabel.setText ("This page demonstrates how to record a wave file from the live audio input..\n\nPressing record will start recording a file in your \"Documents\" folder.", dontSendNotification);
        // explanationLabel.setFont (Font (15.00f, Font::plain));
        // explanationLabel.setJustificationType (Justification::topLeft);
        // explanationLabel.setEditable (false, false, false);
        // explanationLabel.setColour (TextEditor::textColourId, Colours::black);
        // explanationLabel.setColour (TextEditor::backgroundColourId, Colour (0x00000000));

        addAndMakeVisible (recordButton);
        recordButton.setButtonText ("Record");
        recordButton.addListener (this);
        recordButton.setColour (TextButton::buttonColourId, Colour (0xffff5c5c));
        recordButton.setColour (TextButton::textColourOnId, Colours::black);

        addAndMakeVisible (recordingThumbnail);

        deviceManager.initialise (2, 2, nullptr, true, String(), nullptr);
        deviceManager.addAudioCallback (&liveAudioScroller);
        deviceManager.addAudioCallback (&recorder);

        setSize (480, 320);
        std::cout << "Application start" << std::endl;
    }

    ~AudioRecordingDemo()
    {
        deviceManager.removeAudioCallback (&recorder);
        deviceManager.removeAudioCallback (&liveAudioScroller);
        std::cout << "Application close" << std::endl;
    }

    void paint (Graphics& g) override
    {
        g.fillAll (Colour (0xffaaaaaa));
        g.setColour (Colour (0xffffffff));
    }

    void resized() override
    {
        Rectangle<int> area (getLocalBounds());
        liveAudioScroller.setBounds (area.removeFromTop (80).reduced (0));
        recordingThumbnail.setBounds (area.removeFromTop (80).reduced (0));
        recordButton.setBounds (area.removeFromTop (36).removeFromLeft (140).reduced (8));
        // explanationLabel.setBounds (area.reduced (8));
    }

private:
    AudioDeviceManager deviceManager;
    LiveScrollingAudioDisplay liveAudioScroller;
    RecordingThumbnail recordingThumbnail;
    AudioRecorder recorder;
    Label explanationLabel;
    TextButton recordButton;

    // void startRecording()
    // {
    //     // const File file (File::getSpecialLocation (File::userDocumentsDirectory)
    //     //                     .getNonexistentChildFile ("Juce Demo Audio Recording", ".wav"));
    //     recorder.startRecording ();

    //     recordButton.setButtonText ("Stop");
    //     recordingThumbnail.setDisplayFullThumbnail (false);
    // }

    // void stopRecording()
    // {
    //     recorder.stop();
    //     recordButton.setButtonText ("Record");
    //     recordingThumbnail.setDisplayFullThumbnail (true);
    // }

    void buttonClicked (Button* button) override
    {
        // if (button == &recordButton)
        // {
        //     if (recorder.isRecording())
        //         stopRecording();
        //     else
        //         startRecording();
        // }
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioRecordingDemo)
};
