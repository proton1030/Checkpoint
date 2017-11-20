//
//  ConvertAmpEnvToSig.cpp
//  checkpoint - App
//
//  Created by Hanyu Liu on 11/20/17.
//

//
//  ConvertEnvelopeToSig.cpp
//  checkpoint - App
//
//  Created by Hanyu Liu on 11/20/17.
//

#import <vector>
#import <iostream>
#import <algorithm>
#define MaxVoltage 5.0f
#include "SoundProfileStruct.h"

class ConvertAmpEnvToSig
{
public:
    ConvertAmpEnvToSig(SoundProfiles &profile)
    {
        ADSR = profile.ampADSR;
        envelope = profile.ampEnv;
        envMax = 0.0;
        currentState = 0;
        outputVoltage = 0.0f;
        stateCnt = 0;
        outputOrNot = 0;
        retriggerFlag = 0;
        
    };
    
    void update(SoundProfiles &profile)
    {
        ADSR = profile.ampADSR;
        envelope = profile.ampEnv;
        for (int i = 0; i < envelope.size(); i++)
            envMax = envelope[i] > envMax ? envelope[i] : envMax;
        for (int i = 0; i < envelope.size(); i++)
            envelope[i] *= MaxVoltage / envMax;
    }
    
    float process(bool buttonStatus)
    {
        if (buttonStatus && !outputOrNot)
        {
            currentState = 1;
            outputOrNot = 1;
        }
        else if (!buttonStatus && outputOrNot)
        {
            retriggerFlag = 1;
        }
        else if (buttonStatus && retriggerFlag)
        {
            currentState = 1;
            stateCnt = 0;
            outputVoltage = 0;
            outputOrNot = 1;
            retriggerFlag = 0;
        }
        switch(currentState) {
            case 0:
                outputVoltage = 0.0f;
                break;
            case 1:
                if (buttonStatus == false)
                {
                    currentState = 3;
                    stateCnt = 0;
                }
                else if (buttonStatus == true && (int)ADSR[0] == 0)
                {
                    currentState = 2;
                    stateCnt = 0;
                }
                else if (buttonStatus == true && stateCnt < (int)ADSR[0])
                {
                    outputVoltage = envelope[stateCnt];
                    stateCnt ++;
                }
                else{
                    currentState = 2;
                    stateCnt = 0;
                }
                break;
            case 2:
                if (buttonStatus == false)
                {
                    currentState = 3;
                    stateCnt = 0;
                }
                else
                {
                    outputVoltage = envelope[(int)ADSR[0]+stateCnt];
                    stateCnt ++;
                    if (stateCnt + (int)ADSR[0] == envelope.size()-(int)ADSR[3]-1)
                        stateCnt = 0;
                }
                break;
            case 3:
                if ((int)ADSR[2] == 0 || (int)ADSR[3] == 0 || stateCnt >= (int)ADSR[3])
                {
                    outputVoltage = 0.0f;
                    currentState = 0;
                    stateCnt = 0;
                }
                else
                {
                    outputVoltage = envelope[envelope.size()-(int)ADSR[3]-1+stateCnt];
                    stateCnt++;
                }
                break;
        }
        return outputVoltage;
    }
    
    
private:
    std::vector<float> ADSR;
    std::vector<float> envelope;
    int currentState, stateCnt;
    float outputVoltage, envMax;
    int envelopeType; // 1 = ampEnv, 2 = specEnv
    int outputOrNot, retriggerFlag;
    
};

