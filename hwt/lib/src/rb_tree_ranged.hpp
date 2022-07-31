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
#include <cstddef>
#include <iostream>
#include <stdexcept>
#include <tuple>

namespace throttle {
namespace detail {

template <typename t_node> class rb_tree_dumper__ {
  typename t_node::size_type m_current_index;

public:
  rb_tree_dumper__() : m_current_index{} {}

  void dump_helper(t_node *p_node, std::ostream &p_ostream) {
    if (!p_node) {
      p_ostream << "\tnode_" << m_current_index++ << " [label = \"NIL\", color = \""
                << "black"
                << "\"];\n";
      return;
    }

    auto this_index = m_current_index;
    p_ostream << "\tnode_" << m_current_index++ << " [label = \"" << p_node->m_value_ << ":" << p_node->m_size_
              << "\", color = \"" << p_node->color_name() << "\"];\n";

    p_ostream << "\tnode_" << this_index << " -> node_" << this_index + 1 << ";\n";
    dump_helper(static_cast<t_node *>(p_node->m_left_), p_ostream);

    p_ostream << "\tnode_" << this_index << " -> node_" << m_current_index << ";\n";
    dump_helper(static_cast<t_node *>(p_node->m_right_), p_ostream);
  }

  void dump(t_node *p_node, std::ostream &p_ostream) {
    m_current_index = 0;

    p_ostream << "digraph RedBlackTree {\n";
    dump_helper(p_node, p_ostream);
    p_ostream << "}\n";
  }
};

enum rb_tree_ranged_color_ { k_black_, k_red_ };

struct rb_tree_ranged_node_base_ {
  using base_ptr_ = rb_tree_ranged_node_base_ *;
  using const_base_ptr_ = const rb_tree_ranged_node_base_ *;
  using size_type = std::size_t;

  rb_tree_ranged_color_ m_color_;
  size_type m_size_;
  base_ptr_ m_left_;
  base_ptr_ m_right_;
  base_ptr_ m_parent_;

  static rb_tree_ranged_color_ get_color_(const_base_ptr_ p_x) { return (p_x ? p_x->m_color_ : k_black_); }

  const char *color_name() { return (m_color_ == k_black_ ? "black" : "red"); }

  static size_type size(base_ptr_ p_x) noexcept {
    return (p_x ? p_x->m_size_ : 0); // By definitions size of "NIL" node is 0;
  }

  static base_ptr_ minimum_(base_ptr_ p_x) noexcept {
    while (p_x->m_left_)
      p_x = p_x->m_left_;
    return p_x;
  }

  static const_base_ptr_ minimum_(const_base_ptr_ p_x) noexcept {
    while (p_x->m_left_)
      p_x = p_x->m_left_;
    return p_x;
  }

  static base_ptr_ maximum_(base_ptr_ p_x) noexcept {
    while (p_x->m_right_)
      p_x = p_x->m_right_;
    return p_x;
  }

  static const_base_ptr_ maximum_(const_base_ptr_ p_x) noexcept {
    while (p_x->m_right_)
      p_x = p_x->m_right_;
    return p_x;
  }

  static base_ptr_ successor_(base_ptr_ p_x) noexcept {
    if (p_x->m_right_) return minimum_(p_x->m_right_);
    return nullptr;
  }

  static const_base_ptr_ successor_(const_base_ptr_ p_x) noexcept {
    if (p_x->m_right_) return minimum_(p_x->m_right_);
    return nullptr;
  }

  static base_ptr_ predecessor_(base_ptr_ p_x) noexcept {
    if (p_x->m_left_) return maximum_(p_x->m_left_);
    return nullptr;
  }

  static const_base_ptr_ predecessor_(const_base_ptr_ p_x) noexcept {
    if (p_x->m_left_) return maximum_(p_x->m_left_);
    return nullptr;
  }

  static base_ptr_ get_sibling_(base_ptr_ p_x) noexcept {
    if (is_left_child_(p_x))
      return p_x->m_parent_->m_right_;
    else
      return p_x->m_parent_->m_left_;
  }

  static const_base_ptr_ get_sibling_(const_base_ptr_ p_x) noexcept {
    if (is_left_child_(p_x))
      return p_x->m_parent_->m_right_;
    else
      return p_x->m_parent_->m_left_;
  }

  static base_ptr_ get_uncle_(base_ptr_ p_x) noexcept {
    if (is_right_child_(p_x->m_parent_))
      return p_x->m_parent_->m_parent_->m_left_;
    else
      return p_x->m_parent_->m_parent_->m_right_;
  }

  static const_base_ptr_ get_uncle_(const_base_ptr_ p_x) noexcept {
    if (is_right_child_(p_x->m_parent_))
      return p_x->m_parent_->m_parent_->m_left_;
    else
      return p_x->m_parent_->m_parent_->m_right_;
  }

  static bool is_left_child_(const_base_ptr_ p_x) noexcept { return (p_x == p_x->m_parent_->m_left_); }

  static bool is_right_child_(const_base_ptr_ p_x) noexcept { return (p_x == p_x->m_parent_->m_right_); }

  static bool is_linear(const_base_ptr_ p_x) noexcept {
    return ((is_left_child_(p_x) && is_left_child_(p_x->m_parent_)) ||
            (is_right_child_(p_x) && is_right_child_(p_x->m_parent_)));
  }
};

template <typename t_value_type_> struct rb_tree_ranged_node_ : public rb_tree_ranged_node_base_ {
  using node_type_ = rb_tree_ranged_node_<t_value_type_>;
  using node_ptr_ = node_type_ *;
  using const_node_ptr_ = const node_type_ *;

  t_value_type_ m_value_;

  rb_tree_ranged_node_(const t_value_type_ &p_key)
      : rb_tree_ranged_node_base_{k_red_, 1}, m_value_{p_key} {};
};

class rb_tree_ranged_impl_ {
public:
  using base_ptr_ = rb_tree_ranged_node_base_::base_ptr_;
  using const_base_ptr_ = rb_tree_ranged_node_base_ ::const_base_ptr_;
  using link_type_ = rb_tree_ranged_node_base_;
  using size_type = rb_tree_ranged_node_base_::size_type;

protected:
  base_ptr_ m_root_;

  void rotate_left_(base_ptr_) noexcept;
  void rotate_right_(base_ptr_) noexcept;
  void rotate_to_parent_(base_ptr_) noexcept;

  void rebalance_after_insert_(base_ptr_) noexcept;
  void rebalance_after_erase_(base_ptr_) noexcept;

  rb_tree_ranged_impl_() : m_root_{} {}
};

template <typename t_value_type, typename t_comp> class rb_tree_ranged_ : public rb_tree_ranged_impl_ {
private:
  using node_type_ = rb_tree_ranged_node_<t_value_type>;
  using node_ptr_ = typename node_type_::node_ptr_;
  using const_node_ptr_ = typename node_type_::const_node_ptr_;

  using self_type_ = rb_tree_ranged_<t_value_type, t_comp>;
  using const_self_type_ = const self_type_;

public:
  bool empty() const noexcept { return !m_root_; }
  size_type size() const noexcept { return (m_root_ ? m_root_->m_size_ : 0); }
  bool contains(const t_value_type &p_key) const noexcept { return bst_lookup(p_key); }

private:
  void prune_leaf(node_ptr_ p_n) {
    if (!p_n->m_parent_) {
      m_root_ = nullptr;
    } else if (link_type_::is_left_child_(p_n)) {
      p_n->m_parent_->m_left_ = nullptr;
    } else {
      p_n->m_parent_->m_right_ = nullptr;
    }

    delete p_n;
  }

  template <typename F>
  std::tuple<node_ptr_, node_ptr_, bool> traverse_binary_search(node_ptr_ p_r, const t_value_type &p_key, F p_f) {
    if (!p_r) { return std::make_tuple(nullptr, nullptr, false); }

    node_ptr_ prev = nullptr;
    bool is_less_than_key{};

    while (p_r && (p_r->m_value_ != p_key)) {
      is_less_than_key = t_comp{}(p_r->m_value_, p_key);
      p_f(*p_r);
      prev = p_r;
      if (is_less_than_key) {
        p_r = static_cast<node_ptr_>(p_r->m_right_);
      } else {
        p_r = static_cast<node_ptr_>(p_r->m_left_);
      }
    }

    return std::make_tuple(p_r, prev, is_less_than_key);
  }

  template <typename F>
  std::tuple<const_node_ptr_, const_node_ptr_, bool> traverse_binary_search(const_node_ptr_ p_r,
                                                                            const t_value_type &p_key, F p_f) const {
    if (!p_r) { return std::make_tuple(nullptr, nullptr, false); }

    const_node_ptr_ prev = nullptr;
    bool is_less_than_key{};

    while (p_r && (p_r->m_value_ != p_key)) {
      is_less_than_key = t_comp{}(p_r->m_value_, p_key);
      p_f(*p_r);
      prev = p_r;
      if (is_less_than_key) {
        p_r = static_cast<node_ptr_>(p_r->m_right_);
      } else {
        p_r = static_cast<node_ptr_>(p_r->m_left_);
      }
    }

    return std::make_tuple(p_r, prev, is_less_than_key);
  }

  template <typename F> void traverse_postorder(base_ptr_ p_r, F p_f) {
    base_ptr_ prev{};

    while (p_r) {
      base_ptr_ parent{p_r->m_parent_}, left{p_r->m_left_}, right{p_r->m_right_};

      if (prev == parent) {
        prev = p_r;
        if (left) {
          p_r = left;
        } else if (right) {
          p_r = right;
        } else {
          p_f(p_r);
          p_r = parent;
        }
      }

      else if (prev == left) {
        prev = p_r;
        if (right) {
          p_r = right;
        } else {
          p_f(p_r);
          p_r = parent;
        }
      }

      else {
        prev = p_r;
        p_f(p_r);
        p_r = parent;
      }
    }
  }

  template <typename F> void traverse_postorder(const_base_ptr_ p_r, F p_f) const {
    const_base_ptr_ prev{};

    while (p_r) {
      const_base_ptr_ parent{p_r->m_parent_}, left{p_r->m_left_}, right{p_r->m_right_};

      if (prev == parent) {
        prev = p_r;
        if (left) {
          p_r = left;
        } else if (right) {
          p_r = right;
        } else {
          p_f(p_r);
          p_r = parent;
        }
      }

      else if (prev == left) {
        prev = p_r;
        if (right) {
          p_r = right;
        } else {
          p_f(p_r);
          p_r = parent;
        }
      }

      else {
        prev = p_r;
        p_f(p_r);
        p_r = parent;
      }
    }
  }

  node_ptr_ bst_lookup(const t_value_type &p_key) noexcept {
    auto [found, prev, is_prev_less] =
        traverse_binary_search(static_cast<node_ptr_>(m_root_), p_key, [](node_type_ &) {});
    return found;
  }

  const_node_ptr_ bst_lookup(const t_value_type &p_key) const noexcept {
    auto [found, prev, is_prev_less] =
        traverse_binary_search(static_cast<node_ptr_>(m_root_), p_key, [](const node_type_ &) {});
    return found;
  }

  node_ptr_ bst_insert(const t_value_type &p_key) {
    node_ptr_ to_insert = new node_type_{p_key};

    if (empty()) {
      to_insert->m_color_ = k_black_;
      m_root_ = to_insert;
      return to_insert;
    }

    auto [found, prev, is_prev_less] =
        traverse_binary_search(static_cast<node_ptr_>(m_root_), p_key, [](node_type_ &p_node) { p_node.m_size_++; });

    if (found) {
      traverse_binary_search(static_cast<node_ptr_>(m_root_), p_key, [](node_type_ &p_node) { p_node.m_size_--; });
      delete to_insert;
      throw std::invalid_argument("");
    }

    to_insert->m_parent_ = prev;
    if (is_prev_less) {
      prev->m_right_ = to_insert;
    } else {
      prev->m_left_ = to_insert;
    }

    return to_insert;
  }

  node_ptr_ move_to_leaf(const t_value_type &p_key) {
    node_ptr_ node = bst_lookup(p_key);

    if (!node) { throw std::invalid_argument(""); }

    while (node->m_right_ || node->m_left_) {
      node_ptr_ next =
          static_cast<node_ptr_>(node->m_right_ ? link_type_::successor_(node) : link_type_::predecessor_(node));
      std::swap(node->m_value_, next->m_value_);
      node = next;
    }

    return node;
  }

public:
  using value_type = t_value_type;
  using comp = t_comp;
  using size_type = typename node_type_::size_type;

  void insert(const t_value_type &p_key) {
    node_ptr_ leaf = bst_insert(p_key);
    if (!leaf->m_parent_ || !leaf->m_parent_->m_parent_) { return; }
    rebalance_after_insert_(leaf);
  }

  void erase(const t_value_type &p_key) {
    node_ptr_ leaf = move_to_leaf(p_key);
    rebalance_after_erase_(leaf);
    prune_leaf(leaf);
  }

  void clear() noexcept {
    // A more optimized version of iterative approach for postorder traversal. In this case node pointers can be used to
    // indicate which nodes have already been visited.
    base_ptr_ curr = m_root_;
    while (curr) {
      if (curr->m_left_)
        curr = curr->m_left_;
      else if (curr->m_right_)
        curr = curr->m_right_;

      else {
        base_ptr_ parent = curr->m_parent_;
        if (parent) {
          if (link_type_::is_left_child_(curr))
            parent->m_left_ = nullptr;
          else
            parent->m_right_ = nullptr;
        }
        delete curr;
        curr = parent;
      }
    }
  }

  void dump(std::ostream &p_ostream) {
    rb_tree_dumper__<node_type_>{}.dump(static_cast<node_ptr_>(m_root_), p_ostream);
  }

  rb_tree_ranged_() : rb_tree_ranged_impl_{} {}
  ~rb_tree_ranged_() { clear(); }

  rb_tree_ranged_(const_self_type_ &p_rhs) {
    self_type_ temp{};
    traverse_postorder(static_cast<const_node_ptr_>(p_rhs.m_root_),
                       [&](const_base_ptr_ p_n) { temp.insert(static_cast<const_node_ptr_>(p_n)->m_value_); });
    *this = std::move(temp);
  }

  self_type_ &operator=(const_self_type_ &p_rhs) {
    if (this != &p_rhs) {
      clear();
      traverse_postorder(static_cast<const_node_ptr_>(p_rhs.m_root_),
                         [&](const_base_ptr_ p_n) { insert(static_cast<const_node_ptr_>(p_n)->m_value_); });
    }
    return *this;
  }

  rb_tree_ranged_(self_type_ &&p_rhs) noexcept { std::swap(m_root_, p_rhs.m_root_); }

  self_type_ &operator=(self_type_ &&p_rhs) noexcept {
    if (this != &p_rhs) { std::swap(m_root_, p_rhs.m_root_); }
    return *this;
  }
};

} // namespace detail
} // namespace throttle