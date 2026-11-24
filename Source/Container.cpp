#include "Container.h"

void Container::paint(juce::Graphics &g)
{
    setPaintingIsUnclipped(true);
    
    g.setColour(juce::Colours::white);
    
    g.drawLine(0, 0, 0, getHeight());
    g.drawLine(0, getHeight(), getWidth(), getHeight());
    g.drawLine(0, 0, textMargin - 2.f, 0);
    g.drawLine(textMargin + textLayout.getWidth() + 2.f, 0.f, getWidth(), 0.f);
    
    g.drawLine(getWidth(), 0, getWidth(), getHeight());
    
    textLayout.draw(g, juce::Rectangle<float>(textMargin, -TEXT_SIZE / 3.f, 0.f, 0.f));
}

void Container::setDisplayText(const juce::String &text)
{
    this->text = text;
    auto as = juce::AttributedString(text);
    auto font = fnt.withHeight(TEXT_SIZE);
    
    as.setFont(font);
    as.setColour(juce::Colours::white);
    
    
    textLayout = juce::TextLayout();
    textLayout.createLayout(as, 100);
}
