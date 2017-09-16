#include "cumulativethermalinfo.h"

CumulativeThermalInfo::CumulativeThermalInfo(const ThermalInfo* thInfo)
                    :ThermalInfo(),
                     sampleSize(0) 
{
    if (thInfo) 
    {
        ThermalInfo::thermalMeasurements = std::map< std::string, long long >(thInfo->thermalMeasurements);
        sampleSize++;
    }
}

void CumulativeThermalInfo::insert(const ThermalInfo* thInfo) const 
{
    if (thInfo) 
    {
        sampleSize++;
        auto thermalMeasurements = std::map< std::string, long long >(thInfo->thermalMeasurements);
        for (auto &elem: thermalMeasurements) 
        {
            ThermalInfo::thermalMeasurements.find(elem.first)->second += datahelper::computeRunningAverage(elem.second, ThermalInfo::thermalMeasurements.find(elem.first)->second, sampleSize);
        }
    }
}

std::string CumulativeThermalInfo::getHeader(std::string separator) const 
{
    std::string result = "";
    for (auto& elem: thermalMeasurements) 
    {
        result += elem.first + "(Average mC)" + separator;
    }
    return result;
}

CumulativeThermalInfo::~CumulativeThermalInfo() {}