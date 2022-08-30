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

template <typename T> struct vec3 {
  T m_x;
  T m_y;
  T m_z;

  vec3(T p_x, T p_y, T p_z) : m_x{p_x}, m_y{p_y}, m_z{p_z} {}

  static vec3 axis_i() {
    return vec3{1, 0, 0};
  }

  static vec3 axis_j() {
    return vec3{0, 1, 0};
  }

  static vec3 axis_k() {
    return vec3{0, 0, 1};
  }

  vec3 neg() const {
    return vec3{m_x * -1, m_y * -1, m_z * -1};
  }

  vec3 norm() const {
    T length = length();
    return vec3{m_x / length, m_y / length, m_z / length};
  }

  T dot(const vec3 &p_rhs) const {
    return m_x * p_rhs.m_x + m_y * p_rhs.m_y + m_z * p_rhs.m_z;
  }

  vec3 project(const vec3 &p_axis) {
    return dot(p_axis) / p_axis.length_sq() * p_axis;
  }

  // clang-format off
  vec3 cross(const vec3 p_rhs) const {
    return vec3{m_y * p_rhs.m_z - m_z * p_rhs.m_y, 
                -(m_x * p_rhs.m_z - m_z * p_rhs.m_x),
                m_x * p_rhs.m_y - m_y * p_rhs.m_x};
  }
  // clang-format on

  T length_sq() const {
    return dot(*this);
  }

  T length() const {
    return std::sqrt(length_sq());
  }

  vec3 &operator+=(const vec3 &p_rhs) {
    m_x += p_rhs.m_x;
    m_y += p_rhs.m_y;
    m_z += p_rhs.m_z;
    return *this;
  }

  vec3 &operator-=(const vec3 &p_rhs) {
    return *this += p_rhs.neg();
  }

  bool operator==(const vec3 &p_rhs) const {
    return (m_x == p_rhs.m_x && m_y == p_rhs.m_y && m_z == p_rhs.m_z);
  }

  bool operator!=(const vec3 &p_rhs) const {
    return !(*this == p_rhs);
  }
};

template <typename T> vec3<T> operator+(const vec3<T> &p_lhs, const vec3<T> &p_rhs) {
  return vec3<T>{p_lhs} += p_rhs;
}

template <typename T> vec3<T> operator-(const vec3<T> &p_lhs, const vec3<T> &p_rhs) {
  return vec3<T>{p_lhs} -= p_rhs;
}

template <typename T> vec3<T> operator*(const vec3<T> &p_lhs, T p_rhs) {
  return vec3<T>{p_lhs.m_x * p_rhs, p_lhs.m_y * p_rhs, p_lhs.m_z * p_rhs};
}

template <typename T> vec3<T> operator*(T p_lhs, const vec3<T> &p_rhs) {
  return vec3<T>{p_rhs.m_x * p_lhs, p_rhs.m_y * p_lhs, p_rhs.m_z * p_lhs};
}

template <typename T> vec3<T> operator/(const vec3<T> &p_lhs, T p_rhs) {
  return vec3<T>{p_lhs.m_x / p_rhs, p_lhs.m_y / p_rhs, p_lhs.m_z / p_rhs};
}

template <typename T> T dot(vec3<T> p_lhs, vec3<T> p_rhs) {
  return p_lhs.dot(p_rhs);
}

template <typename T> vec3<T> cross(vec3<T> p_lhs, vec3<T> p_rhs) {
  return p_lhs.cross(p_rhs);
}

} // namespace geometry
} // namespace throttle
