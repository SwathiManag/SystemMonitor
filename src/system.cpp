#include <cstddef>
#include <iostream>
#include <set>
#include <string>
#include <unistd.h>
#include <vector>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"
#include "system.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

// TODO: Return the system's CPU
Processor &System::Cpu() { return cpu_; }

// TODO: Return a container composed of the system's processes
vector<Process> &System::Processes() {
  vector<int> pids = LinuxParser::Pids();
  set<int> pid_set;
  for (int p : pids) {
    pid_set.insert(p);
  }
  for (int s : pid_set) {
    Process process(s);
    processes_.push_back(process);
  }

  return processes_;
}

// TODO: Return the system's kernel identifier (string)
std::string System::Kernel() {
  string kernel = LinuxParser::Kernel();
  return kernel;
}

// TODO: Return the system's memory utilization
float System::MemoryUtilization() {
  float system_mem_util = LinuxParser::MemoryUtilization();
  return system_mem_util;
}

// TODO: Return the operating system name
std::string System::OperatingSystem() {
  string system_os = LinuxParser::OperatingSystem();
  return system_os;
}

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() {
  int running_processes = LinuxParser::RunningProcesses();
  return running_processes;
}

// TODO: Return the total number of processes on the system
int System::TotalProcesses() {
  int total_processes = LinuxParser::TotalProcesses();
  return total_processes;
}

// TODO: Return the number of seconds since the system started running
long int System::UpTime() {
  long int up_time = LinuxParser::UpTime();
  return up_time;
}