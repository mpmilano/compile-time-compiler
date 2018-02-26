#pragma once
#include "ctutils.hpp"

template<std::size_t depth_max, std::size_t child_max, typename... options>
struct tree;

template<std::size_t child_max, typename... options>
struct tree<0,child_max, options...>{

	template<typename... T>
	constexpr tree(const T&...){}
	
	template<typename F>
	constexpr auto map(F &&){return ok_result();}
};

template<std::size_t depth_max, std::size_t child_max, typename... options>
struct tree{
	Union<options...> _this;
	using child_tree = tree<depth_max - 1, child_max, options...>;
	Option<child_tree> children[child_max];

	template<typename Arg>
	constexpr tree(Arg a):
		_this{a},children{{Option<child_tree>{}}}{}

	constexpr tree():
		_this{},children{{Option<child_tree>{}}}{}

	template<typename F>
	struct map_specialize{
		F& f;
		tree& closure;
		constexpr map_specialize(F& f, tree& c):f(f),closure(c){}
		template<typename Arg>
		auto operator()(Arg&& arg){
			return f(arg,closure.children);
		}
	};
	
	template<typename F>
	constexpr auto map(F &&f){
		map_specialize<F> ms{f,*this};
		return _this.map(ms);
	}
};

namespace ast {
	
	struct transaction {
		constexpr transaction(){}
	};

	
	struct plus {
		constexpr plus(){}
	};

	struct for_each {
		constexpr for_each(){}
	};

	using maxval = std::integral_constant<std::size_t,3>;
	template<std::size_t value>
	using ast = tree<value,maxval::value,transaction,plus,for_each>;
	using top_ast = ast<maxval::value>;
	
	template<typename Implementor> struct Visitor {
		
		struct recur{
			Implementor& i;
			template<typename T>
			auto operator()(T && t){
				return i.visit(t);
			}
		};

		template<typename Children, std::size_t size>
		static constexpr auto visit_children(Implementor &i, Children (&children)[size]){
			for (auto& child : children){
				child.map(recur{i});
			}
			return ok_result();
		}

		struct vst{
			Implementor &i;
			constexpr vst(Implementor& i):i{i}{}
			template<typename T, typename Children, std::size_t size>
			auto operator()(T & t, Children (&children)[size]){
				return i.visit(t,children);
			}
		};
		
		template<std::size_t a, std::size_t b, typename... T>
		static constexpr auto visit(Implementor &i, tree<a,b,T...> &t){
			return t.map(vst{i});
		}
		
	};
	
}

