#include "profiler.h"

Profiler::Profiler(const Config* config)
         :run(0),
          config(config),
          profileTag(""),
          sysfs(Sysfs()),
          procfs(Procfs(&stopCurrentRun))
{   
    this->sysfs.initialize();
    this->elapsedTimes = std::vector<long long>();
    this->measurements = std::vector<const Measurement*>();
    this->processedResults = std::vector<const Measurement*>();
    this->cumulativeResults = std::vector<const Measurement*>();
}

void Profiler::setBackgroundTag() {
    this->profileTag = "Background-";
}

void Profiler::setProgramTag() {
    this->profileTag = "Program-";
}


void Profiler::detectSystem() 
{
    procfs.detectCPU();
}

void Profiler::checkAdapter(int pid)
{
    if (config && !config->getSkipNetwork())
    {
        procfs.checkAdapter(pid);
    }
}

//Unused
bool Profiler::isRunning(void) 
{
    return this->stopCurrentRun;
}

void Profiler::profileSingularResult(int pid) 
{
    if (config) 
    {
        auto timePoint = std::chrono::system_clock::now();
        auto thermalData = (config->getSkipTemp() ? NULL : sysfs.getThermalInfo());
        auto packageData = (config->getSkipCPU() ? NULL : sysfs.getPackageInfo());
        auto batteryData = (config->getSkipBattery() ? NULL : sysfs.getBatteryInfo());
        auto networkData = (config->getSkipNetwork() ? NULL : procfs.getNetworkInfo(pid));
        auto diskData = (config->getSkipDisk() ? NULL : procfs.getDiskInfo(pid));
        measurements.push_back(new RawMeasurement(thermalData, batteryData, packageData, diskData, networkData, timePoint, run));
    } else {
        throw std::logic_error("Config needs to be set before profiling");
    }
}


//Simple loop that profiles the system
void Profiler::startProfiling(int timeout, bool timeoutEnabled, int* pid) 
{
    if (config && pid && *pid > 0) 
    {
        this->stopCurrentRun = false;
        auto start = std::chrono::system_clock::now();
        auto now = std::chrono::system_clock::now();
        auto cumulativeElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - start);
        profileSingularResult(*pid);

        elapsedTimes.push_back(cumulativeElapsed.count());
        std::this_thread::sleep_for(std::chrono::milliseconds(config->getInterval()));
        while(!this->stopCurrentRun) 
        {
            now = std::chrono::system_clock::now();
            cumulativeElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - start);
            elapsedTimes.push_back(cumulativeElapsed.count());
            profileSingularResult(*pid);
            if (timeoutEnabled && (cumulativeElapsed.count() / MILLISECONDS_TO_SECONDS) > timeout ) 
            {
                stopProfiling(false);
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(config->getInterval()));
        }
    } 
}

int Profiler::getElapsedTimeSeconds(void) 
{
    long long elapsedTime = 0;
    if (!cumulativeResults.empty()) 
    {
        for (auto &elem: cumulativeResults) 
        {
            auto m = dynamic_cast<const CumulativeMeasurement*>(elem);
            if (m) 
            {
                elapsedTime = std::max(elapsedTime, m->elapsedTime);
            }
        }
    }
    return (int) (elapsedTime / MILLISECONDS_TO_SECONDS);
}


//Simple function that stops the program from profiling and prints the results
void Profiler::stopProfiling(bool userStop) 
{
    this->stopCurrentRun = true;
    std::this_thread::sleep_for(std::chrono::milliseconds(TIME_TO_STOP));
    if (!userStop) 
    {
        this->printMeasurementData(profileTag);
        this->run++;
        this->processRun();
        this->printProcessedData(profileTag);
    }
    this->clearRun();
}

void Profiler::printMeasurementData(std::string dir) 
{
    if (config && config->getPrintIntermediateValues() && !measurements.empty())
    {
        printer::printData(&measurements, config, dir + "RawMeasurements");
    }
}

void Profiler::printCumulativeData(std::string dir) 
{
    if (!cumulativeResults.empty()) 
    {
        printer::printData(&cumulativeResults, config, dir + "CumulativeMeasurements");
    }
}

void Profiler::printProcessedData(std::string dir) 
{
    if (config && config->getPrintIntermediateValues() && !processedResults.empty()) 
    {
        printer::printData(&processedResults, config, dir + "ProcessedMeasurements");
    }
}

void Profiler::printStartTime(void) 
{
    if (config) 
    {
        printer::printStartTime(std::chrono::system_clock::now(), this->run, config->getInterval());
    }
}

void Profiler::processRun(void) 
{
    if (config) 
    {
        int interval = config->getInterval();

        if (elapsedTimes.size() == measurements.size() && elapsedTimes.size() > 1 ) 
        {
            std::vector< long long >::iterator index;
            auto prev = measurements.front();
            long long prevElapsedTimes = elapsedTimes.front();
            for (index = elapsedTimes.begin() + 1; index != elapsedTimes.end(); index++) 
            { 
                auto curr = measurements.at(index - elapsedTimes.begin());
                double diffTime = (double) (*index - prevElapsedTimes) / MILLISECONDS_TO_SECONDS;
                auto processedmeasurement = new ProcessedMeasurement(curr, prev, *index, diffTime);
                prevElapsedTimes = *index;
                prev = curr;
                
                processedResults.push_back(processedmeasurement);

                //Handle cumulative measurements
                auto timeIndex = *index - (*index % interval);
                bool notFound = true;
                for (auto &elem: cumulativeResults) 
                {
                    auto cumulativeMeasurement = dynamic_cast<const CumulativeMeasurement*> (elem);
                    if (cumulativeMeasurement && cumulativeMeasurement->elapsedTime == timeIndex)
                    {
                        cumulativeMeasurement->insert(processedmeasurement, interval);
                        notFound = false;
                        break;
                    }
                }
                if (notFound)
                {
                    auto cumulativeMeasurement = new CumulativeMeasurement(processedmeasurement, interval);
                    cumulativeResults.push_back(cumulativeMeasurement);
                }

            }
        }
    }

}

void Profiler::processCumulativeResults() 
{
    if (!cumulativeResults.empty()) 
    {
        auto iter = cumulativeResults.begin();
        auto prev = dynamic_cast<const CumulativeMeasurement*>(*iter);
        for (iter; iter != cumulativeResults.end(); iter++) 
        {
            auto cumulativeMeasurement = dynamic_cast<const CumulativeMeasurement*>(*iter);
            if (cumulativeMeasurement && prev) 
            {
                cumulativeMeasurement->setTotals(prev);
                prev = cumulativeMeasurement;
            }
        }

    }
}


void Profiler::clearRun() 
{
    this->elapsedTimes.clear();

    if (!measurements.empty()) 
    {
        for (auto& elem: measurements) 
        {
            delete elem;
            elem = NULL;
        }
        this->measurements.clear();
    }

    if (!processedResults.empty()) 
    {
        for (auto& elem: processedResults) 
        {
            delete elem;
            elem = NULL;
        }
        this->processedResults.clear();
    }

}

void Profiler::resetProfiler(void) {
    this->clearRun();
    if (!cumulativeResults.empty()) 
    {
        for (auto& elem: cumulativeResults) 
        {
            delete elem;
            elem = NULL;
        }
        this->cumulativeResults.clear();
    }
    run = 0;
}

Profiler::~Profiler() 
{
    this->resetProfiler();
}
