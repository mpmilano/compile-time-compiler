#include "allocator.hpp"



struct child {
	allocated_ref<child> c;

	constexpr child(){}
	constexpr child(child&&){}
	template<typename A>
	constexpr void repoint(A&& a){
		c.repoint(a);
	}
};

struct top{
	allocated_ref<child> c;

	constexpr top(){}
	constexpr top(top&&){}
	
	template<typename A>
	constexpr void repoint(A&& a){
		c.repoint(a);
	}
};

/*
constexpr array<child,15> test_allocator
(array<child,15> alloc = array<child,15>{}){
	return alloc;
}

int main() {

	constexpr array<child,15> a{test_allocator()};
	
}
//*/


constexpr Allocator<15,top,child> test_allocator
(Allocator<15,top,child> alloc = Allocator<15,top,child>{}){
	return alloc;
}

struct _int{
	int i;
	constexpr _int():i(0){}
};

constexpr int test_array(array<_int,25> arr = array<_int,25>{}){
	_int* temp{nullptr};
	auto ret = arr[0];
	temp = arr.ptr(0);
	return (*temp).i + ret.i;
}

int main() {

	constexpr Allocator<15,top,child> a{test_allocator()};
	constexpr int arr = test_array();
	
	
	
}

//*/
