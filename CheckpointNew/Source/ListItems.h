/*
  ==============================================================================

    ListItems.h
    Created: 22 Apr 2018 3:18:20pm
    Author:  hanyu

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "HardwareControl.h"
#include <string>

class ListItems : public Component
{
public:
    ListItems(HardwareControl::ControllingParams_t targetParam);
    ~ListItems();
    
    void resized() override;
    void paint(Graphics&) override;
    
    bool isHighlighted;

    
    ScopedPointer<Label> m_Label;
    ScopedPointer<Slider> m_Slider;
    HardwareControl::ControllingParams_t m_eAssociatedParam;
    std::string getParamNameString(HardwareControl::ControllingParams_t eParam);
private:


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ListItems)
};
