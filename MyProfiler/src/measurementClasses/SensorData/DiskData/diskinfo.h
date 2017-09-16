#ifndef DISKINFO_H
#define DISKINFO_H

#include "datameasurement.h"
#include "json.hpp"
#include <string>
#include <stdexcept>

using json = nlohmann::json;

class DiskInfo: public DataMeasurement
{
    public:
        DiskInfo();
        DiskInfo(long long readBytes, long long writeBytes);
        std::string toString(std::string separator) const;
        std::string getHeader(std::string separator) const;
        json toJSON() const;
        virtual void insert(const DiskInfo*) const;
        const DiskInfo* clone() const;
        const DiskInfo* process(const DiskInfo* diskInfo, double timeInterval) const;
        virtual ~DiskInfo();

        mutable long long bytesRead;
        mutable long long bytesWritten;

};

#endif