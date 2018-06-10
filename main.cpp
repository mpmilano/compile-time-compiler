#include "ast.hpp"
#include "mutils/CTString.hpp"
#include "mutils/cstring.hpp"
#include "parse.hpp"
#include "pretty_print.hpp"
#include <iostream>

/*

*/

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

auto test_input() {
  using namespace as_types;
  using namespace mutils;
  return Statement<transaction<
      Statement<Sequence<
          Statement<Operation<String<'t'>, Expression<Constant<5>>,
                              operation_args_exprs<>,
                              operation_args_varrefs<>> /*Skip*/>,
          Statement<Sequence<
              Statement<LetRemote<Binding<mutils::String<'b', 'a', 'r'>,
                                          Expression<Constant<9>>>,
                                  Statement<Skip>>>,
              Statement<
                  Sequence<Statement<Let<Binding<mutils::String<'f', 'o', 'o'>,
                                                 Expression<Constant<7>>>,
                                         Statement<Skip>>>,
                           Statement<Return<Expression<VarReference<
                               mutils::String<'f', 'o', 'o'>>>>>>>>>>>,
      0>>{};
}
using round_trip_send = DECT(test_input());
struct prev_t {
  as_values::AST_Allocator<50> allocator =
      as_types::as_value<50, round_trip_send>();
  constexpr prev_t() {}
};
struct round_trip_test {
  static constexpr prev_t prev{};
  constexpr round_trip_test() {}
};

using round_trip_return = DECT(as_values::as_type<round_trip_test>());

#define CSTR(wrapper, x...)                                                    \
  struct wrapper {                                                             \
    constexpr wrapper() {}                                                     \
    const char str[::mutils::cstring::str_len(#x) + 1]{#x};                    \
  };

int main() {
  CSTR(wrapper, var iterator = users, var b1 = 89.endorse(words),
       var b2 = 4894.ensure(otherwords), b2 = 2341,
       var b3 = *b2.field.more.field->field.isValid(),
       if (true || false) { return b1 } else {return b2},
       while (iterator.isValid()){iterator = iterator->next}, return b1);
  parse<wrapper> p;
  pretty_print(std::cout, p.allocator.top, p.allocator);
  std::cout << std::endl << "Original string: " << wrapper{}.str << std::endl;

  constexpr flatten<wrapper> f;
  constexpr auto second_allocator =
      as_types::as_value<50, typename flatten<wrapper>::parse_t>();
  pretty_print(std::cout, second_allocator.top, second_allocator); //*/
  std::cout << std::endl << std::endl;
  // print(std::cout, f.prev.allocator.top,f.prev.allocator);
  // std::cout << std::endl;
  // flatten<str>::parse_t::print();
  round_trip_return::print();
  // static_assert(std::is_same<round_trip_return, round_trip_send>::value);
  constexpr prev_t to_print;

  pretty_print(std::cout, to_print.allocator.top, to_print.allocator);
  std::cout << std::endl;
  return 0;
}
