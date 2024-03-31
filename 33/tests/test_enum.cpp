#include <iostream>

enum class ThreadType : uint8_t {
  THREAD_IO,
  THREAD_WORK,
  THREAD_TEST,
  THREAD_OTHER
};

void func(const ThreadType& type) {
  if(type == ThreadType::THREAD_IO)
    std::cout << "THREAD_IO" << "\n";
  else if(type == ThreadType::THREAD_WORK)
    std::cout << "THREAD_WORK" << "\n";
  else
    std::cout << "THREAD_TEST" << "\n";
}

void test1() {
  std::cout << sizeof(ThreadType) << "\n";
  std::cout << sizeof(bool) << "\n";
}

int main() {
  test1();
  return 0;
}
