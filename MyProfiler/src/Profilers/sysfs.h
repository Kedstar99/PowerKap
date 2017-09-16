#ifndef SYSFS_H
#define SYSFS_H

#include "cpuinfo.h"
#include "cpuzone.h"
#include "batteryinfo.h"
#include "thermalinfo.h"
#include "packageinfo.h"
#include "filehelper.h"
#include "config.h"
#include <vector>
#include <array>
#include <string>

class Sysfs {
    public:
        Sysfs();

        // Call this before using to detect valid interfaces.
        void initialize();

        const ThermalInfo* getThermalInfo(void);
        const BatteryInfo* getBatteryInfo(void);
        const PackageInfo* getPackageInfo(void);
        ~Sysfs();

    private:
        const CpuInfo* readCPUInfo(int);
        const CpuZone* readCPUZone(std::string dir);
        
        void setActiveThermalZones(void);
        void setActivePowerZones(int);
        
        long long getEnergyInfo(int);
        
        std::vector<int> activeThermalZones;
        std::array< std::array<bool, 4>, 2> activePowerZones;


        // Some constants
        const std::string SYSFS_RAPL_BASEDIR           = "/sys/class/powercap/intel-rapl/intel-rapl:";
        const std::string SYSFS_THERMAL_BASEDIR        = "/sys/class/thermal/thermal_zone";
        const std::string SYSFS_LAPTOP_BATTERY_STATUS  = "/sys/class/power_supply/BAT0/status";
        const std::string SYSFS_LAPTOP_BATTERY_VOLTAGE = "/sys/class/power_supply/BAT0/current_now";
        const std::string SYSFS_LAPTOP_BATTERY_CURRENT = "/sys/class/power_supply/BAT0/voltage_now";
        const std::string RAPL_STRING                  = "/intel-rapl:";
        const std::string ENERGY_UJ                    = "/energy_uj";
        const std::string POWER_UJ                     = "/power_uj";
        const std::string DOMAIN_NAME_STRING           = "/name";
        const std::string TEMP_STRING                  = "/temp";
        const std::string TYPE_STRING                  = "/type";
        const std::string DISCHARGE_STRING             = "Discharging";

        const int PACKAGE_0                            = 0;
        const int PACKAGE_1                            = 1;
        const int CORE                                 = 0;
        const int UNCORE                               = 1;
        const int DRAM                                 = 2;
        const int PACKAGE_BOOL                         = 3;
};

#endif