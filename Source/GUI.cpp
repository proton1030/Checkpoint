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

#include "GUI.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
GUI::GUI ()
    : deviceManager (),
      recorder (recordingThumbnail->getAudioThumbnail(),recordingThumbnail->getDisplayfull())
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    addAndMakeVisible (liveAudioScroller = new LiveScrollingAudioDisplay());
    liveAudioScroller->setName ("audioScroller");

    addAndMakeVisible (recordingThumbnail = new RecordingThumbnail());
    recordingThumbnail->setName ("audioThumbnail");


    //[UserPreSize]
    //[/UserPreSize]

    setSize (320, 240);


    //[Constructor] You can add your own custom stuff here..
    deviceManager.initialise (2, 2, nullptr, true, String(), nullptr);
    deviceManager.addAudioCallback (liveAudioScroller);
    deviceManager.addAudioCallback (&recorder);
    //[/Constructor]
}

GUI::~GUI()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    deviceManager.removeAudioCallback (&recorder);
    // deviceManager.removeAudioCallback (liveAudioScroller);
    //[/Destructor_pre]

    liveAudioScroller = nullptr;
    recordingThumbnail = nullptr;


    //[Destructor]. You can add your own custom destruction code here..

    //[/Destructor]
}

//==============================================================================
void GUI::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xff323e44));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void GUI::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    liveAudioScroller->setBounds (0, 0, proportionOfWidth (1.0000f), proportionOfHeight (0.3000f));
    recordingThumbnail->setBounds (0, 0 + proportionOfHeight (0.3000f), proportionOfWidth (1.0000f), proportionOfHeight (0.3000f));
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void GUI::buttonClicked (Button* button) {}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="GUI" componentName="" parentClasses="public Component"
                 constructorParams="" variableInitialisers="deviceManager (),&#10;    recorder (recordingThumbnail-&gt;getAudioThumbnail(),recordingThumbnail-&gt;getDisplayfull())"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="320" initialHeight="240">
  <BACKGROUND backgroundColour="ff323e44"/>
  <GENERICCOMPONENT name="audioScroller" id="abce2015b47965fb" memberName="liveAudioScroller"
                    virtualName="" explicitFocusOrder="0" pos="0 0 100% 30%" class="LiveScrollingAudioDisplay"
                    params=""/>
  <GENERICCOMPONENT name="audioThumbnail" id="a1c1303067ed82c0" memberName="recordingThumbnail"
                    virtualName="" explicitFocusOrder="0" pos="0 0R 100% 30%" posRelativeY="abce2015b47965fb"
                    class="RecordingThumbnail" params=""/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
