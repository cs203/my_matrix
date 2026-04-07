export module matrix:base;
import std;
import :slice;
import :support;
using std::size_t;

export template<typename T, size_t N>
class Matrix_base{
public:
	static constexpr size_t order_ = N; // Number of dimensions
	using value_type = T;
	Matrix_base() = default;
	Matrix_base(Matrix_base&&) = default; // move
	Matrix_base& operator=(Matrix_base&&) = default;
	Matrix_base(const Matrix_base&) = default; // copy
	Matrix_base& operator=(const Matrix_base&) = default;
	~Matrix_base() = default;

	template<typename... Exts>
	explicit Matrix_base(Exts... exts): desc_{exts...}{}

	explicit Matrix_base(const Matrix_slice<N>& ms): desc_{ms}{}

	// number of dimensions
	static constexpr size_t order() { return order_;}
	
	// #elements in the nth dimension
	size_t extent(size_t n) const { return desc_.extents[n];}

	// total number of elements
	virtual size_t size() const = 0;


	//! the slice defining subscripting
	const Matrix_slice<N>& descriptor() const{ return desc_;}
	// "flat" element access
	virtual T* data() = 0;
	virtual const T* data() const = 0;

	size_t n_rows() const { return desc_.extents[0];}
	size_t n_cols() const { return desc_.extents[1];}
/*
	template<typename... Args>
		std::enable_if_t<matrix_impl::Requesting_element<Args...>(), T&> operator()(Args... args);
*/
	template<std::convertible_to<std::size_t>... Args>  T& operator()(Args... args);
	template<std::convertible_to<std::size_t>... Args> const T& operator()(Args... args) const;

protected:
	Matrix_slice<N> desc_;
};

template<typename T, size_t N>
template<std::convertible_to<std::size_t>... Args> 
T& Matrix_base<T, N>::operator()(Args... args) 
{ 
	return *(data() + this->desc_(args...)); 
};



template<typename T, size_t N>
template<std::convertible_to<std::size_t>... Args> 
const T& Matrix_base<T, N>::operator()(Args... args) const 
		{ 
			return *(data() + this->desc_(args...)); 
		};


