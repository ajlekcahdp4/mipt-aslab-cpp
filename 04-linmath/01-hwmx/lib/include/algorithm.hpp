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

#include <concepts>
#include <iterator>

namespace throttle {
namespace algorithm {

template <std::input_iterator iter>
auto multiply_accumulate(iter start1, iter finish1, iter start2,
                         typename std::remove_cv_t<typename std::iterator_traits<iter>::value_type> init) {
  for (; start1 != finish1; ++start1, ++start2) {
    init += (*start1) * (*start2);
  }
  return init;
}

} // namespace algorithm
} // namespace throttle