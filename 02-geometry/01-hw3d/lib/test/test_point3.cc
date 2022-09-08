/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <tsimmerman.ss@phystech.edu>, <alex.rom23@mail.ru> wrote this file.  As long as you
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

#include "equal.hpp"
#include "point3.hpp"
#include "vec3.hpp"

using point = throttle::geometry::point3<float>;
using vec = throttle::geometry::vec3<float>;
using namespace throttle::geometry;

template class throttle::geometry::point3<float>;

TEST(TestPoint3, TestSubscriptOperator_1) {
  point a{1.0, 3.0, 5.0};

  EXPECT_TRUE(is_roughly_equal(a[0], 1.0f));
  EXPECT_TRUE(is_roughly_equal(a[1], 3.0f));
  EXPECT_TRUE(is_roughly_equal(a[2], 5.0f));

  EXPECT_THROW(a[666], std::out_of_range);
}

TEST(TestPoint3, TestDifference) {
  point a{1, 3, 5};
  point b{10, 16, 24};
  auto  res = b - a;
  vec   right{9, 13, 19};
  EXPECT_EQ(res, right);
}

TEST(TestPoint3, TestAdd) {
  point a{1, 3, 5};
  vec   b{10, 16, 24};
  auto  res = b + a;
  point right{11, 19, 29};
  EXPECT_EQ(res, right);
}