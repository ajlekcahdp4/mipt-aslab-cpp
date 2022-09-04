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
#include <type_traits>

namespace throttle {
namespace geometry {

// Precision to be used for floating point comparisons
template <typename T> struct default_precision { static constexpr T m_prec = 1.0e-6f; };

template <typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
bool is_roughly_equal(T p_first, T p_second, T p_precision = default_precision<T>::m_prec) {
  using std::abs;
  using std::max;
  T epsilon = p_precision;
  return (abs(p_first - p_second) <= epsilon * max({abs(p_first), abs(p_second), T{1.0}}));
};

template <typename... Ts, typename = std::enable_if_t<std::conjunction_v<std::is_convertible<bool, Ts>...>>>
bool are_all_true(Ts... args) {
  return (... && args);
}

template <typename... Ts> bool are_same_sign(Ts... args) {
  return (are_all_true(std::greater{}(args, 0)...) || are_all_true(std::less{}(args, 0)...));
}

} // namespace geometry
} // namespace throttle

#include "point3.hpp"
#include "vec3.hpp"

namespace throttle {
namespace geometry {
template <typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
bool is_roughly_equal(vec3<T> p_first, vec3<T> p_second, T p_precision = default_precision<T>::m_prec) {
  return is_roughly_equal(p_first.x, p_second.x, p_precision) && is_roughly_equal(p_first.y, p_second.y, p_precision) &&
         is_roughly_equal(p_first.z, p_second.z, p_precision);
};

template <typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
bool is_roughly_equal(point3<T> p_first, point3<T> p_second, T p_precision = default_precision<T>::m_prec) {
  return is_roughly_equal(p_first.x, p_second.x, p_precision) && is_roughly_equal(p_first.y, p_second.y, p_precision) &&
         is_roughly_equal(p_first.z, p_second.z, p_precision);
};

} // namespace geometry
} // namespace throttle
