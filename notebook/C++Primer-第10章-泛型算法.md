# 第10章-泛型算法

## &lt;algorithm&gt;

### equal

用途：逐个对比两个序列中的元素是否相同，相同则返回true，否则返回false。

```C++
bool equal( InputIt1 first1, InputIt1 last1,
            InputIt2 first2 );
bool equal( InputIt1 first1, InputIt1 last1, 
            InputIt2 first2, InputIt2 last2 );
```

- 在三个参数版本中，equal基于一个假设：即第二个序列至少要去第一个序列一样长，如果不满足equal会试图访问第二个序列末尾之后的元素（不存在），就产生错误。



### fill

用途：将 `value` 值赋给输入序列中的每个元素。

```C++
void fill( ForwardIt first, ForwardIt last, const T& value );
```



### fill_n

用途：将 `value` 值赋给从迭代器`first`开始的`count`个元素。

```C++
void fill_n( OutputIt first, Size count, const T& value );
```

- 要确保`first`之后存在`count`个元素，否则该行为为未定义。

#### 题目：找出下面代码的错误：

- ```C++
  vector<int> vec;
  vec.reserve(10); 
  fill_n(vec.begin(), 10, 0);
  ```

- 答案：

  ```C++
  vector<int> vec;
  vec.resize(10); 	// √
  fill_n(vec.begin(), 10, 0);
  ```

  `reserve ` 分配了足够的空间，但是不会创建新元素，即 ` vector ` 的 `size = 0` 。**标准库中的算法可能改变容器中保存的元素的值，也可能在容器内移动元素，永远不会直接添加和删除元素**，所以此处应该改为 `resize(10)`。



### copy

用途：将**输入序列**的元素拷贝到从 `d_first` 开始的**目的序列**。

```C++
OutputIt copy( InputIt first, InputIt last, OutputIt d_first );
```

- **返回值**：返回最后一个被拷贝的位置之后的迭代器。

- 要确保**目的序列**的元素个数至少和**输入序列**中的个数一样。



### replace、replace_if

用途：`(1)` 将输入序列中的 `old_value` 替换为 `new_value` 。

​			`(2)` 将一元谓词 `p` 返回为真的值替换为 `new_value` 。

```C++
void replace( ForwardIt first, ForwardIt last,
              const T& old_value, const T& new_value ); 	// (1)

void replace_if( ForwardIt first, ForwardIt last,
                 UnaryPredicate p, const T& new_value ); 	// (2)
```



### replace_copy、replace_copy_if

用途：`(1)` 相比 `replace` 会修改原序列，`replace_copy ` 会将替换后的数据存到另一个容器中。

​			`(2)` 相比 `replace_if` 会修改原序列，`replace_copy_if ` 会将替换后的数据存到另一个容器中。

```C++
OutputIt replace_copy( InputIt first, InputIt last, 
                       OutputIt d_first,
                       const T& old_value, const T& new_value );	// (1)

OutputIt replace_copy_if( InputIt first, InputIt last, 
                          OutputIt d_first,
                          UnaryPredicate p, const T& new_value );	// (2)
```

- **返回值**：返回 `d_first` 的past the last的迭代器。



### unique、unique_copy

用途：用于重排输入序列，将相邻的重复项“消除”。（这里的“消除”指的是用后面元素覆盖前面重复元素。）

```C++
ForwardIt unique( ForwardIt first, ForwardIt last ); 						// (1)
ForwardIt unique( ForwardIt first, ForwardIt last, BinaryPredicate p );		// (2)

OutputIt unique_copy( InputIt first, InputIt last, 							
                      OutputIt d_first );									// (3)
OutputIt unique_copy( InputIt first, InputIt last, 
                      OutputIt d_first, BinaryPredicate p );				// (4)
```

- **返回值**：返回一个指向**不重复序列范围**后一个的元素的迭代器。
- `(1)`、`(2)`的区别是元素的比较方式不同，`(2)`采用二元谓词。
- `(3)`、`(4)` 的特殊之处在于，将结果**拷贝到另一个**以 `d_first` 开始的序列。



### sort、stable_sort

用途：用于对输入序列中的元素排序。

```C++
void sort( RandomIt first, RandomIt last );
void sort( RandomIt first, RandomIt last, Compare comp );

void stable_sort( RandomIt first, RandomIt last );
void stable_sort( RandomIt first, RandomIt last, Compare comp );
```

- `comp` 是自定义的比较函数。
- `stable_sort` 和 `sort` 函数功能相似，不同之处在于，该函数不会改变相等元素的相对位置。
- `stable_sort` 函数是基于归并排序实现的。
- `sort` 函数是基于快速排序实现的。




### partition、stable_partition

用途：用于将输入序列中的元素按照某种条件划分为两部分。

```C++
ForwardIt partition( ForwardIt first, ForwardIt last, UnaryPredicate p ); 	

BidirIt stable_partition( BidirIt first, BidirIt last, UnaryPredicate p );	//保持原有次序
```

- 将输入序列中的元素分为两部分，使谓词 `p` 返回 `true` 的元素位于谓词 `p` 返回 `false` 的元素**之前**。元素的相对顺序不保留。
- **返回值**：返回第一部分末尾元素之后位置的迭代器（即，第二部分的首个元素的迭代器）。



### find、find_if、find_if_not

```C++
InputIt find( InputIt first, InputIt last, const T& value );			// (1)

InputIt find_if( InputIt first, InputIt last, UnaryPredicate p );		// (2)

InputIt find_if_not( InputIt first, InputIt last, UnaryPredicate q );	// (3)
```

- 1) find查找等于value的元素

  3) find_if查找谓词p返回true的元素

  5) find_if_not查找谓词q返回false的元素

- **返回值**：返回第一个满足条件的元素的迭代器，如果这样的元素不存在，则返回 `past-the-last` 位置的迭代器。



### for_each

用途：对输入序列中的每个元素调用一元函数 `f`。

```C++
UnaryFunction for_each( InputIt first, InputIt last, UnaryFunction f );
```

- **返回值**：返回 `for_each` 所使用的可调用对象。
- `UnaryFunction f` 不需要有返回值。



### transform

用途：`(1)`对输入序列中的每个元素调用 `unary_op` ，并将结果存到 `d_first` ，保证了输入序列不被改动。

​			`(2)`对**第一序列**和**第二序列**中的各个相对应元素调用 `binary_op` ，并将结果存到 `d_first` 。

```C++
OutputIt transform( InputIt first1, InputIt last1, 
                    OutputIt d_first,
                    UnaryOperation unary_op );			// (1)
OutputIt transform( InputIt1 first1, InputIt1 last1, 
                    InputIt2 first2,
                    OutputIt d_first,
                    BinaryOperation binary_op );		// (2)
```

- **返回值**：返回一个迭代器，指向目的序列中最后一个被transform的元素的后一个位置。
- `(1)` 和 `(2)` 中假定 `d_first` 开始的序列，有足够的空间存储输入序列处理后的各个元素。
- `(2)` 中假定**第二序列**的元素个数至少和**第一序列**的元素个数一样。
- `unary_op` 、`binary_op`必须有返回值，解释如下：

- ```C++
  // 从源码中可以知道，1、2版本中的可调用对象都进行了赋值操作，即可调用对象需要具有返回值。
  for (; _UFirst != _ULast; ++_UFirst, (void) ++_UDest) {
  	*_UDest = _Func(*_UFirst); //关键
  }													// (1)
  for (; _UFirst1 != _ULast1; ++_UFirst1, (void) ++_UFirst2, ++_UDest) {
      *_UDest = _Func(*_UFirst1, *_UFirst2); //关键
  }													// (2)
  ```



### reverse、reverse_copy

用途：反转输入序列。

```C++
void reverse( BidirIt first, BidirIt last );

OutputIt reverse_copy( BidirIt first, BidirIt last, OutputIt d_first );
```



### remove、remove_if

用途：`(1)` 移除所有等于 `value` 的元素。

​			`(2)` 移除谓词 `p` 返回 `true` 的所有元素。

```c++
ForwardIt remove( ForwardIt first, ForwardIt last, const T& value ); 		// (1)

ForwardIt remove_if( ForwardIt first, ForwardIt last, UnaryPredicate p );  	// (2)
```



### remove_copy、remove_copy_if

- 与 `remove、remove_if` 相比，将处理后的序列存到了 `d_first` 。

```C++
OutputIt remove_copy( InputIt first, InputIt last, 
                      OutputIt d_first,
                      const T& value );

OutputIt remove_copy_if( InputIt first, InputIt last, 
                         OutputIt d_first,
                         UnaryPredicate p );
```



## &lt;numeric&gt;

### accumulate

  用途：将 `first` 、`last` 所指范围内的值加到 `init` 上，并返回最终的和。

```C++
T accumulate( InputIt first, InputIt last, T init );
```

- 第三个参数类型 `T` 决定了函数中使用 `T` 类型的加法运算符。

- 第三个参数作为求和的初始值



## &lt;functional&gt;

### bind

用途：用于接受一个可调用对象，并生成一个新的可调用对象。

```C++
/*unspecified*/ bind( F&& f, Args&&... args );
```

- `f` 是可调用对象，它将绑定到 `args` 上。
- `args` 是逗号分隔的参数列表，对应给定的 `f` 的参数。

- 未绑定的参数被命名空间 `std::placeholders::` 中的占位符 `_1`，`_2`，`_3` . . . `_n` 占位。
- `bind` 接受的参数个数是 *被绑定的可调用对象的参数个数*  + 1。

#### 应用1：重排参数

```C++
// 按单词长度由长到短排序
sort(words.begin(), words.end(), isShorter);
// 按单词长度由短到长排序
sort(words.begin(), words.end(), bind(isShorter, _2, _1));
```

#### 应用2：绑定引用参数

- `bind` 中**参数绑定**默认是通过**拷贝**进行的；但是对于一些没有拷贝构造函数的类型，只能以引用来绑定。
- 例如输出流对象`cout`：

```C++
ostream& print(ostream& os, const string& s, char c) {
	return os << s << c;
}

void test() {
	vector<string> v{ "AA", "BB", "CC" };
    // for_each(v.begin(), v.end(), bind(print, cout, _1, ' '));		// ×
	for_each(v.begin(), v.end(),
				 bind(print, ref(cout), std::placeholders::_1, ' ')); 	// √
}
```



## ref、cref

用途：`(1)` 用于返回接受对象的**引用**，该引用是可以拷贝的。

​			`(2)` 用于返回接受对象的**常量引用**，该引用是可以拷贝的。

```C++
std::reference_wrapper<T> ref(T& t);

std::reference_wrapper<const T> cref( const T& t );
```



## &lt;iterator&gt;

### 插入迭代器

1. **插入器**是一种迭代器适配器，插入器有三种`back_inserter`、`front_inserter`、`inserter` 。

2. **插入器 ≠ 插入迭代器** 。

3. 假定存在**插入迭代器** `it` ，*C++*中 `*it`，`++it`，`it++` 这些操作均存在，但对 `it` 没有任何效果。

4. 不能通过对**插入迭代器**解引用 `*` ，来取到其所指向的元素的值。

5. 三种插入器的不同：

   - `back_inserter` 使用 `push_back` 。

   - `front_inserter` 使用 `push_front` 。

   - `inserter` 使用 `insert`，此函数接受第二个参数，这个参数必须是一个指向给定容器的迭代器。元素将被插入到给定迭代器所表示的元素之前。

#### back_inserter

用途：创建一个使用 `push_back` 的迭代器。

```C++
back_insert_iterator back_inserter( Container& c );
```

- **返回值**：返回一个 `back_insert_iterator` 。



#### front_inserter

用途：创建一个使用 `push_front` 的迭代器。

```c++
front_insert_iterator front_inserter( Container& c );
```

- **返回值**：返回一个 `front_insert_iterator` 。
- `front_insert_iterator` 类型的插入迭代器始终指向容器的第一个元素。



#### inserter

用途：创建一个使用 `insert` 的迭代器。

```C++
insert_iterator inserter( Container& c, Container::iterator i );
```

- **返回值**：返回一个 `insert_iterator` 。

- 向 `insert_iterator`类型的 **插入迭代器** `it` 赋值 `val`，实际进行了**两部**：
  - 向 `it`  指向的前一个位置插入 `val`，`it`  指向新插入的元素。
  - 递增 `it`，使 `it` 指向原来的元素。

##### 应用：

- ```c++
  vector<string> v{ "AA", "BB", "CC" };
  //打印v; AA BB CC
  auto it = inserter(v, begin(v) + 1);
  it = "DD";
  //打印v; AA DD BB CC
  *it = "EE"; //等效于it = "EE";
  //打印v; AA DD EE BB CC
  ```



### 流迭代器

1. 流迭代器的使用将对应的流当作是一个特定类型的元素序列来处理。这样做的好处很明显：
   - 对流对象的读写过程也可以使用算法，方便快捷。**例如，传入流迭代器以迭代器范围初始化的方式构造vector对象。**
   - 将算法进一步泛化。

#### istream_iterator

用途：`(1)` 默认构造函数创建一个**尾后迭代器** (相当于  `end()` )，指向流对象序列的**终止位置**。

​		    `(2)` 将一个**流迭代器**绑定到**流对象**上，指向流对象序列的**起始位置**。

​	 	   `(3)` 拷贝构造函数（**流迭代器支持拷贝**）。

```C++
istream_iterator();												// (1)
istream_iterator( istream_type& stream );						// (2)
istream_iterator( const istream_iterator& other ) = default;	// (3)
```

- | 成员函数                    | 用途         |
  | --------------------------- | ------------ |
  | operator*、operator->       | 返回当前元素 |
  | operator++、operator++(int) | 前移迭代器   |



#### ostream_iterator

用途：向某个输出流中写入数据。

```c++
ostream_iterator(ostream_type& stream);							// (1)
ostream_iterator(ostream_type& stream, const CharT* delim);		// (2)
```

- `(2)` 接受第二个参数，是一个C-style字符串，在输出每个元素后都会打印此字符串。

- | 成员函数                    | 用途   |
  | --------------------------- | ------ |
  | operator*                   | 无操作 |
  | operator++、operator++(int) | 无操作 |



### 反向迭代器

#### reverse_iterator

1. 当我们从一个普通迭代器初始化一个反向迭代器，或是给一个反向迭代器赋值时，结果迭代器与原迭代器指向的并不是相同的元素。

```C++
reverse_iterator();
reverse_iterator( Iterator x );
reverse_iterator( const reverse_iterator<U>& other );
```

| 成员函数 | 用途                 |
| -------- | -------------------- |
| base     | 用于返回底层的迭代器 |

##### 应用：

- 将 `vector` 中位置 `3` 到 `7` 之间的元素依次按逆序拷贝到 `list` 中。（位置从0开始）

- ```c++
  vector<int> v{ 0,1,2,3,4,5,6,7,8,9 };
  list<int> l;
  reverse_iterator<vector<int>::iterator> end_iter(v.begin() + 3), 				 
  										beg_iter(v.begin() + 8);
  copy(beg_iter, end_iter, back_inserter(l));
  ```





























































































































































































































































































































































