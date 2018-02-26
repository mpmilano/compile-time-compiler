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
	Union<options<tree>...> _this;
	using child_tree = tree<depth_max - 1, child_max, options...>;
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

namespace ast {

	template<typename Owner>
	struct _transaction {
		constexpr _transaction(){}
		template<typename Child, std::size_t size>
		constexpr auto provide_children(Child(&)[size]){
		}
	};

	template<typename Owner>
	struct _plus {
		constexpr _plus(){}
		template<typename Child, std::size_t size>
		constexpr auto provide_children(Child(&)[size]){
		}
	};
	
	template<typename Owner>
	struct _for_each {
		constexpr _for_each(){}
		template<typename Child, std::size_t size>
		constexpr auto provide_children(Child(&)[size]){
		}
	};

	using maxval = std::integral_constant<std::size_t,3>;
	template<std::size_t value>
	using ast = tree<value,maxval::value,_transaction,_plus,_for_each>;
	using top_ast = ast<maxval::value>;

	using for_each = _for_each<top_ast>;
	using plus = _plus<top_ast>;
	using transaction = _transaction<top_ast>;
	
	template<typename Implementor> struct Visitor {
		
		struct recur{
			Implementor& i;
			template<typename T, typename U>
			constexpr auto operator()(T && t, U& u){
				return i.visit(t, u);
			}
		};

		template<typename Children, std::size_t size>
		static constexpr auto visit_children(Implementor &i, Children (&children)[size]){
			for (auto& child : children){
				if (child && child->map(recur{i}).status == result::status::error){
					return err_result("");
				}
			}
			return ok_result();
		}

		struct vst{
			Implementor &i;
			constexpr vst(Implementor& i):i{i}{}
			template<typename T, typename Children, std::size_t size>
			constexpr auto operator()(T & t, Children (&children)[size]){
				return i.visit(t,children);
			}
		};
		
		template<std::size_t a, std::size_t b, template<typename> class... T>
		static constexpr auto visit(Implementor &i, tree<a,b,T...> &t){
			return t.map(vst{i});
		}
		
	};

	
template<typename target, typename... t> struct is_in;

template<typename target> struct is_in<target> : public std::false_type {};

template<typename target, typename cand, typename... t>
struct is_in<target,cand,t...> :
	public std::integral_constant<
	bool,
	std::is_same<target,cand>::value ||
	is_in<target,t...>::value
	>{};

#define IMPLEMENTED_CASES(x...)																					\
	template<template<typename> class T, typename U, typename Children, std::size_t size, \
					 typename = std::enable_if_t<!is_in<T<top_ast> , ## x>::value>* \
					 >																														\
	constexpr auto visit(T<U>& t, Children (&rest)[size]){								\
		return visit_children(*this,rest);																	\
	}																																			\
																																				\
	template<std::size_t d, std::size_t w, template<typename> class... o>				\
	constexpr auto visit(tree<d,w,o...> &t){															\
		return visit(*this, t);																							\
	}

	
}

