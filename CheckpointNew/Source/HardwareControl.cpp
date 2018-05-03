/*
  ==============================================================================

    HardwareControl.cpp
    Created: 23 Apr 2018 1:00:06pm
    Author:  Hanyu Liu

  ==============================================================================
*/

#include "HardwareControl.h"


HardwareControl::HardwareControl()
{
    
}

HardwareControl::~HardwareControl()
{
    
}

void HardwareControl::setParam(ControllingParams_t eParam, int iParamValue)
{
    switch (eParam)
    {
        case k_VCO_VoltagePerOctave:
            iControllingParams[eParam] = iParamValue;
            iSPIMode = 1;
            setVCOBaseFreq();
            break;
        case k_VCO_FMGen:
        case k_VCO_FMType:
            iControllingParams[eParam] = iParamValue;
            iSPIMode = 2;
            setVCOFreqModulation();
            break;
        case k_VCO_PWM:
            iControllingParams[eParam] = iParamValue;
            iSPIMode = 0;
            setVCOPWAndFMAmt();
            break;
        case k_MIX1_SineAmt:
        case k_MIX1_TriangleAmt:
        case k_MIX1_SawAmt:
        case k_MIX1_SquareAmt:
            iControllingParams[eParam] = iParamValue;
            iSPIMode = 0;
            setMIX1Values();
            break;
        case k_VCF_Resonance:
        case k_VCF_Cutoff:
            iControllingParams[eParam] = iParamValue;
            iSPIMode = 0;
            setVCFCutoffAndResonance();
            break;
        case k_MIX2_LowpassAmt:
        case k_MIX2_BandpassAmt:
        case k_MIX2_HighpassAmt:
        case k_MIX2_VCOAmt:
            iControllingParams[eParam] = iParamValue;
            iSPIMode = 0;
            setMIX2Values();
            break;
        case k_VCA_Level:
        case k_VCA_BleedAmt:
            iControllingParams[eParam] = iParamValue;
            iSPIMode = 0;
            setVCALevelAndBleed();
            break;
        default:
        case kNumControllingParams:
            break;
    }
}

void HardwareControl::setVCOBaseFreq()
{
    std::cout << "--------------------------------------" << std::endl;
    std::cout << "Updated VCO V/O: " << iControllingParams[k_VCO_VoltagePerOctave] << std::endl;
}

void HardwareControl::setVCOFreqModulation()
{
    std::cout << "--------------------------------------" << std::endl;
    std::cout << "Updated FM Freq: " << iControllingParams[k_VCO_FMGen] << std::endl;
    std::cout << "FM Waveform Type: " << iControllingParams[k_VCO_FMType] << std::endl;
}

void HardwareControl::setVCOPWAndFMAmt()
{
    std::cout << "--------------------------------------" << std::endl;
    std::cout << "Set VCO PW: " << iControllingParams[k_VCO_PWM] << std::endl;
}

void HardwareControl::setMIX1Values()
{
    std::cout << "--------------------------------------" << std::endl;
    std::cout << "Set MIX 1 Levels: " << iControllingParams[k_MIX1_SineAmt] << "/" << iControllingParams[k_MIX1_TriangleAmt] << "/"<< iControllingParams[k_MIX1_SawAmt] << "/"<< iControllingParams[k_MIX1_SquareAmt] << "/"<< std::endl;
}

void HardwareControl::setVCFCutoffAndResonance()
{
    std::cout << "--------------------------------------" << std::endl;
    std::cout << "Set VCF Cutoff: " << iControllingParams[k_VCF_Cutoff] << std::endl;
    std::cout << "Set VCF Reso: " << iControllingParams[k_VCF_Resonance] << std::endl;
}

void HardwareControl::setMIX2Values()
{
    std::cout << "--------------------------------------" << std::endl;
    std::cout << "Set MIX 2 Levels: " << iControllingParams[k_MIX2_LowpassAmt] << "/" << iControllingParams[k_MIX2_BandpassAmt] << "/" << iControllingParams[k_MIX2_HighpassAmt] << "/" << iControllingParams[k_MIX2_VCOAmt] << "/" << std::endl;
}

void HardwareControl::setVCALevelAndBleed()
{
    std::cout << "--------------------------------------" << std::endl;
    std::cout << "Set VCA Level : " << iControllingParams[k_VCA_Level] << std::endl;
    std::cout << "Set VCA Bleed: " << iControllingParams[k_VCA_BleedAmt] << std::endl;
}


