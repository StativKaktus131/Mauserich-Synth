#include "Sliders.h"
#include <format>

juce::String DecibelSlider::getTextFromValue(double value)
{
    return juce::Decibels::toString(value);
}

double DecibelSlider::getValueFromText(const juce::String& text)
{
    double minusInfinityDB = -100.0;
    juce::String decibelText = text.upToFirstOccurrenceOf("dB", false, true).trim();
    return decibelText.equalsIgnoreCase("-INF") ? minusInfinityDB : decibelText.getDoubleValue();
}


juce::String TimeSlider::getTextFromValue(double value)
{
    char buffer[16];

    if (value >= 1.0)
        std::snprintf(buffer, 16, "%.3gs", value);
    else
        std::snprintf(buffer, 16, "%dms", (int) (value * 1000));
    
    return juce::String(buffer);
}

double TimeSlider::getValueFromText(const juce::String& text)
{
    bool ms = text.contains("ms");
    
    return ms ? text.upToFirstOccurrenceOf("ms", false, true).trim().getDoubleValue() :
                text.upToFirstOccurrenceOf("s", false, true).trim().getDoubleValue() * 1000;
}

juce::String PercentSlider::getTextFromValue(double value)
{
    char buffer[16];
    std::snprintf(buffer, 16, "%d%%", (int) (value * 100));
    return juce::String(buffer);
}

double PercentSlider::getValueFromText(const juce::String &text)
{
    return text.upToFirstOccurrenceOf("%", false, true).trim().getDoubleValue() / 100.0;
}
