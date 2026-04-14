export module matrix;

import std;
import :base;
export import :slice;
export import :support;


using std::size_t;
using std::vector;


export template<typename T, size_t N>
class Matrix: public Matrix_base<T, N>
{
public:
	using iterator = typename vector<T>::iterator;
	using const_iterator = typename vector<T>::const_iterator;

	Matrix() = default;
	Matrix(Matrix&&) = default;
	Matrix& operator=(Matrix&&) = default;

	Matrix(Matrix const&) = default;
	Matrix& operator=(const Matrix&) = default;
	~Matrix() = default;

	Matrix(Matrix_initializer<T,N> init);

	template<typename U>
		Matrix(std::initializer_list<U>) = delete;

	template<typename... Exts>
		explicit Matrix(Exts... exts);


	size_t size() const { return elems_.size();}

	T* data() {return elems_.data();}
	const T* data() const{ return elems_.data();}


private:
	vector<T> elems_;
};

//------------

struct substitution_failure{};

template<typename T>
struct substitution_succeeded: std::true_type {};

template<>
struct substitution_succeeded<substitution_failure>: std::false_type{};

template <typename M>
struct get_matrix_type_result {
  template <typename T, size_t N, typename = std::enable_if<(N >= 1)>>
  static bool check(const Matrix<T, N> &m);

  static substitution_failure check(...);

  using type = decltype(check(std::declval<M>()));
};

template<typename T>
struct has_matrix_type: substitution_succeeded<typename get_matrix_type_result<T>::type>{};
//----


template<typename T, size_t N>
template<typename... Exts>
Matrix<T, N>::Matrix(Exts... exts):Matrix_base<T, N>{exts...}, elems_(this->desc_.size)
{
}

template<typename T, size_t N>
Matrix<T,N>::Matrix(Matrix_initializer<T, N> init)
{
	this->desc_.start = 0;
	this->desc_.extents = matrix_impl::derive_extents<N>(init);
	this->desc_.size = matrix_impl::compute_strides(this->desc_.extents, this->desc_.strides);
	elems_.reserve(this->desc_.size);
	matrix_impl::insert_flat(init, elems_);
}

