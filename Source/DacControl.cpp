//
//  DacControl.cpp
//  checkpoint - App
//
//  Created by Hanyu Liu on 10/25/17.
//
#include "../JuceLibraryCode/JuceHeader.h"
#include "DacControl.h"
#include <iostream>

DacControl::DacControl(const ScopedPointer<AmplitudeConfigTab>& amplitude)
{
    amplitudeTabContents = amplitude;
    amplitudeTabContents->addChangeListener(this);
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
    
//   std::cout << ADSR[0] << " | " << ADSR[1] << " | "<< ADSR[2] << " | "<< ADSR[3] << std::endl;
    float temp = ADSRVoltageOutput();
    std::cout << temp << " | " << ADSRStatus << " | " << currentStatusCnt << " | " << (int)ADSR[0] << std::endl;
//    test++;
}

void DacControl::changeListenerCallback(ChangeBroadcaster* source)
{
    ADSR = amplitudeTabContents->getADSRSettings();
    buttonPlay = amplitudeTabContents->getTriggerButton();
    if (buttonPlay == true)
    {
        outputVoltageStatus = true;
    }
//    std::cout << buttonPlay << std::endl;
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
            if (currentStatusCnt == (int)ADSR[0])
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
            if (currentStatusCnt == (int)ADSR[1])
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
            if (buttonPlay == false)
                ADSRStatus++;
        }
        else if (ADSRStatus == 4)
        {
            if (currentStatusCnt == (int)ADSR[3])
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




