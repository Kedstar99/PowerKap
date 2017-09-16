#ifndef THERMALINFO_H
#define THERMALINFO_H

#include "datameasurement.h"
#include <string>
#include <map>
#include "json.hpp"

using json = nlohmann::json;

class ThermalInfo: public DataMeasurement 
{
    public:
        ThermalInfo();
        ThermalInfo(const std::map< std::string, long long > toCopy);
        std::string toString(std::string separator) const;
        std::string getHeader(std::string separator) const;
        json toJSON() const;
        virtual void insert(const ThermalInfo* thInfo) const;
        void addMeasurement(std::string type, long long temp) const;
        virtual ~ThermalInfo();
        
        const ThermalInfo* clone() const;
        const ThermalInfo* process(const ThermalInfo*, double) const;
        mutable std::map< std::string, long long > thermalMeasurements;
};

#endif
