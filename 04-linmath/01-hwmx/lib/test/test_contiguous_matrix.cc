#include "contiguous_matrix.hpp"

#include <gtest/gtest.h>

using matrix = throttle::linmath::contiguous_matrix<float>;
template class throttle::linmath::contiguous_matrix<float>;

TEST(test_contiguous_matrix, test_zero) {
  matrix a = matrix::zero(9, 8);
  for (int i = 0; i < 9; i++)
    for (int j = 0; j < 8; j++)
      EXPECT_EQ(a[i][j], 0.0);
}

TEST(test_contiguous_matrix, test_unity) {
  const matrix a = matrix::unity(10);
  for (int i = 0; i < 10; i++)
    EXPECT_EQ(a[i][i], 1.0);
}

TEST(test_contiguous_matrix, test_sq_braces_1) {
  const matrix a = matrix::unity(10);
  EXPECT_EQ(a[0][0], 1);
}