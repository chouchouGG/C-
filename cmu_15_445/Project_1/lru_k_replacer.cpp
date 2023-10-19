//===----------------------------------------------------------------------===//
//
//                         BusTub
//
// lru_k_replacer.cpp
//
// Identification: src/buffer/lru_k_replacer.cpp
//
// Copyright (c) 2015-2022, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

/* ==================================================================
 * ------------------------------------------------------------------
 * 代码只是截取自己实现的部分，没有包含其他头文件，所以打开该开关后会有报错
 * ------------------------------------------------------------------
 * ==================================================================
 */
#if 0

#include "buffer/lru_k_replacer.h"

/* ==============================================================
 * LRUKReplacer 是一个独立的类，它与任何其他的 Replacer 类都没有关系。
 * 只需要实现LRU-K替换策略。不必实现LRU或时钟替换策略，即使有相应的文件。
 * ==============================================================
 */
namespace bustub
{

  LRUKReplacer::LRUKReplacer(size_t num_frames, size_t k) : replacer_size_(num_frames), k_(k) {}

  auto LRUKReplacer::Evict(frame_id_t *frame_id) -> bool
  {
    std::scoped_lock<std::mutex> lock(this->latch_);
    /* A frame with less than k historical references is given +inf as its backward k-distance.
     * 这句话意思是：访问次数小于K的frame的向后k-距离被认为是正无穷大（+infinity）*/
    /* When multipe frames have +inf backward k-distance, the replacer evicts the frame with the earliest timestamp.
     * 这句话意思是：当暂存队列中有多个frame时，要选择时间戳最小的那个frame驱逐，即temp_list_应该采用FIFO替换策略*/

    // 如果没有缓存任何frame，立即返回false
    if (this->cache_map_.size() == 0U)
      return false;
    // 如果temp_list_不为空，就先从temp_list_驱逐
    if (this->temp_list_.size() != 0U)
    {
      for (auto iter = temp_list_.rbegin(); iter != temp_list_.rend(); ++iter)
      {
        if (iter->is_evictable == false)
          continue;
        auto id = iter->fid_;
        this->temp_list_.erase(this->cache_map_[id]); // 从暂存队列删除frame信息（可能其他博客称“历史链表”，都是一个意思）
        this->cache_map_.erase(id);                   // 从哈希索引表中删除
        *frame_id = id;
        --curr_size_; // 不要忘记将可逐出frame数量减一
        return true;
      }
    }
    // cache_map_不为空有缓存，且temp_list_为空，则从cache_list_驱逐
    frame_id_t candidate;
    size_t min_Kth_timestamp = -1;       // -1赋值给无符号整数，则min_Kth_timestamp是当前类型所能表示的最大整数
    for (auto &node : this->cache_list_) // 遍历cache_list_，寻找可驱逐的且k-distance最大的frame，即为候选的待删除frame
    {
      if (node.is_evictable == false)
        continue;
      /* Backward k-distance is computed as the difference in time between current
       * timestamp and the timestamp of kth previous access.
       * 计算公式：向后的k-距离 = 当前时间戳 - 倒数第k次访问的时间戳
       * */
      // 实际计算时，由于当前时间戳是固定的，所以哪个frame的倒数第k次访问的时间戳最小，即向后的k-distance最大。
      auto val = node.timestamps_.back();
      if (val < min_Kth_timestamp)
      {
        candidate = node.fid_;
        min_Kth_timestamp = val;
      }
    }
    // 有可能cache_list_中的frame都被设置为不可驱逐，那么返回false
    if (min_Kth_timestamp == static_cast<size_t>(-1))
    {
      return false;
    }
    auto cur = this->cache_map_[candidate];
    this->cache_list_.erase(cur);
    this->cache_map_.erase(candidate);
    *frame_id = candidate;
    --curr_size_; // 不要忘记将可驱逐页的数量减一
    return true;
  }

  void LRUKReplacer::RecordAccess(frame_id_t frame_id)
  {
    std::scoped_lock<std::mutex> lock(this->latch_);

    // 如果frame id无效
    BUSTUB_ASSERT(frame_id < static_cast<frame_id_t>(replacer_size_), "frame id is invalid");

    if (cache_map_.find(frame_id) != cache_map_.end())
    { // 如果frame id已经存在
      auto cur = cache_map_[frame_id];
      // 修改时间戳
      cur->timestamps_.push_front(this->current_timestamp_); // 存入当前时间戳
      if (cur->k_ >= this->k_)
      {
        cur->timestamps_.pop_back(); // 时间戳只用保持最近的K次即可
      }
      // 修改访问次数
      ++cur->k_;
      if (cur->k_ == this->k_)
      { // 如果访问满K次
        /* std::list中splice函数原型之一：void splice( const_iterator pos, list& other, const_iterator it ); */
        cache_list_.splice(cache_list_.begin(), this->temp_list_, cur);
      }
    }
    else
    {
      // 第一次访问
      temp_list_.emplace_front(frame_id, this->current_timestamp_); // 调用emplace系列函数，直接进行构造
      cache_map_[frame_id] = temp_list_.begin();
    }

    this->current_timestamp_++;
    return;
  }

  void LRUKReplacer::SetEvictable(frame_id_t frame_id, bool set_evictable)
  {
    std::scoped_lock<std::mutex> lock(this->latch_);

    // 如果frame id无效
    BUSTUB_ASSERT(frame_id < static_cast<frame_id_t>(replacer_size_), "frame id is invalid");
    // 如果frame id不在缓存中
    if (cache_map_.find(frame_id) == cache_map_.end())
      return;

    auto cur = cache_map_[frame_id];
    if (cur->is_evictable != set_evictable)
    { // 如果可驱逐性和修改的可驱逐性不同，才进行处理
      cur->is_evictable = set_evictable;
      if (set_evictable)
        ++this->curr_size_;
      else
        --this->curr_size_;
    }
  }

  void LRUKReplacer::Remove(frame_id_t frame_id)
  {
    std::scoped_lock<std::mutex> lock(this->latch_);

    // 如果frame id无效
    BUSTUB_ASSERT(frame_id < static_cast<frame_id_t>(replacer_size_), "frame id is invalid");
    // 如果frame id不在缓存中
    if (cache_map_.find(frame_id) == cache_map_.end())
      return;
    auto cur = cache_map_[frame_id];
    // frame_id是不可驱逐
    BUSTUB_ASSERT(cur->is_evictable == true, "Remove is called on a non-evictable frame");
    if (cur->k_ < this->k_)
    {
      temp_list_.erase(cur);
    }
    else
    {
      cache_list_.erase(cur);
    }
    cache_map_.erase(frame_id);

    // This function should also decrement replacer's size if removal is successful。
    --this->curr_size_;
  }

  // 返回可驱逐frame的数量
  auto LRUKReplacer::Size() -> size_t
  {
    std::scoped_lock<std::mutex> lock(this->latch_);

    return this->curr_size_;
  }

} // namespace bustub

#endif
