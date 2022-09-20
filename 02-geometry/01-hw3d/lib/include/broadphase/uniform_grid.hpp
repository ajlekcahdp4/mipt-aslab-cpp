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
#include <array>
#include <cmath>
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
  return point3<int>{std::floor(vec.x), std::floor(vec.y), std::floor(vec.z)};
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

  using cell_type = int_point_type;

  struct cell_hash {
    std::size_t operator()(const cell_type &cell) const {
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
  using stored_shapes_elem_t = typename std::pair<t_shape, cell_type>;
  std::vector<stored_shapes_elem_t> m_stored_shapes;

  /* A list of indexes of shapes in m_stored_shapes */
  using shape_idx_vec_t = typename std::vector<index_t>;
  /* map cell into shape_list_t */
  using map_t = typename std::unordered_map<cell_type, shape_idx_vec_t, cell_hash>;
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
    if (is_definitely_greater(max_width, m_cell_size)) m_cell_size = 2 * max_width;

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

    std::transform(m_stored_shapes.begin(), m_stored_shapes.end(), std::back_inserter(m_waiting_queue),
                   [&](const auto &pair) { return pair.first; });
    m_stored_shapes.clear();

    /* insert all the new shapes into the grid */
    while (!m_waiting_queue.empty()) {
      insert(m_waiting_queue.back());
      m_waiting_queue.pop_back();
    }

    m_waiting_queue.clear();
  }

private:
  /* insert shape into m_stored shapes and into m_map */
  void insert(const shape_type &shape) {
    auto old_stored_size = m_stored_shapes.size();
    auto cell = compute_cell(shape);

    m_stored_shapes.emplace_back(shape, cell);

    m_map[cell].push_back(old_stored_size);
  }

  cell_type compute_cell(const shape_type &shape) const {
    auto bbox = shape.bounding_box();
    auto min_corner = bbox.m_center - point_type::origin();
    return convert_to_int_point(min_corner / m_cell_size);
  }

  static constexpr std::array<vec3<int>, 27> offsets() {
    return std::array<vec3<int>, 27>{{{0, 0, 0},   {0, 0, 1},  {0, 0, -1},  {0, 1, 0},   {0, -1, 0},   {1, 0, 0},
                                      {-1, 0, 0},  {0, 1, -1}, {0, 1, 1},   {0, -1, 1},  {0, -1, -1},  {1, 0, 1},
                                      {1, 0, -1},  {-1, 0, 1}, {-1, 0, -1}, {1, 1, 1},   {1, 1, -1},   {1, -1, 1},
                                      {1, -1, -1}, {-1, 1, 1}, {-1, 1, -1}, {-1, -1, 1}, {-1, -1, -1}, {-1, -1, 0},
                                      {-1, 1, 0},  {1, -1, 0}, {1, 1, 0}}};
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
      for (auto &bucket : map) {
        auto offsets_a = offsets();
        /* all the neighbors */
        for (auto &offset : offsets_a) {
          auto bucket_to_test_with = map[bucket.first + offset];
          /*for all of the shapes they containes to intersect */
          for (auto to_test_idx : bucket.second)
            for (auto to_test_with_idx : bucket_to_test_with)
              if (to_test_idx != to_test_with_idx &&
                  stored_shapes[to_test_idx].first.collide(stored_shapes[to_test_with_idx].first)) {
                in_collision.insert(to_test_idx);
                in_collision.insert(to_test_with_idx);
              }
        }
      }
    };
  };
};

} // namespace geometry
} // namespace throttle