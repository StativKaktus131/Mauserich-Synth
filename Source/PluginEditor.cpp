#include "PluginProcessor.h"
#include "PluginEditor.h"

void JohnSynthAudioProcessorEditor::initializeParameter(juce::Slider* slider, std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>* attachment, const juce::String& identifier, SceneComponent::Type type)
{
    sceneComponent.addAndMakeVisibleTo(type, slider);
    slider->setSliderStyle(juce::Slider::RotaryVerticalDrag);
    slider->setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    slider->setNumDecimalPlacesToDisplay(2);
    *attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.vts, identifier, *slider);
}

void JohnSynthAudioProcessorEditor::initializeADSRParameter(juce::Slider* slider, std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>* attachment, const juce::String& identifier, SceneComponent::Type type)
{
    sceneComponent.addAndMakeVisibleTo(type, slider);
    slider->setPaintingIsUnclipped(true);
    slider->setSliderStyle(juce::Slider::LinearVertical);
    slider->setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    slider->setNumDecimalPlacesToDisplay(2);
    *attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.vts, identifier, *slider);
}

void JohnSynthAudioProcessorEditor::initializeLabel(const juce::String& text, juce::Label* label, SceneComponent::Type type)
{
    label->setText(text, juce::dontSendNotification);
    sceneComponent.addAndMakeVisibleTo(type, label);
}

//==============================================================================
JohnSynthAudioProcessorEditor::JohnSynthAudioProcessorEditor (JohnSynthAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), sceneComponent()
{
    using Type = SceneComponent::Type;
    
    initializeParameter(&filterCutoffSlider, &filterCutoffAttachment, "filter_cutoff", Type::Filter);
    initializeLabel("Cutoff Frequency", &filterCutoffLabel, Type::Filter);
    filterCutoffSlider.setTextValueSuffix("Hz");
    
    initializeParameter(&filterResonanceSlider, &filterResonanceAttachment, "filter_resonance", Type::Filter);
    initializeLabel("Resonance", &filterResonanceLabel, Type::Filter);
    
    
    initializeParameter(&detuneSlider, &detuneAttachment, "detune", Type::Style);
    initializeLabel("Detune", &detuneLabel, Type::Style);
    
    initializeParameter(&glideSlider, &glideAttachment, "glide", Type::Style);
    initializeLabel("Glide", &glideLabel, Type::Style);
    
    initializeParameter(&saturationSlider, &saturationAttachment, "saturation", Type::Style);
    initializeLabel("Saturation", &saturationLabel, Type::Style);

    initializeParameter(&pitchEnvTimeSlider, &pitchEnvTimeAttachment, "pitch_env_time", Type::PitchEnv);
    initializeLabel("Time", &pitchEnvTimeLabel, Type::PitchEnv);
    
    initializeParameter(&pitchEnvDepthSlider, &pitchEnvDepthAttachment, "pitch_env_depth", Type::PitchEnv);
    initializeLabel("Depth", &pitchEnvDepthLabel, Type::PitchEnv);

    initializeParameter(&cheeseSlider, &cheeseAttachment, "cheese", Type::Control);
    initializeLabel("Cheese", &cheeseLabel, Type::Control);
    cheeseSlider.setName("cheese");
    
    initializeParameter(&noiseMixSlider, &noiseMixAttachment, "noise_mix", Type::Control);
    initializeLabel("Noise Mix", &noiseMixLabel, Type::Control);
    
    initializeParameter(&gainSlider, &gainAttachment, "gain", Type::Control);
    initializeLabel("Master Volume", &gainLabel, Type::Control);

    
    initializeADSRParameter(&attackSlider, &attackAttachment, "attack", Type::ADSR);
    initializeLabel("A", &attackLabel, Type::ADSR);
    attackSlider.onValueChange = [this] { adsrDisplay.setAttackPercent(attackSlider.getValue() / 2.f); };
    
    initializeADSRParameter(&decaySlider, &decayAttachment, "decay", Type::ADSR);
    initializeLabel("D", &decayLabel, Type::ADSR);
    decaySlider.onValueChange = [this] { adsrDisplay.setDecayPercent(decaySlider.getValue() / 2.f); };

    initializeADSRParameter(&sustainSlider, &sustainAttachment, "sustain", Type::ADSR);
    initializeLabel("S", &sustainLabel, Type::ADSR);
    sustainSlider.onValueChange = [this] { adsrDisplay.setSustainPercent(sustainSlider.getValue()); };

    initializeADSRParameter(&releaseSlider, &releaseAttachment, "release", Type::ADSR);
    initializeLabel("R", &releaseLabel, Type::ADSR);
    releaseSlider.onValueChange = [this] { adsrDisplay.setReleasePercent(releaseSlider.getValue() / 2.f); };

    
    glideSlider.onValueChange = [this] { audioProcessor.changeGlideValue(); };
    
    setSize (1000, 700);
    
    backgroundImage = juce::ImageFileFormat::loadFrom(BinaryData::background_png, BinaryData::background_pngSize);
    dialImage = juce::ImageFileFormat::loadFrom(BinaryData::dial_png, BinaryData::dial_pngSize);
    faderImage = juce::ImageFileFormat::loadFrom(BinaryData::fader_blue_png, BinaryData::fader_blue_pngSize);
    cheeseImage = juce::ImageFileFormat::loadFrom(BinaryData::cheese_png, BinaryData::cheese_pngSize);

    mlaf.setDialImage(&dialImage);
    mlaf.setFaderImage(&faderImage);
    mlaf.setCheeseImage(&cheeseImage);
    
    
    addAndMakeVisible(sceneComponent);
    
    
    adsrDisplay.setAttackPercent(attackSlider.getValue() / 2.f);
    adsrDisplay.setDecayPercent(decaySlider.getValue() / 2.f);
    adsrDisplay.setSustainPercent(sustainSlider.getValue());
    adsrDisplay.setReleasePercent(releaseSlider.getValue() / 2.f);
    sceneComponent.getContainer(SceneComponent::Type::ADSR)->addAndMakeVisible(adsrDisplay);
    setLookAndFeel(&mlaf);
    
}

JohnSynthAudioProcessorEditor::~JohnSynthAudioProcessorEditor()
{
    setLookAndFeel(&juce::LookAndFeel::getDefaultLookAndFeel());
}

//==============================================================================
void JohnSynthAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.drawImage(backgroundImage, 0, 0, getWidth(), getHeight(), 0, 0, backgroundImage.getWidth(), backgroundImage.getHeight());
}

void JohnSynthAudioProcessorEditor::resized()
{
    sceneComponent.setBounds(getLocalBounds());
    float margin = 10.f;
    float labelHeight = mlaf.getLabelHeight();
    
    Container* filterContainer = sceneComponent.getContainer(SceneComponent::Type::Filter);
    Container* styleContainer = sceneComponent.getContainer(SceneComponent::Type::Style);
    Container* pitchEnvContainer = sceneComponent.getContainer(SceneComponent::Type::PitchEnv);
    Container* controlContainer = sceneComponent.getContainer(SceneComponent::Type::Control);
    Container* ADSRContainer = sceneComponent.getContainer(SceneComponent::Type::ADSR);
    
    auto filterContainerLocalBounds = filterContainer->getLocalBounds().reduced(margin);
    auto styleContainerLocalBounds = styleContainer->getLocalBounds().reduced(margin);
    auto pitchEnvContainerLocalBounds = pitchEnvContainer->getLocalBounds().reduced(margin);
    auto controlContainerLocalBounds = controlContainer->getLocalBounds().reduced(margin);
    auto ADSRContainerLocalBounds = ADSRContainer->getLocalBounds().reduced(margin);
    
    float fUpperAvailableSpace = filterContainerLocalBounds.getHeight() / 8.f * 5.f;
    float fLowerAvailableSpace = filterContainerLocalBounds.getHeight() / 8.f * 3.f;
    float sThird = filterContainerLocalBounds.getHeight() / 3.f;
    float pHalf = pitchEnvContainerLocalBounds.getHeight() / 2.f;
    float cMore = controlContainerLocalBounds.getHeight() / 8.f * 3.f;
    float cLess = controlContainerLocalBounds.getHeight() / 8.f * 2.f;
    float adsrElementWidth = 60.f;
    
    
    filterCutoffLabel.setBounds(filterContainerLocalBounds.removeFromTop(labelHeight));
    filterCutoffSlider.setBounds(filterContainerLocalBounds.removeFromTop(fUpperAvailableSpace - labelHeight));
    filterResonanceSlider.setBounds(filterContainerLocalBounds.removeFromBottom(fLowerAvailableSpace - labelHeight));
    filterResonanceLabel.setBounds(filterContainerLocalBounds.removeFromBottom(labelHeight));

    
    detuneLabel.setBounds(styleContainerLocalBounds.removeFromTop(labelHeight));
    detuneSlider.setBounds(styleContainerLocalBounds.removeFromTop(sThird - labelHeight));
    glideLabel.setBounds(styleContainerLocalBounds.removeFromTop(labelHeight));
    glideSlider.setBounds(styleContainerLocalBounds.removeFromTop(sThird - labelHeight));
    saturationLabel.setBounds(styleContainerLocalBounds.removeFromTop(labelHeight));
    saturationSlider.setBounds(styleContainerLocalBounds.removeFromTop(sThird - labelHeight));
    
    
    pitchEnvTimeLabel.setBounds(pitchEnvContainerLocalBounds.removeFromTop(labelHeight));
    pitchEnvTimeSlider.setBounds(pitchEnvContainerLocalBounds.removeFromTop(pHalf - labelHeight));
    pitchEnvDepthLabel.setBounds(pitchEnvContainerLocalBounds.removeFromTop(labelHeight));
    pitchEnvDepthSlider.setBounds(pitchEnvContainerLocalBounds.removeFromTop(pHalf - labelHeight));
    
    
    cheeseLabel.setBounds(controlContainerLocalBounds.removeFromTop(labelHeight));
    cheeseSlider.setBounds(controlContainerLocalBounds.removeFromTop(cMore - labelHeight));
    noiseMixLabel.setBounds(controlContainerLocalBounds.removeFromTop(labelHeight));
    noiseMixSlider.setBounds(controlContainerLocalBounds.removeFromTop(cMore - labelHeight));
    gainLabel.setBounds(controlContainerLocalBounds.removeFromTop(labelHeight));
    gainSlider.setBounds(controlContainerLocalBounds.removeFromTop(cLess - labelHeight));
    
    
    auto currBounds = ADSRContainerLocalBounds.removeFromLeft(adsrElementWidth);
    attackLabel.setBounds(currBounds.removeFromTop(labelHeight));
    attackSlider.setBounds(currBounds);
    
    currBounds = ADSRContainerLocalBounds.removeFromLeft(adsrElementWidth);
    decayLabel.setBounds(currBounds.removeFromTop(labelHeight));
    decaySlider.setBounds(currBounds);
    
    currBounds = ADSRContainerLocalBounds.removeFromLeft(adsrElementWidth);
    sustainLabel.setBounds(currBounds.removeFromTop(labelHeight));
    sustainSlider.setBounds(currBounds);

    currBounds = ADSRContainerLocalBounds.removeFromLeft(adsrElementWidth);
    releaseLabel.setBounds(currBounds.removeFromTop(labelHeight));
    releaseSlider.setBounds(currBounds);
    
    ADSRContainerLocalBounds.removeFromLeft(40);
    adsrDisplay.setBounds(ADSRContainerLocalBounds);
}
