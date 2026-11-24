#pragma once

#include "JuceHeader.h"

class Container : public juce::Component
{
public:
    void paint(juce::Graphics& g) override;
    void setDisplayText(const juce::String& text);
    
private:
    const int TEXT_SIZE = 23.f;
    juce::FontOptions fnt = { juce::Typeface::createSystemTypefaceFor(BinaryData::BarlowCondensedItalic_ttf, BinaryData::BarlowCondensedItalic_ttfSize) };

    juce::String text;
    juce::TextLayout textLayout;
    float textMargin = 10.f;
};
