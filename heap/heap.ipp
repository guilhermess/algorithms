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

#include <heap/heap.h>
#include <cmath>

namespace heap {

  template<typename T, typename Container, typename Compare>
  Heap<T, Container, Compare>::Heap() : size_{0} {
  }

  template<typename T, typename Container, typename Compare>
  Heap<T, Container, Compare>::Heap(Compare const &compare) : compare_{compare}, size_{0} {}

  template<typename T, typename Container, typename Compare>
  Heap<T, Container, Compare>::Heap(Compare const &compare, Container const &cont)
    : compare_{compare}, container_{cont}, size_{container_.size()} {
    make_heap(container_.begin(), container_.end(), compare_);
  }

  template<typename T, typename Container, typename Compare>
  Heap<T, Container, Compare>::Heap(Compare const &compare, Container &&cont)
    : compare_{compare}, container_{std::move(cont)}, size_{container_.size()} {
    make_heap(container_.begin(), container_.end(), compare_);
  }

  template<typename T, typename Container, typename Compare>
  Heap<T, Container, Compare>::Heap(Heap const &other)
    : compare_{other.compare_}, container_{other.container_}, size_{other.size_} {
  }

  template<typename T, typename Container, typename Compare>
  Heap<T, Container, Compare>::Heap(Heap &&other)
    : compare_{std::move(other.compare_)},
      container_{std::move(other.container_)},
      size_{other.size_} {
  }

  template<typename T, typename Container, typename Compare>
  template<class InputIt>
  Heap<T, Container, Compare>::Heap(InputIt first,
                                    InputIt last,
                                    Compare const &compare,
                                    Container const &cont)
    : compare_{compare}, container_{cont}, size_{container_.size() + std::distance(first, last)} {
    container_.insert(container_.end(), first, last);
    make_heap(container_.begin(), container_.end(), compare_);
  }

  template<typename T, typename Container, typename Compare>
  template<class InputIt>
  Heap<T, Container, Compare>::Heap(InputIt first,
                                    InputIt last,
                                    Compare const &compare,
                                    Container &&cont)
    : compare_{compare},
      container_{std::move(cont)},
      size_{container_.size() + std::distance(first, last)} {
    container_.insert(container_.end(), first, last);
    make_heap(container_.begin(), container_.end(), compare_);
  }

  template<typename T, typename Container, typename Compare>
  template<typename RandomAccessIterator>
  void Heap<T, Container, Compare>::make_heap(RandomAccessIterator first,
                                              RandomAccessIterator last,
                                              Compare comp) {
    for (auto i = first + std::floor(std::distance(first, last) / 2) - 1; i >= first; --i) {
      heapify(i, first, last, comp);
    }
  }

  template<typename T, typename Container, typename Compare>
  template<typename RandomAccessIterator>
  void Heap<T, Container, Compare>::heapify(RandomAccessIterator i,
                                            RandomAccessIterator first,
                                            RandomAccessIterator last,
                                            Compare comp) {
    if (std::distance(i, last) <= 0)
      return;
    auto l = left(first, i);
    auto r = right(first, i);
    auto best = i;
    if (std::distance(l, last) > 0 && comp(*i, *l))
      best = l;
    if (std::distance(r, last) > 0 && comp(*best, *r))
      best = r;
    if (best != i) {
      std::swap(*best, *i);
      heapify(best, first, last, comp);
    }
  }

  template<typename T, typename Container, typename Compare>
  template<typename RandomAccessIterator>
  constexpr RandomAccessIterator Heap<T, Container, Compare>::left(RandomAccessIterator first,
                                                                   RandomAccessIterator i) {
    return first + 2 * std::distance(first, i) + 1;
  }
  template<typename T, typename Container, typename Compare>
  template<typename RandomAccessIterator>
  constexpr RandomAccessIterator Heap<T, Container, Compare>::right(RandomAccessIterator first,
                                                                    RandomAccessIterator i) {
    return first + 2 * (std::distance(first, i) + 1);
  }

  template<typename T, typename Container, typename Compare>
  template<typename RandomAccessIterator>
  constexpr RandomAccessIterator Heap<T, Container, Compare>::parent(RandomAccessIterator first,
                                                                     RandomAccessIterator i) {
    return (std::distance(first, i) > 0) ? first + std::floor((std::distance(first, i) - 1) / 2)
                                         : first;
  }

  template<typename T, typename Container, typename Compare>
  typename Heap<T, Container, Compare>::const_reference Heap<T, Container, Compare>::top() const {
    return container_.front();
  }

  template<typename T, typename Container, typename Compare>
  typename Heap<T, Container, Compare>::size_type Heap<T, Container, Compare>::size() const {
    return size_;
  }

  template<typename T, typename Container, typename Compare>
  bool Heap<T, Container, Compare>::empty() const {
    return size() == 0;
  }

  template<typename T, typename Container, typename Compare>
  void Heap<T, Container, Compare>::pop() {
    if (size_ > 0) {
      container_[0] = std::move(container_[size_ - 1]);
      --size_;
      heapify(container_.begin(), container_.begin(), container_.begin() + size_, compare_);
    }
  }

  template<typename T, typename Container, typename Compare>
  template<typename RandomAccessIterator>
  void Heap<T, Container, Compare>::sort(RandomAccessIterator first,
                                         RandomAccessIterator last,
                                         Compare comp) {
    make_heap(first, last, comp);
    auto size = std::distance(first, last);
    auto sentinel = last - 1;
    while (std::distance(first, sentinel) > 0) {
      std::swap(*first, *sentinel);
      heapify(first, first, sentinel, comp);
      --sentinel;
    }
  }

}