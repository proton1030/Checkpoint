//
//  TrainingTab.cpp
//  checkpoint - App
//
//  Created by Hanyu Liu on 10/24/17.
//

#include "../JuceLibraryCode/JuceHeader.h"
#include "TrainingTab.h"
#include <iostream>
#include <vector>


TrainingTab::TrainingTab() : deviceManager()
{
    std::cout << "Initialized" << std::endl;
    
    addAndMakeVisible (liveAudioScroller = new LiveScrollingAudioDisplay());
    liveAudioScroller->setName ("audioScroller");
   
    addAndMakeVisible (recordingThumbnail = new RecordingThumbnail());
    recordingThumbnail->setName ("audioThumbnail");
    
    addAndMakeVisible (trigger = new TextButton ("Test"));
    trigger->addListener (this);
    
    addAndMakeVisible (reset = new TextButton ("Reset"));
    reset->addListener (this);
    
    deviceManager = new AudioDeviceManager();
    deviceManager->initialiseWithDefaultDevices(2, 2);
    deviceManager->addAudioCallback (liveAudioScroller);
    recorder = new AudioRecorder(recordingThumbnail->getAudioThumbnail(),recordingThumbnail->getDisplayfull());
    deviceManager->addAudioCallback (recorder);
    
    
    
}

TrainingTab::~TrainingTab()
{
    deviceManager->removeAudioCallback (recorder);
    deviceManager->removeAudioCallback (liveAudioScroller);
    liveAudioScroller = nullptr;
    recordingThumbnail = nullptr;
}

void TrainingTab::paint (Graphics& g)
{
    g.fillAll (Colour (0xff323e44));
}

void TrainingTab::resized()
{
    liveAudioScroller->setBounds (0, 0, proportionOfWidth (1.0000f), proportionOfHeight (0.1500f));
    recordingThumbnail->setBounds (0, proportionOfHeight (0.1500f), proportionOfWidth (1.0000f), proportionOfHeight (0.3000f));
    trigger->setBounds(proportionOfWidth (0.0300f), proportionOfHeight (0.8000f), proportionOfWidth (0.2000f), proportionOfHeight (0.1500f));
    reset->setBounds(proportionOfWidth (0.7700f), proportionOfHeight (0.8000f), proportionOfWidth (0.2000f), proportionOfHeight (0.1500f));
}

void TrainingTab::buttonStateChanged(Button* buttonThatWasClicked)
{
    if ( buttonThatWasClicked == trigger && trigger->isDown() )
    {
        sendChangeMessage();
    }
}

void TrainingTab::buttonClicked (Button* buttonThatWasClicked)
{
    if ( buttonThatWasClicked == trigger )
        sendChangeMessage();
    else if ( buttonThatWasClicked == reset )
    {
        
    }
}

std::vector<float>& TrainingTab::getADSRValues()
{
    return recorder->getADSRValues();
}

bool TrainingTab::getTriggerButtonStatus()
{
    return trigger->isDown();
}

ScopedPointer<AmplitudeExtractor>& TrainingTab::getAmpExtModule()
{
    return recorder->getAmpExtModule();
}


