//
//  MainUITabs.h
//  checkpoint
//
//  Created by Hanyu Liu on 10/24/17.
//
#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "TrainingTab.h"
#include "AmplitudeConfigTab.h"
#include "DacControl.h"
#include "PresetTab.h"


class MainUITabs : public TabbedComponent, public ChangeListener, public ButtonListener, public Value::Listener
{
public:
    
    MainUITabs();
    ~MainUITabs();
    
private:
    void paint (Graphics& g) override;
    
    ScopedPointer<DacControl> DAC;
    ScopedPointer<TrainingTab> training;
    ScopedPointer<AmplitudeConfigTab> amplitude;
    ScopedPointer<PresetTab> presetSetting;
    
    int ampMode;
    AmplitudeExtractor* ampExtModule;
    void changeListenerCallback(ChangeBroadcaster* source) override;
    void buttonClicked (Button* buttonThatWasClicked) override;
    void valueChanged (Value& value) override;
};
    
