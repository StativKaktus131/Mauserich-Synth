#include "Saturator.h"

void Saturator::setDrive(float drive)
{
    this->drive = drive;
}

float Saturator::processSample(float input)
{
    float saturated = input * drive;
    saturated = std::clamp(saturated, -1.f, 1.f);
    return saturated;
}
