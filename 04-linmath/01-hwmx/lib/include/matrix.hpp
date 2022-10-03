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
#include <functional>
#include <initializer_list>
#include <iterator>
#include <limits>
#include <stdexcept>
#include <utility>

namespace throttle {
namespace linmath {
template <typename T> class matrix {
  using value_type = T;
  using reference = T &;
  using const_reference = const T &;
  using pointer = T *;
  using const_pointer = const T *;
  using size_type = typename std::size_t;

  contiguous_matrix<T>        m_contiguous_matrix;
  containers::vector<pointer> m_rows_vec;

  void update_rows_vec() {
    auto rows = this->rows();
    m_rows_vec.reserve(rows);
    for (size_type i = 0; i < rows; i++)
      m_rows_vec.push_back(&m_contiguous_matrix[i][0]);
  }

public:
  matrix(size_type rows, size_type cols, value_type val = value_type{}) : m_contiguous_matrix{rows, cols, val} {
    update_rows_vec();
  }

  template <std::input_iterator it>
  matrix(size_type rows, size_type cols, it start, it finish) : m_contiguous_matrix{rows, cols, start, finish} {
    update_rows_vec();
  }

  matrix(size_type rows, size_type cols, std::initializer_list<value_type> list)
      : m_contiguous_matrix{rows, cols, list} {
    update_rows_vec();
  }

  matrix(contiguous_matrix<T> &&c_matrix) : m_contiguous_matrix(std::move(c_matrix)) { update_rows_vec(); }

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

  size_type rows() const { return m_contiguous_matrix.rows(); }

  size_type cols() const { return m_contiguous_matrix.cols(); }

  bool equal(const matrix &other) const {
    return (rows() == other.rows()) && (cols() == other.cols()) && (m_contiguous_matrix == other.m_contiguous_matrix);
  }

  matrix &transpose() & {
    m_contiguous_matrix.transpose();
    update_rows_vec();
    return *this;
  }

  matrix &operator*=(const matrix &rhs) & {
    if (rows() != rhs.rows()) throw std::runtime_error("Mismatched matrix sizes");
    matrix res{cols(), rhs.cols()}, t_rhs = rhs;
    t_rhs.transpose();
    for (size_type i = 0; i < rows(); i++) {
      for (size_type j = 0; j < rhs.cols(); j++) {
        value_type tmp{};
        for (size_type l = 0; l < cols(); l++)
          tmp += (*this)[i][l] * t_rhs[j][l];
        res[i][j] = tmp;
      }
    }
    std::swap(*this, res);
    return *this;
  }

  void swap_rows(size_type idx1, size_type idx2) { std::swap(m_rows_vec[idx1], m_rows_vec[idx2]); }

  template <typename Comp = std::less<value_type>>
  std::pair<size_type, value_type> max_in_col(size_type col, Comp cmp = Comp{}) {
    size_type max_row_idx = 0;
    auto      rows = this->rows();
    for (size_type row = 0; row < rows; row++)
      max_row_idx = (cmp((*this)[max_row_idx][col], (*this)[row][col]) ? row : max_row_idx);
    return std::pair<size_type, value_type>{max_row_idx, (*this)[max_row_idx][col]};
  }

  value_type determinant() const requires std::is_integral_v<value_type>;

  value_type determinant() const requires std::is_floating_point_v<value_type>;
};

template <typename T> bool operator==(const matrix<T> &lhs, const matrix<T> &rhs) { return lhs.equal(rhs); }

template <typename T> bool operator!=(const matrix<T> &lhs, const matrix<T> &rhs) { return !(lhs.equal(rhs)); }

template <typename T> matrix<T> operator*(const matrix<T> &lhs, const matrix<T> &rhs) {
  matrix res = lhs;
  res *= rhs;
  return res;
}

template <typename T> matrix<T> transpose(const matrix<T> &mat) {
  matrix res = mat;
  res.transpose();
  return res;
}

} // namespace linmath
} // namespace throttle