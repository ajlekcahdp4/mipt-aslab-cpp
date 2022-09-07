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

#include <cmath>

namespace throttle {
namespace geometry {

template <typename T> struct point3 {
  T x;
  T y;
  T z;

  static point3 origin() { return {0, 0, 0}; }

  // clang-format off
  std::pair<unsigned, T> max_component() const { return (*this - origin()).max_component(); }

  T &operator[](unsigned index) {
    switch (index) {
      case 0: return x;
      case 1: return y;
      case 2: return z;
      default: throw std::out_of_range("Incorrect coordinate of point3 was requested.");
    }
  }

  const T &operator[](unsigned index) const {
    switch (index) {
      case 0: return x;
      case 1: return y;
      case 2: return z;
      default: throw std::out_of_range("Incorrect coordinate of point3 was requested.");
    }
  }
  // clang-format on

  bool operator==(const point3 &p_other) const { return (x == p_other.x && y == p_other.y && z == p_other.z); }
  bool operator!=(const point3 &p_other) const { return !(*this == p_other); }
};

} // namespace geometry
} // namespace throttle

#include "vec3.hpp"

namespace throttle {
namespace geometry {

template <typename T> vec3<T> operator-(const point3<T> &lhs, const point3<T> &rhs) {
  return {lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z};
}

template <typename T> point3<T> operator+(const point3<T> &lhs, const vec3<T> &rhs) {
  return {lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z};
}

template <typename T> point3<T> operator+(const vec3<T> &lhs, const point3<T> &rhs) {
  return {lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z};
}

} // namespace geometry
} // namespace throttle
