//
//  MainUITabs.cpp
//  checkpoint - App
//
//  Created by Hanyu Liu on 10/24/17.
//

#include "../JuceLibraryCode/JuceHeader.h"
#include "MainUITabs.h"


MainUITabs::MainUITabs() : TabbedComponent (TabbedButtonBar::TabsAtTop)
{
    ampMode = 1;
    
    training = new TrainingTab();
    amplitude = new AmplitudeConfigTab();
    DAC = new DacControl();
    presetSetting = new PresetTab();
    
    addTab ("Train", Colours::black, training, true);
    addTab ("VCA", Colours::black, amplitude, true);
    addTab ("Morph", Colours::black, presetSetting, true);
    setTabBarDepth(25);
    
    //Set linteners to modules
    ampExtModule = training->getAmpExtModule();
    ampExtModule->addChangeListener(this);
    training->addChangeListener(this);
    amplitude->addChangeListener(this);
    
    //Set linsteners to buttons
    training->train->addListener(this);
    training->reset->addListener(this);
    amplitude->switchMode->addListener(this);
    
    
    //Set listeners to Flags
    amplitude->currentOutputMode.addListener(this);
    
//    //Init settings
//    training->setExtractorWorking(true);
    
    
    for (int i = 0; i < getNumTabs(); ++i)
    {
//        setTabBackgroundColour (i, getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
    }
}

MainUITabs::~MainUITabs()
{
    ampExtModule->removeAllChangeListeners();
    training->removeAllChangeListeners();
    amplitude->removeAllChangeListeners();
    DAC=nullptr;
    training=nullptr;
    amplitude=nullptr;
    presetSetting=nullptr;
    clearTabs();
}

void MainUITabs::paint (Graphics& g)
{
    //    g.fillAll (setUIColour (LookAndFeel_V4::ColourScheme::UIColour::windowBackground));
    g.fillAll (Colours::black);
    g.setFont(Font("Courier New", "Regular", 20.0f));
    
    
}

void MainUITabs::changeListenerCallback(ChangeBroadcaster* source)
{
    if (source == ampExtModule && ampMode == 1)
    {
        DAC->setADSRValues(training->getADSRValues());
        amplitude->setSliderValues(training->getADSRValues());
    }
    else if (source == training)
    {
        DAC->buttonPlay = training->getTriggerButtonStatus();
    }
    else if (source == amplitude  && ampMode == 2)
    {
        DAC->setADSRValues(amplitude->getADSRValues());
    }
    else if (ampMode == 3)
    {
        std::cout << "morph" << std::endl;
    }
    else
    {
        std::cout << ampMode << std::endl;
    }
}

void MainUITabs::buttonClicked (Button* buttonThatWasClicked)
{
    if ( buttonThatWasClicked == training->reset )
    {
        std::vector<float> temp = {0.0, 0.0, 0.0, 0.0};
        ampExtModule->clear();
        DAC->setADSRValues(temp);
        amplitude->setSliderValues(temp);
    }
    else if ( buttonThatWasClicked == training->train )
    {
        
        std::cout << training->train->getState() << std::endl;
//        training->setExtractorWorking(true);
    }
    else if ( buttonThatWasClicked == amplitude->switchMode )
    {

    }
    
    
}

void MainUITabs::valueChanged(Value& value)
{
    ampMode = amplitude->currentOutputMode.getValue();
}

