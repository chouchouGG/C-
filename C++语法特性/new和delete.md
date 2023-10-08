# 关于`new`/`delete`，看以下三篇就够了
1. [C/C++——new和delete的实现原理（详解）](https://blog.csdn.net/qq_45657288/article/details/114699235)
2. [关于c++ new操作符的重载](https://blog.csdn.net/qq_35987777/article/details/102969055)
3. [C++ 中 new 操作符内幕：new operator、operator new、placement new](https://www.cnblogs.com/slgkaifa/p/6887887.html)


# 举例
```C++ {.line-numbers}
class Point
{
public:
  int _x;
  int _y;

  Point(int x = 0, int y = 0) : _x(x), _y(y) {
    cout << "Point(int x = " << this->_x
         << ", int y = " << this->_y
         << ")" << endl;
  }

  ~Point() { cout << "~Point()" << endl; };

  Point(const Point &rhs) : _x(rhs._x), _y(rhs._y) {
    // copy constructor
    cout << "Point(const Point& rhs)" << endl;
  }

  Point &operator=(Point &&rhs) noexcept {
    // move assignment
    cout << "operator=(Point&& rhs)" << endl;
    _x = std::move(rhs._x);
    _y = std::move(rhs._y);
    rhs._x = 0;
    rhs._y = 0;
    return *this;
  }

  // 因为要求operator new要求第一个参数必须是size_t类型，所以声明为静态static成员函数
  static void *operator new(size_t sz) {
    cout << "Point::operator new()" << endl;
    cout << "size_t sz = " << sz << endl;
    auto ptr = ::malloc(sz);
    return ptr;
  }

  // operator new[]和operator new一个用于申请数组，一个用于申请单个对象
  static void *operator new[](size_t sz) {
    cout << "Point::operator new[]()" << endl;
    // sz的大小大于实际需要的数组大小，原因是会额外加size_t类型大小的内存来存储数组大小信息
    cout << "size_t sz = " << sz << endl;
    auto ptr = ::malloc(sz);
    cout << "ptr = " << ptr << endl;
    return ptr;
  }

  static void operator delete(void *ptr) {
    cout << "Point::operator delete()" << endl;
    ::free(ptr);
  }

  static void operator delete[](void *ptr) {
    cout << "Point::operator delete[]()" << endl;
    ::free(ptr);
  }

  void print() {
    cout << this->_x << " " << this->_y << endl;
  }
};


int main() {
  /* 测试单个对象申请 */
  auto pp1 = new Point(); 

  /* 测试数组申请 */
  auto pp = new Point[2]();
  {
    // 以下代码用于测试数组申请是多出来的size_t个字节：
    cout << "pp = " << (void *)pp << endl;
    void *psz = (size_t *)pp - 1; // psz指向存放堆空间数组大小的存放位置
    cout << "psz = " << (void *)psz << endl;
    cout << "sz = " << *(long long *)psz << endl;
  }
  pp[0].print();
  pp[1] = Point(3, 4);
  pp[1].print();
  delete[] pp;
}
```