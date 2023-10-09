// 两种使用 enable_if 的例子：
#include <iostream>
#include <type_traits>

// 1. 只有当 T 是一个整型类型，is_odd 才被实例化为一个返回值类型为 bool 的模板函数
template <class T>
typename std::enable_if<std::is_integral<T>::value, bool>::type
is_odd(T i)
{
  return bool(i % 2);
}

// 2. 只有当 T 是一个整型类型，is_even 才会被实例化
template <class T, class T2 = typename std::enable_if<std::is_integral<T>::value, bool>::type>
int is_even(T i)
{
  return !int(i % 2);
}

int main()
{
  short i = 2; // 如果 i 是非整型类型，则代码不能被编译（原因是编译器不能实例化出相应的is_odd和is_even）

  std::cout << std::boolalpha;
  std::cout << i << " is odd: " << is_odd(i) << std::endl;
  std::cout << i << " is even: " << is_even(i) << std::endl;

  return 0;
}
