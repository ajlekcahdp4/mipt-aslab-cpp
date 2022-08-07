#include <algorithm>
#include <cstddef>
#include <cstdlib>
#include <functional>
#include <gtest/gtest.h>
#include <numeric>
#include <set>
#include <string>

#define private public
#define protected public
#include "detail/splay_order_tree.hpp"
#include "splay_order_set.hpp"
#undef private
#undef protected

// Implicit instantiation for testing puproses
template class throttle::detail::splay_order_tree<int, std::less<int>, int>;
template class throttle::detail::splay_order_tree<std::string, std::less<std::string>, std::string>;

using namespace throttle::detail;

namespace {
using base_node_ptr = bst_order_node_base *;
using base_node = bst_order_node_base;

bool validate_size_helper(base_node_ptr p_base) {
  if (!p_base) {
    return {true};
  }

  if (base_node::size(p_base) != base_node::size(p_base->m_left) + base_node::size(p_base->m_right) + 1) {
    return false;
  }

  bool valid_left = validate_size_helper(p_base->m_left);
  bool valid_right = validate_size_helper(p_base->m_right);

  return valid_left && valid_right;
}
} // namespace

TEST(test_rb_tree_private, test_1) {
  splay_order_tree<int, std::less<int>, int> t;

  EXPECT_NO_THROW(for (int i = 0; i < 256000; i++) { t.insert(i); });
  EXPECT_NO_THROW(for (int i = 0; i < 100000; i++) { t.erase(i); });
  EXPECT_EQ(t.size(), 256000 - 100000);
  EXPECT_EQ(validate_size_helper(t.m_root), true);

  size_t count = 0;
  for (const auto v : t) {
    count++;
  }

  EXPECT_EQ(count, 256000 - 100000);
}

TEST(test_rb_tree_private, test_2) {
  throttle::splay_order_set<int> t{};

  EXPECT_NO_THROW(for (int i = 0; i < 65536; i++) {
    int temp = std::rand();
    if (!t.contains(temp)) {
      t.insert(temp);
    }
  });

  EXPECT_EQ(validate_size_helper(t.m_tree_impl.m_root), true);
}

TEST(test_rb_tree_private, test_3) {
  throttle::splay_order_set<int> t{};
  for (int i = 0; i < 1024; i++) {
    t.insert(i);
  }

  EXPECT_EQ(validate_size_helper(t.m_tree_impl.m_root), true);

  for (int i = 0; i < 1024; i++) {
    ASSERT_TRUE(t.contains(i));
  }

  t.erase(666);
  ASSERT_FALSE(t.contains(666));

  EXPECT_EQ(validate_size_helper(t.m_tree_impl.m_root), true);
  ASSERT_THROW(t.erase(666), std::out_of_range);

  EXPECT_EQ(validate_size_helper(t.m_tree_impl.m_root), true);
}

TEST(test_rb_tree_private, test_4) {
  throttle::splay_order_set<int> t{};
  for (int i = 1; i < 131072; i++) {
    t.insert(i);
  }

  ASSERT_EQ(*t.select_rank(1), 1);

  for (int i = 1; i < 131072; i++) {
    ASSERT_EQ(*t.select_rank(i), i);
  }

  for (int i = 1; i < 4096; i++) {
    t.erase(i);
  }

  EXPECT_EQ(validate_size_helper(t.m_tree_impl.m_root), true);
}

TEST(test_rb_tree_private, test_5) {
  throttle::splay_order_set<int> t{};
  for (int i = 1; i < 131072; i++) {
    t.insert(i);
  }

  auto sum = std::count_if(t.begin(), t.end(), [](int i) { return i < 1000; });
  EXPECT_EQ(sum, 999);
}

TEST(test_rb_tree_private, test_6) {
  throttle::splay_order_set<int> t{};

  t.insert(1);
  t.insert(5);
  t.insert(10);
  t.insert(12);
  t.insert(14);
  t.insert(18);
  t.insert(21);
  t.insert(276);

  ASSERT_EQ(*t.select_rank(1), 1);
  ASSERT_EQ(*t.select_rank(4), 12);
  ASSERT_EQ(*t.select_rank(8), 276);
  ASSERT_EQ(*t.select_rank(5), 14);
  ASSERT_EQ(*t.select_rank(7), 21);

  ASSERT_EQ(t.select_rank(200), t.end());
}

TEST(test_rb_tree_private, test_7) {
  throttle::splay_order_set<int> t{};

  t.insert(1);
  t.insert(5);
  t.insert(10);
  t.insert(12);
  t.insert(14);
  t.insert(18);
  t.insert(21);
  t.insert(276);

  ASSERT_EQ(t.get_rank_of(1), 1);
  ASSERT_EQ(t.get_rank_of(5), 2);
  ASSERT_EQ(t.get_rank_of(10), 3);
  ASSERT_EQ(t.get_rank_of(12), 4);
  ASSERT_EQ(t.get_rank_of(14), 5);
  ASSERT_EQ(t.get_rank_of(18), 6);
  ASSERT_EQ(t.get_rank_of(21), 7);
  ASSERT_EQ(t.get_rank_of(276), 8);
}

TEST(test_rb_tree_private, test_8) {
  throttle::splay_order_set<int> t{};

  t.insert(1);
  t.insert(5);
  t.insert(10);
  t.insert(12);
  t.insert(14);
  t.insert(18);
  t.insert(21);
  t.insert(276);

  EXPECT_EQ(*t.closest_left(1), 1);
  EXPECT_EQ(*t.closest_left(2), 1);
  EXPECT_EQ(*t.closest_right(5), 10);
  EXPECT_EQ(*t.closest_right(9), 10);
  EXPECT_EQ(t.closest_right(276), t.end());

  std::set<int> s{};
  std::copy(t.begin(), t.end(), std::inserter(s, s.end()));

  for (const auto v : t) {
    EXPECT_NE(s.find(v), s.end());
  }

  EXPECT_EQ(s.size(), t.size());
}

TEST(test_rb_tree_private, test_9) {
  throttle::splay_order_set<int> t{};
  std::set<int> s{};
  
  for (int i = 0; i < 262144; i+=2) {
    t.insert(i);
    s.insert(i);
  }

  for (int i = -262144; i < 262144; i++) {
    auto upper_1 = t.upper_bound(i);
    auto upper_2 = s.upper_bound(i);
    ASSERT_FALSE((upper_1 == t.end() && upper_2 != s.end()) || (upper_1 != t.end() && upper_2 == s.end()));
    EXPECT_TRUE((upper_1 == t.end() && upper_2 == s.end()) || *upper_1 == *upper_2);
  }

  for (int i = -262144; i < 262144; i++) {
    auto lower_1 = t.lower_bound(i);
    auto lower_2 = s.lower_bound(i);
    ASSERT_FALSE((lower_1 == t.end() && lower_2 != s.end()) || (lower_1 != t.end() && lower_2 == s.end()));
    EXPECT_TRUE((lower_1 == t.end() && lower_2 == s.end()) || *lower_1 == *lower_2);
  }
}

TEST(test_rb_tree_private, test_10) {
  throttle::splay_order_set<int> t{};
  
  for (int i = 0; i < 262144; i++) {
    int temp = rand();
    if (!t.contains(temp)) t.insert(temp);
  }

  ASSERT_TRUE(std::is_sorted(t.begin(), t.end()));
}

int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}