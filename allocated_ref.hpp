#pragma

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
