#include <cctype>
#include <sstream>
#include <string>
#include <unistd.h>
#include <vector>

#include "linux_parser.h"
#include "process.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid) { pid_ = pid; }

// TODO: Return this process's ID
int Process::Pid() { return pid_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() {
  float cpu_usage = LinuxParser::CpuUtilization(pid_);
  return cpu_usage;
}

// TODO: Return the command that generated this process
string Process::Command() {
  string command = LinuxParser::Command(pid_);
  return command;
}

// TODO: Return this process's memory utilization
string Process::Ram() {
  string ram = LinuxParser::Ram(pid_);
  return ram;
}

// TODO: Return the user (name) that generated this process
string Process::User() {
  string user = LinuxParser::User(pid_);
  return user;
}

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() {
  long int up_time = LinuxParser::UpTime(pid_);
  return up_time;
}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const &a) const {
  if (LinuxParser::CpuUtilization(a.pid_) < LinuxParser::CpuUtilization(pid_)) {
    return true;
  }
  return false;
}