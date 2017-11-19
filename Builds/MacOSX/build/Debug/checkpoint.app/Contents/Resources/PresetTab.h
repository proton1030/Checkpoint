//
//  PresetTab.h
//  checkpoint
//
//  Created by Hanyu Liu on 11/14/17.
//

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include <vector>

class PresetTab : public Component, public ChangeBroadcaster
{
public:
    PresetTab();
    ~PresetTab();
    
    void paint (Graphics& g) override;
    void resized() override;
    
private:
//    ScopedPointer<Slider> xaxis, yaxis;
    Point<float> lastMousePosition;
    std::vector<float> dragKnobBoundries;
    float circleRadius;
    
    void mouseDown (const MouseEvent& e) override;
    void mouseDrag (const MouseEvent& e) override;
    void mouseUp (const MouseEvent&) override;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PresetTab)
};
