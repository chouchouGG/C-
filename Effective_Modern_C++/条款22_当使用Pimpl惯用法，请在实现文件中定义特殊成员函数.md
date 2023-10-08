# 关于Pimple阅读以下两篇
[C++ pimpl 模式](https://blog.csdn.net/xingkongyidian/article/details/67637367)
[C++编程技巧: Pimpl](https://zhuanlan.zhihu.com/p/458947637)

# Pimpl是什么？
`Pimpl`(**P**ointer to **impl**ementation) 是一种减少代码依赖和编译时间的C++编程技巧，其基本思想是将一个外部可见类(visible class)的实现细节（一般是所有私有的非虚成员）放在一个单独的实现类(implementation class)中，并在可见类中通过一个私有的实现类的类型指针来间接访问该实现类。

# Pimpl有什么用？
在类中使用`Pimpl`惯用法，具有如下优点：
1. 降低耦合
2. 信息隐藏
3. 降低编译依赖，提高编译速度
4. 接口与实现分离

# 如果不使用Pimpl，代码可能是这样的
```C++ {.line-numbers}
// 在头文件person.h中
// 需要包含额外的头文件<string> 和 "basic_info.hpp"
#include "basic_info.hpp"
#include <string>
class Person {
 private:
  std::string name;
  std::string id;
  // 需要经常变动的个人信息类BasicInfo
  BasicInfo basic_info;
};
```
`basic_info.hpp`需要经常变动，如果发生改动`Person`类不仅需要重新编译，包含`person.h`的`*.cpp`文件也需要重新编译，由于耦合度太高编译依赖严重，可能导致多个文件都需要重新编译。

# 通过`unique_ptr`实现Pimpl的代码实现
- person.h
```C++ {.line-numbers}
#include <memory>
#include <string>

class Person 
{
public:
  Person(std::string name = "---", std::string id = "---");
  ~Person(); // 析构函数声明
  void print_info();
private:
  struct Impl;
  std::unique_ptr<Impl> pimpl_;
};

Person::~Person() = default; //析构不能定义在person.h中

/* 移动赋值因为会析构左操作数，而进行类型完整性检查 */
Person& Person::operator=(Person&& rhs) = default; //移动赋值不能定义在person.h中

/* 以下三个都是因为异常安全代码中，当出现异常需要调用析构销毁临时对象，而进行类型完整性检查 */
Person::Person(Person&& rhs) = default; // 移动构造不能定义在person.h中
Person::Person(const Person& rhs) // 拷贝构造
	: pimpl_{ std::make_unique<Impl>(*rhs.pimpl_) } {}
Person& Person::operator=(const Person& rhs) { // 拷贝赋值
	*pimpl_ = *rhs.pimpl_;
	return *this;
};
```


- person.cpp
```C++ {.line-numbers}
#include "person.h"
#include <string>
#include <memory>
#include <iostream>

struct Person::Impl {
  std::string _name;
  std::string _id;

  Impl(std::string name, std::string id) : _name(name), _id(id) {}
};

/*  Person::~Person() = default; // 正确做法 */

Person::Person(std::string name, std::string id) 
  : pimpl_(std::make_unique<Impl>(name, id)) {}

void Person::print_info() {
  std::cout << "name:" << pimpl_->_name << "  id:" << pimpl_->_id << std::endl;
}
```

```C++
int main() {
  Person p;
}
```
> **一个已经被声明，却还未被实现的类型，被称为不完整类型（incomplete type）**。

## 产生新的问题...
实际上，`person.h`代码中`15`至`26`行代码在编译过程中会报错。

> 以下解释节选自《Effective Modern C++》的条款22

在对象`p`离开了作用域时，问题出现了。由于`std::unique_ptr<Impl> pimpl_;`是由智能指针管理的对象，故在调用`Person`的析构函数时又会去调用`Person::Impl`的析构函数。然而编译到删除器相关代码时，会使默认删除器使用C++11的特性`static_assert`进行**类型完整性检查**，来确保原始指针指向的类型是一个完整类型。 当编译器为`Person p;`的析构生成代码时，它会遇到`static_assert`检查并且失败，这通常是错误信息的来源。

如果我们使用`std::shared_ptr`而不是`std::unique_ptr`来做`pimpl_`指针， 我们会发现不会产生任何新的问题...

`std::unique_ptr`和`std::shared_ptr`在`pimpl_`指针上的表现上的区别的深层原因在于，他们支持自定义删除器的方式不同。 对`std::unique_ptr`而言，**删除器的类型是这个智能指针的一部分**，这让编译器有可能生成更小的运行时数据结构和更快的运行代码。 这种更高效率的后果之一就是`std::unique_ptr`指向的类型，在编译器的生成特殊成员函数（如析构函数，移动操作）被调用时，必须已经是一个完整类型。 而对`std::shared_ptr`而言，删除器的类型不是该智能指针的一部分，这让它会生成更大的运行时数据结构和稍微慢点的代码，但是当编译器生成的特殊成员函数被使用的时候，指向的对象不必是一个完整类型。（知道`std::unique_ptr`和`std::shared_ptr`的实现，这一段才比较容易理解。）