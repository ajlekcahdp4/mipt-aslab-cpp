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

#include "vec3.hpp"
#include "equal.hpp"

using vec = throttle::geometry::vec3<float>;
using namespace throttle::geometry;

TEST(test_vec, test_1) {
  vec a = vec::axis_i(), b = vec::axis_j();
  EXPECT_TRUE(is_roughly_equal(cross(a, b), vec::axis_k()));
  vec c{1.5, 2, 1};
  EXPECT_TRUE(is_roughly_equal(c.project(vec::axis_i()), 1.5f * vec::axis_i()));
  EXPECT_TRUE(is_roughly_equal(c.project(vec::axis_j()), 2.0f * vec::axis_j()));
  EXPECT_TRUE(is_roughly_equal(c.project(vec::axis_k()), 1.0f * vec::axis_k()));
}

TEST(test_vec, test_2) {
  vec a = vec::axis_i(), b = vec::axis_j();
  EXPECT_TRUE(is_roughly_equal(dot(a, b), 0.0f));
}