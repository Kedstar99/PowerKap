#ifndef MEASUREMENT_H
#define MEASUREMENT_H

#include "config.h"
#include "batteryinfo.h"
#include "packageinfo.h"
#include "thermalinfo.h"
#include "diskinfo.h"
#include "networkinfo.h"
#include "json.hpp"
#include "json.hpp"
#include <string>

using json = nlohmann::json;

class Measurement {
    public:
        Measurement(const ThermalInfo* thInfo, 
                    const BatteryInfo* btInfo, 
                    const PackageInfo* pckInfo,
                    const DiskInfo* dskInfo,
                    const NetworkInfo* netInfo);
        Measurement();
        virtual std::string toString(std::string separator, const Config* config) const = 0;
        virtual std::string getHeader(std::string separator, const Config* config) const = 0;
        virtual json toJSON(const Config* config) const = 0;
        json callMembersToJSON(const Config* config) const;
        std::string callMembersToString(std::string separator, const Config* config) const;
        std::string callMembersGetHeader(std::string separator, const Config* config) const;

        virtual ~Measurement();

        const BatteryInfo* batteryData;
        const PackageInfo* packageData;
        const ThermalInfo* thermalData;
        const DiskInfo* diskData;
        const NetworkInfo* networkData;

};

#endif