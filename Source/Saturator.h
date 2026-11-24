
#pragma once

#include "JuceHeader.h"

class Saturator
{
public:
    Saturator() {};
    ~Saturator() {};
    
    void setDrive(float drive);
    
    float processSample(float input);
    
private:
    float drive = 1.f;
};
