/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <tsimmerman.ss@phystech.edu>, wrote this file.  As long as you
 * retain this notice you can do whatever you want with this stuff. If we meet
 * some day, and you think this stuff is worth it, you can buy me a beer in
 * return.
 * ----------------------------------------------------------------------------
 */

#pragma once

#include "vec3.hpp"
#include <cmath>

namespace throttle {
namespace geometry {

template <typename T> struct point3 {
  T x;
  T y;
  T z;

  static point3 origin() { return {0, 0, 0}; }
};

template <typename T> vec3<T> operator-(const point3<T> &lhs, const point3<T> &rhs) {
  return vec3{lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z};
}

template <typename T> point3<T> operator+(const point3<T> &lhs, const vec3<T> &rhs) {
  return point3{lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z};
}

template <typename T> point3<T> operator+(const vec3<T> &lhs, const point3<T> &rhs) {
  return point3{lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z};
}

} // namespace geometry
} // namespace throttle
