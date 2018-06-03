#pragma once
#include "ast.hpp"

/*
<?php require_once 'common.php'; require_once 'util.php'; ?>
*/

using Alloc = as_values::AST_Allocator<15>;

template <typename string> struct parse {
  const string _str;
  using string_length = std::integral_constant<std::size_t,::mutils::cstring::str_len(string{}.str)>;
  using str_t = char const[string_length::value + 1];
  using str_nc = char[string_length::value + 1];
  Alloc allocator;

  constexpr allocated_ref<as_values::AST_elem> parse_expression(const str_t &str) {
    <?php alloc("ret","voidref","Constant")?>
    (void)voidref;
    return ret;
  }

  constexpr allocated_ref<as_values::AST_elem> parse_statement(const str_t &str) {
    <?php alloc("ret","skipref","Skip")?>
    (void)skipref;
    return ret;
  }

  constexpr allocated_ref<as_values::AST_elem> parse_sequence(const str_t &str) {
    using namespace mutils;
    using namespace cstring;
    <?php alloc("ret","seqref","Sequence") ?>
    auto *seq = &seqref;
    str_nc string_bufs[20] = {{0}};
    auto groups = split_outside_parens(';', str, string_bufs);
    bool sequence_empty = true;
    assert(groups < 20);
    assert(groups > 0);
    for (auto i = 0u; i < groups; ++i){
      seq->e = parse_statement(string_bufs[i]);
      if ((i + 1) < groups){
        <?php alloc("newret","seqref","Sequence")?>
        seq->next = std::move(newret);
        seq = &seqref;
      }
      else {
        <?php alloc("skipref","voidref","Skip")?>
        (void)voidref;
        seq->next = std::move(skipref);
      }
    }
    return ret;
  }

  constexpr parse() {
    str_nc local_copy{0};
    constexpr string w;
    for (std::size_t i = 0; i < string_length::value; ++i){
      local_copy[i] = w.str[i];
    }
    // all parsing implemented in the constructor, so that
    // future things can just build this and expect it to work
    allocator.top.e = parse_sequence(local_copy);
  }
};
