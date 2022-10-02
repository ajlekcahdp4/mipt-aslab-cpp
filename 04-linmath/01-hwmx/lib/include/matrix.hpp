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
class matrix : {
  using value_type = T;
  using reference = T &;
  using const_reference = const T &;
  using pointer = T *;
  using const_pointer = const T *;
  using size_type = typename std::size_t;

  contiguous_matrix<T>        m_contiguous_matrix;
  containers::vector<pointer> m_rows;

public:
  matrix(size_type rows, size_type cols, value_type val = value_type{}) : m_contiguous_matrix{rows, cols, val} {
    for (size_type i = 0; i < rows; i++)
      m_rows.push_back(&m_buffer[i * cols]);
  }

  template <std::input_iterator it>
  matrix(size_type rows, size_type cols, it start, it finish) : m_contiguous_matrix{rows, cols, start, finish} {
    for (size_type i = 0; i < rows; i++)
      m_rows.push_back(&m_buffer[i * cols]);
  }

  matrix(size_type rows, size_type cols, std::initializer_list<value_type> list)
      : m_contiguous_matrix{rows, cols, list} {
    for (size_type i = 0; i < rows; i++)
      m_rows.push_back(&m_buffer[i * cols]);
  }

  matrix(contiguous_matrix &&c_matrix) : m_contiguous_matrix(std::move(c_matrix)) {
    for (size_type i = 0; i < rows; i++)
      m_rows.push_back(&m_buffer[i * cols]);
  }

  static matrix zero(size_type rows, size_type cols) { return matrix{rows, cols}; }

  static matrix unity(size_type size) { return matrix{std::move(contiguous_matrix::unity(size))}; }
};
} // namespace linmath
} // namespace throttle