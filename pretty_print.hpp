#include "ast.hpp"

namespace as_values {
/*
 */

template <typename Allocator>
std::ostream &pretty_print(std::ostream &o, const FieldReference &e,
                           const Allocator &allocator) {
  pretty_print(o, e.Struct, allocator);
  o << ".";
  pretty_print(o, e.Field, allocator);
  return o;
}

template <typename Allocator>
std::ostream &pretty_print(std::ostream &o, const transaction &e,
                           const Allocator &allocator) {
  o << "Transaction {";
  pretty_print(o, e.e, allocator);
  return o << " }";
}
template <typename Allocator>
std::ostream &pretty_print(std::ostream &o, const FieldPointerReference &e,
                           const Allocator &allocator) {
  pretty_print(o, e.Struct, allocator);
  o << "->";
  pretty_print(o, e.Field, allocator);
  return o;
}
template <typename Allocator>
std::ostream &pretty_print(std::ostream &o, const Dereference &e,
                           const Allocator &allocator) {
  o << "*";
  pretty_print(o, e.Struct, allocator);
  return o;
}
template <typename Allocator>
std::ostream &pretty_print(std::ostream &o, const IsValid &e,
                           const Allocator &allocator) {
  pretty_print(o, e.Hndl, allocator);
  return o << ".isValid()";
}
template <typename Allocator>
std::ostream &pretty_print(std::ostream &o, const VarReference &e,
                           const Allocator &allocator) {
  return o << e.Var;
}
template <typename Allocator>
std::ostream &pretty_print(std::ostream &o, const Constant &e,
                           const Allocator &allocator) {
  return o << e.i;
}
template <typename Allocator>
std::ostream &pretty_print(std::ostream &o, const BinOp &e,
                           const Allocator &allocator) {
  pretty_print(o, e.L, allocator);
  char op[2] = {e.op, 0};
  o << op;
  pretty_print(o, e.R, allocator);
  return o;
}
template <typename Allocator>
std::ostream &pretty_print(std::ostream &o, const Let &e,
                           const Allocator &allocator) {
  o << "var ";
  pretty_print(o, e.Binding, allocator);
  o << "; ";
  pretty_print(o, e.Body, allocator);
  return o;
}
template <typename Allocator>
std::ostream &pretty_print(std::ostream &o, const LetRemote &e,
                           const Allocator &allocator) {
  o << "remote ";
  pretty_print(o, e.Binding, allocator);
  o << "; ";
  pretty_print(o, e.Body, allocator);
  return o;
}
template <typename Allocator>
std::ostream &pretty_print(std::ostream &o, const Assignment &e,
                           const Allocator &allocator) {
  pretty_print(o, e.Var, allocator);
  o << " = ";
  pretty_print(o, e.Expr, allocator);
  return o;
}
template <typename Allocator>
std::ostream &pretty_print(std::ostream &o, const Return &e,
                           const Allocator &allocator) {
  o << "return ";
  pretty_print(o, e.Expr, allocator);
  return o;
}
template <typename Allocator>
std::ostream &pretty_print(std::ostream &o, const If &e,
                           const Allocator &allocator) {
  o << "if (";
  pretty_print(o, e.condition, allocator);
  o << ") {";
  pretty_print(o, e.then, allocator);
  o << "} else {";
  pretty_print(o, e.els, allocator);
  return o << "}";
}
template <typename Allocator>
std::ostream &pretty_print(std::ostream &o, const While &e,
                           const Allocator &allocator) {
  o << "while (";
  pretty_print(o, e.condition, allocator);
  o << ") {";
  pretty_print(o, e.body, allocator);
  return o << "}";
}
template <typename Allocator>
std::ostream &pretty_print(std::ostream &o, const Sequence &e,
                           const Allocator &allocator) {
  pretty_print(o, e.e, allocator);
  o << "; ";
  pretty_print(o, e.next, allocator);
  return o;
}
template <typename Allocator>
std::ostream &pretty_print(std::ostream &o, const Skip &, const Allocator &) {
  return o;
}

} // namespace as_values