#include <iostream>
#include <memory>
#include <vector>

template <class T>
class Animal
{
public:
  void SayHello()
  {
    static_cast<T *>(this)->SayHello();
  }
  std::weak_ptr<std::vector<int>> vec;
};

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

int main()
{
  Dog dog;
  Cat cat;

  dog.SayHello();
  cat.SayHello();
  return 0;
}
