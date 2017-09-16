#include "datahelper.h"

namespace datahelper 
{
    long long computeRunningAverage(long long newVal, long long oldAvg, int sampleSize) 
    {
        return (sampleSize > 0 ? (newVal - oldAvg) / sampleSize : newVal);
    }

    double computeRunningAverage(double newVal, double oldAvg, int sampleSize) 
    {
        return (sampleSize > 0 ? (newVal - oldAvg) / sampleSize : newVal);
    }

    long long computeRunningVariance(long long oldVar, long long newVal, long long oldMean, long long newMean)
    {
        return (newVal - oldMean) * (newVal - newMean);
    }

    double computeRunningVariance(double oldVar, double newVal, double oldMean, double newMean)
    {
        return (newVal - oldMean) * (newVal - newMean);
    }

    bool withinRange(long long newVal, long long mean, long long sd)
    {
        return (newVal < (mean + 2*sd)) && (newVal > (mean - 2*sd));
    }

    long long computeStandardDeviation(long long variance, int sampleSize) 
    {
        if (variance < 0) {
            throw std::logic_error("Error Variance is negative " + std::to_string(variance));
        }
        return (sampleSize > 1 ? (long long)std::sqrt( variance / (sampleSize - 1)) : 0.0);
    }
    
    double computeStandardDeviation(double variance, int sampleSize) 
    {
        if (variance < 0) {
            throw std::logic_error("Error Variance is negative " + std::to_string(variance));
        }
        return (sampleSize > 1 ? std::sqrt( variance / (sampleSize - 1)) : 0.0);
    }

    json createMaxMinAvgJSON(long long max, long long min, long long avg, long long stdDev) 
    {
        json result;
        result["max"] = max;
        result["min"] = min;
        result["avg"] = avg;
        result["stddev"] = stdDev;
        return result;
    }

    json createMaxMinAvgJSON(double max, double min, double avg, double stdDev) 
    {
        json result;
        result["max"] = max;
        result["min"] = min;
        result["avg"] = avg;
        result["stddev"] = stdDev;
        return result;
    }

    std::string createMaxMinAvgString(long long max, long long min, long long avg, long long stdVar, int sampleSize, std::string separator)
    {
        std::string result = "";
        result += std::to_string(max) + separator;
        result += std::to_string(min) + separator;
        result += std::to_string(avg) + separator;
        result += std::to_string(datahelper::computeStandardDeviation(stdVar, sampleSize));
        return result;
    }

    std::string createMaxMinAvgString(double max, double min, double avg, double stdVar, int sampleSize, std::string separator)
    {
        std::string result = "";
        result += std::to_string(max) + separator;
        result += std::to_string(min) + separator;
        result += std::to_string(avg) + separator;
        result += std::to_string(datahelper::computeStandardDeviation(stdVar, sampleSize));
        return result;
    }

    std::string getStringFromTime(std::chrono::system_clock::time_point timePoint) 
    {
        time_t timeTPoint = std::chrono::system_clock::to_time_t(timePoint);
        char buffer[20];
        strftime(buffer, 20, "%Y:%m:%d %H:%M:%S", localtime(&timeTPoint));
        auto duration = timePoint.time_since_epoch();
        auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() % 1000;
        std::string result = std::string(buffer);
        result += ".";
        result += std::to_string(milliseconds);
        return result;
    }

    std::string genHeader(std::string separator) 
    {
        std::string tabHeader = "max:" + separator;
        tabHeader += "min:" + separator;
        tabHeader += "average:" + separator;
        tabHeader += "stddev:" + separator;
        return tabHeader;
    }

}