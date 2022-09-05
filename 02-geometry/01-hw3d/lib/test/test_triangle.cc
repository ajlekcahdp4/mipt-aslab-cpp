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

#include <cmath>

#include "equal.hpp"
#include "primitives/plane.hpp"
#include "primitives/triangle.hpp"

using plane = throttle::geometry::plane<float>;
using triangle3 = throttle::geometry::triangle3<float>;

using throttle::geometry::is_roughly_equal;

TEST(test_triangle, test_1) {
  triangle3 t{{1, 2, 3}, {-1, 2, 3}, {5, 8, -1}};
  EXPECT_FALSE(t.lies_on_one_side(plane::plane_xy()));
  EXPECT_FALSE(t.lies_on_one_side(plane::plane_yz()));
  EXPECT_TRUE(t.lies_on_one_side(plane::plane_xz()));
}