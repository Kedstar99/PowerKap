#ifndef CONFIG_H
#define CONFIG_H

class Config {
    public:
        Config(int interval, 
               bool printJSON, 
               bool printCSV, 
               bool printTSV, 
               bool printToScreen, 
               bool printIntermediateValues, 
               bool skipBattery,
               bool skipTemp, 
               bool skipDisk, 
               bool skipNetwork, 
               bool skipCPU);
        ~Config();
        const int getInterval(void) const;
        const bool getPrintJSON(void) const;
        const bool getPrintTSV(void) const;
        const bool getPrintCSV(void) const;
        const bool getPrintToScreen(void) const;
        const bool getPrintIntermediateValues(void) const;
        const bool getSkipBattery(void) const;
        const bool getSkipTemp(void) const;
        const bool getSkipCPU(void) const;
        const bool getSkipDisk(void) const;
        const bool getSkipNetwork(void) const;
    private:
        const int interval;
        const bool printJSON;
        const bool printCSV;
        const bool printTSV;
        const bool printToScreen;
        const bool printIntermediateValues; 
        const bool skipBattery;
        const bool skipTemp;
        const bool skipCPU;
        const bool skipDisk;
        const bool skipNetwork;
};

#endif