#include "processedmeasurement.h"

ProcessedMeasurement::ProcessedMeasurement(const Measurement* curr, 
                                           const Measurement* prev, 
                                           long long elapsedTime, 
                                           double diffTime)
                   :Measurement(),
                    elapsedTime(0L)
{ 
    if (prev && curr && diffTime > 0.0) 
    {
        if (curr->packageData && prev->packageData) 
        {
            Measurement::packageData = curr->packageData->process(prev->packageData, diffTime);
        }
        if (curr->batteryData) 
        {
            Measurement::batteryData = curr->batteryData->clone();
        }
        if (curr->thermalData) {
            Measurement::thermalData = curr->thermalData->clone();
        }
        if (curr->diskData && prev->diskData) 
        {
            Measurement::diskData = curr->diskData->process(prev->diskData, diffTime);
        }
        if (curr->networkData && prev->networkData) 
        {
            Measurement::networkData = curr->networkData->process(prev->networkData, diffTime);
        }
        ProcessedMeasurement::elapsedTime = elapsedTime;
    }
}

ProcessedMeasurement::ProcessedMeasurement()
                     :Measurement(), 
                      elapsedTime(0L) {};

std::string ProcessedMeasurement::getHeader(std::string separator, const Config* config) const 
{
    return "Elapsed Time:" + separator + Measurement::callMembersGetHeader(separator, config);
}

std::string ProcessedMeasurement::toString(std::string separator, const Config* config) const 
{
    return std::to_string(elapsedTime) + separator + Measurement::callMembersToString(separator, config);
}

json ProcessedMeasurement::toJSON(const Config* config) const
{
    json result;
    result["ElapsedTime (ms)"] = elapsedTime;
    result["Data"] = Measurement::callMembersToJSON(config);
    return result;
}

ProcessedMeasurement::~ProcessedMeasurement() {}