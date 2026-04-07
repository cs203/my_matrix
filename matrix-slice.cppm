export module matrix:slice;
import std;
import :support;

using std::initializer_list;
using std::array;
using std::inner_product;

export template<std::size_t N>
struct Matrix_slice{
	Matrix_slice()=default; // an empty matrix: no elements

	Matrix_slice(std::size_t s, initializer_list<std::size_t> exts);// extents
	Matrix_slice(std::size_t s, initializer_list<std::size_t> exts, 
			initializer_list<std::size_t> strs);//extents and strides
	template<typename... Dims>
	Matrix_slice(Dims... dims);

	template<std::convertible_to<std::size_t>... Dims>
		std::size_t operator()(Dims... dims) const;
				
	std::size_t size; //total number of elements
	std::size_t start; // starting offset
	array<std::size_t, N> extents; // number of elements in each dimension
	array<std::size_t, N> strides; // offsets between elements in each dimension

};

template<std::size_t N>
template<typename... Dims>
Matrix_slice<N>::Matrix_slice(Dims... dims):start{0}
{
	static_assert(sizeof...(Dims) == N, 
			"Matrix_slice<N>::Matrix_slice(Dims... dims): dimensions mismatch");
	std::size_t args[N]{std::size_t(dims)...};
	std::copy(std::begin(args), std::end(args), extents.begin());
	size = matrix_impl::compute_strides(extents, strides);
}

template<std::size_t N>
template<std::convertible_to<std::size_t>... Dims>
std::size_t Matrix_slice<N>::operator()(Dims... dims) const
{
	static_assert(sizeof...(Dims) == N, "");
	std::size_t args[N]{std::size_t(dims)...};
	return start + std::inner_product(args, args+N, strides.begin(), std::size_t(0));
}


template<std::size_t N>
Matrix_slice<N>::Matrix_slice(std::size_t s, initializer_list<std::size_t> exts): start{s}
{
	//assert(exts.size() == N);
	std::copy(exts.begin(), exts.end(), extents.begin());
	size = matrix_impl::compute_strides(extents, strides);
}

template<std::size_t N>
Matrix_slice<N>::Matrix_slice(std::size_t s, initializer_list<std::size_t> exts, initializer_list<std::size_t> strs):start{s}
{
	//assert(exts.size() == N);
	std::copy(exts.begin(), exts.end(), extents.begin());
	std::copy(strs.begin(), strs.end(), strides.begin());
	return matrix_impl::compute_size(extents);
}

