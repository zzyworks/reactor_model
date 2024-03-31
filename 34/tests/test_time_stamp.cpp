#include <iostream>

#include "time_stamp.h"

int
main() {
  TimeStamp ts;
  std::cout << ts.timeToInteger() << "\n";
  std::cout << ts.timeToString() << "\n";

  std::cout << TimeStamp().timeToInteger() << "\n";
  std::cout << TimeStamp().timeToString() << "\n";
  return 0;
}
