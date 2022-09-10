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

  point_type a;
  point_type b;

  T signed_distance(const point_type &p_point) const {
    vec_type radius = p_point - b, dir = (b - a).norm(), norm = dir.perp();
    vec_type proj = dir * dot(dir, radius), perp_component = radius - proj;
    return (dot(norm, perp_component) > 0 ? perp_component.length() : -perp_component.length());
  }

  bool contains(const point_type &point) const {
    vec_type segment_vec = b - a, vec = b - point;
    return (co_directional(segment_vec, vec) && segment_vec.length_sq() > vec.length_sq());
  }
};

} // namespace geometry
} // namespace throttle