#include "Oscillator.h"

void Oscillator::setSampleRate(double sampleRate)
{
    this->sampleRate = sampleRate;
}

float Oscillator::getNextSample(double detune, double pitchEnvOff)
{
    float samples[5];
    
    for (int i = 0; i < 5; i++)
    {
        samples[i] = (phases[i] / juce::MathConstants<double>::twoPi) * 2.f - 1.f;
        float detuneValue = (i / 5.f - 0.5f) * detune;
        float frequency = 440.0 * std::pow(2.0, (currentNote + pitchEnvOff + detuneValue - 69.0) / 12.0);
        phases[i] += juce::MathConstants<double>::twoPi * (frequency / sampleRate);
        phases[i] = std::fmod(phases[i], juce::MathConstants<double>::twoPi);
    }
    
    float mixedSample = 0.f;
    
    for (int i = 0; i < 5; i++)
        mixedSample += samples[i];
    
    mixedSample /= 5.f;

    // onepole for spectral tilt
    float coefficient = 0.15f;
    float filteredSample = lastFilteredSample + coefficient * (mixedSample - lastFilteredSample);
    lastFilteredSample = filteredSample;
    
    return filteredSample;
}

void Oscillator::changeGlideValue(float glide)
{
    float sampleIncrease = (targetNote - lastNote) / sampleRate;
    glideDelta = glide == 0.0 ? 0.0 : (1.f / glide) * sampleIncrease;
}

void Oscillator::target(double note)
{
    targetNote = note;
}

void Oscillator::setCurrent(double note)
{
    currentNote = note;
}

void Oscillator::setLast(double note)
{
    lastNote = note;
}

void Oscillator::glide()
{
    bool lastSign = std::signbit(currentNote - targetNote);
    currentNote += glideDelta;
    bool currentSign = std::signbit(currentNote - targetNote);
    
    if (lastSign != currentSign)
    {
        currentSign = lastNote = targetNote;
        glideDelta = 0.0;
    }
}

void Oscillator::reset()
{
    targetNote = lastNote = currentNote;
}

void Oscillator::resetPhases()
{
    for (int i = 0; i < 5; i++)
        phases[i] = 0.0;
}
