/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <tsimmerman.ss@phystech.edu>, wrote this file.  As long as you
 * retain this notice you can do whatever you want with this stuff. If we meet
 * some day, and you think this stuff is worth it, you can buy us a beer in
 * return.
 * ----------------------------------------------------------------------------
 */

#pragma once

#include <functional>

// Haha - as opposed to Boost this library will be called Throttle.
namespace throttle {
namespace detail {
enum rb_tree_color_e {
  COLOR_RED = 0,
  COLOR_BLACK = 1,
};

template <typename T, typename t_comp = std::less<T>, typename t_size_type = std::size_t> struct rb_tree_ranged_node_t__ {
  using self_t__ = rb_tree_ranged_node_t__<T, t_comp>;

  self_t__ *m_left, *m_right, *m_parent;
  rb_tree_color_e m_color; // NOTE[]: This information could be moved to the lower bits of the parent pointer.
  t_size_type m_count_left, m_count_right; // NOTE[]: This information can also be consolidated.

  rb_tree_ranged_node_t__(rb_tree_color_e p_color) : m_color{p_color} {
  }
};
} // namespace detail
} // namespace throttle