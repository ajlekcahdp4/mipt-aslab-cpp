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

class print_statement final : public i_ast_node {
private:
  i_ast_node *m_expr;

public:
  print_statement(i_ast_node *p_expr, location l) : i_ast_node{l}, m_expr{p_expr} {}

  print_statement(const print_statement &) = default;
  print_statement &operator=(const print_statement &) = default;

  void accept(i_ast_visitor &visitor) override { visitor.visit(this); }

  i_ast_node *&expr() { return m_expr; }
};

} // namespace paracl::frontend::ast