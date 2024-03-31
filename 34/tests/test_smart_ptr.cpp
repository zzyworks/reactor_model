#include <memory>

#include "thread_pool.h"

struct AA {
  void show() { std::cout << "call show()\n"; }

  ~AA() { std::cout << "call ~AA()\n"; }
};

void fun(std::shared_ptr<AA> aa) {
  sleep(5);
  aa->show();
}

int main() {
  ThreadPool tp(2, ThreadType::THREAD_TEST);
  {
    std::shared_ptr<AA> a(new AA);
    tp.push(std::bind(fun, a));
  }
  sleep(10);
  return 0;
}
