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
#include "vec3.hpp"

#include <algorithm>
#include <list>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include <boost/functional/hash.hpp>

namespace throttle {
namespace geometry {

template <typename T> point3<int> convert_to_int_point(const vec3<T> &vec) {
  return point3<int>{static_cast<int>(vec.x), static_cast<int>(vec.y), static_cast<int>(vec.z)};
}

template <typename T, typename t_shape = collision_shape<T>,
          typename = std::enable_if_t<std::is_base_of_v<collision_shape<T>, t_shape>>>
class uniform_grid : broadphase_structure<uniform_grid<T>, t_shape> {

  using shape_ptr = t_shape *;
  using int_point_type = point3<int>;
  using point_type = point3<T>;
  using vector_type = vec3<T>;
  using int_vector_type = vec3<int>;
  using index_t = unsigned;

  using cell = int_point_type;

  struct cell_hash {
    std::size_t operator()(const cell &cell) const {
      std::size_t seed{};

      boost::hash_combine(seed, cell.x);
      boost::hash_combine(seed, cell.y);
      boost::hash_combine(seed, cell.z);

      return seed;
    }
  };

  // grid's cells size
  T m_cell_size;

  // queue of shapes to insert
  std::vector<t_shape> m_waiting_queue;

  // The vector of inserted elements and vectors from all the cells that the element overlaps
  using stored_shapes_elem_t = typename std::pair<t_shape, std::vector<cell>>;
  std::vector<stored_shapes_elem_t> m_stored_shapes;

  /* A list of indexes of shapes in m_stored_shapes */
  using shape_list_t = typename std::list<index_t>;
  /* map cell into shape_list_t */
  using map_t = typename std::unordered_map<cell, shape_list_t, cell_hash>;
  map_t m_map;

  // minimum and maximum values of the bounding box coordinates
  std::optional<T> m_min_val, m_max_val;

public:
  using shape_type = t_shape;

  // ctor with hint about the number of shapes to insert
  uniform_grid(index_t number_hint) {
    m_waiting_queue.reserve(number_hint);
    m_stored_shapes.reserve(number_hint);
  }

  void add_collision_shape(const shape_type &shape) {
    m_waiting_queue.push_back(shape);
    auto bbox = shape.bounding_box();
    auto bbox_max_corner = bbox.maximum_corner();
    auto bbox_min_corner = bbox.minimum_corner();
    auto max_width = bbox.max_width();

    /* remain cell size to be large enough to fit the largest shape in any rotation */
    if (is_definitely_greater(max_width, m_cell_size)) m_cell_size = max_width;

    if (!m_min_val) { /* first insertion */
      m_min_val = vmin(bbox_min_corner.x, bbox_min_corner.y, bbox_min_corner.z);
      m_max_val = vmax(bbox_max_corner.x, bbox_max_corner.y, bbox_max_corner.z);
    } else {
      m_min_val = vmin(m_min_val.value(), bbox_min_corner.x, bbox_min_corner.y, bbox_min_corner.z);
      m_max_val = vmax(m_max_val.value(), bbox_max_corner.x, bbox_max_corner.y, bbox_max_corner.z);
    }
  }

  std::vector<shape_ptr> many_to_many() {
    rebuild();

    many_to_many_collider collider(m_map, m_stored_shapes);
    collider.collide();

    std::vector<shape_ptr> result;
    std::transform(collider.in_collision.begin(), collider.in_collision.end(), std::back_inserter(result),
                   [&](const auto &idx) { return std::addressof(m_stored_shapes[idx].first); });
    return result;
  }

  void rebuild() {
    m_map.clear();

    /* re-insert all old elements into the grid */
    auto size = m_stored_shapes.size();
    for (index_t idx = 0; idx < size; idx++) {
      m_stored_shapes[idx].second.clear();

      insert(idx);
    }

    /* insert all the new shapes into the grid */
    for (index_t idx = 0; idx < size; idx++) {
      m_stored_shapes.push_back(stored_shapes_elem_t{m_stored_shapes[idx].first, std::vector<cell>{}});

      insert(idx);
    }

    m_waiting_queue.clear();
  }

private:
  /*
   * find all cells the shape overlaps and insert shape's index into
   * the corresponding lists.
   */
  void insert(index_t idx) {
    /* unordered set is there used for storing unique cells */
    std::unordered_set<int_point_type, cell_hash> cells;
    find_all_cells_shape_overlaps(cells, idx);

    for (auto &cell : cells) {
      m_stored_shapes[idx].second.push_back(cell);
      m_map[cell].push_back(idx);
    }
  }

  // fill "cells" with unique cells that "idx"th shape overlapse.
  void find_all_cells_shape_overlaps(std::unordered_set<int_point_type, cell_hash> &cells, const index_t idx) const {
    auto          &shape = m_stored_shapes[idx].first;
    auto           bbox = shape.bounding_box();
    auto           min_corner = bbox.minimum_corner() - point_type::origin();
    std::vector<T> widths = {2 * bbox.m_halfwidth_x, 2 * bbox.m_halfwidth_y, 2 * bbox.m_halfwidth_x};

    cells.insert(convert_to_int_point(min_corner / m_cell_size));

    cells.insert(convert_to_int_point((min_corner + vector_type{widths[0], 0, 0}) / m_cell_size));
    cells.insert(convert_to_int_point((min_corner + vector_type{0, widths[1], 0}) / m_cell_size));
    cells.insert(convert_to_int_point((min_corner + vector_type{0, 0, widths[2]}) / m_cell_size));

    cells.insert(convert_to_int_point((min_corner + vector_type{0, widths[1], widths[2]}) / m_cell_size));
    cells.insert(convert_to_int_point((min_corner + vector_type{widths[0], 0, widths[2]}) / m_cell_size));
    cells.insert(convert_to_int_point((min_corner + vector_type{widths[0], widths[1], 0}) / m_cell_size));

    cells.insert(convert_to_int_point((min_corner + vector_type{widths[0], widths[1], widths[2]}) / m_cell_size));
  }

  struct many_to_many_collider {
    std::set<index_t> in_collision;

    map_t &map;

    const std::vector<stored_shapes_elem_t> &stored_shapes;

    many_to_many_collider(map_t &map_a, const std::vector<stored_shapes_elem_t> &stored_shapes_a)
        : map(map_a), stored_shapes(stored_shapes_a) {}

    // fills "in_collision" set with all intersecting shapes in the grid
    void collide() {
      /* For each cell we will test */
      for (auto &bucket : map)
        /* each shape */
        for (auto &to_test_idx : bucket.second)
          /* with all the cell it overlaps  */
          for (auto &cell : stored_shapes[to_test_idx].second)
            /* for intersection with all shapes they contains */
            for (auto &to_test_with_idx : map[cell])
              if (to_test_with_idx != to_test_idx &&
                  stored_shapes[to_test_idx].first.collide(stored_shapes[to_test_with_idx].first)) {
                in_collision.insert(to_test_idx);
                in_collision.insert(to_test_with_idx);
              }
    };
  };
};

} // namespace geometry
} // namespace throttle