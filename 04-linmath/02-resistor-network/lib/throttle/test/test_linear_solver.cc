#include <gtest/gtest.h>
#include <iostream>

#include "linear_solver.hpp"
#include "matrix.hpp"

TEST(test_linear_solver, test_1) {
  throttle::matrix_d coefs{3, 3, {1, 1, 1, 0, 2, 5, 2, 5, -1}};
  throttle::matrix_d col{3, 1, {6, -4, 27}};
  throttle::matrix_d sol{3, 1, {5, 3, -2}};

  auto res = throttle::nonsingular_solver(coefs, col);
  for (unsigned i = 0; i < res.rows(); i++)
    for (unsigned j = 0; j < res.cols(); j++)
      std::cout << res[i][j] << " ";
  std::cout << std::endl;
  EXPECT_EQ(res, sol);
}

TEST(test_linear_solver, test_2) {
  throttle::matrix_d coefs{3, 3, {1, 3, -2, 3, 5, 6, 2, 4, 3}};
  throttle::matrix_d col{3, 1, {5, 7, 8}};
  throttle::matrix_d sol{3, 1, {-15, 8, 2}};

  auto res = throttle::nonsingular_solver(coefs, col);
  EXPECT_EQ(res, sol);
}

TEST(test_linear_solver, dependent_system_1) {
  throttle::matrix_d coefs{3, 2, {-1, 2, 2, 3, 1, -2}};
  throttle::matrix_d col{3, 1, {0, 0, 0}};
  throttle::matrix_d sol{2, 1, {0, 0}};
  auto               res = throttle::nonsingular_solver(coefs, col);
  EXPECT_EQ(res, sol);
}

TEST(test_linear_solver, dependent_system_2) {
  throttle::matrix_d coefs{4, 3, {1, 1, 1, 0, -4, -10, 0, 2, 5, 2, 5, -1}};
  throttle::matrix_d col{4, 1, {6, 8, -4, 27}};
  throttle::matrix_d sol{3, 1, {5, 3, -2}};

  auto res = throttle::nonsingular_solver(coefs, col);
  EXPECT_EQ(res, sol);
}

TEST(test_linear_solver, singular_system) {
  throttle::matrix_d coefs{4, 3, {1, 1, 1, 0, 5, -10, 0, 2, 5, 2, 5, -1}};
  throttle::matrix_d col{4, 1, {6, 8, -4, 27}};
  EXPECT_THROW(throttle::nonsingular_solver(coefs, col), std::runtime_error);
}