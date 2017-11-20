//
//  ConvertEnvelopeToSig.cpp
//  checkpoint - App
//
//  Created by Hanyu Liu on 11/20/17.
//

#import <vector>
#import <iostream>
#define MaxVoltage 5.0f
#include "SoundProfileStruct.h"

class ConvertEnvelopeToSig
{
public:
    ConvertEnvelopeToSig(SoundProfiles &profile, int envelopeType)
    {
        ADSR = profile.ampADSR;
        if (envelopeType == 1)
            envelope = profile.ampEnv;
        else
            envelope = profile.specCutoffEnv;
        currentState = 0;
        outputVoltage = 0.0f;
        stateCnt = 0;
        
    };
    
//    void setADSRValues(std::vector<float> ADSRInput)
//    {
//        ADSR = ADSRInput;
//    }
//    
//    void setEnvelope(std::vector<float> EnvInput)
//    {
//        envelope = EnvInput;
//    }
    
    float process(bool buttonStatus)
    {
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
    float outputVoltage;
    int envelopeType; // 1 = ampEnv, 2 = specEnv
    
};
