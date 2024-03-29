/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <tsimmerman.ss@phystech.edu>, wrote this file.  As long as you
 * retain this notice you can do whatever you want with this stuff. If we meet
 * some day, and you think this stuff is worth it, you can buy me a beer in
 * return.
 * ----------------------------------------------------------------------------
 */

#pragma once

#include "i_ast_node.hpp"

namespace paracl::frontend::ast {

class constant_expression final : public i_ast_node {
  int m_val;

public:
  constant_expression(int p_val, location l) : i_ast_node{l}, m_val{p_val} {}

  constant_expression(const constant_expression &) = default;
  constant_expression &operator=(const constant_expression &) = default;

  void accept(i_ast_visitor &visitor) override { visitor.visit(this); }

  int &value() { return m_val; }
};

} // namespace paracl::frontend::ast