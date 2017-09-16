#include "networkinfo.h"

NetworkInfo::NetworkInfo():
            bytesReceived(0),
            bytesSent(0) {}

NetworkInfo::NetworkInfo(long long bytesReceived, long long bytesSent)
            :bytesReceived(bytesReceived),
             bytesSent(bytesSent) {}

std::string NetworkInfo::toString(std::string separator) const 
{
   std::string result = "";
   result += " " + separator;
   result += std::to_string(bytesReceived) + separator;
   result += std::to_string(bytesSent) + separator;
   return result;
}

std::string NetworkInfo::getHeader(std::string separator) const 
{
    std::string result = "";
    result += "Network (bytes)" + separator;
    result += "received:" + separator;
    result += "sent:" + separator;
    return result;
}

json NetworkInfo::toJSON() const 
{
    json result;
    result["Received"] = bytesReceived;
    result["Sent"] = bytesSent;
    return result;
}

void NetworkInfo::insert(const NetworkInfo* thInfo) const {}

const NetworkInfo* NetworkInfo::clone() const 
{
    return new NetworkInfo(bytesReceived, bytesSent);
}

const NetworkInfo* NetworkInfo::process(const NetworkInfo* networkInfo, double diffTime) const 
{
    const NetworkInfo* result = NULL;
    if (networkInfo) 
    {
        if (diffTime == 0.0) 
        {
            throw std::logic_error("Division by 0 error");
        }
        long long bytesReceived = this->bytesReceived - networkInfo->bytesReceived;
        long long bytesSent = this->bytesSent -networkInfo->bytesSent;
        result = new NetworkInfo(bytesReceived, bytesSent);
    }
    return result;
}

NetworkInfo::~NetworkInfo() {}