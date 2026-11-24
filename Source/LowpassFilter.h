#pragma once

#include "JuceHeader.h"

class LowpassFilter
{
public:
    LowpassFilter() = default;
    ~LowpassFilter() = default;
    
    void setCoefficients (float cutoff, float resonance, float sampleRate);
    
    float processSample(float input);
    
private:
    float a1 = 0.f, a2 = 0.f, a3 = 0.f;
    float ic1eq = 0.f, ic2eq = 0.f;
    
};

