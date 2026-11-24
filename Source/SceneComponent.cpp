#include "SceneComponent.h"

SceneComponent::SceneComponent()
{
    addAndMakeVisible(filterContainer);
    addAndMakeVisible(styleContainer);
    addAndMakeVisible(pitchEnvContainer);
    addAndMakeVisible(controlContainer);
    addAndMakeVisible(ADSRContainer);
    
    filterContainer.setDisplayText("Filter");
    styleContainer.setDisplayText("Style");
    pitchEnvContainer.setDisplayText("Pitch Envelope");
    controlContainer.setDisplayText("Control");
    ADSRContainer.setDisplayText("ADSR");
}

void SceneComponent::resized()
{
    auto bounds = getLocalBounds();
    auto ADSRBounds = bounds.removeFromBottom(200).reduced(spacing);
    bounds.removeFromTop(spacing);
    bounds.removeFromLeft(spacing);
    bounds.removeFromBottom(47.f);
    
    auto width = bounds.getWidth() / 4 - spacing;
    
    ADSRBounds.removeFromRight(width + spacing);
    ADSRContainer.setBounds(ADSRBounds);
    
    filterContainer.setBounds(bounds.removeFromLeft(width));
    bounds.removeFromLeft(spacing);
    
    styleContainer.setBounds(bounds.removeFromLeft(width));
    bounds.removeFromLeft(spacing);

    pitchEnvContainer.setBounds(bounds.removeFromLeft(width));
    bounds.removeFromLeft(spacing);

    bounds.removeFromBottom(70.f);
    controlContainer.setBounds(bounds.removeFromLeft(width));
}

void SceneComponent::addAndMakeVisibleTo(Type type, juce::Component* component)
{
    Component* comp = nullptr;
    switch (type) {
        case Type::Filter:
            comp = &filterContainer;
            break;
            
        case Type::Style:
            comp = &styleContainer;
            break;
            
        case Type::PitchEnv:
            comp = &pitchEnvContainer;
            break;
            
        case Type::Control:
            comp = &controlContainer;
            break;
            
        case Type::ADSR:
            comp = &ADSRContainer;
            break;

        default:
            return;
    }
    
    comp->addAndMakeVisible(component);
}

Container* SceneComponent::getContainer(Type type)
{
    switch (type) {
        case Type::Filter:
            return &filterContainer;
            
        case Type::Style:
            return &styleContainer;
            
        case Type::PitchEnv:
            return &pitchEnvContainer;
            
        case Type::Control:
            return &controlContainer;
            
        case Type::ADSR:
            return &ADSRContainer;
            
        default:
            return nullptr;
    }
}
