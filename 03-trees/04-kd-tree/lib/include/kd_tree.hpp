/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <tsimmerman.ss@phystech.edu>, wrote this file.  As long as you
 * retain this notice you can do whatever you want with this stuff. If we meet
 * some day, and you think this stuff is worth it, you can buy me a beer in
 * return.
 * ----------------------------------------------------------------------------
 */

#pragma once

#include <cstddef>
#include <utility>

#include <range/v3/action.hpp>
#include <range/v3/algorithm.hpp>
#include <range/v3/iterator.hpp>
#include <range/v3/numeric.hpp>
#include <range/v3/view.hpp>

#include <variant>
#include <vector>

#include <type_traits>

#include "point_n.hpp"
#include "vec_n.hpp"

namespace throttle {

template <typename t_point, std::size_t N> class kd_tree {
  using size_type = std::size_t;
  using value_type = typename t_point::value_type;

  using point_type = t_point;
  using vec_type = vec_n<value_type, N>;

private:
  using kd_tree_node = std::variant<std::monostate, point_type, std::vector<point_type>>;

  std::vector<point_type>   m_pending_insertion;
  std::vector<kd_tree_node> m_tree_structure;

public:
  static constexpr std::size_t dimension = N;
  static constexpr size_type   max_leaf_capacity = 2;

  kd_tree() = default;
  kd_tree(size_type depth_hint) : m_tree_structure{size_type{1} << depth_hint} {}

  void insert(const point_type &point) { m_pending_insertion.push_back(point); }

public:
  static bool      is_root(size_type index) { return (index == 1); }
  static size_type left_child(size_type index) { return 2 * index; }
  static size_type right_child(size_type index) { return 2 * index + 1; }
  static size_type parent(size_type index) { return index / 2; }

  kd_tree_node &node_at_index(size_type index) {
    if (index >= m_tree_structure.size()) m_tree_structure.resize(index + 1);
    return m_tree_structure.at(index);
  }

  void construct(auto &&node_range, size_type depth = 0, size_type curr_index = 1) {
    auto size = ranges::size(node_range);

    if (size <= max_leaf_capacity) {
      node_at_index(curr_index) = ranges::to_vector(node_range);
      return;
    }

    size_type axis = depth % dimension;
    auto      median_index = size / 2;

    auto sorted_range = ranges::to_vector(node_range);
    sorted_range |= ranges::actions::sort([axis](auto &&i, auto &&j) { return i[axis] < j[axis]; });
    auto left_range = sorted_range | ranges::views::slice(size_type{0}, median_index);
    auto right_range = sorted_range | ranges::views::slice(median_index + 1, ranges::end);

    node_at_index(curr_index) = sorted_range[median_index];
    construct(left_range, depth + 1, left_child(curr_index));
    construct(right_range, depth + 1, right_child(curr_index));
  }

  template <class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
  template <class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

  void move_all_to_pending() {
    // clang-format off
    for (const auto &v : m_tree_structure) {
      std::visit(overloaded {
        [&](const std::vector<point_type> &vec) -> void { ranges::copy(vec, ranges::back_inserter(m_pending_insertion)); },
        [&](const point_type &point) -> void { m_pending_insertion.push_back(point); },
        [](auto) -> void {}
      }, v);
    }
    // clang-format on
    m_tree_structure.clear();
  }

  void reconstruct() {
    if (m_pending_insertion.empty()) return;
    move_all_to_pending();
    construct(m_pending_insertion);
    m_pending_insertion.clear();
  }
};

} // namespace throttle