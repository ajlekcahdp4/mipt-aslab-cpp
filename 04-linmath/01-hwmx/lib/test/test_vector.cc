#include <vector.hpp>

#include <gtest/gtest.h>

template class throttle::containers::vector<int>;

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