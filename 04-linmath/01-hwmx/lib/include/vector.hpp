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

#include <algorithm>
#include <cstddef>
#include <memory>
#include <type_traits>
#include <stdexcept>

namespace throttle {
namespace containers {

template <typename T>
requires std::is_nothrow_move_assignable_v<T> && std::is_nothrow_destructible_v<T>
class vector {
private:
  T *m_buffer_ptr = nullptr;
  T *m_past_capacity_ptr = nullptr;
  T *m_past_end_ptr = nullptr;

  static constexpr std::size_t default_capacity = 128;

public:
  using size_type = std::size_t;
  using value_type = T;

private:
  using pointer = value_type *;
  using reference = value_type &;

  void delete_elements() noexcept {
    if constexpr (!std::is_trivially_destructible_v<value_type>) {
      std::destroy(m_buffer_ptr, m_past_end_ptr);
    }
    m_past_end_ptr = m_buffer_ptr;
  }

public:
  vector(size_type capacity = default_capacity)
      : m_buffer_ptr{static_cast<pointer>(::operator new(sizeof(value_type) * std::max(capacity, default_capacity)))},
        m_past_capacity_ptr{m_buffer_ptr + std::max(capacity, default_capacity)}, m_past_end_ptr{m_buffer_ptr} {}

  ~vector() {
    auto deleter = [](pointer ptr) { ::operator delete(ptr); };

    std::unique_ptr<value_type, decltype(deleter)> uptr{m_buffer_ptr, deleter};
    delete_elements();
  }

  vector(vector &&rhs) noexcept {
    std::swap(m_buffer_ptr, rhs.m_buffer_ptr);
    std::swap(m_past_capacity_ptr, rhs.m_past_capacity_ptr);
    std::swap(m_past_end_ptr, rhs.m_past_end_ptr);
  }

  vector &operator=(vector &&rhs) noexcept {
    if (this == std::addressof(rhs)) return *this;
    std::swap(m_buffer_ptr, rhs.m_buffer_ptr);
    std::swap(m_past_capacity_ptr, rhs.m_past_capacity_ptr);
    std::swap(m_past_end_ptr, rhs.m_past_end_ptr);
    return *this;
  }

private:
  void resize_if_necessary() {
    if (m_past_capacity_ptr == m_past_end_ptr) throw std::runtime_error{"Not yet implemented resize"};
  }

public:
  void push_back(const value_type &val) {
    resize_if_necessary();
    new (m_past_end_ptr++) T{val};
  }

  void push_back(value_type &&val) {
    resize_if_necessary();
    new (m_past_end_ptr++) T{std::move(val)};
  }

  void clear() { delete_elements(); }
  void pop_back() { std::destroy_at(--m_past_end_ptr); }

  size_type size() const noexcept { return m_past_end_ptr - m_buffer_ptr; }
  size_type capacity() const noexcept { return m_past_capacity_ptr - m_buffer_ptr; }

  value_type &operator[](size_type index) { return *(m_buffer_ptr + index); }
  const value_type &operator[](size_type index) const { return *(m_buffer_ptr + index); }
};

} // namespace containers
} // namespace throttle