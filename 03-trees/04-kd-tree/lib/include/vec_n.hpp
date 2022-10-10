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
#include <functional>
#include <utility>

#include <stdexcept>

#include <range/v3/algorithm.hpp>
#include <range/v3/numeric.hpp>
#include <range/v3/view.hpp>

namespace throttle {

template <typename T, std::size_t N> struct vec_n {
  std::array<T, N> arr;

  using value_type = T;

  static constexpr std::size_t dimension = N;
  static vec_n zero() { return vec_n{}; }

  static vec_n axis(std::size_t index) {
    if (index >= N) throw std::invalid_argument{"Index out of range"};
    vec_n result{};
    result[index] = 1;
    return result;
  }

  T       &operator[](std::size_t index) { return arr[index]; }
  const T &operator[](std::size_t index) const { return arr[index]; }

  bool operator==(const vec_n &other) const { return ranges::equal(arr, other.arr); }
  bool operator!=(const vec_n &other) const { return !(*this == other); }

  // clang-format off
  vec_n neg() const {
    auto res = *this; ranges::transform(res.arr, res.arr.begin(), std::negate<T>{}); return res;
  }

  vec_n norm() const {
    auto res = *this; res /= length(); return (length() ? res : *this);
  }
  // clang-format on

  T dot(const vec_n &other) const { return ranges::inner_product(arr, other.arr, T{}); }
  T length_sq() const { return dot(*this); }
  T length() const { return std::sqrt(length_sq()); }

  vec_n &operator+=(const vec_n &other) {
    ranges::transform(arr, other.arr, arr.begin(), std::plus<T>{});
    return *this;
  }

  vec_n &operator-=(const vec_n &other) {
    ranges::transform(arr, other.arr, arr.begin(), std::minus<T>{});
    return *this;
  }

  vec_n &operator*=(const auto &right) {
    ranges::transform(arr, ranges::views::repeat(right), arr.begin(), std::multiplies<T>{});
    return *this;
  }

  vec_n &operator/=(const auto &right) {
    ranges::transform(arr, ranges::views::repeat(right), arr.begin(), std::divides<T>{});
    return *this;
  }
};

} // namespace throttle

#include "point_n.hpp"

namespace throttle {
template <typename T, std::size_t N> T dot(const vec_n<T, N> &lhs, const vec_n<T, N> &rhs) { return lhs.dot(rhs); }

// clang-format off
template <typename T, std::size_t N> vec_n<T, N> operator+(const vec_n<T, N> &lhs, const vec_n<T, N> &rhs) {
  auto res = lhs; res += rhs; return res; 
}

template <typename T, std::size_t N> vec_n<T, N> operator-(const vec_n<T, N> &lhs, const vec_n<T, N> &rhs) {
  auto res = lhs; res -= rhs; return res; 
}

template <typename T, std::size_t N> vec_n<T, N> operator*(const vec_n<T, N> &lhs, auto rhs) {
  auto res = lhs; res *= rhs; return res; 
}

template <typename T, std::size_t N> vec_n<T, N> operator*(auto lhs, const vec_n<T, N> &rhs) {
  auto res = rhs; res *= lhs; return res; 
}

template <typename T, std::size_t N> vec_n<T, N> operator/(const vec_n<T, N> &lhs, auto rhs) {
  auto res = lhs; res /= rhs; return res; 
}

// clang-format on
} // namespace throttle