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
    spectral = new SpectralConfigTab();
    DAC = new DacControl();
    presetSetting = new PresetTab();
    
    addTab ("Train", Colours::black, training, true);
    addTab ("VCA", Colours::black, amplitude, true);
    addTab ("VCF", Colours::black, spectral, true);
    addTab ("Morph", Colours::black, presetSetting, true);
    setTabBarDepth(30);
    
    //Set linteners to modules
    ampExtModule = training->getAmpExtModule();
    specExtModule = training->getSpecExtModule();
    training->addChangeListener(this);
    amplitude->addChangeListener(this);
    training->getRecorderModule()->addChangeListener(this);
    
    //Set linsteners to buttons
    training->train->addListener(this);
    training->reset->addListener(this);
    amplitude->switchMode->addListener(this);
    presetSetting->switchMode->addListener(this);
    
    
    //Set listeners to Flags
    amplitude->currentOutputMode.addListener(this);
    spectral->currentOutputMode.addListener(this);
    presetSetting->currentOutputMode.addListener(this);
    
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
    if (source == training->getRecorderModule() && ampMode == 1)
    {
//        std::cout << "snippet" << std::endl;
        DAC->DAC_TL.ampADSR = training->getADSRValues();
        DAC->DAC_TL.ampEnv = ampExtModule->currentAmplitudeEnvelope;
        DAC->DAC_TL.specCutoffEnv = specExtModule->specEnvCache;
        DAC->DAC_TL.ampMode = 1;
        DAC->DAC_TL.specMode = 1;
        DAC->updateAllSig();
        amplitude->setSliderValues(training->getADSRValues());
    }
    else if (source == training)
    {
        DAC->buttonPlay = training->getTriggerButtonStatus();
    }
    else if (source == amplitude  && ampMode == 2)
    {
        DAC->DAC_TL.ampADSR = amplitude->getADSRValues();
        DAC->updateAllSig();
    }
    else if (ampMode == 3)
    {
        std::cout << "morph" << std::endl;
    }
    else
    {
        std::cout << ampMode << std::endl;
    }
    
    
//    if (source == ampExtModule && ampMode == 1)
//    {
//        DAC->setADSRValues(training->getADSRValues());
//        amplitude->setSliderValues(training->getADSRValues());
//    }
}

void MainUITabs::buttonClicked (Button* buttonThatWasClicked)
{
    if ( buttonThatWasClicked == training->reset )
    {
        std::vector<float> temp = {0.0, 0.0, 0.0, 0.0};
        ampExtModule->clear();
        DAC->DAC_TL.ampADSR = temp;
        DAC->updateAllSig();
        amplitude->setSliderValues(temp);
    }
    else if ( buttonThatWasClicked == presetSetting->switchMode )
    {
//        DAC->outputSig = presetSetting->currentOutputMode.getValue();
    }
    
}

void MainUITabs::valueChanged(Value& value)
{
    if (value == presetSetting->currentOutputMode)
        DAC->outputSig = presetSetting->currentOutputMode.getValue();
    else
    {
        ampMode = amplitude->currentOutputMode.getValue();
        specMode = spectral->currentOutputMode.getValue();
        DAC->DAC_TL.ampMode = ampMode;
        DAC->DAC_TL.specMode = specMode;
    }
}

