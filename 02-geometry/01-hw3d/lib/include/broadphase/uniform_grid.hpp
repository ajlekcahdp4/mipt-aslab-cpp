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
#include "equal.hpp"
#include "narrowphase/collision_shape.hpp"
#include "point3.hpp"

#include <list>
#include <unordered_map>
#include <vector>

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

  T                    m_cell_size;
  std::vector<t_shape> m_waiting_queue;
  map_t                m_map;

  std::optional<T> m_min_val, m_max_val;

public:
  using shape_type = t_shape;

  uniform_grid(T min_size) : m_cell_size{min_size} {}
  uniform_grid(T min_size, unsigned number_hint) : m_cell_size{min_size} { m_waiting_queue.reserve(number_hint); }

  void add_collision_shape(const shape_type &shape) {
    m_waiting_queue.push_back(shape);
    auto &bbox = shape.bounding_box();
    auto  bbox_max_corner = bbox.maximum_corner();
    auto  bbox_min_corner = bbox.minimum_corner();

    if (!m_min_val) { /* first insertion */
      m_min_val = vmin(bbox_min_corner.x, bbox_min_corner.y, bbox_min_corner.z);
      m_max_val = vmax(bbox_max_corner.x, bbox_max_corner.y, bbox_max_corner.z);
    } else {
      m_min_val = vmin(m_min_val, bbox_min_corner.x, bbox_min_corner.y, bbox_min_corner.z);
      m_max_val = vmax(m_max_val, bbox_max_corner.x, bbox_max_corner.y, bbox_max_corner.z);
    }
  }
};

} // namespace geometry
} // namespace throttle