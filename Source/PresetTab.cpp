//
//  PresetTab.cpp
//  checkpoint - App
//
//  Created by Hanyu Liu on 11/14/17.
//

#include "PresetTab.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include <iostream>

PresetTab::PresetTab()
{
    addAndMakeVisible (switchMode = new TextButton ("ADSR Sig"));
    switchMode->addListener (this);
    
    lastMousePosition.x = 80.0f;
    lastMousePosition.y = 80.0f;
    dragKnobBoundries = {20,430,20,430};
    circleRadius = 10.0f;
    currentOutputMode = 1;
    
    setBufferedToImage(true);

}

PresetTab::~PresetTab()
{
    this->removeAllChangeListeners();
}

void PresetTab::paint (Graphics& g)
{
    g.fillAll (Colours::black);
    g.setColour (Colours::white);
    g.fillEllipse  (lastMousePosition.x - circleRadius*2, lastMousePosition.y - circleRadius*2, circleRadius*2, circleRadius*2);
    g.drawRect(float(dragKnobBoundries[0]),float(dragKnobBoundries[2]),float(dragKnobBoundries[1]-dragKnobBoundries[0]),float(dragKnobBoundries[3]-dragKnobBoundries[2]),0.5f);
    
}

void PresetTab::resized()
{

    switchMode->setBounds(proportionOfWidth (0.8200f), proportionOfHeight (0.1000f), proportionOfWidth (0.1500f), proportionOfHeight (0.1100f));
}


//Mouse manipulation

void PresetTab::mouseDown (const MouseEvent& e)
{
    mouseDrag (e);
}

void PresetTab::mouseDrag (const MouseEvent& e)
{
    
    if ((e.position.x >= dragKnobBoundries[0]+circleRadius) && (e.position.x <= dragKnobBoundries[1]-circleRadius))
    {
        lastMousePosition.x = e.position.x + circleRadius;
    }
    if ((e.position.y >= dragKnobBoundries[2]+circleRadius) && (e.position.y <= dragKnobBoundries[3]-circleRadius))
    {
        lastMousePosition.y = e.position.y + circleRadius;
    }
    
    repaint(dragKnobBoundries[0],dragKnobBoundries[2],dragKnobBoundries[1]-dragKnobBoundries[0],dragKnobBoundries[3]-dragKnobBoundries[2]);
}

void PresetTab::mouseUp (const MouseEvent&)
{
    repaint(dragKnobBoundries[0],dragKnobBoundries[2],dragKnobBoundries[1]-dragKnobBoundries[0],dragKnobBoundries[3]-dragKnobBoundries[2]);
}

void PresetTab::buttonClicked (Button* buttonThatWasClicked)
{
    if (buttonThatWasClicked == switchMode)
    {
        if (currentOutputMode == 3)
        {
            switchMode->setButtonText (TRANS("ADSR Sig"));
            currentOutputMode = 1;
        }
        else if (currentOutputMode == 1)
        {
            switchMode->setButtonText (TRANS("Amp Env Sig"));
            currentOutputMode = 2;
        }
        else if (currentOutputMode == 2)
        {
            switchMode->setButtonText (TRANS("Spec Env Sig"));
            currentOutputMode = 3;
        }
    }
}

