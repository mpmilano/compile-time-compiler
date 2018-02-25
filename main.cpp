#include <vector>
#include <type_traits>

template<typename T1, typename T2>
struct Union {
	T1 t1;
	T2 t2;
	bool is_t1;
	constexpr Union(T1 t1):t1{t1},is_t1{true},t2{}{}	
	constexpr Union(T2 t2):t1{},t2{t2},is_t1{false}{}
	constexpr void operator=(const Union &u){
		is_t1 = u.is_t1;
		if (is_t1) t1 = u.t1;
		else t2 = u.t2;
	}

	constexpr void operator=(const T1 &_t1){
		is_t1 = true;
		t1 = _t1;
	}

	constexpr void operator=(const T2 &_t2){
		is_t1 = false;
		t2 = _t2;
	}
};

template<typename T, std::size_t max_depth>
struct FakeTree;

template<typename T>
struct FakeTree<T,0>{};

template<std::size_t length>
struct ctstring{
	constexpr ctstring():data{{0}}{}
	char data[length];
};

using string = ctstring<2048>;

enum class result_status{
	ok,error
		};

using result = Union<string,result_status>;

template<typename T, std::size_t max_depth>
struct FakeTree {
	using child = FakeTree<T,max_depth-1>;
	using child_t = Union<child, std::nullptr_t >;
	using depth = std::integral_constant<std::size_t, max_depth>;

	constexpr FakeTree(){};
	
	child_t _left{nullptr};
	T field;
	child_t _right{nullptr};

	template<typename F>
	/* */
	constexpr result run_on_left(F f){
		/*
			static_assert(std::is_same<result,
			std::result_of<
			F(child*)> >::value,
			"Type error" );
			apparently not the type I wanted...
		*/
		static_assert(max_depth > 1);
		if (_left.is_t1) {
			return f(&_left.t1);
		}
		else return f(_left.t2);
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
		constexpr result operator()(test_tree::child* left){
			left->field.field = 3;
			return result{result_status::ok};
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
	if constexpr (res.left.is_t1){
			static_assert(!res.left.is_t1,"An error occurred");
		}
	else {
		return res.right.left().t1.field.field;
	}
	
}
