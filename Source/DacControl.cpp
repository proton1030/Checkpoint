//
//  DacControl.cpp
//  checkpoint - App
//
//  Created by Hanyu Liu on 10/25/17.
//
#include "../JuceLibraryCode/JuceHeader.h"
#include "DacControl.h"
#include <iostream>
#include <vector>

DacControl::DacControl()
{
    buttonPlay = false;
    ADSRToSig = new ConvertADSRToSig(DAC_TL);
    AmpEnvToSig = new ConvertAmpEnvToSig(DAC_TL);
    SpecEnvToSig = new ConvertSpecEnvToSig(DAC_TL);
    const float sampleRate = 44100.0f;
    const float blkSize = 512.0f;
    startTimer(blkSize*1000.0/sampleRate);
    outputSig = 1;
}

void DacControl::hiResTimerCallback()
{
    std::cout <<outputSig<<std::endl;
    if (outputSig == 1)
        float temp = SpecEnvToSig->process(buttonPlay);
    else if (outputSig == 2)
        float temp = AmpEnvToSig->process(buttonPlay);
    else
        float temp = ADSRToSig->process(buttonPlay);

}

void DacControl::updateAllSig()
{
    ADSRToSig->update(DAC_TL);
    AmpEnvToSig->update(DAC_TL);
    SpecEnvToSig->update(DAC_TL);
}

//void DacControl::setADSRValues(std::vector<float> ADSRvals)
//{
//    ADSRToSig.setADSRValues(ADSRvals);
//    AmpEnvToSig.setADSRValues(ADSRvals);
//    SpecEnvToSig.setADSRValues(ADSRvals);
//}
//
//void DacControl::setEnvelopeValues(std::vector<float> currentEnvelope)
//{
//    AmpEnvToSig.setEnvelope(currentEnvelope);
//}












