#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Sliders.h"
#include "MauserichLookAndFeel.h"
#include "SceneComponent.h"
#include "ADSRDisplay.h"

//==============================================================================
/**
*/
class JohnSynthAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    JohnSynthAudioProcessorEditor (JohnSynthAudioProcessor&);
    ~JohnSynthAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    JohnSynthAudioProcessor& audioProcessor;
    MauserichLookAndFeel mlaf;
    SceneComponent sceneComponent;
    ADSRDisplay adsrDisplay;
    
    
    void initializeParameter(juce::Slider* slider, std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>* attachment, const juce::String& identifier, SceneComponent::Type type);
    void initializeADSRParameter(juce::Slider* slider, std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>* attachment, const juce::String& identifier, SceneComponent::Type type);
    void initializeLabel(const juce::String& text, juce::Label* label, SceneComponent::Type type);
    
    // FILTER
    juce::Slider filterCutoffSlider;
    juce::Label filterCutoffLabel;
    
    juce::Slider filterResonanceSlider;
    juce::Label filterResonanceLabel;
    
    // STYLE
    PercentSlider detuneSlider;
    juce::Label detuneLabel;
    
    TimeSlider glideSlider;
    juce::Label glideLabel;
    
    PercentSlider saturationSlider;
    juce::Label saturationLabel;
    
    
    // PITCH ENVELOPE
    TimeSlider pitchEnvTimeSlider;
    juce::Label pitchEnvTimeLabel;
    
    juce::Slider pitchEnvDepthSlider;
    juce::Label pitchEnvDepthLabel;
    
    
    // CONTROL
    PercentSlider cheeseSlider;
    juce::Label cheeseLabel;
    
    DecibelSlider gainSlider;
    juce::Label gainLabel;
    
    DecibelSlider noiseMixSlider;
    juce::Label noiseMixLabel;
    
    
    // ADSR
    TimeSlider attackSlider;
    juce::Label attackLabel;
    
    TimeSlider decaySlider;
    juce::Label decayLabel;
    
    TimeSlider releaseSlider;
    juce::Label releaseLabel;
    
    PercentSlider sustainSlider;
    juce::Label sustainLabel;
    
    
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> filterCutoffAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> filterResonanceAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> glideAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> gainAttachment;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attackAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> decayAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sustainAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> releaseAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> saturationAttachment;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> detuneAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> noiseMixAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> pitchEnvTimeAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> pitchEnvDepthAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> cheeseAttachment;

    
    
    juce::Image backgroundImage;
    juce::Image dialImage, faderImage, cheeseImage;
    
    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (JohnSynthAudioProcessorEditor)
};
