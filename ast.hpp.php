<?php

    $Expression_t = 0;
    $Statement_t = 1;
    $Binding_t = 2;

    class Field{
        public $type;
        public $name;
        public $initializer = "{}";
          public function __construct($name,$type,$initializer = "{}") {
            $this->name = $name;
            $this->type = $type;
            $this->initializer = $initializer;
          }
    }
    class AST_node{
        public $name;
        public $type;
        public $fields;
        public function __construct($type_name, $type, $fields) {
          $this->name = $type_name;
          $this->type = $type;
          $this->fields = $fields;
        }
    }
    class Statement extends AST_node {
      public function __construct($type_name, ...$fields) {
        global $Statement_t;
        parent::__construct($type_name, $Statement_t, $fields);
      }
    }
    class Binding extends AST_node {
      public function __construct($type_name, ...$fields) {
        global $Binding_t;
        parent::__construct($type_name, $Binding_t, $fields);
      }
    }
    class Expression extends AST_node {
      public function __construct($type_name, ...$fields) {
        global $Expression_t;
        parent::__construct($type_name, $Expression_t, $fields);
      }
    }

    function is_ast_node($node_t){
      global $Statement_t; global $Expression_t; global $Binding_t;
      return $node_t === $Statement_t || $node_t === $Expression_t || $node_t === $Binding_t;
    }

    function value_type_name($node_t){
      if (is_ast_node($node_t)) return "allocated_ref<AST_elem>";
      else return $node_t;
    }

    function type_name_helper(int $node_t) : string{
      global $Statement_t; global $Expression_t; global $Binding_t;
      switch($node_t) {
        case $Statement_t: return "Statement";
        case $Expression_t: return "Expression";
        case $Binding_t: return "Binding";
        default: return "error";
      }
    }


    function type_type_name($type) : string{
      $out = type_name_helper($type->type);
      $out = $out.'<'.($type->name);
      $field_num = count($type->fields);
      if ($field_num > 0) {
        $out = $out."<";
        foreach ($type->fields as $i => $field){
         $out = $out."$field->name";
         if ($i+1 != $field_num){
           $out = $out.",";
         }
        }
        $out = $out.">";
     }
     return $out.">";
    }

    $types = array(new Statement('transaction',new Field('e',$Statement_t),new Field('payload','std::size_t','{0}')),
    new Statement('sequence',new Field('e',$Statement_t),new Field('next',$Statement_t)),
    new Expression('plus', new Field('l',$Expression_t),new Field('r',$Expression_t)),
    new Expression('number',new Field('num','std::size_t','{0}')),
    new Statement('return_val', new Field('v',$Expression_t)),
    new Statement('skip')
   );
?>

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

#define generate_value1(name) struct name {constexpr name(){}}

#define generate_value_IMPL2(count, ...) generate_value ## count (__VA_ARGS__)
#define generate_value_IMPL(count, ...) generate_value_IMPL2(count, __VA_ARGS__)
#define generate_value(...) generate_value_IMPL(VA_NARGS(__VA_ARGS__), __VA_ARGS__)

<?php function comma_separated($arr){
  $target = count($arr) - 1;
  foreach ($arr as $i => $e){
    echo "$e";
    if ($i != $target) echo ",";
  }
}

  function name($e){
    return $e->name;
  }
  function names($arr){
    return array_map("name",$arr);
  }
?>

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
  echo "generate_value($type->name";
  foreach ($type->fields as $field){
    $fieldtype = value_type_name($field->type);
    echo ", $fieldtype, $field->name, $field->initializer";
  }
  echo ");\n";
}
?>
} // namespace as_values

<?php
 function template_defn($type){
   $out = '';
  $field_num = count($type->fields);
  if ($field_num > 0) {
    $out = $out."template <";
    foreach ($type->fields as $i => $field){
     if (is_ast_node($field->type)){
       $out = $out."typename ";
     }
     else {$out = $out. "$field->type ";}
     $out = $out. "$field->name";
     if ($i+1 != $field_num){
       $out = $out. ",";
     }
    }
    $out = $out. "> ";
 }
 return $out;
}

function full_template_defn($type){
  return template_defn($type);
}

?>

namespace as_types {
  template<typename> struct Expression;
  template<typename> struct Statement;
  template<typename,typename> struct Binding;
<?php foreach ($types as $type){
     echo template_defn($type);
     echo "struct $type->name{};\n";
    $tmp = full_template_defn($type);
    if ($tmp === '') $tmp = 'template<>';
    echo "$tmp struct ".type_type_name($type)."{};\n";
  }
?>

} // namespace as_types

namespace as_values {
template <typename prev_holder>
          struct as_type_f{
static constexpr const DECT(prev_holder::prev.allocator) &allocator{prev_holder::prev.allocator};
template<std::size_t budget, typename F>
constexpr static auto as_type() {
  static_assert(budget > 0);
  if (budget > 0) {
    constexpr const AST_elem &e = F{}();
    <?php foreach ($types as $type){
      echo "if constexpr (e.template get_<$type->name>().is_this_elem) {\n";
      foreach ($type->fields as $i => $field){
        if (is_ast_node($field->type)) {
        echo "struct arg$i {
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
  }
  static_assert(budget > 0);
  struct error {};
  return error{};
}
};

template <typename prev_holder> constexpr auto as_type() {
  struct arg {
    constexpr arg() {}
    constexpr const AST_elem &operator()() const {
      return prev_holder::prev.allocator.top.e.get(prev_holder::prev.allocator);
    }
  };
  return as_type_f<prev_holder>::template as_type<15,arg>();
}
} // namespace as_values

namespace as_types{

template<typename AST_Allocator>
struct as_values_ns_fns{
  <?php 
  foreach ($types as $type){
    echo full_template_defn($type);
    $decl = type_type_name($type);
  echo "constexpr static void as_value(AST_Allocator& allocator, const $decl &){
    auto elem = allocator.template allocate<AST_elem>();
    auto &this_node = elem.template get_<as_values::$type->name>();";
    foreach ($type->fields as $field){
      this_node.
      echo "$field->name";
    }
  echo "}\n";
  }
  ?>
};

  template<std::size_t budget, typename hd>
  constexpr as_values::AST_Allocator<budget> as_value(){
    as_values::AST_Allocator<budget> head;
    return as_values_ns_fns<as_values::AST_Allocator<budget>>::as_value(head,hd{});
  }
}