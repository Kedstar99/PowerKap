#ifndef PROCFS_H
#define PROCFS_H

#include <string>
#include <sstream>
#include <fstream>
#include <array>
#include <stdexcept>

#include "diskinfo.h"
#include "networkinfo.h"

class Procfs {
    public:
        Procfs(bool*);
        void detectCPU(void);
        const DiskInfo* getDiskInfo(int pid);
        const NetworkInfo* getNetworkInfo(int pid);
        bool checkAdapter(int pid);
        ~Procfs();
    private:
        bool detectNetworkInterface(int pid, std::string adapter);
        std::string printCPUModel(int cpu_model_code);
        const NetworkInfo* parseNetString(std::string);
        const DiskInfo* prevDiskInfo;
        const NetworkInfo* prevNetworkInfo;
        bool adapterPresent;
        bool* profilerState;


        // Some constants
        const std::string CPU_INFO_PATH  = "/proc/cpuinfo";
        const std::string PROC_STRING    = "/proc";
        const std::string NET_STRING     = "/net/dev";
        const std::string IO_STRING      = "/io";
        const std::string VETH_A         = "veth-a";
        const std::string VETH_B         = "veth-b";
        const std::string READ_BYTES     = "read_bytes";
        const std::string WRITE_BYTES    = "write_bytes";

        const int MAX_CPU_NAME 			= 22;
        const int MAX_CPUS			    = 1024;
        const int MAX_PACKAGES  	    = 16;
        const int ERROR_MESSAGE 		= 40;
        const int NUM_RAPL_DOMAINS		= 4;

        enum CpuCode
        {
            CPU_SANDYBRIDGE	        = 42,
            CPU_SANDYBRIDGE_EP	    = 45,
            CPU_IVYBRIDGE		    = 58,
            CPU_IVYBRIDGE_EP	    = 62,
            CPU_HASWELL		        = 60,
            CPU_HASWELL_EP		    = 63,
            CPU_BROADWELL		    = 61,
            CPU_BROADWELL_EP	    = 79,
            CPU_BROADWELL_DE	    = 86,
            CPU_SKYLAKE		        = 78,
            CPU_KABYLAKE            = 158
        };


};

#endif