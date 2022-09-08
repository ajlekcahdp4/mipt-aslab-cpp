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
#include "primitives/segment.hpp"
#include "vec3.hpp"

using segment_type = typename throttle::geometry::segment<float>;
using point = typename throttle::geometry::point3<float>;
using vec = typename throttle::geometry::vec3<float>;

using namespace throttle::geometry;

template class throttle::geometry::segment<float>;

TEST(test_segment, test_contain_point) {
  segment_type a({0, 0, 0}, {0, 0, 10});
  point        p1{0, 0, 5};
  point        p2{0, 0, 11};
  EXPECT_TRUE(a.contain(p1));
  EXPECT_FALSE(a.contain(p2));
}