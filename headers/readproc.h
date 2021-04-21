#ifndef HEADER_Process
#define HEADER_Process
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/module.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <vector>
#include <string.h>
#include <pwd.h>
#include <dirent.h>
#include <cstdio>
#include <iostream>
#include <string>
#include <algorithm>
#include <map>
#include <ctime>
#include <iterator>

class IOReader {
public:
    IOReader(pid_t pid);
    ~IOReader();
    pid_t pid;
    unsigned long long int read_chars;
    unsigned long long int write_chars;
    unsigned int read_syscalls;
    unsigned int write_syscalls;
    unsigned long long int read_bytes;
    unsigned long long int write_bytes;
    unsigned long long int callcelled_write_bytes;
    void update();
private:
    FILE* io_stream;
    void reset_stream();
};
class Process {
public:
    pid_t pid;
    char comm[100];
    char state;
    pid_t ppid;
    int pgrp;
    int session;
    int tty_nr;
    int tpgid;
    unsigned int flags;
    unsigned long int minflt;
    unsigned long int cminflt;
    unsigned long int majflt;
    unsigned long int cmajflt;
    unsigned long int utime;
    unsigned long int stime;
    long int cutime;
    long int cstime;
    long int priority;
    long int nice;
    long int num_threads;
    long int itrealvalue;
    unsigned long long starttime;
    unsigned int long vsize;
    long int rss;
    unsigned long int rsslim;
    unsigned long int startcode;
    unsigned long int endcode;
    unsigned long int startstack;
    unsigned long int kstkesp;
    unsigned long int kstkeip;
    unsigned long int signal;
    unsigned long int blocked;
    unsigned long int sigignore;
    unsigned long int sigcatch;
    unsigned long int wchan;
    unsigned long int nswap;
    unsigned long int cnswap;
    int exit_signal;
    int processor;
    unsigned int rt_priority;
    unsigned int policy;
    unsigned long long int delayacct_blkio_ticks;
    unsigned long int guest_time;
    unsigned long int cguest_time;
    unsigned long int start_data;
    unsigned long int end_data;
    unsigned long int start_brk;
    unsigned long int arg_start;
    unsigned long int arg_end;
    unsigned long int env_start;
    unsigned long int env_end;
    unsigned long int exit_code;
    float mem_percent;
    std::string user;
    float cpu_percent;
    int vst;
    std::string tty;
    std::string stat;
    time_t start_time;
    std::string cmdline;
    std::string dir;
    bool readStat();
    bool readLibrary();
    bool readNetwork();
    bool readFileInfo();
    bool readIO();
    Process(pid_t pid);
    ~Process();
    std::vector<Process*> child_processes;
    std::vector<Process*> threads;
    IOReader* ioReader;
    std::map<int,std::string> openFiles;
private:
    char* stat_string;
    int stat_fd;
    struct stat stat_st;
    bool test();
    bool write_fields();
    void getUserFromUID(uid_t uid);
    /*write process status from /proc/.../stat */
    
};



class Proc {
public:
    Proc();
    ~Proc();
    struct dirent* getNextFile();
private:
    DIR* dp;
};

class NetworkReader {

};


static std::map<pid_t,Process*> process_list;
int readProc();

#endif