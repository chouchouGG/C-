## RVO和NRVO的区别
RVO在函数**返回临时对象**时生效，NRVO适用于函数**返回命名对象**时生效。
```C++ {.line-numbers}
#include <iostream>
using namespace std;

class T
{
public:
  T() { cout << "create T" << endl; }
  ~T() { cout << "delete T" << endl; }
  T &operator=(const T &t) {
    cout << "copy operator=" << endl;
    return *this;
  }
  T(const T &t) { cout << "create T by copy constructor" << endl; }
};

T f1() { // RVO
  return T();
}

T f2() { // NRVO
  T t;
  return t;
}

int main() {
  {
    auto t1 = f1();
  }
  cout << "=========" << endl;
  {
    auto t2 = f2();
  }
}
```

## RVO 优化原理
它遵守这么一个原则，对于临时对象（也就是右值对象）编译器不会在内存中创建它， 什么时候创建它呢？ 

对于一个函数返回一个函数内的局部的临时对象时，编译器优化之后相当于在函数的调用点处才会创建这个对象。


## NRVO 优化原理
它是这样的：在函数的调用点处，先创建这个对象，然后通过对象指针传递到函数内，对该对象进行相关操作。 类似于开头讲的那种人工写代码的方法。

NRVO 优化前可能是这样的：
```C++ {.line-numbers}
std::vector<int> GetVector() {
  std::vector<int> result(100000, 1); 
  return result;
}

void foo() {
  auto vect = GetVector();
}
```
NRVO 优化后的伪代码可能是这样的：
```C++ {.line-numbers}
void GetVector(std::vector<int> *x) {
  new (x) std::vector<int>(100000, 1); // placement new的使用调用构造函数
}

void foo() {
  auto *x = static_cast<std::vector<int> *>( 
                  alloca(sizeof(std::vector<int>)) );
  GetVector(x);
  delete x;
}
```


# 参考资料
- [RVO and NRVO](https://pvs-studio.com/en/blog/terms/6516/)
- [C++的那些事——返回值优化](https://zhuanlan.zhihu.com/p/588852649)
