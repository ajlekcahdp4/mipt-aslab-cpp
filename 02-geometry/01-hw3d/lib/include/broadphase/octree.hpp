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

#include "broadphase_structure.hpp"
#include "narrowphase/collision_shape.hpp"

#include <algorithm>
#include <queue>
#include <set>
#include <vector>

namespace throttle {
namespace geometry {

template <typename T, typename t_shape = collision_shape<T>,
          typename = std::enable_if_t<std::is_base_of_v<collision_shape<T>, t_shape>>>
class octree : broadphase_structure<bruteforce<T>, t_shape> {
  using shape_ptr = t_shape *;
  std::vector<t_shape> m_stored_shapes;
  std::queue<t_shape>  m_waiting_queue;

  unsigned m_max_depth;

  struct octree_node {};

public:
  using shape_type = t_shape;

  octree() = default;
  octree(unsigned number_hint) { m_stored_shapes.reserve(number_hint); }

  void add_collision_shape(const shape_type &shape) {}
  void rebuid() { return; }
  std::vector<shape_ptr> many_to_many() {}
};

} // namespace geometry
} // namespace throttle