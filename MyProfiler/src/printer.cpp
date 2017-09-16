#include "printer.h"

namespace printer 
{
   
    void printData(const std::vector< const Measurement* >* measurements, const Config* config, std::string dir) 
    {
        if (config && measurements && !measurements->empty())
        {
            if (config->getPrintJSON()) 
            {
                ::printJSON(measurements, config, dir);
            }
            if (config->getPrintCSV()) 
            {
                ::printCSV(measurements, config, dir);
            }
            if (config->getPrintTSV())
            {
                ::printTSV(measurements, config, dir);
            }
            if (config->getPrintToScreen())
            {
                ::printToScreen(measurements, config);
            }
        }
    }

    void printStartTime(std::chrono::system_clock::time_point timePoint, int run, int interval) 
    {
        std::ofstream elapsedTimeFile;
        if (run == 0) 
        {
            elapsedTimeFile.open("elapsedtimes.txt");
        } else {
            elapsedTimeFile.open("elapsedtimes.txt", std::ios_base::app);
        }
        if (!elapsedTimeFile.fail()) 
        {   
            elapsedTimeFile << "#########################" << "RUN:" << std::to_string(run) << "#########################" << std::endl;
            elapsedTimeFile << "0, " << datahelper::getStringFromTime(timePoint) << ", STARTPOINT, N/A, N/A, " << interval << std::endl;
            elapsedTimeFile.close();
        }
    }

}

namespace {
    void printJSON(const std::vector< const Measurement* >* measurements, const Config* config, std::string dir) 
    {
        if (config && measurements && !(measurements->empty())) 
        {
            std::ofstream logFile = filehelper::openLogFile(dir + ".json");
            json result;
            for(const Measurement* m: (*measurements))
            {
                if (m)
                {
                    result.push_back(m->toJSON(config));
                }
            }

            logFile<< std::setw(4) << result << std::endl;
        }
    }

    void printCSV(const std::vector< const Measurement* >* measurements, const Config* config, std::string dir) 
    {
        if (config && measurements && !(measurements->empty()))
        {
            if (measurements->front()) 
            {
                std::ofstream logFile = filehelper::openLogFile(dir + ".csv");
                ::printHeader(measurements->front(), logFile, config, std::string(", "));
                ::printMeasurements(measurements, logFile, config, std::string(", "));
                logFile.close();
            }
        }
    }

    void printTSV(const std::vector< const Measurement* >* measurements, const Config* config, std::string dir) 
    {
        if (config && measurements && !(measurements->empty())) 
        {
            if (measurements->front()) 
            {
                std::ofstream logFile = filehelper::openLogFile(dir + ".tsv");
                ::printHeader(measurements->front(), logFile, config, std::string("\t"));
                ::printMeasurements(measurements, logFile, config, std::string("\t"));
                logFile.close();
            }
        }
    }


    void printToScreen(const std::vector< const Measurement* >* measurements, const Config* config) 
    {
        if (config && measurements && !(measurements->empty())) 
        {
            if (measurements->front()) 
            {
                ::printHeader(measurements->front(), std::cout, config, std::string("|"));
                ::printMeasurements(measurements, std::cout, config, std::string("|"));
            }
        } 
    }


    void printMeasurements(const std::vector< const Measurement* >* measurements, std::ostream& o, const Config* config, std::string separator) 
    {
        if (o && config && measurements && !(measurements->empty())) 
        {
            for (const Measurement* m: (*measurements)) 
            {
                if (m) 
                {
                    auto result = m->toString(separator, config);
                    o << result << std::endl;
                }
            }
        }
    }

    void printHeader(const Measurement* m, std::ostream& o, const Config* config, std::string separator) 
    {
        if (config && m && o) 
        {
            auto result = m->getHeader(separator, config);
            o << result << std::endl;
        }
    }
}

