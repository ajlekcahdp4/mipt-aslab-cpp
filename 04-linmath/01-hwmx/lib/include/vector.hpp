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

#include <cstddef>
#include <type_traits>
#include <algorithm>
#include <memory>

namespace throttle {
namespace containers {

template <typename T>
requires std::is_nothrow_move_assignable_v<T>
class vector {
  private:
  T *m_buffer_ptr;
  T *m_past_capacity_ptr;
  T *m_past_end_ptr;

  static constexpr std::size_t default_size = 128;

public:
  vector(std::size_t capacity = default_size)
      : m_buffer_ptr{static_cast<T *>(::operator new(sizeof(T) * capacity))}, m_past_capacity_ptr{m_buffer_ptr + capacity},
        m_past_end_ptr{m_buffer_ptr} {}
  
  ~vector() {
    if constexpr (!std::is_trivially_destructible_v<T>) {
      std::destroy(m_buffer_ptr, m_past_end_ptr);
    }
    ::operator delete(m_buffer_ptr);
  }
};

} // namespace containers
} // namespace throttle