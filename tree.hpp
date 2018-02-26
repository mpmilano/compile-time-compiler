#pragma once
#include "ctutils.hpp"

template<std::size_t depth_max, std::size_t child_max, template<typename> class... options>
struct tree;

template<std::size_t child_max, template<typename> class... options>
struct tree<0,child_max, options...>{

	template<typename... T>
	constexpr tree(const T&...){}
	
	template<typename F>
	constexpr auto map(F &&){return ok_result();}
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
		tree &t;
		constexpr provide_children(tree &t):t(t){}
		template<typename N>
		constexpr auto operator()(N& n){
			//return n.provide_children(t.children);
			return ok_result();
		}
	};
	
	template<typename Arg>
	constexpr tree(Arg a):_this{a}{
		_this.map(provide_children{*this});
	}

	constexpr tree():
		_this{}{
		_this.map(provide_children{*this});
	}

	template<typename F>
	struct map_specialize{
		F& f;
		tree& closure;
		constexpr map_specialize(F& f, tree& c):f(f),closure(c){}
		template<typename Arg>
		constexpr auto operator()(Arg&& arg){
			return f(arg,closure.children);
		}
	};
	
	template<typename F>
	constexpr auto map(F &&f){
		return _this.map(map_specialize<F>{f,*this});
	}
};
