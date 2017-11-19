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
//    addAndMakeVisible (xaxis = new Slider ("new slider"));
//    xaxis->setRange (0, 10, 0);
//    xaxis->setSliderStyle (Slider::LinearHorizontal);
//    xaxis->setTextBoxStyle (Slider::TextBoxRight, false, 40, 20);
//    xaxis->addListener (this);
//
//    addAndMakeVisible (yaxis = new Slider ("new slider"));
//    yaxis->setRange (0, 10, 0);
//    yaxis->setSliderStyle (Slider::LinearVertical);
//    yaxis->setTextBoxStyle (Slider::TextBoxAbove, false, 40, 20);
//    yaxis->addListener (this);
    
    lastMousePosition.x = 80.0f;
    lastMousePosition.y = 80.0f;
    dragKnobBoundries = {10.0f,200.0f,10.0f,200.0f};
    circleRadius = 6.0f;

}

PresetTab::~PresetTab()
{
    this->removeAllChangeListeners();
//    xaxis = nullptr;
//    yaxis = nullptr;
}

void PresetTab::paint (Graphics& g)
{
//    g.fillAll (Colour (0xff323e44));
    g.fillAll (Colours::black);
    g.setColour (Colours::white);
    g.fillEllipse  (lastMousePosition.x - circleRadius*2, lastMousePosition.y - circleRadius*2, circleRadius*2, circleRadius*2);
    g.drawRect(dragKnobBoundries[0],dragKnobBoundries[2],dragKnobBoundries[1]-dragKnobBoundries[0],dragKnobBoundries[3]-dragKnobBoundries[2],0.5f);
}

void PresetTab::resized()
{
//    xaxis->setBounds (proportionOfWidth (0.0500f), proportionOfHeight (0.1000f), proportionOfWidth (0.1500f), proportionOfHeight (0.8000f));
//    yaxis->setBounds (proportionOfWidth (0.2000f), proportionOfHeight (0.1000f), proportionOfWidth (0.1500f), proportionOfHeight (0.8000f));
    
//    xaxis->setBounds (40, 170, 200, 40);
//    yaxis->setBounds (10, 10, 40, 170);
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
    
    
    repaint();
}

void PresetTab::mouseUp (const MouseEvent&)
{
    repaint();
}

