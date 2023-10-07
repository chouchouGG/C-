## `auto` 和 `模板类型推导` 的唯一区别
1. 当使用 `auto` 声明的变量使用花括号的语法进行初始化的时候，会推导出 `std::initializer_list<T>` 的实例化；而模板类型推导不能直接推导出 `std::initializer_list<T>`，除非明确指出（如下代码所示）。
```C++
template<typename T>
void f(std::initializer_list<T> initList);
```

3. **此 `auto` 非彼 `auto`**
在C++14中 `auto` 允许出现在函数返回值或者 `lambda` 函数形参中，但是它的工作机制是模板类型推导那一套方案，而不是 `auto` 类型推导。