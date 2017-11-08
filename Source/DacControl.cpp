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
    ADSR = {0.0, 0.0, 0.0, 0.0};
    
    outputFrequency = 86.1328f;
    startTimer(1000.0/outputFrequency);
    
    outputVoltageStatus = false;
    currentOutputVoltage = 0.0f;
    ADSRStatus = 0;
    currentStatusCnt = 1;
    
}

void DacControl::hiResTimerCallback()
{
//    float temp =
    ADSRVoltageOutput();
//    std::cout << buttonPlay << std::endl;
//    std::cout << temp << " | " << ADSRStatus << " | " << currentStatusCnt << " | " << (int)ADSR[0] << std::endl;
}

void DacControl::setADSRValues(std::vector<float> ADSRvals)
{
    ADSR = ADSRvals;
//    std::cout << ADSR[0] << " | " << ADSR[1] << " | " << ADSR[2] << " | " << ADSR[3] << std::endl;
}

void DacControl::setADSROutputVoltageStatus()
{
    buttonPlay = !buttonPlay;
    if (buttonPlay && !outputVoltageStatus)
        outputVoltageStatus = true;
    else if (buttonPlay && outputVoltageStatus)
    {
        outputVoltageStatus = false;
        outputVoltageStatus = true;
    }
    
    
}

float DacControl::ADSRVoltageOutput()
{
    if (outputVoltageStatus == false)
        currentOutputVoltage = 0.0f;
    else
    {
        if (ADSRStatus == 0)
            ADSRStatus++;
//            outputVoltageStatus = false;
        else if (ADSRStatus == 1)
        {
            if (currentStatusCnt == (int)ADSR[0] || (int)ADSR[0] == 0)
            {
                ADSRStatus++;
                currentStatusCnt = 1;
            }
            else
            {
                currentOutputVoltage += MaxVoltage / (int)ADSR[0];
                currentStatusCnt++;
            }
        }
        else if (ADSRStatus == 2)
        {
            if ((int)ADSR[1] == 0 && (int)ADSR[0] > 0)
            {
                currentOutputVoltage = 0.0f;
                ADSRStatus++;
                currentStatusCnt = 1;
            }
            if (currentStatusCnt == (int)ADSR[1] || (int)ADSR[1] == 0)
            {
                ADSRStatus++;
                currentStatusCnt = 1;
            }
            else
            {
                currentOutputVoltage -= MaxVoltage * (1 - ADSR[2]) / ADSR[1];
                currentStatusCnt++;
            }
        }
        else if (ADSRStatus == 3)
        {
            currentOutputVoltage = MaxVoltage * ADSR[2];
            if (buttonPlay == false)
                ADSRStatus++;
        }
        else if (ADSRStatus == 4)
        {
            if (currentStatusCnt == (int)ADSR[3]  || (int)ADSR[3] == 0)
            {
                ADSRStatus = 0;
                outputVoltageStatus = false;
                currentStatusCnt = 1;
            }
            else
            {
                currentOutputVoltage -= MaxVoltage * ADSR[2] / ADSR[3];
                currentStatusCnt++;
            }
        }
    }
    
    return currentOutputVoltage;
}




