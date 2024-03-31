#include <memory>
#include <iostream>

// struct Foo {
//   std::shared_ptr<Foo> getSelfPtr() {
//     return std::shared_ptr<Foo>(this);
//   }
// };

struct Foo : public std::enable_shared_from_this<Foo> {
  std::shared_ptr<Foo> getSelfPtr() {
    return shared_from_this();
  }
};

int main() {
  // auto p(new int(12));
  // std::shared_ptr<int> sp1(p);
  // std::cout << *sp1 << "\n";
  // delete p;
  // 不能使用原始指针初始化智能指针
  // std::shared_ptr<int> sp1(std::make_shared<int>(15));
  // std::cout << sp1.use_count() << "\n";
  // std::shared_ptr<int> sp2(sp1);
  // std::cout << sp1.use_count() << "\n";

  // 两个不同的控制块，但是指向同一个指针对象
  // auto sp1 = std::make_shared<Foo>();
  // std::cout << sp1.use_count() << "\n";
  // auto sp2 = sp1->getSelfPtr();
  // std::cout << sp1.use_count() << "\n";
  // std::cout << sp2.use_count() << "\n";
 
  // 解决以上方法
  auto sp1 = std::make_shared<Foo>();
  std::cout << sp1.use_count() << "\n";
  auto sp2 = sp1->getSelfPtr();
  std::cout << sp1.use_count() << "\n";
  std::cout << sp2.use_count() << "\n";
}
