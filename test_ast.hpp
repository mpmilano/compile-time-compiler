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
		
		//IMPLEMENTED_CASES(skip,plus);

		template<template<typename> class T, typename U,
						 typename Children, std::size_t size,	typename Accum,
						 typename = std::enable_if_t<!is_in<T<top_ast> , skip,plus>::value>*
						 >
		constexpr Accum* visit(T<U>& t, Children (&rest)[size], Accum *accum){
			return visit_children(*this,rest,accum);
		}
		template<std::size_t d, std::size_t w, typename Accum,
						 template<typename> class... o>
		constexpr Accum* visit(tree<d,w,o...> &t, Accum *a){
			return visit(*this, t,a);
		}

		
	};
}
