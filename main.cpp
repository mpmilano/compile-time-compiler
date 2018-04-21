#include "ast.hpp"
#include "mutils/CTString.hpp"
#include "mutils/cstring.hpp"
#include <iostream>

#define $ITEM(trn, e, plus)                                                    \
  trn.e = allocator.template allocate<as_values::AST_elem>();                             \
  (trn.$(e) = as_values::plus{}).template get<as_values::plus>()
#define $PLUS(trn, e) $ITEM(trn, e, plus)

struct compiled {
  static constexpr as_values::AST_Allocator<15> test_compile() {
    as_values::AST_Allocator<15> allocator;
    auto &trn = allocator.top;
    $PLUS(trn, e).payload = 14;
    $ITEM(trn.$(e).template get<as_values::plus>(), e, skip).payload = 13;
    return allocator;
  }
};

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
    split_outside_parens(';', str, string_bufs);
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

template <typename compiled> struct tc {

  static constexpr const as_values::AST_Allocator<15> allocator =
      compiled::test_compile();

  template <std::size_t budget, typename F> auto evaluate() const {
    if constexpr (budget > 0) {
      constexpr const as_values::AST_elem &this_node = F{}();
      static_assert(this_node.well_formed());
      static_assert(!this_node.template get_<as_values::transaction>().is_this_elem);
      if constexpr (this_node.template get_<as_values::skip>().is_this_elem) {
        std::cout << "encountered skip with payload: "
                  << this_node.template get<as_values::skip>().payload << std::endl;
        return this_node.template get<as_values::skip>().payload;
      } else if constexpr (this_node.template get_<as_values::plus>().is_this_elem) {
        constexpr auto &plusv = this_node.template get_<as_values::plus>();
        struct left {
          constexpr const as_values::AST_elem &operator()() const {
            // constexpr auto& this_node = F{}();
            return /*plusv.*/ F{}().template get<as_values::plus>().$(e);
          }
        };

        std::cout << "encountered plus with payload: " << plusv.t.payload
                  << " whose child has index " << plusv.t.e.indx << std::endl;
        return plusv.t.payload + evaluate<budget - 1, left>();
      } else
        return 0;
    } else
      return 0;
  }

  auto evaluate() const {
    struct start {
      constexpr const as_values::AST_elem &operator()() const {
        return allocator.top.$(e);
      }
    };
    return evaluate<15, start>();
  }
  constexpr tc() {}
};

int main() {
  constexpr tc<compiled> testing;
  std::cout << testing.evaluate() << std::endl;
  constexpr auto allocator = compiled::test_compile();
  constexpr auto fourteen = allocator.top.$(e).template get<as_values::plus>().payload;
  static_assert(fourteen == 14);
  constexpr auto length = ::mutils::cstring::str_len("this is a string!")+1;
  struct wrapper {constexpr wrapper(){} const char str[length]{"this is a string!"};};
  constexpr flatten<wrapper> f;
  // flatten<str>::parse_t::print();
  std::cout << fourteen << " " /*<< str{} */ << std::endl;
  return fourteen;
}
