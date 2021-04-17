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
#include <stack/stack.h>
#include <stack/stack.ipp>
#include <random>

namespace stack::test {

  TEST(Stack, push_pop_single) {
    stack::Stack<int> s;
    s.push(1);
    EXPECT_EQ(s.top(), 1);
    EXPECT_EQ(s.size(), 1);
    EXPECT_FALSE(s.empty());
    EXPECT_EQ(s.pop(), 1);
    EXPECT_EQ(s.size(), 0);
    EXPECT_TRUE(s.empty());
  }

  TEST(Stack, push_pop_multiple) {
    stack::Stack<int> s;
    EXPECT_TRUE(s.empty());
    for (int i = 0; i < 10; ++i) {
      s.push(i);
      EXPECT_EQ(s.top(), i);
    }
    for (int i = 9; i >= 0; --i) {
      EXPECT_EQ(s.top(), i);
      EXPECT_EQ(s.size(), i+1);
      s.pop();
      EXPECT_EQ(s.size(), i);
    }
    EXPECT_TRUE(s.empty());
  }

}
