#include "config.h"

Config::Config(int interval, 
               bool printJSON, bool printCSV, bool printTSV, bool printToScreen, bool printIntermediateValues, 
               bool skipBattery, bool skipTemp,  bool skipDisk,  bool skipNetwork,  bool skipCPU)
        :interval(interval),
         printJSON(printJSON), printCSV(printCSV), printTSV(printTSV), printToScreen(printToScreen), printIntermediateValues(printIntermediateValues),
         skipBattery(skipBattery), skipTemp(skipTemp), skipCPU(skipCPU), skipDisk(skipDisk), skipNetwork(skipNetwork)
{

}

const int Config::getInterval(void) const
{
    return this->interval;
}

const bool Config::getPrintJSON(void) const
{
    return this->printJSON;
}

const bool Config::getPrintTSV(void) const
{
    return this->printTSV;
}

const bool Config::getPrintCSV(void) const
{
    return this->printCSV;
}

const bool Config::getPrintToScreen(void) const
{
    return this->printToScreen;
}

const bool Config::getPrintIntermediateValues(void) const
{
    return this->printIntermediateValues;
}

const bool Config::getSkipBattery(void) const
{
    return this->skipBattery;
}

const bool Config::getSkipTemp(void) const
{
    return this->skipTemp;
}

const bool Config::getSkipCPU(void) const
{
    return this->skipCPU;
}

const bool Config::getSkipDisk(void) const
{
    return this->skipDisk;
}

const bool Config::getSkipNetwork(void) const
{
    return this->skipNetwork;
}


Config::~Config() 
{

}