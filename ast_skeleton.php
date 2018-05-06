
#pragma once
#include "allocator.hpp"
#include "mutils/type_utils.hpp"
#include "union.hpp"
#include <ostream>

namespace as_values {
  <?php 
  foreach ($types as $type){
    echo "struct $type->name;\n";
  } ?>
using AST_elem = Union<<?php comma_separated(names($types))?>>;
template <std::size_t budget>
using AST_Allocator = Allocator<budget, <?php echo $types[0]->name;?>, AST_elem>;

<?php
foreach ($types as $type){
  echo "struct $type->name { ";
    foreach ($type->fields as $field){
      $fieldtype = value_type_name($field->type);
      echo "$fieldtype $field->name $field->initializer;";
    }
    echo "
    constexpr $type->name(){}; 
    constexpr $type->name($type->name &&p) ";
    foreach ($type->fields as $i => $field){
      if ($i === 0) echo ':';
      echo "$field->name{std::move(p.$field->name)}";
      if ($i +1 < count($type->fields)) echo ",";
    }
    echo '{}';
    echo "constexpr $type->name &operator=($type->name &&p) {"; 
      foreach ($type->fields as $field){
        echo "$field->name = std::move(p.$field->name);"; 
      }
      echo "return *this; 
    } 
  };";
}
?>
} // namespace as_values

namespace as_types {
  template<typename> struct Expression;
  template<typename> struct Statement;
  template<typename,typename> struct Binding;
  
<?php foreach ($types as $type){
     echo $type->template_defn();
     echo "struct $type->name{};\n";
    $tmp = $type->full_template_defn('_');
    if ($tmp === '') $tmp = 'template<>';
    echo "$tmp struct ".$type->type_type_name('_')."{";
    foreach ($type->fields as $field){
      if (is_ast_node($field->type)){
        echo "using $field->name = _$field->name;";
      }
      else {
        echo "$field->type $field->name{_$field->name};";
      }
    }
    echo "};\n";
    echo "template<typename> struct is_astnode_$type->name : public std::false_type{};";
    echo "$tmp struct is_astnode_$type->name<".$type->type_type_name("_")."> : public std::true_type{};";
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
      foreach ($type->fields as $i => $field){
        if (is_ast_node($field->type)) {
        echo "struct arg$i {
#ifndef __clang__
          const AST_elem &e{F{}()};
#endif
          constexpr arg$i() {}
          constexpr const AST_elem &operator()() const {
            return e.template get_<$type->name>().t.$field->name.get(allocator);
          }
        };
        
        ";
          echo "using _arg$i = DECT(as_type<budget - 1, arg$i>());\n";
        }
        else echo "constexpr auto _arg$i = e.template get_<$type->name>().t.$field->name;\n";
      }
    echo "return as_types::".type_name_helper($type->type)."<as_types::$type->name";
    $field_count = count($type->fields);
    if ($field_count > 0) echo "<";
    foreach ($type->fields as $i => $field){
      echo "_arg$i";
      if ($i + 1 != $field_count){
        echo ",";
      }
    }
    if ($field_count > 0) echo ">";
    echo '>';
    echo "{};}\n";
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
    $decl = $type->type_type_name();
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