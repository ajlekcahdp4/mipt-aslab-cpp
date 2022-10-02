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

#include "contiguous_matrix.hpp"

#include <algorithm>
#include <cstddef>
#include <initializer_list>
#include <iterator>
#include <stdexcept>

namespace throttle {
namespace linmath {
template <typename T>
requires std::is_arithmetic_v<T>
class matrix {
  using value_type = T;
  using reference = T &;
  using const_reference = const T &;
  using pointer = T *;
  using const_pointer = const T *;
  using size_type = typename std::size_t;

  contiguous_matrix<T>        m_contiguous_matrix;
  containers::vector<pointer> m_rows_vec;

public:
  matrix(size_type rows, size_type cols, value_type val = value_type{}) : m_contiguous_matrix{rows, cols, val} {
    m_rows_vec.reserve(rows);
    for (size_type i = 0; i < rows; i++)
      m_rows_vec.push_back(&m_contiguous_matrix[i][0]);
  }

  template <std::input_iterator it>
  matrix(size_type rows, size_type cols, it start, it finish) : m_contiguous_matrix{rows, cols, start, finish} {
    m_rows_vec.reserve(rows);
    for (size_type i = 0; i < rows; i++)
      m_rows_vec.push_back(&m_contiguous_matrix[i][0]);
  }

  matrix(size_type rows, size_type cols, std::initializer_list<value_type> list)
      : m_contiguous_matrix{rows, cols, list} {
    m_rows_vec.reserve(rows);
    for (size_type i = 0; i < rows; i++)
      m_rows_vec.push_back(&m_contiguous_matrix[i][0]);
  }

  matrix(contiguous_matrix<T> &&c_matrix) : m_contiguous_matrix(std::move(c_matrix)) {
    auto rows = m_contiguous_matrix.rows();
    m_rows_vec.reserve(rows);
    for (size_type i = 0; i < rows; i++)
      m_rows_vec.push_back(&m_contiguous_matrix[i][0]);
  }

  static matrix zero(size_type rows, size_type cols) { return matrix<T>{rows, cols}; }

  static matrix unity(size_type size) { return matrix{std::move(contiguous_matrix<T>::unity(size))}; }

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
  proxy_row       operator[](size_type index) { return proxy_row{m_rows_vec[index]}; }
  const_proxy_row operator[](size_type index) const { return const_proxy_row{m_rows_vec[index]}; }

  size_type rows() { return m_contiguous_matrix.rows(); }

  size_type cols() { return m_contiguous_matrix.cols(); }
};
} // namespace linmath
} // namespace throttle