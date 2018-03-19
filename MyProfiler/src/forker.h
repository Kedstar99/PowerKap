#ifndef FORKER_H
#define FORKER_H

#include "profiler.h"
#include <ostream>
#include <chrono>
#include <vector>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <limits.h>
#include <sys/mount.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/sched.h>

#define NAMESPACE_LOCATION "/var/run/netns/test_ns"
#define SLEEP_PROG         "/bin/sleep"
#define STACK_SPACE        4096
#define SLEEP_TIME         20000

namespace forker {
    
    // Takes a pointer to the profiler object, arguments to setup a program, and calibration such as runs, timeout and timeoutEnabled. 
    // Returns the approximate time the profiler was run.
    int backgroundProfiling(Profiler* pptr, char** envp, int runs, int timeout);
    int programProfiling(Profiler* pptr, char** args, char** envp, int numberProgArgs, int runs, int timeout, bool timeoutEnabled);

    // Sets the internal process id to -1. Stops the profiler from reading the wrong pid.
    void resetPID();

    // Force kills a child process. Sends sigterm and waits 20 seconds before sending a sigkill.
    void killProcess(Profiler* profiler);
}  

namespace {
    pid_t pid;
}


struct cloneArgs{
    char** args;
    char** envp;
};

// Setup the proc directories and executes the target program
static int childProg(void *args);

// Sets the network namespace to the correct file.
static int netns_switch();


#endif 
