
#pragma once
#include "allocator.hpp"
#include "mutils/type_utils.hpp"
#include "union.hpp"

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

struct transaction {
  allocated_ref<AST_elem> e{};
  std::size_t payload{0};
  constexpr transaction(){};
  constexpr transaction(transaction &&p)
      : e{std::move(p.e)}, payload{std::move(p.payload)} {}
  constexpr transaction &operator=(transaction &&p) {
    e = std::move(p.e);
    payload = std::move(p.payload);
    return *this;
  }
};
struct sequence {
  allocated_ref<AST_elem> e{};
  allocated_ref<AST_elem> next{};
  constexpr sequence(){};
  constexpr sequence(sequence &&p)
      : e{std::move(p.e)}, next{std::move(p.next)} {}
  constexpr sequence &operator=(sequence &&p) {
    e = std::move(p.e);
    next = std::move(p.next);
    return *this;
  }
};
struct plus {
  allocated_ref<AST_elem> l{};
  allocated_ref<AST_elem> r{};
  constexpr plus(){};
  constexpr plus(plus &&p) : l{std::move(p.l)}, r{std::move(p.r)} {}
  constexpr plus &operator=(plus &&p) {
    l = std::move(p.l);
    r = std::move(p.r);
    return *this;
  }
};
struct number {
  std::size_t num{0};
  constexpr number(){};
  constexpr number(number &&p) : num{std::move(p.num)} {}
  constexpr number &operator=(number &&p) {
    num = std::move(p.num);
    return *this;
  }
};
struct return_val {
  allocated_ref<AST_elem> v{};
  constexpr return_val(){};
  constexpr return_val(return_val &&p) : v{std::move(p.v)} {}
  constexpr return_val &operator=(return_val &&p) {
    v = std::move(p.v);
    return *this;
  }
};
struct skip {
  constexpr skip(){};
  constexpr skip(skip &&p) {}
  constexpr skip &operator=(skip &&p) { return *this; }
};
} // namespace as_values

namespace as_types {
template <typename> struct Expression;
template <typename> struct Statement;
template <typename, typename> struct Binding;

template <typename e, std::size_t payload> struct transaction {};
template <typename _e, std::size_t _payload>
struct Statement<transaction<_e, _payload>> {
  using e = _e;
  std::size_t payload{_payload};
};
template <typename> struct is_astnode_transaction : public std::false_type {};
template <typename _e, std::size_t _payload>
struct is_astnode_transaction<Statement<transaction<_e, _payload>>>
    : public std::true_type {};
template <typename e, typename next> struct sequence {};
template <typename _e, typename _next> struct Statement<sequence<_e, _next>> {
  using e = _e;
  using next = _next;
};
template <typename> struct is_astnode_sequence : public std::false_type {};
template <typename _e, typename _next>
struct is_astnode_sequence<Statement<sequence<_e, _next>>>
    : public std::true_type {};
template <typename l, typename r> struct plus {};
template <typename _l, typename _r> struct Expression<plus<_l, _r>> {
  using l = _l;
  using r = _r;
};
template <typename> struct is_astnode_plus : public std::false_type {};
template <typename _l, typename _r>
struct is_astnode_plus<Expression<plus<_l, _r>>> : public std::true_type {};
template <std::size_t num> struct number {};
template <std::size_t _num> struct Expression<number<_num>> {
  std::size_t num{_num};
};
template <typename> struct is_astnode_number : public std::false_type {};
template <std::size_t _num>
struct is_astnode_number<Expression<number<_num>>> : public std::true_type {};
template <typename v> struct return_val {};
template <typename _v> struct Statement<return_val<_v>> { using v = _v; };
template <typename> struct is_astnode_return_val : public std::false_type {};
template <typename _v>
struct is_astnode_return_val<Statement<return_val<_v>>>
    : public std::true_type {};
struct skip {};
template <> struct Statement<skip> {};
template <typename> struct is_astnode_skip : public std::false_type {};
template <> struct is_astnode_skip<Statement<skip>> : public std::true_type {};
} // namespace as_types

namespace as_values {
template <typename prev_holder> struct as_type_f {
  static constexpr const DECT(prev_holder::prev.allocator) &
      allocator{prev_holder::prev.allocator};
  template <long budget, typename F>
  constexpr static auto
  as_type(std::enable_if_t<(budget > 0) && (budget < 50)> * = nullptr) {
    static_assert(budget > 0);
    if constexpr (budget > 0) {
      constexpr const AST_elem &e = F{}();
      if constexpr (e.template get_<transaction>().is_this_elem) {
        struct arg0 {
#ifndef __clang__
          const AST_elem &e{F{}()};
#endif
          constexpr arg0() {}
          constexpr const AST_elem &operator()() const {
            return e.template get_<transaction>().t.e.get(allocator);
          }
        };

        using _arg0 = DECT(as_type<budget - 1, arg0>());
        constexpr auto _arg1 = e.template get_<transaction>().t.payload;
        return as_types::Statement<as_types::transaction<_arg0, _arg1>>{};
      } else if constexpr (e.template get_<sequence>().is_this_elem) {
        struct arg0 {
#ifndef __clang__
          const AST_elem &e{F{}()};
#endif
          constexpr arg0() {}
          constexpr const AST_elem &operator()() const {
            return e.template get_<sequence>().t.e.get(allocator);
          }
        };

        using _arg0 = DECT(as_type<budget - 1, arg0>());
        struct arg1 {
#ifndef __clang__
          const AST_elem &e{F{}()};
#endif
          constexpr arg1() {}
          constexpr const AST_elem &operator()() const {
            return e.template get_<sequence>().t.next.get(allocator);
          }
        };

        using _arg1 = DECT(as_type<budget - 1, arg1>());
        return as_types::Statement<as_types::sequence<_arg0, _arg1>>{};
      } else if constexpr (e.template get_<plus>().is_this_elem) {
        struct arg0 {
#ifndef __clang__
          const AST_elem &e{F{}()};
#endif
          constexpr arg0() {}
          constexpr const AST_elem &operator()() const {
            return e.template get_<plus>().t.l.get(allocator);
          }
        };

        using _arg0 = DECT(as_type<budget - 1, arg0>());
        struct arg1 {
#ifndef __clang__
          const AST_elem &e{F{}()};
#endif
          constexpr arg1() {}
          constexpr const AST_elem &operator()() const {
            return e.template get_<plus>().t.r.get(allocator);
          }
        };

        using _arg1 = DECT(as_type<budget - 1, arg1>());
        return as_types::Expression<as_types::plus<_arg0, _arg1>>{};
      } else if constexpr (e.template get_<number>().is_this_elem) {
        constexpr auto _arg0 = e.template get_<number>().t.num;
        return as_types::Expression<as_types::number<_arg0>>{};
      } else if constexpr (e.template get_<return_val>().is_this_elem) {
        struct arg0 {
#ifndef __clang__
          const AST_elem &e{F{}()};
#endif
          constexpr arg0() {}
          constexpr const AST_elem &operator()() const {
            return e.template get_<return_val>().t.v.get(allocator);
          }
        };

        using _arg0 = DECT(as_type<budget - 1, arg0>());
        return as_types::Statement<as_types::return_val<_arg0>>{};
      } else if constexpr (e.template get_<skip>().is_this_elem) {
        return as_types::Statement<as_types::skip>{};
      } else {
        struct error {};
        return error{};
      }
    } else {
      static_assert(budget > 0);
      struct error {};
      return error{};
    }
  }
};

template <typename prev_holder> constexpr auto as_type() {
  struct arg {
    constexpr arg() {}
    constexpr const AST_elem &operator()() const {
      return prev_holder::prev.allocator.top.e.get(prev_holder::prev.allocator);
    }
  };
  return as_type_f<prev_holder>::template as_type<15, arg>();
}
} // namespace as_values

namespace as_types {

template <typename AST_Allocator> struct as_values_ns_fns {
  using AST_elem = as_values::AST_elem;
  template <typename e, std::size_t payload>
  constexpr static allocated_ref<AST_elem>
  as_value(AST_Allocator &allocator,
           const Statement<transaction<e, payload>> &) {
    auto elem = allocator.template allocate<AST_elem>();
    auto &this_node =
        elem.get(allocator).template get_<as_values::transaction>();
    this_node.is_this_elem = true;
    this_node.t.e = as_value(allocator, e{});
    this_node.t.payload = payload;
    return std::move(elem);
  }
  template <typename e, typename next>
  constexpr static allocated_ref<AST_elem>
  as_value(AST_Allocator &allocator, const Statement<sequence<e, next>> &) {
    auto elem = allocator.template allocate<AST_elem>();
    auto &this_node = elem.get(allocator).template get_<as_values::sequence>();
    this_node.is_this_elem = true;
    this_node.t.e = as_value(allocator, e{});
    this_node.t.next = as_value(allocator, next{});
    return std::move(elem);
  }
  template <typename l, typename r>
  constexpr static allocated_ref<AST_elem>
  as_value(AST_Allocator &allocator, const Expression<plus<l, r>> &) {
    auto elem = allocator.template allocate<AST_elem>();
    auto &this_node = elem.get(allocator).template get_<as_values::plus>();
    this_node.is_this_elem = true;
    this_node.t.l = as_value(allocator, l{});
    this_node.t.r = as_value(allocator, r{});
    return std::move(elem);
  }
  template <std::size_t num>
  constexpr static allocated_ref<AST_elem>
  as_value(AST_Allocator &allocator, const Expression<number<num>> &) {
    auto elem = allocator.template allocate<AST_elem>();
    auto &this_node = elem.get(allocator).template get_<as_values::number>();
    this_node.is_this_elem = true;
    this_node.t.num = num;
    return std::move(elem);
  }
  template <typename v>
  constexpr static allocated_ref<AST_elem>
  as_value(AST_Allocator &allocator, const Statement<return_val<v>> &) {
    auto elem = allocator.template allocate<AST_elem>();
    auto &this_node =
        elem.get(allocator).template get_<as_values::return_val>();
    this_node.is_this_elem = true;
    this_node.t.v = as_value(allocator, v{});
    return std::move(elem);
  }
  constexpr static allocated_ref<AST_elem> as_value(AST_Allocator &allocator,
                                                    const Statement<skip> &) {
    auto elem = allocator.template allocate<AST_elem>();
    auto &this_node = elem.get(allocator).template get_<as_values::skip>();
    this_node.is_this_elem = true;
    return std::move(elem);
  }
};

template <std::size_t budget, typename hd>
constexpr as_values::AST_Allocator<budget> as_value() {
  static_assert(is_astnode_transaction<hd>::value);
  as_values::AST_Allocator<budget> head;
  head.top = std::move(
      as_values_ns_fns<as_values::AST_Allocator<budget>>::as_value(head, hd{})
          .get(head)
          .template get<as_values::transaction>());
  return head;
}
} // namespace as_types