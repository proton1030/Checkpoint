//
//  TrainingTab.h
//  checkpoint
///Users/liuhanyu1030/Documents/JUCE/Checkpoint/Source/TrainingTab.h
//  Created by Hanyu Liu on 11/1/17.
//
#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "MainComponent.cpp"

class TrainingTab : public Component, public ChangeBroadcaster, public ButtonListener
{
public:
    
    TrainingTab();
    ~TrainingTab();
    
    void paint (Graphics& g) override;
    void resized() override;
    std::vector<float>& getADSRValues();
    bool getTriggerButtonStatus();
    ScopedPointer<TextButton> train, trigger, reset;
    ScopedPointer<AmplitudeExtractor>& getAmpExtModule();
    ScopedPointer<AudioRecorder>& getRecorderModule();
    ScopedPointer<SpectralExtractor>& getSpecExtModule();
    void setExtractorWorking(bool workingOrNot);
    
private:
    bool trainingStatus;
    
    void buttonClicked (Button* buttonThatWasClicked) override;
    void buttonStateChanged(Button* buttonThatWasClicked) override;
    
    ScopedPointer<AudioDeviceManager> deviceManager;
    ScopedPointer<AudioRecorder> recorder;
    
    ScopedPointer<LiveScrollingAudioDisplay> liveAudioScroller;
    ScopedPointer<RecordingThumbnail> recordingThumbnail;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TrainingTab)
};
