#include "processor.h"
#include "linux_parser.h"
#include <string>

// DONE: Return the aggregate CPU utilization
float Processor::Utilization() {
  cpu_utilization_ = std::stof(LinuxParser::CpuUtilization());
  return cpu_utilization_;
}