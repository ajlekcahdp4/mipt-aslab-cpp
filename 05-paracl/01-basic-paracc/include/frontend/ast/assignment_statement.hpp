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

class assignment_statement : public i_expression_statement_node {
public:
  i_expression_node_uptr m_left, m_right;

  assignment_statement(i_expression_node_uptr &&left, i_expression_node_uptr &&right)
      : m_left{std::move(left)}, m_right{std::move(right)} {}

  void accept(i_ast_visitor &visitor) { visitor.visit(this); }
};

static inline i_expression_statement_node_uptr make_assignment_statement(i_expression_node_uptr &&left,
                                                                         i_expression_node_uptr &&right) {
  return std::make_unique<assignment_statement>(std::move(left), std::move(right));
}

} // namespace paracl::frontend::ast