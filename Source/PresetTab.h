//
//  PresetTab.h
//  checkpoint
//
//  Created by Hanyu Liu on 11/14/17.
//

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include <vector>

class PresetTab : public Component, public ChangeBroadcaster, public ButtonListener
{
public:
    PresetTab();
    ~PresetTab();
    
    void paint (Graphics& g) override;
    void resized() override;
    ScopedPointer<TextButton> switchMode;
    Value currentOutputMode;
    
private:

    Point<float> lastMousePosition;
    
    std::vector<float> dragKnobBoundries;
    float circleRadius;
    
    void mouseDown (const MouseEvent& e) override;
    void mouseDrag (const MouseEvent& e) override;
    void mouseUp (const MouseEvent&) override;
    void buttonClicked (Button* buttonThatWasClicked) override;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PresetTab)
};
