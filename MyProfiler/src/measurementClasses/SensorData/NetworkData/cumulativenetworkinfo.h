#ifndef CUMULATIVE_NETWORKINFO_H
#define CUMULATIVE_NETWORKINFO_H

#include <string>
#include <climits>
#include "networkinfo.h"
#include "datahelper.h"

class CumulativeNetworkInfo: public NetworkInfo 
{
    public:
        CumulativeNetworkInfo(const NetworkInfo*);
        ~CumulativeNetworkInfo();
        virtual void insert(const NetworkInfo*) const;
        void setBytesTotal(const CumulativeNetworkInfo* prev) const;
        std::string toString(std::string separator) const;
        std::string getHeader(std::string separator) const;
        json toJSON() const;

        mutable long long minBytesReceived;
        mutable long long maxBytesReceived;
        mutable long long totalBytesReceived;
        mutable int receivedSampleSize;
        mutable long long receivedVariance;
        mutable long long receivedOldMean;

        mutable long long minBytesSent;
        mutable long long maxBytesSent;
        mutable long long totalBytesSent;
        mutable int sentSampleSize;
        mutable long long sentVariance;
        mutable long long sentOldMean;
        
};

#endif
