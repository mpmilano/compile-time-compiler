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
    using namespace mutils;
    using namespace cstring;

    allocated_ref<as_values::AST_elem> ret =
        allocator.template allocate<as_values::AST_elem>();
    ret.get(allocator).template get_<as_values::Constant>().is_this_elem = true;
    auto &voidref = ret.get(allocator).template get_<as_values::Constant>().t;
    (void)voidref;
    return ret;
  }

  constexpr allocated_ref<as_values::AST_elem> parse_var(const str_t &str) {
    using namespace mutils;
    using namespace cstring;

    allocated_ref<as_values::AST_elem> ret =
        allocator.template allocate<as_values::AST_elem>();
    ret.get(allocator).template get_<as_values::Skip>().is_this_elem = true;
    auto &skipref = ret.get(allocator).template get_<as_values::Skip>().t;
    (void)skipref;
    return ret;
  }
  constexpr allocated_ref<as_values::AST_elem> parse_return(const str_t &str) {
    using namespace mutils;
    using namespace cstring;

    allocated_ref<as_values::AST_elem> ret =
        allocator.template allocate<as_values::AST_elem>();
    ret.get(allocator).template get_<as_values::Return>().is_this_elem = true;
    auto &retref = ret.get(allocator).template get_<as_values::Return>().t;
    str_nc next = {0};
    remove_first_word(next, str);
    retref.Expr = parse_expression(next);
    return ret;
  }
  constexpr allocated_ref<as_values::AST_elem> parse_while(const str_t &str) {
    using namespace mutils;
    using namespace cstring;

    allocated_ref<as_values::AST_elem> ret =
        allocator.template allocate<as_values::AST_elem>();
    ret.get(allocator).template get_<as_values::While>().is_this_elem = true;
    auto &whileref = ret.get(allocator).template get_<as_values::While>().t;
    str_nc without_first = {0};
    remove_first_word(without_first, str);
    str_nc condition = {0};
    auto offset = next_paren_group(condition, without_first);
    str_nc body = {0};
    copy_within_parens(body, without_first + offset);
    whileref.condition = parse_expression(condition);
    whileref.body = parse_statement(body);
    return ret;
  }
  constexpr allocated_ref<as_values::AST_elem> parse_if(const str_t &str) {
    using namespace mutils;
    using namespace cstring;

    allocated_ref<as_values::AST_elem> ret =
        allocator.template allocate<as_values::AST_elem>();
    ret.get(allocator).template get_<as_values::Skip>().is_this_elem = true;
    auto &skipref = ret.get(allocator).template get_<as_values::Skip>().t;
    (void)skipref;
    return ret;
  }
  constexpr allocated_ref<as_values::AST_elem>
  parse_assignment(const str_t &str) {
    using namespace mutils;
    using namespace cstring;

    allocated_ref<as_values::AST_elem> ret =
        allocator.template allocate<as_values::AST_elem>();
    ret.get(allocator).template get_<as_values::Skip>().is_this_elem = true;
    auto &skipref = ret.get(allocator).template get_<as_values::Skip>().t;
    (void)skipref;
    return ret;
  }

  /*  */

  constexpr allocated_ref<as_values::AST_elem>
  parse_statement(const str_t &str) {
    using namespace mutils;
    using namespace cstring;
    if (contains_outside_parens(',', str)) {
      return parse_sequence(str);
    } else if (contains_outside_parens("var", str)) {
      assert(first_word_is("var", str));
      return parse_var(str);
    } else if (contains_outside_parens("return", str)) {
      assert(first_word_is("return", str));
      return parse_return(str);
    } else if (contains_outside_parens("while", str)) {
      assert(first_word_is("while", str));
      return parse_while(str);
    } else if (contains_outside_parens("if", str)) {
      assert(first_word_is("if", str));
      return parse_if(str);
    } else if (contains_outside_parens("=", str)) {
      return parse_assignment(str);
    } else if (first_word_is("{", str)) {
      str_nc new_string = {0};
      copy_within_parens(new_string, str);
      return parse_statement(new_string);
    } else
      throw "Ran off the end!";
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
    auto groups = split_outside_parens(',', str, string_bufs);
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
