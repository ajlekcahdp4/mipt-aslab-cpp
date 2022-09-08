/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <tsimmerman.ss@phystech.edu>, wrote this file.  As long as you
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

#include <array>
#include <cmath>

#include "equal.hpp"
#include "narrowphase/triangletriangle.hpp"
#include "primitives/triangle2.hpp"

using triangle2 = throttle::geometry::triangle2<float>;
using throttle::geometry::is_roughly_equal;

template struct throttle::geometry::triangle2<float>;

TEST(test_triangle2, test_1) {
  triangle2 t{{0, 0}, {2, 6}, {4, -1}};
  EXPECT_TRUE(t.point_in_triangle({1, 3}));
  EXPECT_TRUE(t.point_in_triangle({2, 1}));
  EXPECT_FALSE(t.point_in_triangle({0, 1}));
  EXPECT_TRUE(t.point_in_triangle({0, 0}));
  EXPECT_TRUE(t.point_in_triangle({2, 6}));
  EXPECT_TRUE(t.point_in_triangle({4, -1}));
}