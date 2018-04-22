
#pragma once
#include "allocator.hpp"
#include "mutils/type_utils.hpp"
#include "union.hpp"

#define generate_value7(name, ftype1, f1, f1init, ftype2, f2, f2init)          \
  struct name {                                                                \
    ftype1 f1 f1init;                                                          \
    ftype2 f2 f2init;                                                          \
    constexpr name(){};                                                        \
    constexpr name(name &&p) : f1{std::move(p.f1)}, f2(std::move(p.f2)) {}     \
    constexpr name &operator=(name &&p) {                                      \
      f1 = std::move(p.f1);                                                    \
      f2 = std::move(f2);                                                      \
      return *this;                                                            \
    }                                                                          \
  };

#define generate_value4(name, ftype1, f1, f1init)                              \
  struct name {                                                                \
    ftype1 f1 f1init;                                                          \
    constexpr name(){};                                                        \
    constexpr name(name &&p) : f1{std::move(p.f1)} {}                          \
    constexpr name &operator=(name &&p) {                                      \
      f1 = std::move(p.f1);                                                    \
      return *this;                                                            \
    }                                                                          \
  };

#define generate_value1(name)                                                  \
  struct name {                                                                \
    constexpr name() {}                                                        \
  }

#define generate_value_IMPL2(count, ...) generate_value##count(__VA_ARGS__)
#define generate_value_IMPL(count, ...) generate_value_IMPL2(count, __VA_ARGS__)
#define generate_value(...)                                                    \
  generate_value_IMPL(VA_NARGS(__VA_ARGS__), __VA_ARGS__)

namespace as_values {
struct transaction;
struct sequence;
struct plus;
struct number;
struct return_val;
struct skip;
using AST_elem = Union<transaction, sequence, plus, number, return_val, skip>;
template <std::size_t budget>
using AST_Allocator = Allocator<budget, transaction, AST_elem>;

generate_value(transaction, allocated_ref<AST_elem>, e, {}, std::size_t,
               payload, {0});
generate_value(sequence, allocated_ref<AST_elem>, e, {},
               allocated_ref<AST_elem>, next, {});
generate_value(plus, allocated_ref<AST_elem>, l, {}, allocated_ref<AST_elem>, r,
               {});
generate_value(number, std::size_t, num, {0});
generate_value(return_val, allocated_ref<AST_elem>, v, {});
generate_value(skip);
} // namespace as_values

namespace as_types {
template <typename e, std::size_t payload> struct transaction {};
template <typename e, typename next> struct sequence {};
template <typename l, typename r> struct plus {};
template <std::size_t num> struct number {};
template <typename v> struct return_val {};
struct skip {};

} // namespace as_types

namespace as_values {
template <std::size_t budget, typename F, std::size_t alloc_budget,
          typename... allocates>
constexpr auto as_type(const Allocator<alloc_budget, allocates...> &allocator) {
  static_assert(budget > 0);
  if (budget > 0) {
    constexpr const AST_elem &e = F{}();
    if constexpr (e.template get_<transaction>().is_this_elem) {
      struct arg0 {
        constexpr arg0() {}
        constexpr const AST_elem &operator()() const {
          return e.template get_<transaction>().t.e.get(allocator);
        }
      };

      using _arg0 = DECT(as_type<budget - 1, arg0>(allocator));
      constexpr auto _arg1 = e.template get_<transaction>().t.payload;
      return as_types::transaction<_arg0, _arg1>{};
    }
    if constexpr (e.template get_<sequence>().is_this_elem) {
      struct arg0 {
        constexpr arg0() {}
        constexpr const AST_elem &operator()() const {
          return e.template get_<sequence>().t.e.get(allocator);
        }
      };

      using _arg0 = DECT(as_type<budget - 1, arg0>(allocator));
      struct arg1 {
        constexpr arg1() {}
        constexpr const AST_elem &operator()() const {
          return e.template get_<sequence>().t.next.get(allocator);
        }
      };

      using _arg1 = DECT(as_type<budget - 1, arg1>(allocator));
      return as_types::sequence<_arg0, _arg1>{};
    }
    if constexpr (e.template get_<plus>().is_this_elem) {
      struct arg0 {
        constexpr arg0() {}
        constexpr const AST_elem &operator()() const {
          return e.template get_<plus>().t.l.get(allocator);
        }
      };

      using _arg0 = DECT(as_type<budget - 1, arg0>(allocator));
      struct arg1 {
        constexpr arg1() {}
        constexpr const AST_elem &operator()() const {
          return e.template get_<plus>().t.r.get(allocator);
        }
      };

      using _arg1 = DECT(as_type<budget - 1, arg1>(allocator));
      return as_types::plus<_arg0, _arg1>{};
    }
    if constexpr (e.template get_<number>().is_this_elem) {
      constexpr auto _arg0 = e.template get_<number>().t.num;
      return as_types::number<_arg0>{};
    }
    if constexpr (e.template get_<return_val>().is_this_elem) {
      struct arg0 {
        constexpr arg0() {}
        constexpr const AST_elem &operator()() const {
          return e.template get_<return_val>().t.v.get(allocator);
        }
      };

      using _arg0 = DECT(as_type<budget - 1, arg0>(allocator));
      return as_types::return_val<_arg0>{};
    }
    if constexpr (e.template get_<skip>().is_this_elem) {
      return as_types::skip{};
    }
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

namespace as_types {

template <typename AST_Allocator> struct as_values_ns_fns {
  template <typename e, std::size_t payload>
  constexpr static void as_value(AST_Allocator &allocator,
                                 const transaction<e, payload> &) {}
  template <typename e, typename next>
  constexpr static void as_value(AST_Allocator &allocator,
                                 const sequence<e, next> &) {}
  template <typename l, typename r>
  constexpr static void as_value(AST_Allocator &allocator, const plus<l, r> &) {

  }
  template <std::size_t num>
  constexpr static void as_value(AST_Allocator &allocator,
                                 const number<num> &) {}
  template <typename v>
  constexpr static void as_value(AST_Allocator &allocator,
                                 const return_val<v> &) {}
  constexpr static void as_value(AST_Allocator &allocator, const skip &) {}
};

template <std::size_t budget, typename hd>
constexpr as_values::AST_Allocator<budget> as_value() {
  as_values::AST_Allocator<budget> head;
  return as_values_ns_fns<as_values::AST_Allocator<budget>>::as_value(head,
                                                                      hd{});
}
} // namespace as_types