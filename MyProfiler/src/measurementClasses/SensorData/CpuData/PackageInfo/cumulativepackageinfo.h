#ifndef CUMULATIVE_PACKAGE_INFO_H
#define CUMULATIVE_PACKAGE_INFO_H

#include "cumulativecpuinfo.h"
#include "packageinfo.h"

class CumulativePackageInfo: public PackageInfo {
    public:
          CumulativePackageInfo(const PackageInfo* pckInfo);
          void insert (const PackageInfo* pckInfo) const;
          ~CumulativePackageInfo();
          void setEnergyTotal(const CumulativePackageInfo* pckInfo) const;
          
          mutable int sampleSize;
};

#endif