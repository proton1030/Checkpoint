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
    ampMode = 0;
    
    training = new TrainingTab();
    amplitude = new AmplitudeConfigTab();
    DAC = new DacControl();
    
    const Colour c;
    addTab ("Training", c, training, true);
    addTab ("VCA", c, amplitude, true);
    
    ampExtModule = training->getAmpExtModule();
    ampExtModule->addChangeListener(this);
    training->addChangeListener(this);
    amplitude->addChangeListener(this);
    
    training->reset->addListener(this);
    amplitude->switchMode->addListener(this);
    
    
    for (int i = 0; i < getNumTabs(); ++i)
    {
        setTabBackgroundColour (i, getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
    }
}

MainUITabs::~MainUITabs()
{
    clearTabs();
}

void MainUITabs::changeListenerCallback(ChangeBroadcaster* source)
{
    if (source == ampExtModule && ampMode == 1)
    {
//        std::vector<float> temp = training->getADSRValues();
//        std::cout << temp[0] << std::endl;
        DAC->setADSRValues(training->getADSRValues());
        amplitude->setSliderValues(training->getADSRValues());
    }
    else if (source == training)
    {
        DAC->setADSROutputVoltageStatus();
    }
    else if (source == amplitude  && ampMode == 0)
    {
        std::cout << "slider changed" << " | " << ampMode << std::endl;
        DAC->setADSRValues(amplitude->getADSRValues());
    }
    else
    {
        std::cout << "unexpected" << std::endl;
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
    else if ( buttonThatWasClicked == amplitude->switchMode )
    {
        std::cout << amplitude->currentOutputMode << std::endl;
        ampMode = amplitude->currentOutputMode;
    }
    
}

