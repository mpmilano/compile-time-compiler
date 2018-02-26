#include <vector>
#include <type_traits>

template<typename LEFT, typename RIGHT>
struct Union {
	LEFT left;
	RIGHT right;
	bool is_left;
	constexpr Union(LEFT left):left{left},is_left{true},right{}{}	
	constexpr Union(RIGHT right):left{},right{right},is_left{false}{}
	constexpr void operator=(const Union &u){
		is_left = u.is_left;
		if (is_left) left = u.left;
		else right = u.right;
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

template<typename T, std::size_t max_depth>
struct FakeTree;

template<typename T>
struct FakeTree<T,0>{};

template<typename T, std::size_t max_depth>
struct FakeTree {
	using child = FakeTree<T,max_depth-1>;
	using child_t = Option<child>;
	using depth = std::integral_constant<std::size_t, max_depth>;

	constexpr FakeTree(){};
	
	child_t _left;
	T field;
	child_t _right;

	template<typename F>
	/* */
	constexpr result run_on_left(F f){
		static_assert(max_depth > 1);
		return _left.map(f);
	}

	constexpr child_t& left()  {
		static_assert(max_depth > 1);
		return _left;
	};

	constexpr child_t& right()  {
		static_assert(max_depth > 1);
		return _right;
	};

	constexpr const child_t& left() const {
		static_assert(max_depth > 1);
		return _left;
	};

	constexpr const child_t& right() const {
		static_assert(max_depth > 1);
		return _right;
	};
	
};

struct tmp{
	constexpr tmp():field{0}{};
	std::size_t field;
};
using test_tree = FakeTree<tmp,3>;


constexpr result phase1(test_tree& input){
	input.left() = FakeTree<tmp,2>{};
	struct on_left {
		constexpr on_left(){}
		constexpr result operator()(test_tree::child& left){
			left.field.field = 3;
			return ok_result();
		}
	};
	return input.run_on_left(on_left{});
}

template<typename T, typename I>
struct pair{
	T left;
	I right;
	constexpr pair(T t, I i):left(t),right(i){}
};

constexpr pair<result, test_tree> phases (test_tree input){
	auto res = phase1(input);
	return pair<result,test_tree>(res, input);
}

int main(){

	constexpr auto res = phases(test_tree{});
	if constexpr (res.left.status == result::status::error){
			static_assert(res.left.status == result::status::ok,"An error occurred");
		}
	else {
		return res.right.left().match([](auto *ptr){return ptr->field.field;});
	}
	
}
