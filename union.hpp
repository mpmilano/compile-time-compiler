#pragma once

#include <vector>
#include <type_traits>

template<typename...> struct Union;
template<typename LEFT, typename RIGHT>
struct Union<LEFT,RIGHT> {
	LEFT left;
	RIGHT right;
	bool is_left;
	constexpr Union(LEFT left):left{left},is_left{true},right{}{}	
	constexpr Union(RIGHT right):left{},right{right},is_left{false}{}
	constexpr Union():is_left(false){}
	constexpr void operator=(const Union &u){
		is_left = u.is_left;
		if (is_left) left = u.left;
		else right = u.right;
	}

	template<typename l, typename r>
	static constexpr std::size_t which(const Union<l,r> &o){
		return o.which();
	}
	
	template<typename... T>
	static constexpr std::size_t which(T&&... t){
		return 0;
	}
	
	constexpr std::size_t which() const {
		if (is_left) return 0;
		else {
			return which(right) + 1;
		}
	}

	template<std::size_t index, typename T>
	static constexpr RIGHT& get(T& right){
		static_assert(index == 0, "Error: variant size exceeded");
		return right;
	}

	template<std::size_t index, typename l, typename r>
	static constexpr auto& get(Union<l,r>& o){
		return o.template get<index>();
	}

	template<std::size_t index>
	constexpr auto& get(){
		if constexpr (index == 0) return left;
		else return get<index-1>(right);
	}

	
	template<typename target, typename T>
	static constexpr RIGHT& get(T& right){
		static_assert(std::is_same<target,RIGHT>::value,
									"Error: variant size exceeded");
		return right;
	}

	template<typename target, typename l, typename r>
	static constexpr auto& get(Union<l,r>& o){
		return o.template get<target>();
	}

	template<typename target>
	constexpr auto& get(){
		if constexpr (std::is_same<target,LEFT>::value) return left;
		else return get<target>(right);
	}

	
	template<std::size_t index, typename T>
	static constexpr const RIGHT& get(const T& right){
		static_assert(index == 0, "Error: variant size exceeded");
		return right;
	}

	template<std::size_t index, typename l, typename r>
	static constexpr const auto& get(const Union<l,r>& o){
		return o.template get<index>();
	}

	template<std::size_t index>
	constexpr const auto& get() const {
		if constexpr (index == 0) return left;
		else return get<index-1>(right);
	}

	
	template<typename target, typename T>
	static constexpr const RIGHT& get(const T& right){
		static_assert(std::is_same<target,RIGHT>::value,
									"Error: variant size exceeded");
		return right;
	}

	template<typename target, typename l, typename r>
	static constexpr const auto& get(const Union<l,r>& o){
		return o.template get<target>();
	}

	template<typename target>
	constexpr const auto& get() const {
		if constexpr (std::is_same<target,LEFT>::value) return left;
		else return get<target>(right);
	}

	template<typename F, typename T>
	static constexpr auto map(F&& f, T&& t){
		return f(t);
	}

	template<typename F, typename l, typename r>
	static constexpr auto map(F&& f, Union<l,r> &&o){
		return o.map(f);
	}

	template<typename F>
	constexpr auto map(F&& f){
		if (is_left) return f(left);
		else return map(f,right);
	}

	constexpr void operator=(const LEFT &_left){
		is_left = true;
		left = _left;
	}

	constexpr void operator=(const RIGHT &_right){
		is_left = false;
		right = _right;
	}
};

template<typename LEFT, typename... rest>
struct Union<LEFT,rest...> : public Union<LEFT, Union<rest...>>{
	using super = Union<LEFT, Union<rest...>>;
	constexpr Union(LEFT l):super{l}{}
	template<typename... Args>
		constexpr Union(Args&&... args):super{Union<rest...>{args...}}{}
};
