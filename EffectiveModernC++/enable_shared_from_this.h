// _Ty：派生类的类型信息通过模版参数的方式传入
template <class _Ty>
class enable_shared_from_this
{
private:
  weak_ptr<_Ty> _Wptr;

public:
  shared_ptr<_Ty> shared_from_this()
  {
    return shared_ptr<_Ty>(_Wptr);
  }

  shared_ptr<const _Ty> shared_from_this() const
  {
    return shared_ptr<const _Ty>(_Wptr);
  }

protected:
  enable_shared_from_this() : _Wptr() {}

  enable_shared_from_this(const enable_shared_from_this &) : _Wptr() {}

  enable_shared_from_this &operator=(const enable_shared_from_this &)
  { // assign (must not change _Wptr)
    return *this;
  }

private:
  template <class _Yty>
  friend class shared_ptr;
};

/* shared_from_this的调用过程 */
enable_shared_from_this<_Ty>::shared_ptr<_Ty> shared_from_this()
{
  return shared_ptr<_Ty>(_Wptr); /* _Wptr是一个weak_ptr */
}

template <class _Ty2>
shared_ptr<_Ty>::shared_ptr(const weak_ptr<_Ty2> &_Other)
{ // construct shared_ptr object that owns resource *_Other
  if (!this->_Construct_from_weak(_Other))
  {
    _Throw_bad_weak_ptr();
  }
}

template <class _Ty2>
bool _Ptr_base<_Ty>::_Construct_from_weak(const weak_ptr<_Ty2> &_Other)
{
  if (_Other._Rep && _Other._Rep->_Incref_nz()) //_Other._Rep->_Incref_nz() 判断引用计数是否为0，不为零则加一并返回
  {
    _Ptr = _Other._Ptr;
    _Rep = _Other._Rep;
    return true;
  }

  return false;
}

// 对enable_shared_from_this的数据成员_Wptr完成初始化
template <class _Ux, >
shared_ptr<_Ty>::shared_ptr(_Ux *_Px)
{ // construct shared_ptr object that owns _Px
  _Temporary_owner<_Ux> _Owner(_Px);
  _Set_ptr_rep_and_enable_shared(_Owner._Ptr, new _Ref_count<_Ux>(_Owner._Ptr));
  _Owner._Ptr = nullptr;
}

template <class _Ux>
void shared_ptr<_Ty>::_Set_ptr_rep_and_enable_shared(_Ux *const _Px, _Ref_count_base *const _Rx)
{ // take ownership of _Px
  this->_Ptr = _Px;
  this->_Rep = _Rx;
  if constexpr (_Can_enable_shared < _Ux >>)
  {
    if (_Px && _Px->_Wptr.expired())
    {
      /* _Wptr完成初始化 */
      _Px->_Wptr = shared_ptr<remove_cv_t<_Ux>>(*this, const_cast<remove_cv_t<_Ux> *>(_Px));
    }
  }
}