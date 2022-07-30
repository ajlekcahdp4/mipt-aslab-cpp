#include <cstddef>
#include <cstdlib>
#include <functional>
#include <gtest/gtest.h>

#define private public
#define protected public
#include "rb_tree_ranged.hpp"
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

int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}