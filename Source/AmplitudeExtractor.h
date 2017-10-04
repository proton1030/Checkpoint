//
//  AmplitudeExtractor.h
//  recordingtest
//
//  Created by Hanyu Liu on 10/3/17.
//

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include <vector>

class AmplitudeExtractor
{
public:
    AmplitudeExtractor(
                 const int& systemBufferSize,
                 const double& sampleRate );
    
    ~AmplitudeExtractor() {}
    
    void initialize();
    void clear();
    
    void process(const float* currentBlockPtr);
    void backgroundPowerEstimation(float blockPower);
    void calculateADSR();

    
    
    
private:
    //System-acquired parameters
    int systemBufferSize;
    double sampleRate;
    
    //Internal variables
    float currentBlockPower;
    int earlyBlockNums;
    float backgroundPower;
    bool currentlyInSignalFlag;
    std::vector<float> currentSignalPowerSeq;
    float currentSignalPowerSum;
    int currentSignalDuration;
    std::vector<float> attackAndReleaseTime;

    //Tweak setting variables
    int backgroundEstimationBlockNumThres;
    int signalDetectionThres;
    int signalDurationThres;
    

    


    
};
