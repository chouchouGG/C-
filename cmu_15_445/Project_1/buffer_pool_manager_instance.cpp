//===----------------------------------------------------------------------===//
//
//                         BusTub
//
// buffer_pool_manager_instance.cpp
//
// Identification: src/buffer/buffer_pool_manager.cpp
//
// Copyright (c) 2015-2021, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

/* ==================================================================
 * ------------------------------------------------------------------
 * 代码只是截取自己实现的部分，没有包含其他头文件，所以打开该开关后会有报错
 * ------------------------------------------------------------------
 * ==================================================================
 */
#if 0

#include "buffer/buffer_pool_manager_instance.h"

#include "common/exception.h"
#include "common/macros.h"

namespace bustub {

// 构造函数
BufferPoolManagerInstance::BufferPoolManagerInstance(size_t pool_size, DiskManager *disk_manager, size_t replacer_k,
                                                     LogManager *log_manager)
    : pool_size_(pool_size), disk_manager_(disk_manager), log_manager_(log_manager) {
  // we allocate a consecutive memory space for the buffer pool
  pages_ = new Page[pool_size_];
  page_table_ = new ExtendibleHashTable<page_id_t, frame_id_t>(bucket_size_);
  replacer_ = new LRUKReplacer(pool_size, replacer_k);

  // Initially, every page is in the free list.
  for (size_t i = 0; i < pool_size_; ++i) {
    free_list_.emplace_back(static_cast<int>(i));
  }

#if 0
  // TODO(students): remove this line after you have implemented the buffer pool manager
  throw NotImplementedException(
      "BufferPoolManager is not implemented yet. If you have finished implementing BPM, please remove the throw "
      "exception line in `buffer_pool_manager_instance.cpp`.");
#endif
}

BufferPoolManagerInstance::~BufferPoolManagerInstance() {
  delete[] pages_;
  delete page_table_;
  delete replacer_;
}

auto BufferPoolManagerInstance::NewPgImp(page_id_t *page_id) -> Page * {
  std::scoped_lock<std::mutex> lock(latch_);
  frame_id_t frame_id;
  if (free_list_.empty() && replacer_->Size() == 0) return nullptr;  //
  // call the AllocatePage() method to get a new page id
  *page_id = AllocatePage();
  if (!free_list_.empty()) {
    frame_id = free_list_.front();
    free_list_.pop_front();

    SetMetadata_UpdateTableAndCache(pages_[frame_id], *page_id, frame_id);
    // auto &page = pages_[frame_id];
    // page.pin_count_ = 1;
    // page.page_id_ = *page_id;
    // page_table_->Insert(*page_id, frame_id);
    // replacer_->RecordAccess(frame_id);
    // replacer_->SetEvictable(frame_id, false);

    // return &page;
    return &pages_[frame_id];
  }

  if (replacer_->Evict(&frame_id)) {
    auto &page = pages_[frame_id];  // 取到相应的frame

    if (page.IsDirty()) {
      disk_manager_->WritePage(page.GetPageId(),
                               page.GetData());  // WritePage的第一个参数应该是要写回的页面，而不是要取的页面page_id
      page.is_dirty_ = false;
    }
    page.ResetMemory();
    page_table_->Remove(page.GetPageId());  // 把page_id到frame_id的映射删除

    SetMetadata_UpdateTableAndCache(page, *page_id, frame_id);

    // disk_manager_->ReadPage(*page_id, page.GetData());  // New一个新页是不需要读数据的，因为此时磁盘上还没数据
    return &page;
  }

  return nullptr;
}

auto BufferPoolManagerInstance::FetchPgImp(page_id_t page_id) -> Page * {
  std::scoped_lock<std::mutex> lock(latch_);
  frame_id_t frame_id;
  // Find函数如果返回true，就会将page_id映射的值存到frame_id中
  // 如果page_id已经存在在内存里
  if (page_table_->Find(page_id, frame_id)) {
    auto &page = pages_[frame_id];  // 取到相应的frame
    ++page.pin_count_;
    replacer_->RecordAccess(frame_id);
    replacer_->SetEvictable(frame_id, false);
    return &page;
  }

  // 如果page_id不存在于内存，就需要分配一个frame
  if (!free_list_.empty()) {
    frame_id = free_list_.front();
    free_list_.pop_front();
    auto &page = pages_[frame_id];  // 取到相应的frame
    SetMetadata_UpdateTableAndCache(page, page_id, frame_id);
    disk_manager_->ReadPage(page_id, page.GetData());  // 从磁盘中读取页面
    return &page;
  }

  if (replacer_->Evict(&frame_id)) {
    auto &page = pages_[frame_id];  // 取到相应的frame
    if (page.IsDirty()) {
      disk_manager_->WritePage(page.GetPageId(),
                               page.GetData());  // WritePage的第一个参数应该是要写回的页面，而不是要取的页面page_id
      page.is_dirty_ = false;
    }
    page.ResetMemory();
    page_table_->Remove(page.GetPageId());  // 把page_id到frame_id的映射删除
    SetMetadata_UpdateTableAndCache(page, page_id, frame_id);
    disk_manager_->ReadPage(page_id, page.GetData());  // 从磁盘中读取页面
    return &page;
  }
  return nullptr;
}

auto BufferPoolManagerInstance::UnpinPgImp(page_id_t page_id, bool is_dirty) -> bool {
  std::scoped_lock<std::mutex> lock(latch_);
  frame_id_t frame_id;
  // page_id不在缓存池中，或page_id在缓存池中但pin_count_已经为0，返回false
  if (!page_table_->Find(page_id, frame_id) || pages_[frame_id].GetPinCount() == 0) {
    return false;
  }

  auto &page = pages_[frame_id];
  if (is_dirty) {
    page.is_dirty_ = is_dirty;
  }
  if (0 == --page.pin_count_) {
    replacer_->SetEvictable(frame_id, true);
  }

  return true;
}

auto BufferPoolManagerInstance::FlushPgImp(page_id_t page_id) -> bool {
  std::scoped_lock<std::mutex> lock(latch_);
  if (page_id == INVALID_PAGE_ID) {
    return false;
  }
  frame_id_t frame_id;
  if (!page_table_->Find(page_id, frame_id)) {
    return false;
  }
  auto &page = pages_[frame_id];
  disk_manager_->WritePage(page_id, page.GetData());
  page.is_dirty_ = false;
  return true;
}

void BufferPoolManagerInstance::FlushAllPgsImp() {
  std::scoped_lock<std::mutex> lock(latch_);
  for (size_t frame_id = 0; frame_id < pool_size_; ++frame_id) {
    FlushPgImp(pages_[frame_id].GetPageId());
  }
  return;
}

auto BufferPoolManagerInstance::DeletePgImp(page_id_t page_id) -> bool {
  std::scoped_lock<std::mutex> lock(latch_);
  frame_id_t frame_id;
  if (!page_table_->Find(page_id, frame_id)) {
    return true;
  }

  auto &page = pages_[frame_id];
  if (page.GetPinCount() > 0) {
    return false;
  }

  page_table_->Remove(page_id);
  replacer_->Remove(frame_id);
  free_list_.push_front(frame_id);

  page.is_dirty_ = false;
  page.ResetMemory();
  page.page_id_ = INVALID_PAGE_ID;
  page.pin_count_ = 0;

  DeallocatePage(page_id);
  return false;
}

auto BufferPoolManagerInstance::AllocatePage() -> page_id_t { return next_page_id_++; }

void BufferPoolManagerInstance::SetMetadata_UpdateTableAndCache(Page &page, page_id_t page_id, frame_id_t frame_id) {
  page.pin_count_ = 1;
  page.page_id_ = page_id;
  page_table_->Insert(page_id, frame_id);
  replacer_->RecordAccess(frame_id);
  replacer_->SetEvictable(frame_id, false);
  return;
}

}  // namespace bustub

#endif