#include <iostream>
#include <string>
#include <vector>

using namespace std;

struct T
{
    int i;

    // 前置++
    T &operator++()
    {
        ++(this->i);
        return *this;
    }

    // 后置++
    T operator++(int)
    {
        T t = *this; // 拷贝一个副本
        ++(*this);
        return t; // 返回副本
    }
};

class MyClass
{
public:
    MyClass(const char *str = "chouchou") // 默认构造
        : _str(str)
    {
        std::cout << "default constructor" << std::endl;
    }

    MyClass(const MyClass &rhs) // 拷贝构造
        : _str(rhs._str)
    {
        std::cout << "copy constructor" << std::endl;
    }

    MyClass(MyClass &&rhs) noexcept // 移动构造
        : _str(std::move(rhs._str))
    {
        std::cout << "move constructor" << std::endl;
    }

private:
    std::string _str;
};

MyClass get_class(MyClass &classes)
{
    return classes;
}

MyClass make_class()
{
    MyClass cc;
    return get_class(cc);
}

int main()
{
#if 0
    int a = 10;
    // int *p1 = &(a++);
    // int *p2 = &(++a);

    // T val;
    // val.i = 10;
    // val++;
    // ++val;
    // std::cout << val.i << std::endl;
    
    // int & ra = a;
    // std::cout << &a << std::endl;
  
    // int && rra = std::move(a);
    // std::cout << &a << std::endl;

    // std::cout << &("Helloworld") << std::endl;
#endif

    // 容器vector支持动态扩容，在扩容后不仅要将新元素添加进来，还需要将老元素挪到新的内存空间中。
    // 但是如果老元素的挪动采用拷贝的话效率太低，为了在老元素的挪动过程使用移动语义提高效率，就需要在移动构造函数添加上noexcept说明符，
    // noexcept说明符作用是告诉编译器此操作**不会产生异常**（即“被拷贝对象的原始数据是不会丢失的”），即人为实现了“强异常保证（strong exception guarantee）”。
    // 编译器只有确保强异常保证，才会使用移动来挪动老元素。

    // "强异常保证（strong exception guarantee）"。所谓强异常保证，即当我们调用一个函数时，如果发生了异常，那么应用程序的状态能够回滚到函数调用之前
    vector<MyClass> classes;

    cout << "***" << endl;
    MyClass temp;
    cout << "***" << endl;
    classes.push_back(temp);
    cout << "***" << endl;
    classes.push_back(move(temp));
    // MyClass cc = make_class();
}