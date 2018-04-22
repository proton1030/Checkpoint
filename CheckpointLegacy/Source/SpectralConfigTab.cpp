//
//  SpectralConfigTab.cpp
//  checkpoint - App
//
//  Created by Hanyu Liu on 11/20/17.
//

#include "../JuceLibraryCode/JuceHeader.h"
#include <iostream>
#include <vector>
#include "SpectralConfigTab.h"

SpectralConfigTab::SpectralConfigTab()
{
    currentOutputMode = 1;
    
    addAndMakeVisible (cutoff = new Slider);
    cutoff->setRange (0, 10, 0);
    cutoff->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    cutoff->setTextBoxStyle (Slider::TextBoxBelow, false, 40, 20);
    cutoff->setRange(20.0, 20000.0, 1);
    cutoff->setSkewFactorFromMidPoint(1000.0);
    cutoff->setColour(Slider::ColourIds::thumbColourId, Colours::darkgrey);
    cutoff->addListener (this);
    
    addAndMakeVisible(cutoffLabel = new Label);
    cutoffLabel->setText("Cutoff Freq", dontSendNotification);
    
    addAndMakeVisible (switchMode = new TextButton ("Cutoff Env"));
    switchMode->addListener (this);
    
}

SpectralConfigTab::~SpectralConfigTab()
{
    
}

void SpectralConfigTab::paint (Graphics& g)
{
    
}

void SpectralConfigTab::resized()
{
    cutoff->setBounds(proportionOfWidth (0.0100f), proportionOfHeight (0.1500f), proportionOfWidth (0.3000f), proportionOfHeight (0.3500f));
    cutoffLabel->setBounds(proportionOfWidth (0.0300f), proportionOfHeight (0.0500f), proportionOfWidth (0.4000f), proportionOfHeight (0.1000f));
    switchMode->setBounds(proportionOfWidth (0.8200f), proportionOfHeight (0.1000f), proportionOfWidth (0.1500f), proportionOfHeight (0.1100f));
}

void SpectralConfigTab::sliderValueChanged(Slider* slider)
{
    
}

void SpectralConfigTab::buttonClicked (Button* buttonThatWasClicked)
{
    if (buttonThatWasClicked == switchMode)
    {
        if (currentOutputMode == 2)
        {
            switchMode->setButtonText (TRANS("Cutoff Env"));
            currentOutputMode = 1;
            cutoff->setColour(Slider::ColourIds::thumbColourId, Colours::darkgrey);
        }
        else if (currentOutputMode == 1)
        {
            switchMode->setButtonText (TRANS("Override"));
            currentOutputMode = 2;
            cutoff->setColour(Slider::ColourIds::thumbColourId, Colours::lightgrey);
        }
    }
}
