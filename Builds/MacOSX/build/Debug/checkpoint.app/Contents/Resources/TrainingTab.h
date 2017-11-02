//
//  TrainingTab.h
//  checkpoint
//
//  Created by Hanyu Liu on 11/1/17.
//
#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "MainComponent.cpp"

class TrainingTab : public Component, public ChangeListener, public ChangeBroadcaster, public ButtonListener
{
public:
    
    TrainingTab();
    ~TrainingTab();
    
    void paint (Graphics& g) override;
    void resized() override;
    std::vector<float>& getADSRValues();
    bool getTriggerButtonStatus();
    ScopedPointer<TextButton> trigger;
    
private:
    void changeListenerCallback (ChangeBroadcaster* source) override;
    void buttonClicked (Button* buttonThatWasClicked) override;
    void buttonStateChanged(Button* buttonThatWasClicked) override;
    
    AudioDeviceManager deviceManager;
    ScopedPointer<AudioRecorder> recorder;
    
    ScopedPointer<LiveScrollingAudioDisplay> liveAudioScroller;
    ScopedPointer<RecordingThumbnail> recordingThumbnail;
    
    ScopedPointer<TextButton> reset;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TrainingTab)
};
