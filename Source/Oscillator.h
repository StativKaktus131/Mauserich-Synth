#pragma once

#include "JuceHeader.h"


class Oscillator
{
public:
    double lastNote = 69.0, targetNote = 69.0, currentNote = 69.0;
    Oscillator() = default;
    ~Oscillator() = default;
    
    void setSampleRate(double sampleRate);
    float getNextSample(double detune, double pitchEnvOff);
    void changeGlideValue(float glide);
    void target(double note);
    void setCurrent(double note);
    void setLast(double note);
    void glide();
    void reset();
    void resetPhases();
    
private:
    double phases[5] = {0.0, 0.0, 0.0, 0.0, 0.0};
    double phaseIncrements[5] = {0.0, 0.0, 0.0, 0.0, 0.0};
    double sampleRate;
    float lastFilteredSample = 0.0;
    
    double glideDelta = 0.f;
};
