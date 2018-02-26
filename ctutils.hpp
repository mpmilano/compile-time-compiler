#pragma once

#include <vector>
#include <type_traits>
#include <iostream>
#include "union.hpp"


using test_union = Union<int,std::size_t,double>;

template<typename T>
struct Option{
	
	Union<std::nullptr_t, T> internal;
	constexpr Option(T t):internal{t}{}
	constexpr Option():internal(nullptr){}
	
	template<typename F>
	constexpr auto map(F &&f);

	template<typename F>
	constexpr auto map(F &&f) const;
	template<typename F>
	constexpr auto match(F &&f){
		switch (internal.which()){
		case 0: return f(internal.template get<0>());
		case 1: return f(&internal.template get<1>());
		default: throw "impossible";
		}
	}
	
	template<typename F>
	constexpr auto match(F &&f) const {
		switch (internal.which()){
		case 0: return f(internal.template get<0>());
		case 1: return f(&internal.template get<1>());
		default: throw "impossible";
		}
	}

	constexpr bool is_null() const {
		assert(internal.is_initialized);
		assert(internal.which() < 2);
		return internal.which() == 0;
	}

	constexpr auto operator=(const Option &t2){
		internal.operator=(t2.internal);
		return *this;
	}

	constexpr auto operator=(const T &t2){
		internal.operator=(t2);
		return *this;
	}
	
};

template<std::size_t length>
struct ctstring{
	constexpr ctstring():data{{0}}{}
	char data[length];
};

using string = ctstring<2048>;


struct result {
	enum class status{
		ok,error
			};
	constexpr result(status s)
		:status(s){}
	constexpr result()
		:status(status::ok){}
	status status;
	Option<string> message;
	
};

auto& operator<<(std::ostream& o, result r){
	switch(r.status){
	case result::status::ok :
		return o << "ok";
	case result::status::error:
		return o << "error";
	}
}

constexpr result ok_result(){
	return result{result::status::ok};
}

template<std::size_t size>
constexpr result err_result(const char (&)[size]){
	return result{result::status::error};
}

template<typename T>
template<typename F>
constexpr auto Option<T>::map(F &&f){
	if (internal.which() == 1) return f(internal.template get<1>());
	else return ok_result();
};

template<typename T>
template<typename F>
constexpr auto Option<T>::map(F &&f) const {
	if (internal.which() == 1) return f(internal.template get<1>());
	else return ok_result();
};
