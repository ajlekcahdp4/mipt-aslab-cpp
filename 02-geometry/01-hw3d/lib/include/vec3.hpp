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

// clang-format off
template <typename T> struct vec3 {
  T x;
  T y;
  T z;

  // Static pseudoconstructors.
  static vec3 zero() { return vec3{0, 0, 0}; }
  static vec3 axis_i() { return vec3{1, 0, 0}; }
  static vec3 axis_j() { return vec3{0, 1, 0}; }
  static vec3 axis_k() { return vec3{0, 0, 1}; }

  vec3 neg() const { return vec3{x * -1, y * -1, z * -1}; }
  vec3 norm() const { T length = vec3::length(); return vec3{x / length, y / length, z / length}; }

  T length_sq() const { return dot(*this); }
  T length() const { return std::sqrt(length_sq()); }

  T dot(const vec3 &rhs) const { return x * rhs.x + y * rhs.y + z * rhs.z; }
  vec3 cross(const vec3 rhs) const { return vec3{y * rhs.z - z * rhs.y, 
                                                  -(x * rhs.z - z * rhs.x), 
                                                  x * rhs.y - y * rhs.x}; }

  vec3 project(const vec3 &p_axis) { return dot(p_axis) / p_axis.length_sq() * p_axis; }

  vec3 &operator+=(const vec3 &rhs) { x += rhs.x; y += rhs.y; z += rhs.z; return *this; }
  vec3 &operator-=(const vec3 &rhs) { return *this += rhs.neg(); }
  vec3 operator-() const { return neg(); }
  bool operator==(const vec3 &rhs) const { return (x == rhs.x && y == rhs.y && z == rhs.z); }
  bool operator!=(const vec3 &rhs) const { return !(*this == rhs); }
};
// clang-format on

template <typename T> T       dot(vec3<T> lhs, vec3<T> rhs) { return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z; }
template <typename T> vec3<T> cross(vec3<T> lhs, vec3<T> rhs) { return lhs.cross(rhs); }

template <typename T> vec3<T> operator+(const vec3<T> &lhs, const vec3<T> &rhs) { return vec3<T>{lhs} += rhs; }
template <typename T> vec3<T> operator-(const vec3<T> &lhs, const vec3<T> &rhs) { return vec3<T>{lhs} -= rhs; }

template <typename T> vec3<T> operator*(const vec3<T> &lhs, T rhs) { return {lhs.x * rhs, lhs.y * rhs, lhs.z * rhs}; }
template <typename T> vec3<T> operator*(T lhs, const vec3<T> &rhs) { return {rhs.x * lhs, rhs.y * lhs, rhs.z * lhs}; }
template <typename T> vec3<T> operator/(const vec3<T> &lhs, T rhs) { return {lhs.x / rhs, lhs.y / rhs, lhs.z / rhs}; }

} // namespace geometry
} // namespace throttle

#include "equal.hpp"

namespace throttle {
namespace geometry {

template <typename T>
bool colinear(const vec3<T> &lhs, const vec3<T> &rhs,
              T p_tolerance = ::throttle::geometry::default_precision<T>::m_prec) {
  return is_roughly_equal(cross(lhs, rhs), vec3<T>::zero(), p_tolerance);
}

} // namespace geometry
} // namespace throttle
