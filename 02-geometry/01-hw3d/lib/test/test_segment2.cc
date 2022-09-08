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
#include "point2.hpp"
#include "primitives/segment2.hpp"
#include "vec2.hpp"

using segment_type = typename throttle::geometry::segment2<float>;
using point = typename throttle::geometry::point2<float>;
using vec = typename throttle::geometry::vec2<float>;

using namespace throttle::geometry;

template struct throttle::geometry::segment2<float>;

TEST(test_segment2, test_contain_point) {
  segment_type a({0, 0}, {0, 10});
  point        p1{0, 5};
  point        p2{0, 11};
  EXPECT_TRUE(a.contains(p1));
  EXPECT_FALSE(a.contains(p2));
}