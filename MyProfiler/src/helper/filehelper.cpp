#include "filehelper.h"

namespace filehelper {

    bool fileDirExists(std::string dir) 
    {
        std::ifstream file(dir);
        bool result = false;
        if (!file.fail()) 
        {
            result = true;
        }
        file.close();
        return result;
    }


    bool testDirForActivity(std::string dir) 
    {
        bool result = true;
        long long measurementOne = readLongFromFile(dir);
        if (measurementOne == 0L) 
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            measurementOne = readLongFromFile(dir);
            result = measurementOne != 0L;
        }
        return result;
    }

    long long readLongFromFile(std::string dir) 
    {
        std::ifstream file(dir);
        long long result = 0L;
        if (!file.fail()) 
        {
            std::string line = "";
            getline(file, line);
            if (!line.empty()) 
            {
                result = std::stoll(line);
            }
            file.close();
        }

        return result;
    }

    std::string readLineFromFile(std::string dir) 
    {
        std::ifstream file(dir);
        std::string result = "";
        if (!file.fail()) 
        {
            std::getline(file, result);
            file.close();
        }
        return result;
    }

    std::ofstream openLogFile(std::string fileName) 
    {
        std::ofstream logFile;
        logFile.open(fileName, std::ios::app);
        if (!logFile.fail()) 
        {
            return logFile;
        } else 
        {
            throw "Error opening file";
        }
    }
}