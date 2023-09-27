#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <random>
#include <memory>

using namespace std;

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
