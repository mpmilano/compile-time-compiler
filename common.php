<?php
  function alloc(string $elem, string $objname, string $type){
    echo "
    allocated_ref<as_values::AST_elem> $elem = allocator.template allocate<as_values::AST_elem>();
    $elem.get(allocator).template get_<as_values::$type>().is_this_elem = true;
    auto& $objname = $elem.get(allocator).template get_<as_values::$type>().t;";
  }
?>

