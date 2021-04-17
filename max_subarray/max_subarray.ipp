
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

#include <cmath>
#include <iostream>

namespace max_subarray {

template <typename T, typename I>
std::tuple<I,I,T> max_subarray_brute_force(I begin, I end) {
  I left;
  I right;
  T best = std::numeric_limits<T>::lowest();
  for (auto i = begin; i != end; ++i) {
	T acc = 0;  
    for (auto j = i; j != end; ++j) {
	  acc += *j;
	  if (acc > best) {
		  best = acc;
		  left = i;
		  right = j;
	  }
    }
  }
  return std::make_tuple(left,right,best);
}


template <typename T, typename I>
std::tuple<I,I,T> max_subarray_crossing(I begin, I mid, I end) {
	T best_left = std::numeric_limits<T>::lowest();;
	T left_sum = 0;
	I left_index = mid;
	for (I i = mid; i >= begin; --i) {
		left_sum += *i;
		if (left_sum > best_left) {
			best_left = left_sum;
			left_index = i;
		}
	}
	T best_right = std::numeric_limits<T>::lowest();;
	T right_sum = 0;
	I right_index = mid;
  for (I j = mid+1; j < end; ++j ) {
		right_sum += *j;
		if (right_sum > best_right) {
			best_right = right_sum;
			right_index = j;
		}
	}
  return std::make_tuple(left_index, right_index, best_left + best_right);
}

template <typename T, typename I>
std::tuple<I,I,T> max_subarray_divide_and_conquer(I begin, I end) {
	if (std::distance(begin,end) == 1) {
		return std::make_tuple(begin, end, *begin);
	}
	else {
		I mid = begin + std::floor((std::distance(begin, end) - 1)/2);
		auto [left_begin, left_end, max_left] = max_subarray_divide_and_conquer<T>(begin, mid+1);
  	auto [right_begin, right_end, max_right] = max_subarray_divide_and_conquer<T>(mid+1, end);
		auto [cross_begin, cross_end, max_cross] = max_subarray_crossing<T>(begin, mid, end);

    if (max_left >= max_right && max_left >= max_cross) 
			return std::make_tuple(left_begin, left_end, max_left);
    else if (max_right >= max_cross) 
			return std::make_tuple(right_begin, right_end, max_right);
    else 
			return std::make_tuple(cross_begin, cross_end, max_cross);
	}
}

template <typename T, typename I>
std::tuple<I, I, T> max_subarray_mixed_brute_force_divide_and_conquer(I begin, I end) {
	if (std::distance(begin,end) <= 10) {
		return max_subarray_brute_force<T>(begin, end);
	}
	else {
		I mid = begin + std::floor((std::distance(begin, end) - 1)/2);
		auto [left_begin, left_end, max_left] = max_subarray_divide_and_conquer<T>(begin, mid+1);
  	auto [right_begin, right_end, max_right] = max_subarray_divide_and_conquer<T>(mid+1, end);
		auto [cross_begin, cross_end, max_cross] = max_subarray_crossing<T>(begin, mid, end);

    if (max_left >= max_right && max_left >= max_cross) 
			return std::make_tuple(left_begin, left_end, max_left);
    else if (max_right >= max_cross) 
			return std::make_tuple(right_begin, right_end, max_right);
    else 
			return std::make_tuple(cross_begin, cross_end, max_cross);
	}
}

template <typename T, typename I>
std::tuple<I,I,T> max_subarray_linear(I begin, I end) {
	I l = end;
	I r = end;
	T best = std::numeric_limits<T>::lowest();
	T sub_best = 0;
	I i = begin;
	for (I j = begin; j < end; ++j) {
		sub_best += *j;
		if (sub_best > best) {
			best = sub_best;
			l = i;
			r = j;
		}
		if (sub_best < 0) {
			sub_best = 0;
			i = j + 1;
		}
	}
	return std::make_tuple(l, r, best);
}

}
