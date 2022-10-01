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
  contiguous_matrix(size_type rows, size_type cols, value_type val = value_type{})
      : m_cols{cols}, m_rows{rows}, m_buffer{cols * rows, val} {}

  template <std::input_iterator it>
  contiguous_matrix(size_type rows, size_type cols, it start, it finish) : contiguous_matrix{rows, cols} {
    std::copy(start, finish, m_buffer.begin());
  }

  contiguous_matrix(size_type rows, size_type cols, std::initializer_list<value_type> list)
      : contiguous_matrix{rows, cols, list.begin(), list.end()} {}

  static contiguous_matrix zero(size_type cols, size_type rows) { return self{cols, rows}; }

  static contiguous_matrix unity(size_type size) {
    self ret{size, size};
    for (size_type i = 0; i < size * size; i += size + 1)
      ret.m_buffer[i] = 1;
    return ret;
  }

private:
  struct proxy_row {
    pointer   m_row;
    reference operator[](size_type index) { return m_row[index]; }
  };

  struct const_proxy_row {
    const_pointer   m_row;
    const_reference operator[](size_type index) { return m_row[index]; }
  };

public:
  proxy_row       operator[](size_type index) { return proxy_row{&m_buffer[index * m_cols]}; }
  const_proxy_row operator[](size_type index) const { return const_proxy_row{&m_buffer[index * m_cols]}; }

  size_type rows() const { return m_rows; }
  size_type cols() const { return m_cols; }

  contiguous_matrix &operator+=(const contiguous_matrix &other) {
    if ((m_cols != other.m_cols) || (m_rows != other.m_rows)) throw std::runtime_error("Mismatched matrix sizes");
    for (auto mybegin = m_buffer.begin(), otherbegin = other.m_buffer.begin(), otherend = other.m_buffer.end();
         otherbegin != otherend; ++otherbegin, ++mybegin) {
      *mybegin += *otherbegin;
    }
    return *this;
  }

  contiguous_matrix &operator*=(value_type rhs) {
    for (auto &elem : m_buffer) {
      elem *= rhs;
    }
    return *this;
  }

  contiguous_matrix &operator/=(value_type rhs) {
    if (rhs == 0) throw std::invalid_argument("devision by zero");
    for (auto &elem : m_buffer) {
      elem /= rhs;
    }
    return *this;
  }
};

template <typename T> contiguous_matrix<T> operator*(const contiguous_matrix<T> &lhs, T rhs) {
  contiguous_matrix ret = lhs;
  ret *= rhs;
  return ret;
}

template <typename T> contiguous_matrix<T> operator*(T lhs, const contiguous_matrix<T> &rhs) {
  contiguous_matrix ret = rhs;
  ret *= lhs;
  return ret;
}

template <typename T> contiguous_matrix<T> operator/(const contiguous_matrix<T> &lhs, T rhs) {
  contiguous_matrix ret = lhs;
  ret /= rhs;
  return ret;
}

} // namespace linmath
} // namespace throttle