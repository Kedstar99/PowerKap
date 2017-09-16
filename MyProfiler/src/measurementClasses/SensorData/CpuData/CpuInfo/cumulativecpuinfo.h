#ifndef CUMULATIVE_CPUINFO_H
#define CUMULATIVE_CPUINFO_H

#include <string>
#include <climits>
#include "cumulativecpuzone.h"
#include "cpuinfo.h"
#include "datahelper.h"
#include "json.hpp"

using json = nlohmann::json;

class CumulativeCpuInfo : public CpuInfo 
{

    public:
        CumulativeCpuInfo(const CpuInfo*);
        void insert(const CpuInfo*) const;
        void setEnergyTotal(const CumulativeCpuInfo*) const;
        std::string toString(std::string separator) const;
        std::string getHeader(std::string separator) const;
        json toJSON() const;
        ~CumulativeCpuInfo();
    private:
        mutable long long energyCounterMax;
        mutable long long energyCounterMin;
        mutable long long energyTotal;
        mutable long long energyCounterVariance;
        mutable long long energyCounterOldMean;
        mutable int energyCounterSampleSize;
        mutable long long powerCounterMax;
        mutable long long powerCounterMin;
        mutable long long powerCounterOldMean;
        mutable int powerCounterSampleSize;
        mutable long long powerCounterVariance;
};

#endif