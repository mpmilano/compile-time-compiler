#pragma once

#include <vector>
#include <type_traits>

template<typename T> struct Union_elem {
	T t;
	bool is_this_elem;
	template<typename U>
	constexpr Union_elem(U u):t{},is_this_elem{false}{}
	constexpr Union_elem(T t):t{t},is_this_elem{true}{}
	constexpr Union_elem():t{},is_this_elem{false}{}

	template<typename F, typename R>
	void map(F &&f, R& r){
		if (is_this_elem) r = f(t);
	}
	
};

template<typename T1, typename... T> struct Union :
	public Union_elem<T1>, public Union_elem<T>...{
	template<typename U>
		constexpr Union(U u):
		Union_elem<T1>(u),Union_elem<T>(u)...{}

	constexpr Union():
		Union_elem<T1>(),Union_elem<T>()...{}
	
	template<typename F>
	constexpr auto map(F&& f){
		using R = std::result_of_t<F(T1)>;
		R out_param;
		Union_elem<T1>::map(f,out_param);
		(Union_elem<T>::map(f,out_param),...);
		return out_param;
	}

	template<typename target, typename cand1, typename... candn>
		constexpr auto& get(Union_elem<cand1> &c1, Union_elem<candn>&... c2){
		if constexpr (std::is_same<target,cand1>::value){
				return c1.t;
			}
		else return get<target>(c2...);
	}

	template<typename target>
		constexpr auto& get(){
		return get<target>(*(Union_elem<T1>*)this, *(Union_elem<T>*)this... );
	}

	template<typename target, typename cand1, typename... candn>
		constexpr const auto& get(Union_elem<cand1> &c1, Union_elem<candn>&... c2) const {
		if constexpr (std::is_same<target,cand1>::value){
				return c1.t;
			}
		else return get<target>(c2...);
	}

	template<typename target>
		constexpr const auto& get() const {
		return get<target>(*(Union_elem<T1>*)this, *(Union_elem<T>*)this... );
	}

	
	template<std::size_t target, typename cand1, typename... candn>
		constexpr auto& get(Union_elem<cand1> &c1, Union_elem<candn>&... c2){
		if constexpr (target == 0){
				return c1.t;
			}
		else return get<target-1>(c2...);
	}

	template<std::size_t target>
		constexpr auto& get(){
		return get<target>(*(Union_elem<T1>*)this, *(Union_elem<T>*)this... );
	}

	template<std::size_t target, typename cand1, typename... candn>
		constexpr const auto& get(Union_elem<cand1> &c1, Union_elem<candn>&... c2) const{
		if constexpr (target == 0){
				return c1.t;
			}
		else return get<target-1>(c2...);
	}

	template<std::size_t target>
		constexpr const auto& get() const {
		return get<target>(*(Union_elem<T1>*)this, *(Union_elem<T>*)this... );
	}


	template<typename cand1, typename... candn>
	std::size_t which(Union_elem<cand1> &c1, Union_elem<candn>&... c2) const {
		if (c1.is_this_elem) return 0;
		else return 1 + which(c2...);
	}

	std::size_t which() const {
		return which(*(Union_elem<T1>*)this, *(Union_elem<T>*)this... );
	}
	
};

