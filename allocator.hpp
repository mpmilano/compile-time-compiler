#pragma once
#include <cassert>
#include <utility>
#include "array.hpp"

template<std::size_t size, typename T> struct SingleAllocator{

	array<T,size> data;
	bool open_slots[size];

	template<typename Allocator>
	constexpr SingleAllocator(SingleAllocator&& o, Allocator& _new)
		:data(std::move(o.data)),open_slots{false} {
		//initialize data as copy (if that's a thing)
		for (auto i = 0u; i < size; ++i){
			open_slots[i] = o.open_slots[i];
		}
	}
	constexpr SingleAllocator():open_slots{true}{
		for (auto& b : open_slots){
			b = true;
		}
	}

	constexpr std::size_t allocate(){
		for (auto i = 0u; i < size; ++i){
			if (open_slots[i]){
				open_slots[i] = false;
				return i;
			}
		}
		//this will always assert false;
		//gcc just gets upset if I do that directly,
		//and clang gets upset if I don't. 
		if (!open_slots[3]) assert(false && "out of memory");
		else return 0;
	}


	constexpr void free (std::size_t i){
		open_slots[i] = true;
	}

	
};

template<std::size_t s, typename Top, typename... Subs> struct Allocator;

template<typename T>
struct allocated_ref{
	std::size_t indx;
	
	template<typename Allocator>
	constexpr T& get(Allocator& new_parent){
		return new_parent.template get<T>().data[indx];
	}

	template<typename Allocator>
	constexpr const T& get(Allocator& new_parent) const {
		return new_parent.template get<T>().data[indx];
	}

	constexpr allocated_ref(const allocated_ref&) = delete;
	constexpr allocated_ref(allocated_ref&& o):indx(o.indx){}

	constexpr allocated_ref():indx{0}{}
	
	constexpr allocated_ref& operator=(allocated_ref&& o){
		indx = o.indx;
		return *this;
	}
	
};


template<std::size_t s, typename Top, typename... Subs> struct Allocator
	: public SingleAllocator<s,Subs>...{

	Top top;	

	constexpr Allocator(){}

	constexpr Allocator(Allocator&& o)
		:SingleAllocator<s,Subs>(std::move(o),*this)...{
	}
	

	template<typename T>
		constexpr SingleAllocator<s,T>& get(){
		return *this;
	}

	template<typename T>
		constexpr const SingleAllocator<s,T>& get() const {
		return *this;
	}

	template<typename T> constexpr allocated_ref<T> allocate(){
		allocated_ref<T> ret;
		ret.indx = get<T>().allocate();
		return ret;
	}

	template<typename T> constexpr void free(std::size_t index){
		get<T>().free(index);
	}
	
};

#define $(a) a.get(allocator)


