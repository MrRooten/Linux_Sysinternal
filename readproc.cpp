#include "headers/readproc.h"


bool Process::write_fields() {
        int num = sscanf(this->stat_string,
        "%d %s %c %d %d %d %d %d %u %lu %lu %lu %lu %lu %lu %ld %ld %ld %ld %ld %ld %llu %lu %ld %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %d %d %u %u %llu %lu %ld %lu %lu %lu %lu %lu %lu %lu %d",
        &this->pid,
        &this->comm,
        &this->state,
        &this->ppid,
        &this->pgrp,
        &this->session,
        &this->tty_nr,
        &this->tpgid,
        &this->flags,
        &this->minflt,
        &this->cminflt,
        &this->majflt,
        &this-> cmajflt,
        &this-> utime,
        &this-> stime,
        &this-> cutime,
        &this-> cstime,
        &this-> priority,
        &this->nice,
        &this->num_threads,
        &this-> itrealvalue,
        &this-> starttime,
        &this-> vsize,
        &this-> rss,
        &this-> rsslim,
        &this-> startcode,
        &this-> endcode,
        &this-> startstack,
        &this-> kstkesp,
        &this-> kstkeip,
        &this-> signal,
        &this-> blocked,
        &this-> sigignore,
        &this-> sigcatch,
        &this-> wchan,
        &this-> nswap,
        &this-> cnswap,
        &this-> exit_signal,
        &this-> processor,
        &this-> rt_priority,
        &this-> policy,
        &this-> delayacct_blkio_ticks,
        &this-> guest_time,
        &this-> cguest_time,
        &this-> start_data,
        &this-> end_data,
        &this-> start_brk,
        &this-> arg_start,
        &this-> arg_end,
        &this-> env_start,
        &this-> env_end,
        &this-> exit_code);
        if(num == 52) {
            return true;
        } else {
          return false;
      }
}

bool Process::readStat() {
    read(this->stat_fd,this->stat_string,4096);
    lseek(this->stat_fd,0,SEEK_SET);
    this->write_fields();
    return true;
}

Process::Process(pid_t pid) {
    this->pid = pid;
    this->dir = "/proc/" + std::to_string(this->pid)+"/";
    this->stat_fd = open((this->dir+"stat").c_str(),O_CREAT|S_IRUSR);
    fstat(this->stat_fd,&this->stat_st);
    this->stat_string = (char*)malloc(4096);

    this->ioReader = new IOReader(this->pid);
}

Process::~Process() {
    close(this->stat_fd);
    free(this->stat_string);
}

void Process::getUserFromUID(uid_t uid) {
    struct passwd* pw = getpwuid(uid);
    this->user = pw->pw_name;
}

IOReader::IOReader(pid_t pid) {
    this->pid = pid;
    char path[50];
    snprintf(path,sizeof path,"/proc/%d/io",pid);
    this->io_stream = fopen(path,"r");

}

IOReader::~IOReader() {
    fclose(this->io_stream);
}

void IOReader::reset_stream() {
    fseek(this->io_stream,0,SEEK_SET);
}

void IOReader::update() {
    this->reset_stream();
    char line[4096];
    char _[100];
    unsigned long long int tmp_store[7];
    
    for(int i=0;i < 7;i++) {
        memset(line,0,sizeof line);
        fgets(line,sizeof line,this->io_stream);
        sscanf(line,"%s %d",_,tmp_store+i);
    }

    this->read_chars = tmp_store[0];
    this->write_chars = tmp_store[1];
    this->read_syscalls = tmp_store[2];
    this->write_syscalls = tmp_store[3];
    this->read_bytes = tmp_store[4];
    this->write_bytes = tmp_store[5];
    this->callcelled_write_bytes = tmp_store[6];
}

Proc::Proc() {
    this->dp = opendir("/proc");
}

Proc::~Proc() {
    closedir(dp);
}

struct dirent* Proc::getNextFile() {
    return readdir(this->dp);
}

bool is_number(const std::string& s) {
    return !s.empty() && std::find_if(s.begin(),
        s.end(),[](unsigned char c) { return !std::isdigit(c);}) == s.end();
}

bool readstat(Process* process,pid_t pid) {
    return true;
}

int readproc() {
    Proc procReader;
    struct dirent* dirp;
    while((dirp = procReader.getNextFile())!=NULL) {
        std::string proc_name = dirp->d_name;
        if(is_number(proc_name)) {
            std::cout << proc_name << std::endl;
            int pid = std::stoi(proc_name,nullptr,10);
            Process* process = new Process(pid);
            if(process->readStat()==true){
                process_list.insert(std::pair<pid_t,Process*>(pid,process));
            } else {

            }

        }
    }

    
    return 0;
}

int main() {
    Process p(1839);
    p.readStat();
}