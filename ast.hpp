#pragma once
#include "allocator.hpp"
#include "union.hpp"

struct transaction; struct plus; struct skip;
using AST_elem = Union<transaction, plus, skip>;

struct transaction {
	allocated_ref<AST_elem> e;
	std::size_t payload{0};
	constexpr transaction(){};
	constexpr transaction(transaction&& p)
		:e{std::move(p.e)}{}
	constexpr transaction& operator=(transaction&& p){
		e = std::move(p.e);
		return *this;
	}
};

struct plus {
	allocated_ref<AST_elem> e;
	std::size_t payload{0};
	constexpr plus(){}
	constexpr plus(plus&& p)
		:e{std::move(p.e)}{}
	constexpr plus& operator=(plus&& p){
		e = std::move(p.e);
		return *this;
	}
};

struct skip {
	allocated_ref<AST_elem> e;
	std::size_t payload{0};
	constexpr skip(){}
	constexpr skip(skip&& p)
		:e{std::move(p.e)}{}

	constexpr skip& operator=(skip&& p){
		e = std::move(p.e);
		return *this;
	}
};

