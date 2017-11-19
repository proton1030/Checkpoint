//
//  DacControl.h
//  checkpoint - App
//
//  Created by Hanyu Liu on 11/2/17.
//

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "ConvertADSRToSig.cpp"
#include <vector>

#define MaxVoltage 5.0f

class DacControl : public Component, public HighResolutionTimer
{
public:
    DacControl();
    ~DacControl(){};
    
    void setADSRValues(std::vector<float> ADSRvals);
    
    bool buttonPlay;
private:
    void hiResTimerCallback() override;

    ConvertADSRToSig ADSRToSig;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DacControl)
};


