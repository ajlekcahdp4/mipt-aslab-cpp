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
#include <iterator>
#include <list>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace caches {

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
  using const_it = typename std::list<N>::const_iterator;
  using it = typename std::list<N>::iterator;

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

  void push_front(N p_node) {
    m_list.push_front(p_node);
    m_map.insert({p_node.key(), m_list.begin()});
  }

  void splice_upfront(local_list_t<K, U, W, N> &p_other, const_it p_elem) {
    p_other.m_map.erase(p_elem->key());                    // Erase "p_elem" from other list's lookup map.
    m_list.splice(m_list.begin(), p_other.m_list, p_elem); // Move "p_elem" to the beginning of the "p_other".
    m_map.insert({p_elem->key(), m_list.begin()});         // Insert the element's key into current lookup map.
  }

  void splice_upfront(local_list_t<K, U, W, N> &p_other, it p_elem, const K &p_new_key) {
    p_other.m_map.erase(p_elem->key());                    // Erase "p_elem" from other list's lookup map.
    m_list.splice(m_list.begin(), p_other.m_list, p_elem); // Move "p_elem" to the beginning of the "p_other".
    m_map.insert({p_new_key, m_list.begin()}); // Insert the element's key into current lookup map with updated key.
    p_elem->key() = p_new_key;
  }

  it lookup(const K &p_key) {
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

  using W = std::size_t;

  using freq_list_node_t__ = detail::local_list_t<K, U, W, detail::local_node_lfu_t<K, U>>;
  using freq_node_it__ = typename std::list<freq_list_node_t__>::iterator;

  std::list<freq_list_node_t__> m_freq_list;
  std::unordered_map<K, freq_node_it__> m_weight_map;

  bool is_last(freq_node_it__ p_iter) const {
    return (std::next(p_iter) == m_freq_list.end());
  }

  // Returns node with weight "1" or create a new node with corresponding weight
  // if it doesn't already exist.
  freq_node_it__ first_weight_node() {
    auto front = m_freq_list.begin();

    // In case the list isn't empty and front node has weight "1".
    if (!m_freq_list.empty() && front->weight() == 1) {
      return front;
    }

    m_freq_list.emplace_front(1);
    return m_freq_list.begin();
  }

  // Returns node with the least weight. Because the list is created in an
  // ascending sorted order it is the head of the list.
  freq_node_it__ least_weight_node() {
    // As this function will only be called on a non-empty list, it's safe to
    // call "begin()" that will never return "end".
    assert(!m_freq_list.empty());
    return m_freq_list.begin();
  }

  // This function creates a "frequency node" after "p_prev" with
  // "p_prev->weight() + 1" key in case it does not exists. Otherwise and
  // returns an incremented iterator.
  freq_node_it__ next_weight_node(freq_node_it__ p_prev) {
    W next_weight = p_prev->weight() + 1;

    if (is_last(p_prev)) {
      m_freq_list.emplace_back(next_weight);
      return std::next(p_prev);
    }

    freq_node_it__ next_it = std::next(p_prev);

    if (next_it->weight() == next_weight) {
      return next_it;
    } else {
      return m_freq_list.insert(next_it, next_weight);
    }
  }

  // Removes the node of a list if the "local list" is empty.
  void remove_if_empty(freq_node_it__ p_it) {
    if (p_it->is_empty()) {
      m_freq_list.erase(p_it);
    }
  }

  // Helper function for Case 1 of "lookup". It promotes the the element
  // "p_key" according to LFU policy.
  U promote(const K &p_key) {
    auto found_weight = m_weight_map.find(p_key); // Lookup which local list "p_key" is present in.
    assert(found_weight != m_weight_map.end());

    // Corresponding iterator of "frequency list" and an node with incremented
    // weight.
    freq_node_it__ freq_it = found_weight->second;
    freq_node_it__ next_it = next_weight_node(freq_it);

    found_weight->second = next_it; // Update weight map.

    auto elem_it = freq_it->lookup(p_key);
    next_it->splice_upfront(*freq_it, elem_it); // Move "p_key" to the next local list.
    remove_if_empty(freq_it);

    return elem_it->value();
  }

  void insert(const K &p_key, U p_val) {
    auto first = first_weight_node();
    first->push_front({p_key, p_val});
    m_weight_map.insert({p_key, first});
    m_curr++;
  }

  void evict_and_replace(const K &p_key, U p_val) {
    // First we choose the "to_evict" entry by looking up the tail of a local list with the least weight.
    auto least = least_weight_node();
    auto to_evict = least->last();

    m_weight_map.erase(to_evict->key()); // Erase the entry from key-weight map.
    to_evict->value() = p_val;           // Reuse the local list node.

    // Move the now evicted node to the bucket with weight "1".
    first_weight_node()->splice_upfront(*least, to_evict, p_key);
    remove_if_empty(least); // Clean up the frequency list, if "to_evict" was the only element in the local list.
    m_weight_map.insert({p_key, least_weight_node()}); // Insert the new entry into the key-weight map.
  }

  bool is_present(const K &p_key) const {
    return (m_weight_map.find(p_key) != m_weight_map.end());
  }

public:
  lfu_t(std::size_t p_size) : m_size{p_size}, m_hits{0}, m_curr{0}, m_freq_list{}, m_weight_map{} {
  }

  bool is_full() const {
    return (m_curr == m_size);
  }

  std::size_t get_hits() const {
    return m_hits;
  }

  template <typename F> U lookup(const K &p_key, F p_slow_get) {
    // Case 1. The entry is present in the cache. Then it gets promoted.
    if (is_present(p_key)) {
      m_hits++;
      return promote(p_key);
    }

    U val = p_slow_get(p_key);

    // Case 2. If the entry is not present at the moment, and cache is not full.
    // Then it gets inserted into a frequency bucket with weight "1" and pushed
    // into the beginning of the corresponding frequency list;
    if (!is_full()) {
      insert(p_key, val);
    }

    // Case 3. If we get here, then the cache is full and we need to choose an
    // entry to evict, that the new entry will replace.
    else {
      evict_and_replace(p_key, val);
    }

    return val;
  }
};

namespace detail {
template <typename K, typename U, typename W> class local_node_lfuda_t : public local_node_lfu_t<K, U> {
  W m_freq;

public:
  local_node_lfuda_t(K p_key, U p_val, W p_freq = 1) : local_node_lfu_t<K, U>{p_key, p_val}, m_freq{p_freq} {
  }

  W &frequency() {
    return m_freq;
  }

  const W &frequency() const {
    return m_freq;
  }
};

}; // namespace detail

template <typename U, typename K = int> class lfuda_t {
  using W = std::size_t;

  std::size_t m_size, m_hits, m_curr;
  W m_age;

  using local_node_t__ = typename detail::local_node_lfuda_t<K, U, W>;
  using freq_node_t__ = detail::local_list_t<K, U, W, local_node_t__>;

  std::map<W, freq_node_t__> m_freq_map;
  std::unordered_map<K, W> m_weight_map;

  bool is_present(K p_key) const {
    return (m_weight_map.find(p_key) != m_weight_map.end());
  }

  freq_node_t__ &freq_node_with_weight(W p_weight) {
    auto inserted = m_freq_map.emplace(p_weight, p_weight);
    return inserted.first->second;
  }

  W calculate_next_weight(const local_node_t__ &p_node) const noexcept {
    return p_node.frequency() + m_age;
  }

  void remove_if_empty(const W p_weight) {
    auto found = m_freq_map.find(p_weight);
    assert(found != m_freq_map.end());
    auto freq_list = found->second;
    if (freq_list.is_empty()) {
      m_freq_map.erase(p_weight);
    }
  }

  U promote(const K &p_key) {
    auto found = m_weight_map.find(p_key);
    assert(found != m_weight_map.end());

    W &old_weight = found->second; // Lookup the weight and frequency node in the frequency list.
    auto &[ignored_1, old_freq_node] = *m_freq_map.find(old_weight);

    // Lookup the node in corresponding local list which has to be promoted.
    auto node_to_promote = old_freq_node.lookup(p_key);

    node_to_promote->frequency()++; // Increment the frequency and calculate next weight.
    W new_weight = calculate_next_weight(*node_to_promote);
    freq_node_t__ &new_freq_node = freq_node_with_weight(new_weight);

    new_freq_node.splice_upfront(old_freq_node, node_to_promote); // Move the node to the next list.
    remove_if_empty(old_weight);                                  // Clean up after ourselves.

    old_weight = new_weight; // Update the value stored in map.
    return node_to_promote->value();
  }

  void insert(const K &p_key, U p_val) {
    auto to_insert = local_node_t__{p_key, p_val};
    W new_weight = calculate_next_weight(to_insert);
    freq_node_with_weight(new_weight).push_front({p_key, p_val});
    m_weight_map.insert({p_key, new_weight});
    m_curr++;
  }

  void evict_and_replace(const K &p_key, U p_val) {
    assert(m_freq_map.size()); // Cache should contain some elements or I've made an oopsie somewhere.
    auto &[least_weight, least_node] = *m_freq_map.begin();

    m_age = least_weight; // Upadte weight to the evicted entry's weight.
    auto to_evict = least_node.last();
    m_weight_map.erase(to_evict->key());

    to_evict->value() = p_val; // Reuse the entry for the newly inserted element.
    to_evict->frequency() = 1; //

    W new_weight = calculate_next_weight(*to_evict);
    freq_node_with_weight(new_weight).splice_upfront(least_node, to_evict, p_key);
    remove_if_empty(least_weight); // As always, cleanup after yourself.

    m_weight_map.insert({p_key, new_weight});
  }

public:
  lfuda_t(std::size_t p_size) : m_size{p_size}, m_hits{0}, m_curr{0}, m_age{0}, m_freq_map{} {
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
      return promote(p_key);
    }

    U val = p_slow_get(p_key);

    // Case 2. If the entry is not present at the moment, and cache is not full.
    // Then it gets inserted into a weight bucket with weight "m_age + 1" and pushed
    // into the beginning of the corresponding weight list;
    if (!is_full()) {
      insert(p_key, val);
    }

    // Case 3. If we get here, then the cache is full and we need to choose an
    // entry to evict, that the new entry will replace.
    else {
      evict_and_replace(p_key, val);
    }

    return val;
  }
};

}; // namespace caches