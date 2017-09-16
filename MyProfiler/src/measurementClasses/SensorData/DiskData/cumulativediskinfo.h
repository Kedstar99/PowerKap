#ifndef CUMULATIVE_DISKINFO_H
#define CUMULATIVE_DISKINFO_H

#include "diskinfo.h"
#include "datahelper.h"
#include <string>
#include <iostream>
#include <climits>

class CumulativeDiskInfo: public DiskInfo 
{
    public:
        CumulativeDiskInfo(const DiskInfo*);
        ~CumulativeDiskInfo();
        void insert(const DiskInfo*) const;
        void setBytesTotal(const CumulativeDiskInfo* prev) const;
        std::string toString(std::string separator) const;
        std::string getHeader(std::string separator) const;
        json toJSON() const;
        mutable long long minBytesRead;
        mutable long long maxBytesRead;
        mutable long long totalBytesRead;
        mutable int readSampleSize;
        mutable long long readVariance;
        mutable long long readOldMean;

        mutable long long minBytesWritten;
        mutable long long maxBytesWritten;
        mutable long long totalBytesWritten;
        mutable int writeSampleSize;
        mutable long long writeVariance;
        mutable long long writeOldMean;
};

#endif
