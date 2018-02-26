#include "ctutils.hpp"
#include "ast.hpp"
#include "test_ast.hpp"
#include <iostream>

int main(){

	constexpr test_union tu{3.4};
	std::cout << tu.template get<double>() + tu.template get<2>()<< std::endl;


	ast::top_ast a{ast::transaction{}};
	ast::noop_visitor{}.visit(a);
	
}
