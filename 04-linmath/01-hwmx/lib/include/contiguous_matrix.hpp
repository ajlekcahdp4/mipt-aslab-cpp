/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <tsimmerman.ss@phystech.edu>, <alex.rom23@mail.ru> wrote this file.  As long as you
 * retain this notice you can do whatever you want with this stuff. If we meet
 * some day, and you think this stuff is worth it, you can buy us a beer in
 * return.
 * ----------------------------------------------------------------------------
 */

#pragma once

#include "vector.hpp"

#include <algorithm>
#include <initializer_list>
#include <iterator>
#include <memory>
#include <stdexcept>

namespace throttle {
namespace linmath {

template <typename T, typename = std::enable_if<std::is_arithmetic<T>::value>> class contiguous_matrix {
  using value_type = T;
  using reference = T &;
  using const_reference = const T &;
  using pointer = T *;
  using const_pointer = const T *;
  using size_type = typename std::size_t;
  using self = contiguous_matrix<value_type>;

  size_type                      m_cols = 0;
  size_type                      m_rows = 0;
  containers::vector<value_type> m_buffer;

public:
  contiguous_matrix(size_type size, T val = T{})
      : m_cols{size}, m_rows{size}, m_buffer{containers::vector<value_type>(size * size, val)} {}

  contiguous_matrix(size_type cols, size_type rows, T val = T{})
      : m_cols{cols}, m_rows{rows}, m_buffer{containers::vector<value_type>(cols * rows, val)} {}

  static contiguous_matrix zero(size_type cols, size_type rows) { return self(cols, rows); }

  static contiguous_matrix unity(size_type size) {
    self ret(size);
    for (size_type i = 0; i < size * size; i += size + 1)
      ret.m_buffer[i] = 1;
    return ret;
  }

private:
  struct proxy_row {
    pointer m_row;

    reference operator[](size_type index) { return m_row[index]; }
  };

  struct const_proxy_row {
    const_pointer m_row;

    const_reference operator[](size_type index) { return m_row[index]; }
  };

public:
  proxy_row       operator[](size_type index) { return proxy_row{&m_buffer[index * m_cols]}; }
  const_proxy_row operator[](size_type index) const { return const_proxy_row{&m_buffer[index * m_cols]}; }

  self &operator*=(T rhs) {
    for (size_type i = 0; i < m_cols * m_rows; i++)
      m_buffer[i] *= rhs;
    return *this;
  }

  self &operator/=(T rhs) {
    if (rhs == 0) throw std::invalid_argument("devision by zero");
    for (size_type i = 0; i < m_cols * m_rows; i++)
      m_buffer[i] /= rhs;
    return *this;
  }
};

template <typename T> contiguous_matrix<T> operator*(const contiguous_matrix<T> &lhs, T rhs) {
  contiguous_matrix ret = lhs;
  ret *= rhs;
  return ret;
}

template <typename T> contiguous_matrix<T> operator/(const contiguous_matrix<T> &lhs, T rhs) {
  contiguous_matrix ret = lhs;
  ret /= rhs;
  return ret;
}

} // namespace linmath
} // namespace throttle