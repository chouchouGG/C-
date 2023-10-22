# 1. [std::list为带头节点的双向循环链表](https://blog.csdn.net/weixin_43646672/article/details/89792532)

# 2. remove和remove_if
```C++
/* std::list容器的remove和remove_if */
int main()
{
    std::list<int> list_ = { 1, 100, 2, 3, 10, 1, 11, -1, 12 };
 
    list_.remove(1); // remove both elements equal to 1
    std::for_each(list_.begin(), list_.end(), [](auto & elem){
      std::cout << elem << " ";
    });
    std::cout << '\n';
    

    list_.remove_if([](int n){
      return n > 10; 
    }); // remove all elements greater than 10
    std::for_each(list_.begin(), list_.end(), [](auto & elem){
      std::cout << elem << " ";
    });
    std::cout << '\n';
}
```

# 3. splice函数
```C++ 
void splice(const_iterator pos, list &other); // (1)

void splice(const_iterator pos, list &other, const_iterator it); //(2)

void splice(const_iterator pos, list &other,
            const_iterator first, const_iterator last); // (3)
```
splice有三个函数原型：
必须包含两个参数，一个目标位置`pos` ，一个源数据的`list`链表

1)将`other`的所有元素转移到`pos`。

2)将`other`中`it`指向的节点转移到`pos`指向的位置。

3)将`other`中`[first, last]`范围内的节点转移到`pos`。