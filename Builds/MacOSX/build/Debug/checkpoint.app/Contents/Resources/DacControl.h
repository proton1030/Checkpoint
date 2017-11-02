//
//  DacControl.h
//  checkpoint - App
//
//  Created by Hanyu Liu on 11/2/17.
//

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "AmplitudeConfigTab.h"
#include "TrainingTab.h"
#include <vector>

#define MaxVoltage 5.0f

class DacControl : public Component, public HighResolutionTimer, public ChangeListener
{
public:
    DacControl(const ScopedPointer<AmplitudeConfigTab>& amplitude);
    ~DacControl(){};
    
private:
    void hiResTimerCallback() override;
    float ADSRVoltageOutput();
    void changeListenerCallback(ChangeBroadcaster* source) override;
    
    //ADSR Voltage Parameters
    int ADSRStatus;
    float currentOutputVoltage;
    bool outputVoltageStatus;
    int currentStatusCnt;
    
    float outputFrequency;
    int test;
    AmplitudeConfigTab* amplitudeTabContents;
    std::vector<float> ADSR;
    bool buttonPlay;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DacControl)
};


