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
#ifndef HEAP__HEAP_H
#define HEAP__HEAP_H

#include <vector>

namespace heap {

  template<
    typename T,
    typename Container = std::vector<T>,
    typename Compare = std::less<typename Container::value_type>>
  class Heap {
   public:
    using size_type = typename Container::size_type;
    using const_reference = typename Container::const_reference;

    Heap();

    explicit Heap(Compare const &compare);

    Heap(Compare const &compare, Container const &cont);

    Heap(Compare const &compare, Container &&cont);

    Heap(Heap const &other);

    Heap(Heap &&other);

    template<class InputIt>
    Heap(InputIt first, InputIt last, Compare const &compare, Container const &cont);

    template<class InputIt>
    Heap(InputIt first,
         InputIt last,
         Compare const &compare = Compare(),
         Container &&cont = Container());

    const_reference top() const;
    void pop();
    size_type size() const;
    bool empty() const;

    template<typename RandomAccessIterator>
    static void sort(RandomAccessIterator first, RandomAccessIterator last, Compare comp);

    template<typename RandomAccessIterator>
    static void make_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp);

   private:

    template<typename RandomAccessIterator>
    static void heapify(RandomAccessIterator i,
                        RandomAccessIterator first,
                        RandomAccessIterator last,
                        Compare comp);

    template<typename RandomAccessIterator>
    static constexpr RandomAccessIterator left(RandomAccessIterator first, RandomAccessIterator i);
    template<typename RandomAccessIterator>
    static constexpr RandomAccessIterator right(RandomAccessIterator first, RandomAccessIterator i);
    template<typename RandomAccessIterator>
    static constexpr RandomAccessIterator parent(RandomAccessIterator first, RandomAccessIterator i);

    Compare compare_;
    Container container_;
    size_type size_;
  };

}  // namespace heap
#endif