/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <tsimmerman.ss@phystech.edu>, <alex.rom23@mail.ru> wrote this file.  As long as you
 * retain this notice you can do whatever you want with this stuff. If we meet
 * some day, and you think this stuff is worth it, you can buy us a beer in
 * return.
 * ----------------------------------------------------------------------------
 */

#include <vector.hpp>

#include <gtest/gtest.h>
#include <memory>

template class throttle::containers::vector<int>;
template class throttle::containers::vector<std::unique_ptr<int>>;

using vector = typename throttle::containers::vector<int>;

TEST(test_vector, test_ctor) {
  vector a;
  EXPECT_EQ(a.size(), 0);
}

TEST(test_vector, test_push_pop_back) {
  vector a(5);
  for (int i = 0; i < 5; i++)
    a.push_back(i);

  EXPECT_EQ(a.size(), 5);

  for (int i = 4; i >= 0; i--) {
    EXPECT_EQ(a.back(), i);
    a.pop_back();
  }

  EXPECT_EQ(a.size(), 0);
}

TEST(test_vector, test_copy_ctor) {
  vector a(5);
  for (int i = 0; i < 5; i++)
    a.push_back(i);

  vector b = a;

  EXPECT_EQ(a.capacity(), b.capacity());
  EXPECT_EQ(a.size(), b.size());

  for (int i = 4; i >= 0; i--) {
    EXPECT_EQ(b.back(), i);
    EXPECT_EQ(a.back(), i);
    b.pop_back();
    a.pop_back();
  }
}

TEST(test_vector, test_reserve_1) {
  vector a;
  a.reserve(7);
  EXPECT_GE(a.capacity(), 7);
  EXPECT_EQ(a.size(), 0);
}

TEST(test_vector, test_reserve_2) {
  vector a(5);
  a.reserve(7);
  EXPECT_GE(a.capacity(), 7);
  EXPECT_EQ(a.size(), 0);
}

TEST(test_vector, test_reserve_3) {
  vector a(5);

  for (int i = 0; i < 5; i++)
    a.push_back(i);

  EXPECT_EQ(a.size(), 5);

  a.reserve(7);

  EXPECT_GE(a.capacity(), 7);
  EXPECT_EQ(a.size(), 5);
}

TEST(test_vector, test_new_cap) {

}
