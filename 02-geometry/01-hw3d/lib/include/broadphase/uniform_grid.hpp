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
  struct cell {
    point3<T> point_t m_minimum_corner;
  };

  using cell_type = cell<T>;

  struct cell_hash {
    // compute hash bucket index in range [0, NUM_BUCKETS-1]
    int operator()(const cell_type &cell) {
      const int h1 = 0x8da6b343; // Large multiplication constants;
      const int h2 = 0xd8163841; // here arbirarily chosen primes
      const int h3 = 0xcb1ab31f;

      int hash = h1 * cell.x + h2 * cell.y + h3 * cell.z;
      return hash;
    }
  };

  using list_t = typename std::list<t_shape>;
  using map_t = typename std::unordered_map<cell, list_t, cell_hash>;

  T                    m_cell_size;
  std::vector<t_shape> m_waiting_queue;
  map_t                m_map;

  std::optional<T> m_min_val, m_max_val;

public:
  using shape_type = t_shape;

  uniform_grid(unsigned number_hint) { m_waiting_queue.reserve(number_hint); }

  void add_collision_shape(const shape_type &shape) {
    m_waiting_queue.push_back(shape);
    auto &bbox = shape.bounding_box();
    auto  bbox_max_corner = bbox.maximum_corner();
    auto  bbox_min_corner = bbox.minimum_corner();
    auto  max_width = bbox.max_width();

    /* remain cell size to be large enough to fit the largest shape in any rotation */
    if (is_definitely_greater(max_width, m_cell_size)) m_cell_size = max_width;

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