## const常量不一定是编译期常量
`const`修饰的实体，除了表示该实体不能被修改外，还有可能代表该实体具有编译期常量属性。之所以说有可能，见如下代码：
```C++
const int a = 10;    /* 编译期常量 */
const int n = 1 + 2; /* 编译期常量 */
int b = 20;
const int c = b; /* 非编译期常量 */
``` 
编译期常量意思是该`const`常量是编译器可以读到的，那么在一些需要常量属性值的地方就可以使用`const`常量，如下。
```C++
int array[a]; /* 编译通过 */
int array[c]; /* 编译失败，c不是一个编译期常量 */
```

## constexpr和const修饰变量
1.被`constexpr`成功修饰的变量都一定是**编译期常量**。

2.`constexpr`对象都是`const`，但`const`对象并非都是`constexpr`。

## constexpr修饰函数时



