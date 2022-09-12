/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <tsimmerman.ss@phystech.edu>, <alex.rom23@mail.ru>, wrote this file.  As long as you
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

#include "primitives/aabb.hpp"

using AABB = typename throttle::geometry::aabb<double>;
using namespace throttle::geometry;

template struct throttle::geometry::aabb<float>;

TEST(TestAABB, test_intersect_1) {
  AABB a{{0, 0, 0}, 0.5, 0.5, 0.5};
  AABB b{{100, 100, 100}, 1, 2, 3};

  EXPECT_FALSE(aabb_aabb_intersect(a, b));
}

TEST(TestAABB, test_intersect_2) {
  AABB a{{0, 0, 0}, 0.5, 0.5, 0.5};
  AABB b{{0, 0, 1}, 1, 1, 1};

  EXPECT_TRUE(aabb_aabb_intersect(a, b));
}

TEST(TestAABB, test_intersect_3) {
  AABB a{{0, 0, 0}, 0.5, 0.5, 0.5};
  AABB b{{0, 0, 1}, 0.5, 0.5, 0.5};

  EXPECT_TRUE(aabb_aabb_intersect(a, b));
}

TEST(TestAABB, test_intersect_4) {
  AABB a{{0, 0, 0}, 0.5, 0.5, 0.5};
  AABB b{{0, 0.5, 0}, 0, 0.5, 0.5};

  EXPECT_TRUE(aabb_aabb_intersect(a, b));
}

TEST(TestAABB, test_intersect_5) {
  AABB a{{0, 0, 0}, 0.5, 0.5, 0.5};
  AABB b{{0, 1, 0}, 0, 0.5, 0.5};

  EXPECT_TRUE(aabb_aabb_intersect(a, b));
}

TEST(TestAABB, test_intersect_6) {
  AABB a{{0, 0, 0}, 0.5, 0, 0.5};
  AABB b{{0, 0, 0}, 0, 0.5, 0.5};

  EXPECT_TRUE(aabb_aabb_intersect(a, b));
}

TEST(TestAABB, test_intersect_7) {
  AABB a{{0, 0, 0}, 0.5, 0.5, 0.5};
  AABB b{{-0.5, 0.75, 0}, 0, 0.5, 0.5};

  EXPECT_TRUE(aabb_aabb_intersect(a, b));
}

TEST(TestAABB, test_equal) {
  AABB a{{0, 0, 0}, 1, 2, 3};
  AABB b{{0, 0, 0}, 1, 2, 3};
  AABB c{{0, 1, 0}, 1, 2, 3};

  EXPECT_TRUE(a == b);
  EXPECT_FALSE(a == c);
}

TEST(TestAABB, test_not_equal) {
  AABB a{{0, 0, 0}, 1, 2, 3};
  AABB b{{0, 0, 0}, 1, 2, 3};
  AABB c{{0, 1, 0}, 1, 2, 3};

  EXPECT_FALSE(a != b);
  EXPECT_TRUE(a != c);
}

TEST(TestAABB, test_variadic_constructor) {
  using point = AABB::point_type;
  AABB a{point{1, 2, 3}, point{4, 5, 6}, point{7, 8, 9}};

  EXPECT_TRUE(is_roughly_equal(a.m_center, {4, 5, 6}));
  EXPECT_TRUE(is_roughly_equal(a.m_halfwidth_x, 6.0 / 2));
  EXPECT_TRUE(is_roughly_equal(a.m_halfwidth_y, 6.0 / 2));
  EXPECT_TRUE(is_roughly_equal(a.m_halfwidth_z, 6.0 / 2));
}
