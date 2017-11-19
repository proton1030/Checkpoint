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
    
    const float sampleRate = 44100.0f;
    const float blkSize = 512.0f;
    startTimer(blkSize*1000.0/sampleRate);
}

void DacControl::hiResTimerCallback()
{
//    loat temp = 
    ADSRToSig.process(buttonPlay);
//    std::cout << temp << std::endl;
}

void DacControl::setADSRValues(std::vector<float> ADSRvals)
{
    ADSRToSig.setADSRValues(ADSRvals);
}




