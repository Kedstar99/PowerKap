#ifndef CPUZONE_H
#define CPUZONE_H

#include "datameasurement.h"
#include <string>
#include "json.hpp"
#include <stdexcept>

using json = nlohmann::json;
class CpuZone: public DataMeasurement {
    public:
        CpuZone();
        CpuZone(long long energyCounter, long long microPower);
        std::string toString(std::string) const;
        std::string getHeader(std::string) const;
        json toJSON() const;
        const CpuZone* clone() const;
        const CpuZone* process(const CpuZone*, double) const;
        virtual void insert(const CpuZone* cpuZone) const;
        virtual ~CpuZone();

        
        mutable long long energyCounter;
        mutable long long powerCounter;
};

#endif