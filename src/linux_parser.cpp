#include <dirent.h>
#include <iostream>
#include <regex>
#include <string>
#include <unistd.h>
#include <vector>

#include "format.h"
#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR *directory = opendir(kProcDirectory.c_str());
  struct dirent *file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// Done: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  string line;
  string key;
  string value;
  float MemTotal, MemFree;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "MemTotal:") {
          MemTotal = std::stof(value);
        } else if (key == "MemFree:") {
          MemFree = std::stof(value);
        } else
          continue;
      }
    }
  }
  return (MemTotal - MemFree) / MemTotal;
}

// DONE: Read and return the system uptime
long LinuxParser::UpTime() {
  string line, key;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> key;
  }
  return std::stol(key);
}

// DONE: Read and return CPU utilization
string LinuxParser::CpuUtilization() {
  string line;
  float CPU_Percentage;
  string cpu, user, nice, system, idle, iowait, irq, softirq, steal, guest,
      guest_nice;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    static long prevTotal = 0, prevIdle = 0;
    long Total, Idle;
    float total_diff, idle_diff;
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> cpu >> user >> nice >> system >> idle >> iowait >> irq >>
        softirq >> steal >> guest >> guest_nice;
    Total = std::stol(user) + std::stol(nice) + std::stol(system) +
            std::stol(idle) + std::stol(iowait) + std::stol(irq) +
            std::stol(softirq) + std::stol(steal) + std::stol(guest) +
            std::stol(guest_nice);
    Idle = std::stol(idle) + std::stol(iowait);
    total_diff = (float)(Total - prevTotal);
    idle_diff = (float)(Idle - prevIdle);
    CPU_Percentage = ((total_diff - idle_diff) / total_diff);
    prevTotal = Total;
    prevIdle = Idle;
  }
  return std::to_string(CPU_Percentage);
}
// DONE: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string line;
  string key;
  string value;
  int total_processes;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "processes") {
        total_processes = std::stoi(value);
      }
    }
  }
  return total_processes;
}

// DONE: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string line;
  string key;
  string value;
  int running_processes;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "procs_running") {
        running_processes = std::stoi(value);
      }
    }
  }
  return running_processes;
}

// DONE: Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  string line, key, str_pid;
  str_pid = to_string(pid);
  std::ifstream filestream(kProcDirectory + str_pid + kCmdlineFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> key;
  }
  return key;
}

// DONE: Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
  string line, key, value, MB;
  double KB;
  string str_pid = to_string(pid);
  std::ifstream filestream(kProcDirectory + str_pid + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "VmSize:") {
        KB = std::stod(value);
      }
    }
    MB = std::to_string((int)(KB * 0.0009765625));
  }
  return MB;
}

// DONE: Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
  string line, key, value, UID;
  string str_pid = to_string(pid);
  std::ifstream filestream(kProcDirectory + str_pid + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "Uid:") {
        UID = value;
      }
    }
  }
  return UID;
}

// DONE: Read and return the user associated with a process
string LinuxParser::User(int pid) {
  string line, user, x, uid_, result;
  string UID = LinuxParser::Uid(pid);
  std::ifstream file(kPasswordPath);
  if (file.is_open()) {
    while (std::getline(file, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream stream(line);
      while (stream >> user >> x >> uid_) {
        if (uid_ == UID)
          result = user;
      }
    }
  }
  return result;
}

// DONE: Read and return the uptime of a process
long LinuxParser::UpTime(int pid) {
  string line, key;
  int count = 1;
  long up_time;
  string str_pid = to_string(pid);
  std::ifstream filestream(kProcDirectory + str_pid + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key) {
        if (count == 22) {
          up_time = std::stol(key);
        }
        count++;
      }
    }
  }
  up_time /= sysconf(_SC_CLK_TCK);
  return up_time;
}

// Read and return cpu utilization of the process
float LinuxParser::CpuUtilization(int pid) {
  string line, key;
  long utime, stime, cutime, cstime, starttime, uptime, Hertz, seconds,
      total_time;
  int count = 1;
  float cpu_usage;
  string str_pid = to_string(pid);
  std::ifstream filestream(kProcDirectory + str_pid + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key) {
        switch (count) {
        case 14:
          utime = std::stol(key);
          break;
        case 15:
          stime = std::stol(key);
          break;
        case 16:
          cutime = std::stol(key);
          break;
        case 17:
          cstime = std::stol(key);
          break;
        case 22:
          starttime = std::stol(key);
          break;
        }
        count++;
      }
    }
  }
  uptime = LinuxParser::UpTime();
  Hertz = sysconf(_SC_CLK_TCK);
  total_time = utime + stime + cutime + cstime;
  seconds = uptime - (long)((float)starttime / (float)Hertz);
  cpu_usage = (((float)total_time / (float)Hertz) / (float)seconds);
  return cpu_usage;
}
