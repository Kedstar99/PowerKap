#ifndef PROFILER_H
#define PROFILER_H

#define MILLISECONDS_TO_SECONDS 1000
#define TIME_TO_STOP 5000

#include "sysfs.h"
#include "printer.h"
#include "procfs.h"
#include "measurement.h"
#include "processedmeasurement.h"
#include "cumulativemeasurement.h"
#include "rawmeasurement.h"
#include "config.h"
#include <vector>
#include <map>
#include <chrono>
#include <climits>
#include <stdexcept>


class Profiler{
    public:
        // Constructor which takes a conifugration pointer. Takes ownership of the configuration pointer
        Profiler(const Config*);
        // Returns the CPU details
        void detectSystem(void);
        
        // Check if the profiler isRunning, currently unused.
        bool isRunning(void);
        
        // Profiler that takes a timeout, a boolean for if timeout is enabled, and a pointer to a pid 
        void startProfiling(int, bool, int*);

        void stopProfiling(bool);

        // reset and clear all heap allocated data
        void resetProfiler(void);

        // tell the printer to print various data, takes a directory 
        void printProcessedData(std::string dir);
        void printCumulativeData(std::string dir);
        void printMeasurementData(std::string dir);

        // Returns the total time(seconds) taken for gathering the results
        int getElapsedTimeSeconds(void);

        // Tells the printer to print a start time for profiling. This is necessary for LJEA as a reference point.
        void printStartTime(void);

        // sets printer strings
        void setProgramTag(void);
        void setBackgroundTag(void);

        // Gathers statistics such as cumulative totals, that are gathered after gathering all the measurements.
        void processCumulativeResults(void);

        //checks if the network namespace is setup
        void checkAdapter(int pid);
        ~Profiler();

      
    private:
        void profileSingularResult(int);

        // Consolidates the raw results gathered into ProcessedMeasurements and CumulativeMeasurements
        void processRun(void);
        void clearRun(void);

        bool stopCurrentRun;
        std::vector< const Measurement*> measurements;
        std::vector< long long > elapsedTimes;
        std::vector< const Measurement* > processedResults;
        std::vector< const Measurement* > cumulativeResults;
        std::string profileTag;    
        int run;
        const Config* config;
        Sysfs sysfs;
        Procfs procfs;
};

#endif