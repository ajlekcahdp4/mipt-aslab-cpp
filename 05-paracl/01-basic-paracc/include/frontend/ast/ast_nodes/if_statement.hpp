/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <tsimmerman.ss@phystech.edu>, <alex.rom23@mail.ru> wrote this file.  As long as you
 * retain this notice you can do whatever you want with this stuff. If we meet
 * some day, and you think this stuff is worth it, you can buy me a beer in
 * return.
 * ----------------------------------------------------------------------------
 */

#pragma once

#include "frontend/symtab.hpp"
#include "i_ast_node.hpp"
#include "statement_block.hpp"

namespace paracl::frontend::ast {

class if_statement final : public i_ast_node {
  symtab m_control_block_symtab;
  symtab m_true_symtab;
  symtab m_false_symtab;

  i_ast_node *m_condition;
  i_ast_node *m_true_block;
  i_ast_node *m_else_block; // Optional, can be nullptr

public:
  if_statement(i_ast_node *cond, i_ast_node *true_block, location l)
      : i_ast_node{l}, m_condition{cond}, m_true_block{true_block}, m_else_block{nullptr} {}

  if_statement(i_ast_node *cond, i_ast_node *true_block, i_ast_node *else_block, location l)
      : i_ast_node{l}, m_condition{cond}, m_true_block{true_block}, m_else_block{else_block} {}

  if_statement(const if_statement &) = default;
  if_statement &operator=(const if_statement &) = default;

  void accept(i_ast_visitor &visitor) override { visitor.visit(this); }

  i_ast_node *&cond() { return m_condition; }
  i_ast_node *&true_block() { return m_true_block; }
  i_ast_node *&else_block() { return m_else_block; }

  symtab *true_symtab() { return &m_true_symtab; }
  symtab *else_symtab() { return &m_false_symtab; }
  symtab *control_block_symtab() { return &m_control_block_symtab; }
};

} // namespace paracl::frontend::ast