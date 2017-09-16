#include "cumulativenetworkinfo.h"

CumulativeNetworkInfo::CumulativeNetworkInfo(const NetworkInfo* networkInfo)
                    :NetworkInfo(),
                     minBytesReceived(0),
                     maxBytesReceived(0),
                     totalBytesReceived(0),
                     receivedSampleSize(0),
                     receivedVariance(0),
                     receivedOldMean(0),
                     minBytesSent(0),
                     maxBytesSent(0),
                     totalBytesSent(0),
                     sentSampleSize(0),
                     sentVariance(0),
                     sentOldMean(0)
{
    if (networkInfo) 
    {
        if (networkInfo->bytesReceived >= 0 && networkInfo->bytesReceived < LONG_MAX)
        {
            minBytesReceived = networkInfo->bytesReceived;
            maxBytesReceived = networkInfo->bytesReceived;
            bytesReceived = networkInfo->bytesReceived;
            receivedOldMean = networkInfo->bytesReceived;
            receivedSampleSize++;
        }
        if (networkInfo->bytesSent >= 0 && networkInfo->bytesSent < LONG_MAX)
        {
            minBytesSent = networkInfo->bytesSent;
            maxBytesSent = networkInfo->bytesSent;
            bytesSent = networkInfo->bytesSent;
            sentOldMean = networkInfo->bytesSent;
            sentSampleSize++;
        }
        
    }
}

void CumulativeNetworkInfo::insert(const NetworkInfo* networkInfo) const 
{
    if (networkInfo) 
    {
        if (networkInfo->bytesReceived >= 0 && networkInfo->bytesReceived < LONG_MAX)
        {
            receivedSampleSize++;
            minBytesReceived = std::min(networkInfo->bytesReceived, this->minBytesReceived);
            maxBytesReceived = std::max(networkInfo->bytesReceived, this->maxBytesReceived);
            NetworkInfo::bytesReceived += datahelper::computeRunningAverage(networkInfo->bytesReceived,
                                                                            NetworkInfo::bytesReceived, 
                                                                            receivedSampleSize);
            receivedVariance += datahelper::computeRunningVariance(receivedVariance,
                                                                networkInfo->bytesReceived,
                                                                receivedOldMean, 
                                                                NetworkInfo::bytesReceived);
            receivedOldMean = NetworkInfo::bytesReceived;
        }
        if (networkInfo->bytesSent >= 0 && networkInfo->bytesSent < LONG_MAX)
        {
            sentSampleSize++;
            minBytesSent = std::min(networkInfo->bytesSent, this->minBytesSent);
            maxBytesSent = std::max(networkInfo->bytesSent, this->maxBytesSent);
            NetworkInfo::bytesSent += datahelper::computeRunningAverage(networkInfo->bytesSent, 
                                                                        NetworkInfo::bytesSent, 
                                                                        sentSampleSize);
            sentVariance += datahelper::computeRunningVariance(sentVariance, 
                                                            networkInfo->bytesSent,
                                                            sentOldMean,
                                                            NetworkInfo::bytesSent);
            sentOldMean = NetworkInfo::bytesSent;

        }


    }
}


void CumulativeNetworkInfo::setBytesTotal(const CumulativeNetworkInfo* prev) const
{
    if (prev)
    {
        totalBytesReceived = prev->totalBytesReceived + NetworkInfo::bytesReceived;
        totalBytesSent = prev->totalBytesSent + NetworkInfo::bytesSent;
    }
}

std::string CumulativeNetworkInfo::getHeader(std::string separator) const
{
    std::string tabHeader = "max:" + separator;
    tabHeader += "min:" + separator;
    tabHeader += "average:" + separator;
    tabHeader += "stddev:" + separator;

    std::string result =  "BytesReceived:" + separator + tabHeader;
    result += "BytesSent:" + separator + tabHeader;
    result += "TotalBytesReceived:" + separator;
    result += "TotalBytesSent:" + separator;
    return result;
}

std::string CumulativeNetworkInfo::toString(std::string separator) const
{
    std::string readBytesString = datahelper::createMaxMinAvgString(this->maxBytesReceived,
                                                                    this->minBytesReceived,
                                                                    NetworkInfo::bytesReceived,
                                                                    this->receivedVariance,
                                                                    this->receivedSampleSize,
                                                                    separator); 
    std::string bytesSentString = datahelper::createMaxMinAvgString(this->maxBytesSent,
                                                                    this->minBytesSent,
                                                                    NetworkInfo::bytesSent,
                                                                    this->sentVariance,
                                                                    this->sentSampleSize,
                                                                    separator);
    //Bytes Received
    std::string result = " " + separator;
    result += readBytesString + separator;

    //Bytes Sent
    result += " " + separator;
    result += bytesSentString + separator;
    
    //Totals
    result += std::to_string(totalBytesReceived) + separator;
    result += std::to_string(totalBytesSent) + separator;
    return result;
}

json CumulativeNetworkInfo::toJSON() const
{
    json result;
    result["ReceivedBytes"] = datahelper::createMaxMinAvgJSON(this->maxBytesReceived, 
                                                              this->minBytesReceived, 
                                                              NetworkInfo::bytesReceived, 
                                                              datahelper::computeStandardDeviation(this->receivedVariance, 
                                                                                                   this->receivedSampleSize));
    result["SentBytes"] = datahelper::createMaxMinAvgJSON(this->maxBytesSent, 
                                                           this->minBytesSent, 
                                                           NetworkInfo::bytesSent, 
                                                           datahelper::computeStandardDeviation(this->sentVariance, 
                                                                                                this->sentSampleSize));
    result["ReceivedTotal"] = totalBytesReceived;
    result["SentTotal"] = totalBytesSent;
    return result;
}


CumulativeNetworkInfo::~CumulativeNetworkInfo() {}