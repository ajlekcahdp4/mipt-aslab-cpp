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

#include <algorithm>
#include <cstdint>
#include <stdexcept>
#include <string>
#include <unordered_map>

#include "bytecode_vm.hpp"
#include "bytecode_vm/virtual_machine.hpp"
#include "frontend/ast.hpp"

namespace paracl::codegen {

class codegen_symtab_stack final {
private:
  std::vector<std::unordered_map<std::string, uint32_t>> m_stack;

  uint32_t total_count = 0;

public:
  uint32_t lookup_location(const std::string &name) const {
    for (auto its = m_stack.rbegin(), ite = m_stack.rend(); its != ite; ++its) {
      const auto &elem = *its;
      auto        found = elem.find(name);
      if (found != elem.end()) return found->second;
    }

    throw std::logic_error{"Codegen: trying to look up location of a variable not present in the symbol table"};
  }

  void begin_scope() { m_stack.emplace_back(); }

  void end_scope() {
    total_count -= m_stack.back().size();
    m_stack.pop_back();
  }

  void push_variable(const std::string &name) {
    m_stack.back().insert({name, total_count});
    total_count++;
  }
};

class codegen_visitor final : public paracl::frontend::ast::i_ast_visitor {
  using builder_type =
      paracl::bytecode_vm::builder::bytecode_builder<decltype(paracl::bytecode_vm::instruction_set::paracl_isa)>;

  std::unordered_map<int, uint32_t> m_constant_map;

  codegen_symtab_stack m_symtab_stack;
  builder_type         m_builder;

  bool m_is_currently_statement = false;

  void set_currently_statement();
  void reset_currently_statement();
  bool is_currently_statement() const;

  void visit_if_no_else(paracl::frontend::ast::if_statement *);
  void visit_if_with_else(paracl::frontend::ast::if_statement *);

  uint32_t lookup_or_insert_constant(int constant);

public:
  codegen_visitor() = default;

  void visit(paracl::frontend::ast::assignment_statement *) override;
  void visit(paracl::frontend::ast::binary_expression *) override;
  void visit(paracl::frontend::ast::constant_expression *) override;
  void visit(paracl::frontend::ast::if_statement *) override;
  void visit(paracl::frontend::ast::print_statement *) override;
  void visit(paracl::frontend::ast::read_expression *) override;
  void visit(paracl::frontend::ast::statement_block *) override;
  void visit(paracl::frontend::ast::unary_expression *) override;
  void visit(paracl::frontend::ast::variable_expression *) override;
  void visit(paracl::frontend::ast::while_statement *) override;
  void visit(paracl::frontend::ast::error_node *) override;

  paracl::bytecode_vm::decl_vm::chunk to_chunk();
};

paracl::bytecode_vm::decl_vm::chunk generate_code(paracl::frontend::ast::i_ast_node *);

} // namespace paracl::codegen