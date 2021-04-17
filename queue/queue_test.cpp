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
#include <queue/queue.h>
#include <queue/queue.ipp>
#include <random>

namespace queue::test {

  TEST(Queue, push_pop_single) {
    queue::Queue<int> q;
    q.push(1);
    EXPECT_EQ(q.size(), 1);
    EXPECT_FALSE(q.empty());
    EXPECT_EQ(q.pop(), 1);
    EXPECT_EQ(q.size(), 0);
    EXPECT_TRUE(q.empty());
  }

  TEST(Queue, push_pop_multiple) {
    queue::Queue<int> q;
    EXPECT_TRUE(q.empty());
    for (int i = 0; i < 10; ++i) {
      q.push(i);
    }
    for (int i = 0; i < 10; ++i) {
      EXPECT_EQ(q.pop(), i);
      EXPECT_EQ(q.size(), 10 - i - 1);
    }
    EXPECT_TRUE(q.empty());
  }

}
