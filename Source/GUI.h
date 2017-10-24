
#pragma once


#include "../JuceLibraryCode/JuceHeader.h"
#include "MainComponent.cpp"


class GUI  : public Component
{
public:
   
    GUI ();
    ~GUI();

    void paint (Graphics& g) override;
    void resized() override;



private:
 
    AudioDeviceManager deviceManager;
    ScopedPointer<AudioRecorder> recorder;

    ScopedPointer<TabbedComponent> tabbedComponent;
    ScopedPointer<LiveScrollingAudioDisplay> liveAudioScroller;
    ScopedPointer<RecordingThumbnail> recordingThumbnail;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GUI)
};

