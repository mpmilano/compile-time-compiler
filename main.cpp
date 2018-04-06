#include "ast.hpp"
#include <iostream>
#include "mutils/CTString.hpp"
#include "mutils/cstring.hpp"

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

using Alloc = Allocator<15,transaction, AST_elem>;

template<typename string>
struct parse{
	const string _str;
	using str_t = char const[string::string_length+1];
	using str_nc = char[string::string_length+1];
	Alloc allocator;

	constexpr allocated_ref<AST_elem> parse_expression(const str_t& str){
		return allocated_ref<AST_elem>{};
	}
	
	constexpr allocated_ref<AST_elem> parse_statement(const str_t& str){
		return allocated_ref<AST_elem>{};
	}

	constexpr allocated_ref<AST_elem> parse_sequence(const str_t& str){
		using namespace mutils;
		using namespace cstring;
		str_nc string_bufs[20] = {{0}};
		split_outside_parens(';',str,string_bufs);
		return allocated_ref<AST_elem>{};
	}
	
	constexpr parse(){
		//all parsing implemented in the constructor, so that
		//future things can just build this and expect it to work
		allocator.top.e = parse_sequence(_str.string);
	}

};

template<typename string>
struct flatten {
	//parsing happens during construction
	static const constexpr parse<string> prev{};
	template<std::size_t budget, typename F>
	static constexpr auto parse_as_type(){
		static_assert(budget > 0);
		constexpr const AST_elem& e = F{}();
		if constexpr (e.template get_<skip>().is_this_elem){
			return as_types::skip{};
		}
		if constexpr (e.template get_<transaction>().is_this_elem){
			struct arg{
				constexpr arg(){}
				constexpr const AST_elem& operator()() const {
					return e.template get_<transaction>().t.e.get(prev.allocator);
				}
			};
			using body = DECT(parse_as_type<budget-1,arg>());
			return as_types::transaction<body>{};
		}
		if constexpr (e.template get_<plus>().is_this_elem){
			struct arg1{
				constexpr arg1(){}
				constexpr const AST_elem& operator()() const {
					return e.template get_<plus>().t.e.get(prev.allocator);
				}
			};
			struct arg2{
				constexpr arg2(){}
				constexpr const AST_elem& operator()() const {
					return e.template get_<plus>().t.e.get(prev.allocator);
				}
			};
			using left = DECT(parse_as_type<budget-1,arg1>());
			using right = DECT(parse_as_type<budget-1,arg2>());
			return as_types::plus<left,right>{};
		}
		struct error{};
		return error{};
	}
	static constexpr auto parse_as_type(){
		struct arg{
			constexpr arg(){}
			constexpr const AST_elem& operator()() const{
				return prev.allocator.top.e.get(prev.allocator);
			}
		};
		return parse_as_type<15,arg>();
	}
	using parse_t = DECT(parse_as_type());
	Alloc allocator;
	constexpr flatten(){
		
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
	using str = DECT(MUTILS_STRING("hello")::trim_ends());
	constexpr flatten<str> f;
	flatten<str>::parse_t::print();
	std:: cout << fourteen << " " /*<< str{} */<< std::endl;
	return fourteen;
}
