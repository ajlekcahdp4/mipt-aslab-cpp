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

namespace throttle {
namespace geometry {

template <typename T> class bruteforce : broadphase_structure<bruteforce<T>> {

};

} // namespace geometry
} // namespace throttle