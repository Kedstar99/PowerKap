#include "batteryinfo.h"

BatteryInfo::BatteryInfo(long long microVoltage, long long microCurrent)
                         :microVoltage(microVoltage), 
                          microCurrent(microCurrent) 
                          {}

BatteryInfo::BatteryInfo()
            :microVoltage(0L), 
             microCurrent(0L)
             {}

std::string BatteryInfo::toString(std::string separator) const {
    std::string result = "";
    if (!(this->microVoltage == 0L && this->microCurrent == 0L)) 
    {
        result = std::to_string(this->microVoltage) + separator;
        result += std::to_string(this->microCurrent) + separator;
        result += std::to_string((this->microCurrent * this->microVoltage) / PICO_TO_NORMAL); 
    }
    return result;
}

std::string BatteryInfo::getHeader(std::string separator) const {
    std::string result = "";
    if (!(this->microVoltage == 0L && this->microCurrent == 0L)) 
    {
        result = "MicroVoltage" + separator + "MicroCurrent" + separator + "PicoPower";
    }
    return result;
} 

json BatteryInfo::toJSON() const 
{
    json result;
    result["MicroVoltage"] = this->microVoltage;
    result["MicroCurrent"] = this->microCurrent;
    result["Power"] = (this->microVoltage * this->microCurrent) / PICO_TO_NORMAL;
    return result;
}

const BatteryInfo* BatteryInfo::clone() const 
{
    return new BatteryInfo(this->microVoltage, this->microCurrent);
}

const BatteryInfo* BatteryInfo::process(const BatteryInfo*, double) const 
{
    return new BatteryInfo();
}

void BatteryInfo::insert(const BatteryInfo* btInfo) const {}

BatteryInfo::~BatteryInfo() {}