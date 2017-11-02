
#include <iostream>
#include "GUI.h"
#include "MainUITabs.h"

GUI::GUI ()
{
    setOpaque (true);
    addAndMakeVisible (tabs);
    setSize (320, 240);
    customLook.setColour (TabbedComponent::outlineColourId, Colours::transparentBlack);
    tabs.setLookAndFeel (&customLook);
}

GUI::~GUI ()
{
}

void GUI::paint (Graphics& g)
{
//    g.fillAll (setUIColour (LookAndFeel_V4::ColourScheme::UIColour::windowBackground));
    g.fillAll (Colour (0xff323e44));
    
    
}

void GUI::resized()
{
//    Rectangle<int> area (getLocalBounds());
//    tabs.setBounds (getLocalBounds().reduced (4));
    tabs.setBoundsInset (BorderSize<int> (0));
//    tabs.setBounds (getLocalBounds().reduced (4));
    
}


