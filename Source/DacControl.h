//
//  DacControl.h
//  checkpoint - App
//
//  Created by Hanyu Liu on 11/2/17.
//

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "ConvertADSRToSig.cpp"
#include "ConvertAmpEnvToSig.cpp"
#include "ConvertSpecEnvToSig.cpp"
#include "SoundProfileStruct.h"
#include <vector>


class DacControl : public Component, public HighResolutionTimer
{
public:
    DacControl();
    ~DacControl(){};
    
    void setADSRValues(std::vector<float> ADSRvals);
    void setEnvelopeValues(std::vector<float> currentEnvelope);
    void updateAllSig();
    
    bool buttonPlay;
    SoundProfiles DAC_TL;
    int outputSig;
private:
    void hiResTimerCallback() override;
    
    ScopedPointer<ConvertADSRToSig> ADSRToSig;
    ScopedPointer<ConvertAmpEnvToSig> AmpEnvToSig;
    ScopedPointer<ConvertSpecEnvToSig> SpecEnvToSig;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DacControl)
};


