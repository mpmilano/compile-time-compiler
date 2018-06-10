#pragma once
#include "ast.hpp"

/*

*/

using Alloc = as_values::AST_Allocator<50>;

template <typename string> struct parse {
  const string _str;
  using string_length =
      std::integral_constant<std::size_t,
                             ::mutils::cstring::str_len(string{}.str)>;
  using str_t = char const[string_length::value + 1];
  using str_nc = char[string_length::value + 1];
  Alloc allocator;

  constexpr allocated_ref<as_values::AST_elem> parse_binop(const str_t &str,
                                                           const char *cause) {
    using namespace mutils;
    using namespace cstring;

    allocated_ref<as_values::AST_elem> ret =
        allocator.template allocate<as_values::AST_elem>();
    ret.get(allocator).template get_<as_values::BinOp>().is_this_elem = true;
    auto &ref = ret.get(allocator).template get_<as_values::BinOp>().t;
    str_nc operands[2] = {0};
    last_split(cause, str, operands);
    ref.L = parse_expression(operands[0]);
    ref.R = parse_expression(operands[1]);
    ref.op = cause[0];
    return ret;
  }

  constexpr allocated_ref<as_values::AST_elem>
  parse_builtin_op(const str_t &str, const char *specific_op) {
    /*
     */
    using namespace mutils;
    using namespace cstring;
    str_nc trimmed = {0};
    trim(trimmed, str);
    str_nc split[2] = {{0}};
    last_split('.', trimmed, split);
    str_nc op_args = {0};
    copy_within_parens(op_args, split[1]);
    switch (specific_op[3]) {
    case 'd': {
      allocated_ref<as_values::AST_elem> ret =
          allocator.template allocate<as_values::AST_elem>();
      ret.get(allocator).template get_<as_values::Endorse>().is_this_elem =
          true;
      auto &ref = ret.get(allocator).template get_<as_values::Endorse>().t;
      str_nc final_str = {0};
      ref.Hndl = parse_expression(split[0]);
      str_cpy(ref.label.label, op_args);
      return ret;
    }
      // endorse
    case 's': {
      allocated_ref<as_values::AST_elem> ret =
          allocator.template allocate<as_values::AST_elem>();
      ret.get(allocator).template get_<as_values::Ensure>().is_this_elem = true;
      auto &ref = ret.get(allocator).template get_<as_values::Ensure>().t;
      str_nc final_str = {0};
      ref.Hndl = parse_expression(split[0]);
      str_cpy(ref.label.label, op_args);
      return ret;
    }
      // ensure
    case 'V': {
      allocated_ref<as_values::AST_elem> ret =
          allocator.template allocate<as_values::AST_elem>();
      ret.get(allocator).template get_<as_values::IsValid>().is_this_elem =
          true;
      auto &ref = ret.get(allocator).template get_<as_values::IsValid>().t;
      str_nc final_str = {0};
      ref.Hndl = parse_expression(split[0]);
      return ret;
    }
      // isValid
    }
    throw "ran off the end";
  }

  constexpr allocated_ref<as_values::AST_elem>
  parse_operation(const str_t &str) {
    using namespace mutils;
    using namespace cstring;

    allocated_ref<as_values::AST_elem> ret =
        allocator.template allocate<as_values::AST_elem>();
    ret.get(allocator).template get_<as_values::Constant>().is_this_elem = true;
    auto &ref = ret.get(allocator).template get_<as_values::Constant>().t;
    return ret;
  }

  constexpr allocated_ref<as_values::AST_elem>
  parse_fieldref(const str_t &str) {
    using namespace mutils;
    using namespace cstring;
    str_nc trimmed = {0};
    str_nc operands[2] = {{0}};
    trim(trimmed, str);
    last_split('.', trimmed, operands);
    {
      // error checking
      if (contains_paren(operands[1]))
        throw "Parse error: We thought this was a fieldref, but it contains "
              "parens";
      if (contains_outside_parens('.', operands[1]))
        throw "Parse error: This should be a field, but it contains a '.', "
              "which is not allowed";
      if (contains(' ', operands[1]))
        throw "Parse error: a space snuck in here somehow";
    }

    allocated_ref<as_values::AST_elem> ret =
        allocator.template allocate<as_values::AST_elem>();
    ret.get(allocator).template get_<as_values::FieldReference>().is_this_elem =
        true;
    auto &ref = ret.get(allocator).template get_<as_values::FieldReference>().t;
    ref.Struct = parse_expression(operands[0]);
    str_nc field_trimmed = {0};
    trim(field_trimmed, operands[1]);
    str_cpy(ref.Field, field_trimmed);
    return ret;
  }

  constexpr allocated_ref<as_values::AST_elem>
  parse_fieldptrref(const str_t &str, const char *cause) {
    using namespace mutils;
    using namespace cstring;
    str_nc trimmed = {0};
    str_nc operands[2] = {{0}};
    trim(trimmed, str);
    last_split("->", trimmed, operands);
    {
      // error checking
      if (contains_paren(operands[1]))
        throw "Parse error: We thought this should be a field, but it contains "
              "parens";
      if (contains_outside_parens('.', operands[1]))
        throw "Parse error: This should be a field, but it contains a '.', "
              "which is not allowed";
      if (contains(' ', operands[1]))
        throw "Parse error: a space snuck in here somehow";
    }

    allocated_ref<as_values::AST_elem> ret =
        allocator.template allocate<as_values::AST_elem>();
    ret.get(allocator)
        .template get_<as_values::FieldPointerReference>()
        .is_this_elem = true;
    auto &ref =
        ret.get(allocator).template get_<as_values::FieldPointerReference>().t;
    ref.Struct = parse_expression(operands[0]);
    str_nc field_trimmed = {0};
    trim(field_trimmed, operands[1]);
    str_cpy(ref.Field, field_trimmed);
    return ret;
  }

  constexpr allocated_ref<as_values::AST_elem> parse_deref(const str_t &str,
                                                           const char *cause) {
    using namespace mutils;
    using namespace cstring;
    str_nc trimmed = {0};
    trim(trimmed, str);
    assert(trimmed[0] = '*'); // this better be true;
    str_nc body = {0};
    str_cpy(body, trimmed + 1);

    allocated_ref<as_values::AST_elem> ret =
        allocator.template allocate<as_values::AST_elem>();
    ret.get(allocator).template get_<as_values::Dereference>().is_this_elem =
        true;
    auto &ref = ret.get(allocator).template get_<as_values::Dereference>().t;
    ref.Struct = parse_expression(body);
    return ret;
  }

  constexpr allocated_ref<as_values::AST_elem>
  parse_constant(const str_t &str) {
    using namespace mutils;
    using namespace cstring;

    allocated_ref<as_values::AST_elem> ret =
        allocator.template allocate<as_values::AST_elem>();
    ret.get(allocator).template get_<as_values::Constant>().is_this_elem = true;
    auto &ref = ret.get(allocator).template get_<as_values::Constant>().t;
    ref.i = parse_int(str);
    return ret;
  }

  constexpr allocated_ref<as_values::AST_elem> parse_varref(const str_t &str) {
    using namespace mutils;
    using namespace cstring;

    allocated_ref<as_values::AST_elem> ret =
        allocator.template allocate<as_values::AST_elem>();
    ret.get(allocator).template get_<as_values::VarReference>().is_this_elem =
        true;
    auto &ref = ret.get(allocator).template get_<as_values::VarReference>().t;
    trim(ref.Var, str);
    {
      // error checking
      if (contains_paren(ref.Var))
        throw "Parse error: We thought this should be a variable, but it "
              "contains parens";
      if (contains_outside_parens('.', ref.Var))
        throw "Parse error: This should be a variable, but it contains a '.', "
              "which is not allowed";
      if (contains(' ', ref.Var))
        throw "Parse error: a space snuck in here somehow";
    }
    return ret;
  }

  /*
   */

  constexpr allocated_ref<as_values::AST_elem>
  parse_expression(const str_t &str) {
    using namespace mutils;
    using namespace cstring;
    if (contains_outside_parens("+", str)) {
      return parse_binop(str, "+");
    } else if (contains_outside_parens("- ", str)) {
      return parse_binop(str, "- ");
    } else if (contains_outside_parens("* ", str)) {
      return parse_binop(str, "* ");
    } else if (contains_outside_parens("/", str)) {
      return parse_binop(str, "/");
    } else if (contains_outside_parens("==", str)) {
      return parse_binop(str, "==");
    } else if (contains_outside_parens("&&", str)) {
      return parse_binop(str, "&&");
    } else if (contains_outside_parens("||", str)) {
      return parse_binop(str, "||");
    } else if (contains_outside_parens("!=", str)) {
      return parse_binop(str, "!=");
    } else if (contains_outside_parens(".", str)) {
      str_nc pretrim_splits[2] = {{0}};
      last_split(".", str, pretrim_splits);
      str_nc splits[2] = {{0}};
      trim(splits[0], pretrim_splits[0]);
      trim(splits[1], pretrim_splits[1]);
      assert(!contains_outside_parens(".", splits[1]));
      if (contains_outside_parens("->", splits[1])) {
        return parse_fieldptrref(str, "->");
      } else if (contains_outside_parens(".isValid(", splits[1])) {
        return parse_builtin_op(str, ".isValid(");
      } else if (contains_outside_parens(".endorse(", splits[1])) {
        return parse_builtin_op(str, ".endorse(");
      } else if (contains_outside_parens(".ensure(", splits[1])) {
        return parse_builtin_op(str, ".ensure(");
      } else if (contains_paren(splits[1])) {
        return parse_operation(str);
      } else {
        // it's just a normal string at this point.
        return parse_fieldref(str);
      }
    } else if (contains_outside_parens("->", str)) {
      return parse_fieldptrref(str, "->");
    } else if (contains_outside_parens("*", str)) {
      return parse_deref(str, "*");
    } else {
      // constants and variables here.
      str_nc atom = {0};
      trim(atom, str);
      static_assert('0' < '9');
      if (atom[0] >= '0' && atom[0] <= '9')
        return parse_constant(atom);
      else
        return parse_varref(atom);
    }
    throw "Ran off the end!";
  }

  constexpr allocated_ref<as_values::AST_elem> parse_binding(const str_t &str) {
    using namespace mutils;
    using namespace cstring;

    allocated_ref<as_values::AST_elem> ret =
        allocator.template allocate<as_values::AST_elem>();
    ret.get(allocator).template get_<as_values::Binding>().is_this_elem = true;
    auto &binding = ret.get(allocator).template get_<as_values::Binding>().t;
    ;
    str_nc binding_components[2] = {{0}};
    first_split('=', str, binding_components);
    trim(binding.var, binding_components[0]);
    binding.rhs = parse_expression(binding_components[1]);
    return ret;
  }

  constexpr allocated_ref<as_values::AST_elem> parse_var(const str_t &str) {
    using namespace mutils;
    using namespace cstring;

    allocated_ref<as_values::AST_elem> ret =
        allocator.template allocate<as_values::AST_elem>();
    ret.get(allocator).template get_<as_values::Let>().is_this_elem = true;
    auto &var = ret.get(allocator).template get_<as_values::Let>().t;
    str_nc let_expr = {0};
    remove_first_word(let_expr, str);
    str_nc let_components[2] = {{0}};
    first_split(',', let_expr, let_components);
    var.Binding = parse_binding(let_components[0]);
    var.Body = parse_statement(let_components[1]);
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
    ret.get(allocator).template get_<as_values::If>().is_this_elem = true;
    auto &ifref = ret.get(allocator).template get_<as_values::If>().t;
    str_nc if_condition = {0};
    auto if_offset = next_paren_group(if_condition, str);
    const char *post_condition = str + if_offset;
    str_nc then_body = {0};
    auto then_offset = next_paren_group(then_body, post_condition);
    const char *post_then = then_offset + post_condition;
    str_nc else_body = {0};
    ifref.condition = parse_expression(if_condition);
    ifref.then = parse_statement(then_body);
    if (first_word_is("else", post_then)) {
      next_paren_group(else_body, post_then);
      ifref.els = parse_statement(else_body);
    } else {

      allocated_ref<as_values::AST_elem> ptr =
          allocator.template allocate<as_values::AST_elem>();
      ptr.get(allocator).template get_<as_values::Skip>().is_this_elem = true;
      auto &skipref = ptr.get(allocator).template get_<as_values::Skip>().t;
      (void)skipref;
      ifref.els = std::move(ptr);
    }
    return ret;
  }
  constexpr allocated_ref<as_values::AST_elem>
  parse_assignment(const str_t &str) {
    using namespace mutils;
    using namespace cstring;

    allocated_ref<as_values::AST_elem> ret =
        allocator.template allocate<as_values::AST_elem>();
    ret.get(allocator).template get_<as_values::Assignment>().is_this_elem =
        true;
    auto &assignment =
        ret.get(allocator).template get_<as_values::Assignment>().t;
    str_nc string_bufs[2] = {{0}};
    split_outside_parens('=', str, string_bufs);
    assignment.Var = parse_expression(string_bufs[0]);
    assignment.Expr = parse_expression(string_bufs[1]);
    return ret;
  }

  /*  */

  constexpr allocated_ref<as_values::AST_elem>
  parse_statement(const str_t &str) {
    using namespace mutils;
    using namespace cstring;
    if (first_word_is("var", str)) {
      return parse_var(str);
    } else if (contains_outside_parens(',', str)) {
      return parse_sequence(str);
    } else if (contains_outside_parens("return", str)) {
      // assert(first_word_is("return",str));
      return parse_return(str);
    } else if (contains_outside_parens("while", str)) {
      // assert(first_word_is("while",str));
      return parse_while(str);
    } else if (contains_outside_parens("if", str)) {
      // assert(first_word_is("if",str));
      return parse_if(str);
    } else if (contains_outside_parens("=", str)) {
      return parse_assignment(str);
    } else if (first_word_is("{", str)) {
      str_nc new_string = {0};
      copy_within_parens(new_string, str);
      return parse_statement(new_string);
    } else if (str[0] == 0) {

      allocated_ref<as_values::AST_elem> ret =
          allocator.template allocate<as_values::AST_elem>();
      ret.get(allocator).template get_<as_values::Skip>().is_this_elem = true;
      auto &sr = ret.get(allocator).template get_<as_values::Skip>().t;
      ;
      (void)sr;
      return ret;
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
    using namespace mutils::cstring;
    if (contains(';', local_copy))
      throw "Parse Error: Semicolons have no place here.  Did you mean ','? ";
    allocator.top.e = parse_statement(local_copy);
  }
};
