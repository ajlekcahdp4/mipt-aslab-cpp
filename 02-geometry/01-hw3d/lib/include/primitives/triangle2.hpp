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

#include "point2.hpp"
#include "vec2.hpp"

namespace throttle {
namespace geometry {

template <typename T> struct triangle2 {
  using point_type = point2<T>;
  using vec_type = vec2<T>;

  point_type a;
  point_type b;
  point_type c;

  bool operator==(const triangle2 &other) const { return (a == other.a && b == other.b && c = other.c); }
  bool operator!=(const triangle2 &other) const { return (*this == other); }
};

} // namespace geometry
} // namespace throttle