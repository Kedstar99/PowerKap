#ifndef PACKAGEINFO_H
#define PACKAGEINFO_H

#include "cpuinfo.h"
#include "json.hpp"
#include <string>

using json = nlohmann::json;

class PackageInfo : public DataMeasurement {
    public:
        PackageInfo(const CpuInfo*, const CpuInfo*);
        PackageInfo();
        const PackageInfo* clone() const;
        const PackageInfo* process(const PackageInfo*, double) const;
        std::string toString(std::string separator) const;
        std::string getHeader(std::string separator) const;
        json toJSON() const;
        virtual void insert(const PackageInfo* pckInfo) const;
        virtual ~PackageInfo();

        
        const CpuInfo* packageOne;
        const CpuInfo* packageTwo;
};

#endif