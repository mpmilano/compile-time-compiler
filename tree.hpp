#pragma once
#include "ctutils.hpp"

template<std::size_t depth_max, std::size_t child_max, template<typename> class... options>
struct tree;

template<std::size_t child_max, template<typename> class... options>
struct tree<0,child_max, options...>{

	template<typename... T>
	constexpr tree(const T&...){}
	
	template<typename F, typename Accum>
	constexpr Accum* fold(F &&, Accum *r){return r;}
};

template<std::size_t depth_max,
				 std::size_t child_max,
				 /* We fill in the parent for each node directly.*/
				 template<typename> class... options>
struct tree{
	using child_tree = tree<depth_max - 1, child_max, options...>;
	Union<options<child_tree>...> _this;
	using child_tree_array = child_tree*[child_max];
	child_tree_array children = {nullptr};

	struct provide_children{
		constexpr provide_children(){}
		template<typename N>
		constexpr auto* operator()(N& n, tree* t){
			n.provide_children(t->children);
			return t;
		}
	};
	
	template<typename Arg>
	constexpr tree(Arg a):_this{a}{
		_this.fold(provide_children{},this);
	}

	constexpr tree():
		_this{}{
		_this.fold(provide_children{},this);
	}

	template<typename F>
	struct fold_specialize{
		F& f;
		tree& closure;
		constexpr fold_specialize(F& f, tree& c):f(f),closure(c){}
		template<typename Arg, typename Accum>
		constexpr Accum* operator()(Arg&& arg, Accum *accum){
			return f(arg,closure.children,accum);
		}
	};
	
	template<typename F, typename Accum>
	constexpr Accum* fold(F &&f, Accum* accum){
		return _this.fold(fold_specialize<F>{f,*this},accum);
	}
};
