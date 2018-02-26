#pragma once
#include "tree.hpp"

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
