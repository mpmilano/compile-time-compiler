#pragma once
#include "ast.hpp"
#include <iostream>
#include <typeinfo>



namespace ast{
	struct noop_visitor : public Visitor<noop_visitor> {
		
		using super = Visitor<noop_visitor>;
		using super::visit;
		using super::visit_children;
		
		template<typename Parent, typename Children, std::size_t size>
		constexpr auto visit(const _for_each<Parent>&, Children (&rest)[size]){
			visit_children(*this,rest);
			return err_result("testing");
		}
		
		IMPLEMENTED_CASES(for_each);
	};
}
