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


class MainUITabs : public TabbedComponent
{
public:
    
    MainUITabs();
    ~MainUITabs();
    
private:
    ScopedPointer<DacControl> DAC;
    ScopedPointer<TrainingTab> training;
    ScopedPointer<AmplitudeConfigTab> amplitude;
    Value attack;
    
};
    
