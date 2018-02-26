#pragma once
#include "ctutils.hpp"
#include "tree.hpp"


namespace ast {

	template<typename Subtree>
	struct _transaction {
		Subtree child;
		constexpr _transaction(){}
		template<std::size_t size>
		constexpr auto provide_children(Subtree*(&)[size]){
		}
	};

	template<typename Subtree>
	struct _plus {
		constexpr _plus(){}
		template<std::size_t size>
		constexpr auto provide_children(Subtree*(&)[size]){
		}
	};
	
	template<typename Subtree>
	struct _for_each {
		constexpr _for_each(){}
		template<std::size_t size>
		constexpr auto provide_children(Subtree*(&)[size]){
		}
	};

	using maxval = std::integral_constant<std::size_t,3>;
	template<std::size_t value>
	using ast = tree<value,maxval::value,_transaction,_plus,_for_each>;
	using top_ast = ast<maxval::value>;

	using for_each = _for_each<top_ast>;
	using plus = _plus<top_ast>;
	using transaction = _transaction<top_ast>;
	

	
}

