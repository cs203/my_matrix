export module matrix:support;
import std;
import :trait;
using std::size_t;
using std::initializer_list;

namespace matrix_impl
{
	template<typename T, size_t N>
		struct Matrix_init
		{
			using type = initializer_list<typename Matrix_init<T, N - 1>::type>;
		};

	template<typename T>
		struct Matrix_init<T, 1>
		{
			using type = initializer_list<T>;
		};

	template<typename T>
		struct Matrix_init<T, 0>;


	export template<size_t N>
		size_t compute_strides(const std::array<size_t, N> &exts, std::array<size_t, N> & strs)
		{
			size_t st = 1;
			for(int i = N - 1; 0 <= i; --i)
			{
				strs[i] = st;
				st *= exts[i];
			}
			return st;
		}

	export template<size_t N>
		size_t compute_size(const std::array<size_t, N> &exts)
		{
			return std::accumulate(exts.begin(), exts.end(), 1, 
					std::multiplies<size_t>{});
		}

	
	
	template<typename T, typename Vec>
		void add_list(const T* first, const T* last, Vec& vec)
		{
			vec.insert(vec.end(), first, last);
		}

	template<typename T, typename Vec>
		void add_list(const initializer_list<T>* first, const initializer_list<T>* last, Vec &vec)
		{
			for(;first != last; ++first)
				add_list(first->begin(), first->end(), vec);
		}

	export template<typename T, typename Vec>
		void insert_flat(initializer_list<T> list, Vec& vec)
		{
			add_list(list.begin(), list.end(), vec);
		}

	template<size_t N, typename I, typename List>
		std::enable_if_t<(N==1), void> add_extents(I& fisrt, const List& list)
		{
			*fisrt = list.size();
		}


	template<size_t N, typename I, typename List>
		std::enable_if_t<(N>1), void> add_extents(I& first, const List& list)
		{
			*first = list.size();
			add_extents<N-1>(++first, *list.begin());
		}

	export template<size_t N, typename List>
		std::array<size_t, N> derive_extents(const List& list)
		{
			std::array<size_t, N> a;
			auto f = a.begin();
			add_extents<N>(f, list); // put extents from list into f[]
			return a;
		}


	export template<typename... Args>
		constexpr bool Requesting_element()
		{
			return All(std::is_convertible_v<Args, size_t>...);
		}

};

export template<typename T, size_t N>
using Matrix_initializer = typename matrix_impl::Matrix_init<T, N>::type;
