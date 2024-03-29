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

#include <array>
#include <cstdint>
#include <iterator>
#include <numeric>
#include <stdexcept>
#include <string_view>
#include <utility>

#include <tuple>
#include <variant>
#include <vector>

#include "decl_vm.hpp"
#include "utils.hpp"
#include "utils/serialization.hpp"

namespace paracl::bytecode_vm::builder {

template <typename t_desc> struct encoded_instruction {
  using attribute_types = typename t_desc::attribute_types;

  attribute_types m_attr;

  template <std::size_t I> void encode_attributes(auto iter) const {
    paracl::utils::serialization::write_little_endian<std::tuple_element_t<I, attribute_types>>(std::get<I>(m_attr),
                                                                                                iter);
  }

  template <std::size_t... I> void encode_attributes(auto iter, std::index_sequence<I...>) const {
    (..., encode_attributes<I>(iter));
  }

public:
  auto    get_size() const { return t_desc::get_size(); }
  uint8_t get_opcode() const { return t_desc::get_opcode(); }

  void encode(auto iter) const {
    *iter = t_desc::get_opcode();
    encode_attributes(iter, std::make_index_sequence<std::tuple_size_v<attribute_types>>{});
  }

  template <typename... Ts> encoded_instruction(t_desc, Ts... p_args) : m_attr{std::forward<Ts>(p_args)...} {}
};

template <typename t_tuple> struct encoded_tuple_from_desc_tuple;
template <typename... Ts> struct encoded_tuple_from_desc_tuple<std::tuple<Ts...>> {
  using type = std::tuple<encoded_instruction<typename Ts::description_type>...>;
};

template <typename t_tuple>
using encoded_tuple_from_desc_tuple_t = typename encoded_tuple_from_desc_tuple<t_tuple>::type;

template <typename t_instruction_set> class bytecode_builder {
public:
  using instruction_variant_type =
      utils::variant_from_tuple_t<encoded_tuple_from_desc_tuple_t<typename t_instruction_set::instruction_tuple_type>>;

private:
  std::vector<instruction_variant_type> m_code;
  uint32_t m_cur_loc = 0;

public:
  bytecode_builder() = default;

  template <typename t_desc> auto emit_operation(encoded_instruction<t_desc> instruction) {
    m_code.push_back(instruction_variant_type{instruction});
    m_cur_loc += instruction.get_size();
    return m_code.size() - 1;
  }

  template <typename as_desc> auto &get_as(as_desc, std::size_t index) {
    return std::get<encoded_instruction<as_desc>>(m_code.at(index));
  }

  decl_vm::chunk to_chunk() const {
    decl_vm::chunk ch;

    auto iter = std::back_inserter(ch);
    for (const auto &v : m_code) {
      std::visit([iter](auto &&var) { var.encode(iter); }, v);
    }

    return ch;
  }

  uint32_t current_loc() const { return m_cur_loc; }
};

} // namespace paracl::bytecode_vm::builder