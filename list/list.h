/*
MIT License

Copyright (c) 2020 Guilherme Simoes Schlinker

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
#ifndef LIST__LIST_H
#define LIST__LIST_H

#include <deque>

namespace list {

  namespace detail {

    enum class ListLinkType {
      Single,
      Double
    };

    template<typename T, ListLinkType type>
    struct ListNode;
  }

  template<
    typename T,
    detail::ListLinkType type = detail::ListLinkType::Single,
    typename NodeType = detail::ListNode<T, type>>
  class List {
   public:
    using value_type = T;
    List();
    List(List const &other);
    List(List &&other);

    template<typename U = T>
    void emplace_front(U &&value);

    template<typename U = T>
    void emplace_back(U &&value);

    [[nodiscard]] T &front();
    [[nodiscard]] T &back();

    [[nodiscard]] const T &front() const;
    [[nodiscard]] const T &back() const;

    void pop_front();
    void pop_back();

    [[nodiscard]] size_t size() const;

    [[nodiscard]] bool empty() const;

    template<bool is_const = true>
    class base_iterator {
      using list_type = std::conditional_t<is_const, List const, List>;
      using node_type = std::conditional_t<is_const, NodeType const, NodeType>;
     public:
      using iterator_category = std::conditional_t<type == detail::ListLinkType::Single,
                                                   std::forward_iterator_tag, std::bidirectional_iterator_tag>;
      using value_type = std::conditional_t<is_const, T const, T>;
      using difference_type = std::ptrdiff_t;
      using pointer = std::conditional_t<is_const, T const *, T *>;
      using reference = std::conditional_t<is_const, T const &, T &>;

      inline base_iterator(base_iterator<false> const &src) : base_iterator{src.current()} {}

      explicit base_iterator(list_type *list);
      explicit base_iterator(NodeType *node);
      explicit base_iterator();
      base_iterator &operator++();
      base_iterator operator++(int);
      base_iterator &operator--() requires (type == detail::ListLinkType::Double);
      base_iterator operator--(int) requires (type == detail::ListLinkType::Double);
      [[nodiscard]] bool operator==(const base_iterator &other) const;
      [[nodiscard]] bool operator!=(const base_iterator &other) const;
      [[nodiscard]] reference operator*();

     protected:
      NodeType *current() const { return current_; }
      friend class base_iterator<!is_const>;
      friend class List;

     private:
      NodeType *current_;
    };

    using iterator = base_iterator<false>;
    using const_iterator = base_iterator<true>;

    [[nodiscard]] inline iterator begin() { return iterator(this); }
    [[nodiscard]] inline iterator end() { return iterator(); }

    [[nodiscard]] inline const_iterator begin() const { return const_iterator(this); }
    [[nodiscard]] inline const_iterator end() const { return const_iterator(); }

    void reverse();

   private:
    NodeType *head_;
    int size_;

    NodeType *tail() const;
  };

  template<typename T>
  using DoubleList = List<T, detail::ListLinkType::Double, detail::ListNode<T, detail::ListLinkType::Double>>;

}
#endif