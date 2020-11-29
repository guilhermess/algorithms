//
// Created by Guilherme Schlinker on 11/01/20.
//

#ifndef ALGORITHMS_TREES_AVL_AVLTREE_IPP_
#define  ALGORITHMS_TREES_AVL_AVLTREE_IPP_

#include <trees/avl/avl_tree.h>
#include <trees/bst.ipp>

namespace trees::avl {

template<typename T, typename Comparator>
AVLTree<T, Comparator>::AVLTree(Comparator const &comp)
    : BST<T, Comparator, detail::AVLNode<T>>
          (comp) {
}

template<typename T, typename Comparator>
std::pair<typename AVLTree<T, Comparator>::iterator, bool>
AVLTree<T, Comparator>::insert(value_type value) {
  auto[itr, success] = BST<T,
                           Comparator,
                           detail::AVLNode<T>>::insert(std::forward<value_type>(value));
  update_path_balance_insert(itr);
  return std::make_pair(itr, success);
}

template<typename T, typename Comparator>
typename AVLTree<T, Comparator>::iterator
AVLTree<T, Comparator>::erase(const_iterator position) {
  if (position == this->end())
    return this->end();
  auto node = this->current(position);
  auto parent = node->parent();
  if (node->left() && node->right()) {
    auto next = node->right()->leftmost();
    node->data(std::move(next->data_move()));
    erase(const_iterator(next));
    return iterator(node);
  } else if (node->left()) {
    auto prev = node->left();
    node->data(std::move(prev->data_move()));
    erase(const_iterator(prev));
    return ++iterator(node);
  } else if (node->right()) {
    auto next = node->right();
    node->data(std::move(next->data_move()));
    erase(const_iterator(next));
    return iterator(node);
  } else {
    auto child_offset = 0;
    auto itr = ++iterator(node);
    if (parent && parent->child_is_left(node)) {
      child_offset = -1;
      parent->left(nullptr);
    } else if (parent && parent->child_is_right(node)) {
      parent->right(nullptr);
      child_offset = 1;
    } else {
      this->root(nullptr);
    }
    this->size(this->size() - 1);
    update_path_balance_erase(const_iterator(parent), child_offset);
    return itr;
  }
  return this->end();
}

template<typename T, typename Comparator>
std::pair<bool, typename AVLTree<T, Comparator>::NodeType*>
AVLTree<T, Comparator>::update_path_instance_erase(NodeType *curr,
                                                   NodeType *child) {
  if (curr->balance() == 2 && child->balance() >= 0) {
    auto child_balance = child->balance();
    rotate_left(curr, child);
    child->balance(-1 + child_balance);
    curr->balance(1 - child_balance);
    assert(curr->check_balance());
    assert(child->check_balance());
    return std::make_pair(curr->balance() > 0, child);
  } else if (curr->balance() == -2 && child->balance() <= 0) {
    auto node_balance = child->balance();
    rotate_right(curr, child);
    child->balance(1 + node_balance);
    curr->balance(-1 - node_balance);
    assert(curr->check_balance());
    assert(child->check_balance());
    return std::make_pair(curr->balance() < 0, child);
  } else if (curr->balance() == 2 && child->balance() == -1) {
    auto child_left_balance = child->left()->balance();
    auto child_left = child->left();
    rotate_right(child, child_left);
    child_left->balance(0);
    auto curr_right = curr->right();
    rotate_left(curr, curr_right);
    curr_right->balance(0);
    if (child_left_balance > 0) {
      curr->balance(-1);
      child->balance(0);
    } else {
      if (child_left_balance == 0) {
        curr->balance(0);
        child->balance(0);
      } else {
        curr->balance(0);
        child->balance(1);
      }
    }
    assert(curr->check_balance());
    assert(child->check_balance());
    return std::make_pair(false, child_left);
  } else if (curr->balance() == -2 && child->balance() == 1) {
    auto child_right_balance = child->right()->balance();
    auto child_right = child->right();
    rotate_left(child, child_right);
    child_right->balance(0);
    auto curr_left = curr->left();
    rotate_right(curr, curr_left);
    curr_left->balance(0);
    if (child_right_balance < 0) {
      curr->balance(1);
      child->balance(0);
    } else {
      if (child_right_balance == 0) {
        curr->balance(0);
        child->balance(0);
      } else {
        curr->balance(0);
        child->balance(-1);
      }
    }
    assert(curr->check_balance());
    assert(child->check_balance());
    return std::make_pair(false, child_right);
  }
  return std::make_pair(false, curr);
}

template<typename T, typename Comparator>
void AVLTree<T, Comparator>::update_path_balance_insert(const_iterator position) {
  if (position == this->end())
    return;
  auto curr = this->current(position);
  auto child_offset = 0; // left = -1, center = 0, right = 1
  while (curr) {
    curr->balance(curr->balance() + child_offset);
    if (curr->balance() == 0 && child_offset != 0)
      break;
    auto child = curr->child(child_offset);
    if (curr->balance() == 2 && child->balance() == 1) {
      rotate_left(curr, child);
      curr->balance(0);
      child->balance(0);
      assert(curr->check_balance());
      assert(child->check_balance());
      break;
    } else if (curr->balance() == -2 && child->balance() == -1) {
      rotate_right(curr, child);
      curr->balance(0);
      child->balance(0);
      assert(curr->check_balance());
      assert(child->check_balance());
      break;
    } else if (curr->balance() == 2 && child->balance() == -1) {
      auto balance_child_left = child->left()->balance();
      auto child_left = child->left();
      rotate_right(child, child_left);
      child_left->balance(0);
      auto curr_right = curr->right();
      rotate_left(curr, curr_right);
      curr_right->balance(0);
      if (balance_child_left > 0) {
        curr->balance(-1);
        child->balance(0);
      } else {
        if (balance_child_left == 0) {
          curr->balance(0);
          child->balance(0);
        } else {
          curr->balance(0);
          child->balance(1);
        }
      }
      assert(curr->check_balance());
      assert(child->check_balance());
      break;
    } else if (curr->balance() == -2 && child->balance() == 1) {
      auto balance_child_right = child->right()->balance();
      auto child_right = child->right();
      rotate_left(child, child_right);
      child_right->balance(0);
      auto curr_left = curr->left();
      rotate_right(curr, curr_left);
      curr_left->balance(0);
      if (balance_child_right < 0) {
        curr->balance(1);
        child->balance(0);
      } else {
        if (balance_child_right == 0) {
          curr->balance(0);
          child->balance(0);
        } else {
          curr->balance(0);
          child->balance(-1);
        }
      }
      assert(curr->check_balance());
      assert(child->check_balance());
      break;
    }
    if (curr->parent())
      child_offset = (curr->parent()->child_is_left(curr)) ? -1 : 1;
    curr = curr->parent();
  }
}

template<typename T, typename Comparator>
void AVLTree<T, Comparator>::update_path_balance_erase(const_iterator position, int child_offset) {
  if (position == this->end())
    return;
  auto curr = this->current(position);
  while (curr) {
    auto other = curr->child(-1 * child_offset);
    bool stop = (curr->balance() == 0);
    curr->balance(curr->balance() - child_offset);
    if (stop)
      break;
    auto [new_stop, new_curr] = update_path_instance_erase(curr, other);
    if (new_stop)
      break;
    curr = new_curr;
    if (curr->parent())
      child_offset = (curr->parent()->child_is_left(curr)) ? -1 : 1;
    curr = curr->parent();
  }
}

template<typename T, typename Comparator>
typename AVLTree<T, Comparator>::NodeType *AVLTree<T, Comparator>::rotate_left(NodeType *subroot,
                                                                               NodeType *right) {
  std::unique_ptr<NodeType> subroot_right = subroot->right_move();
  subroot->right(right->left_move());
  std::unique_ptr<NodeType>
      subroot_move = this->move_node_and_replace(subroot, std::move(subroot_right));
  right->left(std::move(subroot_move));
  return right;
}

template<typename T, typename Comparator>
typename AVLTree<T, Comparator>::NodeType *AVLTree<T, Comparator>::rotate_right(NodeType *subroot,
                                                                                NodeType *left) {
  std::unique_ptr<NodeType> subroot_left = subroot->left_move();
  subroot->left(left->right_move());
  std::unique_ptr<NodeType>
      subroot_move = this->move_node_and_replace(subroot, std::move(subroot_left));
  left->right(std::move(subroot_move));
  return left;
}

template<typename T, typename Comparator>
char AVLTree<T, Comparator>::balance(const_iterator position) {
  if (position == this->end())
    return -3;
  return this->current(position)->balance();
}

template<typename T, typename Comparator>
char AVLTree<T, Comparator>::balance(value_type const &value) {
  return balance(this->find(value));
}
}
#endif