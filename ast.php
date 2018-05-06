<?php
require_once 'util.php';

$Expression_t = 0;
$Statement_t = 1;
$Binding_t = 2;
$String_t = 3;
$Label_t = 4;
$Argument_pack_t = 5;
$List_t = 6;
$NODEMAX = 7;

class Field{
        public $type;
        public $name;
        public $initializer = "{}";
        public function __construct($name,$type,$initializer = "{}") {
            $this->name = $name;
            $this->type = $type;
            $this->initializer = $initializer;
          }

        public function value_type_name(){
            if (is_ast_node($this->type)) return "allocated_ref<AST_elem>";
            else return $this->type;
        }
        public function declare_struct_member(){
            return $this->value_type_name()." $this->name $this->initializer;";
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

        public function template_defn($prefix = ''){
          $type = $this;
          $out = '';
         $field_num = count($type->fields);
         if ($field_num > 0) {
           $out = $out."template <";
           foreach ($type->fields as $i => $field){
            if (is_ast_node($field->type)){
              $out = $out."typename ";
            }
            else {$out = $out. "$field->type ";}
            $out = $out. $prefix."$field->name";
            if ($i+1 != $field_num){
              $out = $out. ",";
            }
           }
           $out = $out. "> ";
        }
        return $out;
       }
       public function encapsulated_type_name($prefix = '') : string{
         $type = $this;
        global $Argument_pack_t;
        assert ($type->type != $Argument_pack_t);
        $out = type_name_helper($type->type);
        $out = $out.'<'.($type->name);
        $field_num = count($type->fields);
        if ($field_num > 0) {
          $out = $out."<";
          foreach ($type->fields as $i => $field){
           $out = $out.$prefix."$field->name";
           if ($i+1 != $field_num){
             $out = $out.",";
           }
          }
          $out = $out.">";
       }
       return $out.">";
      }

      public function define_type() : string{
          return $this->template_defn() .
          "struct $this->name{};\n";
      }

      public function encapsulate_type() : string {
        $tmp = $this->full_template_defn('_');
        $out = "$tmp struct ".$this->encapsulated_type_name('_')."{";
        foreach ($this->fields as $field){
          if (is_ast_node($field->type)){
            $out = $out."using $field->name = _$field->name;";
          }
          else {
            $out = $out."$field->type $field->name{_$field->name};";
          }
        }
        $out = $out."};\n";
        return $out;
      }
   
      public function value_declaration(){
          return "struct $this->name;";
      }

      public function full_template_defn($prefix = ''){
        return $this->template_defn($prefix);
      }

      public function is_astnode_defn(){
          $type = $this;
        return "template<typename> struct is_astnode_$type->name : public std::false_type{};".
        $type->full_template_defn()." struct is_astnode_$type->name<".$type->encapsulated_type_name()."> : public std::true_type{};";
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
    class Argument_pack extends AST_node {
      public function __construct($type_name,$field_name) {
        global $Expression_t; global $Argument_pack_t; global $List_t;
        parent::__construct($type_name, $Argument_pack_t, array(new Field($field_name,$List_t)));
      }
      public function encapsulated_type_name($prefix = '') : string{
        return $this->name;
      }
    }

    class Skip extends AST_node {
        public function __construct($type_name) {
            global $Statement_t;
            parent::__construct($type_name, $Statement_t, array());
          }

          public function template_defn($prefix = ''){
              return '';
          }

          public function full_template_defn($prefix = ''){
              return 'template<>';
          }
    }

    function is_ast_node($node_t){
      global $Statement_t; global $Expression_t; global $Binding_t;
      global $String_t; global $Label_t; global $Argument_pack_t;
      global $List_t; global $NODEMAX;
      if (is_int($node_t)){
        return $node_t >= $Expression_t && $node_t <= $NODEMAX;
      }
    }

    function type_name_helper(int $node_t) : string{
      global $Statement_t; global $Expression_t; global $Binding_t;
      global $String_t; global $Label_t; global $Argument_pack_t;
      global $List_t;
      switch($node_t) {
        case $Statement_t: return "Statement";
        case $Expression_t: return "Expression";
        case $Binding_t: return "Binding";
        case $String_t: return '$String__tnht';
        case $Label_t: return '$Label_t_tnh';
        case $Argument_pack_t: return '$Argument_pack_t_tnh';
        case $List_t: return '$list_t_tnh';
        default: return "error";
      }
    }
?>