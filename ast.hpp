#pragma once
#include "allocator.hpp"
#include "mutils/type_utils.hpp"
#include "union.hpp"
#include <ostream>

template <typename T> using plain_array = T[20];

namespace as_values {
struct transaction;
struct FieldReference;
struct FieldPointerReference;
struct Dereference;
struct Endorse;
struct Ensure;
struct IsValid;
struct VarReference;
struct Constant;
struct BinOp;
struct Let;
struct LetRemote;
struct operation_args_exprs;
struct operation_args_varrefs;
struct Operation;
struct Assignment;
struct Return;
struct If;
struct While;
struct Sequence;
struct Skip;
using AST_elem =
    Union<transaction, FieldReference, FieldPointerReference, Dereference,
          Endorse, Ensure, IsValid, VarReference, Constant, BinOp, Let,
          LetRemote, operation_args_exprs, operation_args_varrefs, Operation,
          Assignment, Return, If, While, Sequence, Skip>;
template <std::size_t budget>
using AST_Allocator = Allocator<budget, transaction, AST_elem>;

// Define structs.
struct Expression {
  constexpr Expression() {}
};
struct Statement {
  constexpr Statement() {}
};
struct Binding {
  constexpr Binding() {}
};
struct transaction : public Statement {
  allocated_ref<AST_elem> e{};
  std::size_t payload{0};
  // default constructor
  constexpr transaction(){};
  constexpr transaction(transaction &&p)
      : e{std::move(p.e)}, payload{std::move(p.payload)} {}
  // move-assignment
  constexpr transaction &operator=(transaction &&p) {
    e = std::move(p.e);
    payload = std::move(p.payload);
    return *this;
  }
};
struct FieldReference : public Expression {
  allocated_ref<AST_elem> Struct{};
  allocated_ref<AST_elem> Field{};
  // default constructor
  constexpr FieldReference(){};
  constexpr FieldReference(FieldReference &&p)
      : Struct{std::move(p.Struct)}, Field{std::move(p.Field)} {}
  // move-assignment
  constexpr FieldReference &operator=(FieldReference &&p) {
    Struct = std::move(p.Struct);
    Field = std::move(p.Field);
    return *this;
  }
};
struct FieldPointerReference : public Expression {
  allocated_ref<AST_elem> Struct{};
  allocated_ref<AST_elem> Field{};
  // default constructor
  constexpr FieldPointerReference(){};
  constexpr FieldPointerReference(FieldPointerReference &&p)
      : Struct{std::move(p.Struct)}, Field{std::move(p.Field)} {}
  // move-assignment
  constexpr FieldPointerReference &operator=(FieldPointerReference &&p) {
    Struct = std::move(p.Struct);
    Field = std::move(p.Field);
    return *this;
  }
};
struct Dereference : public Expression {
  allocated_ref<AST_elem> Struct{};
  // default constructor
  constexpr Dereference(){};
  constexpr Dereference(Dereference &&p) : Struct{std::move(p.Struct)} {}
  // move-assignment
  constexpr Dereference &operator=(Dereference &&p) {
    Struct = std::move(p.Struct);
    return *this;
  }
};
struct Endorse : public Expression {
  allocated_ref<AST_elem> label{};
  allocated_ref<AST_elem> Hndl{};
  // default constructor
  constexpr Endorse(){};
  constexpr Endorse(Endorse &&p)
      : label{std::move(p.label)}, Hndl{std::move(p.Hndl)} {}
  // move-assignment
  constexpr Endorse &operator=(Endorse &&p) {
    label = std::move(p.label);
    Hndl = std::move(p.Hndl);
    return *this;
  }
};
struct Ensure : public Expression {
  allocated_ref<AST_elem> label{};
  allocated_ref<AST_elem> Hndl{};
  // default constructor
  constexpr Ensure(){};
  constexpr Ensure(Ensure &&p)
      : label{std::move(p.label)}, Hndl{std::move(p.Hndl)} {}
  // move-assignment
  constexpr Ensure &operator=(Ensure &&p) {
    label = std::move(p.label);
    Hndl = std::move(p.Hndl);
    return *this;
  }
};
struct IsValid : public Expression {
  allocated_ref<AST_elem> Hndl{};
  // default constructor
  constexpr IsValid(){};
  constexpr IsValid(IsValid &&p) : Hndl{std::move(p.Hndl)} {}
  // move-assignment
  constexpr IsValid &operator=(IsValid &&p) {
    Hndl = std::move(p.Hndl);
    return *this;
  }
};
struct VarReference : public Expression {
  allocated_ref<AST_elem> Var{};
  // default constructor
  constexpr VarReference(){};
  constexpr VarReference(VarReference &&p) : Var{std::move(p.Var)} {}
  // move-assignment
  constexpr VarReference &operator=(VarReference &&p) {
    Var = std::move(p.Var);
    return *this;
  }
};
struct Constant : public Expression {
  std::size_t i{0};
  // default constructor
  constexpr Constant(){};
  constexpr Constant(Constant &&p) : i{std::move(p.i)} {}
  // move-assignment
  constexpr Constant &operator=(Constant &&p) {
    i = std::move(p.i);
    return *this;
  }
};
struct BinOp : public Expression {
  char op{};
  allocated_ref<AST_elem> L{};
  allocated_ref<AST_elem> R{};
  // default constructor
  constexpr BinOp(){};
  constexpr BinOp(BinOp &&p)
      : op{std::move(p.op)}, L{std::move(p.L)}, R{std::move(p.R)} {}
  // move-assignment
  constexpr BinOp &operator=(BinOp &&p) {
    op = std::move(p.op);
    L = std::move(p.L);
    R = std::move(p.R);
    return *this;
  }
};
struct Let : public Statement {
  allocated_ref<AST_elem> Binding{};
  allocated_ref<AST_elem> Body{};
  // default constructor
  constexpr Let(){};
  constexpr Let(Let &&p)
      : Binding{std::move(p.Binding)}, Body{std::move(p.Body)} {}
  // move-assignment
  constexpr Let &operator=(Let &&p) {
    Binding = std::move(p.Binding);
    Body = std::move(p.Body);
    return *this;
  }
};
struct LetRemote : public Statement {
  allocated_ref<AST_elem> Binding{};
  allocated_ref<AST_elem> Body{};
  // default constructor
  constexpr LetRemote(){};
  constexpr LetRemote(LetRemote &&p)
      : Binding{std::move(p.Binding)}, Body{std::move(p.Body)} {}
  // move-assignment
  constexpr LetRemote &operator=(LetRemote &&p) {
    Binding = std::move(p.Binding);
    Body = std::move(p.Body);
    return *this;
  }
};
struct operation_args_exprs {
  plain_array<allocated_ref<AST_elem>> exprs;
  // default constructor
  constexpr operation_args_exprs(){};
  // move constructor
  constexpr operation_args_exprs(operation_args_exprs &&p)
      : exprs{{allocated_ref<AST_elem>{}}} {
    for (auto i = 0u; i < 20; ++i) {
      exprs[i] = std::move(p.exprs[i]);
    }
  }

  // move-assignment
  constexpr operation_args_exprs &operator=(operation_args_exprs &&p) {
    for (auto i = 0u; i < 20; ++i) {
      exprs[i] = std::move(p.exprs[i]);
    }
    return *this;
  }
};
struct operation_args_varrefs {
  plain_array<allocated_ref<AST_elem>> vars;
  // default constructor
  constexpr operation_args_varrefs(){};
  // move constructor
  constexpr operation_args_varrefs(operation_args_varrefs &&p)
      : vars{{allocated_ref<AST_elem>{}}} {
    for (auto i = 0u; i < 20; ++i) {
      vars[i] = std::move(p.vars[i]);
    }
  }

  // move-assignment
  constexpr operation_args_varrefs &operator=(operation_args_varrefs &&p) {
    for (auto i = 0u; i < 20; ++i) {
      vars[i] = std::move(p.vars[i]);
    }
    return *this;
  }
};
struct Operation : public Statement, public Expression {
  allocated_ref<AST_elem> name{};
  allocated_ref<AST_elem> Hndl{};
  allocated_ref<AST_elem> expr_args{};
  allocated_ref<AST_elem> var_args{};
  // default constructor
  constexpr Operation(){};
  constexpr Operation(Operation &&p)
      : name{std::move(p.name)}, Hndl{std::move(p.Hndl)},
        expr_args{std::move(p.expr_args)}, var_args{std::move(p.var_args)} {}
  // move-assignment
  constexpr Operation &operator=(Operation &&p) {
    name = std::move(p.name);
    Hndl = std::move(p.Hndl);
    expr_args = std::move(p.expr_args);
    var_args = std::move(p.var_args);
    return *this;
  }
};
struct Assignment : public Statement {
  allocated_ref<AST_elem> Var{};
  allocated_ref<AST_elem> Expr{};
  // default constructor
  constexpr Assignment(){};
  constexpr Assignment(Assignment &&p)
      : Var{std::move(p.Var)}, Expr{std::move(p.Expr)} {}
  // move-assignment
  constexpr Assignment &operator=(Assignment &&p) {
    Var = std::move(p.Var);
    Expr = std::move(p.Expr);
    return *this;
  }
};
struct Return : public Statement {
  allocated_ref<AST_elem> Expr{};
  // default constructor
  constexpr Return(){};
  constexpr Return(Return &&p) : Expr{std::move(p.Expr)} {}
  // move-assignment
  constexpr Return &operator=(Return &&p) {
    Expr = std::move(p.Expr);
    return *this;
  }
};
struct If : public Statement {
  allocated_ref<AST_elem> condition{};
  allocated_ref<AST_elem> then{};
  allocated_ref<AST_elem> els{};
  // default constructor
  constexpr If(){};
  constexpr If(If &&p)
      : condition{std::move(p.condition)}, then{std::move(p.then)},
        els{std::move(p.els)} {}
  // move-assignment
  constexpr If &operator=(If &&p) {
    condition = std::move(p.condition);
    then = std::move(p.then);
    els = std::move(p.els);
    return *this;
  }
};
struct While : public Statement {
  allocated_ref<AST_elem> condition{};
  allocated_ref<AST_elem> body{};
  // default constructor
  constexpr While(){};
  constexpr While(While &&p)
      : condition{std::move(p.condition)}, body{std::move(p.body)} {}
  // move-assignment
  constexpr While &operator=(While &&p) {
    condition = std::move(p.condition);
    body = std::move(p.body);
    return *this;
  }
};
struct Sequence : public Statement {
  allocated_ref<AST_elem> e{};
  allocated_ref<AST_elem> next{};
  // default constructor
  constexpr Sequence(){};
  constexpr Sequence(Sequence &&p)
      : e{std::move(p.e)}, next{std::move(p.next)} {}
  // move-assignment
  constexpr Sequence &operator=(Sequence &&p) {
    e = std::move(p.e);
    next = std::move(p.next);
    return *this;
  }
};
struct Skip : public Statement {
  // default constructor
  constexpr Skip(){};
  constexpr Skip(Skip &&p) {}
  // move-assignment
  constexpr Skip &operator=(Skip &&p) { return *this; }
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
template <typename Struct, typename Field> struct FieldReference {};
template <typename _Struct, typename _Field>
struct Expression<FieldReference<_Struct, _Field>> {
  using Struct = _Struct;
  using Field = _Field;
};
template <typename Struct, typename Field> struct FieldPointerReference {};
template <typename _Struct, typename _Field>
struct Expression<FieldPointerReference<_Struct, _Field>> {
  using Struct = _Struct;
  using Field = _Field;
};
template <typename Struct> struct Dereference {};
template <typename _Struct> struct Expression<Dereference<_Struct>> {
  using Struct = _Struct;
};
template <typename label, typename Hndl> struct Endorse {};
template <typename _label, typename _Hndl>
struct Expression<Endorse<_label, _Hndl>> {
  using label = _label;
  using Hndl = _Hndl;
};
template <typename label, typename Hndl> struct Ensure {};
template <typename _label, typename _Hndl>
struct Expression<Ensure<_label, _Hndl>> {
  using label = _label;
  using Hndl = _Hndl;
};
template <typename Hndl> struct IsValid {};
template <typename _Hndl> struct Expression<IsValid<_Hndl>> {
  using Hndl = _Hndl;
};
template <typename Var> struct VarReference {};
template <typename _Var> struct Expression<VarReference<_Var>> {
  using Var = _Var;
};
template <std::size_t i> struct Constant {};
template <std::size_t _i> struct Expression<Constant<_i>> {
  std::size_t i{_i};
};
template <char op, typename L, typename R> struct BinOp {};
template <char _op, typename _L, typename _R>
struct Expression<BinOp<_op, _L, _R>> {
  char op{_op};
  using L = _L;
  using R = _R;
};
template <typename Binding, typename Body> struct Let {};
template <typename _Binding, typename _Body>
struct Statement<Let<_Binding, _Body>> {
  using Binding = _Binding;
  using Body = _Body;
};
template <typename Binding, typename Body> struct LetRemote {};
template <typename _Binding, typename _Body>
struct Statement<LetRemote<_Binding, _Body>> {
  using Binding = _Binding;
  using Body = _Body;
};
template <typename...> struct operation_args_exprs;
template <typename...> struct operation_args_varrefs;
template <typename name, typename Hndl, typename expr_args, typename var_args>
struct Operation {};
template <typename _name, typename _Hndl, typename _expr_args,
          typename _var_args>
struct Statement<Operation<_name, _Hndl, _expr_args, _var_args>> {
  using name = _name;
  using Hndl = _Hndl;
  using expr_args = _expr_args;
  using var_args = _var_args;
};
template <typename _name, typename _Hndl, typename _expr_args,
          typename _var_args>
struct Expression<Operation<_name, _Hndl, _expr_args, _var_args>> {
  using name = _name;
  using Hndl = _Hndl;
  using expr_args = _expr_args;
  using var_args = _var_args;
};
template <typename Var, typename Expr> struct Assignment {};
template <typename _Var, typename _Expr>
struct Statement<Assignment<_Var, _Expr>> {
  using Var = _Var;
  using Expr = _Expr;
};
template <typename Expr> struct Return {};
template <typename _Expr> struct Statement<Return<_Expr>> {
  using Expr = _Expr;
};
template <typename condition, typename then, typename els> struct If {};
template <typename _condition, typename _then, typename _els>
struct Statement<If<_condition, _then, _els>> {
  using condition = _condition;
  using then = _then;
  using els = _els;
};
template <typename condition, typename body> struct While {};
template <typename _condition, typename _body>
struct Statement<While<_condition, _body>> {
  using condition = _condition;
  using body = _body;
};
template <typename e, typename next> struct Sequence {};
template <typename _e, typename _next> struct Statement<Sequence<_e, _next>> {
  using e = _e;
  using next = _next;
};
struct Skip {};
template <> struct Statement<Skip> {};

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
        using _arg0 = DECT(as_type<budget - 1, arg0>());
        struct arg0 {
#ifndef __clang__
          const AST_elem &e{F{}()};
#endif
          constexpr arg0() {}
          constexpr const AST_elem &operator()() const {
            return e.template get_<transaction>().t.e.get(allocator);
          }
        };

        constexpr auto _arg1 = e.template get_<transaction>().t.payload;
        return as_types::Statement<as_types::transaction<_arg0, _arg1>>{};
      } else if constexpr (e.template get_<FieldReference>().is_this_elem) {
        using _arg0 = DECT(as_type<budget - 1, arg0>());
        struct arg0 {
#ifndef __clang__
          const AST_elem &e{F{}()};
#endif
          constexpr arg0() {}
          constexpr const AST_elem &operator()() const {
            return e.template get_<FieldReference>().t.Struct.get(allocator);
          }
        };

        using _arg1 = DECT(as_type<budget - 1, arg1>());
        struct arg1 {
#ifndef __clang__
          const AST_elem &e{F{}()};
#endif
          constexpr arg1() {}
          constexpr const AST_elem &operator()() const {
            return e.template get_<FieldReference>().t.Field.get(allocator);
          }
        };

        return as_types::Expression<as_types::FieldReference<_arg0, _arg1>>{};
      } else if constexpr (e.template get_<FieldPointerReference>()
                               .is_this_elem) {
        using _arg0 = DECT(as_type<budget - 1, arg0>());
        struct arg0 {
#ifndef __clang__
          const AST_elem &e{F{}()};
#endif
          constexpr arg0() {}
          constexpr const AST_elem &operator()() const {
            return e.template get_<FieldPointerReference>().t.Struct.get(
                allocator);
          }
        };

        using _arg1 = DECT(as_type<budget - 1, arg1>());
        struct arg1 {
#ifndef __clang__
          const AST_elem &e{F{}()};
#endif
          constexpr arg1() {}
          constexpr const AST_elem &operator()() const {
            return e.template get_<FieldPointerReference>().t.Field.get(
                allocator);
          }
        };

        return as_types::Expression<
            as_types::FieldPointerReference<_arg0, _arg1>>{};
      } else if constexpr (e.template get_<Dereference>().is_this_elem) {
        using _arg0 = DECT(as_type<budget - 1, arg0>());
        struct arg0 {
#ifndef __clang__
          const AST_elem &e{F{}()};
#endif
          constexpr arg0() {}
          constexpr const AST_elem &operator()() const {
            return e.template get_<Dereference>().t.Struct.get(allocator);
          }
        };

        return as_types::Expression<as_types::Dereference<_arg0>>{};
      } else if constexpr (e.template get_<Endorse>().is_this_elem) {
        using _arg0 = DECT(as_type<budget - 1, arg0>());
        struct arg0 {
#ifndef __clang__
          const AST_elem &e{F{}()};
#endif
          constexpr arg0() {}
          constexpr const AST_elem &operator()() const {
            return e.template get_<Endorse>().t.label.get(allocator);
          }
        };

        using _arg1 = DECT(as_type<budget - 1, arg1>());
        struct arg1 {
#ifndef __clang__
          const AST_elem &e{F{}()};
#endif
          constexpr arg1() {}
          constexpr const AST_elem &operator()() const {
            return e.template get_<Endorse>().t.Hndl.get(allocator);
          }
        };

        return as_types::Expression<as_types::Endorse<_arg0, _arg1>>{};
      } else if constexpr (e.template get_<Ensure>().is_this_elem) {
        using _arg0 = DECT(as_type<budget - 1, arg0>());
        struct arg0 {
#ifndef __clang__
          const AST_elem &e{F{}()};
#endif
          constexpr arg0() {}
          constexpr const AST_elem &operator()() const {
            return e.template get_<Ensure>().t.label.get(allocator);
          }
        };

        using _arg1 = DECT(as_type<budget - 1, arg1>());
        struct arg1 {
#ifndef __clang__
          const AST_elem &e{F{}()};
#endif
          constexpr arg1() {}
          constexpr const AST_elem &operator()() const {
            return e.template get_<Ensure>().t.Hndl.get(allocator);
          }
        };

        return as_types::Expression<as_types::Ensure<_arg0, _arg1>>{};
      } else if constexpr (e.template get_<IsValid>().is_this_elem) {
        using _arg0 = DECT(as_type<budget - 1, arg0>());
        struct arg0 {
#ifndef __clang__
          const AST_elem &e{F{}()};
#endif
          constexpr arg0() {}
          constexpr const AST_elem &operator()() const {
            return e.template get_<IsValid>().t.Hndl.get(allocator);
          }
        };

        return as_types::Expression<as_types::IsValid<_arg0>>{};
      } else if constexpr (e.template get_<VarReference>().is_this_elem) {
        using _arg0 = DECT(as_type<budget - 1, arg0>());
        struct arg0 {
#ifndef __clang__
          const AST_elem &e{F{}()};
#endif
          constexpr arg0() {}
          constexpr const AST_elem &operator()() const {
            return e.template get_<VarReference>().t.Var.get(allocator);
          }
        };

        return as_types::Expression<as_types::VarReference<_arg0>>{};
      } else if constexpr (e.template get_<Constant>().is_this_elem) {
        constexpr auto _arg0 = e.template get_<Constant>().t.i;
        return as_types::Expression<as_types::Constant<_arg0>>{};
      } else if constexpr (e.template get_<BinOp>().is_this_elem) {
        constexpr auto _arg0 = e.template get_<BinOp>().t.op;
        using _arg1 = DECT(as_type<budget - 1, arg1>());
        struct arg1 {
#ifndef __clang__
          const AST_elem &e{F{}()};
#endif
          constexpr arg1() {}
          constexpr const AST_elem &operator()() const {
            return e.template get_<BinOp>().t.L.get(allocator);
          }
        };

        using _arg2 = DECT(as_type<budget - 1, arg2>());
        struct arg2 {
#ifndef __clang__
          const AST_elem &e{F{}()};
#endif
          constexpr arg2() {}
          constexpr const AST_elem &operator()() const {
            return e.template get_<BinOp>().t.R.get(allocator);
          }
        };

        return as_types::Expression<as_types::BinOp<_arg0, _arg1, _arg2>>{};
      } else if constexpr (e.template get_<Let>().is_this_elem) {
        using _arg0 = DECT(as_type<budget - 1, arg0>());
        struct arg0 {
#ifndef __clang__
          const AST_elem &e{F{}()};
#endif
          constexpr arg0() {}
          constexpr const AST_elem &operator()() const {
            return e.template get_<Let>().t.Binding.get(allocator);
          }
        };

        using _arg1 = DECT(as_type<budget - 1, arg1>());
        struct arg1 {
#ifndef __clang__
          const AST_elem &e{F{}()};
#endif
          constexpr arg1() {}
          constexpr const AST_elem &operator()() const {
            return e.template get_<Let>().t.Body.get(allocator);
          }
        };

        return as_types::Statement<as_types::Let<_arg0, _arg1>>{};
      } else if constexpr (e.template get_<LetRemote>().is_this_elem) {
        using _arg0 = DECT(as_type<budget - 1, arg0>());
        struct arg0 {
#ifndef __clang__
          const AST_elem &e{F{}()};
#endif
          constexpr arg0() {}
          constexpr const AST_elem &operator()() const {
            return e.template get_<LetRemote>().t.Binding.get(allocator);
          }
        };

        using _arg1 = DECT(as_type<budget - 1, arg1>());
        struct arg1 {
#ifndef __clang__
          const AST_elem &e{F{}()};
#endif
          constexpr arg1() {}
          constexpr const AST_elem &operator()() const {
            return e.template get_<LetRemote>().t.Body.get(allocator);
          }
        };

        return as_types::Statement<as_types::LetRemote<_arg0, _arg1>>{};
      } else if constexpr (e.template get_<operation_args_exprs>()
                               .is_this_elem) {
        if (there_is_anything_here(
                e.template get_<operation_args_exprs>().t.exprs[0])) {
          using _arg0 = DECT(as_type<budget - 1, arg0>());
          struct arg0 {
#ifndef __clang__
            const AST_elem &e{F{}()};
#endif
            constexpr arg0() {}
            constexpr const AST_elem &operator()() const {
              return e.template get_<operation_args_exprs>().t.exprs[0].get(
                  allocator);
            }
          };

          if (there_is_anything_here(
                  e.template get_<operation_args_exprs>().t.exprs[1])) {
            using _arg1 = DECT(as_type<budget - 1, arg1>());
            struct arg1 {
#ifndef __clang__
              const AST_elem &e{F{}()};
#endif
              constexpr arg1() {}
              constexpr const AST_elem &operator()() const {
                return e.template get_<operation_args_exprs>().t.exprs[1].get(
                    allocator);
              }
            };

            if (there_is_anything_here(
                    e.template get_<operation_args_exprs>().t.exprs[2])) {
              using _arg2 = DECT(as_type<budget - 1, arg2>());
              struct arg2 {
#ifndef __clang__
                const AST_elem &e{F{}()};
#endif
                constexpr arg2() {}
                constexpr const AST_elem &operator()() const {
                  return e.template get_<operation_args_exprs>().t.exprs[2].get(
                      allocator);
                }
              };

              if (there_is_anything_here(
                      e.template get_<operation_args_exprs>().t.exprs[3])) {
                using _arg3 = DECT(as_type<budget - 1, arg3>());
                struct arg3 {
#ifndef __clang__
                  const AST_elem &e{F{}()};
#endif
                  constexpr arg3() {}
                  constexpr const AST_elem &operator()() const {
                    return e.template get_<operation_args_exprs>()
                        .t.exprs[3]
                        .get(allocator);
                  }
                };

                if (there_is_anything_here(
                        e.template get_<operation_args_exprs>().t.exprs[4])) {
                  using _arg4 = DECT(as_type<budget - 1, arg4>());
                  struct arg4 {
#ifndef __clang__
                    const AST_elem &e{F{}()};
#endif
                    constexpr arg4() {}
                    constexpr const AST_elem &operator()() const {
                      return e.template get_<operation_args_exprs>()
                          .t.exprs[4]
                          .get(allocator);
                    }
                  };

                  if (there_is_anything_here(
                          e.template get_<operation_args_exprs>().t.exprs[5])) {
                    using _arg5 = DECT(as_type<budget - 1, arg5>());
                    struct arg5 {
#ifndef __clang__
                      const AST_elem &e{F{}()};
#endif
                      constexpr arg5() {}
                      constexpr const AST_elem &operator()() const {
                        return e.template get_<operation_args_exprs>()
                            .t.exprs[5]
                            .get(allocator);
                      }
                    };

                    if (there_is_anything_here(
                            e.template get_<operation_args_exprs>()
                                .t.exprs[6])) {
                      using _arg6 = DECT(as_type<budget - 1, arg6>());
                      struct arg6 {
#ifndef __clang__
                        const AST_elem &e{F{}()};
#endif
                        constexpr arg6() {}
                        constexpr const AST_elem &operator()() const {
                          return e.template get_<operation_args_exprs>()
                              .t.exprs[6]
                              .get(allocator);
                        }
                      };

                      if (there_is_anything_here(
                              e.template get_<operation_args_exprs>()
                                  .t.exprs[7])) {
                        using _arg7 = DECT(as_type<budget - 1, arg7>());
                        struct arg7 {
#ifndef __clang__
                          const AST_elem &e{F{}()};
#endif
                          constexpr arg7() {}
                          constexpr const AST_elem &operator()() const {
                            return e.template get_<operation_args_exprs>()
                                .t.exprs[7]
                                .get(allocator);
                          }
                        };

                        if (there_is_anything_here(
                                e.template get_<operation_args_exprs>()
                                    .t.exprs[8])) {
                          using _arg8 = DECT(as_type<budget - 1, arg8>());
                          struct arg8 {
#ifndef __clang__
                            const AST_elem &e{F{}()};
#endif
                            constexpr arg8() {}
                            constexpr const AST_elem &operator()() const {
                              return e.template get_<operation_args_exprs>()
                                  .t.exprs[8]
                                  .get(allocator);
                            }
                          };

                          if (there_is_anything_here(
                                  e.template get_<operation_args_exprs>()
                                      .t.exprs[9])) {
                            using _arg9 = DECT(as_type<budget - 1, arg9>());
                            struct arg9 {
#ifndef __clang__
                              const AST_elem &e{F{}()};
#endif
                              constexpr arg9() {}
                              constexpr const AST_elem &operator()() const {
                                return e.template get_<operation_args_exprs>()
                                    .t.exprs[9]
                                    .get(allocator);
                              }
                            };

                            if (there_is_anything_here(
                                    e.template get_<operation_args_exprs>()
                                        .t.exprs[10])) {
                              using _arg10 = DECT(as_type<budget - 1, arg10>());
                              struct arg10 {
#ifndef __clang__
                                const AST_elem &e{F{}()};
#endif
                                constexpr arg10() {}
                                constexpr const AST_elem &operator()() const {
                                  return e.template get_<operation_args_exprs>()
                                      .t.exprs[10]
                                      .get(allocator);
                                }
                              };

                              if (there_is_anything_here(
                                      e.template get_<operation_args_exprs>()
                                          .t.exprs[11])) {
                                using _arg11 =
                                    DECT(as_type<budget - 1, arg11>());
                                struct arg11 {
#ifndef __clang__
                                  const AST_elem &e{F{}()};
#endif
                                  constexpr arg11() {}
                                  constexpr const AST_elem &operator()() const {
                                    return e
                                        .template get_<operation_args_exprs>()
                                        .t.exprs[11]
                                        .get(allocator);
                                  }
                                };

                                if (there_is_anything_here(
                                        e.template get_<operation_args_exprs>()
                                            .t.exprs[12])) {
                                  using _arg12 =
                                      DECT(as_type<budget - 1, arg12>());
                                  struct arg12 {
#ifndef __clang__
                                    const AST_elem &e{F{}()};
#endif
                                    constexpr arg12() {}
                                    constexpr const AST_elem &
                                    operator()() const {
                                      return e
                                          .template get_<operation_args_exprs>()
                                          .t.exprs[12]
                                          .get(allocator);
                                    }
                                  };

                                  if (there_is_anything_here(
                                          e.template get_<
                                               operation_args_exprs>()
                                              .t.exprs[13])) {
                                    using _arg13 =
                                        DECT(as_type<budget - 1, arg13>());
                                    struct arg13 {
#ifndef __clang__
                                      const AST_elem &e{F{}()};
#endif
                                      constexpr arg13() {}
                                      constexpr const AST_elem &
                                      operator()() const {
                                        return e
                                            .template get_<
                                                operation_args_exprs>()
                                            .t.exprs[13]
                                            .get(allocator);
                                      }
                                    };

                                    if (there_is_anything_here(
                                            e.template get_<
                                                 operation_args_exprs>()
                                                .t.exprs[14])) {
                                      using _arg14 =
                                          DECT(as_type<budget - 1, arg14>());
                                      struct arg14 {
#ifndef __clang__
                                        const AST_elem &e{F{}()};
#endif
                                        constexpr arg14() {}
                                        constexpr const AST_elem &
                                        operator()() const {
                                          return e
                                              .template get_<
                                                  operation_args_exprs>()
                                              .t.exprs[14]
                                              .get(allocator);
                                        }
                                      };

                                      if (there_is_anything_here(
                                              e.template get_<
                                                   operation_args_exprs>()
                                                  .t.exprs[15])) {
                                        using _arg15 =
                                            DECT(as_type<budget - 1, arg15>());
                                        struct arg15 {
#ifndef __clang__
                                          const AST_elem &e{F{}()};
#endif
                                          constexpr arg15() {}
                                          constexpr const AST_elem &
                                          operator()() const {
                                            return e
                                                .template get_<
                                                    operation_args_exprs>()
                                                .t.exprs[15]
                                                .get(allocator);
                                          }
                                        };

                                        if (there_is_anything_here(
                                                e.template get_<
                                                     operation_args_exprs>()
                                                    .t.exprs[16])) {
                                          using _arg16 = DECT(
                                              as_type<budget - 1, arg16>());
                                          struct arg16 {
#ifndef __clang__
                                            const AST_elem &e{F{}()};
#endif
                                            constexpr arg16() {}
                                            constexpr const AST_elem &
                                            operator()() const {
                                              return e
                                                  .template get_<
                                                      operation_args_exprs>()
                                                  .t.exprs[16]
                                                  .get(allocator);
                                            }
                                          };

                                          if (there_is_anything_here(
                                                  e.template get_<
                                                       operation_args_exprs>()
                                                      .t.exprs[17])) {
                                            using _arg17 = DECT(
                                                as_type<budget - 1, arg17>());
                                            struct arg17 {
#ifndef __clang__
                                              const AST_elem &e{F{}()};
#endif
                                              constexpr arg17() {}
                                              constexpr const AST_elem &
                                              operator()() const {
                                                return e
                                                    .template get_<
                                                        operation_args_exprs>()
                                                    .t.exprs[17]
                                                    .get(allocator);
                                              }
                                            };

                                            if (there_is_anything_here(
                                                    e.template get_<
                                                         operation_args_exprs>()
                                                        .t.exprs[18])) {
                                              using _arg18 = DECT(
                                                  as_type<budget - 1, arg18>());
                                              struct arg18 {
#ifndef __clang__
                                                const AST_elem &e{F{}()};
#endif
                                                constexpr arg18() {}
                                                constexpr const AST_elem &
                                                operator()() const {
                                                  return e
                                                      .template get_<
                                                          operation_args_exprs>()
                                                      .t.exprs[18]
                                                      .get(allocator);
                                                }
                                              };

                                              if (there_is_anything_here(
                                                      e.template get_<
                                                           operation_args_exprs>()
                                                          .t.exprs[19])) {
                                                using _arg19 =
                                                    DECT(as_type<budget - 1,
                                                                 arg19>());
                                                struct arg19 {
#ifndef __clang__
                                                  const AST_elem &e{F{}()};
#endif
                                                  constexpr arg19() {}
                                                  constexpr const AST_elem &
                                                  operator()() const {
                                                    return e
                                                        .template get_<
                                                            operation_args_exprs>()
                                                        .t.exprs[19]
                                                        .get(allocator);
                                                  }
                                                };

                                                return as_types::<
                                                    as_types::
                                                        operation_args_exprs<
                                                            _arg0, _arg1, _arg2,
                                                            _arg3, _arg4, _arg5,
                                                            _arg6, _arg7, _arg8,
                                                            _arg9, _arg10,
                                                            _arg11, _arg12,
                                                            _arg13, _arg14,
                                                            _arg15, _arg16,
                                                            _arg17, _arg18,
                                                            _arg19>>{};
                                              } else {
                                                return as_types::<
                                                    as_types::
                                                        operation_args_exprs<
                                                            _arg0, _arg1, _arg2,
                                                            _arg3, _arg4, _arg5,
                                                            _arg6, _arg7, _arg8,
                                                            _arg9, _arg10,
                                                            _arg11, _arg12,
                                                            _arg13, _arg14,
                                                            _arg15, _arg16,
                                                            _arg17, _arg18>>{};
                                              }
                                            } else {
                                              return as_types::<
                                                  as_types::
                                                      operation_args_exprs<
                                                          _arg0, _arg1, _arg2,
                                                          _arg3, _arg4, _arg5,
                                                          _arg6, _arg7, _arg8,
                                                          _arg9, _arg10, _arg11,
                                                          _arg12, _arg13,
                                                          _arg14, _arg15,
                                                          _arg16, _arg17>>{};
                                            }
                                          } else {
                                            return as_types::<
                                                as_types::operation_args_exprs<
                                                    _arg0, _arg1, _arg2, _arg3,
                                                    _arg4, _arg5, _arg6, _arg7,
                                                    _arg8, _arg9, _arg10,
                                                    _arg11, _arg12, _arg13,
                                                    _arg14, _arg15, _arg16>>{};
                                          }
                                        } else {
                                          return as_types::<
                                              as_types::operation_args_exprs<
                                                  _arg0, _arg1, _arg2, _arg3,
                                                  _arg4, _arg5, _arg6, _arg7,
                                                  _arg8, _arg9, _arg10, _arg11,
                                                  _arg12, _arg13, _arg14,
                                                  _arg15>>{};
                                        }
                                      } else {
                                        return as_types::<
                                            as_types::operation_args_exprs<
                                                _arg0, _arg1, _arg2, _arg3,
                                                _arg4, _arg5, _arg6, _arg7,
                                                _arg8, _arg9, _arg10, _arg11,
                                                _arg12, _arg13, _arg14>>{};
                                      }
                                    } else {
                                      return as_types::<
                                          as_types::operation_args_exprs<
                                              _arg0, _arg1, _arg2, _arg3, _arg4,
                                              _arg5, _arg6, _arg7, _arg8, _arg9,
                                              _arg10, _arg11, _arg12,
                                              _arg13>>{};
                                    }
                                  } else {
                                    return as_types::<
                                        as_types::operation_args_exprs<
                                            _arg0, _arg1, _arg2, _arg3, _arg4,
                                            _arg5, _arg6, _arg7, _arg8, _arg9,
                                            _arg10, _arg11, _arg12>>{};
                                  }
                                } else {
                                  return as_types::<
                                      as_types::operation_args_exprs<
                                          _arg0, _arg1, _arg2, _arg3, _arg4,
                                          _arg5, _arg6, _arg7, _arg8, _arg9,
                                          _arg10, _arg11>>{};
                                }
                              } else {
                                return as_types::<
                                    as_types::operation_args_exprs<
                                        _arg0, _arg1, _arg2, _arg3, _arg4,
                                        _arg5, _arg6, _arg7, _arg8, _arg9,
                                        _arg10>>{};
                              }
                            } else {
                              return as_types::<as_types::operation_args_exprs<
                                  _arg0, _arg1, _arg2, _arg3, _arg4, _arg5,
                                  _arg6, _arg7, _arg8, _arg9>>{};
                            }
                          } else {
                            return as_types::<as_types::operation_args_exprs<
                                _arg0, _arg1, _arg2, _arg3, _arg4, _arg5, _arg6,
                                _arg7, _arg8>>{};
                          }
                        } else {
                          return as_types::<as_types::operation_args_exprs<
                              _arg0, _arg1, _arg2, _arg3, _arg4, _arg5, _arg6,
                              _arg7>>{};
                        }
                      } else {
                        return as_types::<as_types::operation_args_exprs<
                            _arg0, _arg1, _arg2, _arg3, _arg4, _arg5, _arg6>>{};
                      }
                    } else {
                      return as_types::<as_types::operation_args_exprs<
                          _arg0, _arg1, _arg2, _arg3, _arg4, _arg5>>{};
                    }
                  } else {
                    return as_types::<as_types::operation_args_exprs<
                        _arg0, _arg1, _arg2, _arg3, _arg4>>{};
                  }
                } else {
                  return as_types::<as_types::operation_args_exprs<
                      _arg0, _arg1, _arg2, _arg3>>{};
                }
              } else {
                return as_types::<
                    as_types::operation_args_exprs<_arg0, _arg1, _arg2>>{};
              }
            } else {
              return as_types::<as_types::operation_args_exprs<_arg0, _arg1>>{};
            }
          } else {
            return as_types::<as_types::operation_args_exprs<_arg0>>{};
          }
        }
        return as_types::<as_types::operation_args_exprs<>>{};
      } else if constexpr (e.template get_<operation_args_varrefs>()
                               .is_this_elem) {
        if (there_is_anything_here(
                e.template get_<operation_args_varrefs>().t.vars[0])) {
          using _arg0 = DECT(as_type<budget - 1, arg0>());
          struct arg0 {
#ifndef __clang__
            const AST_elem &e{F{}()};
#endif
            constexpr arg0() {}
            constexpr const AST_elem &operator()() const {
              return e.template get_<operation_args_varrefs>().t.vars[0].get(
                  allocator);
            }
          };

          if (there_is_anything_here(
                  e.template get_<operation_args_varrefs>().t.vars[1])) {
            using _arg1 = DECT(as_type<budget - 1, arg1>());
            struct arg1 {
#ifndef __clang__
              const AST_elem &e{F{}()};
#endif
              constexpr arg1() {}
              constexpr const AST_elem &operator()() const {
                return e.template get_<operation_args_varrefs>().t.vars[1].get(
                    allocator);
              }
            };

            if (there_is_anything_here(
                    e.template get_<operation_args_varrefs>().t.vars[2])) {
              using _arg2 = DECT(as_type<budget - 1, arg2>());
              struct arg2 {
#ifndef __clang__
                const AST_elem &e{F{}()};
#endif
                constexpr arg2() {}
                constexpr const AST_elem &operator()() const {
                  return e.template get_<operation_args_varrefs>()
                      .t.vars[2]
                      .get(allocator);
                }
              };

              if (there_is_anything_here(
                      e.template get_<operation_args_varrefs>().t.vars[3])) {
                using _arg3 = DECT(as_type<budget - 1, arg3>());
                struct arg3 {
#ifndef __clang__
                  const AST_elem &e{F{}()};
#endif
                  constexpr arg3() {}
                  constexpr const AST_elem &operator()() const {
                    return e.template get_<operation_args_varrefs>()
                        .t.vars[3]
                        .get(allocator);
                  }
                };

                if (there_is_anything_here(
                        e.template get_<operation_args_varrefs>().t.vars[4])) {
                  using _arg4 = DECT(as_type<budget - 1, arg4>());
                  struct arg4 {
#ifndef __clang__
                    const AST_elem &e{F{}()};
#endif
                    constexpr arg4() {}
                    constexpr const AST_elem &operator()() const {
                      return e.template get_<operation_args_varrefs>()
                          .t.vars[4]
                          .get(allocator);
                    }
                  };

                  if (there_is_anything_here(
                          e.template get_<operation_args_varrefs>()
                              .t.vars[5])) {
                    using _arg5 = DECT(as_type<budget - 1, arg5>());
                    struct arg5 {
#ifndef __clang__
                      const AST_elem &e{F{}()};
#endif
                      constexpr arg5() {}
                      constexpr const AST_elem &operator()() const {
                        return e.template get_<operation_args_varrefs>()
                            .t.vars[5]
                            .get(allocator);
                      }
                    };

                    if (there_is_anything_here(
                            e.template get_<operation_args_varrefs>()
                                .t.vars[6])) {
                      using _arg6 = DECT(as_type<budget - 1, arg6>());
                      struct arg6 {
#ifndef __clang__
                        const AST_elem &e{F{}()};
#endif
                        constexpr arg6() {}
                        constexpr const AST_elem &operator()() const {
                          return e.template get_<operation_args_varrefs>()
                              .t.vars[6]
                              .get(allocator);
                        }
                      };

                      if (there_is_anything_here(
                              e.template get_<operation_args_varrefs>()
                                  .t.vars[7])) {
                        using _arg7 = DECT(as_type<budget - 1, arg7>());
                        struct arg7 {
#ifndef __clang__
                          const AST_elem &e{F{}()};
#endif
                          constexpr arg7() {}
                          constexpr const AST_elem &operator()() const {
                            return e.template get_<operation_args_varrefs>()
                                .t.vars[7]
                                .get(allocator);
                          }
                        };

                        if (there_is_anything_here(
                                e.template get_<operation_args_varrefs>()
                                    .t.vars[8])) {
                          using _arg8 = DECT(as_type<budget - 1, arg8>());
                          struct arg8 {
#ifndef __clang__
                            const AST_elem &e{F{}()};
#endif
                            constexpr arg8() {}
                            constexpr const AST_elem &operator()() const {
                              return e.template get_<operation_args_varrefs>()
                                  .t.vars[8]
                                  .get(allocator);
                            }
                          };

                          if (there_is_anything_here(
                                  e.template get_<operation_args_varrefs>()
                                      .t.vars[9])) {
                            using _arg9 = DECT(as_type<budget - 1, arg9>());
                            struct arg9 {
#ifndef __clang__
                              const AST_elem &e{F{}()};
#endif
                              constexpr arg9() {}
                              constexpr const AST_elem &operator()() const {
                                return e.template get_<operation_args_varrefs>()
                                    .t.vars[9]
                                    .get(allocator);
                              }
                            };

                            if (there_is_anything_here(
                                    e.template get_<operation_args_varrefs>()
                                        .t.vars[10])) {
                              using _arg10 = DECT(as_type<budget - 1, arg10>());
                              struct arg10 {
#ifndef __clang__
                                const AST_elem &e{F{}()};
#endif
                                constexpr arg10() {}
                                constexpr const AST_elem &operator()() const {
                                  return e
                                      .template get_<operation_args_varrefs>()
                                      .t.vars[10]
                                      .get(allocator);
                                }
                              };

                              if (there_is_anything_here(
                                      e.template get_<operation_args_varrefs>()
                                          .t.vars[11])) {
                                using _arg11 =
                                    DECT(as_type<budget - 1, arg11>());
                                struct arg11 {
#ifndef __clang__
                                  const AST_elem &e{F{}()};
#endif
                                  constexpr arg11() {}
                                  constexpr const AST_elem &operator()() const {
                                    return e
                                        .template get_<operation_args_varrefs>()
                                        .t.vars[11]
                                        .get(allocator);
                                  }
                                };

                                if (there_is_anything_here(
                                        e.template get_<
                                             operation_args_varrefs>()
                                            .t.vars[12])) {
                                  using _arg12 =
                                      DECT(as_type<budget - 1, arg12>());
                                  struct arg12 {
#ifndef __clang__
                                    const AST_elem &e{F{}()};
#endif
                                    constexpr arg12() {}
                                    constexpr const AST_elem &
                                    operator()() const {
                                      return e
                                          .template get_<
                                              operation_args_varrefs>()
                                          .t.vars[12]
                                          .get(allocator);
                                    }
                                  };

                                  if (there_is_anything_here(
                                          e.template get_<
                                               operation_args_varrefs>()
                                              .t.vars[13])) {
                                    using _arg13 =
                                        DECT(as_type<budget - 1, arg13>());
                                    struct arg13 {
#ifndef __clang__
                                      const AST_elem &e{F{}()};
#endif
                                      constexpr arg13() {}
                                      constexpr const AST_elem &
                                      operator()() const {
                                        return e
                                            .template get_<
                                                operation_args_varrefs>()
                                            .t.vars[13]
                                            .get(allocator);
                                      }
                                    };

                                    if (there_is_anything_here(
                                            e.template get_<
                                                 operation_args_varrefs>()
                                                .t.vars[14])) {
                                      using _arg14 =
                                          DECT(as_type<budget - 1, arg14>());
                                      struct arg14 {
#ifndef __clang__
                                        const AST_elem &e{F{}()};
#endif
                                        constexpr arg14() {}
                                        constexpr const AST_elem &
                                        operator()() const {
                                          return e
                                              .template get_<
                                                  operation_args_varrefs>()
                                              .t.vars[14]
                                              .get(allocator);
                                        }
                                      };

                                      if (there_is_anything_here(
                                              e.template get_<
                                                   operation_args_varrefs>()
                                                  .t.vars[15])) {
                                        using _arg15 =
                                            DECT(as_type<budget - 1, arg15>());
                                        struct arg15 {
#ifndef __clang__
                                          const AST_elem &e{F{}()};
#endif
                                          constexpr arg15() {}
                                          constexpr const AST_elem &
                                          operator()() const {
                                            return e
                                                .template get_<
                                                    operation_args_varrefs>()
                                                .t.vars[15]
                                                .get(allocator);
                                          }
                                        };

                                        if (there_is_anything_here(
                                                e.template get_<
                                                     operation_args_varrefs>()
                                                    .t.vars[16])) {
                                          using _arg16 = DECT(
                                              as_type<budget - 1, arg16>());
                                          struct arg16 {
#ifndef __clang__
                                            const AST_elem &e{F{}()};
#endif
                                            constexpr arg16() {}
                                            constexpr const AST_elem &
                                            operator()() const {
                                              return e
                                                  .template get_<
                                                      operation_args_varrefs>()
                                                  .t.vars[16]
                                                  .get(allocator);
                                            }
                                          };

                                          if (there_is_anything_here(
                                                  e.template get_<
                                                       operation_args_varrefs>()
                                                      .t.vars[17])) {
                                            using _arg17 = DECT(
                                                as_type<budget - 1, arg17>());
                                            struct arg17 {
#ifndef __clang__
                                              const AST_elem &e{F{}()};
#endif
                                              constexpr arg17() {}
                                              constexpr const AST_elem &
                                              operator()() const {
                                                return e
                                                    .template get_<
                                                        operation_args_varrefs>()
                                                    .t.vars[17]
                                                    .get(allocator);
                                              }
                                            };

                                            if (there_is_anything_here(
                                                    e.template get_<
                                                         operation_args_varrefs>()
                                                        .t.vars[18])) {
                                              using _arg18 = DECT(
                                                  as_type<budget - 1, arg18>());
                                              struct arg18 {
#ifndef __clang__
                                                const AST_elem &e{F{}()};
#endif
                                                constexpr arg18() {}
                                                constexpr const AST_elem &
                                                operator()() const {
                                                  return e
                                                      .template get_<
                                                          operation_args_varrefs>()
                                                      .t.vars[18]
                                                      .get(allocator);
                                                }
                                              };

                                              if (there_is_anything_here(
                                                      e.template get_<
                                                           operation_args_varrefs>()
                                                          .t.vars[19])) {
                                                using _arg19 =
                                                    DECT(as_type<budget - 1,
                                                                 arg19>());
                                                struct arg19 {
#ifndef __clang__
                                                  const AST_elem &e{F{}()};
#endif
                                                  constexpr arg19() {}
                                                  constexpr const AST_elem &
                                                  operator()() const {
                                                    return e
                                                        .template get_<
                                                            operation_args_varrefs>()
                                                        .t.vars[19]
                                                        .get(allocator);
                                                  }
                                                };

                                                return as_types::<
                                                    as_types::
                                                        operation_args_varrefs<
                                                            _arg0, _arg1, _arg2,
                                                            _arg3, _arg4, _arg5,
                                                            _arg6, _arg7, _arg8,
                                                            _arg9, _arg10,
                                                            _arg11, _arg12,
                                                            _arg13, _arg14,
                                                            _arg15, _arg16,
                                                            _arg17, _arg18,
                                                            _arg19>>{};
                                              } else {
                                                return as_types::<
                                                    as_types::
                                                        operation_args_varrefs<
                                                            _arg0, _arg1, _arg2,
                                                            _arg3, _arg4, _arg5,
                                                            _arg6, _arg7, _arg8,
                                                            _arg9, _arg10,
                                                            _arg11, _arg12,
                                                            _arg13, _arg14,
                                                            _arg15, _arg16,
                                                            _arg17, _arg18>>{};
                                              }
                                            } else {
                                              return as_types::<
                                                  as_types::
                                                      operation_args_varrefs<
                                                          _arg0, _arg1, _arg2,
                                                          _arg3, _arg4, _arg5,
                                                          _arg6, _arg7, _arg8,
                                                          _arg9, _arg10, _arg11,
                                                          _arg12, _arg13,
                                                          _arg14, _arg15,
                                                          _arg16, _arg17>>{};
                                            }
                                          } else {
                                            return as_types::<
                                                as_types::
                                                    operation_args_varrefs<
                                                        _arg0, _arg1, _arg2,
                                                        _arg3, _arg4, _arg5,
                                                        _arg6, _arg7, _arg8,
                                                        _arg9, _arg10, _arg11,
                                                        _arg12, _arg13, _arg14,
                                                        _arg15, _arg16>>{};
                                          }
                                        } else {
                                          return as_types::<
                                              as_types::operation_args_varrefs<
                                                  _arg0, _arg1, _arg2, _arg3,
                                                  _arg4, _arg5, _arg6, _arg7,
                                                  _arg8, _arg9, _arg10, _arg11,
                                                  _arg12, _arg13, _arg14,
                                                  _arg15>>{};
                                        }
                                      } else {
                                        return as_types::<
                                            as_types::operation_args_varrefs<
                                                _arg0, _arg1, _arg2, _arg3,
                                                _arg4, _arg5, _arg6, _arg7,
                                                _arg8, _arg9, _arg10, _arg11,
                                                _arg12, _arg13, _arg14>>{};
                                      }
                                    } else {
                                      return as_types::<
                                          as_types::operation_args_varrefs<
                                              _arg0, _arg1, _arg2, _arg3, _arg4,
                                              _arg5, _arg6, _arg7, _arg8, _arg9,
                                              _arg10, _arg11, _arg12,
                                              _arg13>>{};
                                    }
                                  } else {
                                    return as_types::<
                                        as_types::operation_args_varrefs<
                                            _arg0, _arg1, _arg2, _arg3, _arg4,
                                            _arg5, _arg6, _arg7, _arg8, _arg9,
                                            _arg10, _arg11, _arg12>>{};
                                  }
                                } else {
                                  return as_types::<
                                      as_types::operation_args_varrefs<
                                          _arg0, _arg1, _arg2, _arg3, _arg4,
                                          _arg5, _arg6, _arg7, _arg8, _arg9,
                                          _arg10, _arg11>>{};
                                }
                              } else {
                                return as_types::<
                                    as_types::operation_args_varrefs<
                                        _arg0, _arg1, _arg2, _arg3, _arg4,
                                        _arg5, _arg6, _arg7, _arg8, _arg9,
                                        _arg10>>{};
                              }
                            } else {
                              return as_types::<
                                  as_types::operation_args_varrefs<
                                      _arg0, _arg1, _arg2, _arg3, _arg4, _arg5,
                                      _arg6, _arg7, _arg8, _arg9>>{};
                            }
                          } else {
                            return as_types::<as_types::operation_args_varrefs<
                                _arg0, _arg1, _arg2, _arg3, _arg4, _arg5, _arg6,
                                _arg7, _arg8>>{};
                          }
                        } else {
                          return as_types::<as_types::operation_args_varrefs<
                              _arg0, _arg1, _arg2, _arg3, _arg4, _arg5, _arg6,
                              _arg7>>{};
                        }
                      } else {
                        return as_types::<as_types::operation_args_varrefs<
                            _arg0, _arg1, _arg2, _arg3, _arg4, _arg5, _arg6>>{};
                      }
                    } else {
                      return as_types::<as_types::operation_args_varrefs<
                          _arg0, _arg1, _arg2, _arg3, _arg4, _arg5>>{};
                    }
                  } else {
                    return as_types::<as_types::operation_args_varrefs<
                        _arg0, _arg1, _arg2, _arg3, _arg4>>{};
                  }
                } else {
                  return as_types::<as_types::operation_args_varrefs<
                      _arg0, _arg1, _arg2, _arg3>>{};
                }
              } else {
                return as_types::<
                    as_types::operation_args_varrefs<_arg0, _arg1, _arg2>>{};
              }
            } else {
              return as_types::<
                  as_types::operation_args_varrefs<_arg0, _arg1>>{};
            }
          } else {
            return as_types::<as_types::operation_args_varrefs<_arg0>>{};
          }
        }
        return as_types::<as_types::operation_args_varrefs<>>{};
      } else if constexpr (e.template get_<Operation>().is_this_elem) {
        using _arg0 = DECT(as_type<budget - 1, arg0>());
        struct arg0 {
#ifndef __clang__
          const AST_elem &e{F{}()};
#endif
          constexpr arg0() {}
          constexpr const AST_elem &operator()() const {
            return e.template get_<Operation>().t.name.get(allocator);
          }
        };

        using _arg1 = DECT(as_type<budget - 1, arg1>());
        struct arg1 {
#ifndef __clang__
          const AST_elem &e{F{}()};
#endif
          constexpr arg1() {}
          constexpr const AST_elem &operator()() const {
            return e.template get_<Operation>().t.Hndl.get(allocator);
          }
        };

        using _arg2 = DECT(as_type<budget - 1, arg2>());
        struct arg2 {
#ifndef __clang__
          const AST_elem &e{F{}()};
#endif
          constexpr arg2() {}
          constexpr const AST_elem &operator()() const {
            return e.template get_<Operation>().t.expr_args.get(allocator);
          }
        };

        using _arg3 = DECT(as_type<budget - 1, arg3>());
        struct arg3 {
#ifndef __clang__
          const AST_elem &e{F{}()};
#endif
          constexpr arg3() {}
          constexpr const AST_elem &operator()() const {
            return e.template get_<Operation>().t.var_args.get(allocator);
          }
        };

        return as_types::$error_identity_unset<
            as_types::Operation<_arg0, _arg1, _arg2, _arg3>>{};
      } else if constexpr (e.template get_<Assignment>().is_this_elem) {
        using _arg0 = DECT(as_type<budget - 1, arg0>());
        struct arg0 {
#ifndef __clang__
          const AST_elem &e{F{}()};
#endif
          constexpr arg0() {}
          constexpr const AST_elem &operator()() const {
            return e.template get_<Assignment>().t.Var.get(allocator);
          }
        };

        using _arg1 = DECT(as_type<budget - 1, arg1>());
        struct arg1 {
#ifndef __clang__
          const AST_elem &e{F{}()};
#endif
          constexpr arg1() {}
          constexpr const AST_elem &operator()() const {
            return e.template get_<Assignment>().t.Expr.get(allocator);
          }
        };

        return as_types::Statement<as_types::Assignment<_arg0, _arg1>>{};
      } else if constexpr (e.template get_<Return>().is_this_elem) {
        using _arg0 = DECT(as_type<budget - 1, arg0>());
        struct arg0 {
#ifndef __clang__
          const AST_elem &e{F{}()};
#endif
          constexpr arg0() {}
          constexpr const AST_elem &operator()() const {
            return e.template get_<Return>().t.Expr.get(allocator);
          }
        };

        return as_types::Statement<as_types::Return<_arg0>>{};
      } else if constexpr (e.template get_<If>().is_this_elem) {
        using _arg0 = DECT(as_type<budget - 1, arg0>());
        struct arg0 {
#ifndef __clang__
          const AST_elem &e{F{}()};
#endif
          constexpr arg0() {}
          constexpr const AST_elem &operator()() const {
            return e.template get_<If>().t.condition.get(allocator);
          }
        };

        using _arg1 = DECT(as_type<budget - 1, arg1>());
        struct arg1 {
#ifndef __clang__
          const AST_elem &e{F{}()};
#endif
          constexpr arg1() {}
          constexpr const AST_elem &operator()() const {
            return e.template get_<If>().t.then.get(allocator);
          }
        };

        using _arg2 = DECT(as_type<budget - 1, arg2>());
        struct arg2 {
#ifndef __clang__
          const AST_elem &e{F{}()};
#endif
          constexpr arg2() {}
          constexpr const AST_elem &operator()() const {
            return e.template get_<If>().t.els.get(allocator);
          }
        };

        return as_types::Statement<as_types::If<_arg0, _arg1, _arg2>>{};
      } else if constexpr (e.template get_<While>().is_this_elem) {
        using _arg0 = DECT(as_type<budget - 1, arg0>());
        struct arg0 {
#ifndef __clang__
          const AST_elem &e{F{}()};
#endif
          constexpr arg0() {}
          constexpr const AST_elem &operator()() const {
            return e.template get_<While>().t.condition.get(allocator);
          }
        };

        using _arg1 = DECT(as_type<budget - 1, arg1>());
        struct arg1 {
#ifndef __clang__
          const AST_elem &e{F{}()};
#endif
          constexpr arg1() {}
          constexpr const AST_elem &operator()() const {
            return e.template get_<While>().t.body.get(allocator);
          }
        };

        return as_types::Statement<as_types::While<_arg0, _arg1>>{};
      } else if constexpr (e.template get_<Sequence>().is_this_elem) {
        using _arg0 = DECT(as_type<budget - 1, arg0>());
        struct arg0 {
#ifndef __clang__
          const AST_elem &e{F{}()};
#endif
          constexpr arg0() {}
          constexpr const AST_elem &operator()() const {
            return e.template get_<Sequence>().t.e.get(allocator);
          }
        };

        using _arg1 = DECT(as_type<budget - 1, arg1>());
        struct arg1 {
#ifndef __clang__
          const AST_elem &e{F{}()};
#endif
          constexpr arg1() {}
          constexpr const AST_elem &operator()() const {
            return e.template get_<Sequence>().t.next.get(allocator);
          }
        };

        return as_types::Statement<as_types::Sequence<_arg0, _arg1>>{};
      } else if constexpr (e.template get_<Skip>().is_this_elem) {
        return as_types::Statement<as_types::Skip>{};
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
  return as_types::Statement<as_types::transaction<
      DECT(as_type_f<prev_holder>::template as_type<15, arg>()),
      prev_holder::prev.allocator.top.payload>>{};
}
} // namespace as_values

namespace as_types {

template <typename AST_Allocator> struct as_values_ns_fns {
  using AST_elem = as_values::AST_elem;
  constexpr as_values_ns_fns() = default;
  as_values::AST_Allocator<budget> allocator;
  template <typename T> struct converter {
    static constexpr auto value() { return as_values_ns_fns::foo(); }
  } template <typename e, std::size_t payload>
  constexpr allocated_ref<AST_elem>
  as_value(const Statement<transaction<e, payload>> &) {
    auto elem = allocator.template allocate<AST_elem>();
    auto &this_node =
        elem.get(allocator).template get_<as_values::transaction>();
    this_node.is_this_elem = true;
    elem.get(allocator).is_initialized = true;
    this_node.t.e = as_value(e{});
    this_node.t.payload = payload;
    return std::move(elem);
  }
  template <typename Struct, typename Field>
  constexpr allocated_ref<AST_elem>
  as_value(const Expression<FieldReference<Struct, Field>> &) {
    auto elem = allocator.template allocate<AST_elem>();
    auto &this_node =
        elem.get(allocator).template get_<as_values::FieldReference>();
    this_node.is_this_elem = true;
    elem.get(allocator).is_initialized = true;
    this_node.t.Struct = as_value(Struct{});
    this_node.t.Field = as_value(Field{});
    return std::move(elem);
  }
  template <typename Struct, typename Field>
  constexpr allocated_ref<AST_elem>
  as_value(const Expression<FieldPointerReference<Struct, Field>> &) {
    auto elem = allocator.template allocate<AST_elem>();
    auto &this_node =
        elem.get(allocator).template get_<as_values::FieldPointerReference>();
    this_node.is_this_elem = true;
    elem.get(allocator).is_initialized = true;
    this_node.t.Struct = as_value(Struct{});
    this_node.t.Field = as_value(Field{});
    return std::move(elem);
  }
  template <typename Struct>
  constexpr allocated_ref<AST_elem>
  as_value(const Expression<Dereference<Struct>> &) {
    auto elem = allocator.template allocate<AST_elem>();
    auto &this_node =
        elem.get(allocator).template get_<as_values::Dereference>();
    this_node.is_this_elem = true;
    elem.get(allocator).is_initialized = true;
    this_node.t.Struct = as_value(Struct{});
    return std::move(elem);
  }
  template <typename label, typename Hndl>
  constexpr allocated_ref<AST_elem>
  as_value(const Expression<Endorse<label, Hndl>> &) {
    auto elem = allocator.template allocate<AST_elem>();
    auto &this_node = elem.get(allocator).template get_<as_values::Endorse>();
    this_node.is_this_elem = true;
    elem.get(allocator).is_initialized = true;
    this_node.t.label = as_value(label{});
    this_node.t.Hndl = as_value(Hndl{});
    return std::move(elem);
  }
  template <typename label, typename Hndl>
  constexpr allocated_ref<AST_elem>
  as_value(const Expression<Ensure<label, Hndl>> &) {
    auto elem = allocator.template allocate<AST_elem>();
    auto &this_node = elem.get(allocator).template get_<as_values::Ensure>();
    this_node.is_this_elem = true;
    elem.get(allocator).is_initialized = true;
    this_node.t.label = as_value(label{});
    this_node.t.Hndl = as_value(Hndl{});
    return std::move(elem);
  }
  template <typename Hndl>
  constexpr allocated_ref<AST_elem>
  as_value(const Expression<IsValid<Hndl>> &) {
    auto elem = allocator.template allocate<AST_elem>();
    auto &this_node = elem.get(allocator).template get_<as_values::IsValid>();
    this_node.is_this_elem = true;
    elem.get(allocator).is_initialized = true;
    this_node.t.Hndl = as_value(Hndl{});
    return std::move(elem);
  }
  template <typename Var>
  constexpr allocated_ref<AST_elem>
  as_value(const Expression<VarReference<Var>> &) {
    auto elem = allocator.template allocate<AST_elem>();
    auto &this_node =
        elem.get(allocator).template get_<as_values::VarReference>();
    this_node.is_this_elem = true;
    elem.get(allocator).is_initialized = true;
    this_node.t.Var = as_value(Var{});
    return std::move(elem);
  }
  template <std::size_t i>
  constexpr allocated_ref<AST_elem> as_value(const Expression<Constant<i>> &) {
    auto elem = allocator.template allocate<AST_elem>();
    auto &this_node = elem.get(allocator).template get_<as_values::Constant>();
    this_node.is_this_elem = true;
    elem.get(allocator).is_initialized = true;
    this_node.t.i = i;
    return std::move(elem);
  }
  template <char op, typename L, typename R>
  constexpr allocated_ref<AST_elem>
  as_value(const Expression<BinOp<op, L, R>> &) {
    auto elem = allocator.template allocate<AST_elem>();
    auto &this_node = elem.get(allocator).template get_<as_values::BinOp>();
    this_node.is_this_elem = true;
    elem.get(allocator).is_initialized = true;
    this_node.t.op = op;
    this_node.t.L = as_value(L{});
    this_node.t.R = as_value(R{});
    return std::move(elem);
  }
  template <typename Binding, typename Body>
  constexpr allocated_ref<AST_elem>
  as_value(const Statement<Let<Binding, Body>> &) {
    auto elem = allocator.template allocate<AST_elem>();
    auto &this_node = elem.get(allocator).template get_<as_values::Let>();
    this_node.is_this_elem = true;
    elem.get(allocator).is_initialized = true;
    this_node.t.Binding = as_value(Binding{});
    this_node.t.Body = as_value(Body{});
    return std::move(elem);
  }
  template <typename Binding, typename Body>
  constexpr allocated_ref<AST_elem>
  as_value(const Statement<LetRemote<Binding, Body>> &) {
    auto elem = allocator.template allocate<AST_elem>();
    auto &this_node = elem.get(allocator).template get_<as_values::LetRemote>();
    this_node.is_this_elem = true;
    elem.get(allocator).is_initialized = true;
    this_node.t.Binding = as_value(Binding{});
    this_node.t.Body = as_value(Body{});
    return std::move(elem);
  }
  assert_fail_argument_packtemplate<typename exprs> constexpr allocated_ref<
      AST_elem>
  as_value(const operation_args_exprs &) {
    auto elem = allocator.template allocate<AST_elem>();
    auto &this_node =
        elem.get(allocator).template get_<as_values::operation_args_exprs>();
    this_node.is_this_elem = true;
    elem.get(allocator).is_initialized = true;
    this_node.t.exprs = as_value(exprs{});
    return std::move(elem);
  }
  assert_fail_argument_packtemplate<typename vars> constexpr allocated_ref<
      AST_elem>
  as_value(const operation_args_varrefs &) {
    auto elem = allocator.template allocate<AST_elem>();
    auto &this_node =
        elem.get(allocator).template get_<as_values::operation_args_varrefs>();
    this_node.is_this_elem = true;
    elem.get(allocator).is_initialized = true;
    this_node.t.vars = as_value(vars{});
    return std::move(elem);
  }
  template <typename name, typename Hndl, typename expr_args, typename var_args>
  constexpr allocated_ref<AST_elem> as_value(
      const $error_identity_unset<Operation<name, Hndl, expr_args, var_args>>
          &) {
    auto elem = allocator.template allocate<AST_elem>();
    auto &this_node = elem.get(allocator).template get_<as_values::Operation>();
    this_node.is_this_elem = true;
    elem.get(allocator).is_initialized = true;
    this_node.t.name = as_value(name{});
    this_node.t.Hndl = as_value(Hndl{});
    this_node.t.expr_args = as_value(expr_args{});
    this_node.t.var_args = as_value(var_args{});
    return std::move(elem);
  }
  template <typename Var, typename Expr>
  constexpr allocated_ref<AST_elem>
  as_value(const Statement<Assignment<Var, Expr>> &) {
    auto elem = allocator.template allocate<AST_elem>();
    auto &this_node =
        elem.get(allocator).template get_<as_values::Assignment>();
    this_node.is_this_elem = true;
    elem.get(allocator).is_initialized = true;
    this_node.t.Var = as_value(Var{});
    this_node.t.Expr = as_value(Expr{});
    return std::move(elem);
  }
  template <typename Expr>
  constexpr allocated_ref<AST_elem> as_value(const Statement<Return<Expr>> &) {
    auto elem = allocator.template allocate<AST_elem>();
    auto &this_node = elem.get(allocator).template get_<as_values::Return>();
    this_node.is_this_elem = true;
    elem.get(allocator).is_initialized = true;
    this_node.t.Expr = as_value(Expr{});
    return std::move(elem);
  }
  template <typename condition, typename then, typename els>
  constexpr allocated_ref<AST_elem>
  as_value(const Statement<If<condition, then, els>> &) {
    auto elem = allocator.template allocate<AST_elem>();
    auto &this_node = elem.get(allocator).template get_<as_values::If>();
    this_node.is_this_elem = true;
    elem.get(allocator).is_initialized = true;
    this_node.t.condition = as_value(condition{});
    this_node.t.then = as_value(then{});
    this_node.t.els = as_value(els{});
    return std::move(elem);
  }
  template <typename condition, typename body>
  constexpr allocated_ref<AST_elem>
  as_value(const Statement<While<condition, body>> &) {
    auto elem = allocator.template allocate<AST_elem>();
    auto &this_node = elem.get(allocator).template get_<as_values::While>();
    this_node.is_this_elem = true;
    elem.get(allocator).is_initialized = true;
    this_node.t.condition = as_value(condition{});
    this_node.t.body = as_value(body{});
    return std::move(elem);
  }
  template <typename e, typename next>
  constexpr allocated_ref<AST_elem>
  as_value(const Statement<Sequence<e, next>> &) {
    auto elem = allocator.template allocate<AST_elem>();
    auto &this_node = elem.get(allocator).template get_<as_values::Sequence>();
    this_node.is_this_elem = true;
    elem.get(allocator).is_initialized = true;
    this_node.t.e = as_value(e{});
    this_node.t.next = as_value(next{});
    return std::move(elem);
  }
  template <>
  constexpr allocated_ref<AST_elem> as_value(const Statement<Skip> &) {
    auto elem = allocator.template allocate<AST_elem>();
    auto &this_node = elem.get(allocator).template get_<as_values::Skip>();
    this_node.is_this_elem = true;
    elem.get(allocator).is_initialized = true;
    return std::move(elem);
  }
};

template <std::size_t budget, typename hd>
constexpr as_values::AST_Allocator<budget> as_value() {
  static_assert(is_astnode_transaction<hd>::value);
  as_values_ns_fns<as_values::AST_Allocator<budget>> ret;
  ret.allocator.top = std::move(ret.as_value(hd{})
                                    .get(ret.allocator)
                                    .template get<as_values::transaction>());
  return std::move(ret.allocator);
}
} // namespace as_types

namespace as_values {
template <typename Allocator>
std::ostream &print(std::ostream &o, const std::size_t &st, const Allocator &) {
  return o << st;
}
template <typename Allocator>
std::ostream &print(std::ostream &o, const AST_elem &e,
                    const Allocator &allocator);

template <typename Allocator>
std::ostream &print(std::ostream &o, const transaction &e,
                    const Allocator &allocator) {
  o << "transaction{";
  print(o, e.e, allocator);
  o << ",";
  print(o, e.payload, allocator);
  o << ",";
  return o << "}";
}
template <typename Allocator>
std::ostream &print(std::ostream &o, const FieldReference &e,
                    const Allocator &allocator) {
  o << "FieldReference{";
  print(o, e.Struct, allocator);
  o << ",";
  print(o, e.Field, allocator);
  o << ",";
  return o << "}";
}
template <typename Allocator>
std::ostream &print(std::ostream &o, const FieldPointerReference &e,
                    const Allocator &allocator) {
  o << "FieldPointerReference{";
  print(o, e.Struct, allocator);
  o << ",";
  print(o, e.Field, allocator);
  o << ",";
  return o << "}";
}
template <typename Allocator>
std::ostream &print(std::ostream &o, const Dereference &e,
                    const Allocator &allocator) {
  o << "Dereference{";
  print(o, e.Struct, allocator);
  o << ",";
  return o << "}";
}
template <typename Allocator>
std::ostream &print(std::ostream &o, const Endorse &e,
                    const Allocator &allocator) {
  o << "Endorse{";
  print(o, e.label, allocator);
  o << ",";
  print(o, e.Hndl, allocator);
  o << ",";
  return o << "}";
}
template <typename Allocator>
std::ostream &print(std::ostream &o, const Ensure &e,
                    const Allocator &allocator) {
  o << "Ensure{";
  print(o, e.label, allocator);
  o << ",";
  print(o, e.Hndl, allocator);
  o << ",";
  return o << "}";
}
template <typename Allocator>
std::ostream &print(std::ostream &o, const IsValid &e,
                    const Allocator &allocator) {
  o << "IsValid{";
  print(o, e.Hndl, allocator);
  o << ",";
  return o << "}";
}
template <typename Allocator>
std::ostream &print(std::ostream &o, const VarReference &e,
                    const Allocator &allocator) {
  o << "VarReference{";
  print(o, e.Var, allocator);
  o << ",";
  return o << "}";
}
template <typename Allocator>
std::ostream &print(std::ostream &o, const Constant &e,
                    const Allocator &allocator) {
  o << "Constant{";
  print(o, e.i, allocator);
  o << ",";
  return o << "}";
}
template <typename Allocator>
std::ostream &print(std::ostream &o, const BinOp &e,
                    const Allocator &allocator) {
  o << "BinOp{";
  print(o, e.op, allocator);
  o << ",";
  print(o, e.L, allocator);
  o << ",";
  print(o, e.R, allocator);
  o << ",";
  return o << "}";
}
template <typename Allocator>
std::ostream &print(std::ostream &o, const Let &e, const Allocator &allocator) {
  o << "Let{";
  print(o, e.Binding, allocator);
  o << ",";
  print(o, e.Body, allocator);
  o << ",";
  return o << "}";
}
template <typename Allocator>
std::ostream &print(std::ostream &o, const LetRemote &e,
                    const Allocator &allocator) {
  o << "LetRemote{";
  print(o, e.Binding, allocator);
  o << ",";
  print(o, e.Body, allocator);
  o << ",";
  return o << "}";
}
template <typename Allocator>
std::ostream &print(std::ostream &o, const operation_args_exprs &e,
                    const Allocator &allocator) {
  o << "operation_args_exprs{";
  print(o, e.exprs, allocator);
  o << ",";
  return o << "}";
}
template <typename Allocator>
std::ostream &print(std::ostream &o, const operation_args_varrefs &e,
                    const Allocator &allocator) {
  o << "operation_args_varrefs{";
  print(o, e.vars, allocator);
  o << ",";
  return o << "}";
}
template <typename Allocator>
std::ostream &print(std::ostream &o, const Operation &e,
                    const Allocator &allocator) {
  o << "Operation{";
  print(o, e.name, allocator);
  o << ",";
  print(o, e.Hndl, allocator);
  o << ",";
  print(o, e.expr_args, allocator);
  o << ",";
  print(o, e.var_args, allocator);
  o << ",";
  return o << "}";
}
template <typename Allocator>
std::ostream &print(std::ostream &o, const Assignment &e,
                    const Allocator &allocator) {
  o << "Assignment{";
  print(o, e.Var, allocator);
  o << ",";
  print(o, e.Expr, allocator);
  o << ",";
  return o << "}";
}
template <typename Allocator>
std::ostream &print(std::ostream &o, const Return &e,
                    const Allocator &allocator) {
  o << "Return{";
  print(o, e.Expr, allocator);
  o << ",";
  return o << "}";
}
template <typename Allocator>
std::ostream &print(std::ostream &o, const If &e, const Allocator &allocator) {
  o << "If{";
  print(o, e.condition, allocator);
  o << ",";
  print(o, e.then, allocator);
  o << ",";
  print(o, e.els, allocator);
  o << ",";
  return o << "}";
}
template <typename Allocator>
std::ostream &print(std::ostream &o, const While &e,
                    const Allocator &allocator) {
  o << "While{";
  print(o, e.condition, allocator);
  o << ",";
  print(o, e.body, allocator);
  o << ",";
  return o << "}";
}
template <typename Allocator>
std::ostream &print(std::ostream &o, const Sequence &e,
                    const Allocator &allocator) {
  o << "Sequence{";
  print(o, e.e, allocator);
  o << ",";
  print(o, e.next, allocator);
  o << ",";
  return o << "}";
}
template <typename Allocator>
std::ostream &print(std::ostream &o, const Skip &e,
                    const Allocator &allocator) {
  o << "Skip{";
  return o << "}";
}
template <typename Allocator>
std::ostream &print(std::ostream &o, const AST_elem &e,
                    const Allocator &allocator) {
  if (e.template get_<transaction>().is_this_elem) {
    return print(o, e.template get<transaction>(), allocator);
  }
  if (e.template get_<FieldReference>().is_this_elem) {
    return print(o, e.template get<FieldReference>(), allocator);
  }
  if (e.template get_<FieldPointerReference>().is_this_elem) {
    return print(o, e.template get<FieldPointerReference>(), allocator);
  }
  if (e.template get_<Dereference>().is_this_elem) {
    return print(o, e.template get<Dereference>(), allocator);
  }
  if (e.template get_<Endorse>().is_this_elem) {
    return print(o, e.template get<Endorse>(), allocator);
  }
  if (e.template get_<Ensure>().is_this_elem) {
    return print(o, e.template get<Ensure>(), allocator);
  }
  if (e.template get_<IsValid>().is_this_elem) {
    return print(o, e.template get<IsValid>(), allocator);
  }
  if (e.template get_<VarReference>().is_this_elem) {
    return print(o, e.template get<VarReference>(), allocator);
  }
  if (e.template get_<Constant>().is_this_elem) {
    return print(o, e.template get<Constant>(), allocator);
  }
  if (e.template get_<BinOp>().is_this_elem) {
    return print(o, e.template get<BinOp>(), allocator);
  }
  if (e.template get_<Let>().is_this_elem) {
    return print(o, e.template get<Let>(), allocator);
  }
  if (e.template get_<LetRemote>().is_this_elem) {
    return print(o, e.template get<LetRemote>(), allocator);
  }
  if (e.template get_<operation_args_exprs>().is_this_elem) {
    return print(o, e.template get<operation_args_exprs>(), allocator);
  }
  if (e.template get_<operation_args_varrefs>().is_this_elem) {
    return print(o, e.template get<operation_args_varrefs>(), allocator);
  }
  if (e.template get_<Operation>().is_this_elem) {
    return print(o, e.template get<Operation>(), allocator);
  }
  if (e.template get_<Assignment>().is_this_elem) {
    return print(o, e.template get<Assignment>(), allocator);
  }
  if (e.template get_<Return>().is_this_elem) {
    return print(o, e.template get<Return>(), allocator);
  }
  if (e.template get_<If>().is_this_elem) {
    return print(o, e.template get<If>(), allocator);
  }
  if (e.template get_<While>().is_this_elem) {
    return print(o, e.template get<While>(), allocator);
  }
  if (e.template get_<Sequence>().is_this_elem) {
    return print(o, e.template get<Sequence>(), allocator);
  }
  if (e.template get_<Skip>().is_this_elem) {
    return print(o, e.template get<Skip>(), allocator);
  }
  return o;
}
} // namespace as_values

namespace as_types {
template <typename> struct is_astnode_transaction : public std::false_type {};
template <typename e, std::size_t payload>
struct is_astnode_transaction<Statement<transaction<e, payload>>>
    : public std::true_type {};
template <typename>
struct is_astnode_FieldReference : public std::false_type {};
template <typename Struct, typename Field>
struct is_astnode_FieldReference<Expression<FieldReference<Struct, Field>>>
    : public std::true_type {};
template <typename>
struct is_astnode_FieldPointerReference : public std::false_type {};
template <typename Struct, typename Field>
struct is_astnode_FieldPointerReference<
    Expression<FieldPointerReference<Struct, Field>>> : public std::true_type {
};
template <typename> struct is_astnode_Dereference : public std::false_type {};
template <typename Struct>
struct is_astnode_Dereference<Expression<Dereference<Struct>>>
    : public std::true_type {};
template <typename> struct is_astnode_Endorse : public std::false_type {};
template <typename label, typename Hndl>
struct is_astnode_Endorse<Expression<Endorse<label, Hndl>>>
    : public std::true_type {};
template <typename> struct is_astnode_Ensure : public std::false_type {};
template <typename label, typename Hndl>
struct is_astnode_Ensure<Expression<Ensure<label, Hndl>>>
    : public std::true_type {};
template <typename> struct is_astnode_IsValid : public std::false_type {};
template <typename Hndl>
struct is_astnode_IsValid<Expression<IsValid<Hndl>>> : public std::true_type {};
template <typename> struct is_astnode_VarReference : public std::false_type {};
template <typename Var>
struct is_astnode_VarReference<Expression<VarReference<Var>>>
    : public std::true_type {};
template <typename> struct is_astnode_Constant : public std::false_type {};
template <std::size_t i>
struct is_astnode_Constant<Expression<Constant<i>>> : public std::true_type {};
template <typename> struct is_astnode_BinOp : public std::false_type {};
template <char op, typename L, typename R>
struct is_astnode_BinOp<Expression<BinOp<op, L, R>>> : public std::true_type {};
template <typename> struct is_astnode_Let : public std::false_type {};
template <typename Binding, typename Body>
struct is_astnode_Let<Statement<Let<Binding, Body>>> : public std::true_type {};
template <typename> struct is_astnode_LetRemote : public std::false_type {};
template <typename Binding, typename Body>
struct is_astnode_LetRemote<Statement<LetRemote<Binding, Body>>>
    : public std::true_type {};
template <typename> struct is_astnode_Operation : public std::false_type {};
template <typename name, typename Hndl, typename expr_args, typename var_args>
struct is_astnode_Operation<
    $error_identity_unset<Operation<name, Hndl, expr_args, var_args>>>
    : public std::true_type {};
template <typename> struct is_astnode_Assignment : public std::false_type {};
template <typename Var, typename Expr>
struct is_astnode_Assignment<Statement<Assignment<Var, Expr>>>
    : public std::true_type {};
template <typename> struct is_astnode_Return : public std::false_type {};
template <typename Expr>
struct is_astnode_Return<Statement<Return<Expr>>> : public std::true_type {};
template <typename> struct is_astnode_If : public std::false_type {};
template <typename condition, typename then, typename els>
struct is_astnode_If<Statement<If<condition, then, els>>>
    : public std::true_type {};
template <typename> struct is_astnode_While : public std::false_type {};
template <typename condition, typename body>
struct is_astnode_While<Statement<While<condition, body>>>
    : public std::true_type {};
template <typename> struct is_astnode_Sequence : public std::false_type {};
template <typename e, typename next>
struct is_astnode_Sequence<Statement<Sequence<e, next>>>
    : public std::true_type {};
template <typename> struct is_astnode_Skip : public std::false_type {};
template <> struct is_astnode_Skip<Statement<Skip>> : public std::true_type {};
} // namespace as_types