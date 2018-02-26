#pragma once
#include "ast.hpp"
#include <iostream>

namespace ast{
	struct noop_visitor : public Visitor<noop_visitor> {
		
		using super = Visitor<noop_visitor>;
		using super::visit;
		using super::visit_children;
		
		template<typename Children, std::size_t size>
		constexpr auto visit(const transaction&, Children (&rest)[size]){
			std::cout << "oh boy" << std::endl;
			return visit_children(*this,rest);
		}
		
		template<typename T, typename Children, std::size_t size>
		constexpr auto visit(T& , Children (&rest)[size]){
			//std::cout << "default case" << std::endl;
			return visit_children(*this,rest);
		}

		template<std::size_t d, std::size_t w, typename... o>
		constexpr auto visit(tree<d,w,o...> &t){
			return visit(*this, t);
		}
	};
}
