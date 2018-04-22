#pragma once
#include "allocator.hpp"
#include "mutils/type_utils.hpp"
#include "union.hpp"

#define generate_value7(name,ftype1,f1,f1init,ftype2,f2,f2init) \
    struct $name { \
  ftype1 f1 f1init; \
  ftype2 f2 f2init; \
  constexpr $name(){}; \
  constexpr $name($name &&p) \
      : f1{std::move(p.f1)}, f2(std::move(p.f2)) {} \
  constexpr $name &operator=($name &&p) { \
    f1 = std::move(p.f1); \
    f2 = std::move(f2); \
    return *this; \
  } \
};
  

namespace as_values {
struct transaction;
struct plus;
struct skip;
struct number;
struct return_val;
using AST_elem = Union<transaction, plus, skip, number, return_val>;
template <std::size_t budget>
using AST_Allocator = Allocator<budget, transaction, AST_elem>;

generate_value7(transaction,allocated_ref<AST_elem>, e, {},std::size_t, payload,{0});


struct sequence {
  allocated_ref<AST_elem> next;
  allocated_ref<AST_elem> e;
  std::size_t payload{0};
  constexpr sequence(){};
  constexpr sequence(sequence &&p)
      : e{std::move(p.e)}, next(std::move(p.next)) {}
  constexpr sequence &operator=(sequence &&p) {
    e = std::move(p.e);
    next = std::move(next);
    return *this;
  }
};

struct plus {
  allocated_ref<AST_elem> l;
  allocated_ref<AST_elem> r;
  constexpr plus() {}
  constexpr plus(plus &&p) : l{std::move(p.l)}, r(std::move(p.r)) {
    static_assert(
        true, "remember to use decltype(auto) in truly-polymorphic returns);");
  }
  constexpr plus &operator=(plus &&p) {
    l = std::move(p.l);
    r = std::move(p.r);
    return *this;
  }
};

struct number {
  std::size_t num{0};
  constexpr number() {}
  constexpr number(number &&p) : num{std::move(p.num)} {
    static_assert(
        true, "remember to use decltype(auto) in truly-polymorphic returns);");
  }
  constexpr number &operator=(number &&p) {
    num = std::move(p.num);
    return *this;
  }
};

struct return_val {
  allocated_ref<AST_elem> v;
  constexpr return_val() {}
  constexpr return_val(return_val &&p) : v{std::move(p.v)} {
    static_assert(
        true, "remember to use decltype(auto) in truly-polymorphic returns);");
  }
  constexpr return_val &operator=(return_val &&p) {
    v = std::move(p.v);
    return *this;
  }
};

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
          return e.template get_<plus>().t.l.get(allocator);
        }
      };
      struct arg2 {
        constexpr arg2() {}
        constexpr const AST_elem &operator()() const {
          return e.template get_<plus>().t.r.get(allocator);
        }
      };
      using left = DECT(as_type<budget - 1, arg1>(allocator));
      using right = DECT(as_type<budget - 1, arg2>(allocator));
      return as_types::plus<left, right>{};
    }
    if constexpr (e.template get_<number>().is_this_elem) {
      return as_types::number<e.template get_<number>().t.num>{};
    }
    if constexpr (e.template get_<return_val>().is_this_elem) {
      struct arg {
        constexpr arg() {}
        constexpr const AST_elem &operator()() const {
          return e.template get_<return_val>().t.v.get(allocator);
        }
      };
      using body = DECT(as_type<budget - 1, arg>(allocator));
      return as_types::return_val<body>{};
    }
    if constexpr (e.template get_<sequence>().is_this_elem) {
      struct fst {
        constexpr fst() {}
        constexpr const AST_elem &operator()() const {
          return e.template get_<sequence>().t.e.get(allocator);
        }
      };
      struct rst {
        constexpr rst() {}
        constexpr const AST_elem &operator()() const {
          return e.template get_<sequence>().t.next.get(allocator);
        }
      };
      using _fst = DECT(as_type<budget - 1, fst>(allocator));
      return as_types::sequence<_fst>::append(
          as_type<budget - 1, rst>(allocator));
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