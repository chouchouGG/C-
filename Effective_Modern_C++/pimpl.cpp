// 使用Pimpl

// 在头文件person.hpp中
#include <memory>
class Person
{
public:
  Person();

private:
  // Person类的实现细节放置在该前向声明的实现类中。
  struct Impl;
  // 指向实现类Impl的私有指针
  std::unique_ptr<Impl> pimpl_;
};

// 在源文件person.cpp中
#include "person.hpp"
#include "basic_info.hpp"
#include <string>
#include <memory>
struct Person::Impl
{
  std::string name;
  std::string id;
  BasicInfo basic_info;
};
Person::Person() : pimpl_(std::make_unique<Impl>()) {}