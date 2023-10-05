## shared_mutex
C++标准库中，利用`RAII`思想实现了两种对互斥锁的包装器`unique_lock`和`shared_lock`。
`shared_lock`是以共享的方式来进行加锁，当共享互斥量（`shared_mutex`）没有被其他线程持有或者被其他线程以共享的方式持有时，则该线程可以以共享的方式立即获取到这把锁（对这把锁进行共享式上锁）。

shared_lock源码节选:
```C++
template <class _Mutex>
class shared_lock
{
private:
  _Mutex *_Pmtx;
  bool _Owns; //_Owns代表当前是否持有这把锁

public:
  shared_lock() : _Pmtx(nullptr), _Owns(false) {}

  shared_lock(_Mutex &_Mtx) : _Pmtx(addressof(_Mtx)), _Owns(true)
  {
    _Mtx.lock_shared();
  }

  ~shared_lock()
  {
    if (_Owns)
    {
      _Pmtx->unlock_shared();
    }
  }

  shared_lock(shared_lock &&_Other) : _Pmtx(_Other._Pmtx), _Owns(_Other._Owns)
  {
    _Other._Pmtx = nullptr;
    _Other._Owns = false;
  }

  shared_lock &operator=(shared_lock &&_rhs)
  {
    if (_Owns)
    {
      _Pmtx->unlock_shared();
    }

    _Pmtx = _rhs._Pmtx;
    _Owns = _rhs._Owns;
    _rhs._Pmtx = nullptr;
    _rhs._Owns = false;
    return *this;
  }
  void unlock() { 
    if (!_Pmtx || !_Owns) { // _Pmtx为空或_Owns为false时，抛出异常
        _Throw_system_error(errc::operation_not_permitted);
    }

    _Pmtx->unlock_shared();
    _Owns = false;
  }

  /* shared_mutex禁止拷贝 */
  shared_lock(const shared_lock &) = delete;
  shared_lock &operator=(const shared_lock &) = delete;
};
```
使用`_Owns`进行为了防止出现**对已经解锁的锁进行重复解锁**的操作，这是不允许的，因为 `shared_mutex` 是非递归锁，不能重复解锁，重复解锁会导致程序崩溃，重复加锁同理。

:question:什么情况下会出现上面说的对已经解锁的锁进行重复解锁的操作？

:star:考虑如下代码，`tick`作为全局变量，在本例中也是临界资源，考虑到并发编程中对临界资源的竞争访问，所以在访问`tick`前要先上锁，使用完毕后立即解锁，防止并发效率下降。重点关注`try-catch`语块，当网络请求发生异常，且异常被`catch`捕获时，会栈展开（stack unwinding），并将局部对象析构，假如没有`_Owns`，那么在`lock_.unlock()`中已经解锁，又在局部对象的析构中再次解锁，总计解锁两次。
```C++
int tick = 0; 
std::shared_mutex _mutex_lock;

void func() {
  std::shared_lock<std::shared_mutex> lock_(_mutex_lock);
  int _tick = tick;
  lock_.unlock();
  try { /* 用临界资源数据（_tick）的信息构建请求去访问远程服务器 */ }
  catch (...) {
    std::cerr << "error message" << std::endl;
    throw;
  }
  lock_.lock();
  if (_tick == tick) {
    // 业务逻辑 ··· 使用服务器返回的结果 
  }
}
```




## 参考资料
[C++ shared_mutex应用以及源码解析-知乎](https://zhuanlan.zhihu.com/p/654973131)