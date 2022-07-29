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
#include <cstdint>
#include <exception>
#include <functional>
#include <iostream>
#include <tuple>
#include <utility>

// Haha - as opposed to Boost this library will be called Throttle.
namespace throttle {

template <typename T, typename t_comp> class ranged_set;
namespace detail {

template <typename t_node> class rb_tree_dumper__ {
  typename t_node::size_type m_current_index;

public:
  rb_tree_dumper__() : m_current_index{} {
  }

  void dump_helper(t_node *p_node, std::ostream &p_ostream) {
    if (!p_node) {
      p_ostream << "\tnode_" << m_current_index++ << " [label = \"NIL\", color = \""
                << t_node{t_node::color_e::BLACK}.color_name() << "\"];\n";
      return;
    }

    auto this_index = m_current_index;
    p_ostream << "\tnode_" << m_current_index++ << " [label = \"" << p_node->m_key << "\", color = \""
              << p_node->color_name() << "\"];\n";

    p_ostream << "\tnode_" << this_index << " -> node_" << this_index + 1 << ";\n";
    dump_helper(p_node->m_left, p_ostream);

    p_ostream << "\tnode_" << this_index << " -> node_" << m_current_index << ";\n";
    dump_helper(p_node->m_right, p_ostream);
  }

  void dump(t_node *p_node, std::ostream &p_ostream) {
    m_current_index = 0;

    p_ostream << "digraph RedBlackTree {\n";
    dump_helper(p_node, p_ostream);
    p_ostream << "}\n";
  }
};

template <typename T, typename t_size_type> struct rb_tree_ranged_node_t__ {
  using size_type = t_size_type;
  using self_t__ = rb_tree_ranged_node_t__<T, t_size_type>;

  enum class color_e {
    BLACK,
    RED,
  };

  self_t__ *m_left, *m_right, *m_parent;
  color_e m_color; // NOTE[]: This information could be moved to the lower bits of the parent pointer.
  t_size_type m_count_below;
  T m_key;

  rb_tree_ranged_node_t__(color_e p_color) : m_left{}, m_right{}, m_color{p_color}, m_count_below{} {
  }

  rb_tree_ranged_node_t__(T p_key, color_e p_color)
      : m_left{}, m_right{}, m_color{p_color}, m_count_below{}, m_key{p_key} {
  }

  constexpr const char *color_name() {
    using enum color_e;
    switch (m_color) {
    case BLACK:
      return "black";
    case RED:
      return "red";
    }
  }

  bool has_parent() const noexcept {
    return m_parent;
  }

  bool is_left_child() const noexcept {
    assert(has_parent());
    return (this == m_parent->m_left);
  }

  bool is_right_child() const noexcept {
    return !is_left_child();
  }

  self_t__ *get_sibling() noexcept {
    return (is_left_child() ? m_parent->m_left : m_parent->m_right);
  };

  self_t__ *get_uncle();
};

} // namespace detail

template <typename T, typename t_comp = std::less<T>> class ranged_set {
public:
  using size_type = std::size_t;
  using key_type = T;

private:
  using node_t__ = detail::rb_tree_ranged_node_t__<T, size_type>;
  using self_t__ = ranged_set<T, t_comp>;

private:
  node_t__ *m_root;

  void left_rotate(node_t__ &p_node);
  void right_rotate(node_t__ &p_node);
  void rotate_to_parent(node_t__ &p_node);

  template <typename F>
  std::tuple<node_t__ *, node_t__ *, bool> traverse_binary_search(node_t__ *p_root, const T &p_key, F p_functor) {
    if (!p_root) {
      return std::make_tuple(nullptr, nullptr, false);
    }

    node_t__ *curr{p_root}, *prev{};
    bool is_less_than_key{};

    while (curr && (curr->m_key != p_key)) {
      is_less_than_key = t_comp{}(curr->m_key, p_key);
      p_functor(*curr);
      prev = curr;
      if (is_less_than_key) {
        curr = curr->m_right;
      } else {
        curr = curr->m_left;
      }
    }

    return std::make_tuple(curr, prev, is_less_than_key);
  }

  template <typename F>
  std::tuple<const node_t__ *, const node_t__ *, bool> traverse_binary_search(const node_t__ *p_root, const T &p_key,
                                                                              F p_functor) const {
    if (!p_root) {
      return std::make_tuple(nullptr, nullptr, false);
    }

    const node_t__ *curr{p_root}, *prev{};
    bool is_less_than_key{};

    while (curr && (curr->m_key != p_key)) {
      is_less_than_key = t_comp{}(curr->m_key, p_key);
      p_functor(*curr);
      prev = curr;
      if (is_less_than_key) {
        curr = curr->m_right;
      } else {
        curr = curr->m_left;
      }
    }

    return std::make_tuple(curr, prev, is_less_than_key);
  }

  node_t__ *bst_lookup(const T &p_key) noexcept {
    auto [found, prev, is_prev_less] = traverse_binary_search(m_root, p_key, [](const node_t__ &) {});
    return found;
  }

  const node_t__ *const bst_lookup(const T &p_key) const noexcept {
    auto [found, prev, is_prev_less] = traverse_binary_search(m_root, p_key, [](const node_t__ &) {});
    return found;
  }

  void bst_insert_failed_backtrace(const T &p_key) {
    traverse_binary_search(p_key, [](node_t__ &p_node) { p_node.m_count_below--; });
  }

  node_t__ *bst_insert(const T &p_key) {
    node_t__ *to_insert = new node_t__{p_key, node_t__::color_e::RED};

    if (is_empty()) {
      to_insert->m_color = node_t__::color_e::BLACK;
      return m_root = to_insert;
    }

    auto [found, prev, is_prev_less] =
        traverse_binary_search(m_root, p_key, [](node_t__ &p_node) { p_node.m_count_below++; });

    if (found) {
      traverse_binary_search(m_root, p_key, [](node_t__ &p_node) { p_node.m_count_below--; });
      delete to_insert;
      throw std::invalid_argument("");
    }

    to_insert->m_parent = prev;
    if (is_prev_less) {
      prev->m_right = to_insert;
    } else {
      prev->m_left = to_insert;
    }

    return to_insert;
  }

  node_t__ *move_to_leaf(node_t__ &p_node) {
  }

  void recolor_after_insert(node_t__ &p_leaf);
  void recolor_after_erase(node_t__ &p_leaf);

  void prune_leaf(node_t__ *p_leaf);

  bool is_root(const node_t__ &p_node) const noexcept {
    return (!p_node.m_parent);
  }

public:
  ranged_set() : m_root{} {
  }

  ~ranged_set() {
    // Postorder iterative destructor
    node_t__ *curr{m_root};
    while (curr) {
      if (curr->m_left) {
        curr = curr->m_left;
      } else if (curr->m_right) {
        curr = curr->m_right;
      } else {
        node_t__ *parent = curr->m_parent;
        if (parent) {
          if (curr->is_left_child()) {
            parent->m_left = nullptr;
          } else {
            parent->m_right = nullptr;
          }
        }
        delete curr;
        curr = parent;
      }
    }
  }

  ranged_set(const self_t__ &p_other);
  self_t__ &operator=(const self_t__ &p_other);

  bool is_empty() const noexcept {
    return (!m_root);
  }

  size_type size() const noexcept {
    return (is_empty() ? 0 : m_root->m_count_below);
  }

  void insert(const T &p_key) {
    bst_insert(p_key);
  }

  void erase(const T &p_key) {
    node_t__ *leaf = move_to_leaf(p_key);
    prune_leaf(leaf);
  }

  bool contains(const T &p_key) const {
    return bst_lookup(p_key);
  }

  void dump(std::ostream &p_ostream) const {
    detail::rb_tree_dumper__<node_t__>{}.dump(m_root, p_ostream);
  }
};

} // namespace throttle