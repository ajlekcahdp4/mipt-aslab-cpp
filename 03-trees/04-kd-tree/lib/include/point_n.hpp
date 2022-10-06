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

#include <array>
#include <cstddef>
#include <utility>

#include <range/v3/algorithm.hpp>
#include <range/v3/numeric.hpp>
#include <range/v3/view.hpp>

namespace throttle {

template <typename T, std::size_t N> struct point_n {
  std::array<T, N> arr;

  static constexpr std::size_t dimension = N;
  static point_n origin() { return point_n{}; }
  
  T       &operator[](std::size_t index) { return arr[index]; }
  const T &operator[](std::size_t index) const { return arr[index]; }

  bool operator==(const point_n &other) const { return ranges::equal(arr, other.arr); }
  bool operator!=(const point_n &other) const { return !(*this == other); }
};

} // namespace throttle

#include "vec_n.hpp"

namespace throttle {

template <typename T, std::size_t N> vec_n<T, N> operator-(const point_n<T, N> &lhs, const point_n<T, N> &rhs) {
  vec_n<T, N> result{};
  ranges::transform(lhs.arr, rhs.arr, result.arr.begin(), std::minus<T>{});
  return result;
}

template <typename T, std::size_t N> point_n<T, N> operator+(const point_n<T, N> &lhs, const vec_n<T, N> &rhs) {
  vec_n<T, N> result{};
  ranges::transform(lhs.arr, rhs.arr, result.arr.begin(), std::plus<T>{});
  return result;
}

template <typename T, std::size_t N> point_n<T, N> operator+(const vec_n<T, N> &lhs, const point_n<T, N> &rhs) {
  vec_n<T, N> result{};
  ranges::transform(lhs.arr, rhs.arr, result.arr.begin(), std::plus<T>{});
  return result;
}

} // namespace throttle