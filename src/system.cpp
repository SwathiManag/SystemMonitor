#include <cstddef>
#include <iostream>
#include <string>
#include <unistd.h>
#include <vector>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"
#include "system.h"

using std::size_t;
using std::string;
using std::vector;

// DONE: Return the system's CPU
Processor &System::Cpu() { return cpu_; }

// DONE: Return a container composed of the system's processes
vector<Process> &System::Processes() {
  processes_.clear();
  vector<int> pids = LinuxParser::Pids();
  for (int p : pids) {
    Process process(p);
    processes_.push_back(process);
  }

  return processes_;
}

// DONE: Return the system's kernel identifier (string)
std::string System::Kernel() {
  string kernel = LinuxParser::Kernel();
  return kernel;
}

// DONE: Return the system's memory utilization
float System::MemoryUtilization() {
  float system_mem_util = LinuxParser::MemoryUtilization();
  return system_mem_util;
}

// DONE: Return the operating system name
std::string System::OperatingSystem() {
  string system_os = LinuxParser::OperatingSystem();
  return system_os;
}

// DONE: Return the number of processes actively running on the system
int System::RunningProcesses() {
  int running_processes = LinuxParser::RunningProcesses();
  return running_processes;
}

// DONE: Return the total number of processes on the system
int System::TotalProcesses() {
  int total_processes = LinuxParser::TotalProcesses();
  return total_processes;
}

// DONE: Return the number of seconds since the system started running
long int System::UpTime() {
  long int up_time = LinuxParser::UpTime();
  return up_time;
}