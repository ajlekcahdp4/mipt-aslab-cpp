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

template <typename K, typename U> class local_node_t {
  K m_key;
  U m_value;

public:
  local_node_t(K p_key, U p_val) : m_key{p_key}, m_value{p_val} {
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

template <typename K, typename U> class local_list_t {
public:
  using it = typename std::list<local_node_t<K, U>>::iterator;
  using const_it = typename std::list<local_node_t<K, U>>::const_iterator;

private:
  std::list<local_node_t<K, U>> m_list;
  std::unordered_map<K, it> m_map;

public:
  local_list_t() : m_list{} {
  }

  bool is_empty() const {
    return m_list.empty();
  }

  void push_front(K p_key, U p_val) {
    m_list.emplace_front(p_key, p_val);
    m_map.insert({p_key, m_list.begin()});
  }

  void splice_upfront(local_list_t<K, U> &p_other, const_it p_elem) {
    p_other.m_map.erase(p_elem->key());
    m_list.splice(m_list.begin(), p_other.m_list, p_elem);
    m_map.insert({p_elem->key(), m_list.begin()});
  }

  it lookup(const K p_key) const {
    auto found = m_map.find(p_key);
    // An element with key p_key will always be contained in a corresponding
    // local list.
    assert(found != m_map.end());

    return found->second;
  }

  it last() {
    // Local lists should never be empty.
    assert(!is_empty());
    return std::prev(m_list.end());
  }
};

template <typename K, typename U, typename W> class freq_node_t {
public:
  using local_t = local_list_t<K, U>;

private:
  local_t m_local;
  W m_weight;

public:
  freq_node_t(W p_weight) : m_local{}, m_weight{p_weight} {
  }

  const local_t &local() const {
    return m_local;
  }

  local_t &local() {
    return m_local;
  }

  const W &weight() const {
    return m_weight;
  }
};

template <typename K, typename U, typename W> class freq_list_t {
  using freq_t = freq_node_t<K, U, W>;

public:
  using it = typename std::list<freq_t>::iterator;

private:
  std::list<freq_t> m_list;

  bool is_last(it p_iter) const {
    return (std::next(p_iter) == m_list.end());
  }

public:
  freq_list_t() : m_list{} {
  }

  it first_weight_node() {
    it front = m_list.begin();

    if (front->weight() == 1) {
      return front;
    }

    m_list.emplace_front(1);
    return m_list.begin();
  }

  it least_weight_node() {
    return m_list.begin();
  }

  // This function creates a "frequency node" after "p_prev" with
  // "p_prev->weight() + 1" key in case it does not exists. Otherwise and
  // returns an incremented iterator.
  it next_weight_node(it p_prev) {
    if (is_last(p_prev)) {
      m_list.emplace_back(p_prev->weight() + 1);
      return std::next(p_prev);
    }

    it next_it = std::next(p_prev);
    W prev_weight = p_prev->weight(), next_weight = prev_weight + 1;

    if (next_it->weight() == next_weight) {
      return next_it;
    } else {
      return m_list.insert(next_it, next_weight);
    }
  }

  void remove_if_empty(it p_it) {
    if (p_it->local().is_empty()) {
      m_list.erase(p_it);
    }
  }
};
} // namespace detail

template <typename U, typename K = int, typename hash = std::hash<K>>
class lfu_t {
  std::size_t m_size, m_hits, m_curr;
  std::unordered_map<K, typename detail::freq_list_t<K, U, std::size_t>::it>
      m_weight_map;
  detail::freq_list_t<K, U, std::size_t> m_freq_list;

  U promote_element(K p_key) {
    auto found = m_weight_map.find(p_key);
    assert(found != m_weight_map.end());

    auto freq_it = found->second;
    auto next_it = m_freq_list.next_weight_node(freq_it);

    found->second = next_it;

    auto elem_it = freq_it->local().lookup(p_key);
    next_it->local().splice_upfront(freq_it->local(), elem_it);

    m_freq_list.remove_if_empty(freq_it);

    return elem_it->value();
  }

public:
  lfu_t(std::size_t p_size)
      : m_size{p_size}, m_hits{0}, m_curr{0}, m_weight_map{}, m_freq_list{} {
  }

  bool is_full() const {
    return (m_curr == m_size);
  }

  std::size_t get_hits() const {
    return m_hits;
  }

  template <typename F> U lookup(K p_key, F p_slow_get) {
    auto found = m_weight_map.find(p_key);

    // Case 1. The entry is present in the cache. Then it gets promoted.
    if (found != m_weight_map.end()) {
      m_hits++;
      return promote_element(p_key);
    }

    U val = p_slow_get();

    // Case 2. If the entry is not present at the moment, and cache is not full.
    // Then it gets inserted into a frequency bucket with weight "1" and pushed
    // into the beginning of the corresponding frequency list;
    if (!is_full()) {
      auto first = m_freq_list.first_weight_node();
      first->local().push_front(p_key, val);
      m_weight_map.insert({p_key, first});
      m_curr++;
    }

    // Case 3. If we get here, then the cache is full and we need to choose an
    // entry to evict, that the new entry will replace.
    else {
      // First we choose the "to_evict" entry by looking up the tail of a local
      // list with the least weight.
      auto least = m_freq_list.least_weight_node();
      auto to_evict = least->local().last();

      // Erase the entry from key-weight map.
      m_weight_map.erase(to_evict->key());
      // Clean up the frequency list, if "to_evict" was the only element in the
      // local list.
      m_freq_list.remove_if_empty(least);

      // Reuse the local list node.
      to_evict->key() = p_key;
      to_evict->value() = val;

      // Move the now evicted node to the bucket with weight "1".
      m_freq_list.first_weight_node()->local().splice_upfront(least->local(),
                                                              to_evict);

      // Insert the new entry into the key-weight map.
      m_weight_map.insert({p_key, m_freq_list.least_weight_node()});
    }

    return val;
  }
};

} // namespace caches