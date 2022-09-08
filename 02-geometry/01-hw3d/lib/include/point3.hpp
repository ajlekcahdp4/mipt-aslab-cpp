/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <tsimmerman.ss@phystech.edu>, <alex.rom23@mail.ru> wrote this file.  As long as you
 * retain this notice you can do whatever you want with this stuff. If we meet
 * some day, and you think this stuff is worth it, you can buy me a beer in
 * return.
 * ----------------------------------------------------------------------------
 */

#pragma once

#include "point2.hpp"
#include <cmath>
#include <stdexcept>

namespace throttle {
namespace geometry {

template <typename T> struct point3 {
  T x;
  T y;
  T z;

  using point_flat_type = point2<T>;

  static point3 origin() { return {0, 0, 0}; }

  std::pair<unsigned, T> max_component() const { return (*this - origin()).max_component(); }

  T       &operator[](unsigned index) { return (*this - origin())[index]; }
  const T &operator[](unsigned index) const { return (*this - origin())[index]; }

  point_flat_type project_coord(unsigned axis) {
    switch (axis) {
    case 0: return point_flat_type{y, z}; // Project onto yz plane
    case 1: return point_flat_type{x, z}; // Project onto xz plane
    case 2: return point_flat_type{x, y}; // Project onto xy plane
    default: throw std::out_of_range("Axis index for point projection is out of range.");
    }
  }

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
