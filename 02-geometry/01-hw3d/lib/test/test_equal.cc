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

#include "equal.hpp"
using namespace throttle::geometry;

TEST(test_equal, test_1) {
  static constexpr float epsilon_flt = 1.0e-6f;
  EXPECT_FALSE(is_roughly_equal(1.0f, 1.0e-7f, epsilon_flt));
  EXPECT_TRUE(is_roughly_equal(1.0f, 1.0000000001f, epsilon_flt));
  EXPECT_FALSE(is_roughly_equal(1.0, 1.0e-11, double(epsilon_flt)));
}