#include "cumulativebatteryinfo.h"

CumulativeBatteryInfo::CumulativeBatteryInfo(const BatteryInfo* btInfo)
                      :BatteryInfo(),
                     uVMax(0L),
                     uVMin(0L),
                     uVVariance(0L),
                     uVOldMean(0L),
                     uVSampleSize(0),
                     uAMax(0L),
                     uAMin(0L),
                     uAVariance(0L),
                     uAOldMean(0L),
                     uASampleSize(0),
                     power(0),
                     pMax(0),
                     pMin(0),
                     pVariance(0),
                     pOldMean(0),
                     pSampleSize(0)

{
    if (btInfo) 
    {
        uVMax = btInfo->microVoltage;
        uVMin = btInfo->microVoltage;
        uVOldMean = btInfo->microVoltage;
        BatteryInfo::microVoltage = btInfo->microVoltage;
        uVSampleSize++;

        uAMax = btInfo->microCurrent;
        uAMin = btInfo->microCurrent;
        BatteryInfo::microCurrent = btInfo->microCurrent;
        uAOldMean = btInfo->microCurrent;
        uASampleSize++;

        power = (btInfo->microVoltage * btInfo->microCurrent) / PICO_TO_NORMAL;
        pMax = power;
        pMin = power;
        pOldMean = power;
        pSampleSize++;
        
    }
}

void CumulativeBatteryInfo::insert(const BatteryInfo* btInfo) const 
{
    if (btInfo) 
    {
        uVSampleSize++;
        uVMax = std::max(btInfo->microVoltage, this->uVMax);
        uVMin = std::min(btInfo->microVoltage, this->uVMin);
        BatteryInfo::microVoltage += datahelper::computeRunningAverage(btInfo->microVoltage, 
                                                                       BatteryInfo::microVoltage, 
                                                                       uVSampleSize);
        uVVariance += datahelper::computeRunningVariance(uVVariance, 
                                                         btInfo->microVoltage, 
                                                         uVOldMean, 
                                                         BatteryInfo::microVoltage);
        uVOldMean = BatteryInfo::microVoltage;

        uASampleSize++;
        uAMax = std::max(btInfo->microCurrent, this->uAMax);
        uAMin = std::min(btInfo->microCurrent, this->uAMin);
        BatteryInfo::microCurrent += datahelper::computeRunningAverage(btInfo->microCurrent, 
                                                                       BatteryInfo::microCurrent, 
                                                                       uASampleSize);
        uAVariance += datahelper::computeRunningVariance(uAVariance, 
                                                         btInfo->microCurrent, 
                                                         uAOldMean, 
                                                         BatteryInfo::microCurrent);
        uAOldMean = BatteryInfo::microCurrent;

        pSampleSize++;
        double newPower = (btInfo->microVoltage * btInfo->microCurrent) / PICO_TO_NORMAL;
        pMax = std::max(newPower, this->pMax);
        pMin = std::min(newPower, this->pMin);
        power += datahelper::computeRunningAverage(newPower, this->power, pSampleSize);
        pVariance += datahelper::computeRunningVariance(pVariance, newPower, pOldMean, this->power);
        pOldMean = power;
    }
}

std::string CumulativeBatteryInfo::toString(std::string separator) const 
{
    std::string result = "";
    if (!(BatteryInfo::microVoltage == 0L && BatteryInfo::microCurrent == 0L))
    {
        result += separator + separator;
        result += datahelper::createMaxMinAvgString(this->uVMax, 
                                                    this->uVMin, 
                                                    BatteryInfo::microVoltage, 
                                                    this->uVVariance, 
                                                    uVSampleSize, 
                                                    separator);

        result += separator + separator;
        result += datahelper::createMaxMinAvgString(this->uAMax, 
                                                    this->uAMin, 
                                                    BatteryInfo::microCurrent, 
                                                    this->uAVariance, 
                                                    uASampleSize, 
                                                    separator);

        result += separator + separator;
        result += datahelper::createMaxMinAvgString(this->pMax, 
                                                    this->pMin, 
                                                    this->power, 
                                                    this->pVariance, 
                                                    pSampleSize, 
                                                    separator);
    }
    return result;
}

std::string CumulativeBatteryInfo::getHeader(std::string separator) const 
{
    std::string result = "";
    
    std::string tabHeader = datahelper::genHeader(separator);
    if (!(BatteryInfo::microVoltage == 0L && BatteryInfo::microCurrent == 0L))
    {
        result += "microVoltage" + separator + tabHeader;
        result += "microCurrent" + separator + tabHeader;
        result += "power" + separator + tabHeader;
    }
    return result;
}

json CumulativeBatteryInfo::toJSON() const
{
    json result;
    result["MicroVoltage"] = datahelper::createMaxMinAvgJSON(uVMax, 
                                                             uVMin, 
                                                             BatteryInfo::microVoltage, 
                                                             datahelper::computeStandardDeviation(uVVariance, uVSampleSize));
    result["MicroCurrent"] = datahelper::createMaxMinAvgJSON(uAMax, 
                                                             uAMin, 
                                                             BatteryInfo::microCurrent, 
                                                             datahelper::computeStandardDeviation(uAVariance, uASampleSize));
    result["Power"] = datahelper::createMaxMinAvgJSON(pMax, 
                                                      pMin, 
                                                      power, 
                                                      datahelper::computeStandardDeviation(pVariance, pSampleSize));
    return result;
    
}

CumulativeBatteryInfo::~CumulativeBatteryInfo() {}