#include "ADSR.h"

void ADSR::setSampleRate(double sampleRate)
{
    this->sampleRate = sampleRate;
}

void ADSR::setParameters(float attack, float decay, float sustain, float release)
{
    this->attack = attack;
    this->decay = decay;
    this->sustain = sustain;
    this->release = release;
}

float ADSR::getNextSample()
{
    float expCoefficient = 1.f / 0.6f;
    
    switch (state)
    {
        case State::Attack:
            
            currentLevel = std::powf(timeInState / sampleRate / attack, expCoefficient);
            
            timeInState++;

            if (currentLevel >= 1.f)
            {
                timeInState = 0;
                currentLevel = 1.f;
                state = State::Decay;
            }
            break;
            
        case State::Decay:
            
            currentLevel = 1.f - std::powf(timeInState / sampleRate / decay, 1.f / expCoefficient);

            timeInState++;
            if (currentLevel <= sustain)
            {
                timeInState = 0;
                currentLevel = sustain;
                state = State::Sustain;
            }
            break;
            
        case State::Sustain:
            // constant
            break;
            
        case State::Release:
            
            currentLevel = sustain - sustain * std::powf(timeInState / sampleRate / release, 1.f / expCoefficient);

            timeInState++;
            
            if (currentLevel <= 1e-10f)
            {
                timeInState = 0;
                currentLevel = 0.f;
                state = State::Idle;
            }
            break;
        
        case State::Idle:
            break;
    }
    
    
    return currentLevel;
}


void ADSR::noteOn()
{
    state = State::Attack;
    currentLevel = 0.f;
}

void ADSR::noteOff()
{
    state = State::Release;
}

bool ADSR::isIdling()
{
    return state == State::Idle;
}

bool ADSR::isDecaying()
{
    return state == State::Decay;
}

bool ADSR::isSustaining()
{
    return state == State::Sustain;
}

bool ADSR::isReleasing()
{
    return state == State::Release;
}

float ADSR::getCurrentLevel()
{
    return currentLevel;
}

std::string ADSR::getStateString()
{
    return  state == State::Idle ? "Idle" :
            state == State::Attack ? "Attack" :
            state == State::Decay ? "Decay" :
            state == State::Sustain ? "Sustain" : "Release";
}
