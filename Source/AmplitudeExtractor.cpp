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
#include <thread>

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
    signalDetectionThres = 800;
    signalDurationThres = 20;
    
    
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

    // std::cout << currentlyInSignalFlag << "  |  " << currentBlockPower << "  |  " << currentSignalDuration << std::endl;

    if (earlyBlockNums <= backgroundEstimationBlockNumThres) {
        backgroundPowerEstimation(currentBlockPower);
        return 0;
    }
    else{
        if (!currentlyInSignalFlag && currentBlockPower >= signalDetectionThres * backgroundPower ) 
        {
            currentlyInSignalFlag = true;
            currentSignalPowerSeq.push_back(currentBlockPower);
            currentSignalDuration += 1;
            currentSignalPowerSum += currentBlockPower;
            return 1;
            
    
        }
        else if (currentlyInSignalFlag && currentBlockPower >= signalDetectionThres * backgroundPower)
        {
            currentSignalDuration += 1;
            currentSignalPowerSeq.push_back(currentBlockPower);
            currentSignalPowerSum += currentBlockPower;
            return 1;
        }
        else if (currentlyInSignalFlag && currentBlockPower < signalDetectionThres * backgroundPower)
        {
            if (currentSignalDuration >= signalDurationThres)
            {
                currentlyInSignalFlag = false;
                std::thread t(&AmplitudeExtractor::calculateADSR, this);
                t.detach();
                std::cout << "test" <<std::endl;
                // calculateADSR();
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

    int currentSignalSize = (int)currentSignalPowerSeq.size();
    float attackEndDetection[currentSignalSize];
    float releaseBeginDetection[currentSignalSize];
    attackEndDetection[0] = releaseBeginDetection[currentSignalSize-1] = 0;
    float tempAttackSum = currentSignalPowerSum-currentSignalPowerSeq[0], tempReleaseSum = currentSignalPowerSum-currentSignalPowerSeq[currentSignalSize];
    float attackDetectionMax = 0.0f, releaseDetectionMax = 0.0f;
    int attackDetectionMaxIndex = 0, releaseDetectionMaxIndex = 0;

    for (int i = 1; i <= currentSignalSize-1; i++) 
    {
        for (int j = 1;j <= i;j++)
        {
            attackEndDetection[i] += j;
            currentSignalPowerSeq[j] * currentSignalPowerSeq[i] * pow(100.0 , j / i) / 100.0;
            releaseBeginDetection[i] += currentSignalPowerSeq[currentSignalSize-1-j] * (j / i); 
        }
        attackEndDetection[i] += currentSignalPowerSeq[i] * (tempAttackSum -= currentSignalPowerSeq[i]);
        releaseBeginDetection[i] += currentSignalPowerSeq[currentSignalSize-1-i] * (tempReleaseSum -= currentSignalPowerSeq[currentSignalSize-1-i]);
        if (attackEndDetection[i] > attackDetectionMax)
        {
            attackDetectionMax = attackEndDetection[i];
            attackDetectionMaxIndex = i;
        }
        if (releaseBeginDetection[i] > releaseDetectionMax)
        {
            releaseDetectionMax = releaseBeginDetection[i];
            releaseDetectionMaxIndex = i;
        }
        const MessageManagerLock mml (Thread::getCurrentThread());
        if (! mml.lockWasGained())  // if something is trying to kill this job, the lock
            return; 
    }

    attackAndReleaseTime[0] = (attackDetectionMaxIndex+1) * systemBufferSize / sampleRate;
    attackAndReleaseTime[1] = (currentSignalSize - releaseDetectionMaxIndex) * systemBufferSize / sampleRate;
    std::cout << attackAndReleaseTime[0] << " | " << attackAndReleaseTime[1] << std::endl;
    std::cout << currentSignalSize <<std::endl;

};


