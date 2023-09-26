# 理解模板类型推导
内容都是根据如下模板进行：
```C++
template<typename T>
void f(ParamType param);
```

```C++
template <class T>
void f(T param)
{
}
```
