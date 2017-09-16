#include "cumulativemeasurement.h"

CumulativeMeasurement::CumulativeMeasurement(const ProcessedMeasurement* pMeasurement, 
                                             int interval)
                      :ProcessedMeasurement() 
{
    if (pMeasurement && interval > 0) 
    {
        if (pMeasurement->thermalData) 
        {
            ProcessedMeasurement::thermalData = new CumulativeThermalInfo(pMeasurement->thermalData);
        } 
        if (pMeasurement->batteryData) 
        {
            ProcessedMeasurement::batteryData = new CumulativeBatteryInfo(pMeasurement->batteryData);
        }
        if (pMeasurement->packageData) 
        {
            ProcessedMeasurement::packageData = new CumulativePackageInfo(pMeasurement->packageData);
        } 
        if (pMeasurement->diskData)
        {
            ProcessedMeasurement::diskData = new CumulativeDiskInfo(pMeasurement->diskData);
        }
        if (pMeasurement->networkData)
        {
            ProcessedMeasurement::networkData = new CumulativeNetworkInfo(pMeasurement->networkData);
        }

        ProcessedMeasurement::elapsedTime = pMeasurement->elapsedTime - (pMeasurement->elapsedTime % interval);
    }
}

CumulativeMeasurement::CumulativeMeasurement() 
                      :ProcessedMeasurement(){}


void CumulativeMeasurement::insert(const ProcessedMeasurement* measurement, int interval) const 
{
    if (measurement) 
    {
        if (ProcessedMeasurement::elapsedTime == (measurement->elapsedTime - (measurement->elapsedTime % interval)) ) 
        {
            if (measurement->thermalData && ProcessedMeasurement::thermalData) 
            {
                ProcessedMeasurement::thermalData->insert(measurement->thermalData);
            }
            if (measurement->batteryData && ProcessedMeasurement::batteryData) 
            {
                ProcessedMeasurement::batteryData->insert(measurement->batteryData);
            }
            if (measurement->packageData && ProcessedMeasurement::packageData) 
            {
                ProcessedMeasurement::packageData->insert(measurement->packageData);
            }
            /*
            if (measurement->diskData && ProcessedMeasurement::diskData)
            {
                ProcessedMeasurement::diskData->insert(measurement->diskData);
            } */
            
            if (measurement->networkData && ProcessedMeasurement::networkData)
            {
                ProcessedMeasurement::networkData->insert(measurement->networkData);
            }

        }
    }
}

void CumulativeMeasurement::setTotals(const CumulativeMeasurement* prev) const
{
    if (prev) 
    {
        auto prevPckData = dynamic_cast<const CumulativePackageInfo*> (prev->packageData);
        auto pckData = dynamic_cast<const CumulativePackageInfo*> (ProcessedMeasurement::packageData);
        if (prevPckData && pckData) 
        {
            pckData->setEnergyTotal(prevPckData);
        }
        auto prevDiskData = dynamic_cast<const CumulativeDiskInfo*> (prev->diskData);
        auto diskData = dynamic_cast<const CumulativeDiskInfo*> (ProcessedMeasurement::diskData);
        if (prevDiskData && diskData) 
        {
            diskData->setBytesTotal(prevDiskData);
        }
        auto prevNetworkData = dynamic_cast<const CumulativeNetworkInfo*> (prev->networkData);
        auto networkData = dynamic_cast<const CumulativeNetworkInfo*> (ProcessedMeasurement::networkData);
        if (prevNetworkData && networkData)
        {
            networkData->setBytesTotal(prevNetworkData);
        }
    }
}


CumulativeMeasurement::~CumulativeMeasurement() {}