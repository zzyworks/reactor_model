// 测试weak_ptr

#include <iostream>
#include <memory>

void func(std::shared_ptr<int> sp2) {
  std::cout << "func begin\n";
  std::cout << "sp2=" << sp2.use_count() << "\n";
  std::weak_ptr<int> wp1(sp2);
  std::cout << "wp1=" << wp1.use_count() << "\n";
  std::cout << "func end\n";
}

void test0() {
  std::shared_ptr<int> sp1 = std::make_shared<int>(10);
  std::cout << "sp1=" << sp1.use_count() << "\n";
  func(sp1);
  std::cout << "sp1=" << sp1.use_count() << "\n";
  std::weak_ptr<int> wp1(sp1);
  std::cout << "wp1=" << wp1.use_count() << "\n";
  std::cout << "sp1=" << sp1.use_count() << "\n";
}

int main() {
  test0();
  return 0;
}
