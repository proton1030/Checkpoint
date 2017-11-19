//
//  ConvertADSRToSig.cpp
//  checkpoint - App
//
//  Created by Hanyu Liu on 11/14/17.
//
#import <vector>
#import <iostream>
#define MaxVoltage 5.0f

class ConvertADSRToSig
{
public:
    ConvertADSRToSig()
    {
        ADSR = {0.0f, 0.0f, 0.0f, 0.0f};
        currentState = 0;
        outputVoltage = 0.0f;
        stateCnt = 0;
        outputOrNot = 0;
        retriggerFlag = 0;
        
    };
    
    void setADSRValues(std::vector<float> ADSRInput)
    {
        ADSR = ADSRInput;
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
                    currentState = 4;
                    stateCnt = 0;
                }
                else if (buttonStatus == true && (int)ADSR[0] == 0)
                {
                    outputVoltage = MaxVoltage;
                    currentState = 2;
                    stateCnt = 0;
                }
                else if (buttonStatus == true && stateCnt < (int)ADSR[0])
                {
                    outputVoltage += MaxVoltage / (int)ADSR[0];
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
                    currentState = 4;
                    stateCnt = 0;
                }
                else if (buttonStatus == true && (int)ADSR[1] == 0)
                {
                    currentState = 3;
                    stateCnt = 0;
                }
                else if (buttonStatus == true && stateCnt < (int)ADSR[1])
                {
                    outputVoltage -= MaxVoltage * (1 - ADSR[2]) / ADSR[1];
                    stateCnt ++;
                }
                else{
                    currentState = 3;
                    stateCnt = 0;
                }
                break;
            case 3:
                if (buttonStatus == false)
                {
                    currentState = 4;
                    stateCnt = 0;
                }
                else
                {
                    outputVoltage = MaxVoltage * ADSR[2];
                }
                break;
            case 4:
                if ((int)ADSR[2] == 0 || (int)ADSR[3] == 0 || stateCnt >= (int)ADSR[3])
                {
                    outputVoltage = 0.0f;
                    currentState = 0;
                    outputOrNot = 0;
                    stateCnt = 0;
                    retriggerFlag = 0;
                }
                else
                {
                    outputVoltage -= MaxVoltage * ADSR[2] / ADSR[3];
                    stateCnt++;
                }
                break;
            default:
                break;
        }
        return outputVoltage;
    };
    
private:
    bool outputOrNot;
    bool retriggerFlag;
    int currentState;
    float outputVoltage;
    int stateCnt;
    std::vector<float> ADSR;
    
};
