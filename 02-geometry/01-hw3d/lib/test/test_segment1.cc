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

#include "primitives/segment1.hpp"

using segment = typename throttle::geometry::segment1<float>;
using namespace throttle::geometry;

template struct throttle::geometry::segment1<float>;

TEST(test_segment1, test_ctor) {
  EXPECT_NO_THROW(segment a(1, 3));
  EXPECT_THROW(segment(3, 1), std::out_of_range);
}

TEST(test_segment1, test_intersect) {
  segment a(1, 5);
  segment b(2, 3);
  segment c(0, 1);
  segment d(2, 30);

  EXPECT_TRUE(intersect(a, b));
  EXPECT_TRUE(intersect(a, c));
  EXPECT_TRUE(intersect(a, d));
  EXPECT_TRUE(intersect(b, d));
}

TEST(test_segment1, test_contains) {
  segment a(1, 5);
  segment b(2, 3);
  segment c(1, 5);

  EXPECT_TRUE(a.contains(b));
  EXPECT_TRUE(a.contains(c));
}