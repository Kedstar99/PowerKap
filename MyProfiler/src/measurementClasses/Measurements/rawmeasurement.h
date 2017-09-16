#ifndef RAWMEASUREMENT_H
#define RAWMEASUREMENT_H

#include <string>
#include <chrono>
#include <vector>
#include "measurement.h"
#include "json.hpp"
#include "datahelper.h"
#include "config.h"


using json = nlohmann::json;


class RawMeasurement : public Measurement 
{
    public:
        RawMeasurement(const ThermalInfo* thInfo, 
                       const BatteryInfo* btInfo, 
                       const PackageInfo* pckInfo, 
                       const DiskInfo* dskInfo,
                       const NetworkInfo* netInfo,
                       std::chrono::system_clock::time_point timePoint,
                       int run);

        std::string toString(std::string separator, const Config* config) const;
        std::string getHeader(std::string separator,  const Config* config) const;
        json toJSON(const Config* config) const;
        ~RawMeasurement();
    private:    
        std::chrono::system_clock::time_point timePoint;
        int run;
};

#endif
