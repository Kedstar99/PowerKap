#ifndef CUMULATIVE_BATTERY_INFO_H
#define CUMULATIVE_BATTERY_INFO_H

#include "batteryinfo.h"
#include "json.hpp"
#include "datahelper.h"
#include <string>

using json = nlohmann::json;

class CumulativeBatteryInfo : public BatteryInfo 
{

    public:
        CumulativeBatteryInfo(const BatteryInfo*);
        void insert(const BatteryInfo*) const;
        std::string toString(std::string separator) const;
        std::string getHeader(std::string separator) const;
        json toJSON() const;
        ~CumulativeBatteryInfo();
    private:
        mutable long long uVMax;
        mutable long long uVMin;
        mutable long long uVVariance;
        mutable int uVSampleSize;
        mutable long long uVOldMean;
        

        mutable long long uAMax;
        mutable long long uAMin;
        mutable long long uAVariance;
        mutable int uASampleSize;
        mutable long long uAOldMean;

        mutable double power;
        mutable double pMax;
        mutable double pMin;
        mutable double pVariance;
        mutable double pOldMean;
        mutable int pSampleSize;
};

#endif