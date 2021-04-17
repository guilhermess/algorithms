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

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <quicksort/quicksort.h>
#include <quicksort/quicksort.ipp>
#include <chrono>
#include <vector>

#include <heap/heap.h>
#include <heap/heap.ipp>

using std::vector;
using namespace ::testing;

namespace quicksort::test {

  template<typename Iter, typename Compare>
  int run_and_clock(Iter first,
                    Iter last,
                    Compare comp,
                    void func(Iter, Iter, Compare)) {
    auto start = std::chrono::high_resolution_clock::now();
    func(first, last, comp);
    auto finish = std::chrono::high_resolution_clock::now();
    int microseconds = std::chrono::duration_cast<std::chrono::microseconds>(finish - start).count();
    return microseconds;
  }

  TEST(quicksort, sort_simple) {
    vector<int> v{10, 9, 8, 7, 6, 1, 4, 3, 2, 5};
    quicksort::sort(v.begin(), v.end());
    EXPECT_THAT(v, ElementsAre(1, 2, 3, 4, 5, 6, 7, 8, 9, 10));
  }

  TEST(quicksort, benchmark_vs_std_sort) {
    const int seed = 42;
    std::default_random_engine g(seed);
    std::uniform_int_distribution<int> distribution(-1000, 1000);

    for (auto &n : {10, 100, 1000, 10000, 100000, 1000000}) {
      std::vector<int> vqs(n);
      for (int i = 0; i < n; ++i) {
        vqs[i] = distribution(g);
      }
      std::vector<int> vstdsort(vqs);
      std::vector<int> vheap(vqs);
      auto duration_qs = run_and_clock(vqs.begin(),
                                       vqs.end(),
                                       std::less<int>(),
                                       quicksort::sort);
      auto duration_stdsort = run_and_clock(vstdsort.begin(),
                                            vstdsort.end(),
                                            std::less<int>(),
                                            std::sort);
      auto duration_heapsort = run_and_clock(vheap.begin(),
                                             vheap.end(),
                                             std::less<int>(),
                                             heap::Heap<int>::sort);
      EXPECT_EQ(vqs, vstdsort);
      EXPECT_EQ(vheap, vstdsort);
      std::cout << "Quicksort: " << duration_qs << "us Standard Sort: " << duration_stdsort << "us Heapsort: "
                << duration_heapsort << "us" << std::endl;
    }

  }
}