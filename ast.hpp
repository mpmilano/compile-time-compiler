#pragma once
#include "ctutils.hpp"
#include "tree.hpp"


namespace ast {

	template<typename Subtree>
	struct _transaction {
		Subtree child;
		template<typename Child>
		constexpr _transaction(Child c):child(c){}

		constexpr _transaction(){}
		template<std::size_t size>
		constexpr auto provide_children(Subtree*(&arr)[size]){
			arr[0] = &child;
		}
	};

	template<typename Subtree>
	struct _plus {
		Subtree left;
		Subtree right;
		template<typename Left, typename Right>
		constexpr _plus(Left l, Right r):left(l),right(r){}

		constexpr _plus(){}
		template<std::size_t size>
		constexpr auto provide_children(Subtree*(&arr)[size]){
			arr[0] = &left;
			arr[1] = &right;
		}

		template<std::size_t d, std::size_t w, template<typename> class... o>
		constexpr auto resize(tree<d,w,o...> const * const) const {
			return _plus<tree<d,w,o...> >{left,right /* need to map resize these*/};
		}
	};
	
	template<typename Subtree>
	struct _for_each {
		Subtree var;
		Subtree collection;
		Subtree body;
		template<typename Var, typename Collection, typename Body>
		constexpr _for_each(Var var, Collection collection, Body body)
			:var(Subtree::resize(var)),
			 collection(Subtree::resize(collection)),
			 body(Subtree::resize(body)){}

			constexpr _for_each(){}
		template<std::size_t size>
		constexpr auto provide_children(Subtree*(&arr)[size]){
			arr[0] = &var;
			arr[1] = &collection;
			arr[2] = &body;
		}
	};

	template<typename Subtree> struct _skip {
		constexpr _skip(){}
		template<std::size_t size>
		constexpr auto provide_children(Subtree*(&)[size]){}

		template<std::size_t d, std::size_t w, template<typename> class... o>
		constexpr auto resize(tree<d,w,o...> const * const) const {
			return _skip<tree<d,w,o...> >{};
		}
	};

	using maxval = std::integral_constant<std::size_t,3>;
	template<std::size_t value>
	using ast = tree<value,maxval::value,_transaction,_plus,_for_each,_skip>;
	using top_ast = ast<maxval::value>;

	using for_each = _for_each<top_ast>;
	using plus = _plus<top_ast>;
	using transaction = _transaction<top_ast>;
	using skip = _skip<top_ast>;
	

	
}

