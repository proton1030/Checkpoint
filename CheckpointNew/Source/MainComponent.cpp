/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
MainComponent::MainComponent()
{
    addAndMakeVisible (listBox);
    listBox.setRowHeight (20);
    // listBox.setModel (this);   // Tell the listbox where to get its data model
    listBox.setColour (ListBox::textColourId, Colours::black);
    listBox.setColour (ListBox::backgroundColourId, Colours::white);

    setSize (320, 240);


}

MainComponent::~MainComponent()
{
}

//==============================================================================
void MainComponent::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setFont (Font (16.0f));
    g.setColour (Colours::white);
    g.drawText ("Hello World!", getLocalBounds(), Justification::centred, true);
}

void MainComponent::resized()
{
    // This is called when the MainComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
}
