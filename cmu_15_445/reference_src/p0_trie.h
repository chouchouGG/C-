//===----------------------------------------------------------------------===//
//
//                         BusTub
//
// p0_trie.h
//
// Identification: src/include/primer/p0_trie.h
//
// Copyright (c) 2015-2022, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

#pragma once

#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "common/exception.h"
#include "common/rwlatch.h"

namespace bustub
{

  class TrieNode
  {
  public:
    explicit TrieNode(char key_char)
    {
      is_end_ = false;
      key_char_ = key_char;
    }

    TrieNode(TrieNode &&other_trie_node) noexcept
    {
      children_ = std::move(other_trie_node.children_);
      is_end_ = other_trie_node.is_end_;
      key_char_ = other_trie_node.key_char_;
    }

    virtual ~TrieNode() = default;
    auto HasChild(char key_char) const -> bool { return children_.count(key_char) == 1; }
    auto HasChildren() const -> bool { return !children_.empty(); }
    auto IsEndNode() const -> bool { return is_end_; }
    auto GetKeyChar() const -> char { return key_char_; }

    auto InsertChildNode(char key_char, std::unique_ptr<TrieNode> &&child) -> std::unique_ptr<TrieNode> *
    {
      if (children_.count(key_char) != 0U)
      {
        return nullptr;
      }
      if (child->key_char_ != key_char)
      {
        return nullptr;
      }
      children_[key_char] = std::move(child);
      return &children_[key_char];
    }

    auto GetChildNode(char key_char) -> std::unique_ptr<TrieNode> *
    {
      if (children_.count(key_char) == 0)
      {
        return nullptr;
      }
      return &children_[key_char];
    }

    void RemoveChildNode(char key_char)
    {
      if (children_.count(key_char) == 0)
      {
        return;
      }
      children_.erase(key_char);
    }

    void SetEndNode(bool is_end) { is_end_ = is_end; }

  protected:
    char key_char_;
    bool is_end_{false};
    std::unordered_map<char, std::unique_ptr<TrieNode>> children_;
  };

  /**
   * TrieNodeWithValue is a node that marks the ending of a key, and it can
   * hold a value of any type T.
   */
  template <typename T>
  class TrieNodeWithValue : public TrieNode
  {
  private:
    T value_;

  public:
    TrieNodeWithValue(TrieNode &&trieNode, T value) : TrieNode(std::move(trieNode))
    {
      value_ = value;
      is_end_ = true;
    }

    TrieNodeWithValue(char key_char, T value) : TrieNode(key_char)
    {
      value_ = value;
      is_end_ = true;
    }

    ~TrieNodeWithValue() override = default;
    auto GetValue() const -> T { return value_; }
  };

  /**
   * Trie is a concurrent key-value store. Each key is a string and its corresponding
   * value can be any type.
   */
  class Trie
  {
  private:
    /* Root node of the trie */
    std::unique_ptr<TrieNode> root_;
    /* Read-write lock for the trie */
    ReaderWriterLatch latch_;

  public:
    /**
     * TODO(P0): Add implementation
     *
     * @brief Construct a new Trie object. Initialize the root node with '\0'
     * character.
     */
    Trie() { root_ = std::make_unique<TrieNode>(TrieNode('\0')); }

    /**
     * TODO(P0): Add implementation
     *
     * @brief Insert key-value pair into the trie.
     *
     * If the key is an empty string, return false immediately.
     *
     * If the key already exists, return false. Duplicated keys are not allowed and
     * you should never overwrite value of an existing key.
     *
     * When you reach the ending character of a key:
     * 1. If TrieNode with this ending character does not exist, create new TrieNodeWithValue
     * and add it to parent node's children_ map.
     * 2. If the terminal node is a TrieNode, then convert it into TrieNodeWithValue by
     * invoking the appropriate constructor.
     * 3. If it is already a TrieNodeWithValue,
     * then insertion fails and returns false. Do not overwrite existing data with new data.
     *
     * You can quickly check whether a TrieNode pointer holds TrieNode or TrieNodeWithValue
     * by checking the is_end_ flag. If is_end_ == false, then it points to TrieNode. If
     * is_end_ == true, it points to TrieNodeWithValue.
     *
     * @param key Key used to traverse the trie and find the correct node
     * @param value Value to be inserted
     * @return True if insertion succeeds, false if the key already exists
     */
    template <typename T>
    auto Insert(const std::string &key, T value) -> bool
    {
      // latch_.WLock();
      std::unique_ptr<TrieNode> *parent;
      // if (key.empty())
      // {
      //   latch_.WUnlock();
      //   return false;
      // }
      auto p = &root_;
      for (auto key_char : key)
      {
        if (!p->get()->HasChild(key_char))
        {
          p->get()->InsertChildNode(key_char, std::make_unique<TrieNode>(key_char));
        }
        parent = p;
        p = p->get()->GetChildNode(key_char);
      }
      // if (p->get()->IsEndNode())
      // {
      //   latch_.WUnlock();
      //   return false;
      // }
      auto temp_node = std::move(*p);
      parent->get()->RemoveChildNode(key.back());
      auto temp = std::make_unique<TrieNodeWithValue<T>>(std::move(*temp_node), value);
      parent->get()->InsertChildNode(key.back(), std::move(temp));
      // latch_.WUnlock();
      return true;
    }

    auto Dfs(const std::string &key, std::unique_ptr<TrieNode> *root, size_t index, bool *success)
        -> std::unique_ptr<TrieNode> *
    {
      if (index == key.size())
      {
        if (!root->get()->IsEndNode())
        {
          *success = false;
          return nullptr;
        }
        if (!root->get()->HasChildren())
        {
          *success = true;
          return nullptr;
        }
        *success = true;
        root->get()->SetEndNode(false);
        return root;
      }
      std::unique_ptr<TrieNode> *node;
      if (root->get()->HasChild(key[index]))
      {
        node = Dfs(key, root->get()->GetChildNode(key[index]), index + 1, success);
        if (!*success)
        {
          return nullptr;
        }
        if (node == nullptr)
        {
          root->get()->RemoveChildNode(key[index]);
          if (!root->get()->HasChildren() && !root->get()->IsEndNode())
          {
            return nullptr;
          }
        }
        return root;
      }
      *success = false;
      return nullptr;
    }
    /**
     * TODO(P0): Add implementation
     *
     * @brief Remove key value pair from the trie.
     * This function should also remove nodes that are no longer part of another
     * key. If key is empty or not found, return false.
     *
     * You should:
     * 1) Find the terminal node for the given key.
     * 2) If this terminal node does not have any children, remove it from its
     * parent's children_ map.
     * 3) Recursively remove nodes that have no children and are not terminal node
     * of another key.
     *
     * @param key Key used to traverse the trie and find the correct node
     * @return True if the key exists and is removed, false otherwise
     */
    auto Remove(const std::string &key) -> bool
    {
      latch_.WLock();
      bool a = true;
      bool *success = &a;
      Dfs(key, &root_, 0, success);
      latch_.WUnlock();
      return *success;
    }

    /**
     * TODO(P0): Add implementation
     *
     * @brief Get the corresponding value of type T given its key.
     * If key is empty, set success to false.
     * If key does not exist in trie, set success to false.
     * If the given type T is not the same as the value type stored in TrieNodeWithValue
     * (ie. GetValue<int> is called but terminal node holds std::string),
     * set success to false.
     *
     * To check whether the two types are the same, dynamic_cast
     * the terminal TrieNode to TrieNodeWithValue<T>. If the casted result
     * is not nullptr, then type T is the correct type.
     *
     * @param key Key used to traverse the trie and find the correct node
     * @param success Whether GetValue is successful or not
     * @return Value of type T if type matches
     */
    template <typename T>
    auto GetValue(const std::string &key, bool *success) -> T
    {
      latch_.RLock();
      if (key.empty())
      {
        *success = false;
        latch_.RUnlock();
        return {};
      }
      auto p = &root_;
      for (auto key_char : key)
      {
        if (p->get()->HasChild(key_char))
        {
          p = p->get()->GetChildNode(key_char);
        }
        else
        {
          latch_.RUnlock();
          *success = false;
          return {};
        }
      }
      if (!p->get()->IsEndNode())
      {
        *success = false;
        latch_.RUnlock();
        return {};
      }
      auto res = dynamic_cast<TrieNodeWithValue<T> *>(p->get());
      if (res)
      {
        *success = true;
        latch_.RUnlock();
        return res->GetValue();
      }
      *success = false;
      latch_.RUnlock();
      return {};
    }
  };
} // namespace bustub
