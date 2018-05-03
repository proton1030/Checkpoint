/*
  ==============================================================================

    ListItems.cpp
    Created: 22 Apr 2018 3:18:20pm
    Author:  hanyu

  ==============================================================================
*/
#include "../JuceLibraryCode/JuceHeader.h"

#include "ListItems.h"
#include <string>


ListItems::ListItems(HardwareControl::ControllingParams_t targetParam):isHighlighted(false), m_eAssociatedParam(targetParam)
{
    addAndMakeVisible(m_Label = new Label(getParamNameString(m_eAssociatedParam), TRANS(getParamNameString(m_eAssociatedParam))));

    addAndMakeVisible(m_Slider = new Slider("new slider"));
    if (m_eAssociatedParam == HardwareControl::k_VCO_FMType)
        m_Slider->setRange(0, 2, 1);
    else
        m_Slider->setRange(0, 200, 1);
    m_Slider->setSliderStyle(Slider::LinearBar);
    m_Slider->setTextBoxStyle(Slider::TextBoxRight, false, 40, proportionOfHeight(0.8));
    
}

ListItems::~ListItems()
{
}

void ListItems::resized()
{
    m_Label->setBounds(0, proportionOfHeight(0.1), proportionOfWidth(0.3), proportionOfHeight(0.8));
    m_Slider->setBounds(proportionOfWidth(0.3), proportionOfHeight(0.1), proportionOfWidth(0.69), proportionOfHeight(0.8));
}

void ListItems::paint(Graphics& g)
{
    if (isHighlighted)
    {
        g.fillAll(Colours::lightblue);
    }
    else
    {
        g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
    }
    g.setFont (Font (16.0f));
    g.setColour (Colours::white);
}

std::string ListItems::getParamNameString(HardwareControl::ControllingParams_t eParam)
{
    switch (eParam)
    {
        case HardwareControl::k_VCO_VoltagePerOctave:
            return "VCO:V/O";
            break;
        case HardwareControl::k_VCO_FMGen:
            return "VCO:FM Freq";
            break;
        case HardwareControl::k_VCO_PWM:
            return "VCO:PWM";
            break;
        case HardwareControl::k_VCO_FMType:
            return "VCO:FM Type";
            break;
        case HardwareControl::k_MIX1_SineAmt:
            return "VCO:Sine Mix";
            break;
        case HardwareControl::k_MIX1_TriangleAmt:
            return "VCO:Tri Mix";
            break;
        case HardwareControl::k_MIX1_SawAmt:
            return "VCO:Saw Mix";
            break;
        case HardwareControl::k_MIX1_SquareAmt:
            return "VCO:Square Mix";
            break;
        case HardwareControl::k_VCF_Resonance:
            return "VCF:Reso";
            break;
        case HardwareControl::k_VCF_Cutoff:
            return "VCF:Cutoff";
            break;
        case HardwareControl::k_MIX2_LowpassAmt:
            return "VCF:LPF Mix";
            break;
        case HardwareControl::k_MIX2_BandpassAmt:
            return "VCF:BPF Mix";
            break;
        case HardwareControl::k_MIX2_HighpassAmt:
            return "VCF:HPF Mix";
            break;
        case HardwareControl::k_MIX2_VCOAmt:
            return "VCF:Ori Mix";
            break;
        case HardwareControl::k_VCA_Level:
            return "VCA:Level";
            break;
        case HardwareControl::k_VCA_BleedAmt:
            return "VCA:Bleed";
            break;
        default:
        case HardwareControl::kNumControllingParams:
            return "ERROR";
            break;
    }
}





