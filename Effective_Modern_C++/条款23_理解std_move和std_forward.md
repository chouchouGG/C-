## 从实现的角度理解 `std::move`
 为了避免C++14之前，如 `typename std::remove_reference<T>::type` 这样冗长的写法，C++14声明了别名模板，如下：

```C++
template <typename T>
using remove_reference_t = typename std::remove_reference<T>::type;
```

### 动手实现自己的 std::move

```C++
/* 实现自己的mymove函数模板 */
template <class T>
std::remove_reference_t<T> &&   /* 返回类型 */
mymove(T &&param)
{
  using return_type = std::remove_reference_t<T> &&;
  return static_cast<return_type>(param);
}

int a = 10;
int &&ra = mymove(a); /* 编译通过 */
```
返回类型中的 `std::remove_reference_t<T>` 确保了返回值类型是一个**右值引用**。因为**函数返回的右值引用是右值**。



## `std::move` 的注意事项
1.不要在你希望能移动对象的时候，声明他们为const。(具有右值属性的 `const` 对象的**移动操作**会被转化为**拷贝操作**)。

2.`std::move` 不移动任何对象，也不保证对象被移动。

总结：关于 `std::move`，能确保的唯一一件事就是将 `std::move` 应用到一个对象上，你能够得到一个右值（ `const` 属性的右值仍然执行**拷贝语义**而非**移动语义**）。