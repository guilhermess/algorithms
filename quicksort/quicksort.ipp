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
#include <random>

namespace quicksort {

  template<typename RandomAccessIterator, typename Compare>
  std::tuple<RandomAccessIterator, RandomAccessIterator> partition(RandomAccessIterator first,
                                                                   RandomAccessIterator last,
                                                                   Compare comp) {

    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution(0, std::distance(first, last) - 1);
    auto r = first + distribution(generator);
    auto l = first - 1;
    auto e = first - 1;
    auto pivot = last - 1;
    iter_swap(pivot, r);
    for (auto g = first; std::distance(g, last) > 1; ++g) {
      if (comp(*g, *pivot)) {
        ++l;
        iter_swap(g, l);
        ++e;
        if (!comp(*g, *pivot) && !comp(*pivot, *g))
          iter_swap(g, e);
      } else if (!comp(*g, *pivot) && !comp(*pivot, *g)) {
        ++e;
        iter_swap(g, e);
      }
    }
    ++e;
    iter_swap(pivot, e);
    return std::make_tuple(l, e);
  }

  template<typename RandomAccessIterator, typename Compare>
  void sort(RandomAccessIterator first, RandomAccessIterator last, Compare comp) {
    if (std::distance(first, last) > 1) {
      auto[l, e] = quicksort::partition(first, last, comp);
      quicksort::sort(first, l + 1, comp);
      quicksort::sort(e + 1, last, comp);
    }
  }

}
