#include "format.h"
#include <iomanip>
#include <sstream>
#include <string>

using std::string;

// DONE: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) {
  int HH = seconds / 3600;
  int SS = seconds % 3600;
  int MM = SS / 60;
  SS = SS % 60;
  std::ostringstream time;
  time << std::setfill('0') << std::setw(2) << HH << ":";
  time << std::setfill('0') << std::setw(2) << MM << ":";
  time << std::setfill('0') << std::setw(2) << SS;
  return time.str();
}