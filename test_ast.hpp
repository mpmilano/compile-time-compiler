#pragma once
#include "visitor.hpp"
#include "ast.hpp"
#include <iostream>
#include <typeinfo>



namespace ast{
	struct noop_visitor : public Visitor<noop_visitor> {
		
		using super = Visitor<noop_visitor>;
		using super::visit;
		using super::visit_children;
		
		template<typename Parent, typename Children, std::size_t size, typename Ac>
		constexpr Ac* visit(const _skip<Parent>&, Children (&rest)[size],Ac* a){
			auto r = super::visit_children(*this,rest,a);
			std::cout << "setting data" << std::endl;
			(r)->data[0] = 'c';
			return r;
		}

		template<typename Parent, typename Children, std::size_t size, typename Ac>
		constexpr Ac* visit(const _plus<Parent>&, Children (&rest)[size],Ac* a){
			auto r = super::visit_children(*this,rest,a);
			std::cout << "setting data" << std::endl;
			(r)->data[0] = 'c';
			return r;
		}
		
		IMPLEMENTED_CASES(skip,plus);
	};
}
