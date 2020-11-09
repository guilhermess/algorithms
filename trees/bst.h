//
// Created by Guilherme Schlinker on 11/1/20.
//

#ifndef ALGORITHMS_TREES_BST_HXX_
#define ALGORITHMS_TREES_BST_HXX_

#include <memory>
#include <stack>

namespace trees {

namespace detail {

template<typename T>
class BSTNode {
 public:
  inline explicit BSTNode(T &&data,
                          BSTNode *parent = nullptr,
                          std::unique_ptr<BSTNode> left = nullptr,
                          std::unique_ptr<BSTNode> right = nullptr) : data_{std::forward<T>(data)},
                                                                      parent_{parent},
                                                                      left_{std::move(left)},
                                                                      right_{std::move(right)} {
  }

  inline T const &data() const { return data_; }
  inline T &data() { return data_; }

  inline BSTNode *left() const { return left_.get(); }
  inline BSTNode *right() const { return right_.get(); }
  inline BSTNode *parent() const { return parent_; }

  inline std::unique_ptr<BSTNode> &&left_move() { return std::move(left_); }
  inline std::unique_ptr<BSTNode> &&right_move() { return std::move(right_); }

  inline BSTNode * leftmost() {
    BSTNode * res = this;
    while (res->left()) {
      res = res->left();
    }
    return res;
  }

  inline BSTNode *rightmost() {
    BSTNode * res = this;
    while (res->right()) {
      res = res->right();
    }
    return res;
  }

  inline void left(std::unique_ptr<BSTNode> &&node) {
    left_ = std::move(node);
    left_->parent(this);
  }
  inline void right(std::unique_ptr<BSTNode> &&node) {
    right_ = std::move(node);
    right_->parent(this);
  }

  inline void parent(BSTNode *node) {
    parent_ = node;
  }

  inline BSTNode *add_left(T &&data) {
    left_ = std::make_unique<BSTNode>(std::forward<T>(data), this);
    return left_.get();
  }

  inline BSTNode *add_right(T &&data) {
    right_ = std::make_unique<BSTNode>(std::forward<T>(data), this);
    return right_.get();
  }

  [[nodiscard]] inline bool child_is_left(BSTNode const *child) const { return left_.get() == child; }
  [[nodiscard]] inline bool child_is_right(BSTNode const *child) const { return right_.get() == child; }

 private:
  T data_;
  BSTNode *parent_;
  std::unique_ptr<BSTNode> left_;
  std::unique_ptr<BSTNode> right_;
};

}

template<typename T, typename Comparator = std::less<T>, typename NodeType = detail::BSTNode<T>>
class BST {
 public:
  using value_type = T;

  inline BST(Comparator comp = Comparator()) : comp_{comp}, size_{0} {}
  inline virtual ~BST() {}

  template<bool is_const = true>
  class base_iterator {
    using tree_type = std::conditional_t<is_const, BST const, BST>;
    using node_type = std::conditional_t<is_const, NodeType const, NodeType>;
   public:
    using base_iterator_category = std::bidirectional_iterator_tag;
    using value_type = std::conditional_t<is_const, T const, T>;
    using difference_type = std::ptrdiff_t;
    using pointer = std::conditional_t<is_const, T const *, T *>;
    using reference = std::conditional_t<is_const, T const &, T &>;

    template<typename = std::enable_if_t<is_const>>
    base_iterator(const base_iterator<false> &src) : base_iterator{src.current()} {}
    explicit base_iterator(tree_type *bst);
    explicit base_iterator(NodeType *node);
    explicit base_iterator();
    base_iterator(base_iterator const &src);
    base_iterator &operator++();
    base_iterator operator++(int);
    base_iterator &operator--();
    base_iterator operator--(int);
    [[nodiscard]] bool operator==(const base_iterator &other) const;
    [[nodiscard]] bool operator!=(const base_iterator &other) const;
    [[nodiscard]] reference operator*();

   protected:
    NodeType *current() const { return current_; }
    friend class base_iterator<!is_const>;
    friend class BST;

   private:
    NodeType *current_;
    value_type dummy_;

    [[nodiscard]] NodeType *down_rightmost(NodeType *node) const;
    [[nodiscard]] NodeType *down_leftmost(NodeType *node) const;
    [[nodiscard]] NodeType *up_first_left(NodeType *node) const;
    [[nodiscard]] NodeType *up_first_right(NodeType *node) const;

    [[nodiscard]] bool child_is_left(NodeType const *parent, NodeType const *child) const;
    [[nodiscard]] bool child_is_right(NodeType const *parent, NodeType const *child) const;
  };

  using iterator = base_iterator<false>;
  using const_iterator = base_iterator<true>;

  [[nodiscard]] inline iterator begin() { return iterator(this); }
  [[nodiscard]] inline iterator end() { return iterator(); }

  [[nodiscard]] inline const_iterator begin() const { return const_iterator(this); }
  [[nodiscard]] inline const_iterator end() const { return const_iterator(); }

  virtual std::pair<iterator, bool> insert(value_type &&value);
  virtual iterator erase(const_iterator position);
  std::size_t erase(value_type const &value);
  iterator erase(const_iterator first, const_iterator last);

  [[nodiscard]] inline std::size_t size() const { return size_; }

  [[nodiscard]] const_iterator find(value_type const &value) const;
  [[nodiscard]] iterator find(value_type const &value);

 protected:
  template<bool is_const> friend
  class base_iterator;
  inline NodeType *root() const { return root_.get(); };

  std::pair<iterator, bool> insert(value_type &&value, NodeType *node);

 private:
  Comparator comp_;
  std::size_t size_;
  std::unique_ptr<NodeType> root_;

  [[nodiscard]] NodeType *find_node(value_type const &value) const;
  [[nodiscard]] NodeType *find_node(value_type const &value, NodeType *node) const;

};

}
#endif //ALGORITHMS_TREES_BST_HXX_
