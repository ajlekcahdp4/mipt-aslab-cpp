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

#include <cassert>
#include <functional>
#include <iostream>
#include <iterator>
#include <list>
#include <map>
#include <unordered_map>
#include <vector>

namespace caches {

namespace detail {

template <typename U, typename K> class local_node_t {
  U m_value;
  K m_key;

public:
  local_node_t(U p_val, K p_key) : m_value{p_val}, m_key{m_key} {
  }

  const U &value() const {
    return m_value;
  }

  U &value() {
    return m_value;
  }

  const K &key() const {
    return m_key;
  }

  K &key() {
    return m_key;
  }
};

template <typename U, typename K> class local_list_t {
  std::list<local_node_t<U, K>> m_list;

  local_list_t() : m_list{} {
  }

public:
  using it = typename std::list<local_node_t<U, K>>::iterator;

  bool is_empty() const {
    return m_list.empty();
  }
};

template <typename U, typename K> class freq_list_t {
  using freq_node_t = typename std::pair<unsigned, local_list_t<U, K>>;

public:
  using it = typename std::list<freq_node_t>::iterator;

private:
  std::list<freq_node_t> m_list;

  bool is_last(it p_iter) const {
    return (std::next(p_iter) == m_list.end());
  }

public:
  freq_list_t() : m_list{} {
  }
};
} // namespace detail

template <typename U, typename K = int, typename hash = std::hash<K>>
class lfu_t {
  std::size_t m_size, m_hits;

  std::unordered_map<K, U> m_value_map;
  std::unordered_map<K, typename detail::freq_list_t<U, K>::it> m_key_map;

  detail::freq_list_t<U, K> m_freq_list;

  void promote_element(K p_key) {
    auto found = m_key_map.find(p_key);

    assert(found != m_key_map.end());

    auto old_it = found->second;
    if (m_freq_list.is_last(old_it)) {
    }
  }

public:
  lfu_t(std::size_t p_size)
      : m_size{p_size}, m_hits{0}, m_value_map{}, m_key_map{}, m_freq_list{} {
  }

  bool is_full() const {
    return (m_value_map.size() == m_size);
  }

  template <typename F> U lookup(K p_key, F p_slow_get) {
    auto found = m_value_map.find(p_key);

    if (found != m_value_map.end()) {
    }
  }
};

} // namespace caches