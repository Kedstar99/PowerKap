#include "cpuinfo.h"

CpuInfo::CpuInfo(long long energyCounter, 
                 long long powerCounter, 
                 std::string name, 
                 const CpuZone* core, 
                 const CpuZone* uncore, 
                 const CpuZone* dram)
        :energyCounter(energyCounter), 
         powerCounter(powerCounter), 
         name(name), 
         core(core), 
         uncore(uncore), 
         dram(dram) {}

CpuInfo::CpuInfo()
        :energyCounter(0L),
         powerCounter(0L),
         name(""),
         core(NULL),
         uncore(NULL),
         dram(NULL) {}


const CpuInfo* CpuInfo::clone() const 
{
    const CpuZone* tempCore = (core ? this->core->clone() : NULL);
    const CpuZone* tempUnCore = (uncore ? this->uncore->clone() : NULL);
    const CpuZone* tempDram = (dram ? this->dram->clone() : NULL);

    return new CpuInfo(this->energyCounter, 
                       this->powerCounter, 
                       this->name, 
                       tempCore, 
                       tempUnCore, 
                       tempDram);
}

std::string CpuInfo::toString(std::string separator) const 
{
    std::string result = " " + separator + 
    result += std::to_string(this->energyCounter) + separator;
    result += std::to_string(this->powerCounter) + separator;
    if (core) 
    {
        result += " " + separator;
        result += separator + separator + this->core->toString(separator);
    }
    if (uncore) 
    {
        result += " " + separator;
        result += separator + separator + this->uncore->toString(separator);
    }
    if (dram) 
    {
        result += " " + separator;
        result += separator + separator + this->dram->toString(separator);
    }
    return result;
}

std::string CpuInfo::getHeader(std::string separator) const 
{
    std::string result = this->name + separator + "Energy Counter(mJ):" + separator + "Power(mJ):" + separator;
    if (core) 
    {
        result += "core:" + separator;
        result += separator + separator + this->core->getHeader(separator);
    }
    if (uncore) 
    {
        result += "uncore:" + separator;
        result += separator + separator + this->uncore->getHeader(separator);
    }
    if (dram) 
    {
        result += "dram:" + separator;
        result += separator + separator + this->dram->getHeader(separator);
    }
    return result;
}


const CpuInfo* CpuInfo::process(const CpuInfo* prev, double timeDiff) const 
{
    const CpuInfo* result = NULL;
    if (prev) 
    {
        if (timeDiff == 0.0) 
        {
            throw std::logic_error("Division by 0, provide different CpuInfo results");
        } 
        else 
        {
            long long powerCounter = this->powerCounter;
            std::string name = this->name;
            long long energyCounter = (this->energyCounter - prev->energyCounter) / timeDiff;
            const CpuZone* core = NULL;
            const CpuZone* uncore = NULL;
            const CpuZone* dram = NULL;
            if (this->core) 
            {
                core = this->core->process(prev->core, timeDiff);
            }
            if (this->uncore) 
            {
                uncore = this->uncore->process(prev->uncore, timeDiff);
            }
            if (this->dram) 
            {
                dram = this->dram->process(prev->dram, timeDiff);
            }
            result = new CpuInfo(energyCounter, powerCounter, name, core, uncore, dram);
        }
    }
    return result;
}

json CpuInfo::toJSON() const 
{
    json result;
    result["Name"] = this->name;
    result["EnergyCounter"] = this->energyCounter;
    result["PowerCounter"] = this->powerCounter;
    if (this->core) 
    {
        result["core"] = this->core->toJSON();
    }
    if (this->uncore) 
    {
        result["uncore"] = this->uncore->toJSON();
    }
    if (this->dram) 
    {
        result["dram"] = this->dram->toJSON();
    }
    return result;
}

void CpuInfo::insert(const CpuInfo* cpuInfo) const {}

CpuInfo::~CpuInfo() 
{
    if (this->core) 
    {
        delete this->core;
        this->core = NULL;
    }
    if (this->uncore) 
    {
        delete this->uncore;
        this->uncore = NULL;
    }
    if (this->dram) 
    {
        delete this->dram;
        this->dram = NULL;
    }    
}
