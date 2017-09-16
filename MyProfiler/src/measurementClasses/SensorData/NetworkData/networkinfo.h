#ifndef NETWORKINFO_H
#define NETWORKINFO_H

#include "datameasurement.h"
#include <string>
#include <map>
#include <stdexcept>
#include "json.hpp"

using json = nlohmann::json;

class NetworkInfo: public DataMeasurement 
{
    public:
        NetworkInfo();
        NetworkInfo(long long, long long);
        std::string toString(std::string separator) const;
        std::string getHeader(std::string separator) const;
        json toJSON() const;
        void insert(const NetworkInfo*) const;
        virtual ~NetworkInfo();
        
        const NetworkInfo* clone() const;
        const NetworkInfo* process(const NetworkInfo*, double) const;
        mutable long long bytesReceived;
        mutable long long bytesSent;
};

#endif
