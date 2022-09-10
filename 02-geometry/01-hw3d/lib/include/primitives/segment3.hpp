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

#include <cmath>

#include "point3.hpp"
#include "vec3.hpp"
namespace throttle {
namespace geometry {

template <typename T> struct segment3 {
  using vec_type = vec3<T>;
  using point_type = point3<T>;

  point_type a;
  point_type b;

  bool contains(const point_type &point) const {
    vec_type segment_vec = b - a, vec = b - point;
    return (co_directional(segment_vec, vec) && segment_vec.length_sq() > vec.length_sq());
  }
};

} // namespace geometry
} // namespace throttle
