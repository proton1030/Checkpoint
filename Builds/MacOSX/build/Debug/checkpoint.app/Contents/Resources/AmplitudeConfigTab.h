//
//  AmplitudeConfigTab.h
//  checkpoint
//
//  Created by Hanyu Liu on 11/1/17.
//
#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "TrainingTab.h"

class AmplitudeConfigTab : public Component, public Slider::Listener, public ChangeListener, public ButtonListener, public ChangeBroadcaster
{
public:
    AmplitudeConfigTab(const ScopedPointer<TrainingTab>& trainingtab);
    ~AmplitudeConfigTab(){};
    
    void paint (Graphics& g) override;
    void resized() override;
    
    std::vector<float> getADSRSettings();
    bool getTriggerButton();
    
private:
    std::vector<float>& fetchADSRValues(std::vector<float>& AverageADSR);
    void setSliderValues(std::vector<float>&);
    void sliderValueChanged (Slider* slider) override;
    void changeListenerCallback(ChangeBroadcaster* source) override;
    void buttonClicked (Button* buttonThatWasClicked) override;
    
    bool currentOutputMode;
    std::vector<float> currentSliderValues;
    bool currentTriggerStatus;
    TrainingTab* trainingTabContents;
    Label infoLabel;
    Slider attack, decay, sustain, release;
    ScopedPointer<TextButton> switchMode;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AmplitudeConfigTab)
};
