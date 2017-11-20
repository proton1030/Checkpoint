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


AmplitudeConfigTab::AmplitudeConfigTab()
{
    
    addAndMakeVisible(attack = new Slider ("Attack"));
    attack->setSliderStyle (Slider::LinearVertical);
    attack->setTextBoxStyle (Slider::TextBoxBelow, false, 100, 20);
    attack->setRange(0.0, 300.0, 0.1);
    attack->addListener(this);

    addAndMakeVisible(decay = new Slider ("Decay"));
    decay->setSliderStyle (Slider::LinearVertical);
    decay->setTextBoxStyle (Slider::TextBoxBelow, false, 100, 20);
    decay->setRange(0.0, 100.0, 0.1);
    decay->addListener(this);

    addAndMakeVisible(sustain = new Slider ("Sustain"));
    sustain->setSliderStyle (Slider::LinearVertical);
    sustain->setTextBoxStyle (Slider::TextBoxBelow, false, 100, 20);
    sustain->setRange(0.0, 1.0, 0.1);
    sustain->addListener(this);

    addAndMakeVisible(release = new Slider ("Release"));
    release->setSliderStyle (Slider::LinearVertical);
    release->setTextBoxStyle (Slider::TextBoxBelow, false, 100, 20);
    release->setRange(0.0, 300.0, 0.1);
    release->addListener(this);
    
    addAndMakeVisible (switchMode = new TextButton ("Training"));
    switchMode->addListener (this);
    
    currentSliderValues = {0.0, 0.0, 0.0, 0.0};
    currentTriggerStatus = false;
    
    currentOutputMode = 1;
    
    attack->setColour(Slider::ColourIds::thumbColourId, Colours::darkgrey);
    decay->setColour(Slider::ColourIds::thumbColourId, Colours::darkgrey);
    sustain->setColour(Slider::ColourIds::thumbColourId, Colours::darkgrey);
    release->setColour(Slider::ColourIds::thumbColourId, Colours::darkgrey);
}

AmplitudeConfigTab::~AmplitudeConfigTab()
{
    attack = nullptr;
    decay = nullptr;
    sustain = nullptr;
    release = nullptr;
}

void AmplitudeConfigTab::paint (Graphics& g)
{
    g.fillAll (Colours::black);
}

void AmplitudeConfigTab::resized()
{
    attack->setBounds (proportionOfWidth (0.0300f), proportionOfHeight (0.1000f), proportionOfWidth (0.1500f), proportionOfHeight (0.8000f));
    decay->setBounds (proportionOfWidth (0.1500f), proportionOfHeight (0.1000f), proportionOfWidth (0.1500f), proportionOfHeight (0.8000f));
    sustain->setBounds (proportionOfWidth (0.2700f), proportionOfHeight (0.1000f), proportionOfWidth (0.1500f), proportionOfHeight (0.8000f));
    release->setBounds (proportionOfWidth (0.3900f), proportionOfHeight (0.1000f), proportionOfWidth (0.1500f), proportionOfHeight (0.8000f));
    switchMode->setBounds(proportionOfWidth (0.6800f), proportionOfHeight (0.1000f), proportionOfWidth (0.2500f), proportionOfHeight (0.1500f));
}

void AmplitudeConfigTab::sliderValueChanged(Slider* slider)
{
    if (currentOutputMode == 2)
    {
        currentSliderValues[0] = attack->getValue();
        currentSliderValues[1] = decay->getValue();
        currentSliderValues[2] = sustain->getValue();
        currentSliderValues[3] = release->getValue();
        sendChangeMessage();
    }
    else
    {
        attack->setValue(currentSliderValues[0]);
        decay->setValue(currentSliderValues[1]);
        sustain->setValue(currentSliderValues[2]);
        release->setValue(currentSliderValues[3]);
    }
}

void AmplitudeConfigTab::setSliderValues(std::vector<float> sliderValues)
{
    currentSliderValues = sliderValues;
    attack->setValue(round(sliderValues[0]));
    decay->setValue(round(sliderValues[1]));
    sustain->setValue(sliderValues[2]);
    release->setValue(round(sliderValues[3]));
}

std::vector<float> AmplitudeConfigTab::getADSRValues()
{
    return currentSliderValues;
}

void AmplitudeConfigTab::buttonClicked (Button* buttonThatWasClicked)
{
    if (buttonThatWasClicked == switchMode)
    {
        if (currentOutputMode == 3)
        {
            switchMode->setButtonText (TRANS("Training"));
            currentOutputMode = 1;
            attack->setColour(Slider::ColourIds::thumbColourId, Colours::darkgrey);
            decay->setColour(Slider::ColourIds::thumbColourId, Colours::darkgrey);
            sustain->setColour(Slider::ColourIds::thumbColourId, Colours::darkgrey);
            release->setColour(Slider::ColourIds::thumbColourId, Colours::darkgrey);
        }
        else if (currentOutputMode == 1)
        {
            switchMode->setButtonText (TRANS("Override"));
            currentOutputMode = 2;
            attack->setColour(Slider::ColourIds::thumbColourId, Colours::lightgrey);
            decay->setColour(Slider::ColourIds::thumbColourId, Colours::lightgrey);
            sustain->setColour(Slider::ColourIds::thumbColourId, Colours::lightgrey);
            release->setColour(Slider::ColourIds::thumbColourId, Colours::lightgrey);
        }
        else if (currentOutputMode == 2)
        {
            switchMode->setButtonText (TRANS("Envelope"));
            currentOutputMode = 3;
            attack->setColour(Slider::ColourIds::thumbColourId, Colours::darkgrey);
            decay->setColour(Slider::ColourIds::thumbColourId, Colours::darkgrey);
            sustain->setColour(Slider::ColourIds::thumbColourId, Colours::darkgrey);
            release->setColour(Slider::ColourIds::thumbColourId, Colours::darkgrey);
        }
        //[UserButtonCode_textButton] -- add your button handler code here..
        //[/UserButtonCode_textButton]
//        std::cout << "ampConfigTab:" << (int)currentOutputMode.getValue() << std::endl;
    }
}



