#include "sysfs.h"

Sysfs::Sysfs()
{	
	activePowerZones = {{{false, false, false, false}, {false, false, false, false}}};
}

void Sysfs::initialize()
{
	setActiveThermalZones();
	setActivePowerZones(PACKAGE_0);
	setActivePowerZones(PACKAGE_1);
}

const ThermalInfo* Sysfs::getThermalInfo()
{
	const ThermalInfo* result = NULL;
	result = new ThermalInfo();
	for (auto elem : activeThermalZones) 
	{
		std::string tempDir = SYSFS_THERMAL_BASEDIR + std::to_string(elem);
		std::string temperaturePath = tempDir +  TEMP_STRING;
		std::string namePath = tempDir + TYPE_STRING;
		result->addMeasurement(filehelper::readLineFromFile(namePath), filehelper::readLongFromFile(temperaturePath));
	}
	return result;
}

const PackageInfo* Sysfs::getPackageInfo() 
{
	const PackageInfo* result = NULL;
	const CpuInfo* cpuOne = NULL;
	const CpuInfo* cpuTwo = NULL;
	if (activePowerZones.at(PACKAGE_0).at(PACKAGE_BOOL)) 
	{
		cpuOne = readCPUInfo(PACKAGE_0);
	}
	if (activePowerZones.at(PACKAGE_1).at(PACKAGE_BOOL)) 
	{
		cpuTwo = readCPUInfo(PACKAGE_1);
	}
	if (cpuOne || cpuTwo) 
	{
		result = new PackageInfo(cpuOne, cpuTwo);
	}
	return result;
}


//Takes a packageID and returns the energy values from it's file
long long Sysfs::getEnergyInfo(int package) 
{
	std::string basePath = SYSFS_RAPL_BASEDIR + std::to_string(package);
	std::string energyPath = basePath + ENERGY_UJ;
	return filehelper::readLongFromFile(energyPath);
}

const BatteryInfo* Sysfs::getBatteryInfo() 
{
	const BatteryInfo* result = NULL;
	if (filehelper::readLineFromFile(SYSFS_LAPTOP_BATTERY_STATUS) == DISCHARGE_STRING) 
	{
		long long microCurrent = filehelper::readLongFromFile(SYSFS_LAPTOP_BATTERY_CURRENT);
		long long microVoltage = filehelper::readLongFromFile(SYSFS_LAPTOP_BATTERY_VOLTAGE);
		result = new BatteryInfo(microCurrent, microVoltage);
	}
	return result;
}


const CpuInfo* Sysfs::readCPUInfo(int package) 
{	
	const CpuInfo* result = NULL;
	std::string basePath = SYSFS_RAPL_BASEDIR + std::to_string(package);
	std::string corePath = basePath + RAPL_STRING + std::to_string(package) + ":" + std::to_string(CORE);
	std::string uncorePath = basePath + RAPL_STRING + std::to_string(package) + ":" + std::to_string(UNCORE);
	std::string dramPath = basePath + RAPL_STRING + std::to_string(package) + ":" + std::to_string(DRAM);
	std::string energyPath = basePath + ENERGY_UJ;
	std::string powerPath = basePath + POWER_UJ;
	std::string namePath = basePath + DOMAIN_NAME_STRING;
	long long energyCtr = filehelper::readLongFromFile(energyPath);
	long long microPwr = filehelper::readLongFromFile(powerPath);
	std::string name = filehelper::readLineFromFile(namePath);
	const CpuZone* core = NULL;
	const CpuZone* uncore = NULL;
	const CpuZone* dram = NULL;
	auto array = activePowerZones.at(package);
	if (array.at(CORE)) 
	{
		core = readCPUZone(corePath);
	}
	if (array.at(UNCORE)) 
	{
		uncore = readCPUZone(uncorePath);
	}
	if (array.at(DRAM)) 
	{
		dram = readCPUZone(dramPath);
	}
	if (energyCtr != 0L || microPwr != 0L || core || uncore || dram) {
		result = new CpuInfo(energyCtr, microPwr, name, core, uncore, dram);
	}
	return result;
}

const CpuZone* Sysfs::readCPUZone(std::string dir) 
{
	const CpuZone* result = NULL;
	std::string energyPath = dir + ENERGY_UJ;
	std::string powerPath = dir + POWER_UJ;
	result = new CpuZone(filehelper::readLongFromFile(energyPath), filehelper::readLongFromFile(powerPath));
	return result;
}


void Sysfs::setActivePowerZones(int package) 
{
	std::string basePath = SYSFS_RAPL_BASEDIR + std::to_string(package);
	std::string corePath = basePath + RAPL_STRING + std::to_string(package) + ":" + std::to_string(CORE);
	std::string uncorePath = basePath + RAPL_STRING + std::to_string(package) + ":" + std::to_string(UNCORE);
	std::string dramPath = basePath + RAPL_STRING + std::to_string(package) + ":" + std::to_string(DRAM);
	activePowerZones.at(package).at(CORE) = filehelper::fileDirExists(corePath);
	activePowerZones.at(package).at(UNCORE) = filehelper::fileDirExists(uncorePath);
	activePowerZones.at(package).at(DRAM) = filehelper::fileDirExists(dramPath);
	for (int i = 0; i < 3; i++) 
	{
		if (activePowerZones.at(package).at(i)) 
		{
			activePowerZones.at(package).at(PACKAGE_BOOL) = true;
			break;
		}
	}
}

void Sysfs::setActiveThermalZones() 
{
	std::string testDir = SYSFS_THERMAL_BASEDIR;
	for (int i = 0; i < 255; i++) 
	{
		if (filehelper::fileDirExists(SYSFS_THERMAL_BASEDIR + std::to_string(i) + TYPE_STRING)) 
		{
			if (filehelper::testDirForActivity(SYSFS_THERMAL_BASEDIR + std::to_string(i) + TEMP_STRING)) 
			{
				activeThermalZones.push_back(i);
			}
		} 
		else 
		{
			break;
		}
	}
}

Sysfs::~Sysfs() 
{

}