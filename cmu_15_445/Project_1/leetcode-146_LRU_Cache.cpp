#include <unordered_map>
#include <memory>

class LRUCache
{
private:
  struct Node
  {
    int _key, _value;
    Node *_prev{nullptr};
    Node *_next{nullptr};

    Node() : _key(-1), _value(-1) {}
    Node(int k, int v) : _key(k), _value(v) {}
  };

private:
  void Move_To_Head(Node *p)
  {
    Remove(p);
    Add_To_Head(p);
  }

  void Remove(Node *p)
  {
    p->_prev->_next = p->_next;
    p->_next->_prev = p->_prev;
    --_sz;
  }

  void Add_To_Head(Node *p)
  {
    p->_next = _h->_next;
    p->_prev = _h;
    _h->_next->_prev = p;
    _h->_next = p;
    ++_sz;
  }

  auto Remove_From_Back() -> int
  {
    int key = _t->_prev->_key;
    Remove(_t->_prev);
    return key;
  }

  size_t _sz;    // size
  size_t _cp;    // capacity
  Node *_h, *_t; // 头节点head，尾节点tail
  std::unordered_map<int, std::shared_ptr<Node>> _cache_pool;

  bool isFull() { return _sz == _cp; }

public:
  LRUCache(int capacity) : _sz(0), _cp(capacity)
  {
    _h = new Node(-1, -1);
    _t = new Node(-1, -1);
    _h->_next = _t;
    _t->_prev = _h;
  }

  int get(int key)
  {
    if (_cache_pool.count(key) == 0U)
    {
      return -1;
    }
    auto sptr = _cache_pool[key];
    Move_To_Head(sptr.get());
    return sptr->_value;
  }

  void put(int key, int value)
  {
    // 如果key已经存在
    if (_cache_pool.count(key) != 0U)
    {
      auto sptr = _cache_pool[key];
      sptr->_value = value;
      Move_To_Head(sptr.get());
      return;
    }
    // key不存在时，先判断是否缓冲区已满
    if (isFull())
    {
      int key = Remove_From_Back();
      _cache_pool.erase(key);
    }
    auto sptr = std::make_shared<Node>(key, value);
    _cache_pool[key] = sptr;
    Add_To_Head(sptr.get());
  }
};

int main()
{
  LRUCache *lRUCache = new LRUCache(2);
  lRUCache->put(1, 1); // 缓存是 {1=1}
  lRUCache->put(2, 2); // 缓存是 {1=1, 2=2}
  lRUCache->get(1);    // 返回 1
  lRUCache->put(3, 3); // 该操作会使得关键字 2 作废，缓存是 {1=1, 3=3}
  lRUCache->get(2);    // 返回 -1 (未找到)
  lRUCache->put(4, 4); // 该操作会使得关键字 1 作废，缓存是 {4=4, 3=3}
  lRUCache->get(1);    // 返回 -1 (未找到)
  lRUCache->get(3);    // 返回 3
  lRUCache->get(4);    // 返回 4
}