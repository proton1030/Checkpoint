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
    
    
    const Colour c;
    training = new TrainingTab();
    amplitude = new AmplitudeConfigTab(training);
    DAC = new DacControl(amplitude);
    addTab ("Training", c, training, true);
    addTab ("VCA", c, amplitude, true);
    
    for (int i = 0; i < getNumTabs(); ++i)
    {
        setTabBackgroundColour (i, getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
    }
    
    

}

MainUITabs::~MainUITabs()
{
    clearTabs();
    
}
