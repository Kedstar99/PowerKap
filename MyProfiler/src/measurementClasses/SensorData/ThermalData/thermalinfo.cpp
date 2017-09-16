#include "thermalinfo.h"

ThermalInfo::ThermalInfo() {}

ThermalInfo::ThermalInfo(const std::map< std::string, long long > toCopy)
{
    thermalMeasurements = std::map< std::string, long long >(toCopy);
}

void ThermalInfo::addMeasurement(std::string type, long long temp) const 
{
    this->thermalMeasurements.insert(std::make_pair(type, temp));
}

std::string ThermalInfo::toString(std::string separator) const 
{
   std::string result = "";
   for (auto& elem: thermalMeasurements) 
   {
        result += std::to_string(elem.second) + separator;
   }
   return result;
}

std::string ThermalInfo::getHeader(std::string separator) const 
{
    std::string result = "";
    for (auto& elem: thermalMeasurements) {
        result += elem.first + "(mC)" + separator;
    }
    return result;
}

json ThermalInfo::toJSON() const 
{
    json result;
    for (auto& elem: thermalMeasurements)
    {
        result[elem.first] = elem.second;
    }
    return result;
}

void ThermalInfo::insert(const ThermalInfo* thInfo) const {}

const ThermalInfo* ThermalInfo::clone() const 
{
    return new ThermalInfo(this->thermalMeasurements);
}

const ThermalInfo* ThermalInfo::process(const ThermalInfo* thInfo, double timeInterval) const 
{
    return new ThermalInfo();
}

ThermalInfo::~ThermalInfo() {}