//
//  AmplitudeExtractor.h
//  recordingtest
//
//  Created by Hanyu Liu on 10/3/17.
//

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include <vector>

class AmplitudeExtractor : public Component, public ChangeBroadcaster, public Value::Listener
{
public:
    AmplitudeExtractor(
                 const int& systemBufferSize,
                 const double& sampleRate );
    ~AmplitudeExtractor() {}
    
    void initialize();
    void clear();
    
    int process(const float* currentBlockPtr);
    void backgroundPowerEstimation(float blockPower);
    void averageFiltering(int order, int signalSize);
    std::vector<float> getAverageADSRCache();
    std::vector<float>& getFinalADSR();
    void finalADSRSet(std::vector<float> ADSR);

    
    
    
private:
    void calculateADSR();
    void valueChanged (Value& value);
    
    //System-acquired parameters
    int systemBufferSize;
    double sampleRate;
    
    //Internal variables
    float currentBlockPower;
    float currentBlockRMS;
    int earlyBlockNums;
    float backgroundPower;
    bool currentlyInSignalFlag;
    std::vector<float> currentSignalPowerSeq;
    float currentSignalPowerSum;
    int currentSignalDuration;
    std::vector<float> ADSRTime;
    std::vector<float> finalADSR;
    std::vector<std::vector<float>> ADSRCache;
    int averagingOrder;
    Value currentCacheSize;

    //Tweak setting variables
    int backgroundEstimationBlockNumThres;
    int signalDetectionThres;
    int signalDurationThres;
    int leastSearchSustainBlkLength;
    int minAtkRelDist;
    float sustainDiffRatio;
    

    


    
};
