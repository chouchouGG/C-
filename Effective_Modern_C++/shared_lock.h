class shared_mutex
{ // class for mutual exclusion shared across threads
public:
  using native_handle_type = void *;

  shared_mutex() noexcept // strengthened
      : _Myhandle(nullptr)
  {
  }

  ~shared_mutex() noexcept {}

  void lock() noexcept /* strengthened */
  {                    // lock exclusive
    _Smtx_lock_exclusive(&_Myhandle);
  }

  bool try_lock() noexcept /* strengthened */
  {                        // try to lock exclusive
    return _Smtx_try_lock_exclusive(&_Myhandle) != 0;
  }

  void unlock() noexcept /* strengthened */
  {                      // unlock exclusive
    _Smtx_unlock_exclusive(&_Myhandle);
  }

  void lock_shared() noexcept /* strengthened */
  {                           // lock non-exclusive
    _Smtx_lock_shared(&_Myhandle);
  }

  bool try_lock_shared() noexcept /* strengthened */
  {                               // try to lock non-exclusive
    return _Smtx_try_lock_shared(&_Myhandle) != 0;
  }

  void unlock_shared() noexcept /* strengthened */
  {                             // unlock non-exclusive
    _Smtx_unlock_shared(&_Myhandle);
  }

  _NODISCARD native_handle_type native_handle() noexcept /* strengthened */
  {                                                      // get native handle
    return &_Myhandle;
  }

  shared_mutex(const shared_mutex &) = delete;
  shared_mutex &operator=(const shared_mutex &) = delete;

private:
  _Smtx_t _Myhandle;
};
