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
    
    //Adding components
    addAndMakeVisible (liveAudioScroller = new LiveScrollingAudioDisplay());
    liveAudioScroller->setName ("audioScroller");
   
    addAndMakeVisible (recordingThumbnail = new RecordingThumbnail());
    recordingThumbnail->setName ("audioThumbnail");
    recordingThumbnail->setColour(GroupComponent::outlineColourId, Colours::red);
    
    addAndMakeVisible (train = new TextButton ("Train"));
    train->addListener (this);
    
    addAndMakeVisible (trigger = new TextButton ("Test"));
    trigger->addListener (this);
    
    addAndMakeVisible (reset = new TextButton ("Reset"));
    reset->addListener (this);
    
    //Components init
    deviceManager = new AudioDeviceManager();
    deviceManager->initialiseWithDefaultDevices(2, 2);
    deviceManager->addAudioCallback (liveAudioScroller);
    recorder = new AudioRecorder(recordingThumbnail->getAudioThumbnail(),recordingThumbnail->getDisplayfull());
    deviceManager->addAudioCallback (recorder);
    setExtractorWorking(false);
    
    //Variable init
    trainingStatus = 0;
    
}

TrainingTab::~TrainingTab()
{
    deviceManager->removeAudioCallback (recorder);
    deviceManager->removeAudioCallback (liveAudioScroller);
    recorder = nullptr;
    liveAudioScroller = nullptr;
    liveAudioScroller = nullptr;
    recordingThumbnail = nullptr;
}

void TrainingTab::paint (Graphics& g)
{
//    g.fillAll (Colour (0x000000ff));
    g.fillAll (Colours::black);
    g.setFont(Font("Courier New", "Regular", 20.0f));
}

void TrainingTab::resized()
{
    liveAudioScroller->setBounds (0, 0, proportionOfWidth (1.0000f), proportionOfHeight (0.1500f));
    recordingThumbnail->setBounds (0, proportionOfHeight (0.1500f), proportionOfWidth (1.0000f), proportionOfHeight (0.3000f));
    train->setBounds(proportionOfWidth (0.2600f), proportionOfHeight (0.8000f), proportionOfWidth (0.4800f), proportionOfHeight (0.1500f));
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
    else if ( buttonThatWasClicked == train )
    {
        if (trainingStatus == 0)
        {
            setExtractorWorking(true);
            trainingStatus = 1;
            train->setButtonText (TRANS("Training..."));
            train->setColour(TextButton::ColourIds::buttonColourId, Colours::indianred);
            recordingThumbnail->thumbnailText = "No Signal Detected";
            recordingThumbnail->standbyFlag = 0;
        }
        else
        {
            setExtractorWorking(false);
            trainingStatus = 0;
            train->setButtonText (TRANS("Train"));
            train->setColour(TextButton::ColourIds::buttonColourId, Colours::black);
            recordingThumbnail->thumbnailText = "Standby";
            recordingThumbnail->standbyFlag = 1;
        }
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

ScopedPointer<SpectralExtractor>& TrainingTab::getSpecExtModule()
{
    return recorder->getSpecExtModule();
}

ScopedPointer<AudioRecorder>& TrainingTab::getRecorderModule()
{
    return recorder;
}

void TrainingTab::setExtractorWorking(bool workingOrNot)
{
    recorder->setExtractorWorking(workingOrNot);
}



