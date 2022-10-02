/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <tsimmerman.ss@phystech.edu>, <alex.rom23@mail.ru> wrote this file.  As long as you
 * retain this notice you can do whatever you want with this stuff. If we meet
 * some day, and you think this stuff is worth it, you can buy me a beer in
 * return.
 * ----------------------------------------------------------------------------
 */

#include "matrix.hpp"

#include <gtest/gtest.h>

using matrix = typename throttle::linmath::matrix<float>;
template class throttle::linmath::matrix<float>;

TEST(test_matrix, test_ctor_1) {
  matrix A = matrix::zero(3, 4);
  for (unsigned i = 0; i < 3; i++)
    for (unsigned j = 0; j < 4; j++)
      EXPECT_EQ(A[i][j], 0.0f);
}

TEST(test_matrix, test_ctor_2) {
  matrix A = matrix::unity(10);
  for (unsigned i = 0; i < 10; i++)
    EXPECT_EQ(A[i][i], 1.0f);
}

