#include "ast.hpp"
#include <iostream>
#include "mutils/CTString.hpp"

#define $ITEM(trn,e,plus) trn.e = allocator.template allocate<AST_elem>(); (trn.$(e) = plus{}).template get<plus>()
#define $PLUS(trn,e) $ITEM(trn,e,plus)

struct compiled{
		static constexpr Allocator<15,transaction, AST_elem> test_compile(){
			Allocator<15,transaction, AST_elem> allocator;
			auto& trn = allocator.top;
			$PLUS(trn,e).payload = 14;
			$ITEM(trn.$(e).template get<plus>(),e,skip).payload = 13;
			return allocator;
		}
};

template<typename compiled>
	struct tc{

		static constexpr const Allocator<15,transaction, AST_elem> 
		allocator = compiled::test_compile();

		template<std::size_t budget, typename F> auto evaluate() const {
			if constexpr (budget > 0){
			constexpr const AST_elem& this_node = F{}();
			static_assert(this_node.well_formed());
			static_assert(!this_node.template get_<transaction>().is_this_elem);
			if constexpr (this_node.template get_<skip>().is_this_elem){
				std::cout << "encountered skip with payload: "
				<< this_node.template get<skip>().payload
				<< std::endl;
				return this_node.template get<skip>().payload;
			}
			else if constexpr (this_node.template get_<plus>().is_this_elem){
				constexpr auto& plusv = this_node.template get_<plus>();
				struct left { constexpr const AST_elem& operator()() const {
					//constexpr auto& this_node = F{}();
					return /*plusv.*/F{}().template get<plus>().$(e);
				}};
				
				std::cout << "encountered plus with payload: " 
				<< plusv.t.payload
				<< " whose child has index "
				<< plusv.t.e.indx
				<< std::endl;
				return plusv.t.payload + 
				evaluate<budget-1,left>();
			}
			else return 0;
			}
			else return 0;
		}

		auto evaluate() const {
			struct start {
				constexpr const AST_elem& operator()() const {
					return allocator.top.$(e);
				}
			};
			return evaluate<15,start>();
		}
		constexpr tc(){}
	};


int main(){
	constexpr tc<compiled> testing;
	std::cout << testing.evaluate() << std::endl;
	constexpr auto allocator = compiled::test_compile();
	constexpr auto fourteen = allocator.top.$(e).template get<plus>().payload;
	static_assert(fourteen == 14);
	//using str = DECT(MUTILS_STRING("hello")::trim_ends());
	std:: cout << fourteen << " " /*<< str{} */<< std::endl;
	return fourteen;
}
