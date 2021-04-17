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
#ifndef MAX_SUBARRAY__MAX_SUBARRAY_H
#define MAX_SUBARRAY__MAX_SUBARRAY_H

namespace max_subarray {

  template<typename T, typename I>
  std::tuple<I, I, T> max_subarray_brute_force(I begin, I end);

  template<typename T, typename I>
  std::tuple<I, I, T> max_subarray_divide_and_conquer(I begin, I end);

  template<typename T, typename I>
  std::tuple<I, I, T> max_subarray_mixed_brute_force_divide_and_conquer(I begin, I end);

  template<typename T, typename I>
  std::tuple<I, I, T> max_subarray_linear(I begin, I end);

}  // namespace max_subarray
#endif