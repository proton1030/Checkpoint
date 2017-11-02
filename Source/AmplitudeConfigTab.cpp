//
//  AmplitudeConfigTab.cpp
//  checkpoint - App
//
//  Created by Hanyu Liu on 10/24/17.
//

#include "../JuceLibraryCode/JuceHeader.h"
#include <iostream>
#include "AmplitudeConfigTab.h"
#include <math.h>


AmplitudeConfigTab::AmplitudeConfigTab(const ScopedPointer<TrainingTab>& trainingtab)
{
    
    addAndMakeVisible(attack);
    attack.setSliderStyle (Slider::LinearVertical);
    attack.setTextBoxStyle (Slider::TextBoxBelow, false, 100, 20);
    attack.setRange(0.0, 100.0, 0.1);
    attack.addListener(this);

    addAndMakeVisible(decay);
    decay.setSliderStyle (Slider::LinearVertical);
    decay.setTextBoxStyle (Slider::TextBoxBelow, false, 100, 20);
    decay.setRange(0.0, 100.0, 0.1);
    decay.addListener(this);

    addAndMakeVisible(sustain);
    sustain.setSliderStyle (Slider::LinearVertical);
    sustain.setTextBoxStyle (Slider::TextBoxBelow, false, 100, 20);
    sustain.setRange(0.0, 1.0, 0.1);
    sustain.addListener(this);

    addAndMakeVisible(release);
    release.setSliderStyle (Slider::LinearVertical);
    release.setTextBoxStyle (Slider::TextBoxBelow, false, 100, 20);
    release.setRange(0.0, 100.0, 0.1);
    release.addListener(this);
    
    addAndMakeVisible (switchMode = new TextButton ("new button"));
    switchMode->setButtonText (TRANS("ADSR"));
    switchMode->addListener (this);
    
    trainingTabContents = trainingtab;
    trainingTabContents->addChangeListener(this);
    currentSliderValues = {0.0, 0.0, 0.0, 0.0};
    currentTriggerStatus = false;
    
    currentOutputMode = true;
    
}


void AmplitudeConfigTab::paint (Graphics& g)
{
    g.fillAll (Colour (0xff323e44));
}

//std::vector<float>& AmplitudeConfigTab::fetchADSRValues(std::vector<float>& AverageADSR)
//{
//    return AverageADSR;
//}

void AmplitudeConfigTab::resized()
{
    attack.setBounds (proportionOfWidth (0.0500f), proportionOfHeight (0.1000f), proportionOfWidth (0.1500f), proportionOfHeight (0.8000f));
    decay.setBounds (proportionOfWidth (0.2000f), proportionOfHeight (0.1000f), proportionOfWidth (0.1500f), proportionOfHeight (0.8000f));
    sustain.setBounds (proportionOfWidth (0.3500f), proportionOfHeight (0.1000f), proportionOfWidth (0.1500f), proportionOfHeight (0.8000f));
    release.setBounds (proportionOfWidth (0.5000f), proportionOfHeight (0.1000f), proportionOfWidth (0.1500f), proportionOfHeight (0.8000f));
    switchMode->setBounds(proportionOfWidth (0.6800f), proportionOfHeight (0.1000f), proportionOfWidth (0.2500f), proportionOfHeight (0.1500f));
}

void AmplitudeConfigTab::sliderValueChanged(Slider* slider)
{
    currentSliderValues[0] = attack.getValue();
    currentSliderValues[1] = decay.getValue();
    currentSliderValues[2] = sustain.getValue();
    currentSliderValues[3] = release.getValue();
    sendChangeMessage();
//    std::cout << attack.getValue() << std::endl;
//    dispADSRValues(trainingTabContents->getADSRValues());
}

void AmplitudeConfigTab::setSliderValues(std::vector<float>& sliderValues)
{
    attack.setValue(round(sliderValues[0]));
    decay.setValue(round(sliderValues[1]));
    sustain.setValue(round(sliderValues[2]));
    release.setValue(round(sliderValues[3]));
}

void AmplitudeConfigTab::changeListenerCallback(ChangeBroadcaster* source)
{
    if (currentOutputMode == false)
    {
        currentSliderValues = trainingTabContents->getADSRValues();
        currentTriggerStatus = trainingTabContents->getTriggerButtonStatus();
        setSliderValues(currentSliderValues);
    }
    else
    {
        
    }
    sendChangeMessage();
}

std::vector<float> AmplitudeConfigTab::getADSRSettings()
{
    return currentSliderValues;
}

bool AmplitudeConfigTab::getTriggerButton()
{
    return trainingTabContents->getTriggerButtonStatus();
}

void AmplitudeConfigTab::buttonClicked (Button* buttonThatWasClicked)
{
    if (buttonThatWasClicked == switchMode)
    {
        if (currentOutputMode == true)
        {
            switchMode->setButtonText (TRANS("Training"));
            currentOutputMode = false;
        }
        else
        {
            switchMode->setButtonText (TRANS("ADSR"));
            currentOutputMode = true;
        }
        //[UserButtonCode_textButton] -- add your button handler code here..
        //[/UserButtonCode_textButton]
    }
}

