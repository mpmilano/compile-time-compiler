#include "ctutils.hpp"
#include "ast.hpp"
#include "test_ast.hpp"
#include <iostream>

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

	constexpr test_union tu{3.4};
	std::cout << tu.template get<double>() + tu.template get<2>()<< std::endl;

	constexpr auto res = phases(test_tree{});
	if constexpr (res.left.status == result::status::error){
			static_assert(res.left.status == result::status::ok,"An error occurred");
		}
	else {
		std::cout << res.right.left().match([](auto *ptr){return ptr->field.field;})
							<< std::endl;
	}


	ast::top_ast a{ast::transaction{}};
	ast::noop_visitor{}.visit(a);
	
}
