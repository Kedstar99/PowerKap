#include "diskinfo.h"

DiskInfo::DiskInfo()
         :bytesRead(0),
          bytesWritten(0)
{

}

DiskInfo::DiskInfo(long long readBytes, long long writeBytes)
         :bytesRead(readBytes),
          bytesWritten(writeBytes)
{

}


std::string DiskInfo::toString(std::string separator) const 
{
    std::string result = " " + separator;
    result += std::to_string(bytesRead) + separator;
    result += std::to_string(bytesWritten) + separator;
    return result;
}

std::string DiskInfo::getHeader(std::string separator) const
{
    std::string result = "DiskIO" + separator;
    result += "read(bytes):" + separator;
    result += "written(bytes):" + separator;
    return result;
}

json DiskInfo::toJSON() const 
{
    json result;
    json data;
    data["ReadBytes"] = bytesRead;
    data["WrittenBytes"] = bytesWritten;
    result["DiskIO"] = data;
    return result;
}

const DiskInfo* DiskInfo::clone() const 
{
    return new DiskInfo(this->bytesRead, this->bytesWritten);
}


void DiskInfo::insert(const DiskInfo* thInfo) const
{

}

const DiskInfo* DiskInfo::process(const DiskInfo* diskInfo, double timeInterval) const 
{
    const DiskInfo* result = NULL;
    if (diskInfo) 
    {
        if (timeInterval == 0) {
            throw std::logic_error("Division by 0, provide different CpuInfo results");
        }
        long long bytesRead = this->bytesRead - diskInfo->bytesRead;
        long long bytesWritten = this->bytesWritten - diskInfo->bytesWritten;
        result = new DiskInfo(bytesRead, bytesWritten);
    }
    return result;
}


DiskInfo::~DiskInfo(){}