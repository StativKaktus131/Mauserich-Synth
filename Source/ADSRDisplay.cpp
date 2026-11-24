#include "ADSRDisplay.h"

void ADSRDisplay::paint(juce::Graphics& g)
{
    float margin = 30.f;
    
    auto drawableBounds = getLocalBounds().reduced(margin);
    
    g.setColour(juce::Colour::fromRGB(40, 40, 40));
    g.fillRoundedRectangle(0, 0, getWidth(), getHeight(), margin / 2.f);
    
    float attackX = drawableBounds.getX() + 100 * attackPercent;
    float decayX = 100 * decayPercent;
    float sustainY = drawableBounds.getY() + drawableBounds.getHeight() * (1.f - sustainPercent);
    float releaseX = drawableBounds.getX() + 0.8 * drawableBounds.getWidth();
    float maxReleaseWidth = 0.2 * drawableBounds.getWidth();
    float lineThickness = 2.4f;
    
    g.setColour(juce::Colours::red);
    
    g.drawLine(drawableBounds.getX(), drawableBounds.getY() + drawableBounds.getHeight(), attackX, drawableBounds.getY(), lineThickness);
    g.drawLine(attackX, drawableBounds.getY(), attackX + decayX, sustainY, lineThickness);
    g.drawLine(attackX + decayX, sustainY, releaseX, sustainY, lineThickness);
    g.drawLine(releaseX, sustainY, releaseX + releasePercent * maxReleaseWidth, drawableBounds.getY() + drawableBounds.getHeight(), lineThickness);
    
    
}

void ADSRDisplay::setAttackPercent(float v)
{
    this->attackPercent = v;
    repaint();
}
void ADSRDisplay::setDecayPercent(float v)
{
    this->decayPercent = v;
    repaint();
}
void ADSRDisplay::setSustainPercent(float v)
{
    this->sustainPercent = v;
    repaint();
}
void ADSRDisplay::setReleasePercent(float v)
{
    this->releasePercent = v;
    repaint();
    std::cout << v << std::endl;
}
