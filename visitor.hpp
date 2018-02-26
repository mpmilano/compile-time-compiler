#pragma once
#include "tree.hpp"

template<typename Implementor> struct Visitor {
		
		struct recur{
			Implementor& i;
			template<typename T, typename U, typename Acc>
			constexpr Acc* operator()(T && t, U& u, Acc* acc){
				return i.visit(std::forward<T>(t), u, acc);
			}
		};

	template<typename Children, std::size_t size, typename Accum>
	static constexpr Accum*
	visit_children(Implementor &i, Children *children, Accum *accum) {
		if constexpr (size == 0) return accum;
		else {
			auto *child = children[0];
			if (child)
				return Visitor::visit_children<Children, size-1>
					(i,children + 1, child->fold(recur{i},accum));
			else return accum;
		}
	}

	template<typename Children, std::size_t size, typename Accum>
	static constexpr Accum*
	visit_children(Implementor &i, Children (&children)[size], Accum *accum) {
		auto *child = children[0];
		if (child)
			return Visitor::visit_children<Children, size-1>
				(i,children + 1, child->fold(recur{i},accum));
		else return accum;
	}
	
		struct vst{
			Implementor &i;
			constexpr vst(Implementor& i):i{i}{}
			template<typename T, typename Children, std::size_t size, typename Accum>
			constexpr Accum* operator()(T & t, Children (&children)[size], Accum *ac){
				return i.visit(t,children,ac);
			}
		};
		
	template<std::size_t a, std::size_t b, typename Accum,
					 template<typename> class... T>
	static constexpr Accum* visit(Implementor &i, tree<a,b,T...> &t, Accum *ac){
		return t.fold(vst{i},ac);
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
	template<template<typename> class T, typename U,											\
					 typename Children, std::size_t size,	typename Accum,					\
					 typename = std::enable_if_t<!is_in<T<top_ast> , ## x>::value>* \
					 >																														\
	constexpr Accum* visit(T<U>& t, Children (&rest)[size], Accum *accum){	\
		return visit_children(*this,rest,accum);				\
	}																																			\
																																				\
	template<std::size_t d, std::size_t w, typename Accum,								\
					 template<typename> class... o>																\
	constexpr Accum* visit(tree<d,w,o...> &t, Accum *a){									\
		return visit(*this, t,a);																						\
	}
