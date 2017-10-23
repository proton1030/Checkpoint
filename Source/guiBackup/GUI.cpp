/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.1.2

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright (c) 2015 - ROLI Ltd.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
//[/Headers]
#include "../JuceLibraryCode/JuceHeader.h"
#include "GUI.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
GUI::GUI () : deviceManager (),
recorder (recordingThumbnail.getAudioThumbnail(),recordingThumbnail.getDisplayfull())
{
    setOpaque (true);
    addAndMakeVisible (liveAudioScroller);

    addAndMakeVisible (recordButton);
    recordButton.setButtonText ("Record");
    recordButton.addListener (this);
    recordButton.setColour (TextButton::buttonColourId, Colour (0xffff5c5c));
    recordButton.setColour (TextButton::textColourOnId, Colours::black);

    addAndMakeVisible (recordingThumbnail);

    deviceManager.initialise (2, 2, nullptr, true, String(), nullptr);
    deviceManager.addAudioCallback (&liveAudioScroller);
    deviceManager.addAudioCallback (&recorder);

    setSize (320 , 240);
    std::cout << "Application start" << std::endl;
}

GUI::~GUI()
{
    deviceManager.removeAudioCallback (&recorder);
    deviceManager.removeAudioCallback (&liveAudioScroller);
    std::cout << "Application close" << std::endl;
}

//==============================================================================
void GUI::paint (Graphics& g)
{
    g.fillAll (Colour (0xffaaaaaa));
    g.setColour (Colour (0xffffffff));
}

void GUI::resized()
{
    Rectangle<int> area (getLocalBounds());
    liveAudioScroller.setBounds (area.removeFromTop (80).reduced (0));
    recordingThumbnail.setBounds (area.removeFromTop (80).reduced (0));
    // recordButton.setBounds (area.removeFromTop (36).removeFromLeft (140).reduced (8));
    // explanationLabel.setBounds (area.reduced (8));
}

void GUI::buttonClicked (Button* button)
{

}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="GUI" componentName="" parentClasses="public Component"
                 constructorParams="" variableInitialisers="" snapPixels="8" snapActive="1"
                 snapShown="1" overlayOpacity="0.330" fixedSize="0" initialWidth="600"
                 initialHeight="400">
  <BACKGROUND backgroundColour="ff323e44"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
