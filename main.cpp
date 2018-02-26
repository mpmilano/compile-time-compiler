#include "ctutils.hpp"
#include "ast.hpp"
#include "test_ast.hpp"
#include <iostream>

constexpr auto visit_test(ast::top_ast a){
	return ast::noop_visitor{}.visit(a);
}

int main(){

	constexpr test_union tu{3.4};
	std::cout << tu.template get<double>() + tu.template get<2>()<< std::endl;

	Option<int> o;
	assert(o.internal.which() == 0);
	o = 3;
	assert(o.internal.which() == 1);

	constexpr auto result{visit_test(
			ast::top_ast{ast::plus{}}
			.add_child(
				ast::transaction{})
			.add_child(ast::for_each{}))};
	std::cout << result << std::endl;
}
