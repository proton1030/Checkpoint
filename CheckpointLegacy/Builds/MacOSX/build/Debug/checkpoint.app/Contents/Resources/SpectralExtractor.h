//
//  SpectralExtractor.h
//  checkpoint
//
//  Created by Hanyu Liu on 11/19/17.
//

#pragma once
#include <vector>
#include "../JuceLibraryCode/JuceHeader.h"

class SpectralExtractor : public Component
{
public:
    SpectralExtractor(
                      const int& systemBufferSize,
                      const double& sampleRate);
    ~SpectralExtractor(){};
    
    void initialize();
    void clear(){};
    void process(const float* currentBlockPtr, int signalStatus);
    
    std::vector<float> specEnvCache;
private:
    void pushNextSampleIntoFifo (float sample) noexcept;
    void cutoffFreqExtractor();
    void averageFiltering(int order, int signalSize, int circshift);
    
    //System-acquired parameters
    int systemBufferSize;
    double sampleRate;
    
    ScopedPointer<dsp::FFT> forwardFFT;
    
    float fftSize = 512;
    float fifo [512];
    float fftData [2 * 512];
    int fifoIndex, sampleLengthCnt, signalDurationThres;
    bool processFlag;
    float cutoffThres;
    std::vector<float> currentSpectralEnvelope;
   
    
    
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SpectralExtractor)
};
