#pragma once

#include "JuceHeader.h"

class DecibelSlider : public juce::Slider
{
    juce::String getTextFromValue(double value) override;
    double getValueFromText(const juce::String& text) override;
};

class TimeSlider : public juce::Slider
{
    juce::String getTextFromValue(double value) override;
    double getValueFromText(const juce::String& text) override;
};

class PercentSlider : public juce::Slider
{
    juce::String getTextFromValue(double value) override;
    double getValueFromText(const juce::String& text) override;
};

