# CRTP
`CRTP`英文全称`Curiously Recurring Template Pattern`，对应中文翻译为“**奇异递归模版模式**”。

`CRTP`在语法上，是派生类继承自用该派生类作为模板实参实例化的基类。
```C++ {.line-numbers}
template<class _Ty>
class Base{
  // ···
};

class Derived : public Base<Derived>{
  // ···
}；
```
行`6`所示，`Derived`公有继承自类模板`Base`，并将其本身作为模板实参实例化`Base`。

## CRTP名字来源
`CRTP`中的递归，体现在派生类继承基类，而基类又需要派生类的类型参数作为模版参数，并且在基类又调用派生类方法，这种类似于“套娃”的方式，就是`CRTP`中的递归。

`CRTP`中的奇异，体现在编译器编译`Dog`类时，需要先实例化基类模板`Animal<T>`（**以此来知道基类模板的完整定义，从而确定派生类的基类部分占用的内存空间**），而基类模板的实例化又是依赖于派生类`Dog`的。基类是依赖于派生类的类型信息的，而派生类又依赖于基类的定义，这种类似于互相依赖的定义过程就是奇异的地方.

## CRTP实现编译期多态
- 基类
```C++ {.line-numbers}
template <class T>
class Animal
{
public:
  void SayHello()
  {
    static_cast<T *>(this)->SayHello();
  }
};
```

- 派生类
```C++ {.line-numbers}
class Dog : public Animal<Dog>
{
public:
  void SayHello()
  {
    std::cout << "wang! wang!" << std::endl;
  }
};

class Cat : public Animal<Cat>
{
public:
  void SayHello()
  {
    std::cout << "miao! miao!" << std::endl;
  }
};
```


## CRTP的其他应用
STL中的`enable_shared_from_this`就是一个典型的以`CRTP`来**实现对派生类的扩展**的应用实例。

继承自`enable_shared_from_this<T>`的类，可以通过调用基类提供的`shared_from_this`成员函数，来获得指向自身的`shared_ptr`智能指针。

像`enable_shared_from_this`这种，通过继承一个类（基类）扩展另外一个类（派生类）的功能，但是该功能需要用到被扩展的类的类型信息的时候，可以采用`CRTP`技术进行实现。

[使用`enable_shared_from_this`解决返回`shared_ptr`对象的控制块不一致问题](./智能指针.md)
***
# 参考资料
[CRTP从原理到应用](https://zhuanlan.zhihu.com/p/641758930)