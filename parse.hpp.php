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
    using namespace mutils;
    using namespace cstring;
    <?php alloc("ret","voidref","Constant")?>
    (void)voidref;
    return ret;
  }

constexpr allocated_ref<as_values::AST_elem> parse_var(const str_t &str) {
    using namespace mutils;
    using namespace cstring;
    <?php alloc("ret","skipref","Skip")?>
    (void)skipref;
    return ret;
}
constexpr allocated_ref<as_values::AST_elem> parse_return(const str_t &str) {
    using namespace mutils;
    using namespace cstring;
    <?php alloc("ret","retref","Return")?>
    str_nc next = {0};
    remove_first_word(next,str);
    retref.Expr = parse_expression(next);
    return ret;
}
constexpr allocated_ref<as_values::AST_elem> parse_while(const str_t &str) {
    using namespace mutils;
    using namespace cstring;
    <?php alloc("ret","whileref","While")?>
    str_nc without_first = {0};
    remove_first_word(without_first,str);
    str_nc condition = {0};
    auto offset = next_paren_group(condition,without_first);
    str_nc body = {0};
    copy_within_parens(body, without_first + offset);
    whileref.condition = parse_expression(condition);
    whileref.body = parse_statement(body);
    return ret;
}
constexpr allocated_ref<as_values::AST_elem> parse_if(const str_t &str) {
    using namespace mutils;
    using namespace cstring;
    <?php alloc("ret","skipref","Skip")?>
    (void)skipref;
    return ret;
}
constexpr allocated_ref<as_values::AST_elem> parse_assignment(const str_t &str) {
    using namespace mutils;
    using namespace cstring;
    <?php alloc("ret","skipref","Skip")?>
    (void)skipref;
    return ret;
}

/* <?php function statement_case($target) { 
    return "
    if (contains_outside_parens(\"$target\",str)) {
        assert(first_word_is(\"$target\",str));
        return parse_$target(str);
    }
    ";
} ?> */

  constexpr allocated_ref<as_values::AST_elem> parse_statement(const str_t &str) {
    using namespace mutils;
    using namespace cstring;
    if (contains_outside_parens(',',str)){
        return parse_sequence(str);
    }
    else <?php echo statement_case("var")?>
    else <?php echo statement_case("return")?>
    else <?php echo statement_case("while")?>
    else <?php echo statement_case("if")?>
    else if (contains_outside_parens("=",str)){
        return parse_assignment(str);
    }
    else if (first_word_is("{",str)){
        str_nc new_string = {0};
        copy_within_parens(new_string, str);
        return parse_statement(new_string);
    }
    else throw "Ran off the end!";
  }

  constexpr allocated_ref<as_values::AST_elem> parse_sequence(const str_t &str) {
    using namespace mutils;
    using namespace cstring;
    <?php alloc("ret","seqref","Sequence") ?>
    auto *seq = &seqref;
    str_nc string_bufs[20] = {{0}};
    auto groups = split_outside_parens(',', str, string_bufs);
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
