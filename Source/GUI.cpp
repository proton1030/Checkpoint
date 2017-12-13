
#include <iostream>
#include "GUI.h"


GUI::GUI ()
{
    customLook.setColour(TabbedComponent::outlineColourId, Colours::transparentBlack);
    customLook.setColour(TextButton::ColourIds::buttonColourId, Colours::black);
    
    customLook.setDefaultSansSerifTypefaceName("Times New Roman");

    customLook.setColour(Slider::backgroundColourId, Colour(0xff222222));
    customLook.setColour(Slider::thumbColourId, Colours::lightgrey);
    customLook.setColour(Slider::ColourIds::trackColourId, Colours::darkgrey);
    customLook.setColour(Slider::ColourIds::textBoxOutlineColourId, Colour(0x00222222));
    customLook.setColour(Slider::ColourIds::textBoxTextColourId, Colours::lightgrey);
    customLook.setColour(Slider::textBoxOutlineColourId, Colours::white);
    
    addAndMakeVisible (tabs);
    tabs.setLookAndFeel (&customLook);
//    tabs.setTabBarDepth(40);
//    tabs.setCurrentTabIndex (0);
    
    setOpaque (true);
    setSize (800, 480);
}

GUI::~GUI ()
{
}

void GUI::paint (Graphics& g)
{
//    g.fillAll (setUIColour (LookAndFeel_V4::ColourScheme::UIColour::windowBackground));
    g.fillAll (Colours::black);
    g.setFont(Font("Courier New", "Regular", 20.0f));
    
}

void GUI::resized()
{
//    Rectangle<int> area (getLocalBounds());
//    tabs.setBounds (getLocalBounds().reduced (4));
    tabs.setBoundsInset (BorderSize<int> (0));
//    tabs.setBounds (getLocalBounds().reduced (4));
    
}


