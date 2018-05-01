#pragma

template<std::size_t size, typename T> struct SingleAllocator;

template<typename T>
struct allocated_ref{
private:
	std::size_t indx;
public:
	
	template<typename Allocator>
	constexpr T& get(Allocator& new_parent){
		return new_parent.template get<T>().data[indx-1];
	}

	template<typename Allocator>
	constexpr const T& get(Allocator& new_parent) const {
		return new_parent.template get<T>().data[indx-1];
	}

	template<typename Allocator>
	constexpr const T& get(const Allocator& new_parent) const {
		return new_parent.template get<T>().data[indx-1];
	}

	constexpr allocated_ref(const allocated_ref&) = delete;
	constexpr allocated_ref(allocated_ref&& o):indx(o.indx){}

	constexpr allocated_ref():indx{0}{}

	template<std::size_t s>
	constexpr allocated_ref(SingleAllocator<s,T>&);
	
	constexpr allocated_ref& operator=(allocated_ref&& o){
		indx = o.indx;
		return *this;
	}
	
};
