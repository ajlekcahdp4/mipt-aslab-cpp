/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <tsimmerman.ss@phystech.edu> wrote this file.  As long as you
 * retain this notice you can do whatever you want with this stuff. If we meet
 * some day, and you think this stuff is worth it, you can buy me a beer in
 * return.
 * ----------------------------------------------------------------------------
 */

#include <algorithm>
#include <cstddef>
#include <cstdlib>
#include <functional>
#include <gtest/gtest.h>
#include <numeric>

#include "point_n.hpp"
#include "vec_n.hpp"

template struct throttle::point_n<float, 4>;
template struct throttle::vec_n<float, 4>;

TEST(test_vec_n, test_1) {
  using vec4 = throttle::vec_n<float, 4>;
  using point4 = throttle::point_n<float, 4>;

  vec4 a{1, 2, 3, 4}, b{1, 1, 1, 1};
  EXPECT_EQ(a.dot(b), 10);
  EXPECT_EQ(dot(a, b), 10);

  vec4 c = a / 2;
  vec4 d{0.5, 1.0, 1.5, 2.0};

  EXPECT_EQ(c, d);
  EXPECT_EQ(a.length_sq(), 30);
  EXPECT_EQ(c.length_sq(), 7.5);

  c = a * 0.5;

  EXPECT_EQ(c, d);
  EXPECT_EQ(a.length_sq(), 30);
  EXPECT_EQ(c.length_sq(), 7.5);
}

TEST(test_vec_n, test_2) {
  using vec4 = throttle::vec_n<float, 4>;
  using point4 = throttle::point_n<float, 4>;

  vec4 a{1, 2, 3, 4}, b{1, 1, 1, 1};
  vec4 c = a + b;
  vec4 d = a - b;

  EXPECT_EQ(c.length_sq(), 4 + 9 + 16 + 25);
  EXPECT_EQ(d.length_sq(), 0 + 1 + 4 + 9);
}
