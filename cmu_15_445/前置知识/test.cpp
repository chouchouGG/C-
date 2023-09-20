#include <iostream>

using namespace std;

template <typename T>
void print(T &t)
{
  cout << "l-value" << endl;
}

template <typename T>
void print(T &&t)
{
  cout << "r-value" << endl;
}

template <typename T>
void testForward(T &&v)
{
  print(v);
  print(std::forward<T>(v));
  print(std::move(v));
}

int main(int argc, char *argv[])
{
  testForward(1);

  cout << "======================" << endl;

  int x = 1;
  testForward(x);
}