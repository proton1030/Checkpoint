/*
  ==============================================================================

    ControllerList.cpp
    Created: 22 Apr 2018 4:40:51pm
    Author:  Hanyu Liu

  ==============================================================================
*/

#include "ControllerList.h"
#include <string>


ControllerList::ControllerList() : iRowSelection(0)
{
    
    for (int i = 0; i < HardwareControl::kNumControllingParams; i++)
    {
        controlKnobs.push_back(new ListItems(HardwareControl::ControllingParams_t(i)));
        controlKnobs[i]->m_Slider->addListener(this);
    }
    
    
    addAndMakeVisible(listBox);
    listBox.setModel(this);
    listBox.setRowHeight(15);
    listBox.selectRow(iRowSelection);
    listBox.setRowSelectedOnMouseDown(true);
    
//    startTimer(1000);
    
}

ControllerList::~ControllerList()
{

}

void ControllerList::resized()
{
    listBox.setBounds(getLocalBounds());
}

int ControllerList::getNumRows()
{
    return int(controlKnobs.size());
}

void ControllerList::getHardwareControlClasss(HardwareControl &ctlclass)
{
    m_pHardwareControl = &ctlclass;
}

Component* ControllerList::refreshComponentForRow(int rowNumber, bool isRowSelected, Component* existingComponentToUpdate)
{
    
    if(rowNumber < controlKnobs.size())
    {
        if (isRowSelected)
        {
            controlKnobs[rowNumber]->isHighlighted = true;
        }
        else
        {
            controlKnobs[rowNumber]->isHighlighted = false;
        }
        return controlKnobs[rowNumber];
    }
    else
    {
        existingComponentToUpdate = nullptr;
        return nullptr;
    }
    
}

void ControllerList::timerCallback()
{
    if (iRowSelection >= controlKnobs.size())
        iRowSelection = 0;
    listBox.selectRow(iRowSelection);
    iRowSelection ++;
}

void ControllerList::sliderValueChanged (Slider* slider)
{
    for (int i = 0; i < controlKnobs.size(); i++)
    {
        if (slider == controlKnobs[i]->m_Slider)
        {
            if (m_pHardwareControl != 0)
            {
                m_pHardwareControl->setParam(controlKnobs[i]->m_eAssociatedParam, int(slider->getValue()));
            }
            else
            {
                std::cout << "Controller class not associated yet!" << std::endl;
            }
        }
    }
    
}
