#ifndef CUMULATIVE_THERMALINFO_H
#define CUMULATIVE_THERMALINFO_H

#include <string>
#include "thermalinfo.h"
#include "datahelper.h"

class CumulativeThermalInfo: public ThermalInfo 
{
    public:
        CumulativeThermalInfo(const ThermalInfo* thInfo);
        ~CumulativeThermalInfo();
        void insert(const ThermalInfo* thInfo) const;
        std::string getHeader(std::string separator) const;

        mutable int sampleSize;
};

#endif
