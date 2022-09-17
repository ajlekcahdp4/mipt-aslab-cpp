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

#include <vector>

namespace throttle {
namespace geometry {

template <typename T, typename t_shape = collision_shape<T>,
          typename = std::enable_if_t<std::is_base_of_v<collision_shape<T>, t_shape>>>
class bruteforce : broadphase_structure<bruteforce<T>, t_shape> {
  std::vector<t_shape> m_stored_shapes;

public:
  using shape_type = t_shape;

  void add_collison_shape(const shape_type &shape) { m_stored_shapes.push_back(shape); }
};

} // namespace geometry
} // namespace throttle