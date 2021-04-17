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
#include <heap/heap.h>
#include <heap/heap.ipp>
#include <random>

namespace heap::test {

  TEST(Heap, root_only) {
    std::vector<int> v{1};
    heap::Heap<int> h{v.begin(), v.end()};
    EXPECT_EQ(h.top(), 1);
  }

  TEST(Heap, max_from_10_and_pop) {
    std::vector<int> v{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    heap::Heap<int> h{std::less<int>(), std::move(v)};
    for (int i = 10; i > 0; --i) {
      EXPECT_EQ(h.top(), i);
      h.pop();
    }
    EXPECT_TRUE(h.empty());
  }

  TEST(Heap, heapsort_decreasing) {
    std::vector<int> v{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    heap::Heap<int, std::vector<int>, std::greater<>>::sort(v.begin(), v.end(), std::greater<>());
    EXPECT_THAT(v, ::testing::ElementsAre(10, 9, 8, 7, 6, 5, 4, 3, 2, 1));
  }

  TEST(Heap, heapsort_increasing) {
    std::vector<int> v{10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
    heap::Heap<int>::sort(v.begin(), v.end(), std::less<int>());
    EXPECT_THAT(v, ::testing::ElementsAre(1, 2, 3, 4, 5, 6, 7, 8, 9, 10));
  }

  TEST(Heap, heapsort_permutation_1) {
    std::vector<int> v{8, 5, 4, 6, 9, 10, 3, 7, 1, 2};
    std::vector<int> vsorted{8, 5, 4, 6, 9, 10, 3, 7, 1, 2};
    std::sort(vsorted.begin(), vsorted.end());
    heap::Heap<int>::sort(v.begin(), v.end(), std::less<int>());
    EXPECT_EQ(v, vsorted);
  }

  TEST(Heap, heapsort_permutation_2) {
    std::vector<int> v{0, 1, -1, 1, -4, -1, 9, 4, 3, -1};
    std::vector<int> vsorted{0, 1, -1, 1, -4, -1, 9, 4, 3, -1};
    std::sort(vsorted.begin(), vsorted.end());
    heap::Heap<int>::sort(v.begin(), v.end(), std::less<int>());
    EXPECT_EQ(v, vsorted);
  }

  TEST(Heap, heapsort_random) {
    const int seed = 42;
    std::default_random_engine g(seed);
    std::uniform_int_distribution<int> distribution(-5, 10);

    for (int j = 0; j < 10; ++j) {
      int n = 10;
      std::vector<int> vhs(n);
      for (int i = 0; i < n; ++i)
        vhs[i] = distribution(g);
      std::vector<int> vstdsort(vhs);
      std::sort(vstdsort.begin(), vstdsort.end());
      heap::Heap<int>::sort(vhs.begin(), vhs.end(), std::less<int>());
      EXPECT_EQ(vhs, vstdsort);
    }
  }

}
