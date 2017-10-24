
#include <iostream>
#include <typeinfo>
#include "GUI.h"

GUI::GUI ()
    : deviceManager()
{


    addAndMakeVisible (tabbedComponent = new TabbedComponent (TabbedButtonBar::TabsAtTop));
    tabbedComponent->setTabBarDepth (25);
    tabbedComponent->addTab (TRANS("Tab 0"), Colours::lightgrey, 0, false);
    tabbedComponent->addTab (TRANS("Tab 1"), Colours::lightgrey, 0, false);
    tabbedComponent->addTab (TRANS("Tab 2"), Colours::lightgrey, 0, false);
    tabbedComponent->setCurrentTabIndex (0);

    addAndMakeVisible (liveAudioScroller = new LiveScrollingAudioDisplay());
    liveAudioScroller->setName ("audioScroller");

    addAndMakeVisible (recordingThumbnail = new RecordingThumbnail());
    recordingThumbnail->setName ("audioThumbnail");


    setSize (320, 240);


    deviceManager.initialise (2, 2, nullptr, true, String(), nullptr);
    deviceManager.addAudioCallback (liveAudioScroller);
    recorder = new AudioRecorder(recordingThumbnail->getAudioThumbnail(),recordingThumbnail->getDisplayfull());
    deviceManager.addAudioCallback (recorder);

}

GUI::~GUI()
{

    deviceManager.removeAudioCallback (recorder);
    deviceManager.removeAudioCallback (liveAudioScroller);

    tabbedComponent = nullptr;
    liveAudioScroller = nullptr;
    recordingThumbnail = nullptr;


}

void GUI::paint (Graphics& g)
{

    g.fillAll (Colour (0xff323e44));

}

void GUI::resized()
{

    tabbedComponent->setBounds (0, 0, proportionOfWidth (1.0000f), proportionOfHeight (1.0000f));
    liveAudioScroller->setBounds (0, 32, proportionOfWidth (1.0000f), proportionOfHeight (0.2000f));
    recordingThumbnail->setBounds (0, 32 + proportionOfHeight (0.2000f), proportionOfWidth (1.0000f), proportionOfHeight (0.2000f));

}


