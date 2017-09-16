#include "rawmeasurement.h"

RawMeasurement::RawMeasurement(const ThermalInfo* thermalData, 
                               const BatteryInfo* batteryData, 
                               const PackageInfo* packageData,
                               const DiskInfo* diskData,
                               const NetworkInfo* networkData,
                               std::chrono::system_clock::time_point timePoint,
                               int run)
                        :Measurement(thermalData, batteryData, packageData, diskData, networkData),
                         timePoint(timePoint),
                         run(run)
{

};

std::string RawMeasurement::toString(std::string separator, const Config* config) const 
{
    std::string result = datahelper::getStringFromTime(this->timePoint) + separator;
    result += Measurement::callMembersToString(separator, config);
    return result;
}

std::string RawMeasurement::getHeader(std::string separator,  const Config* config) const 
{
    std::string result = "RUN:" + std::to_string(run) + "\n" + "Local Time" + separator;
    result +=  Measurement::callMembersGetHeader(separator, config);
    return result;
}

json RawMeasurement::toJSON( const Config* config) const 
{
    json result;
    result["run"] = this->run;
    result["Local Time"] = datahelper::getStringFromTime(this->timePoint);
    result["Data"] = Measurement::callMembersToJSON(config);
    return result;
}

RawMeasurement::~RawMeasurement() {}
