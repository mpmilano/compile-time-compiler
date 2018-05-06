<?php 
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