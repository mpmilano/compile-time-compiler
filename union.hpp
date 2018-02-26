#pragma once

#include <vector>
#include <type_traits>
#include <cassert>

template<typename T> struct Union_elem {
	T t;
	bool is_this_elem;
	template<typename U>
	constexpr Union_elem(U u):t{},is_this_elem{false}{}
	constexpr Union_elem(T t):t{t},is_this_elem{true}{}
	constexpr Union_elem():t{},is_this_elem{false}{}

	constexpr Union_elem& operator=(const Union_elem& u){
		t = u.t;
		is_this_elem = u.is_this_elem;
		return *this;
	}

	template<typename F, typename R>
	constexpr void map(F &&f, R& r){
		if (is_this_elem) r = f(t);
	}
	
};

template<> struct Union_elem<std::nullptr_t> {
	std::nullptr_t t;
	bool is_this_elem;
	template<typename U>
	constexpr Union_elem(U):t{nullptr},is_this_elem{false}{}
	constexpr Union_elem(std::nullptr_t t):t{t},is_this_elem{true}{}
	constexpr Union_elem():t{nullptr},is_this_elem{false}{}

	constexpr Union_elem& operator=(const Union_elem& u){
		return *this;
	}

	template<typename F, typename R>
	constexpr void map(F &&f, R& r){
		if (is_this_elem) r = f(t);
	}
	
};

template<typename T1, typename... T> struct Union :
	public Union_elem<T1>, public Union_elem<T>...{

	bool is_initialized{false};
	
	template<typename U>
		constexpr Union(U u):
		is_initialized{true},
		Union_elem<T1>(u),Union_elem<T>(u)...{}

	constexpr Union():
		is_initialized{false},
		Union_elem<T1>(),Union_elem<T>()...{}
	
	template<typename F>
	constexpr auto map(F&& f){
		using R = decltype(f(*(T1*)nullptr));
		R out_param;
		Union_elem<T1>::map(f,out_param);
		(Union_elem<T>::map(f,out_param),...);
		return out_param;
	}

	constexpr Union& operator=(const Union &u){
		Union_elem<T1>::operator=(u);
		(Union_elem<T>::operator=(u),...);
		return *this;
	}

	template<typename U>
	constexpr Union& operator=(const U &u){
		Union_elem<U>* _this = this;
		Union_elem<T1>::is_this_elem = false;
		((Union_elem<T>::is_this_elem = false),...);
		_this->t = u;
		_this->is_this_elem = true;
		return *this;
	}

	template<typename target>
		constexpr auto& get_(){
		Union_elem<target>* _this = this;
		//assert(_this->is_this_elem);
		return *_this;
	}

	
	template<typename target>
		constexpr auto& get(){
		return get_<target>().t;
	}



	template<typename target, typename cand1, typename... candn>
		constexpr const auto& _get(Union_elem<cand1> &c1, Union_elem<candn>&... c2) const {
		if constexpr (std::is_same<target,cand1>::value){
				return c1.t;
			}
		else return _get<target>(c2...);
	}

	template<typename target>
		constexpr const auto& get() const {
		return _get<target>(*(Union_elem<T1>*)this, *(Union_elem<T>*)this... );
	}

	
	template<std::size_t target, typename cand1, typename... candn>
		static constexpr auto& _get(Union_elem<cand1> &c1, Union_elem<candn>&... c2){
		if constexpr (target == 0){
				return c1.t;
			}
		else return _get<target-1>(c2...);
	}

	template<std::size_t target, typename cand1, typename... candn>
		static const constexpr auto& _get(const Union_elem<cand1> &c1,
																		 const Union_elem<candn>&... c2){
		if constexpr (target == 0){
				return c1.t;
			}
		else return _get<target-1>(c2...);
	}


	template<std::size_t target>
		constexpr auto& get(){
		return _get<target>(*(Union_elem<T1>*)this, *(Union_elem<T>*)this... );
	}

	template<std::size_t target>
		constexpr const auto& get() const {
		return _get<target>(*(Union_elem<T1>*)this, *(Union_elem<T>*)this... );
	}
	
	static constexpr std::size_t _which(){
		return 0;
	}
	
	template<typename cand1, typename... candn>
	static constexpr std::size_t _which(const Union_elem<cand1> &c1,
																		 const Union_elem<candn>&... c2) {
		if (c1.is_this_elem) return 0;
		else return 1 + Union::_which(c2...);
	}

	constexpr std::size_t which() const {
		auto ret = _which(*(Union_elem<T1>*)this, *(Union_elem<T>*)this... );
		return ret;
	}
	
};

