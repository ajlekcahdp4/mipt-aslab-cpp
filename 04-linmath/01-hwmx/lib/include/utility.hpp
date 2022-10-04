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

namespace throttle {
namespace utility {

static inline int clz(unsigned x) { return __builtin_clz(x); }
static inline int clz(unsigned long x) { return __builtin_clzl(x); }

static inline int ctz(unsigned x) { return __builtin_ctz(x); }
static inline int ctz(unsigned long x) { return __builtin_ctzl(x); }

#include <concepts>

template <std::invocable functor> void do_for_n(std::size_t count, functor f) {
  for (std::size_t i = 0; i < count; ++i)
    f();
}

template <typename T> struct contiguous_iterator {
  using iterator_category = std::random_access_iterator_tag;
  using difference_type = std::ptrdiff_t;
  using value_type = T;
  using reference = T &;
  using pointer = T *;

private:
  pointer m_ptr;

public:
  contiguous_iterator(pointer ptr) : m_ptr{ptr} {}

  // clang-format off
  reference operator*() const { return *m_ptr; }
  pointer operator->() { return m_ptr; }

  contiguous_iterator &operator++() { m_ptr++; return *this; }
  contiguous_iterator operator++(int) { contiguous_iterator res{m_ptr}; m_ptr++; return res; }
  contiguous_iterator &operator--() { m_ptr--; return *this; }
  contiguous_iterator operator--(int) { contiguous_iterator res{m_ptr}; m_ptr--; return res; }
  contiguous_iterator operator+=(difference_type n) { m_ptr += n; return *this; }
  contiguous_iterator operator-=(difference_type n) { m_ptr -= n; return *this; }

  // clang-format on
  contiguous_iterator operator+(difference_type n) const { return contiguous_iterator{m_ptr + n}; }
  contiguous_iterator operator-(difference_type n) const { return contiguous_iterator{m_ptr - n}; }

  difference_type operator-(const contiguous_iterator other) const { return (m_ptr - other.m_ptr); }

  bool operator==(const contiguous_iterator &other) const { return m_ptr == other.m_ptr; }
  bool operator!=(const contiguous_iterator &other) const { return !(*this == other); }
};

template <typename T> struct const_contiguous_iterator {
  using iterator_category = std::random_access_iterator_tag;
  using difference_type = std::ptrdiff_t;
  using value_type = T;
  using reference = const T &;
  using pointer = const T *;

private:
  pointer m_ptr;

public:
  const_contiguous_iterator(pointer ptr) : m_ptr{ptr} {}

  // clang-format off
  reference operator*() const { return *m_ptr; }
  pointer operator->() { return m_ptr; }

  const_contiguous_iterator &operator++() { m_ptr++; return *this; }
  const_contiguous_iterator operator++(int) { const_contiguous_iterator res{m_ptr}; m_ptr++; return res; }
  const_contiguous_iterator &operator--() { m_ptr--; return *this; }
  const_contiguous_iterator operator--(int) { const_contiguous_iterator res{m_ptr}; m_ptr--; return res; }
  const_contiguous_iterator operator+=(difference_type n) { m_ptr += n; return *this; }
  const_contiguous_iterator operator-=(difference_type n) { m_ptr -= n; return *this; }

  // clang-format on
  const_contiguous_iterator operator+(difference_type n) const { return const_contiguous_iterator{m_ptr + n}; }
  const_contiguous_iterator operator-(difference_type n) const { return const_contiguous_iterator{m_ptr - n}; }

  difference_type operator-(const const_contiguous_iterator other) const { return (m_ptr - other.m_ptr); }

  bool operator==(const const_contiguous_iterator &other) const { return m_ptr == other.m_ptr; }
  bool operator!=(const const_contiguous_iterator &other) const { return !(*this == other); }
};

} // namespace utility
} // namespace throttle