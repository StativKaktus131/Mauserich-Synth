#pragma once

#include "JuceHeader.h"

class MauserichLookAndFeel : public juce::LookAndFeel_V4
{
public:
    MauserichLookAndFeel() = default;
    ~MauserichLookAndFeel() = default;
    
    void setDialImage(juce::Image* image);
    void setFaderImage(juce::Image* image);
    void setCheeseImage(juce::Image* image);

    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos, const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider& slider) override;
    void drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos, float minSliderPos, float maxSliderPos, juce::Slider::SliderStyle, juce::Slider&) override;
    void drawLabel(juce::Graphics& g, juce::Label& label) override;
    float getLabelHeight();
    

private:
    juce::FontOptions fnt = { juce::Typeface::createSystemTypefaceFor(BinaryData::BarlowCondensedRegular_ttf, BinaryData::BarlowCondensedRegular_ttfSize) };

    juce::Image* dialImage;
    juce::Image* faderImage;
    juce::Image* cheeseImage;
    
    const float LABEL_HEIGHT = 25.f;
};
