#include "cumulativecpuinfo.h"

CumulativeCpuInfo::CumulativeCpuInfo(const CpuInfo* cpuInfo)
                  :energyCounterMax(0),
                   energyCounterMin(0),
                   energyTotal(0L),
                   energyCounterSampleSize(0),
                   energyCounterVariance(0),
                   energyCounterOldMean(0),
                   powerCounterMax(0),
                   powerCounterMin(0),
                   powerCounterSampleSize(0),
                   powerCounterVariance(0),
                   powerCounterOldMean(0),
                   CpuInfo() 
{
    if (cpuInfo) 
    {
        if (cpuInfo->energyCounter > 0 && cpuInfo->energyCounter < LONG_MAX) 
        {
            energyCounter = cpuInfo->energyCounter;
            energyCounterMax = cpuInfo->energyCounter;
            energyCounterMin = cpuInfo->energyCounter;
            energyCounterOldMean = cpuInfo->energyCounter;
            energyCounterSampleSize++;
        }

        if (cpuInfo->powerCounter > 0 && cpuInfo->powerCounter < LONG_MAX)
        {
            powerCounter = cpuInfo->powerCounter;
            powerCounterMax = cpuInfo->powerCounter;
            powerCounterMin = cpuInfo->powerCounter;
            powerCounterOldMean = cpuInfo->powerCounter;
            powerCounterSampleSize++;
        }
        name = cpuInfo->name;
        if (cpuInfo->core) 
        {
            CpuInfo::core = new CumulativeCpuZone(cpuInfo->core);
        }
        if (cpuInfo->uncore) 
        {
            CpuInfo::uncore = new CumulativeCpuZone(cpuInfo->uncore);
        }
        if (cpuInfo->dram) 
        {
            CpuInfo::dram = new CumulativeCpuZone(cpuInfo->dram);
        }
    }
}

void CumulativeCpuInfo::insert (const CpuInfo* cpuInfo) const 
{
    if (cpuInfo) 
    {
        if (cpuInfo->energyCounter > 0 && cpuInfo->energyCounter < LONG_MAX) 
        {
            energyCounterSampleSize++;
            energyCounterMax = std::max(cpuInfo->energyCounter, this->energyCounterMax);
            energyCounterMin = std::min(cpuInfo->energyCounter, this->energyCounterMin);
            CpuInfo::energyCounter += datahelper::computeRunningAverage(cpuInfo->energyCounter, 
                                                                        CpuInfo::energyCounter, 
                                                                        energyCounterSampleSize);
            energyCounterVariance += datahelper::computeRunningVariance(energyCounterVariance, 
                                                                        cpuInfo->energyCounter, 
                                                                        energyCounterOldMean, 
                                                                        CpuInfo::energyCounter);
            energyCounterOldMean = CpuInfo::energyCounter;
        }
        if (cpuInfo->powerCounter > 0 && cpuInfo->powerCounter < LONG_MAX)
        {
            powerCounterSampleSize++;
            powerCounterMax = std::max(cpuInfo->powerCounter, this->powerCounterMax);
            powerCounterMin = std::min(cpuInfo->powerCounter, this->powerCounterMin);
            CpuInfo::powerCounter += datahelper::computeRunningAverage(cpuInfo->powerCounter, 
                                                                       CpuInfo::powerCounter, 
                                                                       powerCounterSampleSize);
            powerCounterVariance += datahelper::computeRunningVariance(powerCounterVariance, 
                                                                       cpuInfo->powerCounter, 
                                                                       powerCounterOldMean, 
                                                                       CpuInfo::powerCounter);
            powerCounterOldMean = CpuInfo::powerCounter;
        }


        if (CpuInfo::core && cpuInfo->core) 
        {
            CpuInfo::core->insert(cpuInfo->core);
        }
        if (CpuInfo::uncore && cpuInfo->uncore) 
        {
            CpuInfo::uncore->insert(cpuInfo->uncore);
        }
        if (CpuInfo::core && cpuInfo->dram) 
        {
            CpuInfo::dram->insert(cpuInfo->dram);
        }

    }
}

void CumulativeCpuInfo::setEnergyTotal(const CumulativeCpuInfo* prev) const
{
    if (prev) 
    {
        this->energyTotal = prev->energyTotal + CpuInfo::energyCounter;

        auto core = dynamic_cast<const CumulativeCpuZone*>(CpuInfo::core);
        auto prevCore = dynamic_cast<const CumulativeCpuZone*>(prev->core);

        if (core && prevCore)
        {
            core->setEnergyTotal(prevCore);
        }

        auto uncore = dynamic_cast<const CumulativeCpuZone*>(CpuInfo::uncore);
        auto prevUncore = dynamic_cast<const CumulativeCpuZone*>(prev->uncore);

        if (uncore && prevUncore)
        {
            uncore->setEnergyTotal(prevUncore);
        }

        auto dram = dynamic_cast<const CumulativeCpuZone*>(CpuInfo::dram);
        auto prevDram = dynamic_cast<const CumulativeCpuZone*>(prev->dram);

        if (dram && prevDram)
        {
            dram->setEnergyTotal(prevDram);
        }
    }
}


std::string CumulativeCpuInfo::toString(std::string separator) const 
{
    std::string energyCounterString = datahelper::createMaxMinAvgString(this->energyCounterMax, 
                                                                        this->energyCounterMin, 
                                                                        CpuInfo::energyCounter, 
                                                                        this->energyCounterVariance, 
                                                                        this->energyCounterSampleSize, 
                                                                        separator);

    std::string powerCounterString = datahelper::createMaxMinAvgString(this->powerCounterMax,
                                                                       this->powerCounterMin,
                                                                       CpuInfo::powerCounter,
                                                                       this->powerCounterVariance,
                                                                       this->powerCounterSampleSize,
                                                                       separator);

    //name + separator
    std::string result = " " + separator;
    //energyCounter: + separator
    result += " " + separator;
    
    //max, min, avg
    result += energyCounterString + separator;
    
    result += std::to_string(this->energyTotal) + separator;
    
    //powerCounter + separator
    result += " " + separator;
    
    //max, min, avg
    result += powerCounterString + separator;
    if (CpuInfo::core) 
    {
        result += " " + separator;
        result += CpuInfo::core->toString(separator);
    }
    if (CpuInfo::uncore) 
    {
        result += " " + separator;
        result += CpuInfo::uncore->toString(separator);
    }
    if (CpuInfo::dram) 
    {
        result += " " + separator;
        result += CpuInfo::dram->toString(separator);
    }
    return result;
}

std::string CumulativeCpuInfo::getHeader(std::string separator) const 
{
    std::string tabHeader = datahelper::genHeader(separator);

    std::string result = "";
    result += this->name + separator;
    result += "Energy Counter(mJ):" + separator + tabHeader;
    result += "Energy Total(mJ):" + separator;
    result += "Power(mJ):" + separator + tabHeader;
    if (CpuInfo::core) 
    {
        result += "core:" + separator;
        result += CpuInfo::core->getHeader(separator);
    }
    if (CpuInfo::uncore) 
    {
        result += "uncore:" + separator;
        result += CpuInfo::uncore->getHeader(separator);
    }
    if (CpuInfo::dram) 
    {
        result += "dram:" + separator;
        result += CpuInfo::dram->getHeader(separator);
    }
    return result;
}

json CumulativeCpuInfo::toJSON() const
{
    json result;
    result["Name"] = this->name;
    result["EnergyCounter"] = datahelper::createMaxMinAvgJSON(energyCounterMax, 
                                                              energyCounterMin, 
                                                              CpuInfo::energyCounter, 
                                                              datahelper::computeStandardDeviation(energyCounterVariance, 
                                                                                                   energyCounterSampleSize));
    result["PowerCounter"] = datahelper::createMaxMinAvgJSON(powerCounterMax, 
                                                             powerCounterMin, 
                                                             CpuInfo::powerCounter, 
                                                             datahelper::computeStandardDeviation(powerCounterVariance, 
                                                                                                  powerCounterSampleSize));
    result["EnergyTotal"] = this->energyTotal;
    if (CpuInfo::core) 
    {
        result["core"] = CpuInfo::core->toJSON();
    }
    if (CpuInfo::uncore)
    {
        result["uncore"] = CpuInfo::uncore->toJSON();
    }
    if (CpuInfo::dram)
    {
        result["dram"] = CpuInfo::dram->toJSON();
    }
    return result;
}


CumulativeCpuInfo::~CumulativeCpuInfo() {}