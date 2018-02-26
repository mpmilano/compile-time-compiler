#pragma once

#include <vector>
#include <type_traits>
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
	constexpr auto match(F &&f){
		if (internal.is_left) return (internal.left);
		else return f(&internal.right);
	}

	template<typename F>
	constexpr auto map(F &&f) const ;
	template<typename F>
	constexpr auto match(F &&f) const {
		if (internal.is_left) return f((T*)internal.left);
		else return f(&internal.right);
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
	status status;
	Option<string> message;
	
};

constexpr result ok_result(){
	return result{result::status::ok};
}

template<typename T>
template<typename F>
constexpr auto Option<T>::map(F &&f){
	if (!internal.is_left) return f(internal.right);
	else return ok_result();
};

template<typename T>
template<typename F>
constexpr auto Option<T>::map(F &&f) const {
	if (!internal.is_left) return f(internal.right);
	else return ok_result();
};
