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

TEST(test_contiguous_matrix, test_mult_eq) {
  matrix a = matrix::unity(10);
  a *= 666;
  for (int i = 0; i < 10; i++)
    EXPECT_EQ(a[i][i], 666);
}

TEST(test_contiguous_matrix, test_dev_eq) {
  matrix a = matrix::unity(10);
  a *= 100;
  a /= 5;
  for (int i = 0; i < 10; i++)
    EXPECT_EQ(a[i][i], 20);
}

TEST(test_contiguous_matrix, test_multiplication) {
  matrix a = matrix::unity(10);
  matrix b = a * 666.0f;
  for (int i = 0; i < 10; i++)
    EXPECT_EQ(b[i][i], 666);
}

TEST(test_contiguous_matrix, test_devision) {
  matrix a = matrix::unity(10);
  a *= 100;
  matrix b = a / 5.0f;
  for (int i = 0; i < 10; i++)
    EXPECT_EQ(b[i][i], 20);
}