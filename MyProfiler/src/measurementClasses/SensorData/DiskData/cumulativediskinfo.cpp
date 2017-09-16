#include "cumulativediskinfo.h"

CumulativeDiskInfo::CumulativeDiskInfo(const DiskInfo* diskInfo) 
                   :readSampleSize(0),
                    minBytesRead(0),
                    maxBytesRead(0),
                    readOldMean(0),
                    readVariance(0),
                    writeSampleSize(0),
                    minBytesWritten(0),
                    maxBytesWritten(0),
                    writeOldMean(0),
                    writeVariance(0),
                    totalBytesRead(0),
                    totalBytesWritten(0),
                    DiskInfo()
{
    if (diskInfo) 
    {
        if (diskInfo->bytesRead >= 0 && diskInfo->bytesRead < LONG_MAX)
        {
            minBytesRead = diskInfo->bytesRead;
            maxBytesRead = diskInfo->bytesRead;
            bytesRead = diskInfo->bytesRead;
            readOldMean = diskInfo->bytesRead;
            readSampleSize++;
        }
        if (diskInfo->bytesWritten >= 0 && diskInfo->bytesWritten < LONG_MAX)
        {
            minBytesWritten = diskInfo->bytesWritten;
            maxBytesWritten = diskInfo->bytesWritten;
            bytesWritten = diskInfo->bytesWritten;
            writeOldMean = diskInfo->bytesWritten;
            writeSampleSize++;
        }
    }
}

void CumulativeDiskInfo::insert(const DiskInfo* diskInfo) const
{
    /*Broken do not use (Does not account for caching)
    if (diskInfo) 
    {
        readSampleSize++;
        minBytesRead = std::min(diskInfo->bytesRead, this->minBytesRead);
        maxBytesRead = std::max(diskInfo->bytesRead, this->maxBytesRead);
        DiskInfo::bytesRead += datahelper::computeRunningAverage(diskInfo->bytesRead, DiskInfo::bytesRead, readSampleSize);
        readVariance += datahelper::computeRunningVariance(readVariance, diskInfo->bytesRead, readOldMean, DiskInfo::bytesRead);
        readOldMean = DiskInfo::bytesRead;

        writeSampleSize++;
        minBytesWritten = std::min(diskInfo->bytesWritten, this->minBytesWritten);
        maxBytesWritten = std::max(diskInfo->bytesWritten, this->maxBytesWritten);
        DiskInfo::bytesWritten += datahelper::computeRunningAverage(diskInfo->bytesWritten, DiskInfo::bytesWritten, writeSampleSize);
        writeVariance += datahelper::computeRunningVariance(writeVariance, diskInfo->bytesWritten, writeOldMean, DiskInfo::bytesWritten);
        writeOldMean = DiskInfo::bytesWritten;
    }*/
}

void CumulativeDiskInfo::setBytesTotal(const CumulativeDiskInfo* prev) const
{
    if (prev)
    {
        totalBytesWritten = prev->totalBytesWritten + DiskInfo::bytesWritten;
        totalBytesRead = prev->totalBytesRead + DiskInfo::bytesRead;
    }
}

std::string CumulativeDiskInfo::getHeader(std::string separator) const
{
    std::string tabHeader = datahelper::genHeader(separator);

    std::string result =  "ReadBytes" + separator + tabHeader;
    result += "WrittenBytes" + separator + tabHeader;
    result += "TotalReadBytes" + separator;
    result += "TotalWriteBytes" + separator;
    return result;
}

std::string CumulativeDiskInfo::toString(std::string separator) const
{

    std::string readBytesString = datahelper::createMaxMinAvgString(this->maxBytesRead, 
                                                                    this->minBytesRead,
                                                                    DiskInfo::bytesRead,
                                                                    this->readVariance,
                                                                    this->readSampleSize,
                                                                    separator); 

    std::string writebytesString = datahelper::createMaxMinAvgString(this->maxBytesWritten,
                                                                     this->minBytesWritten,
                                                                     DiskInfo::bytesWritten,
                                                                     this->writeVariance,
                                                                     this->writeSampleSize,
                                                                     separator);
    //ReadBytes + separator
    std::string result = " " + separator;
    result += readBytesString + separator;
    result += " " + separator;
    result += writebytesString + separator;
    result += std::to_string(totalBytesRead) + separator;
    result += std::to_string(totalBytesWritten) + separator;
    return result;
}

json CumulativeDiskInfo::toJSON() const
{
    json result;
    result["ReadBytes"] = datahelper::createMaxMinAvgJSON(this->maxBytesRead, 
                                                          this->minBytesRead, 
                                                          DiskInfo::bytesRead, 
                                                          datahelper::computeStandardDeviation(this->readVariance, 
                                                                                               this->readSampleSize));
    result["WriteBytes"] = datahelper::createMaxMinAvgJSON(this->maxBytesWritten, 
                                                           this->minBytesWritten, 
                                                           DiskInfo::bytesWritten, 
                                                           datahelper::computeStandardDeviation(this->writeVariance,
                                                                                                this->writeSampleSize));
    result["ReadTotal"] = totalBytesRead;
    result["WriteTotal"] = totalBytesWritten;
    return result;
}

CumulativeDiskInfo::~CumulativeDiskInfo(){}