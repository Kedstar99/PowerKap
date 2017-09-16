#ifndef FILEHELPER_H
#define FILEHELPER_H

#include <fstream>
#include <string>
#include <chrono>
#include <thread>


namespace filehelper {

    bool fileDirExists(std::string dir);
    bool testDirForActivity(std::string dir);
    long long readLongFromFile(std::string dir);
    std::string readLineFromFile(std::string dir);
    std::ofstream openLogFile(std::string fileName);

}

#endif
