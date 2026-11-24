#include "MauserichLookAndFeel.h"

void MauserichLookAndFeel::setDialImage(juce::Image* dialImage)
{
    this->dialImage = dialImage;
}
void MauserichLookAndFeel::setFaderImage(juce::Image* faderImage)
{
    this->faderImage = faderImage;
}
void MauserichLookAndFeel::setCheeseImage(juce::Image* cheeseImage)
{
    this->cheeseImage = cheeseImage;
}

void MauserichLookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos, const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider& slider)
{
    juce::Image& img = slider.getName().equalsIgnoreCase("cheese") ? *cheeseImage : *dialImage;
    
    float angle = rotaryStartAngle + (sliderPos * (rotaryEndAngle - rotaryStartAngle));
    float minSize = std::min(width, height);
    
    juce::AffineTransform transform;
    
    transform = transform.scaled(minSize / (float) img.getWidth(), minSize / (float) img.getHeight())
        .rotated(angle, minSize / 2.f, minSize / 2.f)
        .translated(x + (width - minSize) / 2.f, y + (height - minSize) / 2.f);
    
    g.drawImageTransformed(img, transform);
}

void MauserichLookAndFeel::drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos, float minSliderPos, float maxSliderPos, juce::Slider::SliderStyle, juce::Slider&)
{
    
    juce::Image& img = *faderImage;
    
    float scalar = 0.4f;
    float scale = width / (float) img.getWidth();
    float top = sliderPos - (img.getHeight() * scale * scalar) / 2.f;

    float center = width / 2.f;
    float backgroundWidth = 3.5f;
    int segments = 2 * 5;
    
    
    g.setColour(juce::Colour::fromHSL(0.f, 0.f, 0.7f, 1.f));
    for (int i = 0; i <= segments; i++)
    {
        float amp = (i % 5 == 0) ? 9.f : 6.f;
        float h = juce::jmap((float) i, 0.f, (float) segments, 0.f, 1.f) * (height) + y;
        
        g.drawLine(center - amp, h, center + amp, h);
    }
    
    
    g.setColour(juce::Colour(15, 5, 50));
    g.fillRoundedRectangle(width / 2.f - backgroundWidth / 2.f, (float) y, backgroundWidth, (float) height, backgroundWidth / 2.f);
    
    
    // g.drawImage(img, width / 2 - scalar * width / 2, top, width * scalar, img.getHeight() * scale * scalar, 0, 0, img.getWidth(), img.getHeight());
    
    juce::AffineTransform transform;
    
    transform = transform.scaled(scale * scalar)
        .translated(width / 2 - width * scalar / 2.f, top);
    
    g.drawImageTransformed(img, transform);
    
}

void MauserichLookAndFeel::drawLabel(juce::Graphics& g, juce::Label& label)
{
    label.setPaintingIsUnclipped(true);
    g.setFont(fnt.withHeight(LABEL_HEIGHT));
    g.setColour(juce::Colours::white);
    g.drawText(label.getText(), 0, 0, label.getWidth(), label.getHeight(), juce::Justification::centred);
}

float MauserichLookAndFeel::getLabelHeight()
{
    return LABEL_HEIGHT;
}
