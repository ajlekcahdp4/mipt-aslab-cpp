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
#include <stdexcept>
#include <utility>

#include <range/v3/action.hpp>
#include <range/v3/algorithm.hpp>
#include <range/v3/iterator.hpp>
#include <range/v3/numeric.hpp>
#include <range/v3/view.hpp>

#include <optional>
#include <type_traits>
#include <variant>
#include <vector>

#include "point_n.hpp"
#include "vec_n.hpp"

namespace throttle {

template <typename t_point>
requires std::is_base_of_v<point_n<typename t_point::value_type, t_point::dimension>, t_point> &&
    std::is_nothrow_copy_assignable_v<t_point>
class kd_tree {
  using size_type = std::size_t;
  using value_type = typename t_point::value_type;

  using point_type = t_point;

private:
  using kd_tree_node = std::variant<std::monostate, value_type, std::vector<point_type>>;

  std::vector<point_type>   m_pending_insertion;
  std::vector<kd_tree_node> m_tree_structure;
  size_type                 m_stored_count = 0;

public:
  static constexpr std::size_t dimension = t_point::dimension;
  static constexpr size_type   max_leaf_capacity = 8;

  kd_tree() = default;

  void      insert(const point_type &point) { m_pending_insertion.push_back(point); }
  size_type size() const { return m_pending_insertion.size() + m_stored_count; }
  bool empty() const { return (size() == 0); }
private:
  static bool      is_root(size_type index) { return (index == 1); }
  static size_type left_child(size_type index) { return 2 * index; }
  static size_type right_child(size_type index) { return 2 * index + 1; }
  static size_type parent(size_type index) { return index / 2; }

  kd_tree_node &node_at_index(size_type index) {
    if (index >= m_tree_structure.size()) m_tree_structure.resize(index);
    return m_tree_structure.at(index - 1);
  }

  const kd_tree_node &node_at_index(size_type index) const { return m_tree_structure.at(index - 1); }

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
    auto right_range = sorted_range | ranges::views::slice(median_index, ranges::end);

    node_at_index(curr_index) = sorted_range[median_index][axis];
    construct(left_range, depth + 1, left_child(curr_index));
    construct(right_range, depth + 1, right_child(curr_index));
  }

private:
  std::pair<point_type, value_type> nearest_neighbour_impl(const point_type          current_best,
                                                           std::optional<value_type> current_dist,
                                                           const point_type &query_point, size_type depth = 0,
                                                           size_type curr_index = 1) const {
    const auto &current_node = node_at_index(curr_index);

    if (std::holds_alternative<std::vector<point_type>>(current_node)) {
      auto [best_distance, best_point] = compute_closest(std::get<std::vector<point_type>>(current_node), query_point);

      if (!current_dist || best_distance < current_dist.value()) return std::make_pair(best_point, best_distance);
      return std::make_pair(current_best, current_dist.value());
    }

    size_type axis = depth % dimension;

    auto split_val = std::get<value_type>(current_node);
    auto dist_sq = (split_val - query_point[axis]) * (split_val - query_point[axis]);

    auto first_node = (query_point[axis] < split_val ? left_child(curr_index) : right_child(curr_index));
    auto second_node = (query_point[axis] < split_val ? right_child(curr_index) : left_child(curr_index));

    auto [subtree_best_point, subtree_best_dist] =
        nearest_neighbour_impl(current_best, current_dist, query_point, depth + 1, first_node);

    if (subtree_best_dist >= dist_sq) {
      return nearest_neighbour_impl(subtree_best_point, subtree_best_dist, query_point, depth + 1, second_node);
    }

    return std::make_pair(subtree_best_point, subtree_best_dist);
  }

public:
  point_type nearest_neighbour(const point_type &point) {
    if (empty()) throw std::logic_error{"Calling nearest neighbour on an empty kd-tree"};
    reconstruct();
    return nearest_neighbour_impl(point_type{}, std::nullopt, point).first;
  }

  template <class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
  template <class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

  void copy_all_to_pending() {
    // clang-format off
    for (const auto &v : m_tree_structure) {
      std::visit(overloaded {
        [&](const std::vector<point_type> &vec) -> void { ranges::copy(vec, ranges::back_inserter(m_pending_insertion)); },
        [](auto) -> void {}
      }, v);
    }
    // clang-format on
    m_tree_structure.clear();
  }

  void reconstruct() {
    if (m_pending_insertion.empty()) return;

    copy_all_to_pending();
    try {
      construct(m_pending_insertion);
    } catch (...) {
      m_tree_structure.clear();
      throw;
    }

    m_stored_count = m_pending_insertion.size();
    m_pending_insertion.clear();
  }
};

} // namespace throttle