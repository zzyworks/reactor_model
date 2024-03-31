#include <string>

#include "thread_pool.h"

void
show(int no, const std::string& name) {
  printf("no = %d, name = %s.\n", no, name.c_str());
}

void
test() {
  printf("这是一段测试文字.\n");
}

int
main(void) {
  ThreadPool thread_pool(3);
  std::string name = "西施";
  thread_pool.push(std::bind(show, 8, name));
  sleep(2);
  thread_pool.push(std::bind(test));
  sleep(2);
  thread_pool.push(std::bind([](void) { printf("这是第二段测试文字.\n"); }));
  sleep(2);
  return 0;
}
