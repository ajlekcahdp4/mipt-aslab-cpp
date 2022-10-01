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
using value_type = T;
using reference = T &;
using const_reference = const T &;
using pointer = T *;
using const_pointer = const T *;

template <typename T> class matrix : private contiguous_matrix { containers::vector<pointer> m_rows; };
} // namespace linmath
} // namespace throttle