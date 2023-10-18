#include <unordered_map>
#include <memory>
#include <iostream>

struct Node
{
  int _key, _value;
  size_t _freq;
  Node *_prev{nullptr};
  Node *_next{nullptr};

  Node() : _key(-1), _value(-1), _freq(0) {}
  Node(int k, int v) : _key(k), _value(v), _freq(1) {}
};

class DoubleLinkedList
{
private:
  Node *_h, *_t; // 头节点，尾节点

public:
  DoubleLinkedList()
  {
    _h = new Node();
    _t = new Node();
    _h->_next = _t;
    _t->_prev = _h;
  }

  ~DoubleLinkedList()
  {
    delete _h;
    delete _t;
  }

  void Remove(Node *p)
  {
    p->_prev->_next = p->_next;
    p->_next->_prev = p->_prev;
  }

  void Add_To_Head(Node *p)
  {
    p->_next = _h->_next;
    p->_prev = _h;
    _h->_next->_prev = p;
    _h->_next = p;
  }

  auto Remove_From_Back() -> int
  {
    int key = _t->_prev->_key;
    Remove(_t->_prev);
    return key;
  }

  // 除了头节点和尾节点外没有别的节点，即为空
  bool isEmpty() { return _h->_next == _t; }
};

class LFUCache
{
private:
  using access_count_t = int;
  using key_type_t = int;

private:
  size_t _sz;       // size
  size_t _cp;       // capacity
  size_t _min_freq; // 最小的访问频率
  std::unordered_map<key_type_t, std::shared_ptr<Node>> _key_table;
  std::unordered_map<access_count_t, std::shared_ptr<DoubleLinkedList>> _freqt_table;

  bool isFull() { return _sz == _cp; }

public:
  LFUCache(int capacity) : _sz(0), _cp(capacity), _min_freq(0) {}

  void Add_to(Node *cur, int idx)
  {
    Initialize(idx);
    _freqt_table[idx]->Add_To_Head(cur);
    ++_sz;
  }

  // 从当前的访问次数链表删除改节点，并将其添加到访问次数+1的链表中
  void Level_Up(Node *cur)
  {
    size_t &freq = cur->_freq;
    _freqt_table[freq]->Remove(cur); // 从当前链表删除

    Add_to(cur, freq + 1); // 添加到访问次数+1的链表中
    if (freq == _min_freq && _freqt_table[freq]->isEmpty())
    {
      ++_min_freq;
    }

    freq++;
    return;
  }

  void Initialize(int idx)
  {
    if (_freqt_table.count(idx) != 0U)
      return;
    _freqt_table[idx] = std::make_shared<DoubleLinkedList>();
  }

  int get(int key)
  {
    if (_key_table.count(key) == 0U)
    {
      return -1;
    }
    auto cur = _key_table[key];
    Level_Up(cur.get());
    return cur->_value;
  }

  void put(int key, int value)
  {
    // 如果key已经存在
    if (_key_table.count(key) != 0U)
    {
      auto cur = _key_table[key];
      cur->_value = value; // 更新value
      Level_Up(cur.get());
      return;
    }

    // 如果缓存池已满
    if (isFull())
    {
      auto cur = _freqt_table[_min_freq];
      auto key = cur->Remove_From_Back();
      if (cur->isEmpty())
      {
        _freqt_table.erase(_min_freq);
      }

      _key_table.erase(key);
      --_sz;
    }

    // 更新_cache_table
    auto cur = std::make_shared<Node>(key, value);
    _key_table[key] = cur;

    // 更新_freqt_table
    Add_to(cur.get(), 1);
    _min_freq = 1;
    return;
  }
};

// int main(int argc, char const *argv[])
// {
//   LFUCache *lfu = new LFUCache(2);
//   lfu->put(1, 1); // cache=[1,_], cnt(1)=1
//   lfu->put(2, 2); // cache=[2,1], cnt(2)=1, cnt(1)=1
//   lfu->get(1);    // 返回 1
//                   // cache=[1,2], cnt(2)=1, cnt(1)=2
//   lfu->put(3, 3); // 去除键 2 ，因为 cnt(2)=1 ，使用计数最小
//                   // cache=[3,1], cnt(3)=1, cnt(1)=2
//   lfu->get(2);    // 返回 -1（未找到）
//   lfu->get(3);    // 返回 3
//                   // cache=[3,1], cnt(3)=2, cnt(1)=2
//   lfu->put(4, 4); // 去除键 1 ，1 和 3 的 cnt 相同，但 1 最久未使用
//                   // cache=[4,3], cnt(4)=1, cnt(3)=2
//   std::cout << "#3\n";
//   lfu->get(1); // 返回 -1（未找到）
//   lfu->get(3); // 返回 3
//                // cache=[3,4], cnt(4)=1, cnt(3)=3
//   lfu->get(4); // 返回 4
//                // cache=[3,4], cnt(4)=2, cnt(3)=3

//   return 0;
// }
