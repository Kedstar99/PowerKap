#include "cumulativepackageinfo.h"

CumulativePackageInfo::CumulativePackageInfo(const PackageInfo* pckInfo)
                      :PackageInfo(),
                       sampleSize(0)
{
    if (pckInfo) 
    {
        if (pckInfo->packageOne) 
        {
            PackageInfo::packageOne = new CumulativeCpuInfo(pckInfo->packageOne);
        }
        if (pckInfo->packageTwo) 
        {
            PackageInfo::packageTwo = new CumulativeCpuInfo(pckInfo->packageTwo);
        }
        sampleSize++;
    } 
}


void CumulativePackageInfo::setEnergyTotal(const CumulativePackageInfo* prev) const
{
    if (prev) 
    {
        auto pckOne = dynamic_cast<const CumulativeCpuInfo*>(PackageInfo::packageOne);
        auto prevPckOne = dynamic_cast<const CumulativeCpuInfo*>(prev->packageOne);
        if (pckOne && prevPckOne) 
        {
            pckOne->setEnergyTotal(prevPckOne);
        }
        auto pckTwo = dynamic_cast<const CumulativeCpuInfo*>(PackageInfo::packageTwo);
        auto prevPckTwo = dynamic_cast<const CumulativeCpuInfo*>(prev->packageTwo);
        if (pckTwo && prevPckTwo) 
        {
            pckTwo->setEnergyTotal(prevPckTwo);
        }
    }
}


void CumulativePackageInfo::insert(const PackageInfo* pckInfo) const 
{
    if (pckInfo) 
    {
        sampleSize++;
        if (PackageInfo::packageOne && pckInfo->packageOne) 
        {
            PackageInfo::packageOne->insert(pckInfo->packageOne);
        }
        if (PackageInfo::packageTwo && pckInfo->packageTwo) 
        {
            PackageInfo::packageTwo->insert(pckInfo->packageTwo);
        }
    }
}

CumulativePackageInfo::~CumulativePackageInfo() {}