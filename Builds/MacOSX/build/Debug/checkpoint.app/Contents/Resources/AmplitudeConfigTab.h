//
//  AmplitudeConfigTab.h
//  checkpoint
//
//  Created by Hanyu Liu on 11/1/17.
//
#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "TrainingTab.h"

class AmplitudeConfigTab : public Component, public Slider::Listener, public ButtonListener, public ChangeBroadcaster
{
public:
    AmplitudeConfigTab();
    ~AmplitudeConfigTab();
    void paint (Graphics& g) override;
    void resized() override;
    void setSliderValues(std::vector<float>);
    std::vector<float> getADSRValues();
    
    Value currentOutputMode;
    ScopedPointer<TextButton> switchMode;
private:
    void sliderValueChanged (Slider* slider) override;
    void buttonClicked (Button* buttonThatWasClicked) override;

    std::vector<float> currentSliderValues;
    bool currentTriggerStatus;
    Label infoLabel;
    ScopedPointer<Slider> attack, decay, sustain, release;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AmplitudeConfigTab)
};
