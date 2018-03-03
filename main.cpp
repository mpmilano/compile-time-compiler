#include "ast.hpp"
#include <iostream>
#include "mutils/CTString.hpp"

#define $ITEM(trn,e,plus) trn.e = allocator.template allocate<AST_elem>(); (trn.$(e) = plus{}).template get<plus>()
#define $PLUS(trn,e) $ITEM(trn,e,plus)


template<std::size_t size>
struct stringref{
	using arr = char[size];
	arr& str;
	std::size_t begin_pos{0};
	std::size_t end_pos{0};
	constexpr stringref(arr& str):str{str}{}
};

int main(){
	struct tc{
		static constexpr auto test_compile(){
			Allocator<15,transaction, AST_elem> allocator;
			auto& trn = allocator.top;
			$PLUS(trn,e).payload = 14;
			/*
				trn.e = allocator.template allocate<AST_elem>();
				trn.$(e) = plus{};*/
			return allocator;
			
		}
	};
	constexpr auto allocator = tc::test_compile();
	auto fourteen = allocator.top.$(e).template get<plus>().payload;
	//using str = DECT(MUTILS_STRING("hello")::trim_ends());
	std:: cout << fourteen << " " /*<< str{} */<< std::endl;
	return fourteen;
}
