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

#include "algorithm.hpp"
#include "vector.hpp"
#include "utility.hpp"

#include <algorithm>
#include <initializer_list>
#include <iterator>
#include <memory>
#include <stdexcept>

namespace throttle {
namespace linmath {

template <typename T>
requires std::is_arithmetic_v<T>
class contiguous_matrix {
public:
  using value_type = T;
  using reference = T &;
  using const_reference = const T &;
  using pointer = T *;
  using const_pointer = const T *;
  using size_type = typename std::size_t;

private:
  size_type m_cols = 0;
  size_type m_rows = 0;

  containers::vector<value_type> m_buffer;

public:
  contiguous_matrix(size_type rows, size_type cols, value_type val = value_type{})
      : m_cols{cols}, m_rows{rows}, m_buffer{cols * rows, val} {}

  template <std::input_iterator it>
  contiguous_matrix(size_type rows, size_type cols, it start, it finish) : contiguous_matrix{rows, cols} {
    size_type count = rows * cols;
    std::copy_if(start, finish, m_buffer.begin(), [&count](const auto &) { return count && count--; });
  }

  contiguous_matrix(size_type rows, size_type cols, std::initializer_list<value_type> list)
      : contiguous_matrix{rows, cols, list.begin(), list.end()} {}

  static contiguous_matrix zero(size_type rows, size_type cols) { return contiguous_matrix{rows, cols}; }

  static contiguous_matrix unity(size_type size) {
    contiguous_matrix ret{size, size};
    auto              start = ret.m_buffer.begin();
    for (size_type i = 0; i < size; ++i, start += size + 1) {
      *start = 1;
    }
    return ret;
  }

private:
  class proxy_row {
    pointer m_row, m_past_row;

  public:
    proxy_row() = default;
    proxy_row(pointer row, size_type n_cols) : m_row{row}, m_past_row{m_row + n_cols} {}

    using iterator = utility::contiguous_iterator<value_type>;
    using const_iterator = utility::const_contiguous_iterator<value_type>;

    reference      operator[](size_type index) { return m_row[index]; }
    iterator       begin() const { return iterator{m_row}; }
    iterator       end() const { return iterator{m_past_row}; }
    const_iterator cbegin() const { return const_iterator{m_row}; }
    const_iterator cend() const { return const_iterator{m_past_row}; }
  };

  class const_proxy_row {
    const_pointer m_row, m_past_row;

  public:
    const_proxy_row() = default;
    const_proxy_row(const_pointer row, size_type n_cols) : m_row{row}, m_past_row{m_row + n_cols} {}

    using iterator = utility::const_contiguous_iterator<value_type>;
    using const_iterator = iterator;

    const_reference operator[](size_type index) { return m_row[index]; }
    iterator        begin() const { return iterator{m_row}; }
    iterator        end() const { return iterator{m_past_row}; }
    const_iterator  cbegin() const { return const_iterator{m_row}; }
    const_iterator  cend() const { return const_iterator{m_past_row}; }
  };

public:
  proxy_row       operator[](size_type index) { return proxy_row{&m_buffer[index * m_cols], m_cols}; }
  const_proxy_row operator[](size_type index) const { return const_proxy_row{&m_buffer[index * m_cols], m_cols}; }

  size_type rows() const { return m_rows; }
  size_type cols() const { return m_cols; }
  bool      square() const { return rows() == cols(); }

  contiguous_matrix &operator+=(const contiguous_matrix &other) {
    if ((m_cols != other.m_cols) || (m_rows != other.m_rows)) throw std::runtime_error("Mismatched matrix sizes");
    for (auto mybegin = m_buffer.begin(), otherbegin = other.m_buffer.begin(), otherend = other.m_buffer.end();
         otherbegin != otherend; ++otherbegin, ++mybegin) {
      *mybegin += *otherbegin;
    }
    return *this;
  }

  contiguous_matrix &operator-=(const contiguous_matrix &other) {
    if ((m_cols != other.m_cols) || (m_rows != other.m_rows)) throw std::runtime_error("Mismatched matrix sizes");
    for (auto mybegin = m_buffer.begin(), otherbegin = other.m_buffer.begin(), otherend = other.m_buffer.end();
         otherbegin != otherend; ++otherbegin, ++mybegin) {
      *mybegin -= *otherbegin;
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

  bool equal(const contiguous_matrix &other) const {
    return (m_rows == other.m_rows) && (m_cols == other.m_cols) &&
           (std::equal(m_buffer.begin(), m_buffer.end(), other.m_buffer.begin()));
  }

private:
  void transpose_impl() {}

public:
  contiguous_matrix &transpose() {
    if (m_rows == m_cols) {
      for (size_type i = 0; i < m_rows; i++) {
        for (size_type j = i + 1; j < m_rows; j++) {
          std::swap(m_buffer[i * m_rows + j], m_buffer[j * m_rows + i]);
        }
      }
      std::swap(m_cols, m_rows);
      return *this;
    }

    contiguous_matrix transposed{m_cols, m_rows};
    for (size_type i = 0; i < m_rows; i++) {
      for (size_type j = 0; j < m_cols; j++) {
        transposed.m_buffer[j * m_rows + i] = m_buffer[i * m_cols + j];
      }
    }

    *this = std::move(transposed);
    return *this;
  }

  contiguous_matrix &operator*=(const contiguous_matrix &rhs) {
    if (m_rows != rhs.m_rows) throw std::runtime_error("Mismatched matrix sizes");

    contiguous_matrix res{m_rows, rhs.m_cols}, t_rhs = rhs;
    t_rhs.transpose();

    for (size_type i = 0; i < m_rows; i++) {
      for (size_type j = 0; j < t_rhs.m_rows; j++) {
        res[i][j] = algorithm::multiply_accumulate((*this)[i].cbegin(), (*this)[i].cend(), t_rhs[j].cbegin(), 0);
      }
    }

    std::swap(*this, res);
    return *this;
  }
};

// clang-format off
template <typename T> contiguous_matrix<T> operator*(const contiguous_matrix<T> &lhs, T rhs) { return contiguous_matrix{lhs} *= rhs; }
template <typename T> contiguous_matrix<T> operator*(T lhs, const contiguous_matrix<T> &rhs) { return contiguous_matrix{rhs} *= lhs; }

template <typename T> contiguous_matrix<T> operator*(const contiguous_matrix<T> &lhs, const contiguous_matrix<T> &rhs) { return contiguous_matrix{lhs} *= rhs; }
template <typename T> contiguous_matrix<T> operator/(const contiguous_matrix<T> &lhs, T rhs) { return contiguous_matrix{lhs} /= rhs; }

template <typename T> bool operator==(const contiguous_matrix<T> &lhs, const contiguous_matrix<T> &rhs) { return lhs.equal(rhs); }
template <typename T> bool operator!=(const contiguous_matrix<T> &lhs, const contiguous_matrix<T> &rhs) { return !(lhs.equal(rhs)); }

template <typename T> contiguous_matrix<T> transpose(const contiguous_matrix<T> &matrix) { return contiguous_matrix{matrix}.transpose(); }

} // namespace linmath
} // namespace throttle