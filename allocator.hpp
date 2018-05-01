#pragma once
#include <cassert>
#include <utility>
#include "array.hpp"
#include "allocated_ref.hpp"

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
private:
	constexpr std::size_t _allocate(){
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
public:
	constexpr auto allocate(){
		return allocated_ref<T> {*this};
	}

	constexpr void free (std::size_t i){
		open_slots[i] = true;
	}

	friend class allocated_ref<T>;
};

template<typename T> template<std::size_t s>
constexpr allocated_ref<T>::allocated_ref(SingleAllocator<s,T>& sa)
:indx(sa._allocate() + 1)
{}

template<std::size_t s, typename Top, typename... Subs> struct Allocator
	: public SingleAllocator<s,Subs>...{

	Top top;	

	constexpr Allocator(){}

	constexpr Allocator(Allocator&& o)
		:SingleAllocator<s,Subs>(std::move(o),*this)...,
		top(std::move(o.top)){}
	

	template<typename T>
		constexpr SingleAllocator<s,T>& get(){
		return *this;
	}

	template<typename T>
		constexpr const SingleAllocator<s,T>& get() const {
		return *this;
	}

	template<typename T> constexpr allocated_ref<T> allocate(){
		return get<T>().allocate();
	}

	template<typename T> constexpr void free(std::size_t index){
		get<T>().free(index);
	}

	template<typename T> constexpr void free(allocated_ref<T> ptr){
		get<T>().free(ptr.indx);
	}
	
};

#define $(a) a.get(allocator)


