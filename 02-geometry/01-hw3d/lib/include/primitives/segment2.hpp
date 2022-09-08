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
#include "vec2.hpp"

namespace throttle {
namespace geometry {

template <typename T> struct segment2 {
  using vec_type = vec2<T>;
  using point_type = point2<T>;

  point_type m_a;
  point_type m_b;

  segment2(const point_type &p_start, const vec_type &p_dir) : m_a(p_start), m_b(p_start + p_dir) {}

  bool contains(const point_type &point) const {
    auto segment_vec = m_b - m_a;
    auto vec = m_b - point;
    return (co_directional(segment_vec, vec) && segment_vec.length_sq() > vec.length_sq());
  }
};

} // namespace geometry
} // namespace throttle