#ifndef PMEASUREMENT_H
#define PMEASUREMENT_H

#include <chrono>
#include "measurement.h"
#include "rawmeasurement.h"
#include "json.hpp"
#include "config.h"


using json = nlohmann::json;

class ProcessedMeasurement: public Measurement 
{
    public:
        ProcessedMeasurement();
        ProcessedMeasurement(const Measurement* curr, 
                             const Measurement* prev, 
                             long long elapsedTime, 
                             double diffTime);
        std::string getHeader(std::string separator, const Config* config) const;
        std::string toString(std::string separator, const Config* config) const;
        json toJSON(const Config* config) const;
        long long elapsedTime;
        virtual ~ProcessedMeasurement();
};
#endif
