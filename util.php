<?php 

assert_options(ASSERT_ACTIVE, 1);
assert_options(ASSERT_WARNING, 1);
assert_options(ASSERT_QUIET_EVAL, 1);

    function comma_separated($arr){
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