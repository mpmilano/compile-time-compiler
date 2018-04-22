#include "ast.hpp"
#include "mutils/CTString.hpp"
#include "mutils/cstring.hpp"
#include <iostream>



using Alloc = as_values::AST_Allocator<15>;

template <typename string> struct parse {
  const string _str;
  using string_length = std::integral_constant<std::size_t,::mutils::cstring::str_len(string{}.str)>;
  using str_t = char const[string_length::value + 1];
  using str_nc = char[string_length::value + 1];
  Alloc allocator;

  constexpr allocated_ref<as_values::AST_elem> parse_expression(const str_t &str) {
    return allocated_ref<as_values::AST_elem>{};
  }

  constexpr allocated_ref<as_values::AST_elem> parse_statement(const str_t &str) {
    return allocated_ref<as_values::AST_elem>{};
  }

  constexpr allocated_ref<as_values::AST_elem> parse_sequence(const str_t &str) {
    using namespace mutils;
    using namespace cstring;
    str_nc string_bufs[20] = {{0}};
    auto groups = split_outside_parens(';', str, string_bufs);
    for (auto i = 0u; i < groups; ++i){
      
    }
    return allocated_ref<as_values::AST_elem>{};
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

template <typename string> struct flatten {
  // parsing happens during construction
  static const constexpr parse<string> prev{};
  static constexpr auto parse_as_type() {
    return as_values::as_type<flatten>();
  }
  using parse_t = DECT(parse_as_type());
  Alloc allocator;
  constexpr flatten() {}
};

int main() {
  constexpr auto length = ::mutils::cstring::str_len("this is a string!")+1;
  struct wrapper {constexpr wrapper(){} const char str[length]{"this is a string!"};};
  constexpr flatten<wrapper> f;
  // flatten<str>::parse_t::print();
  return 0;
}
