#ifndef BATTERYINFO_H
#define BATTERYINFO_H

#define PICO_TO_NORMAL 1000000000000.0
#include <string>
#include "datameasurement.h"
#include "json.hpp"

using json = nlohmann::json;

class BatteryInfo: public DataMeasurement 
{
    public:
        BatteryInfo();
        BatteryInfo(long long microVoltage, long long microCurrent);
        const BatteryInfo* clone() const;
        const BatteryInfo* process(const BatteryInfo*, double) const;
        virtual std::string toString(std::string separator) const;
        virtual std::string getHeader(std::string separator) const;
        virtual json toJSON() const; 
        virtual void insert(const BatteryInfo* btInfo) const; 
        virtual ~BatteryInfo();
        mutable long long microVoltage;
        mutable long long microCurrent;

};

#endif