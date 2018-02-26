#include "ctutils.hpp"
#include "ast.hpp"
#include "test_ast.hpp"
#include <iostream>

constexpr auto visit_test(ast::top_ast a){
	string s[1] = {string{}};
	return *ast::noop_visitor{}.visit(a,s);
}

int main(){

	constexpr test_union tu{3.4};
	std::cout << tu.template get<double>() + tu.template get<2>()<< std::endl;

	Option<int> o;
	//assert(o.internal.which() == 0);
	o = 3;
	//assert(o.internal.which() == 1);

	using namespace ast;
	auto result{visit_test(
			top_ast{plus{skip{},skip{}}})};
	std::cout << result.data << std::endl;
}
