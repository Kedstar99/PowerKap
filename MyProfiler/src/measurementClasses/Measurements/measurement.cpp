#include "measurement.h"

Measurement::Measurement(const ThermalInfo* thermalData, 
                         const BatteryInfo* batteryData, 
                         const PackageInfo* packageData,
                         const DiskInfo* diskData,
                         const NetworkInfo* networkData)
           :thermalData(thermalData), 
            batteryData(batteryData), 
            packageData(packageData),
            diskData(diskData),
            networkData(networkData){}

Measurement::Measurement()
            :thermalData(NULL),
             batteryData(NULL),
             packageData(NULL),
             diskData(NULL),
             networkData(NULL) {}

std::string Measurement::callMembersToString(std::string separator, const Config* config) const 
{
    std::string result = "";
    if (config) 
    {
        if (!config->getSkipCPU() && packageData) 
        {
            result += packageData->toString(separator) + separator;
        }
        if (!config->getSkipTemp() && thermalData) 
        {
            result += thermalData->toString(separator) + separator; 
        }
        if (!config->getSkipBattery() && batteryData) 
        {
            result += batteryData->toString(separator) + separator;
        }
        if (!config->getSkipDisk() && diskData)
        {
            result += diskData->toString(separator) + separator;
        }
        if (!config->getSkipNetwork() && networkData)
        {
            result += networkData ->toString(separator) + separator;
        }
    }
    return result;
}


std::string Measurement::callMembersGetHeader(std::string separator, const Config* config) const 
{
    std::string result = "";
    if (config) 
    {
        if (!config->getSkipCPU() && packageData) 
        {
            result += packageData->getHeader(separator) + separator;
        }
        if (!config->getSkipTemp() && thermalData) 
        {
            result += thermalData->getHeader(separator) + separator; 
        }
        if (!config->getSkipBattery() && batteryData) 
        {
            result += batteryData->getHeader(separator) + separator;
        }
        if (!config->getSkipDisk() && diskData)
        {
            result += diskData->getHeader(separator) + separator;
        }
        if (!config->getSkipNetwork() && networkData)
        {
            result += networkData->getHeader(separator) + separator;
        }
    }
    return result;
}

json Measurement::callMembersToJSON(const Config* config) const
{
    json result;
    if (config)
    {
        if (!config->getSkipCPU() && packageData) 
        {
            result["CPUData"] = packageData->toJSON();
        }
        if (!config->getSkipTemp() && thermalData) 
        {
            result["ThermalData"] = thermalData->toJSON(); 
        }
        if (!config->getSkipBattery() && batteryData) 
        {
            result["BatteryData"] = batteryData->toJSON();
        }
        if (!config->getSkipDisk() && diskData)
        {
            result["DiskData"] = diskData->toJSON();
        }
        if (!config->getSkipNetwork() && networkData)
        {
            result["NetworkData"] = networkData->toJSON();
        }
    }
    return result;
}

Measurement::~Measurement() 
{
    if (batteryData) 
    {
        delete batteryData;
        batteryData = NULL;
    }
    if (packageData) 
    {
        delete packageData;
        packageData = NULL;
    }
    if (thermalData) 
    {
        delete thermalData;
        thermalData = NULL;
    }
    if (diskData)
    {
        delete diskData;
        diskData = NULL;
    }
    if (networkData)
    {
        delete networkData;
        networkData = NULL;
    }
}