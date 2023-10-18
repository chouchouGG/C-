# Task #1 - 可扩展哈希表

## 参考资料：
[Extendible Hash Table 可扩展哈希详细理解](https://blog.csdn.net/MelroseLbt/article/details/129329316)
[Extendible Hashing (Dynamic approach to DBMS)](https://www.geeksforgeeks.org/extendible-hashing-dynamic-approach-to-dbms/)

***
# Task #2 - LRU-K替换策略

## 1. LRU
LRU —— 最近最久未使用
LFU —— 最近最少使用

| 题目 | 答案 |
| --- | --- |
| [leetcode-146. LRU 缓存](https://leetcode.cn/problems/lru-cache/) | [leetcode-146代码实现](.\leetcode-146_LRU_Cache.cpp) |
| [leetcode-460. LFU 缓存](https://leetcode.cn/problems/lfu-cache/) | [leetcode-460代码实现](.\leetcode-460_LFU_Cache.cpp) |

`LRU`用语言表述一句话就是：选择出最长时间不访问的页。

## 2. LRU-K
[LRU-K Replacement Policy详细理解](https://blog.csdn.net/MelroseLbt/article/details/130175678)

`LRU-K`不做任何实际的数据操作，仅仅是记录`frame`的状态，并根据一定的策略将可以驱逐的`frame id`号提供给系统。

1. 先使用C++标准库中的`std::list` + `std::unordered_map` 来实现...
2. ...再考虑手动造一个双向链表。

### 重点在于数据结构的设计：
类似于LRU，LFU缓存的设计，需要定义特殊的`LRUKNode`节点数据结构。

关键词：
**时间戳** 
**k-distence**

## 3. 参考资料：
[LRU、LFU和FIFO辨析](https://www.jianshu.com/p/b0ecc4fdb747)
***

# Task #3 - 