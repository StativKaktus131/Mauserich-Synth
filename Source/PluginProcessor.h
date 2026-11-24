#pragma once

#include <JuceHeader.h>


#include "Oscillator.h"
#include "ADSR.h"
#include "LowpassFilter.h"
#include "Saturator.h"






class JohnSynthAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    JohnSynthAudioProcessor();
    ~JohnSynthAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    
    void changeGlideValue();
    
    Oscillator oscillator;
    LowpassFilter lpf;
    ADSR adsr;
    Saturator saturator;
    
    juce::AudioProcessorValueTreeState vts;
    
private:
    std::unique_ptr<juce::AudioParameterFloat> createParameter(const juce::String& pId, const juce::String& name, juce::NormalisableRange<float> range, float defaultValue, const juce::String& suffix);
    std::unique_ptr<juce::AudioParameterFloat> createParameter(const juce::String& pId, const juce::String& name, float min, float max, float defaultValue, const juce::String& suffix);
    
    juce::Random random;
    float lastFilteredNoiseSample = 0.f;
    int pitchEnvSamples = 0;
    
    double currentSampleRate = 48000.0;
    double noteFrequency = 440.0;
    double glideIncrementPerSample = 0.0;
    std::vector<int> pressedMidiNotes;

    
    std::atomic<float>* cutoffParameter;
    std::atomic<float>* resonanceParameter;
    std::atomic<float>* glideParameter;
    std::atomic<float>* gainParameter;
    
    std::atomic<float>* attackParameter;
    std::atomic<float>* decayParameter;
    std::atomic<float>* sustainParameter;
    std::atomic<float>* releaseParameter;
    std::atomic<float>* saturationParameter;
    
    std::atomic<float>* detuneParameter;
    std::atomic<float>* noiseMixParameter;
    std::atomic<float>* pitchEnvTimeParameter;
    std::atomic<float>* pitchEnvDepthParameter;
    std::atomic<float>* cheeseParameter;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (JohnSynthAudioProcessor)
};
