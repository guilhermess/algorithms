
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

#include <gtest/gtest.h>
#include <max_subarray/max_subarray.h>

#include <max_subarray/max_subarray.ipp>
#include <vector>
#include <chrono>
#include <random>

namespace max_subarray::test {

  template<typename Iter, typename T = typename Iter::value_type>
  std::tuple<T, int> run_and_clock(Iter first,
                                   Iter last,
                                   std::tuple<Iter, Iter, T> func(Iter, Iter)) {
    auto start = std::chrono::high_resolution_clock::now();
    auto[l, r, b] = func(first, last);
    auto finish = std::chrono::high_resolution_clock::now();
    int microseconds = std::chrono::duration_cast<std::chrono::microseconds>(finish - start).count();
    return std::make_tuple(b, microseconds);
  }

  TEST(max_subarray_brute_force, simple) {
    std::vector<int> array{13, -3, -25, 20, -3, -16, -23, 18,
                           20, -7, 12, -5, -22, 15, -4, 7};
    auto[l, r, best] = max_subarray_brute_force<int>(array.begin(), array.end());
    EXPECT_EQ(*l, 18);
    EXPECT_EQ(*r, 12);
    EXPECT_EQ(best, 43);
  }

  TEST(max_subarray_divide_and_conquer, simple) {
    std::vector<int> array{13, -3, -25, 20, -3, -16, -23, 18,
                           20, -7, 12, -5, -22, 15, -4, 7};
    auto[l, r, best] = max_subarray_divide_and_conquer<int>(array.begin(), array.end());
    EXPECT_EQ(*l, 18);
    EXPECT_EQ(*r, 12);
    EXPECT_EQ(best, 43);
  }

  TEST(max_subarray_linear, simple) {
    std::vector<int> array{13, -3, -25, 20, -3, -16, -23, 18,
                           20, -7, 12, -5, -22, 15, -4, 7};
    auto[l, r, best] = max_subarray_linear<int>(array.begin(), array.end());
    EXPECT_EQ(*l, 18);
    EXPECT_EQ(*r, 12);
    EXPECT_EQ(best, 43);
  }

  TEST(max_subarray_timer, runtime_analysis_brute_force_vs_divide_and_conquer) {
    const int seed = 42;
    std::default_random_engine g(seed);
    std::uniform_int_distribution<int> distribution(-10, 10);

    for (auto &n : {10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 1000}) {
      std::vector<int> v(n);
      for (int i = 0; i < n; ++i)
        v[i] = distribution(g);
      auto[result_bf, duration_bf] = run_and_clock(v.begin(),
                                                   v.end(),
                                                   max_subarray::max_subarray_brute_force<int>);
      auto[result_dc, duration_dc]  = run_and_clock(v.begin(),
                                                    v.end(),
                                                    max_subarray::max_subarray_divide_and_conquer<int>);
      std::cout << "n: " << n << " duration brute-force: " << duration_bf << "us, duration divide-and-conquer: "
                << duration_dc << "us" << std::endl;
    }
  }

  TEST(max_subarray_timer, runtime_analysis_mixed_vs_divide_and_conquer_vs_linear) {
    const int seed = 42;
    std::default_random_engine g(seed);
    std::uniform_int_distribution<int> distribution(-10, 10);

    for (auto &n : {10, 100, 1000, 10000, 100000, 1000000}) {
      std::vector<int> v(n);
      for (int i = 0; i < n; ++i)
        v[i] = distribution(g);

      auto[result_mx, duration_mx] = run_and_clock(v.begin(),
                                                   v.end(),
                                                   max_subarray::max_subarray_mixed_brute_force_divide_and_conquer<int>);
      auto[result_dc, duration_dc]  = run_and_clock(v.begin(),
                                                    v.end(),
                                                    max_subarray::max_subarray_divide_and_conquer<int>);
      auto[result_ln, duration_ln] = run_and_clock(v.begin(),
                                                   v.end(),
                                                   max_subarray::max_subarray_linear<int>);
      EXPECT_EQ(result_mx, result_dc);
      EXPECT_EQ(result_mx, result_ln);

      std::cout << "n: " << n << " duration mixed: " << duration_mx << "us, duration divide-and-conquer: "
                << duration_dc << "us" << " duration linear: " << duration_ln << "us" << std::endl;
    }
  }

}  // namespace max_subarray::test
