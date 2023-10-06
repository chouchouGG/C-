template <class _Ty>
class shared_ptr : public _Ptr_base<_Ty>
{ // class for reference counted resource management
  using _Mybase = _Ptr_base<_Ty>;

public:
  shared_ptr() = default;

  shared_ptr(nullptr_t) {} // construct empty shared_ptr

  template <class _Ux>
  shared_ptr(_Ux *_Px)
  { // construct shared_ptr object that owns _Px
    if (is_array_v<_Ty>)
    {
      _Setpd(_Px, default_delete<_Ux[]>{});
    }
    else
    {
      _Temporary_owner<_Ux> _Owner(_Px);
      _Set_ptr_rep_and_enable_shared(_Owner._Ptr, new _Ref_count<_Ux>(_Owner._Ptr));
      _Owner._Ptr = nullptr;
    }
  }

  template <class _Ux, class _Dx>
  shared_ptr(_Ux *_Px, _Dx _Dt)
  { // construct with _Px, deleter
    _Setpd(_Px, std::move(_Dt));
  }

  template <class _Ux>
  shared_ptr(_Ux *_Px, _Dx _Dt, _Alloc _Ax)
  { // construct with _Px, deleter, allocator
    _Setpda(_Px, std::move(_Dt), _Ax);
  }

  template <class _Dx>
  shared_ptr(nullptr_t, _Dx _Dt)
  { // construct with nullptr, deleter
    _Setpd(nullptr, std::move(_Dt));
  }

  template <class _Dx>
  shared_ptr(nullptr_t, _Dx _Dt, _Alloc _Ax)
  { // construct with nullptr, deleter, allocator
    _Setpda(nullptr, std::move(_Dt), _Ax);
  }

  ~shared_ptr()
  { // release resource
    this->_Decref();
  }

  shared_ptr &operator=(const shared_ptr &_Right)
  {
    shared_ptr(_Right).swap(*this);
    return *this;
  }

  template <class _Ty2>
  shared_ptr &operator=(const shared_ptr<_Ty2> &_Right)
  {
    shared_ptr(_Right).swap(*this);
    return *this;
  }

  shared_ptr &operator=(shared_ptr &&_Right)
  { // take resource from _Right
    shared_ptr(std::move(_Right)).swap(*this);
    return *this;
  }

  template <class _Ty2>
  shared_ptr &operator=(shared_ptr<_Ty2> &&_Right)
  { // take resource from _Right
    shared_ptr(std::move(_Right)).swap(*this);
    return *this;
  }

  template <class _Ux, class _Dx>
  shared_ptr &operator=(unique_ptr<_Ux, _Dx> &&_Right)
  { // move from unique_ptr
    shared_ptr(std::move(_Right)).swap(*this);
    return *this;
  }

  void swap(shared_ptr &_Other)
  {
    this->_Swap(_Other);
  }

  void reset()
  { // release resource and convert to empty shared_ptr object
    shared_ptr().swap(*this);
  }

  template <class _Ux>
  void reset(_Ux *_Px)
  { // release, take ownership of _Px
    shared_ptr(_Px).swap(*this);
  }

  template <class _Ux, class _Dx>
  void reset(_Ux *_Px, _Dx _Dt)
  { // release, take ownership of _Px, with deleter _Dt
    shared_ptr(_Px, _Dt).swap(*this);
  }

  template <class _Ux, class _Dx, class _Alloc>
  void reset(_Ux *_Px, _Dx _Dt, _Alloc _Ax)
  { // release, take ownership of _Px, with deleter _Dt, allocator _Ax
    shared_ptr(_Px, _Dt, _Ax).swap(*this);
  }

  using _Mybase::get;

  template <class _Ty2 = _Ty>
  _Ty2 &operator*() const { return *get(); }

  template <class _Ty2 = _Ty>
  _Ty2 *operator->() const { return get(); }

  operator bool() const { return get() != nullptr; }
};

// shared_ptr和weak_ptr的基类
template <class _Ty>
class _Ptr_base
{
  using element_type = remove_extent_t<_Ty>;

private:
  element_type *_Ptr{nullptr};
  _Ref_count_base *_Rep{nullptr};

public:
  long use_count() const
  {
    return _Rep ? _Rep->_Use_count() : 0;
  }

  _Ptr_base(const _Ptr_base &) = delete;
  _Ptr_base &operator=(const _Ptr_base &) = delete;

protected:
  element_type *get() const { return _Ptr; }

  void _Swap(_Ptr_base &_Right)
  {
    std::swap(_Ptr, _Right._Ptr);
    std::swap(_Rep, _Right._Rep);
  }
};

// 引用计数的通用代码
class _Ref_count_base
{
private:
  unsigned long _Uses = 1;
  unsigned long _Weaks = 1;

  long _Use_count() const
  {
    return static_cast<long>(_Uses);
  }
};