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

auto test_input(){
    using namespace as_types;
    return Statement<sequence<Statement<skip>,Statement<return_val<Expression<number<5>>>>>>{};
  }
using round_trip_send = DECT(test_input());
struct prev_t{
    as_values::AST_Allocator<10> allocator = as_types::as_value<10,round_trip_send>();
    constexpr prev_t() {}
  };
struct round_trip_test{
  static constexpr prev_t prev{};
  constexpr round_trip_test(){}
};

using round_trip_return = DECT(as_values::as_type<round_trip_test>());

int main() {
  constexpr auto length = ::mutils::cstring::str_len("this is a string!")+1;
  struct wrapper {constexpr wrapper(){} const char str[length]{"this is a string!"};};
  constexpr flatten<wrapper> f;
  // flatten<str>::parse_t::print();
  //round_trip_return::print();
  static_assert(std::is_same<round_trip_return, round_trip_send>::value);
  return 0;
}
