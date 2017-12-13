//
//  SpectralConfigTab.h
//  checkpoint
//
//  Created by Hanyu Liu on 11/20/17.
//

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class SpectralConfigTab : public Component, public Slider::Listener, public ButtonListener, public ChangeBroadcaster
{
public:
    SpectralConfigTab();
    ~SpectralConfigTab();
    void paint (Graphics& g) override;
    void resized() override;
    
    Value currentOutputMode;
private:
    ScopedPointer<Slider> cutoff;
    ScopedPointer<Label> cutoffLabel;
    ScopedPointer<TextButton> switchMode;
    
    void sliderValueChanged (Slider* slider) override;
    void buttonClicked (Button* buttonThatWasClicked) override;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SpectralConfigTab)
};
