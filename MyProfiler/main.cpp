#include "profiler.h"
#include "forker.h"
#include "config.h"
#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

#define ONE_SECOND_MILLISECONDS 1000
#define ONE_MINUTE 60
#define INSTALLDIR "/usr/local/bin/"

Profiler* profiler_ptr = NULL;

//This singal handler just tells the program to stop profiling when the child process exits
void sigchld_handler(int sig)
{
    if (profiler_ptr) {
        profiler_ptr->stopProfiling(false);
    }
    forker::resetPID();
}

//Figure out how to kill zombie processes
void sigterm_handler(int sig) 
{
    std::cout << "stopping the program" << std::endl;
    if (profiler_ptr) {
        profiler_ptr->stopProfiling(true);
        std::cout << "waiting for the program to finish" << std::endl;
        sleep(5);
        std::cout << "Freeing memory" << std::endl;
        profiler_ptr->resetProfiler();

    }
    forker::killProcess(profiler_ptr);
    std::cout << "GoodBye" << std::endl;
    exit(0);
}

void runInitScript() 
{
    std::string shScript = std::string(INSTALLDIR) + std::string("enableVirtNet");
    if (system(shScript.c_str()) <  0)
    {
        std::cerr << "Unable to initialise network adapters" << std::endl;
    }
}



int main(int numArgs, char **args, char **envp) 
{

    // Initial flags and parameters
    int runs = 3;
    char* execProg = NULL;
    bool runExternalProg = false;
    bool timeoutEnabled = false;
    bool skipBackgroundProfile = false;
    bool skipCPU = false;
    bool skipBattery = false;
    bool skipNetwork = false;
    bool skipDisk = false;
    bool skipTemp = false;
    bool printJSON = false;
    bool printCSV = false;
    bool printTSV = false;
    bool printToScreen = false;
    bool printIntermediateValues = false;
    bool init = false;
    int timeout = ONE_MINUTE;
    int externalProgIndex = 0;
    int interval = ONE_SECOND_MILLISECONDS;
    
    int c;
    /* Parse command line arguments */
    while((c = getopt(numArgs, args, "t:r:i:eszxcvbghjkla")) != -1) 
    {
        switch(c) 
        {
            case 't':
                if (optarg != NULL) 
                {
                    timeout = (int) strtol(optarg, (char **) NULL, 10);
                    timeoutEnabled = true;
                }
                break;
            case 'r':
                if (optarg != NULL) 
                {
                    runs = (int) strtol(optarg, (char **) NULL, 10);
                }
                break;
            case 'i' :
                if (optarg != NULL) 
                {
                    interval = (int) strtol(optarg, (char **) NULL, 10);
                }   
                break;
            case 'e':
                runExternalProg = true;
                break;
            case 's' :
                skipBackgroundProfile = true;
                break;
            case 'z' :
                printIntermediateValues = true;
                break;
            case 'x':
                printToScreen = true;
                break;
            case 'c':
                printJSON = true;
                break;
            case 'v':
                printCSV = true;
                break;
            case 'b':
                printTSV = true;
                break;
            case 'g':
                skipNetwork = true;
                break;
            case 'h':
                skipCPU = true;
                break;
            case 'j':
                skipDisk = true;
                break;
            case 'k' :
                skipBattery = true;
                break;
            case 'l' :
                skipTemp = true;
                break;
            case 'a':
                init = true;
                break;
            default:
                break;
        }  
        if (runExternalProg) 
        {
            externalProgIndex = optind + 1;
        }
    }
    if (init) 
    {
        runInitScript();
        return EXIT_SUCCESS;
    }

    if (skipCPU && skipDisk && skipCPU && skipTemp && skipNetwork) 
    {
        return EXIT_SUCCESS;
    }
    Config config = Config(interval, 
                             printJSON, printCSV, printTSV, printToScreen, printIntermediateValues, 
                             skipBattery, skipTemp, skipDisk, skipNetwork, skipCPU);
    //Setup the profiler based on user flags
    Profiler profiler = Profiler(&config);
    profiler.detectSystem();
    profiler_ptr = &profiler;

    signal(SIGCHLD, sigchld_handler);
    signal(SIGINT, sigterm_handler);
    signal(SIGTERM, sigterm_handler);

    if (runExternalProg) 
    {
        //Setup forked process arguments from -e flag
        int numberOfArgs = numArgs - externalProgIndex + 1;
        char *parmList[numberOfArgs];
        for (int i = externalProgIndex; i < numArgs; i++) {
            if (args[i]) {
                parmList[i - externalProgIndex] = args[i];
            }
        } 
        parmList[numberOfArgs - 1] = NULL;
        timeout = forker::programProfiling(profiler_ptr, parmList, envp, externalProgIndex, runs, timeout, timeoutEnabled);
    }
    
    if (!skipBackgroundProfile) 
    {
        forker::backgroundProfiling(profiler_ptr, envp, runs, timeout);
    }   
    
    profiler_ptr = NULL;
    return EXIT_SUCCESS;
}
