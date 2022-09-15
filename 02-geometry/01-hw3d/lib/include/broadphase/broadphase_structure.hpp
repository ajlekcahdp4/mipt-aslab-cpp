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

namespace throttle {
namespace geometry {

template <typename t_derived> class broadphase_structure {
  using derived_ptr = t_derived *;
  using const_derived_ptr = const t_derived *;

  derived_ptr impl() {
    return static_cast<derived_ptr>(this);
  }

  const_derived_ptr impl() const {
    return static_cast<const_derived_ptr>(this);
  }

public:
  void add_collison_shape() {
    impl()->add_collision_shape();
  }
  //
};

} // namespace geometry
} // namespace throttle