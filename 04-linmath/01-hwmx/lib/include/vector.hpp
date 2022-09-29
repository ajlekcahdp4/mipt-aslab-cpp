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
#include <cstring>
#include <limits>
#include <memory>
#include <stdexcept>
#include <type_traits>

namespace throttle {
namespace containers {

template <typename T>
requires std::is_nothrow_move_assignable_v<T> && std::is_nothrow_destructible_v<T>
class vector {
private:
  T *m_buffer_ptr = nullptr;
  T *m_past_capacity_ptr = nullptr;
  T *m_past_end_ptr = nullptr;

  static constexpr std::size_t default_capacity = 8;

public:
  using size_type = std::size_t;
  using value_type = T;

private:
  using pointer = value_type *;
  using reference = value_type &;
  using const_reference = const value_type &;

  void delete_elements() noexcept {
    if constexpr (!std::is_trivially_destructible_v<value_type>) {
      std::destroy(m_buffer_ptr, m_past_end_ptr);
    }
    m_past_end_ptr = m_buffer_ptr;
  }

  size_type amortized_buffer_size(size_type x) { return 1 << (sizeof(size_type) * CHAR_BIT - __builtin_clz(x)); }

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

  vector(const vector &other) {
    vector temp(capacity());

    auto size = other.size();
    auto temp_data = temp.data();
    auto other_data = other.data();

    if constexpr (std::is_trivially_copyable<T>::value)
      std::memcpy(temp_data, other_data, size * sizeof(T));
    else
      for (int i = 0; i < size; i++)
        std::copy(other_data[i], temp_data[i]);

    temp.m_past_end_ptr += size;

    *this = std::move(temp);
  }

  vector &operator=(vector &&rhs) noexcept {
    if (this == std::addressof(rhs)) return *this;
    std::swap(m_buffer_ptr, rhs.m_buffer_ptr);
    std::swap(m_past_capacity_ptr, rhs.m_past_capacity_ptr);
    std::swap(m_past_end_ptr, rhs.m_past_end_ptr);
    return *this;
  }

  void reserve(size_type new_cap) {
    if (new_cap > capacity()) {
      auto   am_new_cap = amortized_buffer_size(new_cap);
      vector new_vec(am_new_cap);

      auto old_size = this->size();
      auto new_data = new_vec.data();
      auto old_data = data();

      for (int i = 0; i < old_size; i++)
        new_data[i] = std::move(old_data[i]);

      new_vec.m_past_end_ptr += old_size;

      *this = std::move(am_new_cap);
    }
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

  bool empty() const noexcept { return (size() == 0); }

  reference       back() { return *(m_past_end_ptr - 1); }
  const_reference back() const { return *(m_past_end_ptr - 1); }

  reference       front() { return *m_buffer_ptr; }
  const_reference front() const { return *m_buffer_ptr; }

  T       *data() { return m_buffer_ptr; }
  const T *data() const { return m_buffer_ptr; }

  reference       operator[](size_type index) { return *(m_buffer_ptr + index); }
  const_reference operator[](size_type index) const { return *(m_buffer_ptr + index); }

  reference at(size_type index) {
    if (!(index < size())) throw std::out_of_range("index out of range.");
    return *(m_buffer_ptr + index);
  }
  const_reference at(size_type index) const {
    if (!(index < size())) throw std::out_of_range("index out of range.");
    return *(m_buffer_ptr + index);
  }
};

} // namespace containers
} // namespace throttle