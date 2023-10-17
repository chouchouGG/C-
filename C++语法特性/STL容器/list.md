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