[C++ std::enable_if解析](https://blog.csdn.net/xiongping_/article/details/54880299)

`SFINAE` 的的全称是 **S**ubstitution **F**ailure **I**s **N**ot **A**n **E**rror（**替换失败并非错误**）。

`SFINAE` 是 `enable_if` 得以实现的基础。

## enable_if的实现
```C++
template<bool B, class T = void>
struct enable_if {};


template<class T>
struct enable_if<true, T> { 
  typedef T type; 
};
```

## enable_if使用原型
```C++
template <bool Cond, class _Ty = void> struct enable_if;
```
`Cond`：必须是**编译期**的布尔值常量。
`_Ty`：是一个类型，默认为`void`。

## 使用enable_if
以下代码，展示了两种使用 `enable_if` 的例子：
```C++ {.line-numbers}
#include <iostream>
#include <type_traits>

// 1. 只有当 T 是一个整型类型，is_odd 才被实例化为一个返回值类型为 bool 的模板函数
template <class T>
typename std::enable_if<std::is_integral<T>::value, bool>::type
is_odd(T i) {
  return bool(i % 2);
}

// 2. 只有当 T 是一个整型类型，is_even 才会被实例化
template <class T, 
            class = typename std::enable_if<std::is_integral<T>::value>::type>
bool is_even(T i) {
  return !bool(i % 2);
}

int main()
{
  // 如果 i 是非整型类型，则代码不能被编译（原因是编译器不能实例化出相应的is_odd和is_even）
  short i = 1; 

  std::cout << std::boolalpha; 
  std::cout << i << " is odd: " << is_odd(i) << std::endl;
  std::cout << i << " is even: " << is_even(i) << std::endl;

  return 0;
}
```
注意到代码第`13`行，这里的模板参数省略了名字。之所以有这种语法，是因为在`is_even`中，这句的重点在`std::enable_if`，即使写了名字也没有什么用处，所以可以省略。 

注意到代码第`23`行，`std::boolalpha`的作用是使`bool`型变量按照相应的字符串`"false"`、`"true"`的格式输出。如不使用该标识符，那么结果会按照`1`、`0`的格式输出。