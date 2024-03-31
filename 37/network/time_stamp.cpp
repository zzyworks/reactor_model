#include "time_stamp.h"

TimeStamp::TimeStamp() : sec_since_epoch_(time(0)) {}

TimeStamp::TimeStamp(int64_t sec_since_epoch)
    : sec_since_epoch_(sec_since_epoch) {}

TimeStamp 
TimeStamp::now() {
  return TimeStamp();
}

std::time_t
TimeStamp::timeToInteger() const {
  return sec_since_epoch_;
}

std::string
TimeStamp::timeToString() const {
  char buf[32] = {0};
  tm* tm_time = localtime(&sec_since_epoch_);
  snprintf(buf, 20, "%4d-%02d-%02d %02d:%02d:%02d", tm_time->tm_year + 1900,
           tm_time->tm_mon + 1, tm_time->tm_mday, tm_time->tm_hour,
           tm_time->tm_min, tm_time->tm_sec);
  return buf;
}
