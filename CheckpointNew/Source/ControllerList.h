/*
  ==============================================================================

    ControllerList.h
    Created: 22 Apr 2018 4:40:51pm
    Author:  Hanyu Liu/Users/liuhanyu1030/Documents/audiosoftware/Checkpoint/CheckpointNew/Source/ListItems.cpp

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "ListItems.h"
#include "HardwareControl.h"
#include <iostream>
#include <vector>

class ControllerList: public ListBox, public ListBoxModel, public Timer, public Slider::Listener
{
public:
    ControllerList();
    ~ControllerList();
    
    void resized() override;
    int getNumRows() override;
    Component* refreshComponentForRow(int rowNumber, bool isRowSelected, Component* existingComponentToUpdate) override;
    void paintListBoxItem (int rowNumber,
                                   Graphics& g,
                                   int width, int height,
                                   bool rowIsSelected) override {};
    
    void getHardwareControlClasss(HardwareControl &ctlclass);
    
    void sliderValueChanged (Slider* slider) override;
    void timerCallback() override;
    
private:
    ListBox listBox;
    std::vector<ListItems*>         controlKnobs;
    
    int iRowSelection;
    HardwareControl*    m_pHardwareControl = 0;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ControllerList)
};
