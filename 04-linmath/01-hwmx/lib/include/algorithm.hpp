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

template <std::input_iterator iter1, std::input_iterator iter2>
auto multiply_accumulate(iter1 start1, iter1 finish1, iter2 start2,
                         typename std::remove_cv_t<typename std::iterator_traits<iter1>::value_type> init) {
  for (; start1 != finish1; ++start1, ++start2) {
    init += (*start1) * (*start2);
  }
  return init;
}

} // namespace algorithm
} // namespace throttle