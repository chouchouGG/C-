## `make_unique`和`make_shared`的优势
### 异常安全
```C++ {.line-numbers}
void F(const std::shared_ptr<Lhs>& lhs, const std::shared_ptr<Rhs>& rhs) {
  //...
}

F(std::shared_ptr<Lhs>(new Lhs("foo")), std::shared_ptr<Rhs>(new Rhs("bar")));
```
> **C++ 是不保证参数求值顺序, 以及内部表达式的求值顺序的**。

结合上面引用中的话，思考为什么上述代码为什么会出现内存泄露的风险？

:star:答案和编译器将源码转换为目标代码有关。

所以可能的执行顺序如下:

```C++ {.line-numbers}
执行 new Lhs(“foo”)
执行 new Rhs(“bar”)
执行 std::shared_ptr 的构造函数
执行 std::shared_ptr 的构造函数
```
假设在第 `2` 步的时候, 抛出了一个异常 (比如 out of memory), 那么第一步申请的 `Lhs` 对象内存就会发生泄露。 

这个问题的核心在于，通过传入`new`出来的原始指针创建`shared_ptr`的操作中内存申请和对象创建并没有做到原子性。

#### 使用`make_shared`和`make_unique`可以防止`new`表达式的内存泄露风险。
```C++ {.line-numbers}
F(std::make_shared<Lhs>("foo"), std::make_shared<Rhs>("bar"));
```
`std::make_shared`使得堆对象创建，和`shared_ptr`的接管及时完成。

同样的使用`make_unique`也可以写出同样的异常安全代码。

### `make_shared`的效率更高

```C++ 
std::shared_ptr<Lhs> spw(new Lhs("foo"));
```
会进行两次内存申请，`new`一次，`shared_ptr`构造函数创建**控制块**还要申请一次。


```C++ 
auto sptr = std::make_shared<Lhs>("foo");
```
`std::make_shared`只会申请一次内存，同时容纳了`Lhs`对象和**控制块**。

## `make_unique`和`make_shared`的弊端
1. `make`函数都不允许传入自定义删除器。

2. `make`函数会将接收的参数**完美转发**给对象构造函数，但是它们是使用**圆括号**，即`make`函数不会调用对象构造函数的`std::initializer_list`作为参数的重载形式的版本。

> ```C++ 
> auto spv = std::make_shared<std::vector<int>>(10, 20);
> // 创建了`10`个元素，每个值为`20`的`std::vector`。
> ```

## `make_shared`的高效率，可能造成内存的延迟释放
**C++中内存只能一起申请一起释放，不能申请一块释放一部分**。

由于`std::make_shared`只会申请一次内存，所以在释放空间时也只能将控制块和对象本身的空间一起释放。

我们知道通过`shared_ptr`管理的资源，其资源的释放由`shared count`决定，但是控制块的释放是由`shared count`和`weak count`一起决定，所以只有当其二者都为 `0` 时，控制块才能释放；否则，控制块不能释放。

通过原始指针创建的shared_ptr，其对象占用内存的释放和控制块占用内存释放是相互独立的。而通过make_shared创建的shared_ptr，其二者的空间是一起申请的，故其二者内存空间的释放是绑定在一起的，只有当控制块被释放时，对象占用的空间才能重新交给操作系统。



## 不要使用`make_shared`函数去创建重载了`operator new`和`operator delete`类的对象
由于`make_shared`把对象和控制块的内存空间一起申请的原因，所以并不适合去调用用户自定义的`operator new`和`operator delete`。实际上`std::make_shared`的实现中也只会去调用标准库中定义的`operator new`和`operator delete`。

