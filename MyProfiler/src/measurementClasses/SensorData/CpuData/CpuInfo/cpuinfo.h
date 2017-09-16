#ifndef CPUINFO_H
#define CPUINFO_H

#include <string>
#include <stdexcept>


#include "cpuzone.h"
#include "datameasurement.h"
#include "json.hpp"

using json = nlohmann::json;

class CpuInfo: public DataMeasurement 
{
    public: 
        CpuInfo();
        CpuInfo(long long energyCounter, long long powerCounter, std::string name, const CpuZone* core, const CpuZone* uncore, const CpuZone* dram);
        std::string toString(std::string separator) const;
        std::string getHeader(std::string separator) const;
        json toJSON() const;
        const CpuInfo* clone() const;
        const CpuInfo* process(const CpuInfo*, double) const;
        virtual void insert(const CpuInfo*) const;
        virtual ~CpuInfo();

    
        mutable long long energyCounter;
        mutable long long powerCounter;
        mutable std::string name;
        const CpuZone* core;
        const CpuZone* uncore;
        const CpuZone* dram;
};

#endif