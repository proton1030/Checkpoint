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
// #include <thread>
#include <algorithm>
#include <math.h>

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
    currentSignalPowerSum = 0;
    currentSignalDuration = 0;
    attackAndReleaseTime = {0.0f , 0.0f};

    //Initialization settings
    backgroundEstimationBlockNumThres = 20;
    signalDetectionThres = 40;
    signalDurationThres = 20;
    averagingOrder = 4;
    
    
};
void AmplitudeExtractor::clear()
{
    
};

int AmplitudeExtractor::process(const float* currentBlockPtr)
{
    currentBlockPower = 0.0f;
    for (int i = 0; i < systemBufferSize; i++) {
        currentBlockPower += pow(currentBlockPtr[i], 2.0f);
    }
    currentBlockRMS = sqrt (currentBlockPower / systemBufferSize);

    if (earlyBlockNums <= backgroundEstimationBlockNumThres) {
        backgroundPowerEstimation(currentBlockRMS);
        return 0;
    }
    else{
        if (!currentlyInSignalFlag && currentBlockRMS >= signalDetectionThres * backgroundPower ) 
        {
            currentlyInSignalFlag = true;
            currentSignalPowerSeq.push_back(currentBlockRMS);
            currentSignalDuration += 1;
            currentSignalPowerSum += currentBlockRMS;
            return 1;
            
    
        }
        else if (currentlyInSignalFlag && currentBlockRMS >= signalDetectionThres * backgroundPower)
        {
            currentSignalDuration += 1;
            currentSignalPowerSeq.push_back(currentBlockRMS);
            currentSignalPowerSum += currentBlockRMS;
            return 1;
        }
        else if (currentlyInSignalFlag && currentBlockRMS < signalDetectionThres * backgroundPower)
        {
            if (currentSignalDuration >= signalDurationThres)
            {
                currentlyInSignalFlag = false;
                // std::thread t(&AmplitudeExtractor::calculateADSR, this);
                // t.detach();
                calculateADSR();
                currentSignalPowerSeq = {};
                currentSignalDuration = 0;
                return 0;
            }
            else
            {
                currentlyInSignalFlag = false;
                currentSignalPowerSeq = {};
                currentSignalDuration = 0;
                return 0;
            }
        }
    }
    return 0;
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
    int currentSignalSize = (int)currentSignalPowerSeq.size();
    float attackEndDetection;
    float releaseBeginDetection;
    float attackDetectionMax = 0.0f, releaseDetectionMax = 0.0f;
    int attackDetectionMaxIndex = 0, releaseDetectionMaxIndex = 0;

    // averageFiltering(averagingOrder);

    for (int i = 1; i < currentSignalSize; i++) 
    {
        attackEndDetection = currentSignalPowerSeq[i] * (currentSignalSize - i/2);
        releaseBeginDetection = currentSignalPowerSeq[i] * (i/2 + currentSignalSize/2);
        if (attackEndDetection > attackDetectionMax)
        {
            attackDetectionMax = attackEndDetection;
            attackDetectionMaxIndex = i;
        }
        if (releaseBeginDetection > releaseDetectionMax)
        {
            releaseDetectionMax = releaseBeginDetection;
            releaseDetectionMaxIndex = i;
        }
    }

    // * systemBufferSize / sampleRate
    attackAndReleaseTime[0] = (attackDetectionMaxIndex+1);
    attackAndReleaseTime[1] = (currentSignalSize - releaseDetectionMaxIndex);
    std::cout << attackAndReleaseTime[0] << " | " << attackAndReleaseTime[1] << std::endl;
    std::cout << currentSignalSize <<std::endl;
};

void AmplitudeExtractor::averageFiltering(int order)
{
    std::vector<float> outputSignal = {};
    float sumBuffer = 0.0;
    for (int i = 0; i < (int)currentSignalPowerSeq.size(); i++)
    {
        if (i < order)
            sumBuffer += currentSignalPowerSeq[i];
        else
            sumBuffer = sumBuffer + currentSignalPowerSeq[i] - currentSignalPowerSeq[i-order];
        outputSignal[i] = sumBuffer / order;
    }
}


