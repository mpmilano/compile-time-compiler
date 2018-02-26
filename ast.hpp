#pragma once
#include "ctutils.hpp"

template<std::size_t depth_max, std::size_t child_max, typename... options>
struct tree;

template<std::size_t child_max, typename... options>
struct tree<0,child_max, options...>{

	template<typename... T>
	constexpr tree(const T&...){}
	
	template<typename F>
	constexpr auto map(F &&){return ok_result();}
};

template<std::size_t depth_max, std::size_t child_max, typename... options>
struct tree{
	Union<options...> _this;
	using child_tree = tree<depth_max - 1, child_max, options...>;
	Option<child_tree> children[child_max] = {Option<child_tree>{}};

	constexpr void initialize_children(){
		for (std::size_t i = 0; i < child_max; ++i){
			auto& child = children[i];
			child = Option<child_tree>{};
			//assert(child.is_null());
			//assert(child.internal.which() < 2);
		}
	}
	
	template<typename Arg>
	constexpr tree(Arg a):
		_this{a}{
			initialize_children();
		}

	constexpr tree():
		_this{}{
			initialize_children();
		}

	template<typename F>
	struct map_specialize{
		F& f;
		tree& closure;
		constexpr map_specialize(F& f, tree& c):f(f),closure(c){}
		template<typename Arg>
		constexpr auto operator()(Arg&& arg){
			return f(arg,closure.children);
		}
	};
	
	template<typename F>
	constexpr auto map(F &&f){
		map_specialize<F> ms{f,*this};
		return _this.map(ms);
	}

	template<typename T>
	constexpr auto add_child(T t){
		for (Option<child_tree>& child : children){
			if (child.is_null()) {
				child = t;
				return *this;
			}
		}
		return *this;
	}
};

namespace ast {
	
	struct transaction {
		constexpr transaction(){}
	};

	
	struct plus {
		constexpr plus(){}
	};

	struct for_each {
		constexpr for_each(){}
	};

	using maxval = std::integral_constant<std::size_t,3>;
	template<std::size_t value>
	using ast = tree<value,maxval::value,transaction,plus,for_each>;
	using top_ast = ast<maxval::value>;
	
	template<typename Implementor> struct Visitor {
		
		struct recur{
			Implementor& i;
			template<typename T>
			constexpr auto operator()(T && t){
				return i.visit(t);
			}
		};

		template<typename Children, std::size_t size>
		static constexpr auto visit_children(Implementor &i, Children (&children)[size]){
			for (auto& child : children){
				if (child.map(recur{i}).status == result::status::error){
					return err_result("");
				}
			}
			return ok_result();
		}

		struct vst{
			Implementor &i;
			constexpr vst(Implementor& i):i{i}{}
			template<typename T, typename Children, std::size_t size>
			constexpr auto operator()(T & t, Children (&children)[size]){
				return i.visit(t,children);
			}
		};
		
		template<std::size_t a, std::size_t b, typename... T>
		static constexpr auto visit(Implementor &i, tree<a,b,T...> &t){
			return t.map(vst{i});
		}
		
	};

	
template<typename target, typename... t> struct is_in;

template<typename target> struct is_in<target> : public std::false_type {};

template<typename target, typename cand, typename... t>
struct is_in<target,cand,t...> :
	public std::integral_constant<
	bool,
	std::is_same<target,cand>::value ||
	is_in<target,t...>::value
	>{};

#define IMPLEMENTED_CASES(x...)																					\
	template<typename T, typename Children, std::size_t size,							\
					 typename = std::enable_if_t<!is_in<T , ## x>::value>* \
					 >																														\
	constexpr auto visit(T& t, Children (&rest)[size]){										\
		return visit_children(*this,rest);																	\
	}																																			\
																																				\
	template<std::size_t d, std::size_t w, typename... o>									\
	constexpr auto visit(tree<d,w,o...> &t){															\
		return visit(*this, t);																							\
	}

	
}

