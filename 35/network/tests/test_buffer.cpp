#include <iostream>

#include "buffer.h"

int main() {
  std::string s1 = "aaaaaaaaaaaaaaaaaa";
  Buffer buf(SeparatorType::FOUR_BYTE);
  buf.appendWithSep(s1.data(), s1.size());
  std::string s2;
  buf.pickMessage(s2);
  std::cout << s2 << "\n";
}
