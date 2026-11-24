#pragma once

#include "JuceHeader.h"

class ADSR
{
public:
    ADSR() = default;
    ~ADSR() = default;
    
    enum class State { Idle, Attack, Decay, Sustain, Release };
    
    void setSampleRate(double sampleRate);
    
    void setParameters(float attack, float decay, float sustain, float release);
    
    void noteOn();
    void noteOff();
    
    float getNextSample();
    
    bool isIdling();
    bool isAttacking();
    bool isDecaying();
    bool isSustaining();
    bool isReleasing();
    
    
    float getCurrentLevel();
    
    std::string getStateString();
    State getState();


private:
    
    State state = State::Idle;
    
    double sampleRate = 48000.f;
    float attack = 0.1f, decay = 0.1f, sustain = 0.8f, release = 0.1f;
    float currentLevel = 0.0f;
    int timeInState = 0;
};

