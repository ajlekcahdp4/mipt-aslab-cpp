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
#include <functional>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace caches {

namespace detail {

template <typename T> class occurence_map_t {
  using map_t__ = typename std::unordered_map<T, std::deque<std::size_t>>;
  map_t__ m_map;

public:
  template<typename t_iterator> occurence_map_t(t_iterator p_begin, t_iterator p_end) : m_map{} {
    std::for_each(p_begin, p_end,
                  [this, i = 0](const T &element) mutable { m_map[element].push_back(i++); });
  }

  const T &find_latest_used(const std::unordered_set<T> &p_curr_set) {
    assert(p_curr_set.size());

    class set_comp__ {
      const map_t__ &m_map;

    public:
      set_comp__(const map_t__ &p_map) : m_map(p_map) {
      }

      bool operator()(const T &lhs, const T &rhs) {
        auto found_lhs = m_map.find(lhs);
        auto found_rhs = m_map.find(rhs);

        if (found_rhs == m_map.end()) {
          return true;
        }

        if (found_lhs == m_map.end()) {
          return false;
        }

        return found_lhs->second.front() < found_rhs->second.front();
      }
    };

    return *std::max_element(p_curr_set.begin(), p_curr_set.end(), set_comp__{m_map});
  }

  void erase_first(const T &p_elem) {
    assert(m_map.find(p_elem) != m_map.end());
    auto &[ignored_1, deq] = *m_map.find(p_elem);
    deq.pop_front();
    if (deq.empty()) {
      m_map.erase(p_elem);
    }
  }
};

template <typename T> class ideal_t {
  std::unordered_set<T> m_set;
  std::vector<T> m_vec;
  occurence_map_t<T> m_occur_map;
  std::size_t m_size, m_hits;

  bool is_full() const noexcept {
    return (m_set.size() == m_size);
  }

  void lookup_elem(const T &p_elem) {
    if (m_set.find(p_elem) != m_set.end()) {
      m_hits++;
    }

    else if (!is_full()) {
      m_set.insert(p_elem);
    }

    else {
      m_set.erase(m_occur_map.find_latest_used(m_set));
      m_set.insert(p_elem);
    }

    m_occur_map.erase_first(p_elem);
  }

public:
  template<typename t_iterator> ideal_t(std::size_t p_size, t_iterator p_begin, t_iterator p_end)
      : m_set{}, m_vec{}, m_occur_map{p_begin, p_end}, m_size{p_size}, m_hits{0} {
    std::copy(p_begin, p_end, std::back_inserter(m_vec));
    m_set.reserve(p_size);
  }

  std::size_t count_hits() {
    for (const auto &v : m_vec) {
      lookup_elem(v);
    }

    return m_hits;
  }
};

} // namespace detail

// Implementation of Belady's algorithm. Returns the number of maximum possible
// hits for a cache of size "size" and "vec" of requests.
template <typename T, typename t_iterator> std::size_t get_optimal_hits(std::size_t p_size, t_iterator p_begin, t_iterator p_end) {
  using namespace detail;

  if (!p_size || !std::distance(p_begin, p_end)) {
    throw std::invalid_argument{"get_optiomal_hits()"};
  }

  ideal_t<T> cache{p_size, p_begin, p_end};
  return cache.count_hits();
}

} // namespace caches