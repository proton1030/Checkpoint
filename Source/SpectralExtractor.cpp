//
//  SpectralExtractor.cpp
//  checkpoint - App
//
//  Created by Hanyu Liu on 11/19/17.
//

#include "../JuceLibraryCode/JuceHeader.h"
#include "SpectralExtractor.h"


SpectralExtractor::SpectralExtractor(const int& systemBufferSize, const double& sampleRate): systemBufferSize(systemBufferSize), sampleRate(sampleRate)
{
    initialize();
};

void SpectralExtractor::initialize()
{
    forwardFFT = new dsp::FFT(9);
    fifoIndex = 0;
    processFlag = false;
    sampleLengthCnt = 0;
    signalDurationThres = 20;
    cutoffThres = 5.0;
    currentSpectralEnvelope = {};
    specEnvCache = {};
};

void SpectralExtractor::process(const float* currentBlockPtr, int signalStatus)
{
    if (signalStatus)
    {
        sampleLengthCnt ++;
        for (int i = 0; i < systemBufferSize; i++)
            pushNextSampleIntoFifo (currentBlockPtr[i]);
        forwardFFT->performFrequencyOnlyForwardTransform(fftData);
        cutoffFreqExtractor();
    }
    else
    {
        if (sampleLengthCnt >= signalDurationThres)
        {
            averageFiltering(10, (int)currentSpectralEnvelope.size(), -5);
            specEnvCache = currentSpectralEnvelope;
        }
        fifoIndex = 0;
        currentSpectralEnvelope = {};
        sampleLengthCnt = 0;
    }
};

void SpectralExtractor::pushNextSampleIntoFifo (float sample) noexcept
{
    if (fifoIndex == fftSize)
    {
        zeromem (fftData, sizeof (fftData));
        memcpy (fftData, fifo, sizeof (fifo));
        fifoIndex = 0;
    }
    fifo[fifoIndex++] = sample;
};

void SpectralExtractor::cutoffFreqExtractor()
{
    Range<float> maxLevel = FloatVectorOperations::findMinAndMax (fftData, fftSize / 2);
    float multParam = 255.0 / maxLevel.getEnd();
    for (int i = 255; i >= 0 ; i--)
    {
        fftData[i] *= multParam;
        if (fftData[i] > cutoffThres)
        {
            currentSpectralEnvelope.push_back(i);
            break;
        }
    }
};

void SpectralExtractor::averageFiltering(int order, int signalSize, int circshift)
{
    
    std::vector<float> outputSignal;
    float sumBuffer = 0.0;
    for (int i = 0; i < signalSize; i++)
    {
        if (i < order)
            sumBuffer += currentSpectralEnvelope[i];
        else
            sumBuffer = sumBuffer + currentSpectralEnvelope[i] - currentSpectralEnvelope[i-order];
        outputSignal.push_back(sumBuffer/order);
    }
    if (circshift == 0)
    {
        for (int i = 0; i < signalSize; i++)
            currentSpectralEnvelope[i] = outputSignal[i];
    }
    else if (circshift > 0 && circshift < signalSize)
    {
        for (int i = 0; i < signalSize-circshift; i++)
            currentSpectralEnvelope[i+circshift] = outputSignal[i];
        for (int i = signalSize-circshift; i < signalSize; i++)
            currentSpectralEnvelope[i+circshift-signalSize-1] = outputSignal[i];
    }
    else if (circshift < 0 && circshift > -signalSize)
    {
        for (int i = 0; i < signalSize-circshift; i++)
            currentSpectralEnvelope[i] = outputSignal[i+circshift];
        for (int i = signalSize-circshift; i < signalSize; i++)
            currentSpectralEnvelope[i] = outputSignal[i+circshift-signalSize-1];
    }
    else
    {
        for (int i = 0; i < signalSize; i++)
            currentSpectralEnvelope[i] = outputSignal[i];
    }
    
}



