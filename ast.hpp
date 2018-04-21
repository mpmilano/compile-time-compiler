#pragma once
#include "allocator.hpp"
#include "mutils/type_utils.hpp"
#include "union.hpp"

namespace as_values {
struct transaction;
struct plus;
struct skip;
using AST_elem = Union<transaction, plus, skip>;
template <std::size_t budget>
using AST_Allocator = Allocator<budget, transaction, AST_elem>;

struct transaction {
  allocated_ref<AST_elem> e;
  std::size_t payload{0};
  constexpr transaction(){};
  constexpr transaction(transaction &&p)
      : e{std::move(p.e)}, payload(std::move(p.payload)) {}
  constexpr transaction &operator=(transaction &&p) {
    e = std::move(p.e);
    payload = std::move(payload);
    return *this;
  }
};

struct plus {
  allocated_ref<AST_elem> e;
  std::size_t payload{0};
  constexpr plus() {}
  constexpr plus(plus &&p) : e{std::move(p.e)}, payload(std::move(p.payload)) {
    static_assert(
        true, "remember to use decltype(auto) in truly-polymorphic returns);");
  }
  constexpr plus &operator=(plus &&p) {
    e = std::move(p.e);
    payload = std::move(payload);
    return *this;
  }
};

struct skip {
  allocated_ref<AST_elem> e;
  std::size_t payload{0};
  constexpr skip() {}
  constexpr skip(skip &&p) : e{std::move(p.e)}, payload(std::move(p.payload)) {}

  constexpr skip &operator=(skip &&p) {
    e = std::move(p.e);
    payload = std::move(payload);
    return *this;
  }
};
} // namespace as_values

namespace as_types {
struct skip {};
template <typename L, typename R> struct plus {};
template <typename body> struct transaction {};

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
    if constexpr (e.template get_<transaction>().is_this_elem) {
      struct arg {
        constexpr arg() {}
        constexpr const AST_elem &operator()() const {
          return e.template get_<transaction>().t.e.get(allocator);
        }
      };
      using body = DECT(as_type<budget - 1, arg>(allocator));
      return as_types::transaction<body>{};
    }
    if constexpr (e.template get_<plus>().is_this_elem) {
      struct arg1 {
        constexpr arg1() {}
        constexpr const AST_elem &operator()() const {
          return e.template get_<plus>().t.e.get(allocator);
        }
      };
      struct arg2 {
        constexpr arg2() {}
        constexpr const AST_elem &operator()() const {
          return e.template get_<plus>().t.e.get(allocator);
        }
      };
      using left = DECT(as_type<budget - 1, arg1>(allocator));
      using right = DECT(as_type<budget - 1, arg2>(allocator));
      return as_types::plus<left, right>{};
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