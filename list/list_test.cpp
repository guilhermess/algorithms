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
#include <list/list.h>
#include <list/list.ipp>
#include <random>

using namespace ::testing;

namespace list::test {

  TEST(List, emplace_back_1) {
    list::List<int> l;
    l.emplace_back(1);
    ASSERT_EQ(l.size(), 1);
    EXPECT_EQ(l.front(), 1);
    EXPECT_EQ(l.back(), 1);
    l.pop_front();
    EXPECT_EQ(l.size(), 0);
  }

  TEST(List, emplace_front_12) {
    list::List<int> l;
    l.emplace_front(1);
    l.emplace_front(2);
    ASSERT_EQ(l.size(), 2);
    EXPECT_EQ(l.front(), 2);
    EXPECT_EQ(l.back(), 1);
    l.pop_front();
    EXPECT_EQ(l.front(), 1);
    EXPECT_EQ(l.back(), 1);
    EXPECT_EQ(l.size(), 1);
    l.pop_back();
    EXPECT_EQ(l.size(), 0);
  }

  TEST(DoubleList, double_list_iterator) {
    list::DoubleList<int> l;
    for (int i = 0; i < 5; ++i) {
      l.emplace_back(i);
    }
    EXPECT_THAT(l, ElementsAre(0,1,2,3,4));
  }

  TEST(List, reverse) {
    list::List<int> l;
    for (int i = 0; i < 5; ++i) {
      l.emplace_back(i);
    }
    l.reverse();
    EXPECT_THAT(l, ElementsAre(4,3,2,1,0));
  }

  TEST(DoubleList, reverse) {
    list::DoubleList<int> l;
    for (int i = 0; i < 5; ++i) {
      l.emplace_back(i);
    }
    l.reverse();
    EXPECT_THAT(l, ElementsAre(4,3,2,1,0));
  }

}
