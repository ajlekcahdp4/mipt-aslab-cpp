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

#include <iostream>
#include <optional>
#include <string>
#include <unordered_set>
#include <vector>

namespace paracl::frontend {

class symtab final {
private:
  std::unordered_set<std::string> m_table;

public:
  void declare(std::string_view name) { m_table.emplace(name); }

  bool declared(std::string_view name) const {
    auto it = m_table.find(std::string{name});
    return (it != m_table.end());
  }

  auto begin() const { return m_table.begin(); }
  auto end() const { return m_table.end(); }
  auto size() const { return m_table.size(); }
};

class symtab_stack final {
private:
  std::vector<symtab *> m_stack;

public:
  void begin_scope(symtab *stab) { m_stack.push_back(stab); }

  void end_scope() { m_stack.pop_back(); }

  std::optional<unsigned> declared(std::string_view name) const {
    for (int i = m_stack.size() - 1; i >= 0; --i) {
      if (m_stack[i]->declared(name)) return i;
    }
    return std::nullopt;
  }

  void declare(std::string_view name) { m_stack.back()->declare(name); }
};

} // namespace paracl::frontend