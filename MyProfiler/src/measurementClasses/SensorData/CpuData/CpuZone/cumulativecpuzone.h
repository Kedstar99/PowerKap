#ifndef CUMULATIVE_CPUZONE_H
#define CUMULATIVE_CPUZONE_H

#include "cpuzone.h"
#include <string>
#include <climits>
#include "datahelper.h"
#include "json.hpp"

using json = nlohmann::json;

class CumulativeCpuZone : public CpuZone {

    public:
        CumulativeCpuZone(const CpuZone*);
        void insert(const CpuZone*) const;
        void setEnergyTotal(const CumulativeCpuZone*) const;
        std::string toString(std::string separator) const;
        std::string getHeader(std::string separator) const;
        json toJSON() const;
        ~CumulativeCpuZone();
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