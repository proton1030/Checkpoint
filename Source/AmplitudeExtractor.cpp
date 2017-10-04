//
//  AmplitudeExtractor.cpp
//  recordingtest - App
//
//  Created by Hanyu Liu on 10/3/17.
//

#include "../JuceLibraryCode/JuceHeader.h"
#include "AmplitudeExtractor.h"
#include <iostream>
#include <vector>

AmplitudeExtractor::AmplitudeExtractor(const int& systemBufferSize, const double& sampleRate): systemBufferSize(systemBufferSize), sampleRate(sampleRate)
{
    initialize();
    
};

void AmplitudeExtractor::initialize()
{
    //Internal variables settings
    currentBlockPower = 0.0f;
    earlyBlockNums = 1;
    backgroundPower = 0.0f;
    currentlyInSignalFlag = false;
    currentSignalPowerSeq = {};
    currentSignalDuration = 0;

    //Initialization settings
    backgroundEstimationBlockNumThres = 20;
    signalDetectionThres = 800;
    signalDurationThres = 20;
    
    
};
void AmplitudeExtractor::clear()
{
    
};

void AmplitudeExtractor::process(const float* currentBlockPtr)
{
    currentBlockPower = 0.0f;
    for (int i = 0; i < systemBufferSize; i++) {
        currentBlockPower += pow(currentBlockPtr[i], 2.0f);
    }

    std::cout << currentlyInSignalFlag << "  |  " << currentBlockPower << "  |  " << currentSignalDuration << std::endl;

    if (earlyBlockNums <= backgroundEstimationBlockNumThres) {
        backgroundPowerEstimation(currentBlockPower);
    }
    else{
        if (!currentlyInSignalFlag && currentBlockPower >= signalDetectionThres * backgroundPower ) 
        {
            currentlyInSignalFlag = true;
            currentSignalPowerSeq.push_back(currentBlockPower);
            currentSignalDuration += 1;
    
        }
        else if (currentlyInSignalFlag && currentBlockPower >= signalDetectionThres * backgroundPower)
        {
            currentSignalDuration += 1;
            currentSignalPowerSeq.push_back(currentBlockPower);
        }
        else if (currentlyInSignalFlag && currentBlockPower < signalDetectionThres * backgroundPower)
        {
            if (currentSignalDuration >= signalDurationThres)
            {
                currentlyInSignalFlag = false;
                calculateADSR();
                currentSignalPowerSeq = {};
                currentSignalDuration = 0;
            }
            else
            {
                currentlyInSignalFlag = false;
                currentSignalPowerSeq = {};
                currentSignalDuration = 0;

            }
            // std::cout << "Signal detected" << std::endl;
        }
    }
    
    

    
};

void AmplitudeExtractor::backgroundPowerEstimation(float blockPower)
{
    backgroundPower += blockPower;
    earlyBlockNums += 1;
    if (earlyBlockNums > backgroundEstimationBlockNumThres){
        backgroundPower /= backgroundEstimationBlockNumThres;
    }

};

void AmplitudeExtractor::calculateADSR()
{
    std::cout << "Perform ADSR Extraction" << std::endl;
};


