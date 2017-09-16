#include "cpuzone.h"

CpuZone::CpuZone(long long energyCounter, long long powerCounter)
        :energyCounter(energyCounter), 
         powerCounter(powerCounter){}

CpuZone::CpuZone()
        :energyCounter(0L),
         powerCounter(0L) {}

std::string CpuZone::toString(std::string separator) const 
{
    return std::to_string(this->energyCounter) + separator + std::to_string(this->powerCounter);
}


std::string CpuZone::getHeader(std::string separator) const 
{
    return "energyCounter (mJ)" + separator + "microPower (mW)";
}

json CpuZone::toJSON() const 
{
    json result;
    result["EnergyCounter"] = this->energyCounter;
    result["PowerCounter"] = this->powerCounter;
    return result;
}
 
const CpuZone* CpuZone::clone() const 
{
    return new CpuZone(this->energyCounter, this->powerCounter);
}

const CpuZone* CpuZone::process(const CpuZone* prev, double timeDiff) const 
{
    const CpuZone* result = NULL;
    if (prev) {
        if (timeDiff == 0.0) 
        {
            throw std::logic_error("Division by 0, provide different measurement results");
        } 
        else 
        {
            long long energyCounter = (this->energyCounter - prev->energyCounter) / timeDiff;
            long long powerCounter = this->powerCounter;
            result = new CpuZone(energyCounter, powerCounter);
        }
    }
    return result;
}


void CpuZone::insert(const CpuZone* cpuZone) const 
{
    
}

CpuZone::~CpuZone() {};