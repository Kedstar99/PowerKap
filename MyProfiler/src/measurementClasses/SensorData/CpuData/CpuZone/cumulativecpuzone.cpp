#include "cumulativecpuzone.h"

CumulativeCpuZone::CumulativeCpuZone(const CpuZone* cpuZone)
                  :energyCounterMax(0L),
                   energyCounterMin(0L),
                   energyTotal(0L),
                   energyCounterSampleSize(0),
                   energyCounterVariance(0),
                   energyCounterOldMean(0),
                   powerCounterMax(0L),
                   powerCounterMin(0L),
                   powerCounterSampleSize(0),
                   powerCounterVariance(0),
                   powerCounterOldMean(0)
{
    if (cpuZone) 
    {
        if (cpuZone->energyCounter > 0 && cpuZone->energyCounter < LONG_MAX) 
        {
            energyCounter = cpuZone->energyCounter;
            energyCounterMax = cpuZone->energyCounter;
            energyCounterMin = cpuZone->energyCounter;
            energyCounterOldMean = cpuZone->energyCounter;
            energyCounterSampleSize++;
        }
        if (cpuZone->powerCounter > 0 && cpuZone->powerCounter < LONG_MAX)
        {
            powerCounter = cpuZone->powerCounter;
            powerCounterMax = cpuZone->powerCounter;
            powerCounterMin = cpuZone->powerCounter;
            powerCounterOldMean = cpuZone->powerCounter;
            powerCounterSampleSize++;
        }
    } 
}

void CumulativeCpuZone::insert(const CpuZone* cpuZone) const 
{
    if (cpuZone) 
    {
        if (cpuZone->energyCounter > 0 && cpuZone->energyCounter < LONG_MAX) 
        {
            energyCounterSampleSize++;
            energyCounterMax = std::max(cpuZone->energyCounter, this->energyCounterMax);
            energyCounterMin = std::min(cpuZone->energyCounter, this->energyCounterMin);
            CpuZone::energyCounter += datahelper::computeRunningAverage(cpuZone->energyCounter,
                                                                        CpuZone::energyCounter, 
                                                                        energyCounterSampleSize);
            energyCounterVariance += datahelper::computeRunningVariance(energyCounterVariance, 
                                                                        cpuZone->energyCounter, 
                                                                        energyCounterOldMean, 
                                                                        CpuZone::energyCounter);
            energyCounterOldMean = CpuZone::energyCounter;
        }
        if (cpuZone->powerCounter > 0 && cpuZone->powerCounter < LONG_MAX)
        {
            powerCounterSampleSize++;
            powerCounterMax = std::max(cpuZone->powerCounter, this->powerCounterMax);
            powerCounterMin = std::min(cpuZone->powerCounter, this->powerCounterMin);
            CpuZone::powerCounter += datahelper::computeRunningAverage(cpuZone->powerCounter, 
                                                                       CpuZone::powerCounter, 
                                                                       powerCounterSampleSize);
            powerCounterVariance += datahelper::computeRunningVariance(powerCounterVariance, 
                                                                       cpuZone->energyCounter, 
                                                                       powerCounterOldMean, 
                                                                       CpuZone::powerCounter);
            powerCounterOldMean = CpuZone::powerCounter;
        }
    }
}

void CumulativeCpuZone::setEnergyTotal(const CumulativeCpuZone* prev) const
{
    if (prev) 
    {
        energyTotal = prev->energyTotal + CpuZone::energyCounter;
    }
}

std::string CumulativeCpuZone::getHeader(std::string separator) const 
{
    std::string tabHeader = datahelper::genHeader(separator);

    std::string result =  "Energy Counter(mJ):" + separator + tabHeader;
    result += "Total Energy(mj):" + separator;
    result += "Power(mJ):" + separator + tabHeader;
    return result;
}

std::string CumulativeCpuZone::toString(std::string separator) const 
{


    std::string energyCounterString = datahelper::createMaxMinAvgString(this->energyCounterMax, 
                                                                        this->energyCounterMin, 
                                                                        CpuZone::energyCounter, 
                                                                        this->energyCounterVariance, 
                                                                        this->energyCounterSampleSize, 
                                                                        separator);

    std::string powerCounterString = datahelper::createMaxMinAvgString(this->powerCounterMax,
                                                                       this->powerCounterMin,
                                                                       CpuZone::powerCounter,
                                                                       this->powerCounterVariance,
                                                                       this->powerCounterSampleSize,
                                                                       separator);
    //Energy Counter + separator 
    std::string result = " " + separator;
    result += energyCounterString + separator;
    //Total energy + separator
    result += std::to_string(this->energyTotal) + separator;
    //Power + separator
    result += " " + separator;
    result += powerCounterString + separator;
    return result;
}

json CumulativeCpuZone::toJSON() const
{
    json result;
    result["EnergyCounter"] = datahelper::createMaxMinAvgJSON(energyCounterMax,
                                                              energyCounterMin, 
                                                              CpuZone::energyCounter, 
                                                              datahelper::computeStandardDeviation(energyCounterVariance, 
                                                                                                   energyCounterSampleSize));
    result["PowerCounter"] = datahelper::createMaxMinAvgJSON(powerCounterMax, 
                                                             powerCounterMin, 
                                                             CpuZone::powerCounter, 
                                                             datahelper::computeStandardDeviation(powerCounterVariance, 
                                                                                                  powerCounterSampleSize));
    result["EnergyTotal"] = this->energyTotal;
    return result;
}

CumulativeCpuZone::~CumulativeCpuZone() {}