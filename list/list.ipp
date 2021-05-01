/*
MIT License

Copyright (c) 2021 Guilherme Simoes Schlinker

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <list/list.h>

namespace list {

  namespace detail {
    template<typename T>
    struct ListNode<T, ListLinkType::Single> {

      template <typename U>
      ListNode(U &&d, ListNode<T, ListLinkType::Single> *n) : data{std::forward<U>(d)}, next{n}
      {}

      T data;
      ListNode<T, ListLinkType::Single> *next;
    };

    template<typename T>
    struct ListNode<T, ListLinkType::Double> {
      template <typename U>
      ListNode(U &&d,
               ListNode<T, ListLinkType::Double> *n,
               ListNode<T, ListLinkType::Double> *p) : data{std::forward<U>(d)}, next{n}, prev{p}
      {}

      T data;
      ListNode<T, ListLinkType::Double> *next;
      ListNode<T, ListLinkType::Double> *prev;
    };
  }

  template<typename T, detail::ListLinkType type, typename NodeType>
  List<T, type, NodeType>::List() : head_{nullptr}, size_{0} {
  }

  //TODO implement copy constructor: deep copy nodes
  template<typename T, detail::ListLinkType type, typename NodeType>
  List<T, type, NodeType>::List(List const &other) {
  }

  template<typename T, detail::ListLinkType type, typename NodeType>
  List<T, type, NodeType>::List(List &&other) : head_{std::move(other.head_)}, size_{std::move(other.size_)} {
  }

  template<typename T, detail::ListLinkType type, typename NodeType>
  template<typename U>
  void List<T, type, NodeType>::emplace_front(U &&value) {
    if constexpr (type == detail::ListLinkType::Single) {
      NodeType *node{new NodeType(std::forward<T>(value), head_)};
      head_ = node;
    } else {
      NodeType *node{new NodeType(std::forward<T>(value), head_, nullptr)};
      if (head_) head_->prev = node;
      head_ = node;
    }
    ++size_;
  }

  template<typename T, detail::ListLinkType type, typename NodeType>
  template<typename U>
  void List<T, type, NodeType>::emplace_back(U &&value) {
    NodeType **noderef = &head_;
    NodeType *nodeprev = nullptr;
    while (*noderef) {
      nodeprev = *noderef;
      noderef = &(*noderef)->next;
    }
    if constexpr (type == detail::ListLinkType::Single) {
      NodeType *newnode{new NodeType(std::forward<T>(value), nullptr)};
      *noderef = newnode;
    } else {
      NodeType *newnode{new NodeType(std::forward<T>(value), nullptr, nodeprev)};
      *noderef = newnode;
      if (nodeprev)
        nodeprev->next = newnode;
    }
    ++size_;
  }

  template<typename T, detail::ListLinkType type, typename NodeType>
  T &List<T, type, NodeType>::front() {
    return head_->data;
  }

  template<typename T, detail::ListLinkType type, typename NodeType>
  T &List<T, type, NodeType>::back() {
    NodeType *node = head_;
    while(node && node->next)
      node = node->next;
    return node->data;
  }

  template<typename T, detail::ListLinkType type, typename NodeType>
  const T &List<T, type, NodeType>::front() const {
    return head_->data;
  }

  template<typename T, detail::ListLinkType type, typename NodeType>
  const T &List<T, type, NodeType>::back() const {
    NodeType *node = head_;
    while(node && node->next)
      node = node->next;
    return node->data;
  }

  template<typename T, detail::ListLinkType type, typename NodeType>
  void List<T, type, NodeType>::pop_front() {
    auto node = head_;
    head_ = node->next;
    if constexpr (type == detail::ListLinkType::Double) {
      if (head_) head_->prev = nullptr;
    }
    --size_;
    delete node;
  }

  template<typename T, detail::ListLinkType type, typename NodeType>
  void List<T, type, NodeType>::pop_back() {
    NodeType **noderef = &head_;
    while (*noderef && (*noderef)->next) {
      noderef = &(*noderef)->next;
    }
    delete *noderef;
    *noderef = nullptr;
    --size_;
  }

  template<typename T, detail::ListLinkType type, typename NodeType>
  NodeType *List<T, type, NodeType>::tail() const {
    NodeType *node = head_;
    while (node->next)
      node = node->next;
    return node;
  }

  template<typename T, detail::ListLinkType type, typename NodeType>
  size_t List<T, type, NodeType>::size() const {
    return size_;
  }

  template<typename T, detail::ListLinkType type, typename NodeType>
  bool List<T, type, NodeType>::empty() const {
    return size() == 0;
  }

  template<typename T, detail::ListLinkType type, typename NodeType>
  void List<T, type, NodeType>::reverse() {
    if constexpr (type == detail::ListLinkType::Single) {
      NodeType *p = nullptr;
      auto node = head_;
      while (node) {
        auto n = node->next;
        node->next = p;
        p = node;
        node = n;
      }
      head_ = p;
    } else {
      auto node = head_;
      NodeType *prev = nullptr;
      while (node) {
        prev = node;
        std::swap(node->prev, node->next);
        node = node->prev;
      }
      head_ = prev;
    }
  }

  template<typename T, detail::ListLinkType type, typename NodeType>
  template<bool is_const>
  List<T, type, NodeType>::base_iterator<is_const>::base_iterator(list_type *list) : current_(list->head_) {
  }

  template<typename T, detail::ListLinkType type, typename NodeType>
  template<bool is_const>
  List<T, type, NodeType>::base_iterator<is_const>::base_iterator(NodeType *node) : current_{node} {}

  template<typename T, detail::ListLinkType type, typename NodeType>
  template<bool is_const>
  List<T, type, NodeType>::base_iterator<is_const>::base_iterator() : current_{nullptr} {}

  template<typename T, detail::ListLinkType type, typename NodeType>
  template<bool is_const>
  typename List<T, type, NodeType>::template base_iterator<is_const> &
  List<T, type, NodeType>::base_iterator<is_const>::operator++() {
    current_ = current_->next;
    return *this;
  }

  template<typename T, detail::ListLinkType type, typename NodeType>
  template<bool is_const>
  typename List<T, type, NodeType>::template base_iterator<is_const>
  List<T, type, NodeType>::base_iterator<is_const>::operator++(int) {
    base_iterator <is_const> retval = *this;
    ++(*this);
    return retval;
  }

  template<typename T, detail::ListLinkType type, typename NodeType>
  template<bool is_const>
  typename List<T, type, NodeType>::template base_iterator<is_const> &
  List<T, type, NodeType>::base_iterator<is_const>::operator--() requires (type == detail::ListLinkType::Double) {
    current_ = current_->prev;
    return *this;
  }

  template<typename T, detail::ListLinkType type, typename NodeType>
  template<bool is_const>
  typename List<T, type, NodeType>::template base_iterator<is_const>
  List<T, type, NodeType>::base_iterator<is_const>::operator--(int) requires (type == detail::ListLinkType::Double) {
    base_iterator <is_const> retval = *this;
    --(*this);
    return retval;
  }

  template<typename T, detail::ListLinkType type, typename NodeType>
  template<bool is_const>
  bool
  List<T, type, NodeType>::base_iterator<is_const>::operator==(const base_iterator &other) const {
    return this->current_ == other.current_;
  }

  template<typename T, detail::ListLinkType type, typename NodeType>
  template<bool is_const>
  bool
  List<T, type, NodeType>::base_iterator<is_const>::operator!=(const base_iterator &other) const {
    return this->current_ != other.current_;
  }

  template<typename T, detail::ListLinkType type, typename NodeType>
  template<bool is_const>
  typename List<T, type, NodeType>::template base_iterator<is_const>::reference
  List<T, type, NodeType>::base_iterator<is_const>::operator*() {
    return current_->data;
  }

}