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
#include <algorithm>
#include <math.h>
#include <cmath>

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
    ADSRTime = {0.0f , 0.0f, 0.0f, 0.0f};
    ADSRCache = {};
    finalADSR = {0.0f , 0.0f, 0.0f, 0.0f};
    currentCacheSize.setValue(0);
    currentCacheSize.addListener(this);

    //Initialization settings
    backgroundEstimationBlockNumThres = 20;
    signalDetectionThres = 40;
    signalDurationThres = 20;
    averagingOrder = 4;
    leastSearchSustainBlkLength = 50; //Release begin time search stops if maxval didn't get updated after searching this much blks.
    minAtkRelDist = 10; // Minimum sustain length and consider the envelope only consists of attack and decay if sustain<minAtkRelDist.
    sustainDiffRatio = 0.01;
    
//    finalADSR.addListener(this);

};
void AmplitudeExtractor::clear()
{
    initialize();
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
                finalADSR = getAverageADSRCache();
                currentCacheSize.setValue((int)currentCacheSize.getValue() + 1);
//                std::cout<<temp[0]<<"|"<<temp[1]<<"|"<<temp[2]<<"|"<<temp[3]<<std::endl;
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
    int releaseSearchNum = 0;

    averageFiltering(averagingOrder, currentSignalSize);

    for (int i = 1; i < currentSignalSize; i++) 
    {
        attackEndDetection = currentSignalPowerSeq[i] * (currentSignalSize - i/2);
        if (attackEndDetection > attackDetectionMax)
        {
            attackDetectionMax = attackEndDetection;
            attackDetectionMaxIndex = i;
        }
        if (releaseSearchNum < leastSearchSustainBlkLength)
        {
            releaseBeginDetection = currentSignalPowerSeq[currentSignalSize-i] * (currentSignalSize - i/2);
            if (releaseBeginDetection > releaseDetectionMax)
            {
                releaseDetectionMax = releaseBeginDetection;
                releaseDetectionMaxIndex = i;
                releaseSearchNum = 0;
            }
            else
                releaseSearchNum ++;
        }
    }

    if (currentSignalSize - (attackDetectionMaxIndex + releaseDetectionMaxIndex) > minAtkRelDist)
    {
        float maxSustainPower = 0.0f;
        float minSustainPower = 0.0f;
        for (int i = 0; i < minAtkRelDist/2; i++)
        {
            maxSustainPower += currentSignalPowerSeq[attackDetectionMaxIndex + i];
            minSustainPower += currentSignalPowerSeq[currentSignalSize - releaseDetectionMaxIndex - i - 1];
        }
        maxSustainPower /= minAtkRelDist/2;
        minSustainPower /= minAtkRelDist/2;
        if (std::abs(maxSustainPower-minSustainPower) / maxSustainPower > sustainDiffRatio)
        {
            ADSRTime[0] = (attackDetectionMaxIndex);
            ADSRTime[1] = 1.0;
            ADSRTime[2] = 1.0;
            ADSRTime[2] = 1.0;
            ADSRTime[3] = (releaseDetectionMaxIndex);   
        }
        else
        {
            ADSRTime[0] = (attackDetectionMaxIndex);
            ADSRTime[1] = 1.0;
            ADSRTime[2] = 1.0;
            ADSRTime[3] = (releaseDetectionMaxIndex);
        }
    }
    else
    {
        ADSRTime[0] = (attackDetectionMaxIndex);
        ADSRTime[1] = (releaseDetectionMaxIndex);
    }
    ADSRCache.push_back(ADSRTime);

    // * systemBufferSize / sampleRate
};

void AmplitudeExtractor::averageFiltering(int order, int signalSize)
{
    
    std::vector<float> outputSignal;
    float sumBuffer = 0.0;
    for (int i = 0; i < signalSize; i++)
    { 
        if (i < order)
            sumBuffer += currentSignalPowerSeq[i];
        else
            sumBuffer = sumBuffer + currentSignalPowerSeq[i] - currentSignalPowerSeq[i-order];
        outputSignal.push_back(sumBuffer/order);
    }
    for (int i = 0; i < signalSize; i++)
        currentSignalPowerSeq[i] = outputSignal[i];
}

std::vector<float> AmplitudeExtractor::getAverageADSRCache()
{
    std::vector<float> returnedADSR = {0.0f, 0.0f, 0.0f, 0.0f};
    int ADSRCacheSize = (int)ADSRCache.size();
    if (ADSRCacheSize > 0)
    {
        for (int i=0;i<4;i++)
        {
            float tempSum = 0.0f;
            for (int k=0;k<ADSRCacheSize;k++)
            {
                tempSum += ADSRCache[k][i];
            }
            returnedADSR[i] = tempSum/ADSRCacheSize;
        }
    }
    return returnedADSR;
}

std::vector<float>& AmplitudeExtractor::getFinalADSR()
{
    return finalADSR;
}

void AmplitudeExtractor::valueChanged (Value& value)
{
    this->sendChangeMessage();
//    std::cout<<(int)currentCacheSize.getValue()<<std::endl;
}


