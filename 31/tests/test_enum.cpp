#include <iostream>

enum class ThreadType {
  THREAD_IO,
  THREAD_WORK,
  THREAD_TEST
};

void func(const ThreadType& type) {
  if(type == ThreadType::THREAD_IO)
    std::cout << "THREAD_IO" << "\n";
  else if(type == ThreadType::THREAD_WORK)
    std::cout << "THREAD_WORK" << "\n";
  else
    std::cout << "THREAD_TEST" << "\n";
}

int main() {
  func(ThreadType::THREAD_IO);
}
