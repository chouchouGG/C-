//===----------------------------------------------------------------------===//
//
//                         BusTub
//
// extendible_hash_table.cpp
//
// Identification: src/container/hash/extendible_hash_table.cpp
//
// Copyright (c) 2022, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

/* ==================================================================
 * ------------------------------------------------------------------
 * 代码只是截取自己实现的部分，没有包含其他头文件，所以打开该开关后会有报错
 * ------------------------------------------------------------------
 * ==================================================================
 */
#if 0

#include <cassert>
#include <cstdlib>
#include <functional>
#include <list>
#include <utility>

#include "container/hash/extendible_hash_table.h"
#include "storage/page/page.h"

#include <algorithm> //添加

namespace bustub
{

  template <typename K, typename V>
  ExtendibleHashTable<K, V>::ExtendibleHashTable(size_t bucket_size)
      : global_depth_(0), bucket_size_(bucket_size), num_buckets_(1)
  {
    dir_.push_back(std::make_shared<Bucket>(bucket_size, 0));
  }

  template <typename K, typename V>
  auto ExtendibleHashTable<K, V>::IndexOf(const K &key) -> size_t
  {
    int mask = (1 << global_depth_) - 1;
    return std::hash<K>()(key) & mask;
  }

  template <typename K, typename V>
  auto ExtendibleHashTable<K, V>::GetGlobalDepth() const -> int
  {
    std::scoped_lock<std::mutex> lock(latch_);
    return GetGlobalDepthInternal();
  }

  template <typename K, typename V>
  auto ExtendibleHashTable<K, V>::GetGlobalDepthInternal() const -> int
  {
    return global_depth_;
  }

  template <typename K, typename V>
  auto ExtendibleHashTable<K, V>::GetLocalDepth(int dir_index) const -> int
  {
    std::scoped_lock<std::mutex> lock(latch_);
    return GetLocalDepthInternal(dir_index);
  }

  template <typename K, typename V>
  auto ExtendibleHashTable<K, V>::GetLocalDepthInternal(int dir_index) const -> int
  {
    return dir_[dir_index]->GetDepth();
  }

  template <typename K, typename V>
  auto ExtendibleHashTable<K, V>::GetNumBuckets() const -> int
  {
    std::scoped_lock<std::mutex> lock(latch_);
    return GetNumBucketsInternal();
  }

  template <typename K, typename V>
  auto ExtendibleHashTable<K, V>::GetNumBucketsInternal() const -> int
  {
    return num_buckets_;
  }
  /* ---------------------------------------------------------------------
   * 以上的代码已经代我们提前实现好了.
   * ---------------------------------------------------------------------
   */

  template <typename K, typename V>
  auto ExtendibleHashTable<K, V>::Find(const K &key, V &value) -> bool
  {
    std::scoped_lock<std::mutex> lock(latch_); // 上锁

    auto idx = IndexOf(key);
    auto old_bkt = this->dir_[idx];
    return old_bkt->Find(key, value);
  }

  template <typename K, typename V>
  auto ExtendibleHashTable<K, V>::Remove(const K &key) -> bool
  {
    std::scoped_lock<std::mutex> lock(latch_); // 上锁

    auto idx = IndexOf(key);
    auto old_bkt = this->dir_[idx];
    return old_bkt->Remove(key);
  }

  /**
   * @brief `ExtendibleHashTable::Insert` 的实现相较来说复杂一些
   */
  template <typename K, typename V>
  void ExtendibleHashTable<K, V>::Insert(const K &key, const V &value)
  {
    std::scoped_lock<std::mutex> lock(latch_); // 上锁

    // 无论是目录翻倍扩容后，还是桶分裂后，key映射到的桶都有可能仍然是满的，如果满就需要不断调整...
    /* 1.
     * 全局深度等于局部深度时，会自增全局深度（同时目录扩容），导致key所映射到的目录项发生变化（通过IndexOf求出key映射到的目录项）。
     * 2. 桶分裂后，会使得目录项所指向的桶发生变化 */
    /* 所以，每一轮while循环都需要重新计算：1. key映射的目录项、2.目录项指向的桶是否满（即while循环的条件） */
    while (this->dir_[IndexOf(key)]->IsFull())
    {
      auto idx = IndexOf(key);
      auto old_bkt = this->dir_[idx];

      /* 考虑到多线程情况，if语句中只能调用GetGlobalDepthInternal，如果调用GetGlobalDepth，会造成死锁。GetLocalDepthInternal同理
       */
      if (this->GetGlobalDepthInternal() == old_bkt->GetDepth())
      { // 方法一
        // if (this->GetGlobalDepthInternal() == this->GetLocalDepthInternal(idx)) { // 方法二

        this->global_depth_++; // 全局深度自增1
        size_t old_sz = dir_.size();
        dir_.resize(old_sz << 1); // 目录数量double

        // 将新增目录项指向已经存在的桶
        for (size_t i = 0; i < old_sz; ++i)
        {
          dir_[i + old_sz] = dir_[i]; // 扩容后低位和扩容前的低位是相同的
        }
      }

      // 添加新桶，局部深度自增1。(因为原来的桶满了，需要增加新的位来区分key)
      auto new_bkt = std::make_shared<Bucket>(bucket_size_, old_bkt->GetDepth() + 1);
      old_bkt->IncrementDepth();

      // 将需要分裂的桶里的key-value重新进行映射，
      int mask = 1 << (old_bkt->GetDepth() - 1);
      auto beg = old_bkt->GetItems().begin();
      auto end = old_bkt->GetItems().end();
      while (beg != end)
      {
        auto hash_val = std::hash<K>()(beg->first);
        auto nxt = std::next(beg);
        if (hash_val & mask)
        {
          new_bkt->Insert(beg->first, beg->second);
          old_bkt->Remove(beg->first);
        }
        beg = nxt;
      }

      // 桶的数量自增1
      ++num_buckets_;

      // 将相应目录项指向新增的桶new_bkt
      for (size_t i = 0; i < dir_.size(); ++i)
      {
        if ((dir_[i] == old_bkt) && (i & mask))
        {
          dir_[i] = new_bkt;
        }
      }

    } // while循环结束

    // 将(key, value)重新映射并插入桶中
    dir_[IndexOf(key)]->Insert(key, value);
    return;
  }

  //===--------------------------------------------------------------------===//
  // Bucket
  //===--------------------------------------------------------------------===//
  template <typename K, typename V>
  ExtendibleHashTable<K, V>::Bucket::Bucket(size_t array_size, int depth) : size_(array_size), depth_(depth) {}

  /**
   * @brief 调用 `Bucket::Find` 时证明已经确定 `The index of key` 映射到当前的桶上，
   * 所以只需要顺序查找 `key` 值是否匹配
   */
  template <typename K, typename V>
  auto ExtendibleHashTable<K, V>::Bucket::Find(const K &key, V &value) -> bool
  {
    return std::any_of(list_.begin(), list_.end(), [&key, &value](const auto &pair_elem) -> bool
                       {
    if (pair_elem.first == key) {
      value = pair_elem.second;
      return true;
    }
    return false; });
  }

  template <typename K, typename V>
  auto ExtendibleHashTable<K, V>::Bucket::Remove(const K &key) -> bool
  {
    return std::any_of(list_.begin(), list_.end(), [&key, this](const auto &pair_elem) -> bool
                       {
    if (pair_elem.first == key) {
      this->list_.remove(pair_elem);
      return true;
    }
    return false; });
  }

  /**
   * @brief `Bucket::Insert`并不需要考虑桶满时的扩容并重新映射的问题，
   * 这些工作理应交给`ExtendibleHashTable`来做。
   */
  template <typename K, typename V>
  auto ExtendibleHashTable<K, V>::Bucket::Insert(const K &key, const V &value) -> bool
  {
    /* 遍历list_，如果key已经存在，则只需要更新value，并返回true */
    if (std::any_of(list_.begin(), list_.end(), [&key, &value](auto &pair_elem) -> bool
                    {
        if (pair_elem.first == key) {
          pair_elem.second = value;
          return true;
        }
        return false; }))
    {
      return true;
    }
    else if (this->IsFull())
    {
      return false;
    }
    else
    {
      list_.emplace_back(key, value); // 调用emplace_back可以原地构造
      return true;
    }
  }

  template class ExtendibleHashTable<page_id_t, Page *>;
  template class ExtendibleHashTable<Page *, std::list<Page *>::iterator>;
  template class ExtendibleHashTable<int, int>;
  // test purpose
  template class ExtendibleHashTable<int, std::string>;
  template class ExtendibleHashTable<int, std::list<int>::iterator>;

} // namespace bustub

#endif
