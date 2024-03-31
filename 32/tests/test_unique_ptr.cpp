// 

#include <iostream>
#include <memory>

int main() {
  std::unique_ptr<int> uptr(new int(3));
  std::cout << sizeof(uptr) << "\n";
  return 0;
}
