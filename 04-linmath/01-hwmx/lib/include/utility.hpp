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

static inline int clz(unsigned x) {return __builtin_clz(x); }
static inline int clz(unsigned long x) {return __builtin_clzl(x); }

static inline int ctz(unsigned x) {return __builtin_ctz(x); }
static inline int ctz(unsigned long x) {return __builtin_ctzl(x); }

#include <concepts>

template <std::invocable functor> void do_for_n(std::size_t count, functor f) {
  for (std::size_t i = 0; i < count; ++i) f();
}

} // namespace throttle