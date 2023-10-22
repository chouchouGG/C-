#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <random>
#include <memory>
#include <stdio.h>
#include <list>
#include <algorithm>
#include <functional>
#include <cassert>
#include <chrono>

using namespace std;

std::ostream &operator<<(std::ostream &ostr, const std::list<int> &list)
{
  for (auto &i : list)
  {
    ostr << " " << i;
  }
  return ostr;
}

/* 测试算法库中的std::move函数 */
#if 0
int main(int argc, char const *argv[])
{
  
  return 0;
}

#endif

/* 测试给无符号整数赋值-1的结果 */
#if 0
int main()
{
  size_t pid = -1;
  unsigned int a = -1;
  unsigned short int b = -1;
  cout << pid << endl;
  cout << a << endl;
  cout << b << endl;
  return 0;
}
#endif

/* 测试assert的条件 */
#if 0
#define BUSTUB_ASSERT(expr, message) assert((expr) && (message))
int main() {
  // bool i = true;
  bool i = false;
  BUSTUB_ASSERT(i, "ERROR");
  cout << "well done!\n";
  return 0;
}
#endif

/* 测试迭代器的操作 */
#if 0
int main()
{
  vector<int> v{3, 1, 4, 8};
  auto it = v.begin();
  std::advance(it, 2);
  cout << *it << '\n';
  it = std::prev(it);
  cout << *it << '\n';
  it = std::prev(it);
  cout << *it << '\n';
  it = std::next(it);
  cout << *it << '\n';
}
#endif

/* 测试std::hash的使用 */
#if 0
int main()
{
  std::hash<int> hash_int;
  cout << hash_int(100) << endl;
  cout << hash_int(10) << endl;
  cout << hash_int(90) << endl;
  
  std::hash<string> hash_str;
  cout << hash_str("asdf") << endl;
  cout << hash_str("asd") << endl;
  cout << hash_str("asd") << endl;
}
#endif

/* 测试std::list容器的remove和remove_if */
#if 0
int main()
{
    std::list<int> list_ = { 1, 100, 2, 3, 10, 1, 11, -1, 12 };
 
    list_.remove(1); // remove both elements equal to 1
    std::for_each(list_.begin(), list_.end(), [](auto & elem){
      std::cout << elem << " ";
    });
    std::cout << '\n';
    

    list_.remove_if([](int n){
      return n > 10; 
    }); // remove all elements greater than 10
    std::for_each(list_.begin(), list_.end(), [](auto & elem){
      std::cout << elem << " ";
    });
    std::cout << '\n';
}
#endif

/* test_5 : 测试浮点数的精度问题 */
#if 0
int main()
{
  float x = 0.1;
  float y = 0.2;
  printf("x+y = %.20f\n", x + y);
  printf("0.3 = %.20f\n", 0.3);
}
#endif

/* test_4 : &this->ch_和&(this->i_)有没有区别*/
#if 0
struct A
{
  A(int i, char ch) : i_(i), ch_(ch) { cout << "constructor" << endl; }
  ~A() { cout << "destructor" << endl; }

  void test()
  {
    cout << &i_ << endl;
    cout << &this->i_ << endl;
    cout << &(this->i_) << endl;
    cout << "------" << endl;
    cout << (void *)&ch_ << endl;
    cout << (void *)&this->ch_ << endl;
    cout << (void *)&(this->ch_) << endl;
  }

  int i_;
  char ch_;
};

int main()
{
  A a(1, 'a');
  a.test();
}
#endif

/* test_3 : STL容器为空时，begin和end是否相同*/
#if 0
int main(int argc, char const *argv[])
{
  unordered_map<char, int> mmap;
  cout << (mmap.begin() == mmap.end() ? "true" : "false") << endl;
  mmap['a'] = 1;
  cout << (mmap.begin() == mmap.end() ? "true" : "false") << endl;
  return 0;
}
#endif

/* test_2 : erase清除智能指针是否会调用析构函数*/
#if 0
struct A
{
  A(int i = 1) : i_(i) { cout << "constructor" << endl; }
  ~A() { cout << "destructor : " << this->i_ << endl; }

  int i_;
};

int main()
{
  vector<unique_ptr<A>> v;
  v.push_back(make_unique<A>(10));
  cout << "---" << endl;
  v.erase(v.begin());
  cout << "---\n"
       << endl;

  unordered_map<char, unique_ptr<A>> m;
  m['a'] = make_unique<A>(20);
  cout << "---" << endl;
  m.erase('a');
  cout << "---\n"
       << endl;
  return 0;
}
#endif

/* test_1 */
#if 0
std::vector<std::string> GenerateNRandomString(int n)
{
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<char> char_dist('A', 'z');
  std::uniform_int_distribution<int> len_dist(1, 30);

  std::vector<std::string> rand_strs(n);

  for (auto &rand_str : rand_strs)
  {
    int str_len = len_dist(gen);
    for (int i = 0; i < str_len; ++i)
    {
      rand_str.push_back(char_dist(gen));
    }
  }
  return rand_strs;
}

int main(int argc, char const *argv[])
{
  int num = 0;
  cout << "Please enter the number of string: ";
  cin >> num;
  vector<string> vec = GenerateNRandomString(num);
  for (auto &str : vec)
  {
    cout << str << endl;
  }
  return 0;
}
#endif
