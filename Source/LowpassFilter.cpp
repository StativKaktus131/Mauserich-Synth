#include "LowpassFilter.h"

void LowpassFilter::setCoefficients(float cutoff, float resonance, float sampleRate)
{
    cutoff = std::clamp(cutoff, 1.f, sampleRate * 0.45f);
    
    const float Q = 0.707f + resonance * (20.f - 0.707f);
    const float k = 1.f / Q;
    
    const float g = std::tan(juce::MathConstants<float>::pi * cutoff / sampleRate);
    const float denom = 1.f + g * (g + k);
    
    a1 = 1.f / denom;
    a2 = g * a1;
    a3 = g * a2;
}

float LowpassFilter::processSample(float input)
{
    const float v3 = input - ic2eq;
    const float v1 = a1 * ic1eq + a2 * v3;
    const float v2 = ic2eq + a2 * ic1eq + a3 * v3;
    
    ic1eq = 2.f * v1 - ic1eq;
    ic2eq = 2.f * v2 - ic2eq;
    
    return v2;
}
