#pragma once
#include "allocator.hpp"
#include "mutils/type_utils.hpp"
#include "union.hpp"

#define generate_value7(name,ftype1,f1,f1init,ftype2,f2,f2init) \
    struct name { \
  ftype1 f1 f1init; \
  ftype2 f2 f2init; \
  constexpr name(){}; \
  constexpr name(name &&p) \
      : f1{std::move(p.f1)}, f2(std::move(p.f2)) {} \
  constexpr name &operator=(name &&p) { \
    f1 = std::move(p.f1); \
    f2 = std::move(f2); \
    return *this; \
  } \
};

#define generate_value4(name,ftype1,f1,f1init) \
    struct name { \
  ftype1 f1 f1init; \
  constexpr name(){}; \
  constexpr name(name &&p) \
      : f1{std::move(p.f1)}{} \
  constexpr name &operator=(name &&p) { \
    f1 = std::move(p.f1); \
    return *this; \
  } \
};

#define generate_internal_value3(name,f1,f2) \
generate_value7(name,allocated_ref<AST_elem>,f1,{},allocated_ref<AST_elem>,f2,{})

#define generate_internal_value2(name,f1) \
generate_value4(name,allocated_ref<AST_elem>,f1,{})

#define convert3(name,l,r)\
 if constexpr (e.template get_<name>().is_this_elem) {\
      struct arg1 {\
        constexpr arg1() {}\
        constexpr const AST_elem &operator()() const {\
          return e.template get_<name>().t.l.get(allocator);\
        }\
      };\
      struct arg2 {\
        constexpr arg2() {}\
        constexpr const AST_elem &operator()() const {\
          return e.template get_<name>().t.r.get(allocator);\
        }\
      };\
      using left = DECT(as_type<budget - 1, arg1>(allocator));\
      using right = DECT(as_type<budget - 1, arg2>(allocator));\
      return as_types::name<left, right>{};}

#define convert2(name,l)\
 if constexpr (e.template get_<name>().is_this_elem) {\
      struct arg1 {\
        constexpr arg1() {}\
        constexpr const AST_elem &operator()() const {\
          return e.template get_<name>().t.l.get(allocator);\
        }\
      };\
      using left = DECT(as_type<budget - 1, arg1>(allocator));\
      return as_types::name<left>{};}

namespace as_values {
struct transaction;
struct plus;
struct skip;
struct number;
struct return_val;
struct sequence;
using AST_elem = Union<transaction, plus, skip, number, return_val, sequence>;
template <std::size_t budget>
using AST_Allocator = Allocator<budget, transaction, AST_elem>;

generate_value7(transaction,allocated_ref<AST_elem>, e, {},std::size_t, payload,{0});
generate_internal_value3(sequence,e,next);
generate_internal_value3(plus,l,r);
generate_value4(number,std::size_t,num,{0});
generate_internal_value2(return_val,v);
struct skip {
  constexpr skip() {}
  constexpr skip(skip &&p) {}
  constexpr skip &operator=(skip &&) { return *this; }
};
} // namespace as_values

namespace as_types {
struct skip {};
template <typename L, typename R> struct plus {};
template <typename body> struct transaction {};
template <std::size_t> struct number {};
template <typename val> struct return_val {};
template <typename... seqs> struct sequence {
  template <typename... more>
  static constexpr sequence<seqs..., more...>
  append(const sequence<more...> &) {
    return sequence<seqs..., more...>{};
  }

  template<typename T> static constexpr auto append(const T&){
    return sequence<seqs...,T>{};
  }
};

} // namespace as_types

namespace as_values {
template <std::size_t budget, typename F, std::size_t alloc_budget,
          typename... allocates>
constexpr auto as_type(const Allocator<alloc_budget, allocates...> &allocator) {
  static_assert(budget > 0);
  if (budget > 0) {
    constexpr const AST_elem &e = F{}();
    if constexpr (e.template get_<skip>().is_this_elem) {
      return as_types::skip{};
    }
    convert2(transaction,e);
    convert3(plus,l,r);
    if constexpr (e.template get_<number>().is_this_elem) {
      return as_types::number<e.template get_<number>().t.num>{};
    }
    convert2(return_val,v);
    convert3(sequence,e,next);
  }
  static_assert(budget > 0);
  struct error {};
  return error{};
}

template <typename prev_holder> constexpr auto as_type() {
  struct arg {
    constexpr arg() {}
    constexpr const AST_elem &operator()() const {
      return prev_holder::prev.allocator.top.e.get(prev_holder::prev.allocator);
    }
  };
  return as_type<15, arg>(prev_holder::prev.allocator);
}
} // namespace as_values