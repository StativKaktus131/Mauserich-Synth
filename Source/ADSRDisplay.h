#pragma once

#include "JuceHeader.h"

class ADSRDisplay : public juce::Component
{
public:
    void paint(juce::Graphics& g) override;
    
    void setAttackPercent(float v);
    void setDecayPercent(float v);
    void setSustainPercent(float v);
    void setReleasePercent(float v);

private:
    float attackPercent = 0.05f;
    float decayPercent = 0.1f;
    float sustainPercent = 0.8f;
    float releasePercent = 0.2f;
};
