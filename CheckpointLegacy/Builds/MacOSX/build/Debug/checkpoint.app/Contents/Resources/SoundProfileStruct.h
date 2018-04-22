//
//  SoundProfileStruct.h
//  checkpoint
//
//  Created by Hanyu Liu on 11/20/17.
//

#pragma once
#include <vector>
#include "../JuceLibraryCode/JuceHeader.h"

struct SoundProfiles {
    std::vector<float> ampEnv;
    std::vector<float> ampADSR;
    std::vector<float> specCutoffEnv;
    int ampMode;
    int specMode;
};
