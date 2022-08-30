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
  T m_x;
  T m_y;
  T m_z;

  point3(T p_x, T p_y, T p_z) : m_x{p_x}, m_y{p_y}, m_z{p_z} {}
};

template <typename T> vec3<T> operator-(const point3<T> &p_lhs, const point3<T> &p_rhs) {
  return vec3{p_lhs.m_x - p_rhs.m_x, p_lhs.m_y - p_rhs.m_y, p_lhs.m_z - p_rhs.m_z};
}

template <typename T> point3<T> operator+(const point3<T> &p_lhs, const vec3<T> &p_rhs) {
  return point3{p_lhs.m_x + p_rhs.m_x, p_lhs.m_y + p_rhs.m_y, p_lhs.m_z + p_rhs.m_z};
}

template <typename T> point3<T> operator+(const vec3<T> &p_lhs, const point3<T> &p_rhs) {
  return point3{p_lhs.m_x + p_rhs.m_x, p_lhs.m_y + p_rhs.m_y, p_lhs.m_z + p_rhs.m_z};
}

} // namespace geometry
} // namespace throttle
