#pragma once
#include "ast.hpp"

/*
<?php require_once 'common.php'; require_once 'util.php'; ?>
*/

using Alloc = as_values::AST_Allocator<50>;

template <typename string> struct parse {
  const string _str;
  using string_length = std::integral_constant<std::size_t,::mutils::cstring::str_len(string{}.str)>;
  using str_t = char const[string_length::value + 1];
  using str_nc = char[string_length::value + 1];
  Alloc allocator;

/*
<?php function parse_expr($subcase, ...$symbols ) : string {
  $ret = "";
  foreach ($symbols as $symbol){
    $ret = $ret."if (contains_outside_parens(\"$symbol\",str)){
      return parse_$subcase(str,\"$symbol\");
    } else ";
  }
  return $ret;

} ?>
*/

  constexpr allocated_ref<as_values::AST_elem> parse_binop(const str_t& str, const char* cause){
    using namespace mutils;
    using namespace cstring;
    <?php alloc("ret", "ref","BinOp")?>
    str_nc operands[2] = {0};
    last_split(cause,str,operands);
    ref.L = parse_expression(operands[0]);
    ref.R = parse_expression(operands[1]);
    ref.op = cause[0];
    return ret;
  }

  constexpr allocated_ref<as_values::AST_elem> parse_isvalid(const str_t& str, const char*){
    using namespace mutils;
    using namespace cstring;
    str_nc trimmed = {0};
    trim(trimmed,str);
    auto len = str_len(trimmed);
    assert(streq(".isValid()",trimmed + (len - str_len(".isValid()"))));
    // we should really end with .isValid() here
    for (auto i = len - str_len(".isValid()"); i < len; ++i){
      trimmed[i] = 0;
    }
    <?php alloc("ret", "ref","IsValid")?>
    ref.Hndl = parse_expression(trimmed);
    return ret;
  }

  constexpr allocated_ref<as_values::AST_elem> parse_fieldref(const str_t& str, const char* ){
    using namespace mutils;
    using namespace cstring;
    str_nc trimmed = {0};
    str_nc operands[2] = {{0}};
    trim(trimmed,str);
    last_split('.',trimmed,operands);
    <?php alloc("ret", "ref","FieldReference")?>
    ref.Struct = parse_expression(operands[0]);
    str_nc field_trimmed = {0};
    trim(field_trimmed,operands[1]);
    str_cpy(ref.Field, field_trimmed);
    return ret;
  }

  constexpr allocated_ref<as_values::AST_elem> parse_fieldptrref(const str_t& str, const char* cause){
    using namespace mutils;
    using namespace cstring;
    str_nc trimmed = {0};
    str_nc operands[2] = {{0}};
    trim(trimmed,str);
    last_split("->",trimmed,operands);
    <?php alloc("ret", "ref","FieldPointerReference")?>
    ref.Struct = parse_expression(operands[0]);
    str_nc field_trimmed = {0};
    trim(field_trimmed,operands[1]);
    str_cpy(ref.Field, field_trimmed);
    return ret;
  }

  constexpr allocated_ref<as_values::AST_elem> parse_deref(const str_t& str, const char* cause){
    using namespace mutils;
    using namespace cstring;
    str_nc trimmed = {0};
    trim(trimmed,str);
    assert(trimmed[0] = '*'); //this better be true;
    str_nc body = {0};
    str_cpy(body,trimmed+1);
    <?php alloc("ret", "ref","Dereference")?>
    ref.Struct = parse_expression(body);
    return ret;
  }

  constexpr allocated_ref<as_values::AST_elem> parse_constant(const str_t& str){
    using namespace mutils;
    using namespace cstring;
    <?php alloc("ret", "ref","Constant")?>
    ref.i = parse_int(str);
    return ret;
  }

  constexpr allocated_ref<as_values::AST_elem> parse_varref(const str_t& str){
    using namespace mutils;
    using namespace cstring;
    <?php alloc("ret", "ref","VarReference")?>
    str_cpy(ref.Var,str);
    return ret;
  }

  constexpr allocated_ref<as_values::AST_elem> parse_expression(const str_t &str) {
    using namespace mutils;
    using namespace cstring;
    <?php echo parse_expr("binop","+","- ","*","/","==","&&","||","!=") ?>
    <?php echo parse_expr("isvalid",".isValid()") ?>
    <?php echo parse_expr("fieldref",".")?>
    <?php echo parse_expr("fieldptrref","->")?>
    <?php echo parse_expr("deref","*")?>
    {
      //constants and variables here.
      str_nc atom = {0};
      trim(atom,str);
      static_assert('0' < '9');
      if (atom[0] >= '0' && atom[0] <= '9') return parse_constant(atom);
      else return parse_varref(atom);
    }
    throw "Ran off the end!";
  }

  constexpr allocated_ref<as_values::AST_elem> parse_binding(const str_t &str){
    using namespace mutils;
    using namespace cstring;
    <?php alloc("ret","binding","Binding")?>;
    str_nc binding_components[2] = {{0}};
    first_split('=',str,binding_components);
    trim(binding.var,binding_components[0]);
    binding.rhs = parse_expression(binding_components[1]);
    return ret;
  }

constexpr allocated_ref<as_values::AST_elem> parse_var(const str_t &str) {
    using namespace mutils;
    using namespace cstring;
    <?php alloc("ret","var","Let")?>
    str_nc let_expr = {0};
    remove_first_word(let_expr,str);
    str_nc let_components[2] = {{0}};
    first_split(',',let_expr,let_components);
    var.Binding = parse_binding(let_components[0]);
    var.Body = parse_statement(let_components[1]);
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
    <?php alloc("ret","ifref","If")?>
    str_nc if_condition = {0};
    auto if_offset = next_paren_group(if_condition,str);
    const char* post_condition = str + if_offset;
    str_nc then_body = {0};
    auto then_offset = next_paren_group(then_body,post_condition);
    const char* post_then = then_offset + post_condition;
    str_nc else_body = {0};
    ifref.condition = parse_expression(if_condition);
    ifref.then = parse_statement(then_body);
    if (first_word_is("else",post_then)){
      next_paren_group(else_body,post_then);
      ifref.els = parse_statement(else_body);
    }
    else {
      <?php alloc("ptr","skipref","Skip")?>
      (void)skipref;
      ifref.els = std::move(ptr);
    }
    return ret;
}
constexpr allocated_ref<as_values::AST_elem> parse_assignment(const str_t &str) {
    using namespace mutils;
    using namespace cstring;
    <?php alloc("ret","assignment","Assignment")?>
    str_nc string_bufs[2] = {{0}};
    split_outside_parens('=',str,string_bufs);
    assignment.Var = parse_expression(string_bufs[0]);
    assignment.Expr = parse_expression(string_bufs[1]);
    return ret;
}

/* <?php function parse_case($target) { 
    return "
    if (contains_outside_parens(\"$target\",str)) {
        //assert(first_word_is(\"$target\",str));
        return parse_$target(str);
    }
    ";
} ?> */

  constexpr allocated_ref<as_values::AST_elem> parse_statement(const str_t &str) {
    using namespace mutils;
    using namespace cstring;
    if (first_word_is("var",str)){
      return parse_var(str);
    }
    else if (contains_outside_parens(',',str)){
        return parse_sequence(str);
    }
    else <?php echo parse_case("return")?>
    else <?php echo parse_case("while")?>
    else <?php echo parse_case("if")?>
    else if (contains_outside_parens("=",str)){
        return parse_assignment(str);
    }
    else if (first_word_is("{",str)){
        str_nc new_string = {0};
        copy_within_parens(new_string, str);
        return parse_statement(new_string);
    }
    else if (str[0] == 0) {
      <?php echo alloc("ret","sr","Skip") ?>;
      (void) sr;
      return ret;
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
    allocator.top.e = parse_statement(local_copy);
  }
};
