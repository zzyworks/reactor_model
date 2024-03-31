#include "thread_pool.h"

struct AA {
  void show() { std::cout << "call show()\n"; }

  ~AA() { std::cout << "call ~AA()\n"; }
};

void fun(AA* aa) {
  sleep(5);
  aa->show();
}

int main() {
  ThreadPool tp()
  return 0;
}
