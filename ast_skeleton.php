#pragma once
#include "allocator.hpp"
#include "mutils/type_utils.hpp"
#include "union.hpp"
#include <ostream>

template<typename T>
using plain_array = T[<?php echo $max_var_length?>];

namespace as_values {
  <?php 
  foreach ($types as $type){
    echo $type->value_declaration();
  } ?>
using AST_elem = Union<<?php comma_separated(names($types))?>>;
template <std::size_t budget>
using AST_Allocator = Allocator<budget, <?php echo $types[0]->name;?>, AST_elem>;

// Define structs. 
struct Expression{constexpr Expression(){}};
struct Statement{constexpr Statement(){}};
struct Binding{constexpr Binding(){}};
<?php
foreach ($types as $type){
  echo $type->struct_declaration();
}
?>
} // namespace as_values

namespace as_types {
  template<typename> struct Expression;
  template<typename> struct Statement;
  template<typename,typename> struct Binding;
  
<?php foreach ($types as $type){
     echo $type->define_type();
    echo $type->encapsulate_type();
  }
?>

} // namespace as_types

namespace as_values {
template <typename prev_holder>
          struct as_type_f{
static constexpr const DECT(prev_holder::prev.allocator) &allocator{prev_holder::prev.allocator};
template<long budget, typename F>
constexpr static auto as_type(std::enable_if_t<(budget > 0) && (budget < 50)>* = nullptr) {
  static_assert(budget > 0);
  if constexpr (budget > 0) {
    constexpr const AST_elem &e = F{}();
    <?php foreach ($types as $i => $type){
      if ($i > 0) echo 'else ';
      echo "if constexpr (e.template get_<$type->name>().is_this_elem) {\n";
      echo $type->to_type_body();
    echo "}\n";
  }?>
  else {struct error {};
    return error{};}
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
  return as_types::Statement<as_types::transaction<DECT(as_type_f<prev_holder>::template as_type<15,arg>()),prev_holder::prev.allocator.top.payload>>{};
}
} // namespace as_values

namespace as_types{

template<typename AST_Allocator>
struct as_values_ns_fns{
  using AST_elem = as_values::AST_elem;
  <?php 
  foreach ($types as $type){
    echo $type->full_template_defn();
    $decl = $type->encapsulated_type_name();
  echo "constexpr static allocated_ref<AST_elem> as_value(AST_Allocator& allocator, const $decl &){
    auto elem = allocator.template allocate<AST_elem>();
    auto &this_node = elem.get(allocator).template get_<as_values::$type->name>();
    this_node.is_this_elem = true;
    elem.get(allocator).is_initialized = true;";
    foreach ($type->fields as $field){
      if (is_ast_node($field->type))
        echo "this_node.t.$field->name = as_value(allocator, $field->name{});";
      else echo "this_node.t.$field->name = $field->name;";
    }
    echo 'return std::move(elem);';
  echo "}\n";
  }
  ?>
};

  template<std::size_t budget, typename hd>
  constexpr as_values::AST_Allocator<budget> as_value(){
    static_assert(is_astnode_transaction<hd>::value);
    as_values::AST_Allocator<budget> head;
    head.top = std::move(as_values_ns_fns<as_values::AST_Allocator<budget>>::as_value(head,hd{}).get(head).template get<as_values::transaction>());
    return head;
  }
}

namespace as_values {
  template<typename Allocator>
  std::ostream& print(std::ostream& o, const std::size_t& st, const Allocator &){
    return o << st;
  }
  template<typename Allocator>
  std::ostream& print(std::ostream& o, const AST_elem& e, const Allocator &allocator);
  <?php 
  foreach ($types as $type){
  echo "
  template<typename Allocator>
  std::ostream& print(std::ostream& o, const $type->name& e, const Allocator& allocator){
    o << \"$type->name{\";";
    foreach ($type->fields as $field){
      echo "print(o,e.$field->name,allocator);
      o << \",\";";
    }
  echo "
    return o << \"}\";
    }";
  }
  ?>
  template<typename Allocator>
  std::ostream& print(std::ostream& o, const AST_elem& e, const Allocator &allocator){
    <?php 
    foreach ($types as $type){
      echo "if (e.template get_<$type->name>().is_this_elem) {
          return print(o,e.template get<$type->name>(),allocator);
      }";
    }
    ?>
    return o;
  }
}

namespace as_types{
    <?php
    foreach ($types as $type){
        echo $type->is_astnode_defn();
    }
    ?>
}