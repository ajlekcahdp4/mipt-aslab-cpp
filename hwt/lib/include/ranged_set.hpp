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

#include <cassert>
#include <functional>
#include <utility>

// Haha - as opposed to Boost this library will be called Throttle.
namespace throttle {
namespace detail {
enum rb_tree_color_e {
  COLOR_RED = 0,
  COLOR_BLACK = 1,
};

template <typename T, typename t_size_type> struct rb_tree_ranged_node_t__ {
  using self_t__ = rb_tree_ranged_node_t__<T, t_size_type>;

  self_t__ *m_left, *m_right, *m_parent;
  rb_tree_color_e m_color; // NOTE[]: This information could be moved to the lower bits of the parent pointer.
  t_size_type m_count_below;
  T m_key;

  rb_tree_ranged_node_t__(T p_key, rb_tree_color_e p_color)
      : m_left{}, m_right{}, m_count_below{}, m_color{p_color}, m_key{p_key} {
  }
};

template <typename T, typename t_comp = std::less<T>> class rb_tree_ranged_t__ {
public:
  using size_type = std::size_t;
  using key_type = T;

private:
  using node_t__ = rb_tree_ranged_node_t__<T, size_type>;
  using self_t__ = rb_tree_ranged_t__<T, t_comp>;

  node_t__ *m_root;

private:
  bool is_left_child(node_t__ &p_node) const {
    assert(p_node.m_parent);
    return (&p_node == p_node.m_parent->m_left);
  }

  bool is_right_child(node_t__ &p_node) const {
    return !is_left_child(p_node);
  }

  node_t__ *get_sibling(const node_t__ &p_node) {
    return (is_left_child(p_node) ? p_node.m_parent->m_left : p_node.m_parent->m_right);
  };

  node_t__ *get_uncle(const node_t__ &p_node);

  void left_rotate(node_t__ &p_node);
  void right_rotate(node_t__ &p_node);
  void rotate_to_parent(node_t__ &p_node);

  const node_t__ *bst_lookup(const T &p_key) const {
    if (is_empty()) {
      return nullptr;
    }

    node_t__ *curr = m_root;
    while (curr && (curr->m_key != p_key)) {
      // Comparator returns true if first is _less_ than second.
      bool is_curr_less_than_key = t_comp{}(curr->m_key, p_key);

      if (is_curr_less_than_key) {
        curr = curr->m_right;
      } else {
        curr = curr->m_left;
      }
    }

    return curr;
  }

  node_t__ *bst_lookup(const T &p_key) {
    return const_cast<node_t__ *>(std::as_const(*this).bst_lookup(p_key));
  }

  node_t__ *bst_insert(const T &p_key);
  node_t__ *bst_erase(const T &p_key);

  void recolor_after_insert(node_t__ &p_leaf);
  void recolor_after_erase(node_t__ &p_leaf);

  void prune_leaf(node_t__ *p_leaf);

  bool is_root(const node_t__ &p_node) const {
    return (!p_node.m_parent);
  }

public:
  rb_tree_ranged_t__() : m_root{} {
  }

  ~rb_tree_ranged_t__();
  rb_tree_ranged_t__(const self_t__ &p_other);
  self_t__ &operator=(const self_t__ &p_other);

  bool is_empty() const {
    return (!m_root);
  }

  size_type size() const {
    return (is_empty() ? 0 : m_root->m_count_below);
  }

  void insert(const T &p_key) {
  }

  void erase(const T &p_key) {
  }

  bool contains(const T &p_key) const {
    return bst_lookup(p_key);
  }
};

} // namespace detail
} // namespace throttle