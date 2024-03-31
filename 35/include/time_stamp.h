/***********************************************************
  > File Name: time_stamp.h
  > Author: Aklzzz
  > Mail: 2429605732@qq.com
  > Created Time: Sat 23 Mar 2024 01:24:28 PM CST
  > Modified Time:Sat 23 Mar 2024 01:24:28 PM CST
 *******************************************************/

#ifndef TIME_STAMP_H
#define TIME_STAMP_H

#include <ctime>
#include <string>

#include "copyable.h"

class TimeStamp : Copyable {
public:
  TimeStamp();

  TimeStamp(int64_t sec_since_epoch);

  static TimeStamp now();

  std::time_t timeToInteger() const;

  std::string timeToString() const;

private:
  std::time_t sec_since_epoch_;
};

#endif // TIME_STAMP_H
