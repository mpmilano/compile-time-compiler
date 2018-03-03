#pragma once
#include <cassert>
#include <utility>
#include "array.hpp"

template<std::size_t size, typename T> struct SingleAllocator{

	array<T,size> data;
	bool open_slots[size];

	template<typename Allocator>
	constexpr SingleAllocator(SingleAllocator&& o, Allocator& _new)
		:data(std::move(o.data)),open_slots{false}{
		//initialize data as copy (if that's a thing)
		for (auto i = 0u; i < size; ++i){
			open_slots[i] = o.open_slots[i];
		}
		for (auto& d : data){
			d.repoint(_new);
		}
	}
	constexpr SingleAllocator():open_slots{true}{}

	constexpr std::size_t allocate(){
		for (auto i = 0u; i < size; ++i){
			if (open_slots[i]){
				open_slots[i] = false;
				return i;
			}
		}
		assert(false && "out of memory");
	}


	constexpr void free (std::size_t i){
		open_slots[i] = true;
	}

	
};

template<std::size_t s, typename Top, typename... Subs> struct Allocator;

template<typename T>
struct allocated_ptr{
	std::size_t indx;
	T* t;
	template<typename Allocator>
	constexpr void repoint(Allocator& new_parent){
		t = new_parent.template get<T>().data.ptr(indx);
	}

	allocated_ptr(const allocated_ptr&) = delete;
	allocated_ptr(allocated_ptr&& o):indx(o.indx),t(o.t){
		o.t = nullptr;
	}

	constexpr allocated_ptr():indx{0},t{nullptr}{}
	
	constexpr allocated_ptr& operator=(allocated_ptr&& o){
		indx = o.indx;
		t = o.t;
		o.t = nullptr;
	}

	constexpr T& operator*() {
		return *t;
	}

	constexpr T* operator->() const {
		return t;
	}
};


template<std::size_t s, typename Top, typename... Subs> struct Allocator
	: public SingleAllocator<s,Subs>...{

	Top top;	

	constexpr Allocator(){}

	constexpr Allocator(Allocator&& o)
		:SingleAllocator<s,Subs>(std::move(o),*this)...{
		top.repoint(*this);
	}
	

	template<typename T>
		constexpr SingleAllocator<s,T>& get(){
		return *this;
	}

	template<typename T> constexpr allocated_ptr<T> allocate(){
		allocated_ptr<T> ret;
		ret.index = get<T>().allocate();
		ret.t = get<T>().data.ptr(ret.index);
		return ret;
	}

	template<typename T> constexpr void free(std::size_t index){
		get<T>().free(index);
	}
	
};

