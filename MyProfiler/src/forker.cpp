#include "forker.h"

namespace forker{
    int backgroundProfiling(Profiler* profiler, char** envp, int runs, int timeout) 
    {
        int currentRun = 0;
        int elapsedTime = 0;
        std::cout << "Runs:" << runs << ' ' << "timeout:" << timeout << std::endl;
        if (profiler) 
        {

            //Profile the system for a baseline
            profiler->setBackgroundTag();
            while(currentRun != runs) 
            {

                //Setup the child stack and arguments
                char stack[STACK_SPACE];
                struct ::cloneArgs childArgs;
                std::string std_timeout = std::to_string(timeout);
                std::vector<char*> argv;
                std::string sleepProg = std::string(SLEEP_PROG);
                argv.push_back(&sleepProg[0]);
                argv.push_back(&std_timeout[0]);
                argv.push_back(nullptr);

                childArgs.args = &argv[0];
                childArgs.envp = envp;

                std::cout << "Profiling the Background" << "\n";
                
                // On Linux systems, the stack space grows downwards, on other systems that may be the case. Safest bet is start the child process halfway down the stack.
                int pid = clone(childProg, stack + (STACK_SPACE / 2), CLONE_NEWPID | CLONE_NEWNS | CLONE_VFORK | SIGCHLD , &childArgs ); 
                if (pid > 0) 
                {
                    ::pid = pid;
                    profiler->checkAdapter((int) pid);
                    profiler->startProfiling(timeout, false, &(::pid));

                    int returnStatus;
                    auto done = wait(&returnStatus);
                    if (done > 0) 
                    {
                        std::cout << "Background run:" << currentRun << " terminated successfully" << "\n";
                    } else if (done == -1) {
                        if (errno != ECHILD) 
                        {
                            std::cerr << "error handling child process stopping execution" << std::endl;
                            break;
                        }
                    }
                    currentRun++;
                } else {
                    std::cerr << "Clone error stopping execution" << std::endl;
                    break;
                }
            }
            profiler->processCumulativeResults();
            profiler->printCumulativeData("Background-");
            elapsedTime = profiler->getElapsedTimeSeconds();
            profiler->resetProfiler();
        }
        return elapsedTime;
    }

    void resetPID() 
    {
        ::pid = -1;
    }

    int programProfiling(Profiler* profiler, char** parmList, char** envp, int externalProgIndex, int runs, int timeout, bool timeoutEnabled) 
    {
        int currentRun = 0;
        int elapsedTime = 0;
        std::cout<< "Runs:" << runs << ' ' << "timeout:" << timeout << std::endl;
        if (profiler && parmList) 
        {
            profiler->setProgramTag();
            while(currentRun != runs) 
            {
                if (externalProgIndex > 0) 
                {
                    char stack[STACK_SPACE];
                    struct ::cloneArgs childArgs;
                    childArgs.args = parmList;
                    childArgs.envp = envp;

                    std::cout << "Profiling the Program" << "\n";

                    // On Linux systems, the stack space grows downwards, on other systems that may be the case. Safest bet is start the child process halfway down the stack.
                    int pid = clone(childProg, stack + (STACK_SPACE / 2), CLONE_NEWPID | CLONE_NEWNS | CLONE_VFORK |SIGCHLD, &childArgs) ;
                    if (pid > 0) 
                    {
                        ::pid = pid;
                        profiler->checkAdapter((int) pid);

                        profiler->printStartTime();
                        profiler->startProfiling(timeout, timeoutEnabled, &::pid);

                        if (timeoutEnabled) 
                        {
                            killProcess(profiler);
                        }
                        int returnStatus;

                        auto done = wait(&returnStatus);
                        if (done > 0) 
                        {
                            std::cout << "Program run:" << currentRun << " terminated successfully" << "\n";
                        } else if (done == -1) 
                        {
                            if (errno != ECHILD) 
                            {
                                std::cout << "error handling child process stopping execution" << std::endl;
                                break;
                            }
                        }
                    } else {
                        std::cerr << "Clone error stopping execution" << std::endl;
                        break;
                    }
                }
                currentRun++;
            }
            profiler->processCumulativeResults();
            profiler->printCumulativeData("Program-");
            elapsedTime = profiler->getElapsedTimeSeconds();
            profiler->resetProfiler();
        }
        return elapsedTime;
    }

    void killProcess(Profiler* profiler) 
    {
        if (::pid > 0) 
        {
            kill(::pid, SIGTERM); 
            std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_TIME));
            int status;
            pid_t pid;
            if ((pid = waitpid(-1, &status, WNOHANG)) <= 0) 
            {
                if (pid == 0) 
                {
                    std::cout << "process didn't terminate in 20 seconds" << std::endl;
                    std::cout << "Killing process now" << std::endl;
                    kill(::pid, SIGKILL);
                    std::cout << "wating for zombie children to be killed" << std::endl;
                    while(waitpid(-1, &status, WNOHANG) > 0) 
                    {
                        
                    }
                } else if (pid == -1) 
                {
                    std::cerr << "Error killing child process, clearing up and exiting" << std::to_string(pid) << std::endl;
                    if (profiler) 
                    {
                        profiler->resetProfiler();
                    }
                    exit(EXIT_FAILURE);
                } 
            }
            std::cout << "Process terminated cleanly" << std::endl;
            ::pid = -1;
        }
    }
}

static int childProg(void* args)
{
    struct ::cloneArgs *childArgs = (struct ::cloneArgs*) args;
    int returnCode = EXIT_FAILURE;

    if (mount("none", "/proc", NULL, MS_SLAVE|MS_REC, NULL) != 0) 
    {
        std::cerr << "remounting proc private failed" << std::endl;
        exit (returnCode);
    }

    if (mount ("proc", "/proc", "proc", 0, NULL) != 0) 
    {
        std::cerr << "mounting proc failed" << std::endl;
        exit (returnCode);
    }
    returnCode = netns_switch();
    if (returnCode < 0) { 
        exit(returnCode);
    }

    if (childArgs && execve(childArgs->args[0], childArgs->args, childArgs->envp) < 0) 
    {
        std::cerr << "Executing program failed" << std::endl;
    }
    
    exit(0);
}

static int netns_switch()
{
    int retVal = EXIT_SUCCESS;
    int netns = open(NAMESPACE_LOCATION, O_RDONLY | O_CLOEXEC);
    if (netns < 0) 
    {
        std::cerr << "Cannot open network namespace " << strerror(errno) << std::endl;
        retVal = EXIT_FAILURE;
    } else {
        if (setns(netns, CLONE_NEWNET) < 0) 
        {
            std::cerr << "Cannot open network namespace " << strerror(errno) << std::endl;
            retVal = EXIT_FAILURE;
        }
        close(netns);
    }
    return retVal;
}
