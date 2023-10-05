# decltype类型推导

## `decltype` 的使用形式:
```C++
decltype(exp) varname;
```

## `decltype` 推导规则：
- 如果 `exp` 是一个类成员访问表达式，或者是一个变量（变量肯定有名字），那么推导类型就和 `exp` 一致。
- 如果 `exp` 是函数调用，那么推导类型就和函数返回值的类型一致。
- 如果 `exp` 是一个左值，或者被括号 `()` 包围，那么推导类型就是 `exp` 的引用。


## 参考资料：
[C++11之decltype类型推导](https://blog.csdn.net/qq_45254369/article/details/127372898)