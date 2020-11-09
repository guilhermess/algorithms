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

/*template<typename T, typename Comparator>
std::pair<typename AVLTree<T, Comparator>::iterator, bool>
AVLTree<T, Comparator>::insert(value_type &&value) {
  return std::make_pair(this->end(), false);

}

template<typename T, typename Comparator>
typename AVLTree<T, Comparator>::iterator
AVLTree<T, Comparator>::erase(const_iterator position) {
  return this->end();

}*/

}
#endif