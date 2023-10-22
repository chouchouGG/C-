//===----------------------------------------------------------------------===//
//
//                         BusTub
//
// lru_k_replacer.h
//
// Identification: src/include/buffer/lru_k_replacer.h
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

#pragma once

#include <limits>
#include <list>
#include <mutex> // NOLINT
#include <unordered_map>
#include <vector>

#include "common/config.h"
#include "common/macros.h"

#include <algorithm>
#include <memory>
#include <utility>

namespace bustub
{
 // 自己添加的LRU-K节点类
  struct LRUKNode
  {
    std::list<size_t> timestamps_; // 历史时间戳的记录
    bool is_evictable{false};      // 是否可驱逐的标志位
    frame_id_t fid_;               // 缓存的frame号，用于删除时反向索引，删掉LRUKReplacer中的记录
    size_t k_{1};

    LRUKNode(frame_id_t id, size_t timestamp) : fid_(id) { this->timestamps_.emplace_front(timestamp); }
  };

  /**
   * LRUKReplacer implements the LRU-k replacement policy.
   *
   * The LRU-k algorithm evicts a frame whose backward k-distance is maximum
   * of all frames. Backward k-distance is computed as the difference in time between
   * current timestamp and the timestamp of kth previous access.
   *
   * A frame with less than k historical references is given
   * +inf as its backward k-distance. When multiple frames have +inf backward k-distance,
   * classical LRU algorithm is used to choose victim.
   */
  class LRUKReplacer
  {
  public:
    /**
     *
     * TODO(P1): Add implementation
     *
     * @brief a new LRUKReplacer.
     * @param num_frames the maximum number of frames the LRUReplacer will be required to store
     */
    explicit LRUKReplacer(size_t num_frames, size_t k);

    DISALLOW_COPY_AND_MOVE(LRUKReplacer); // 禁止复制语义和移动语义

    /**
     * TODO(P1): Add implementation
     *
     * @brief Destroys the LRUReplacer.
     */
    ~LRUKReplacer() = default;

    /**
     * TODO(P1): Add implementation
     *
     * @brief Find the frame with largest backward k-distance and evict that frame. Only frames
     * that are marked as 'evictable' are candidates for eviction.
     *
     * A frame with less than k historical references is given +inf as its backward k-distance.
     * If multiple frames have inf backward k-distance, then evict the frame with the earliest
     * timestamp overall.
     *
     * Successful eviction of a frame should decrement the size of replacer and remove the frame's
     * access history.
     *
     * @param[out] frame_id id of frame that is evicted.
     * @return true if a frame is evicted successfully, false if no frames can be evicted.
     */
    auto Evict(frame_id_t *frame_id) -> bool;

    /**
     * TODO(P1): Add implementation
     *
     * @brief Record the event that the given frame id is accessed at current timestamp.
     * Create a new entry for access history if frame id has not been seen before.
     *
     * If frame id is invalid (ie. larger than replacer_size_), throw an exception. You can
     * also use BUSTUB_ASSERT to abort the process if frame id is invalid.
     *
     * @param frame_id id of frame that received a new access.
     */
    void RecordAccess(frame_id_t frame_id);

    /**
     * TODO(P1): Add implementation
     *
     * @brief Toggle whether a frame is evictable or non-evictable. This function also
     * controls replacer's size. Note that size is equal to number of evictable entries.
     *
     * If a frame was previously evictable and is to be set to non-evictable, then size should
     * decrement. If a frame was previously non-evictable and is to be set to evictable,
     * then size should increment.
     *
     * If frame id is invalid, throw an exception or abort the process.
     *
     * For other scenarios, this function should terminate without modifying anything.
     *
     * @param frame_id id of frame whose 'evictable' status will be modified
     * @param set_evictable whether the given frame is evictable or not
     */
    void SetEvictable(frame_id_t frame_id, bool set_evictable);

    /**
     * TODO(P1): Add implementation
     *
     * @brief Remove an evictable frame from replacer, along with its access history.
     * This function should also decrement replacer's size if removal is successful.
     *
     * Note that this is different from evicting a frame, which always remove the frame
     * with largest backward k-distance. This function removes specified frame id,
     * no matter what its backward k-distance is.
     *
     * If Remove is called on a non-evictable frame, throw an exception or abort the
     * process.
     *
     * If specified frame is not found, directly return from this function.
     *
     * @param frame_id id of frame to be removed
     */
    void Remove(frame_id_t frame_id);

    /**
     * TODO(P1): Add implementation
     *
     * @brief Return replacer's size, which tracks the number of evictable frames.
     *
     * @return size_t
     */
    auto Size() -> size_t;

  private:
    using LRUKNode_Ptr_t = std::list<LRUKNode>::iterator; // std::list中迭代器的类型

    // TODO(student): implement me! You can replace these member variables as you like.
    // Remove maybe_unused if you start using them.
    size_t current_timestamp_{0}; // 当前的时间戳,每进行一次record操作加一。(并不是真正的时间戳)
    size_t curr_size_{0};         // 当前存放的可驱逐页面数量
    size_t replacer_size_;        // 整个缓冲池的容量（包括被pin住的页）
    size_t k_;                    // k_是LRU-K里的K
    std::mutex latch_;            // 用于维护多线程的锁

    // 用链表的主要原因是：暂存和缓存队列经常进行添加、删除操作
    std::list<LRUKNode> temp_list_;  // 暂存列表
    std::list<LRUKNode> cache_list_; // 缓存列表

    // 可以在O(1)时间复杂度内，根据frame id索引到一个LRUKNode节点
    std::unordered_map<frame_id_t, LRUKNode_Ptr_t> cache_map_;
  };

} // namespace bustub

#endif
