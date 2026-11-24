#include "PluginProcessor.h"
#include "PluginEditor.h"

JohnSynthAudioProcessor::JohnSynthAudioProcessor()
    :vts(*this, nullptr, "Parameters", createParameterLayout())
{
    cutoffParameter = vts.getRawParameterValue("filter_cutoff");
    resonanceParameter = vts.getRawParameterValue("filter_resonance");
    glideParameter = vts.getRawParameterValue("glide");
    gainParameter = vts.getRawParameterValue("gain");
    
    attackParameter = vts.getRawParameterValue("attack");
    decayParameter = vts.getRawParameterValue("decay");
    sustainParameter = vts.getRawParameterValue("sustain");
    releaseParameter = vts.getRawParameterValue("release");
    saturationParameter = vts.getRawParameterValue("saturation");
    
    detuneParameter = vts.getRawParameterValue("detune");
    noiseMixParameter = vts.getRawParameterValue("noise_mix");
    pitchEnvTimeParameter = vts.getRawParameterValue("pitch_env_time");
    pitchEnvDepthParameter = vts.getRawParameterValue("pitch_env_depth");
    cheeseParameter = vts.getRawParameterValue("cheese");
    
}

JohnSynthAudioProcessor::~JohnSynthAudioProcessor(){}

juce::AudioProcessorValueTreeState::ParameterLayout JohnSynthAudioProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    
    
    juce::NormalisableRange<float> filterFrequencyRange = { 70.f, 20000.f, 0.01f, 1.f / std::log2(1.f + std::sqrt(20000.f / 70.f)) };
    
    layout.add(createParameter("filter_cutoff", "Filter Cutoff", filterFrequencyRange, 20000.f, "Hz"));
    layout.add(createParameter("filter_resonance", "Filter Resonance", 0.f, 0.99f, 0.f, ""));
    layout.add(createParameter("glide", "Glide", 0.f, 0.8f, 0.f, ""));
    layout.add(createParameter("attack", "Attack", 0.f, 2.f, 0.01f, ""));
    layout.add(createParameter("decay", "Decay", 0.0f, 2.f, 0.1f, ""));
    layout.add(createParameter("sustain", "Sustain", 0.f, 1.f, 0.8f, ""));
    layout.add(createParameter("release", "Release", 0.f, 2.f, 0.1f, ""));
    layout.add(createParameter("saturation", "Saturation", 0.f, 1.f, 0.f, ""));
    layout.add(createParameter("gain", "Gain", -100.f, 0.f, -6.f, "dB"));
    
    layout.add(createParameter("detune", "Detune", 0.f, 1.f, 0.f, ""));
    layout.add(createParameter("noise_mix", "Noise Mix", -100.f, 0.f, -100.f, "dB"));
    layout.add(createParameter("pitch_env_time", "Pitch Envelope Time", 0.f, 2.f, 0.f, ""));
    layout.add(createParameter("pitch_env_depth", "Pitch Envelope Depth", -12.f, 12.f, 0.f, ""));
    layout.add(createParameter("cheese", "Cheese", 0.f, 1.f, 0.f, ""));
    return layout;
}


void JohnSynthAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    random = juce::Random();
    currentSampleRate = sampleRate;
    adsr.setSampleRate(currentSampleRate);
    oscillator.setSampleRate(currentSampleRate);
}

void JohnSynthAudioProcessor::changeGlideValue()
{
    oscillator.changeGlideValue(*glideParameter);
}


void JohnSynthAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    buffer.clear();
    
    float glide = *glideParameter;
    
    
    
    for (auto data : midiMessages)
    {
        // get midi info
        juce::MidiMessage currentMidi = data.getMessage();
        int midiNote = currentMidi.getNoteNumber();
        
        if (currentMidi.isNoteOn())
        {
            pressedMidiNotes.push_back(midiNote);
            
            oscillator.target(midiNote);
            
            // first note / not gliding
            if (glide == 0.f || pressedMidiNotes.size() == 1) {
                adsr.noteOn();
                oscillator.resetPhases();
                oscillator.setCurrent(oscillator.targetNote);
                
                pitchEnvSamples = 0;
            }
            
            oscillator.setLast(oscillator.currentNote);
            
            oscillator.changeGlideValue(glide);
            
            
        }
        if (currentMidi.isNoteOff())
        {
            // look for note in list and erase if found
            int idx = -1;
            for (int i = 0; i < pressedMidiNotes.size(); i++)
                if (pressedMidiNotes[i] == currentMidi.getNoteNumber())
                    idx = i;
            
            if (idx >= 0)
                pressedMidiNotes.erase(pressedMidiNotes.begin() + idx);
            
            
            // target last note
            if (pressedMidiNotes.size() > 0)
            {
                
                oscillator.target(pressedMidiNotes[pressedMidiNotes.size() - 1]);
                
                if (glide == 0.f)
                {
                    // if released note is playing, play last one
                    if (midiNote == oscillator.currentNote)
                        adsr.noteOn();
                    
                    oscillator.setCurrent(oscillator.targetNote);
                }
                    
                oscillator.setLast(oscillator.currentNote);
                
                changeGlideValue();
            }
            else
            {
                oscillator.reset();
                changeGlideValue();
                adsr.noteOff();
            }
        }
    }
    

    // Update parameters
    float cutoff = *cutoffParameter;
    float resonance = *resonanceParameter;
    float attack = *attackParameter;
    float decay = *decayParameter;
    float sustain = *sustainParameter;
    float release = *releaseParameter;
    float saturation = *saturationParameter;
    float gain = *gainParameter;
    float detune = *detuneParameter;
    float noiseDecibels = *noiseMixParameter;
    float noise = juce::Decibels::decibelsToGain(noiseDecibels);
    float pitchEnvTime = *pitchEnvTimeParameter;
    float pitchEnvDepth = *pitchEnvDepthParameter;
    float cheese = *cheeseParameter;
    
    
    float level = juce::Decibels::decibelsToGain(gain);
    
    adsr.setParameters(attack, decay, sustain, release);
    lpf.setCoefficients(cutoff, resonance, currentSampleRate);
    saturator.setDrive(1.0f + saturation * 5.0f); // Scale for better effect
    
    for (int sample = 0; sample < buffer.getNumSamples(); sample++)
    {
        float pitchEnv = 1.f - std::pow(pitchEnvSamples / currentSampleRate / pitchEnvTime, 0.6f);
        
        if (pitchEnv >= 0.f)
            pitchEnvSamples++;
        else
            pitchEnv = 0.f;
        
        oscillator.glide();
        

        float env = adsr.getNextSample();
        float saw = oscillator.getNextSample(detune, pitchEnv * pitchEnvDepth);
        
        // "pink" noise, coeff should be the same as in oscillator
        float noiseFilterCoeff = 0.15f;
        float unfilteredNoiseSample = (random.nextFloat() * 2.f - 1.f);
        float noiseSample = lastFilteredNoiseSample + noiseFilterCoeff * (unfilteredNoiseSample - lastFilteredNoiseSample);
        lastFilteredNoiseSample = noiseSample;
        noiseSample *= noise;
        
        saw += noiseSample;
        
        saw = saturator.processSample(saw);
        saw = lpf.processSample(saw);

        float cheeseSample = std::powf(cheese, 1.f / 0.6f) * 5 * saw;
        float outputSample = (1.f - cheese) * saw + cheese * std::tanhf(std::pow(std::sin(cheeseSample / juce::MathConstants<float>::pi), 2.f));
        
        for (int channel = 0; channel < buffer.getNumChannels(); channel++)
            buffer.getWritePointer(channel)[sample] = std::clamp(outputSample * env, -1.f, 1.f) * level;
        
        
    }
}



// Helper functions to create parameters to add to the layout

std::unique_ptr<juce::AudioParameterFloat> JohnSynthAudioProcessor::createParameter(const juce::String& pId, const juce::String& name, juce::NormalisableRange<float> range, float defaultValue, const juce::String& suffix)
{
    return std::make_unique<juce::AudioParameterFloat>(juce::ParameterID {pId, 1} , name, range, defaultValue, suffix);
};

std::unique_ptr<juce::AudioParameterFloat> JohnSynthAudioProcessor::createParameter(const juce::String& pId, const juce::String& name, float min, float max, float defaultValue, const juce::String& suffix)
{
    return createParameter(pId, name, juce::NormalisableRange<float>(min, max), defaultValue, suffix);
};








//==============================================================================
const juce::String JohnSynthAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool JohnSynthAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool JohnSynthAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool JohnSynthAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double JohnSynthAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int JohnSynthAudioProcessor::getNumPrograms()
{
    return 1;
}

int JohnSynthAudioProcessor::getCurrentProgram()
{
    return 0;
}

void JohnSynthAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String JohnSynthAudioProcessor::getProgramName (int index)
{
    return {};
}

void JohnSynthAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

void JohnSynthAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool JohnSynthAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

//==============================================================================
bool JohnSynthAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* JohnSynthAudioProcessor::createEditor()
{
    return new JohnSynthAudioProcessorEditor (*this);
}

//==============================================================================
void JohnSynthAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = vts.copyState();
    std::unique_ptr<juce::XmlElement> xml (state.createXml());
    copyXmlToBinary(*xml, destData);
}

void JohnSynthAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary(data, sizeInBytes));
    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(vts.state.getType()))
            vts.replaceState(juce::ValueTree::fromXml(*xmlState));
}


juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new JohnSynthAudioProcessor();
}
