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
#include <deque>
#include <functional>
#include <iostream>
#include <iterator>
#include <list>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace caches {

namespace detail {

template <typename T> class occurence_map_t {
  std::unordered_map<T, std::deque<std::size_t>> m_map;

public:
  occurence_map_t(const std::vector<T> &p_vec) : m_map{} {
    for (std::size_t si = 0, ei = p_vec.size(); si != ei; ++si) {
      // Fill up corresponding vectors with occurence number in sorted order.
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

      std::deque<std::size_t> &deq = (found->second);
      if (deq.front() > latest_occur) {
        latest_occur = deq.front();
        latest = &s;
      }
    }

    assert(latest);

    return *latest;
  }

  void erase_first(const T &p_elem) {
    auto found = m_map.find(p_elem);

    if (found == m_map.end()) {
      return;
    }

    std::deque<std::size_t> &deq = found->second;
    deq.pop_front();

    if (deq.empty()) {
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

public:
  ideal_t(std::size_t p_size, const std::vector<T> &p_vec)
      : m_set{}, m_vec{p_vec}, m_occur_map{p_vec}, m_size{p_size}, m_hits{0} {
    m_set.reserve(p_size);
  }

  std::size_t count_hits() {
    for (const auto &v : m_vec) {
      lookup_elem(v);
    }

    // After all elements of vector have been iterated through, occurence map
    // should be empty.
    assert(m_occur_map.is_empty());

    return m_hits;
  }
};

} // namespace detail

// Implementation of Belady's algorithm. Returns the number of maximum possible
// hits for a cache of size "size" and "vec" of requests.
template <typename T>
std::size_t get_optimal_hits(std::size_t size, std::vector<T> &vec) {
  using namespace detail;

  ideal_t<T> cache{size, vec};
  return cache.count_hits();
}

namespace detail {

template <typename K, typename U> class local_node_lfu_t {
  K m_key;
  U m_value;

public:
  local_node_lfu_t(K p_key, U p_val) : m_key{p_key}, m_value{p_val} {
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

template <typename K, typename U, typename W, typename N> class local_list_t {
public:
  using it = typename std::list<N>::iterator;
  using const_it = typename std::list<N>::const_iterator;

private:
  std::list<N> m_list;
  std::unordered_map<K, it> m_map;
  W m_weight;

public:
  local_list_t(W p_weight) : m_list{}, m_map{}, m_weight{p_weight} {
  }

  const W &weight() const {
    return m_weight;
  }

  bool is_empty() const {
    return m_list.empty();
  }

  void push_front(K p_key, U p_val) {
    m_list.emplace_front(p_key, p_val);
    m_map.insert({p_key, m_list.begin()});
  }

  void splice_upfront(local_list_t<K, U, W, N> &p_other, const_it p_elem) {
    // Erase "p_elem" from other list's lookup map.
    p_other.m_map.erase(p_elem->key());
    // Move "p_elem" to the beginning of the "p_other".
    m_list.splice(m_list.begin(), p_other.m_list, p_elem);
    // Insert the element's key into current lookup map.
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
    // Local lists should never be empty if "last" is called.
    assert(!is_empty());
    return std::prev(m_list.end());
  }
};

}; // namespace detail

template <typename U, typename K = int> class lfu_t {
  std::size_t m_size, m_hits, m_curr;
  // detail::freq_list_t<K, U, std::size_t> m_freq_list;

  using W = std::size_t;

  using freq_list_node_t =
      detail::local_list_t<K, U, W, detail::local_node_lfu_t<K, U>>;
  using freq_node_it = typename std::list<freq_list_node_t>::iterator;

  std::list<freq_list_node_t> m_freq_list;
  std::unordered_map<K, freq_node_it> m_weight_map;

  bool is_last(freq_node_it p_iter) const {
    return (std::next(p_iter) == m_freq_list.end());
  }

  // Returns node with weight "1" or create a new node with corresponding weight
  // if it doesn't already exist.
  freq_node_it first_weight_node() {
    freq_node_it front = m_freq_list.begin();

    // In case the list isn't empty and front node has weight "1".
    if (!m_freq_list.empty() && front->weight() == 1) {
      return front;
    }

    m_freq_list.emplace_front(1);
    return m_freq_list.begin();
  }

  // Returns node with the least weight. Because the list is created in an
  // ascending sorted order it is the head of the list.
  freq_node_it least_weight_node() {
    // As this function will only be called on a non-empty list, it's safe to
    // call "begin()" that will never return "end".
    assert(!m_freq_list.empty());
    return m_freq_list.begin();
  }

  // This function creates a "frequency node" after "p_prev" with
  // "p_prev->weight() + 1" key in case it does not exists. Otherwise and
  // returns an incremented iterator.
  freq_node_it next_weight_node(freq_node_it p_prev) {
    if (is_last(p_prev)) {
      m_freq_list.emplace_back(p_prev->weight() + 1);
      return std::next(p_prev);
    }

    freq_node_it next_it = std::next(p_prev);
    W prev_weight = p_prev->weight(), next_weight = prev_weight + 1;

    if (next_it->weight() == next_weight) {
      return next_it;
    } else {
      return m_freq_list.insert(next_it, next_weight);
    }
  }

  // Removes the node of a list if the "local list" is empty.
  void remove_if_empty(freq_node_it p_it) {
    if (p_it->is_empty()) {
      m_freq_list.erase(p_it);
    }
  }

  // Helper function for Case 1 of "lookup". It promotes the the element
  // "p_key" according to LFU policy.
  U promote_element(K p_key) {
    // Lookup which local list "p_key" is present in.
    auto found_weight = m_weight_map.find(p_key);
    assert(found_weight != m_weight_map.end());

    // Corresponding iterator of "frequency list" and an node with incremented
    // weight.
    auto freq_it = found_weight->second;
    auto next_it = next_weight_node(freq_it);

    // Update weight map.
    found_weight->second = next_it;

    // Move "p_key" to the next local list.
    auto elem_it = freq_it->lookup(p_key);
    next_it->splice_upfront(*freq_it, elem_it);
    remove_if_empty(freq_it);

    return elem_it->value();
  }

  bool is_present(K p_key) const {
    return (m_weight_map.find(p_key) != m_weight_map.end());
  }

public:
  lfu_t(std::size_t p_size)
      : m_size{p_size}, m_hits{0}, m_curr{0}, m_freq_list{}, m_weight_map{} {
  }

  bool is_full() const {
    return (m_curr == m_size);
  }

  std::size_t get_hits() const {
    return m_hits;
  }

  template <typename F> U lookup(K p_key, F p_slow_get) {
    // Case 1. The entry is present in the cache. Then it gets promoted.
    if (is_present(p_key)) {
      m_hits++;
      return promote_element(p_key);
    }

    U val = p_slow_get();

    // Case 2. If the entry is not present at the moment, and cache is not full.
    // Then it gets inserted into a frequency bucket with weight "1" and pushed
    // into the beginning of the corresponding frequency list;
    if (!is_full()) {
      auto first = first_weight_node();
      first->push_front(p_key, val);
      m_weight_map.insert({p_key, first});
      m_curr++;
    }

    // Case 3. If we get here, then the cache is full and we need to choose an
    // entry to evict, that the new entry will replace.
    else {
      // First we choose the "to_evict" entry by looking up the tail of a local
      // list with the least weight.
      auto least = least_weight_node();
      auto to_evict = least->last();

      // Erase the entry from key-weight map.
      m_weight_map.erase(to_evict->key());
      // Clean up the frequency list, if "to_evict" was the only element in the
      // local list.
      remove_if_empty(least);

      // Reuse the local list node.
      to_evict->key() = p_key;
      to_evict->value() = val;

      // Move the now evicted node to the bucket with weight "1".
      first_weight_node()->splice_upfront(*least, to_evict);

      // Insert the new entry into the key-weight map.
      m_weight_map.insert({p_key, least_weight_node()});
    }

    return val;
  }
};

namespace detail {
template <typename K, typename U, typename W>
class local_node_lfuda_t : public local_node_lfu_t<K, U> {
  W m_freq;

public:
  local_node_lfuda_t(K p_key, U p_val)
      : local_node_lfu_t<K, U>{p_key, p_val}, m_freq{0} {
  }

  W &frequency() {
    return m_freq;
  }

  const W &frequency() const {
    return m_freq;
  }
};

template <typename K, typename U, typename W> class freq_map_t {
  using freq_t = local_list_t<K, U, W, local_node_lfuda_t<K, U, W>>;

  std::map<W, freq_t> m_freq_map;

public:
  using it = typename std::map<W, freq_t>::iterator;

  freq_map_t() : m_freq_map{} {
  }
};
}; // namespace detail

template <typename U, typename K = int> class lfuda_t {
  std::size_t m_size, m_hits, m_curr, m_age;

  using W = std::size_t;

  using freq_node_t =
      detail::local_list_t<K, U, W, detail::local_node_lfuda_t<K, U, W>>;
  using freq_node_it = typename std::map<W, freq_node_t>::iterator;

  std::map<W, freq_node_t> m_freq_map;
  std::unordered_map<K, W> m_weight_map;

  bool is_present(K p_key) {
    return (m_weight_map.find(p_key) != m_weight_map.end());
  }

  U promote_element(K p_key) {
    auto found_weight = m_weight_map.find(p_key);
    // Can't promote an element not present in the cache.
    assert(found_weight != m_weight_map.end());

    W weight = found_weight->second;

    auto found_freq = m_freq_map.find(weight);
    assert(found_freq != m_freq_map.end());

    freq_node_it freq_it = found_freq->second;

    assert(weight == freq_it->weight());
  }

public:
  lfuda_t(std::size_t p_size)
      : m_size{p_size}, m_hits{0}, m_curr{0}, m_age{0}, m_freq_map{} {
  }

  bool is_full() const {
    return (m_curr == m_size);
  }

  std::size_t get_hits() const {
    return m_hits;
  }

  template <typename F> U lookup(K p_key, F p_slow_get) {
    // Case 1. The entry is present in the cache. Then it gets promoted.
    if (is_present(p_key)) {
      m_hits++;
      return promote_element(p_key);
    }

    U val = p_slow_get();

    // Case 2. If the entry is not present at the moment, and cache is not full.
    // Then it gets inserted into a frequency bucket with weight "1" and pushed
    // into the beginning of the corresponding frequency list;
    if (!is_full()) {
      m_curr++;
    }

    // Case 3. If we get here, then the cache is full and we need to choose an
    // entry to evict, that the new entry will replace.
    else {
    }

    return val;
  }
};

}; // namespace caches