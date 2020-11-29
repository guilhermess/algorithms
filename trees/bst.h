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
struct BSTNodeTraits;

template<typename T, typename NodeTraits = trees::detail::BSTNodeTraits<T>>
class BSTNode {
  using NodeType = typename NodeTraits::NodeType;
 public:
  inline explicit BSTNode(T &&data,
                          NodeType *parent = nullptr,
                          std::unique_ptr<NodeType> left = nullptr,
                          std::unique_ptr<NodeType> right = nullptr) : data_{std::forward<T>(data)},
                                                                       parent_{parent},
                                                                       left_{std::move(left)},
                                                                       right_{std::move(right)} {
  }

  inline BSTNode(BSTNode const &src,
                 NodeType *parent)
      : data_{src.data_},
        parent_{parent},
        left_{src.left() ? std::make_unique<NodeType>(*src.left(), get_this()) : nullptr},
        right_{src.right() ? std::make_unique<NodeType>(*src.right(), get_this()) : nullptr} {}

  virtual ~BSTNode() {}

  inline T const &data() const { return data_; }
  inline T &data() { return data_; }
  inline void data(T &&data) { data_ = std::move(data); }
  inline T &&data_move() { return std::move(data_); }

  inline NodeType *left() const { return left_.get(); }
  inline NodeType *right() const { return right_.get(); }
  inline NodeType *parent() const { return parent_; }

  inline std::unique_ptr<NodeType> &&left_move() { return std::move(left_); }
  inline std::unique_ptr<NodeType> &&right_move() { return std::move(right_); }

  inline void replace_child(NodeType *child, std::unique_ptr<NodeType> new_child) {
    if (this->child_is_left(child))
      left(new_child);
    else if (this->child_is_right(child))
      right(new_child);
  }

  inline std::unique_ptr<NodeType> &&child_move(NodeType *child) {
    if (this->child_is_left(child)) {
      return left_move();
    } else if (this->child_is_right(child))
      return right_move();
    assert(0);
  }

  inline NodeType *leftmost() {
    NodeType *res = get_this();
    while (res->left()) {
      res = res->left();
    }
    return res;
  }

  inline NodeType *rightmost() {
    NodeType *res = get_this();
    while (res->right()) {
      res = res->right();
    }
    return res;
  }

  inline void left(std::unique_ptr<NodeType> node) {
    left_ = std::move(node);
    if (left_)
      left_->parent(get_this());
  }
  inline void right(std::unique_ptr<NodeType> node) {
    right_ = std::move(node);
    if (right_)
      right_->parent(get_this());
  }

  inline void parent(NodeType *node) {
    parent_ = node;
  }

  inline NodeType *add_left(T &&data) {
    left_ = std::make_unique<NodeType>(std::forward<T>(data), get_this());
    return left_.get();
  }

  inline NodeType *add_right(T &&data) {
    right_ = std::make_unique<NodeType>(std::forward<T>(data), get_this());
    return right_.get();
  }

  [[nodiscard]] inline bool child_is_left(NodeType const *child) const {
    return left_.get() == child;
  }
  [[nodiscard]] inline bool child_is_right(NodeType const *child) const {
    return right_.get() == child;
  }

  [[nodiscard]] inline std::size_t height() const {
    std::size_t loffset = 0;
    if (this->left())
      loffset = 1 + this->left()->height();
    std::size_t roffset = 0;
    if (this->right())
      roffset = 1 + this->right()->height();
    return std::max(loffset, roffset);
  }

  [[nodiscard]] inline NodeType *child(int offset) const {
    return (offset == -1) ? left() : (offset == 1) ? right() : nullptr;
  }

  [[nodiscard]] inline virtual bool operator==(NodeType const &other) const {
    bool left_match = compare(left(), other.left());
    if (!left_match) return false;
    bool right_match = compare(right(), other.right());
    if (!right_match) return false;
    return data() == other.data();
  }

 private:
  T data_;
  NodeType *parent_;
  std::unique_ptr<NodeType> left_;
  std::unique_ptr<NodeType> right_;

  NodeType *get_this() {
    return dynamic_cast<NodeType *>(this);
  }

  [[nodiscard]] inline bool compare(NodeType const *a, NodeType const *b) const {
    bool match;
    if (!a && !b) match = true;
    else if (!a || !b) match = false;
    else match = *a == *b;
    return match;
  }

};

template<typename T>
struct BSTNodeTraits {
  using NodeType = typename trees::detail::BSTNode<T, BSTNodeTraits>;

};

}

template<typename T, typename Comparator = std::less<T>, typename NodeType = detail::BSTNode<T>>
class BST {
 public:
  using value_type = T;

  inline explicit BST(Comparator comp = Comparator()) : comp_{comp}, size_{0} {}
  inline BST(BST &&src) noexcept: comp_{src.comp_}, size_{src.size_}, root_{std::move(src.root_)} {}
  inline BST(BST const &src) : comp_{src.comp_},
                               size_{src.size_},
                               root_{(src.root()) ? std::make_unique<NodeType>(*src.root(), nullptr)
                                                  : nullptr} {}
  inline virtual ~BST() = default;

  template<bool is_const = true>
  class base_iterator {
    using tree_type = std::conditional_t<is_const, BST const, BST>;
    using node_type = std::conditional_t<is_const, NodeType const, NodeType>;
   public:
    using iterator_category = std::bidirectional_iterator_tag;
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

  virtual std::pair<iterator, bool> insert(value_type value);

  virtual iterator erase(const_iterator position);
  std::size_t erase(value_type const &value);
  iterator erase(const_iterator first, const_iterator last);

  [[nodiscard]] inline std::size_t size() const { return size_; }
  [[nodiscard]] std::size_t height() const;
  [[nodiscard]] std::size_t level(const_iterator position) const;
  [[nodiscard]] std::size_t level(value_type const &value) const;
  [[nodiscard]] const_iterator find(value_type const &value) const;
  [[nodiscard]] iterator find(value_type const &value);

  [[nodiscard]] inline bool operator==(BST const &other) const {
    if (!root() && !other.root()) return true;
    if (!root() || !other.root()) return false;
    return *root() == *other.root();
  }

 protected:
  template<bool is_const> friend
  class base_iterator;
  inline NodeType *root() const { return root_.get(); };
  inline NodeType *current(const const_iterator &itr) const { return itr.current(); }
  inline void root(std::unique_ptr<NodeType> root) { root_ = std::move(root); }
  inline void size(std::size_t newsize) { size_ = newsize; }

  std::unique_ptr<NodeType> move_node_and_replace(NodeType *node,
                                                  std::unique_ptr<NodeType> replacement);
  std::pair<iterator, bool> insert(value_type &&value, NodeType *node);

 private:
  Comparator comp_;
  std::size_t size_;
  std::unique_ptr<NodeType> root_;

  [[nodiscard]] NodeType *find_node(value_type const &value) const;
  [[nodiscard]] NodeType *find_node(value_type const &value, NodeType *node) const;
  [[nodiscard]] std::size_t height(NodeType *node) const;

};

}
#endif //ALGORITHMS_TREES_BST_HXX_
