#pragma once

#include "JuceHeader.h"
#include "Container.h"

class SceneComponent : public juce::Component
{
public:
    SceneComponent();
    ~SceneComponent() = default;
    void resized() override;
    
    enum class Type { Filter, Style, PitchEnv, Control, ADSR };
    void addAndMakeVisibleTo(Type type, juce::Component* component);
    Container* getContainer(Type type);
    
private:
    const float spacing = 10.f;
    Container filterContainer, styleContainer, pitchEnvContainer, controlContainer, ADSRContainer;
};
