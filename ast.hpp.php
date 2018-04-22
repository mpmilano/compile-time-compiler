<?php
    class Field{
        public $type = "allocated_ref<AST_elem>";
        public $name;
        public $initializer = "{}";
          public function __construct($name,$type = "allocated_ref<AST_elem>",$initializer = "{}") {
            $this->name = $name;
            $this->type = $type;
            $this->initializer = $initializer;
          }
    }
    class AST_node{
        public $name;
        public $fields;
        public function __construct($type_name, ...$fields) {
          $this->name = $type_name;
          $this->fields = $fields;
        }
    }
    $types = array(new AST_node('transaction',new Field('e'),new Field('payload','std::size_t','{0}')),
    new AST_node('sequence',new Field('e'),new Field('next')),
    new AST_node('plus', new Field('l'),new Field('r')),
    new AST_node('number',new Field('num','std::size_t','{0}')),
    new AST_node('return_val', new Field('v')),
    new AST_node('skip')
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

#define generate_internal_value3(name,f1,f2) \
generate_value7(name,allocated_ref<AST_elem>,f1,{},allocated_ref<AST_elem>,f2,{})

#define generate_internal_value2(name,f1) \
generate_value4(name,allocated_ref<AST_elem>,f1,{})

#define generate_value_IMPL2(count, ...) generate_value ## count (__VA_ARGS__)
#define generate_value_IMPL(count, ...) generate_value_IMPL2(count, __VA_ARGS__)
#define generate_value(...) generate_value_IMPL(VA_NARGS(__VA_ARGS__), __VA_ARGS__)

#define generate_internal_value_IMPL2(count, ...) generate_internal_value ## count (__VA_ARGS__)
#define generate_internal_value_IMPL(count, ...) generate_internal_value_IMPL2(count, __VA_ARGS__)
#define generate_internal_value(...) generate_internal_value_IMPL(VA_NARGS(__VA_ARGS__), __VA_ARGS__)


#define convert3(name,l,r)\
 if constexpr (e.template get_<name>().is_this_elem) {\
      struct arg1 {\
        constexpr arg1() {}\
        constexpr const AST_elem &operator()() const {\
          return e.template get_<name>().t.l.get(allocator);\
        }\
      };\
      struct arg2 {\
        constexpr arg2() {}\
        constexpr const AST_elem &operator()() const {\
          return e.template get_<name>().t.r.get(allocator);\
        }\
      };\
      using left = DECT(as_type<budget - 1, arg1>(allocator));\
      using right = DECT(as_type<budget - 1, arg2>(allocator));\
      return as_types::name<left, right>{};}

#define convert2(name,l)\
 if constexpr (e.template get_<name>().is_this_elem) {\
      struct arg1 {\
        constexpr arg1() {}\
        constexpr const AST_elem &operator()() const {\
          return e.template get_<name>().t.l.get(allocator);\
        }\
      };\
      using left = DECT(as_type<budget - 1, arg1>(allocator));\
      return as_types::name<left>{};}

#define convert1(name)\
if constexpr (e.template get_<name>().is_this_elem) {\
      return as_types::name{};}

#define convert_IMPL2(count, ...) convert ## count (__VA_ARGS__)
#define convert_IMPL(count, ...) convert_IMPL2(count, __VA_ARGS__)
#define convert(...) convert_IMPL(VA_NARGS(__VA_ARGS__), __VA_ARGS__)

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
    echo ", $field->type, $field->name, $field->initializer";
  }
  echo ");\n";
}
?>
} // namespace as_values

namespace as_types {
<?php foreach ($types as $type){
  $field_num = count($type->fields);
  if ($field_num > 0) {
    echo "template <";
    foreach ($type->fields as $i => $field){
     if ($field->type == "allocated_ref<AST_elem>"){
       echo "typename ";
     }
     else {echo "$field->type ";}
     echo "$field->name";
     if ($i+1 != $field_num){
       echo ",";
     }
    }
    echo "> ";
  }
  echo "struct $type->name{};\n";
}?>

} // namespace as_types

namespace as_values {
template <std::size_t budget, typename F, std::size_t alloc_budget,
          typename... allocates>
constexpr auto as_type(const Allocator<alloc_budget, allocates...> &allocator) {
  static_assert(budget > 0);
  if (budget > 0) {
    constexpr const AST_elem &e = F{}();
    <?php foreach ($types as $type){
      echo "if constexpr (e.template get_<$type->name>().is_this_elem) {\n";
      foreach ($type->fields as $i => $field){
        if ($field->type === "allocated_ref<AST_elem>") {
        echo "struct arg$i {
          constexpr arg$i() {}
          constexpr const AST_elem &operator()() const {
            return e.template get_<$type->name>().t.$field->name.get(allocator);
          }
        };
        
        ";
          echo "using _arg$i = DECT(as_type<budget - 1, arg$i>(allocator));\n";
        }
        else echo "constexpr auto _arg$i = e.template get_<$type->name>().t.$field->name;\n";
      }
    echo "return as_types::$type->name";
    $field_count = count($type->fields);
    if ($field_count > 0) echo "<";
    foreach ($type->fields as $i => $field){
      echo "_arg$i";
      if ($i + 1 != $field_count){
        echo ",";
      }
    }
    if ($field_count > 0) echo ">";
    echo "{};}\n";
  }?>
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