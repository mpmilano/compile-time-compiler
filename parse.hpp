#pragma once
#include "ast.hpp"

/*

*/

using Alloc = as_values::AST_Allocator<15>;

template <typename string> struct parse {
  const string _str;
  using string_length =
      std::integral_constant<std::size_t,
                             ::mutils::cstring::str_len(string{}.str)>;
  using str_t = char const[string_length::value + 1];
  using str_nc = char[string_length::value + 1];
  Alloc allocator;

  constexpr allocated_ref<as_values::AST_elem>
  parse_expression(const str_t &str) {

    allocated_ref<as_values::AST_elem> ret =
        allocator.template allocate<as_values::AST_elem>();
    ret.get(allocator).template get_<as_values::Constant>().is_this_elem = true;
    auto &voidref = ret.get(allocator).template get_<as_values::Constant>().t;
    (void)voidref;
    return ret;
  }

  constexpr allocated_ref<as_values::AST_elem>
  parse_statement(const str_t &str) {

    allocated_ref<as_values::AST_elem> ret =
        allocator.template allocate<as_values::AST_elem>();
    ret.get(allocator).template get_<as_values::Skip>().is_this_elem = true;
    auto &skipref = ret.get(allocator).template get_<as_values::Skip>().t;
    (void)skipref;
    return ret;
  }

  constexpr allocated_ref<as_values::AST_elem>
  parse_sequence(const str_t &str) {
    using namespace mutils;
    using namespace cstring;

    allocated_ref<as_values::AST_elem> ret =
        allocator.template allocate<as_values::AST_elem>();
    ret.get(allocator).template get_<as_values::Sequence>().is_this_elem = true;
    auto &seqref = ret.get(allocator).template get_<as_values::Sequence>().t;
    auto *seq = &seqref;
    str_nc string_bufs[20] = {{0}};
    auto groups = split_outside_parens(';', str, string_bufs);
    bool sequence_empty = true;
    assert(groups < 20);
    assert(groups > 0);
    for (auto i = 0u; i < groups; ++i) {
      seq->e = parse_statement(string_bufs[i]);
      if ((i + 1) < groups) {

        allocated_ref<as_values::AST_elem> newret =
            allocator.template allocate<as_values::AST_elem>();
        newret.get(allocator)
            .template get_<as_values::Sequence>()
            .is_this_elem = true;
        auto &seqref =
            newret.get(allocator).template get_<as_values::Sequence>().t;
        seq->next = std::move(newret);
        seq = &seqref;
      } else {

        allocated_ref<as_values::AST_elem> skipref =
            allocator.template allocate<as_values::AST_elem>();
        skipref.get(allocator).template get_<as_values::Skip>().is_this_elem =
            true;
        auto &voidref =
            skipref.get(allocator).template get_<as_values::Skip>().t;
        (void)voidref;
        seq->next = std::move(skipref);
      }
    }
    return ret;
  }

  constexpr parse() {
    str_nc local_copy{0};
    constexpr string w;
    for (std::size_t i = 0; i < string_length::value; ++i) {
      local_copy[i] = w.str[i];
    }
    // all parsing implemented in the constructor, so that
    // future things can just build this and expect it to work
    allocator.top.e = parse_sequence(local_copy);
  }
};
