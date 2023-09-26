#include <iostream>
using namespace std;

/* 如果模板中形参的类型别名没有引用修饰符，那么就是按值传递， */
template <class T>
void f(T param)
{
}

int main(int argc, char const *argv[])
{
  int a = 10;
  int &ra = a;
  const int ca = a;
  const int &cra = a;

  f(a);   // 实例化：void f<int>(int param)
  f(ra);  // 实例化：void f<int>(int param)
  f(ca);  // 实例化：void f<int>(int param)
  f(cra); // 实例化：void f<int>(int param)

  return 0;
}
