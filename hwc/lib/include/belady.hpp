/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <tsimmerman.ss@phystech.edu>, wrote this file.  As long as you
 * retain this notice you can do whatever you want with this stuff. If we meet
 * some day, and you think this stuff is worth it, you can buy us a beer in
 * return.
 * ----------------------------------------------------------------------------
 */

#pragma once

#include <algorithm>
#include <cassert>
#include <deque>
#include <iostream>
#include <limits>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace caches {

namespace detail {

template <typename U> class occurence_deque_t {
  std::deque<U> m_deque;

public:
  occurence_deque_t() : m_deque{} {
  }

  void push_back(const U p_val) {
    m_deque.push_back(p_val);
  }

  U first_used() const {
    return m_deque.front();
  }

  bool is_empty() const {
    return m_deque.empty();
  }

  void erase_first() {
    m_deque.pop_front();
  }
};

template <typename T> class occurence_map_t {
  std::unordered_map<T, occurence_deque_t<std::size_t>> m_map;

public:
  occurence_map_t(const std::vector<T> &p_vec) : m_map{} {
    for (std::size_t si = 0, ei = p_vec.size(); si != ei; ++si) {
      const T &curr = p_vec[si];
      m_map[curr].push_back(si + 1);
    }
  }

  const T &find_latest_used(const std::unordered_set<T> &p_curr_set) {
    std::size_t latest_occur = 0;
    const T *latest = nullptr;

    for (const auto &s : p_curr_set) {
      const auto found = m_map.find(s);

      if (found == m_map.end()) {
        return s;
      }

      occurence_deque_t<std::size_t> &deq = (found->second);
      if (deq.first_used() > latest_occur) {
        latest_occur = deq.first_used();
        latest = &s;
      }
    }

    // Assert should not fail in normal cases
    assert(latest);

    return *latest;
  }

  void erase_first(const T &p_elem) {
    auto found = m_map.find(p_elem);

    if (found == m_map.end()) {
      return;
    }

    occurence_deque_t<std::size_t> &deq = found->second;
    deq.erase_first();

    if (deq.is_empty()) {
      m_map.erase(p_elem);
    }
  }

  bool is_empty() const {
    return m_map.empty();
  }
};

template <typename T> class ideal_t {
  std::unordered_set<T> m_set;
  const std::vector<T> &m_vec;
  occurence_map_t<T> m_occur_map;
  std::size_t m_size, m_hits;

public:
  ideal_t(std::size_t p_size, const std::vector<T> &p_vec)
      : m_set{}, m_vec{p_vec}, m_occur_map{p_vec}, m_size{p_size}, m_hits{0} {
    m_set.reserve(p_size);
  }

  bool is_full() const {
    return (m_set.size() == m_size);
  }

  void lookup_elem(const T &p_elem) {
    assert(m_set.size() <= m_size);

    if (m_set.find(p_elem) != m_set.end()) {
      m_hits++;
    }

    else if (!is_full()) {
      m_set.emplace(p_elem);
    }

    else {
      const T &latest = m_occur_map.find_latest_used(m_set);
      m_set.erase(latest);
      m_set.emplace(p_elem);
    }

    m_occur_map.erase_first(p_elem);
  }

  std::size_t count_hits() {
    for (const auto &v : m_vec) {
      lookup_elem(v);
    }

    assert(m_occur_map.is_empty());
    return m_hits;
  }
};

} // namespace detail

template <typename T>
std::size_t get_optimal_hits(std::size_t size, std::vector<T> &vec) {
  using namespace detail;

  ideal_t<T> cache{size, vec};
  return cache.count_hits();
}

}; // namespace caches