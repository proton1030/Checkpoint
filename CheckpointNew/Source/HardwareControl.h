/*
  ==============================================================================

    HardwareControl.h
    Created: 23 Apr 2018 1:00:06pm
    Author:  Hanyu Liu

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class HardwareControl
{
public:
    enum ControllingParams_t
    {
        k_VCO_VoltagePerOctave,
        k_VCO_FMGen,
        k_VCO_PWM,
        k_VCO_FMType,
        k_MIX1_SineAmt,
        k_MIX1_TriangleAmt,
        k_MIX1_SawAmt,
        k_MIX1_SquareAmt,
        k_VCF_Resonance,
        k_VCF_Cutoff,
        k_MIX2_LowpassAmt,
        k_MIX2_BandpassAmt,
        k_MIX2_HighpassAmt,
        k_MIX2_VCOAmt,
        k_VCA_Level,
        k_VCA_BleedAmt,
        
        kNumControllingParams
    };
    
    enum ControllingParamsBounds_t
    {
        kMinValue,
        kMaxValue,
        
        kNumControllingParamsBounds
    };
    
    HardwareControl();
    ~HardwareControl();
    
    void setParam(ControllingParams_t eParam, int iParamValue);
    
    int iControllingParams[kNumControllingParams] = {0};
    
private:
    
    void setVCOBaseFreq();
    void setVCOFreqModulation();
    void setVCOPWAndFMAmt();
    void setMIX1Values();
    void setVCFCutoffAndResonance();
    void setMIX2Values();
    void setVCALevelAndBleed();
    
    int iSPIMode;
    int iVCOFMWaveformType;
    int iParamBounds[kNumControllingParams][kNumControllingParamsBounds] = {0};

    
};
