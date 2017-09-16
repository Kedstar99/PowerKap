#ifndef PRINTER_H
#define PRINTER_H

#include <vector>
#include <map>
#include <ostream>
#include <chrono>
#include <unistd.h>
#include <fstream>
#include <climits>
#include "measurement.h"
#include "filehelper.h"
#include "datahelper.h"
#include "config.h"

namespace printer 
{
    //Prints the given vector of measurements, config and dir. Does not take ownership of the vector, measurements or config.
    void printData(const std::vector< const Measurement* >* measurements, const Config* config, std::string dir); 

    void printStartTime(std::chrono::system_clock::time_point timePoint, int run, int interval);
}

namespace
{
    void printJSON(const std::vector< const Measurement* >* measurements, const Config* config, std::string dir);
    void printCSV(const std::vector< const Measurement* >* measurements, const Config* config, std::string dir);
    void printTSV(const std::vector< const Measurement* >* measurements, const Config* config, std::string dir);
    void printToScreen(const std::vector< const Measurement* >* measurements, const Config* config);
    void printHeader(const Measurement* m, std::ostream& o, const Config* config, std::string separator);
    void printMeasurements(const std::vector< const Measurement* >* measurements, std::ostream& o, const Config* config, std::string separator);
}

#endif