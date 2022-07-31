#include <cstddef>
#include <cstdlib>
#include <functional>
#include <gtest/gtest.h>

#define private public
#define protected public
#include "detail/rb_tree_ranged.hpp"
#undef private
#undef protected

using namespace throttle::detail;

namespace {
using base_node_ptr = rb_tree_ranged_node_base_ *;
using base_node = rb_tree_ranged_node_base_;

std::pair<std::size_t, bool> validate_red_black_helper(base_node_ptr p_base) {
  if (!p_base) { return {1, true}; }

  if (base_node::get_color_(p_base) == k_red_ &&
      (base_node::get_color_(p_base->m_left_) == k_red_ || base_node::get_color_(p_base->m_right_) == k_red_)) {
    return {0, false};
  }

  auto valid_left = validate_red_black_helper(p_base->m_left_);
  auto valid_right = validate_red_black_helper(p_base->m_right_);

  if (!valid_left.second || !valid_right.second || (valid_left.first != valid_right.first)) {
    return {0, false};
  } else {
    return {valid_right.first + (base_node::get_color_(p_base) == k_black_ ? 1 : 0), true};
  }
}

bool validate_size_helper(base_node_ptr p_base) {
  if (!p_base) { return {true}; }

  if (base_node::size(p_base) != base_node::size(p_base->m_left_) + base_node::size(p_base->m_right_) + 1) {
    return false;
  }

  bool valid_left = validate_size_helper(p_base->m_left_);
  bool valid_right = validate_size_helper(p_base->m_right_);

  return valid_left && valid_right;
}
} // namespace

TEST(test_rb_tree_private, test_1) {
  rb_tree_ranged_<int, std::less<int>> t;
  bool thrown = false;

  try {
    for (int i = 0; i < 131072; i++) {
      t.insert(i);
    }
  } catch (...) { thrown = true; }

  EXPECT_EQ(validate_red_black_helper(t.m_root_).second && !thrown, true);
  thrown = false;

  try {
    for (int i = 0; i < 32768; i++) {
      t.erase(i);
    }
  } catch (...) { thrown = true; }

  EXPECT_EQ(validate_red_black_helper(t.m_root_).second && !thrown, true);
}

TEST(test_rb_tree_private, test_2) {
  rb_tree_ranged_<int, std::less<int>> t;
  bool thrown = false;

  try {
    for (int i = 0; i < 65536; i++) {
      int temp = std::rand();
      if (!t.contains(temp)) { t.insert(temp); }
    }
  } catch (...) { thrown = true; }

  EXPECT_EQ(validate_size_helper(t.m_root_) && !thrown, true);
}

TEST(test_rb_tree_private, test_3) {
  rb_tree_ranged_<int, std::less<int>> t;
  for (int i = 0; i < 1024; i++) {
    t.insert(i);
  }

  for (int i = 0; i < 1024; i++) {
    ASSERT_TRUE(t.contains(i));
  }

  t.erase(666);
  ASSERT_FALSE(t.contains(666));
}

TEST(test_rb_tree_private, test_4) {
  rb_tree_ranged_<int, std::less<int>> t;
  for (int i = 1; i < 131072; i++) {
    t.insert(i);
  }

  ASSERT_EQ(t.select_rank(1), 1);

  for (int i = 1; i < 131072; i++) {
    ASSERT_EQ(t.select_rank(i), i);
  }
}

TEST(test_rb_tree_private, test_5) {
  rb_tree_ranged_<int, std::less<int>> t;
  
  t.insert(1);
  t.insert(5);
  t.insert(10);
  t.insert(12);
  t.insert(14);
  t.insert(18);
  t.insert(21);
  t.insert(276);

  ASSERT_EQ(t.select_rank(1), 1);
  ASSERT_EQ(t.select_rank(4), 12);
  ASSERT_EQ(t.select_rank(8), 276);
  ASSERT_EQ(t.select_rank(5), 14);
  ASSERT_EQ(t.select_rank(7), 21);
}

TEST(test_rb_tree_private, test_6) {
  rb_tree_ranged_<int, std::less<int>> t;
  
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

TEST(test_rb_tree_private, test_7) {
  rb_tree_ranged_<int, std::less<int>> t;
  
  t.insert(1);
  t.insert(5);
  t.insert(10);
  t.insert(12);
  t.insert(14);
  t.insert(18);
  t.insert(21);
  t.insert(276);

  ASSERT_EQ(t.upper_bound(4), 5);
  ASSERT_EQ(t.upper_bound(1), 5);
  ASSERT_EQ(t.upper_bound(14), 18);
  ASSERT_EQ(t.upper_bound(17), 18);
  ASSERT_EQ(t.upper_bound(42), 276);

  bool thrown = false;
  try {
    t.upper_bound(276);
  } catch (...) {
    thrown = true;
  }
  ASSERT_TRUE(thrown);
}

TEST(test_rb_tree_private, test_8) {
  rb_tree_ranged_<int, std::less<int>> t;
  
  t.insert(1);
  t.insert(5);
  t.insert(10);
  t.insert(12);
  t.insert(14);
  t.insert(18);
  t.insert(21);
  t.insert(276);

  ASSERT_EQ(t.lower_bound(4), 1);
  ASSERT_EQ(t.lower_bound(1), 1);
  ASSERT_EQ(t.lower_bound(5), 5);
  ASSERT_EQ(t.lower_bound(7), 5);
  ASSERT_EQ(t.lower_bound(276), 276);
  ASSERT_EQ(t.lower_bound(1000), 276);
  ASSERT_EQ(t.lower_bound(20), 18);

  ASSERT_EQ(t.get_rank_of(t.lower_bound(15)), 5);
}

int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}