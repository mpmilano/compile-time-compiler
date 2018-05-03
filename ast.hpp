
#pragma once
#include "allocator.hpp"
#include "mutils/type_utils.hpp"
#include "union.hpp"
#include <ostream>

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
          Operation, Assignment, Return, If, While, Sequence, Skip>;
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
struct FieldReference {
  allocated_ref<AST_elem> Struct{};
  3 Field{};
  constexpr FieldReference(){};
  constexpr FieldReference(FieldReference &&p)
      : Struct{std::move(p.Struct)}, Field{std::move(p.Field)} {}
  constexpr FieldReference &operator=(FieldReference &&p) {
    Struct = std::move(p.Struct);
    Field = std::move(p.Field);
    return *this;
  }
};
struct FieldPointerReference {
  allocated_ref<AST_elem> Struct{};
  3 Field{};
  constexpr FieldPointerReference(){};
  constexpr FieldPointerReference(FieldPointerReference &&p)
      : Struct{std::move(p.Struct)}, Field{std::move(p.Field)} {}
  constexpr FieldPointerReference &operator=(FieldPointerReference &&p) {
    Struct = std::move(p.Struct);
    Field = std::move(p.Field);
    return *this;
  }
};
struct Dereference {
  allocated_ref<AST_elem> Struct{};
  constexpr Dereference(){};
  constexpr Dereference(Dereference &&p) : Struct{std::move(p.Struct)} {}
  constexpr Dereference &operator=(Dereference &&p) {
    Struct = std::move(p.Struct);
    return *this;
  }
};
struct Endorse {
  4 label{};
  allocated_ref<AST_elem> Hndl{};
  constexpr Endorse(){};
  constexpr Endorse(Endorse &&p)
      : label{std::move(p.label)}, Hndl{std::move(p.Hndl)} {}
  constexpr Endorse &operator=(Endorse &&p) {
    label = std::move(p.label);
    Hndl = std::move(p.Hndl);
    return *this;
  }
};
struct Ensure {
  4 label{};
  allocated_ref<AST_elem> Hndl{};
  constexpr Ensure(){};
  constexpr Ensure(Ensure &&p)
      : label{std::move(p.label)}, Hndl{std::move(p.Hndl)} {}
  constexpr Ensure &operator=(Ensure &&p) {
    label = std::move(p.label);
    Hndl = std::move(p.Hndl);
    return *this;
  }
};
struct IsValid {
  allocated_ref<AST_elem> Hndl{};
  constexpr IsValid(){};
  constexpr IsValid(IsValid &&p) : Hndl{std::move(p.Hndl)} {}
  constexpr IsValid &operator=(IsValid &&p) {
    Hndl = std::move(p.Hndl);
    return *this;
  }
};
struct VarReference {
  3 Var{};
  constexpr VarReference(){};
  constexpr VarReference(VarReference &&p) : Var{std::move(p.Var)} {}
  constexpr VarReference &operator=(VarReference &&p) {
    Var = std::move(p.Var);
    return *this;
  }
};
struct Constant {
  std::size_t i{0};
  constexpr Constant(){};
  constexpr Constant(Constant &&p) : i{std::move(p.i)} {}
  constexpr Constant &operator=(Constant &&p) {
    i = std::move(p.i);
    return *this;
  }
};
struct BinOp {
  char op{};
  allocated_ref<AST_elem> L{};
  allocated_ref<AST_elem> R{};
  constexpr BinOp(){};
  constexpr BinOp(BinOp &&p)
      : op{std::move(p.op)}, L{std::move(p.L)}, R{std::move(p.R)} {}
  constexpr BinOp &operator=(BinOp &&p) {
    op = std::move(p.op);
    L = std::move(p.L);
    R = std::move(p.R);
    return *this;
  }
};
struct Let {
  allocated_ref<AST_elem> Binding{};
  allocated_ref<AST_elem> Body{};
  constexpr Let(){};
  constexpr Let(Let &&p)
      : Binding{std::move(p.Binding)}, Body{std::move(p.Body)} {}
  constexpr Let &operator=(Let &&p) {
    Binding = std::move(p.Binding);
    Body = std::move(p.Body);
    return *this;
  }
};
struct LetRemote {
  allocated_ref<AST_elem> Binding{};
  allocated_ref<AST_elem> Body{};
  constexpr LetRemote(){};
  constexpr LetRemote(LetRemote &&p)
      : Binding{std::move(p.Binding)}, Body{std::move(p.Body)} {}
  constexpr LetRemote &operator=(LetRemote &&p) {
    Binding = std::move(p.Binding);
    Body = std::move(p.Body);
    return *this;
  }
};
struct operation_args_exprs {
  6 exprs{};
  constexpr operation_args_exprs(){};
  constexpr operation_args_exprs(operation_args_exprs &&p)
      : exprs{std::move(p.exprs)} {}
  constexpr operation_args_exprs &operator=(operation_args_exprs &&p) {
    exprs = std::move(p.exprs);
    return *this;
  }
};
struct operation_args_varrefs {
  6 vars{};
  constexpr operation_args_varrefs(){};
  constexpr operation_args_varrefs(operation_args_varrefs &&p)
      : vars{std::move(p.vars)} {}
  constexpr operation_args_varrefs &operator=(operation_args_varrefs &&p) {
    vars = std::move(p.vars);
    return *this;
  }
};
struct Operation {
  3 name{};
  allocated_ref<AST_elem> Hndl{};
  5 expr_args{};
  5 var_args{};
  constexpr Operation(){};
  constexpr Operation(Operation &&p)
      : name{std::move(p.name)}, Hndl{std::move(p.Hndl)},
        expr_args{std::move(p.expr_args)}, var_args{std::move(p.var_args)} {}
  constexpr Operation &operator=(Operation &&p) {
    name = std::move(p.name);
    Hndl = std::move(p.Hndl);
    expr_args = std::move(p.expr_args);
    var_args = std::move(p.var_args);
    return *this;
  }
};
struct Operation {
  3 name{};
  allocated_ref<AST_elem> Hndl{};
  5 expr_args{};
  5 var_args{};
  constexpr Operation(){};
  constexpr Operation(Operation &&p)
      : name{std::move(p.name)}, Hndl{std::move(p.Hndl)},
        expr_args{std::move(p.expr_args)}, var_args{std::move(p.var_args)} {}
  constexpr Operation &operator=(Operation &&p) {
    name = std::move(p.name);
    Hndl = std::move(p.Hndl);
    expr_args = std::move(p.expr_args);
    var_args = std::move(p.var_args);
    return *this;
  }
};
struct Assignment {
  allocated_ref<AST_elem> Var{};
  allocated_ref<AST_elem> Expr{};
  constexpr Assignment(){};
  constexpr Assignment(Assignment &&p)
      : Var{std::move(p.Var)}, Expr{std::move(p.Expr)} {}
  constexpr Assignment &operator=(Assignment &&p) {
    Var = std::move(p.Var);
    Expr = std::move(p.Expr);
    return *this;
  }
};
struct Return {
  allocated_ref<AST_elem> Expr{};
  constexpr Return(){};
  constexpr Return(Return &&p) : Expr{std::move(p.Expr)} {}
  constexpr Return &operator=(Return &&p) {
    Expr = std::move(p.Expr);
    return *this;
  }
};
struct If {
  allocated_ref<AST_elem> condition{};
  allocated_ref<AST_elem> then{};
  allocated_ref<AST_elem> els{};
  constexpr If(){};
  constexpr If(If &&p)
      : condition{std::move(p.condition)}, then{std::move(p.then)},
        els{std::move(p.els)} {}
  constexpr If &operator=(If &&p) {
    condition = std::move(p.condition);
    then = std::move(p.then);
    els = std::move(p.els);
    return *this;
  }
};
struct While {
  allocated_ref<AST_elem> condition{};
  allocated_ref<AST_elem> body{};
  constexpr While(){};
  constexpr While(While &&p)
      : condition{std::move(p.condition)}, body{std::move(p.body)} {}
  constexpr While &operator=(While &&p) {
    condition = std::move(p.condition);
    body = std::move(p.body);
    return *this;
  }
};
struct Sequence {
  allocated_ref<AST_elem> e{};
  allocated_ref<AST_elem> next{};
  constexpr Sequence(){};
  constexpr Sequence(Sequence &&p)
      : e{std::move(p.e)}, next{std::move(p.next)} {}
  constexpr Sequence &operator=(Sequence &&p) {
    e = std::move(p.e);
    next = std::move(p.next);
    return *this;
  }
};
struct Skip {
  constexpr Skip(){};
  constexpr Skip(Skip &&p) {}
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
template <typename> struct is_astnode_transaction : public std::false_type {};
template <typename _e, std::size_t _payload>
struct is_astnode_transaction<Statement<transaction<_e, _payload>>>
    : public std::true_type {};
template <typename Struct, 3 Field> struct FieldReference {};
template <typename _Struct, 3 _Field>
struct Expression<FieldReference<_Struct, _Field>> {
  using Struct = _Struct;
  3 Field{_Field};
};
template <typename>
struct is_astnode_FieldReference : public std::false_type {};
template <typename _Struct, 3 _Field>
struct is_astnode_FieldReference<Expression<FieldReference<_Struct, _Field>>>
    : public std::true_type {};
template <typename Struct, 3 Field> struct FieldPointerReference {};
template <typename _Struct, 3 _Field>
struct Expression<FieldPointerReference<_Struct, _Field>> {
  using Struct = _Struct;
  3 Field{_Field};
};
template <typename>
struct is_astnode_FieldPointerReference : public std::false_type {};
template <typename _Struct, 3 _Field>
struct is_astnode_FieldPointerReference<
    Expression<FieldPointerReference<_Struct, _Field>>>
    : public std::true_type {};
template <typename Struct> struct Dereference {};
template <typename _Struct> struct Expression<Dereference<_Struct>> {
  using Struct = _Struct;
};
template <typename> struct is_astnode_Dereference : public std::false_type {};
template <typename _Struct>
struct is_astnode_Dereference<Expression<Dereference<_Struct>>>
    : public std::true_type {};
template <4 label, typename Hndl> struct Endorse {};
template <4 _label, typename _Hndl> struct Expression<Endorse<_label, _Hndl>> {
  4 label{_label};
  using Hndl = _Hndl;
};
template <typename> struct is_astnode_Endorse : public std::false_type {};
template <4 _label, typename _Hndl>
struct is_astnode_Endorse<Expression<Endorse<_label, _Hndl>>>
    : public std::true_type {};
template <4 label, typename Hndl> struct Ensure {};
template <4 _label, typename _Hndl> struct Expression<Ensure<_label, _Hndl>> {
  4 label{_label};
  using Hndl = _Hndl;
};
template <typename> struct is_astnode_Ensure : public std::false_type {};
template <4 _label, typename _Hndl>
struct is_astnode_Ensure<Expression<Ensure<_label, _Hndl>>>
    : public std::true_type {};
template <typename Hndl> struct IsValid {};
template <typename _Hndl> struct Expression<IsValid<_Hndl>> {
  using Hndl = _Hndl;
};
template <typename> struct is_astnode_IsValid : public std::false_type {};
template <typename _Hndl>
struct is_astnode_IsValid<Expression<IsValid<_Hndl>>> : public std::true_type {
};
template <3 Var> struct VarReference {};
template <3 _Var> struct Expression<VarReference<_Var>> { 3 Var{_Var}; };
template <typename> struct is_astnode_VarReference : public std::false_type {};
template <3 _Var>
struct is_astnode_VarReference<Expression<VarReference<_Var>>>
    : public std::true_type {};
template <std::size_t i> struct Constant {};
template <std::size_t _i> struct Expression<Constant<_i>> {
  std::size_t i{_i};
};
template <typename> struct is_astnode_Constant : public std::false_type {};
template <std::size_t _i>
struct is_astnode_Constant<Expression<Constant<_i>>> : public std::true_type {};
template <char op, typename L, typename R> struct BinOp {};
template <char _op, typename _L, typename _R>
struct Expression<BinOp<_op, _L, _R>> {
  char op{_op};
  using L = _L;
  using R = _R;
};
template <typename> struct is_astnode_BinOp : public std::false_type {};
template <char _op, typename _L, typename _R>
struct is_astnode_BinOp<Expression<BinOp<_op, _L, _R>>>
    : public std::true_type {};
template <typename Binding, typename Body> struct Let {};
template <typename _Binding, typename _Body>
struct Statement<Let<_Binding, _Body>> {
  using Binding = _Binding;
  using Body = _Body;
};
template <typename> struct is_astnode_Let : public std::false_type {};
template <typename _Binding, typename _Body>
struct is_astnode_Let<Statement<Let<_Binding, _Body>>> : public std::true_type {
};
template <typename Binding, typename Body> struct LetRemote {};
template <typename _Binding, typename _Body>
struct Statement<LetRemote<_Binding, _Body>> {
  using Binding = _Binding;
  using Body = _Body;
};
template <typename> struct is_astnode_LetRemote : public std::false_type {};
template <typename _Binding, typename _Body>
struct is_astnode_LetRemote<Statement<LetRemote<_Binding, _Body>>>
    : public std::true_type {};
template <6 exprs> struct operation_args_exprs {};
template <6 _exprs> struct error<operation_args_exprs<_exprs>> {
  6 exprs{_exprs};
};
template <typename>
struct is_astnode_operation_args_exprs : public std::false_type {};
template <6 _exprs>
struct is_astnode_operation_args_exprs<error<operation_args_exprs<_exprs>>>
    : public std::true_type {};
template <6 vars> struct operation_args_varrefs {};
template <6 _vars> struct error<operation_args_varrefs<_vars>> {
  6 vars{_vars};
};
template <typename>
struct is_astnode_operation_args_varrefs : public std::false_type {};
template <6 _vars>
struct is_astnode_operation_args_varrefs<error<operation_args_varrefs<_vars>>>
    : public std::true_type {};
template <3 name, typename Hndl, 5 expr_args, 5 var_args> struct Operation {};
template <3 _name, typename _Hndl, 5 _expr_args, 5 _var_args>
struct Expression<Operation<_name, _Hndl, _expr_args, _var_args>> {
  3 name{_name};
  using Hndl = _Hndl;
  5 expr_args{_expr_args};
  5 var_args{_var_args};
};
template <typename> struct is_astnode_Operation : public std::false_type {};
template <3 _name, typename _Hndl, 5 _expr_args, 5 _var_args>
struct is_astnode_Operation<
    Expression<Operation<_name, _Hndl, _expr_args, _var_args>>>
    : public std::true_type {};
template <3 name, typename Hndl, 5 expr_args, 5 var_args> struct Operation {};
template <3 _name, typename _Hndl, 5 _expr_args, 5 _var_args>
struct Statement<Operation<_name, _Hndl, _expr_args, _var_args>> {
  3 name{_name};
  using Hndl = _Hndl;
  5 expr_args{_expr_args};
  5 var_args{_var_args};
};
template <typename> struct is_astnode_Operation : public std::false_type {};
template <3 _name, typename _Hndl, 5 _expr_args, 5 _var_args>
struct is_astnode_Operation<
    Statement<Operation<_name, _Hndl, _expr_args, _var_args>>>
    : public std::true_type {};
template <typename Var, typename Expr> struct Assignment {};
template <typename _Var, typename _Expr>
struct Statement<Assignment<_Var, _Expr>> {
  using Var = _Var;
  using Expr = _Expr;
};
template <typename> struct is_astnode_Assignment : public std::false_type {};
template <typename _Var, typename _Expr>
struct is_astnode_Assignment<Statement<Assignment<_Var, _Expr>>>
    : public std::true_type {};
template <typename Expr> struct Return {};
template <typename _Expr> struct Statement<Return<_Expr>> {
  using Expr = _Expr;
};
template <typename> struct is_astnode_Return : public std::false_type {};
template <typename _Expr>
struct is_astnode_Return<Statement<Return<_Expr>>> : public std::true_type {};
template <typename condition, typename then, typename els> struct If {};
template <typename _condition, typename _then, typename _els>
struct Statement<If<_condition, _then, _els>> {
  using condition = _condition;
  using then = _then;
  using els = _els;
};
template <typename> struct is_astnode_If : public std::false_type {};
template <typename _condition, typename _then, typename _els>
struct is_astnode_If<Statement<If<_condition, _then, _els>>>
    : public std::true_type {};
template <typename condition, typename body> struct While {};
template <typename _condition, typename _body>
struct Statement<While<_condition, _body>> {
  using condition = _condition;
  using body = _body;
};
template <typename> struct is_astnode_While : public std::false_type {};
template <typename _condition, typename _body>
struct is_astnode_While<Statement<While<_condition, _body>>>
    : public std::true_type {};
template <typename e, typename next> struct Sequence {};
template <typename _e, typename _next> struct Statement<Sequence<_e, _next>> {
  using e = _e;
  using next = _next;
};
template <typename> struct is_astnode_Sequence : public std::false_type {};
template <typename _e, typename _next>
struct is_astnode_Sequence<Statement<Sequence<_e, _next>>>
    : public std::true_type {};
struct Skip {};
template <> struct Statement<Skip> {};
template <typename> struct is_astnode_Skip : public std::false_type {};
template <> struct is_astnode_Skip<Statement<Skip>> : public std::true_type {};
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
      } else if constexpr (e.template get_<FieldReference>().is_this_elem) {
        struct arg0 {
#ifndef __clang__
          const AST_elem &e{F{}()};
#endif
          constexpr arg0() {}
          constexpr const AST_elem &operator()() const {
            return e.template get_<FieldReference>().t.Struct.get(allocator);
          }
        };

        using _arg0 = DECT(as_type<budget - 1, arg0>());
        constexpr auto _arg1 = e.template get_<FieldReference>().t.Field;
        return as_types::Expression<as_types::FieldReference<_arg0, _arg1>>{};
      } else if constexpr (e.template get_<FieldPointerReference>()
                               .is_this_elem) {
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

        using _arg0 = DECT(as_type<budget - 1, arg0>());
        constexpr auto _arg1 = e.template get_<FieldPointerReference>().t.Field;
        return as_types::Expression<
            as_types::FieldPointerReference<_arg0, _arg1>>{};
      } else if constexpr (e.template get_<Dereference>().is_this_elem) {
        struct arg0 {
#ifndef __clang__
          const AST_elem &e{F{}()};
#endif
          constexpr arg0() {}
          constexpr const AST_elem &operator()() const {
            return e.template get_<Dereference>().t.Struct.get(allocator);
          }
        };

        using _arg0 = DECT(as_type<budget - 1, arg0>());
        return as_types::Expression<as_types::Dereference<_arg0>>{};
      } else if constexpr (e.template get_<Endorse>().is_this_elem) {
        constexpr auto _arg0 = e.template get_<Endorse>().t.label;
        struct arg1 {
#ifndef __clang__
          const AST_elem &e{F{}()};
#endif
          constexpr arg1() {}
          constexpr const AST_elem &operator()() const {
            return e.template get_<Endorse>().t.Hndl.get(allocator);
          }
        };

        using _arg1 = DECT(as_type<budget - 1, arg1>());
        return as_types::Expression<as_types::Endorse<_arg0, _arg1>>{};
      } else if constexpr (e.template get_<Ensure>().is_this_elem) {
        constexpr auto _arg0 = e.template get_<Ensure>().t.label;
        struct arg1 {
#ifndef __clang__
          const AST_elem &e{F{}()};
#endif
          constexpr arg1() {}
          constexpr const AST_elem &operator()() const {
            return e.template get_<Ensure>().t.Hndl.get(allocator);
          }
        };

        using _arg1 = DECT(as_type<budget - 1, arg1>());
        return as_types::Expression<as_types::Ensure<_arg0, _arg1>>{};
      } else if constexpr (e.template get_<IsValid>().is_this_elem) {
        struct arg0 {
#ifndef __clang__
          const AST_elem &e{F{}()};
#endif
          constexpr arg0() {}
          constexpr const AST_elem &operator()() const {
            return e.template get_<IsValid>().t.Hndl.get(allocator);
          }
        };

        using _arg0 = DECT(as_type<budget - 1, arg0>());
        return as_types::Expression<as_types::IsValid<_arg0>>{};
      } else if constexpr (e.template get_<VarReference>().is_this_elem) {
        constexpr auto _arg0 = e.template get_<VarReference>().t.Var;
        return as_types::Expression<as_types::VarReference<_arg0>>{};
      } else if constexpr (e.template get_<Constant>().is_this_elem) {
        constexpr auto _arg0 = e.template get_<Constant>().t.i;
        return as_types::Expression<as_types::Constant<_arg0>>{};
      } else if constexpr (e.template get_<BinOp>().is_this_elem) {
        constexpr auto _arg0 = e.template get_<BinOp>().t.op;
        struct arg1 {
#ifndef __clang__
          const AST_elem &e{F{}()};
#endif
          constexpr arg1() {}
          constexpr const AST_elem &operator()() const {
            return e.template get_<BinOp>().t.L.get(allocator);
          }
        };

        using _arg1 = DECT(as_type<budget - 1, arg1>());
        struct arg2 {
#ifndef __clang__
          const AST_elem &e{F{}()};
#endif
          constexpr arg2() {}
          constexpr const AST_elem &operator()() const {
            return e.template get_<BinOp>().t.R.get(allocator);
          }
        };

        using _arg2 = DECT(as_type<budget - 1, arg2>());
        return as_types::Expression<as_types::BinOp<_arg0, _arg1, _arg2>>{};
      } else if constexpr (e.template get_<Let>().is_this_elem) {
        struct arg0 {
#ifndef __clang__
          const AST_elem &e{F{}()};
#endif
          constexpr arg0() {}
          constexpr const AST_elem &operator()() const {
            return e.template get_<Let>().t.Binding.get(allocator);
          }
        };

        using _arg0 = DECT(as_type<budget - 1, arg0>());
        struct arg1 {
#ifndef __clang__
          const AST_elem &e{F{}()};
#endif
          constexpr arg1() {}
          constexpr const AST_elem &operator()() const {
            return e.template get_<Let>().t.Body.get(allocator);
          }
        };

        using _arg1 = DECT(as_type<budget - 1, arg1>());
        return as_types::Statement<as_types::Let<_arg0, _arg1>>{};
      } else if constexpr (e.template get_<LetRemote>().is_this_elem) {
        struct arg0 {
#ifndef __clang__
          const AST_elem &e{F{}()};
#endif
          constexpr arg0() {}
          constexpr const AST_elem &operator()() const {
            return e.template get_<LetRemote>().t.Binding.get(allocator);
          }
        };

        using _arg0 = DECT(as_type<budget - 1, arg0>());
        struct arg1 {
#ifndef __clang__
          const AST_elem &e{F{}()};
#endif
          constexpr arg1() {}
          constexpr const AST_elem &operator()() const {
            return e.template get_<LetRemote>().t.Body.get(allocator);
          }
        };

        using _arg1 = DECT(as_type<budget - 1, arg1>());
        return as_types::Statement<as_types::LetRemote<_arg0, _arg1>>{};
      } else if constexpr (e.template get_<operation_args_exprs>()
                               .is_this_elem) {
        constexpr auto _arg0 = e.template get_<operation_args_exprs>().t.exprs;
        return as_types::error<as_types::operation_args_exprs<_arg0>>{};
      } else if constexpr (e.template get_<operation_args_varrefs>()
                               .is_this_elem) {
        constexpr auto _arg0 = e.template get_<operation_args_varrefs>().t.vars;
        return as_types::error<as_types::operation_args_varrefs<_arg0>>{};
      } else if constexpr (e.template get_<Operation>().is_this_elem) {
        constexpr auto _arg0 = e.template get_<Operation>().t.name;
        struct arg1 {
#ifndef __clang__
          const AST_elem &e{F{}()};
#endif
          constexpr arg1() {}
          constexpr const AST_elem &operator()() const {
            return e.template get_<Operation>().t.Hndl.get(allocator);
          }
        };

        using _arg1 = DECT(as_type<budget - 1, arg1>());
        constexpr auto _arg2 = e.template get_<Operation>().t.expr_args;
        constexpr auto _arg3 = e.template get_<Operation>().t.var_args;
        return as_types::Expression<
            as_types::Operation<_arg0, _arg1, _arg2, _arg3>>{};
      } else if constexpr (e.template get_<Operation>().is_this_elem) {
        constexpr auto _arg0 = e.template get_<Operation>().t.name;
        struct arg1 {
#ifndef __clang__
          const AST_elem &e{F{}()};
#endif
          constexpr arg1() {}
          constexpr const AST_elem &operator()() const {
            return e.template get_<Operation>().t.Hndl.get(allocator);
          }
        };

        using _arg1 = DECT(as_type<budget - 1, arg1>());
        constexpr auto _arg2 = e.template get_<Operation>().t.expr_args;
        constexpr auto _arg3 = e.template get_<Operation>().t.var_args;
        return as_types::Statement<
            as_types::Operation<_arg0, _arg1, _arg2, _arg3>>{};
      } else if constexpr (e.template get_<Assignment>().is_this_elem) {
        struct arg0 {
#ifndef __clang__
          const AST_elem &e{F{}()};
#endif
          constexpr arg0() {}
          constexpr const AST_elem &operator()() const {
            return e.template get_<Assignment>().t.Var.get(allocator);
          }
        };

        using _arg0 = DECT(as_type<budget - 1, arg0>());
        struct arg1 {
#ifndef __clang__
          const AST_elem &e{F{}()};
#endif
          constexpr arg1() {}
          constexpr const AST_elem &operator()() const {
            return e.template get_<Assignment>().t.Expr.get(allocator);
          }
        };

        using _arg1 = DECT(as_type<budget - 1, arg1>());
        return as_types::Statement<as_types::Assignment<_arg0, _arg1>>{};
      } else if constexpr (e.template get_<Return>().is_this_elem) {
        struct arg0 {
#ifndef __clang__
          const AST_elem &e{F{}()};
#endif
          constexpr arg0() {}
          constexpr const AST_elem &operator()() const {
            return e.template get_<Return>().t.Expr.get(allocator);
          }
        };

        using _arg0 = DECT(as_type<budget - 1, arg0>());
        return as_types::Statement<as_types::Return<_arg0>>{};
      } else if constexpr (e.template get_<If>().is_this_elem) {
        struct arg0 {
#ifndef __clang__
          const AST_elem &e{F{}()};
#endif
          constexpr arg0() {}
          constexpr const AST_elem &operator()() const {
            return e.template get_<If>().t.condition.get(allocator);
          }
        };

        using _arg0 = DECT(as_type<budget - 1, arg0>());
        struct arg1 {
#ifndef __clang__
          const AST_elem &e{F{}()};
#endif
          constexpr arg1() {}
          constexpr const AST_elem &operator()() const {
            return e.template get_<If>().t.then.get(allocator);
          }
        };

        using _arg1 = DECT(as_type<budget - 1, arg1>());
        struct arg2 {
#ifndef __clang__
          const AST_elem &e{F{}()};
#endif
          constexpr arg2() {}
          constexpr const AST_elem &operator()() const {
            return e.template get_<If>().t.els.get(allocator);
          }
        };

        using _arg2 = DECT(as_type<budget - 1, arg2>());
        return as_types::Statement<as_types::If<_arg0, _arg1, _arg2>>{};
      } else if constexpr (e.template get_<While>().is_this_elem) {
        struct arg0 {
#ifndef __clang__
          const AST_elem &e{F{}()};
#endif
          constexpr arg0() {}
          constexpr const AST_elem &operator()() const {
            return e.template get_<While>().t.condition.get(allocator);
          }
        };

        using _arg0 = DECT(as_type<budget - 1, arg0>());
        struct arg1 {
#ifndef __clang__
          const AST_elem &e{F{}()};
#endif
          constexpr arg1() {}
          constexpr const AST_elem &operator()() const {
            return e.template get_<While>().t.body.get(allocator);
          }
        };

        using _arg1 = DECT(as_type<budget - 1, arg1>());
        return as_types::Statement<as_types::While<_arg0, _arg1>>{};
      } else if constexpr (e.template get_<Sequence>().is_this_elem) {
        struct arg0 {
#ifndef __clang__
          const AST_elem &e{F{}()};
#endif
          constexpr arg0() {}
          constexpr const AST_elem &operator()() const {
            return e.template get_<Sequence>().t.e.get(allocator);
          }
        };

        using _arg0 = DECT(as_type<budget - 1, arg0>());
        struct arg1 {
#ifndef __clang__
          const AST_elem &e{F{}()};
#endif
          constexpr arg1() {}
          constexpr const AST_elem &operator()() const {
            return e.template get_<Sequence>().t.next.get(allocator);
          }
        };

        using _arg1 = DECT(as_type<budget - 1, arg1>());
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
  template <typename e, std::size_t payload>
  constexpr static allocated_ref<AST_elem>
  as_value(AST_Allocator &allocator,
           const Statement<transaction<e, payload>> &) {
    auto elem = allocator.template allocate<AST_elem>();
    auto &this_node =
        elem.get(allocator).template get_<as_values::transaction>();
    this_node.is_this_elem = true;
    elem.get(allocator).is_initialized = true;
    this_node.t.e = as_value(allocator, e{});
    this_node.t.payload = payload;
    return std::move(elem);
  }
  template <typename Struct, 3 Field>
  constexpr static allocated_ref<AST_elem>
  as_value(AST_Allocator &allocator,
           const Expression<FieldReference<Struct, Field>> &) {
    auto elem = allocator.template allocate<AST_elem>();
    auto &this_node =
        elem.get(allocator).template get_<as_values::FieldReference>();
    this_node.is_this_elem = true;
    elem.get(allocator).is_initialized = true;
    this_node.t.Struct = as_value(allocator, Struct{});
    this_node.t.Field = Field;
    return std::move(elem);
  }
  template <typename Struct, 3 Field>
  constexpr static allocated_ref<AST_elem>
  as_value(AST_Allocator &allocator,
           const Expression<FieldPointerReference<Struct, Field>> &) {
    auto elem = allocator.template allocate<AST_elem>();
    auto &this_node =
        elem.get(allocator).template get_<as_values::FieldPointerReference>();
    this_node.is_this_elem = true;
    elem.get(allocator).is_initialized = true;
    this_node.t.Struct = as_value(allocator, Struct{});
    this_node.t.Field = Field;
    return std::move(elem);
  }
  template <typename Struct>
  constexpr static allocated_ref<AST_elem>
  as_value(AST_Allocator &allocator, const Expression<Dereference<Struct>> &) {
    auto elem = allocator.template allocate<AST_elem>();
    auto &this_node =
        elem.get(allocator).template get_<as_values::Dereference>();
    this_node.is_this_elem = true;
    elem.get(allocator).is_initialized = true;
    this_node.t.Struct = as_value(allocator, Struct{});
    return std::move(elem);
  }
  template <4 label, typename Hndl>
  constexpr static allocated_ref<AST_elem>
  as_value(AST_Allocator &allocator, const Expression<Endorse<label, Hndl>> &) {
    auto elem = allocator.template allocate<AST_elem>();
    auto &this_node = elem.get(allocator).template get_<as_values::Endorse>();
    this_node.is_this_elem = true;
    elem.get(allocator).is_initialized = true;
    this_node.t.label = label;
    this_node.t.Hndl = as_value(allocator, Hndl{});
    return std::move(elem);
  }
  template <4 label, typename Hndl>
  constexpr static allocated_ref<AST_elem>
  as_value(AST_Allocator &allocator, const Expression<Ensure<label, Hndl>> &) {
    auto elem = allocator.template allocate<AST_elem>();
    auto &this_node = elem.get(allocator).template get_<as_values::Ensure>();
    this_node.is_this_elem = true;
    elem.get(allocator).is_initialized = true;
    this_node.t.label = label;
    this_node.t.Hndl = as_value(allocator, Hndl{});
    return std::move(elem);
  }
  template <typename Hndl>
  constexpr static allocated_ref<AST_elem>
  as_value(AST_Allocator &allocator, const Expression<IsValid<Hndl>> &) {
    auto elem = allocator.template allocate<AST_elem>();
    auto &this_node = elem.get(allocator).template get_<as_values::IsValid>();
    this_node.is_this_elem = true;
    elem.get(allocator).is_initialized = true;
    this_node.t.Hndl = as_value(allocator, Hndl{});
    return std::move(elem);
  }
  template <3 Var>
  constexpr static allocated_ref<AST_elem>
  as_value(AST_Allocator &allocator, const Expression<VarReference<Var>> &) {
    auto elem = allocator.template allocate<AST_elem>();
    auto &this_node =
        elem.get(allocator).template get_<as_values::VarReference>();
    this_node.is_this_elem = true;
    elem.get(allocator).is_initialized = true;
    this_node.t.Var = Var;
    return std::move(elem);
  }
  template <std::size_t i>
  constexpr static allocated_ref<AST_elem>
  as_value(AST_Allocator &allocator, const Expression<Constant<i>> &) {
    auto elem = allocator.template allocate<AST_elem>();
    auto &this_node = elem.get(allocator).template get_<as_values::Constant>();
    this_node.is_this_elem = true;
    elem.get(allocator).is_initialized = true;
    this_node.t.i = i;
    return std::move(elem);
  }
  template <char op, typename L, typename R>
  constexpr static allocated_ref<AST_elem>
  as_value(AST_Allocator &allocator, const Expression<BinOp<op, L, R>> &) {
    auto elem = allocator.template allocate<AST_elem>();
    auto &this_node = elem.get(allocator).template get_<as_values::BinOp>();
    this_node.is_this_elem = true;
    elem.get(allocator).is_initialized = true;
    this_node.t.op = op;
    this_node.t.L = as_value(allocator, L{});
    this_node.t.R = as_value(allocator, R{});
    return std::move(elem);
  }
  template <typename Binding, typename Body>
  constexpr static allocated_ref<AST_elem>
  as_value(AST_Allocator &allocator, const Statement<Let<Binding, Body>> &) {
    auto elem = allocator.template allocate<AST_elem>();
    auto &this_node = elem.get(allocator).template get_<as_values::Let>();
    this_node.is_this_elem = true;
    elem.get(allocator).is_initialized = true;
    this_node.t.Binding = as_value(allocator, Binding{});
    this_node.t.Body = as_value(allocator, Body{});
    return std::move(elem);
  }
  template <typename Binding, typename Body>
  constexpr static allocated_ref<AST_elem>
  as_value(AST_Allocator &allocator,
           const Statement<LetRemote<Binding, Body>> &) {
    auto elem = allocator.template allocate<AST_elem>();
    auto &this_node = elem.get(allocator).template get_<as_values::LetRemote>();
    this_node.is_this_elem = true;
    elem.get(allocator).is_initialized = true;
    this_node.t.Binding = as_value(allocator, Binding{});
    this_node.t.Body = as_value(allocator, Body{});
    return std::move(elem);
  }
  template <6 exprs>
  constexpr static allocated_ref<AST_elem>
  as_value(AST_Allocator &allocator,
           const error<operation_args_exprs<exprs>> &) {
    auto elem = allocator.template allocate<AST_elem>();
    auto &this_node =
        elem.get(allocator).template get_<as_values::operation_args_exprs>();
    this_node.is_this_elem = true;
    elem.get(allocator).is_initialized = true;
    this_node.t.exprs = exprs;
    return std::move(elem);
  }
  template <6 vars>
  constexpr static allocated_ref<AST_elem>
  as_value(AST_Allocator &allocator,
           const error<operation_args_varrefs<vars>> &) {
    auto elem = allocator.template allocate<AST_elem>();
    auto &this_node =
        elem.get(allocator).template get_<as_values::operation_args_varrefs>();
    this_node.is_this_elem = true;
    elem.get(allocator).is_initialized = true;
    this_node.t.vars = vars;
    return std::move(elem);
  }
  template <3 name, typename Hndl, 5 expr_args, 5 var_args>
  constexpr static allocated_ref<AST_elem>
  as_value(AST_Allocator &allocator,
           const Expression<Operation<name, Hndl, expr_args, var_args>> &) {
    auto elem = allocator.template allocate<AST_elem>();
    auto &this_node = elem.get(allocator).template get_<as_values::Operation>();
    this_node.is_this_elem = true;
    elem.get(allocator).is_initialized = true;
    this_node.t.name = name;
    this_node.t.Hndl = as_value(allocator, Hndl{});
    this_node.t.expr_args = expr_args;
    this_node.t.var_args = var_args;
    return std::move(elem);
  }
  template <3 name, typename Hndl, 5 expr_args, 5 var_args>
  constexpr static allocated_ref<AST_elem>
  as_value(AST_Allocator &allocator,
           const Statement<Operation<name, Hndl, expr_args, var_args>> &) {
    auto elem = allocator.template allocate<AST_elem>();
    auto &this_node = elem.get(allocator).template get_<as_values::Operation>();
    this_node.is_this_elem = true;
    elem.get(allocator).is_initialized = true;
    this_node.t.name = name;
    this_node.t.Hndl = as_value(allocator, Hndl{});
    this_node.t.expr_args = expr_args;
    this_node.t.var_args = var_args;
    return std::move(elem);
  }
  template <typename Var, typename Expr>
  constexpr static allocated_ref<AST_elem>
  as_value(AST_Allocator &allocator, const Statement<Assignment<Var, Expr>> &) {
    auto elem = allocator.template allocate<AST_elem>();
    auto &this_node =
        elem.get(allocator).template get_<as_values::Assignment>();
    this_node.is_this_elem = true;
    elem.get(allocator).is_initialized = true;
    this_node.t.Var = as_value(allocator, Var{});
    this_node.t.Expr = as_value(allocator, Expr{});
    return std::move(elem);
  }
  template <typename Expr>
  constexpr static allocated_ref<AST_elem>
  as_value(AST_Allocator &allocator, const Statement<Return<Expr>> &) {
    auto elem = allocator.template allocate<AST_elem>();
    auto &this_node = elem.get(allocator).template get_<as_values::Return>();
    this_node.is_this_elem = true;
    elem.get(allocator).is_initialized = true;
    this_node.t.Expr = as_value(allocator, Expr{});
    return std::move(elem);
  }
  template <typename condition, typename then, typename els>
  constexpr static allocated_ref<AST_elem>
  as_value(AST_Allocator &allocator,
           const Statement<If<condition, then, els>> &) {
    auto elem = allocator.template allocate<AST_elem>();
    auto &this_node = elem.get(allocator).template get_<as_values::If>();
    this_node.is_this_elem = true;
    elem.get(allocator).is_initialized = true;
    this_node.t.condition = as_value(allocator, condition{});
    this_node.t.then = as_value(allocator, then{});
    this_node.t.els = as_value(allocator, els{});
    return std::move(elem);
  }
  template <typename condition, typename body>
  constexpr static allocated_ref<AST_elem>
  as_value(AST_Allocator &allocator,
           const Statement<While<condition, body>> &) {
    auto elem = allocator.template allocate<AST_elem>();
    auto &this_node = elem.get(allocator).template get_<as_values::While>();
    this_node.is_this_elem = true;
    elem.get(allocator).is_initialized = true;
    this_node.t.condition = as_value(allocator, condition{});
    this_node.t.body = as_value(allocator, body{});
    return std::move(elem);
  }
  template <typename e, typename next>
  constexpr static allocated_ref<AST_elem>
  as_value(AST_Allocator &allocator, const Statement<Sequence<e, next>> &) {
    auto elem = allocator.template allocate<AST_elem>();
    auto &this_node = elem.get(allocator).template get_<as_values::Sequence>();
    this_node.is_this_elem = true;
    elem.get(allocator).is_initialized = true;
    this_node.t.e = as_value(allocator, e{});
    this_node.t.next = as_value(allocator, next{});
    return std::move(elem);
  }
  constexpr static allocated_ref<AST_elem> as_value(AST_Allocator &allocator,
                                                    const Statement<Skip> &) {
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
  as_values::AST_Allocator<budget> head;
  head.top = std::move(
      as_values_ns_fns<as_values::AST_Allocator<budget>>::as_value(head, hd{})
          .get(head)
          .template get<as_values::transaction>());
  return head;
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