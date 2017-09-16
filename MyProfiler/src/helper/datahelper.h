#ifndef DATA_HELPER_H
#define DATA_HELPER_H

#include "json.hpp"
#include <chrono>
#include <string>
#include <stdexcept>

using json = nlohmann::json;

namespace datahelper
{
    long long computeRunningAverage(long long newVal, long long oldVal, int sampleSize);
    double computeRunningAverage(double newVal, double oldVal, int sampleSize);
    
    long long computeRunningVariance(long long oldVar, long long nevVal, long long oldMean, long long newMean);
    double computeRunningVariance(double oldVar, double nevVal, double oldMean, double newMean);
     
    long long computeStandardDeviation(long long variance, int sampleSize);
    double computeStandardDeviation(double variance, int sampleSize);
   
    json createMaxMinAvgJSON(long long max, long long min, long long avg, long long stdDev);
    json createMaxMinAvgJSON(double max, double min, double avg, double stdDev);

    std::string createMaxMinAvgString(long long max, long long min, long long avg, long long stdVar, int sampleSize, std::string separator);
    std::string createMaxMinAvgString(double max, double min, double avg, double stdVar, int sampleSize, std::string separator);

    std::string getStringFromTime(std::chrono::system_clock::time_point timePoint);

    std::string genHeader(std::string separator); 

}

#endif