#include "ctutils.hpp"
#include "ast.hpp"
#include "test_ast.hpp"
#include <iostream>

constexpr ast::top_ast visit_test(ast::top_ast a){
	ast::noop_visitor{}.visit(a);
	return a;
}

int main(){

	constexpr test_union tu{3.4};
	std::cout << tu.template get<double>() + tu.template get<2>()<< std::endl;


	constexpr ast::top_ast a{visit_test(ast::transaction{})};
	
}
