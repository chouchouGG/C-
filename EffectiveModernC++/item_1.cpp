#include <thread>
#include <shared_mutex>
#include <string>
#include <iostream>
#include <chrono>

using std::shared_lock;
using std::shared_mutex;
using std::string;
using std::thread;
using std::unique_lock;

/* 以下代码，利用shared_mutex的两种所有权（共享所有权、独占所有权）来实现对计数器count的独占式读、共享式写 */
shared_mutex _mutex_lock;
size_t count = 0; /* 计数器 */

void read(const string &thread_name)
{
  while (true)
  {
    {
      shared_lock<shared_mutex> rlock{_mutex_lock}; /* 加锁 */
      std::cout << thread_name << " read: " << count << std::endl;
    }
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(1s);
  }
}

void write(const string &thread_name)
{
  while (true)
  {
    {
      unique_lock<shared_mutex> wlock{_mutex_lock}; /* 加锁 */
      std::cout << thread_name << " write: " << ++count << std::endl;
    }
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(1s);
  }
}

int main()
{
  thread read_01(read, string("read_01"));
  thread read_02(read, string("read_02"));
  thread write_01(write, string("write_01"));

  read_01.join();
  read_02.join();
  write_01.join();
  return 0;
}
