#include "packageinfo.h"

PackageInfo::PackageInfo(const CpuInfo* packageOne, 
                         const CpuInfo* packageTwo)
            :packageOne(packageOne), 
             packageTwo(packageTwo) {}

PackageInfo::PackageInfo()
            :packageOne(NULL),
             packageTwo(NULL) {}

std::string PackageInfo::toString(std::string separator) const 
{
    std::string result = "";
    if (packageOne) 
    {
        result += packageOne->toString(separator);
    }
    if (packageTwo) 
    {
        result += packageTwo->toString(separator);
    }
    return result;
}


std::string PackageInfo::getHeader(std::string separator) const 
{
    std::string result = "";
    if (packageOne) 
    {
        result += packageOne->getHeader(separator);
    }
    if (packageTwo) 
    {
        result += packageTwo->getHeader(separator);
    }
    return result;
}

json PackageInfo::toJSON() const
{
    json result;
    if (packageOne)
    {
        result["PackageOne"] = packageOne->toJSON();
    }
    if (packageTwo)
    {
        result["PackageTwo"] += packageTwo->toJSON();
    }
    return result;
}

const PackageInfo* PackageInfo::clone() const
{
    const CpuInfo* one = (packageOne ? packageOne->clone() : NULL);
    const CpuInfo* two = (packageTwo ? packageTwo->clone() : NULL);

    return new PackageInfo(one, two);
}

const PackageInfo* PackageInfo::process(const PackageInfo* prev, double elapsedTime) const
{
    const CpuInfo* one = NULL;
    const CpuInfo* two = NULL;
    if (prev) 
    {
        if (packageOne && prev->packageOne) 
        {
            one = packageOne->process(prev->packageOne, elapsedTime);
        }
        if (packageTwo && prev->packageTwo) 
        {
            two = packageTwo->process(prev->packageTwo, elapsedTime);
        }
    }
    return new PackageInfo(one, two);
}

void PackageInfo::insert(const PackageInfo* pckInfo) const {}


PackageInfo::~PackageInfo() 
{
    if (packageOne) 
    {
        delete packageOne;
        packageOne = NULL;
    }
    if (packageTwo) 
    {
        delete packageTwo;
        packageTwo = NULL;
    }
}