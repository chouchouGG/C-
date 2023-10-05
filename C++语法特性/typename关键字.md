## 形参中的typename
`std::forward` 形参中的 `typename` 必须要加上，才能够说明这个模板形参通过作用域限定符使用类中定义的类型别名，而不是静态成员变量。这个是必须加的，否则编译不通过（编译不通过的原因是因为，C++默认情况下将 `类名::XXX` 解释为数据成员而不是类型）。而且只需要对模板形参使用 `typename`，因为一个已知的类，编译器都是可以直接知道这是类型还是静态数据成员。

如下示例展示了 何时需要使用 `typename`:
```C++
struct A { typedef int type; };
struct B { static int val; };

int B::val = 1; // B类中静态数据成员初始化

template <typename T_1, typename T_2>
class C {
  public:
  // 具体的类名前可以不加 typename
  typedef A::type type_A;
  // 类型参数前必须加上 typename，向编译器说明 T_1::type 中的 type 是一种类型
  typedef typename T_1::type type_T_1;
  

  C() { _c = T_2::val; /* 使用静态成员变量前不用加 typename */ }

  private:
    int _c;
};

int main() {
  C<A, B> c;  // 创建C类的对象
}

```


