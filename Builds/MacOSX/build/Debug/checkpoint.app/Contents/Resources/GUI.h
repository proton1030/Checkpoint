
#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "MainUITabs.h"


class GUI  : public Component
{
public:
   
    GUI ();
    ~GUI();

    void paint (Graphics& g) override;
    void resized() override;

private:

    MainUITabs tabs;
    LookAndFeel_V4 customLook;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GUI)
};

