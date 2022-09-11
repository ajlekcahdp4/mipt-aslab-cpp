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

#include "equal.hpp"

namespace throttle {
namespace geometry {

template <typename T> struct segment1 {
  T a;
  T b;
  // assert that a <= b
  segment1(T a_arg, T b_arg) : a{a_arg}, b{b_arg} {
    if (is_definitely_greater(a, b)) throw std::out_of_range("End of 1d segment is less then the start.");
  }

  T len() const noexcept { return b - a; }

  bool intersect(const segment1<T> &other) const {
    if (is_roughly_greater_eq(a, other.a) && is_roughly_less_eq(a, other.b)) return true;
    if (is_roughly_greater_eq(other.a, a) && is_roughly_less_eq(other.a, b)) return true;
    return false;
  }

  bool contains(const segment1<T> &other) const {
    if (is_roughly_less_eq(a, other.a) && is_roughly_greater_eq(b, other.b)) return true;
  }
};

template <typename T> bool operator==(const segment1<T> &lhs, const segment1<T> &rhs) {
  return (is_roughly_equal(lhs.a, rhs.a) && is_roughly_equal(lhs.b, rhs.b));
}

template <typename T> bool operator!=(const segment1<T> &lhs, const segment1<T> &rhs) { return !(lhs == rhs); }

template <typename T> bool intersect(const segment1<T> &a, const segment1<T> &b) { return a.intersect(b); }

} // namespace geometry
} // namespace throttle