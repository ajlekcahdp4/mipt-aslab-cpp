/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <tsimmerman.ss@phystech.edu>, <alex.rom23@mail.ru> wrote this file.  As long as you
 * retain this notice you can do whatever you want with this stuff. If we meet
 * some day, and you think this stuff is worth it, you can buy me a beer in
 * return.
 * ----------------------------------------------------------------------------
 */

#pragma once

#include "broadphase_structure.hpp"
#include "narrowphase/collision_shape.hpp"
#include "point3.hpp"

#include <list>
#include <unordered_map>

namespace throttle {
namespace geometry {

template <typename T, typename t_shape = collision_shape<T>,
          typename = std::enable_if_t<std::is_base_of_v<collision_shape<T>, t_shape>>>
class uniform_grid : broadphase_structure<uniform_grid<T>, t_shape> {

  using list_t = typename std::list<t_shape>;
  using map_t = typename std::unordered_map<cell, list_t>;

  struct cell {
    point3<T> point_t m_minimum_corner;
  };

  std::size_t m_cell_size;
  map_t       m_map;

public:
  using shape_type = t_shape;
};

} // namespace geometry
} // namespace throttle