// wrap_vector_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>

#include <yggr/lua_wrap/wrap_vector.hpp>

#include <typeinfo>
#include <iostream>
#include <cassert>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

template<typename Cont>
void test_construct(void)
{
	typedef Cont wrap_cont_type;
	typedef typename wrap_cont_type::base_type cont_type;
	typedef typename wrap_cont_type::value_type value_type;

	{
		wrap_cont_type cont;
		yggr_test_assert(cont.empty());
	}

	{
		typename wrap_cont_type::allocator_type alloc;
		wrap_cont_type cont(alloc);
		yggr_test_assert(cont.empty());
	}

	{
		typename wrap_cont_type::wrap_allocator_type alloc;
		wrap_cont_type cont(alloc);
		yggr_test_assert(cont.empty());
	}

	{
		wrap_cont_type cont(5);
		yggr_test_assert(cont.size() == 5);
	}


	{
#	if defined(YGGR_SYSTEM_64)
		typedef yggr::lua_wrap::wrap_base_t<typename wrap_cont_type::size_type> wrap_count_type;
#	else
		typedef yggr::lua_wrap::wrap_base_t<yggr::u64> wrap_count_type;
#	endif // (YGGR_SYSTEM_64)
		wrap_cont_type cont(wrap_count_type(5));
		yggr_test_assert(cont.size() == 5);
	}

	{
		typename wrap_cont_type::wrap_allocator_type alloc;
		wrap_cont_type cont(5, alloc);
		yggr_test_assert(cont.size() == 5);
	}

	{
#	if defined(YGGR_SYSTEM_64)
		typedef yggr::lua_wrap::wrap_base_t<typename wrap_cont_type::size_type> wrap_count_type;
#	else
		typedef yggr::lua_wrap::wrap_base_t<yggr::u64> wrap_count_type;
#	endif // (YGGR_SYSTEM_64)
		typename wrap_cont_type::wrap_allocator_type alloc;
		wrap_cont_type cont(wrap_count_type(5), alloc);
		yggr_test_assert(cont.size() == 5);
	}

	{
		wrap_cont_type cont(5, 1);
		value_type chk[] = {1, 1, 1, 1, 1};
		yggr_test_assert(0 == memcmp(boost::addressof(cont[0]), chk, sizeof(value_type) * cont.size()));
	}

	{
#	if defined(YGGR_SYSTEM_64)
		typedef yggr::lua_wrap::wrap_base_t<typename wrap_cont_type::size_type> wrap_count_type;
#	else
		typedef yggr::lua_wrap::wrap_base_t<yggr::u64> wrap_count_type;
#	endif //(YGGR_SYSTEM_64)
		wrap_cont_type cont(wrap_count_type(5), 1);
		value_type chk[] = {1, 1, 1, 1, 1};
		yggr_test_assert(0 == memcmp(boost::addressof(cont[0]), chk, sizeof(value_type) * cont.size()));
	}

	{
		typename wrap_cont_type::allocator_type alloc;
		wrap_cont_type cont(5, 1, alloc);
		value_type chk[] = {1, 1, 1, 1, 1};
		yggr_test_assert(0 == memcmp(boost::addressof(cont[0]), chk, sizeof(value_type) * cont.size()));
	}

	{
		typename wrap_cont_type::wrap_allocator_type alloc;
		wrap_cont_type cont(5, 1, alloc);
		value_type chk[] = {1, 1, 1, 1, 1};
		yggr_test_assert(0 == memcmp(boost::addressof(cont[0]), chk, sizeof(value_type) * cont.size()));
	}

	{
#	if defined(YGGR_SYSTEM_64)
		typedef yggr::lua_wrap::wrap_base_t<typename wrap_cont_type::size_type> wrap_count_type;
#	else
		typedef yggr::lua_wrap::wrap_base_t<yggr::u64> wrap_count_type;
#	endif //(YGGR_SYSTEM_64)
		typename wrap_cont_type::wrap_allocator_type alloc;
		wrap_cont_type cont(wrap_count_type(5), 1, alloc);
		value_type chk[] = {1, 1, 1, 1, 1};
		yggr_test_assert(0 == memcmp(boost::addressof(cont[0]), chk, sizeof(value_type) * cont.size()));
	}

	{
		value_type arr[] = {1, 2, 3, 4, 5};
		wrap_cont_type cont(arr + 0, arr + 5);
		yggr_test_assert(0 == memcmp(boost::addressof(cont[0]), arr, sizeof(value_type) * cont.size()));
	}

	{
		value_type arr[] = {1, 2, 3, 4, 5};
		wrap_cont_type tmp_count(arr + 0, arr + 5);
		wrap_cont_type cont(tmp_count.wraped_begin(), tmp_count.wraped_end());
		yggr_test_assert(0 == memcmp(boost::addressof(cont[0]), arr, sizeof(value_type) * cont.size()));
	}

	{
		value_type arr[] = {1, 2, 3, 4, 5};
		typename wrap_cont_type::wrap_allocator_type alloc;
		wrap_cont_type cont(arr + 0, arr + 5, alloc);
		yggr_test_assert(0 == memcmp(boost::addressof(cont[0]), arr, sizeof(value_type) * cont.size()));
	}

	{
		value_type arr[] = {1, 2, 3, 4, 5};
		typename wrap_cont_type::wrap_allocator_type alloc;
		wrap_cont_type tmp_cont(arr + 0, arr + 5);
		std::reverse(tmp_cont.begin(), tmp_cont.end());
		wrap_cont_type cont(tmp_cont.wraped_rbegin(), tmp_cont.wraped_rend(), alloc);
		yggr_test_assert(0 == memcmp(boost::addressof(cont[0]), arr, sizeof(value_type) * cont.size()));
	}

	{
		value_type arr[] = {1, 2, 3, 4, 5};
		cont_type tmp_cont(arr + 0, arr + 5);
		wrap_cont_type cont(boost::move(tmp_cont));
		yggr_test_assert(0 == memcmp(boost::addressof(cont[0]), arr, sizeof(value_type) * cont.size()));
	}

	{
		value_type arr[] = {1, 2, 3, 4, 5};
		cont_type tmp_cont(arr + 0, arr + 5);
		wrap_cont_type cont(tmp_cont);
		yggr_test_assert(0 == memcmp(boost::addressof(cont[0]), arr, sizeof(value_type) * cont.size()));
	}

	{
		value_type arr[] = {1, 2, 3, 4, 5};
		typename wrap_cont_type::allocator_type alloc;
		cont_type tmp_cont(arr + 0, arr + 5);
		wrap_cont_type cont(boost::move(tmp_cont), alloc);
		yggr_test_assert(0 == memcmp(boost::addressof(cont[0]), arr, sizeof(value_type) * cont.size()));
	}

	{
		value_type arr[] = {1, 2, 3, 4, 5};
		typename wrap_cont_type::wrap_allocator_type alloc;
		cont_type tmp_cont(arr + 0, arr + 5);
		wrap_cont_type cont(tmp_cont, alloc);
		yggr_test_assert(0 == memcmp(boost::addressof(cont[0]), arr, sizeof(value_type) * cont.size()));
	}

	{
		value_type arr[] = {1, 2, 3, 4, 5};
		wrap_cont_type tmp_cont(arr + 0, arr + 5);
		wrap_cont_type cont(boost::move(tmp_cont));
		yggr_test_assert(0 == memcmp(boost::addressof(cont[0]), arr, sizeof(value_type) * cont.size()));
	}

	{
		value_type arr[] = {1, 2, 3, 4, 5};
		wrap_cont_type tmp_cont(arr + 0, arr + 5);
		wrap_cont_type cont(tmp_cont);
		yggr_test_assert(0 == memcmp(boost::addressof(cont[0]), arr, sizeof(value_type) * cont.size()));
	}

	{
		value_type arr[] = {1, 2, 3, 4, 5};
		typename wrap_cont_type::wrap_allocator_type alloc;
		wrap_cont_type tmp_cont(arr + 0, arr + 5);
		wrap_cont_type cont(boost::move(tmp_cont), alloc);
		yggr_test_assert(0 == memcmp(boost::addressof(cont[0]), arr, sizeof(value_type) * cont.size()));
	}

	{
		value_type arr[] = {1, 2, 3, 4, 5};
		typename wrap_cont_type::wrap_allocator_type alloc;
		wrap_cont_type tmp_cont(arr + 0, arr + 5);
		wrap_cont_type cont(tmp_cont, alloc);
		yggr_test_assert(0 == memcmp(boost::addressof(cont[0]), arr, sizeof(value_type) * cont.size()));
	}

	std::cout << "------test_construct [" << typeid(wrap_cont_type).name() << "] end------" << std::endl;
}

template<typename Cont>
void test_operator_set(void)
{
	typedef Cont wrap_cont_type;
	typedef typename wrap_cont_type::base_type cont_type;
	typedef typename wrap_cont_type::value_type value_type;

	value_type arr[] = {1, 2, 3, 4, 5};

	{
		cont_type tmp_cont(arr + 0, arr + 5);
		wrap_cont_type cont;

		cont = boost::move(tmp_cont);

		yggr_test_assert(tmp_cont.empty());
		yggr_test_assert(0 == memcmp(boost::addressof(cont[0]), arr, sizeof(value_type) * cont.size()));
	}

	{
		cont_type tmp_cont(arr + 0, arr + 5);
		wrap_cont_type cont;

		cont = tmp_cont;

		yggr_test_assert(!tmp_cont.empty());
		yggr_test_assert(0 == memcmp(boost::addressof(cont[0]), arr, sizeof(value_type) * cont.size()));
	}

	{
		wrap_cont_type tmp_cont(arr + 0, arr + 5);
		wrap_cont_type cont;

		cont = boost::move(tmp_cont);

		yggr_test_assert(tmp_cont.empty());
		yggr_test_assert(0 == memcmp(boost::addressof(cont[0]), arr, sizeof(value_type) * cont.size()));
	}

	{
		wrap_cont_type tmp_cont(arr + 0, arr + 5);
		wrap_cont_type cont;

		cont = tmp_cont;

		yggr_test_assert(!tmp_cont.empty());
		yggr_test_assert(0 == memcmp(boost::addressof(cont[0]), arr, sizeof(value_type) * cont.size()));
	}

	std::cout << "------test_operator_set [" << typeid(wrap_cont_type).name() << "] end------" << std::endl;
}

template<typename Cont>
void test_swap(void)
{
	typedef Cont wrap_cont_type;
	typedef typename wrap_cont_type::base_type cont_type;
	typedef typename wrap_cont_type::value_type value_type;

	value_type arr[] = {1, 2, 3, 4, 5};

	{
		cont_type tmp_cont(arr + 0, arr + 5);
		wrap_cont_type cont;

		cont.swap(boost::move(tmp_cont));

		yggr_test_assert(tmp_cont.empty());
		yggr_test_assert(0 == memcmp(boost::addressof(cont[0]), arr, sizeof(value_type) * cont.size()));
	}

	{
		cont_type tmp_cont(arr + 0, arr + 5);
		wrap_cont_type cont;

		cont.swap(tmp_cont);

		yggr_test_assert(tmp_cont.empty());
		yggr_test_assert(0 == memcmp(boost::addressof(cont[0]), arr, sizeof(value_type) * cont.size()));
	}

	{
		wrap_cont_type tmp_cont(arr + 0, arr + 5);
		wrap_cont_type cont;

		cont.swap(boost::move(tmp_cont));

		yggr_test_assert(tmp_cont.empty());
		yggr_test_assert(0 == memcmp(boost::addressof(cont[0]), arr, sizeof(value_type) * cont.size()));
	}

	{
		wrap_cont_type tmp_cont(arr + 0, arr + 5);
		wrap_cont_type cont;

		cont.swap(tmp_cont);

		yggr_test_assert(tmp_cont.empty());
		yggr_test_assert(0 == memcmp(boost::addressof(cont[0]), arr, sizeof(value_type) * cont.size()));
	}

	// non-member swap

	{
		cont_type tmp_cont(arr + 0, arr + 5);
		wrap_cont_type cont;

		yggr::swap(cont, boost::move(tmp_cont));

		yggr_test_assert(tmp_cont.empty());
		yggr_test_assert(0 == memcmp(boost::addressof(cont[0]), arr, sizeof(value_type) * cont.size()));
	}

	{
		cont_type tmp_cont(arr + 0, arr + 5);
		wrap_cont_type cont;

		yggr::swap(cont, tmp_cont);

		yggr_test_assert(tmp_cont.empty());
		yggr_test_assert(0 == memcmp(boost::addressof(cont[0]), arr, sizeof(value_type) * cont.size()));
	}

	{
		wrap_cont_type tmp_cont(arr + 0, arr + 5);
		wrap_cont_type cont;

		yggr::swap(cont, boost::move(tmp_cont));

		yggr_test_assert(tmp_cont.empty());
		yggr_test_assert(0 == memcmp(boost::addressof(cont[0]), arr, sizeof(value_type) * cont.size()));
	}

	{
		wrap_cont_type tmp_cont(arr + 0, arr + 5);
		wrap_cont_type cont;

		yggr::swap(cont, tmp_cont);

		yggr_test_assert(tmp_cont.empty());
		yggr_test_assert(0 == memcmp(boost::addressof(cont[0]), arr, sizeof(value_type) * cont.size()));
	}

	std::cout << "------test_swap [" << typeid(wrap_cont_type).name() << "] end------" << std::endl;
}

template<typename Cont>
void test_assign(void)
{
	typedef Cont wrap_cont_type;
	typedef typename wrap_cont_type::base_type cont_type;
	typedef typename wrap_cont_type::value_type value_type;

	typedef typename yggr::lua_wrap::wrap_base_t_maker<typename wrap_cont_type::size_type>::type size_wrap_base_t;

	{
		value_type arr[] = {1, 1, 1, 1, 1};
		wrap_cont_type cont;

		cont.assign(5, 1);

		yggr_test_assert(0 == memcmp(boost::addressof(cont[0]), arr, sizeof(value_type) * cont.size()));
	}

	{
		value_type arr[] = {1, 1, 1, 1, 1};
		wrap_cont_type cont;

		cont.assign(size_wrap_base_t(5), 1);

		yggr_test_assert(0 == memcmp(boost::addressof(cont[0]), arr, sizeof(value_type) * cont.size()));
	}

	{
		value_type arr[] = {1, 2, 3, 4, 5};
		wrap_cont_type cont;

		cont.assign(arr + 0, arr + 5);

		yggr_test_assert(0 == memcmp(boost::addressof(cont[0]), arr, sizeof(value_type) * cont.size()));
	}

	{
		value_type arr[] = {1, 2, 3, 4, 5};
		wrap_cont_type tmp_cont(arr + 0, arr + 5);
		wrap_cont_type cont;

		cont.assign(tmp_cont.begin(), tmp_cont.end());

		yggr_test_assert(0 == memcmp(boost::addressof(cont[0]), arr, sizeof(value_type) * cont.size()));
	}

	{
		value_type arr[] = {1, 2, 3, 4, 5};
		wrap_cont_type tmp_cont(arr + 0, arr + 5);
		wrap_cont_type cont;

		cont.assign(tmp_cont.cbegin(), tmp_cont.cend());

		yggr_test_assert(0 == memcmp(boost::addressof(cont[0]), arr, sizeof(value_type) * cont.size()));
	}

	{
		value_type arr[] = {1, 2, 3, 4, 5};
		wrap_cont_type tmp_cont(arr + 0, arr + 5);
		wrap_cont_type cont;

		cont.assign(tmp_cont.wraped_begin(), tmp_cont.wraped_end());

		yggr_test_assert(0 == memcmp(boost::addressof(cont[0]), arr, sizeof(value_type) * cont.size()));
	}

	{
		value_type arr[] = {1, 2, 3, 4, 5};
		wrap_cont_type tmp_cont(arr + 0, arr + 5);
		wrap_cont_type cont;

		cont.assign(tmp_cont.wraped_cbegin(), tmp_cont.wraped_cend());

		yggr_test_assert(0 == memcmp(boost::addressof(cont[0]), arr, sizeof(value_type) * cont.size()));
	}

	{
		value_type arr[] = {1, 2, 3, 4, 5};
		wrap_cont_type tmp_cont(arr + 0, arr + 5);
		wrap_cont_type cont;

		cont.assign(tmp_cont.rbegin(), tmp_cont.rend());

		std::reverse(boost::begin(cont), boost::end(cont));

		yggr_test_assert(0 == memcmp(boost::addressof(cont[0]), arr, sizeof(value_type) * cont.size()));
	}

	{
		value_type arr[] = {1, 2, 3, 4, 5};
		wrap_cont_type tmp_cont(arr + 0, arr + 5);
		wrap_cont_type cont;

		cont.assign(tmp_cont.crbegin(), tmp_cont.crend());

		std::reverse(boost::begin(cont), boost::end(cont));

		yggr_test_assert(0 == memcmp(boost::addressof(cont[0]), arr, sizeof(value_type) * cont.size()));
	}

	{
		value_type arr[] = {1, 2, 3, 4, 5};
		wrap_cont_type tmp_cont(arr + 0, arr + 5);
		wrap_cont_type cont;

		cont.assign(tmp_cont.wraped_rbegin(), tmp_cont.wraped_rend());

		std::reverse(boost::begin(cont), boost::end(cont));

		yggr_test_assert(0 == memcmp(boost::addressof(cont[0]), arr, sizeof(value_type) * cont.size()));
	}

	{
		value_type arr[] = {1, 2, 3, 4, 5};
		wrap_cont_type tmp_cont(arr + 0, arr + 5);
		wrap_cont_type cont;

		cont.assign(tmp_cont.wraped_crbegin(), tmp_cont.wraped_crend());

		std::reverse(boost::begin(cont), boost::end(cont));

		yggr_test_assert(0 == memcmp(boost::addressof(cont[0]), arr, sizeof(value_type) * cont.size()));
	}

	{
		wrap_cont_type cont;
		std::cout << typeid(cont.get_allocator()).name() << std::endl;
		std::cout << typeid(cont.get_wraped_allocator()).name() << std::endl;
	}

	std::cout << "------test_assign [" << typeid(wrap_cont_type).name() << "] end------" << std::endl;
}

template<typename Cont>
void test_element_access(void)
{
	typedef Cont wrap_cont_type;
	typedef typename wrap_cont_type::base_type cont_type;
	typedef typename wrap_cont_type::value_type value_type;

	typedef typename yggr::lua_wrap::wrap_base_t_maker<typename wrap_cont_type::size_type>::type size_wrap_base_t;

	value_type arr[] = {1, 2, 3, 4, 5};

	// at operator[]
	{
		wrap_cont_type cont(arr + 0, arr + 5);
		const wrap_cont_type& cont_cref = cont;

		yggr_test_assert(cont.at(0) == 1);
		yggr_test_assert(cont.at(1) == 2);
		yggr_test_assert(cont.at(size_wrap_base_t(0)) == 1);
		yggr_test_assert(cont.at(size_wrap_base_t(1)) == 2);

		yggr_test_assert(cont_cref.at(2) == 3);
		yggr_test_assert(cont_cref.at(3) == 4);
		yggr_test_assert(cont_cref.at(size_wrap_base_t(2)) == 3);
		yggr_test_assert(cont_cref.at(size_wrap_base_t(3)) == 4);

		yggr_test_assert(cont[0] == 1);
		yggr_test_assert(cont[1] == 2);
		yggr_test_assert(cont[size_wrap_base_t(0)] == 1);
		yggr_test_assert(cont[size_wrap_base_t(1)] == 2);

		yggr_test_assert(cont_cref[2] == 3);
		yggr_test_assert(cont_cref[3] == 4);

		yggr_test_assert(cont_cref[size_wrap_base_t(2)] == 3);
		yggr_test_assert(cont_cref[size_wrap_base_t(3)] == 4);
	}

	{
		wrap_cont_type cont(arr + 0, arr + 5);
		const wrap_cont_type& cont_cref = cont;

		yggr_test_assert(cont.get_value(0) == 1);
		yggr_test_assert(cont_cref.get_value(1) == 2);

		yggr_test_assert(cont.get_value(size_wrap_base_t(0)) == 1);
		yggr_test_assert(cont_cref.get_value(size_wrap_base_t(1)) == 2);

		cont.set_value(4, 50);
		yggr_test_assert(cont[4] == 50);
		cont.set_value(size_wrap_base_t(4), 5);
		yggr_test_assert(cont[4] == 5);
	}

	{
		wrap_cont_type cont(arr + 0, arr + 5);
		const wrap_cont_type& cont_cref = cont;

		yggr_test_assert(cont.front() == 1);
		yggr_test_assert(cont_cref.front() == 1);

		yggr_test_assert(cont.back() == 5);
		yggr_test_assert(cont_cref.back() == 5);
	}

	{
		wrap_cont_type cont(arr + 0, arr + 5);
		//const wrap_cont_type& cont_cref = cont;

		yggr_test_assert(cont.data() == boost::addressof(cont[0]));
		yggr_test_assert(*cont.data() == cont.front());
	}

	std::cout << "------test_element_access [" << typeid(wrap_cont_type).name() << "] end------" << std::endl;
}

template<typename Cont>
void test_iterators(void)
{
	typedef Cont wrap_cont_type;
	typedef typename wrap_cont_type::base_type cont_type;
	typedef typename wrap_cont_type::value_type value_type;

	typedef typename yggr::lua_wrap::wrap_base_t_maker<typename wrap_cont_type::size_type>::type size_wrap_base_t;

	value_type arr[] = {1, 2, 3, 4, 5};

	{
		wrap_cont_type cont(arr + 0, arr + 5);

		yggr_test_assert(*cont.begin() == cont.front());
		yggr_test_assert(*cont.cbegin() == cont.front());

		yggr_test_assert(*cont.wraped_begin() == cont.front());
		yggr_test_assert(*cont.wraped_cbegin() == cont.front());

		yggr_test_assert(*yggr::advance_copy(cont.end(), -1) == cont.back());
		yggr_test_assert(*yggr::advance_copy(cont.cend(), -1) == cont.back());

		yggr_test_assert(*yggr::advance_copy(cont.wraped_end(), -1) == cont.back());
		yggr_test_assert(*yggr::advance_copy(cont.wraped_cend(), -1) == cont.back());

		yggr_test_assert(*cont.rbegin() == cont.back());
		yggr_test_assert(*cont.crbegin() == cont.back());

		yggr_test_assert(*cont.wraped_rbegin() == cont.back());
		yggr_test_assert(*cont.wraped_crbegin() == cont.back());

		yggr_test_assert(*yggr::advance_copy(cont.rend(), -1) == cont.front());
		yggr_test_assert(*yggr::advance_copy(cont.crend(), -1) == cont.front());

		yggr_test_assert(*yggr::advance_copy(cont.wraped_rend(), -1) == cont.front());
		yggr_test_assert(*yggr::advance_copy(cont.wraped_crend(), -1) == cont.front());
	}

	std::cout << "------test_iterators [" << typeid(wrap_cont_type).name() << "] end------" << std::endl;
}

template<typename Cont>
void test_capacity(void)
{
	typedef Cont wrap_cont_type;
	typedef typename wrap_cont_type::base_type cont_type;
	typedef typename wrap_cont_type::value_type value_type;
	typedef typename wrap_cont_type::size_type size_type;
	typedef typename yggr::lua_wrap::wrap_base_t_maker<size_type>::type size_wrap_base_t;

	value_type arr[] = {1, 2, 3, 4, 5};

	{
		wrap_cont_type cont(arr + 0, arr + 5);

		yggr_test_assert(!cont.empty());
		yggr_test_assert(cont.size() == 5);
		yggr_test_assert(cont.wraped_size() == 5);
		//yggr_test_assert(cont.max_size() == (size_type(-1) / sizeof(value_type)));
		//yggr_test_assert(cont.wraped_max_size() == (size_type(-1) / sizeof(value_type)));
		yggr_test_assert(cont.max_size() == (static_cast<cont_type&>(cont)).max_size() );
		yggr_test_assert(cont.wraped_max_size() == (static_cast<cont_type&>(cont)).max_size() );

		cont.reserve(cont.size() + 100);
		yggr_test_assert(cont.capacity() == cont.size() + 100);

		cont.shrink_to_fit();
		yggr_test_assert(cont.size() == cont.capacity());


	}

	std::cout << "------test_capacity [" << typeid(wrap_cont_type).name() << "] end------" << std::endl;
}

template<typename Cont>
void test_modify(void)
{
	typedef Cont wrap_cont_type;
	typedef typename wrap_cont_type::base_type cont_type;
	typedef typename wrap_cont_type::value_type value_type;
	typedef typename wrap_cont_type::size_type size_type;
	typedef typename yggr::lua_wrap::wrap_base_t_maker<size_type>::type size_wrap_base_t;

	value_type arr[] = {1, 2, 3, 4, 5};

	{
		wrap_cont_type cont(arr + 0, arr + 5);
		cont.clear();

		yggr_test_assert(cont.empty());
	}

	{
		wrap_cont_type cont;
		cont.insert(cont.begin(), boost::move(arr[0]));
		yggr_test_assert(cont.front() == arr[0]);
		cont.insert(cont.begin(), arr[1]);
		yggr_test_assert(cont.front() == arr[1]);

		cont.insert(cont.end(), 3, arr[2]);
		yggr_test_assert(cont.size() == 5);
		yggr_test_assert(cont.back() == arr[2]);

		cont.insert(cont.begin(), size_wrap_base_t(3), arr[3]);
		yggr_test_assert(cont.size() == 5 + 3);
		yggr_test_assert(cont.front() == arr[3]);
	}

	{
		wrap_cont_type tmp_cont(arr + 0, arr + 5);
		wrap_cont_type cont;

		cont.insert(cont.end(), arr + 0, arr + 5);
		yggr_test_assert(0 == memcmp(arr, boost::addressof(cont[0]), sizeof(arr)));

		cont.insert(cont.end(), tmp_cont.begin(), tmp_cont.end());
		yggr_test_assert(0 == memcmp(boost::addressof(cont[tmp_cont.size() * 1]), boost::addressof(tmp_cont[0]), sizeof(arr)));

		cont.insert(cont.end(), tmp_cont.wraped_begin(), tmp_cont.wraped_end());
		yggr_test_assert(0 == memcmp(boost::addressof(cont[tmp_cont.size() * 2]), boost::addressof(tmp_cont[0]), sizeof(arr)));

		cont.insert(cont.end(), tmp_cont.cbegin(), tmp_cont.cend());
		yggr_test_assert(0 == memcmp(boost::addressof(cont[tmp_cont.size() * 3]), boost::addressof(tmp_cont[0]), sizeof(arr)));

		cont.insert(cont.end(), tmp_cont.wraped_cbegin(), tmp_cont.wraped_cend());
		yggr_test_assert(0 == memcmp(boost::addressof(cont[tmp_cont.size() * 4]), boost::addressof(tmp_cont[0]), sizeof(arr)));

		cont.insert(cont.end(), tmp_cont.rbegin(), tmp_cont.rend());
		yggr_test_assert(std::equal(tmp_cont.rbegin(), tmp_cont.rend(), yggr::advance_copy(cont.begin(), tmp_cont.size() * 5)));

		cont.insert(cont.end(), tmp_cont.wraped_rbegin(), tmp_cont.wraped_rend());
		yggr_test_assert(std::equal(tmp_cont.wraped_rbegin(), tmp_cont.wraped_rend(), yggr::advance_copy(cont.begin(), tmp_cont.size() * 6)));
	}

	{
		wrap_cont_type tmp_cont(arr + 0, arr + 5);
		wrap_cont_type cont;
		typename wrap_cont_type::wrap_iterator_type ret;

		ret = cont.wraped_insert(cont.end(), arr + 0, arr + 5);
		yggr_test_assert(0 == memcmp(arr, boost::addressof(cont[0]), sizeof(arr)));

		ret = cont.wraped_insert(cont.end(), tmp_cont.begin(), tmp_cont.end());
		yggr_test_assert(0 == memcmp(boost::addressof(cont[tmp_cont.size() * 1]), boost::addressof(tmp_cont[0]), sizeof(arr)));

		ret = cont.wraped_insert(cont.end(), tmp_cont.wraped_begin(), tmp_cont.wraped_end());
		yggr_test_assert(0 == memcmp(boost::addressof(cont[tmp_cont.size() * 2]), boost::addressof(tmp_cont[0]), sizeof(arr)));

		ret = cont.wraped_insert(cont.end(), tmp_cont.cbegin(), tmp_cont.cend());
		yggr_test_assert(0 == memcmp(boost::addressof(cont[tmp_cont.size() * 3]), boost::addressof(tmp_cont[0]), sizeof(arr)));

		ret = cont.wraped_insert(cont.end(), tmp_cont.wraped_cbegin(), tmp_cont.wraped_cend());
		yggr_test_assert(0 == memcmp(boost::addressof(cont[tmp_cont.size() * 4]), boost::addressof(tmp_cont[0]), sizeof(arr)));

		ret = cont.wraped_insert(cont.end(), tmp_cont.rbegin(), tmp_cont.rend());
		yggr_test_assert(std::equal(tmp_cont.rbegin(), tmp_cont.rend(), yggr::advance_copy(cont.begin(), tmp_cont.size() * 5)));

		ret = cont.wraped_insert(cont.end(), tmp_cont.wraped_rbegin(), tmp_cont.wraped_rend());
		yggr_test_assert(std::equal(tmp_cont.wraped_rbegin(), tmp_cont.wraped_rend(), yggr::advance_copy(cont.begin(), tmp_cont.size() * 6)));
	}

	{
		wrap_cont_type cont;

		cont.emplace(cont.end());
		yggr_test_assert(cont[0] == 0);
		cont.emplace(cont.end(), 1);
		yggr_test_assert(cont[1] == 1);
	}

	{
		wrap_cont_type cont(arr + 0, arr + 5);

		cont.erase(cont.begin());
		yggr_test_assert(cont.size() == 4);

		cont.erase(cont.cbegin());
		yggr_test_assert(cont.size() == 3);

		cont.erase(cont.begin(), yggr::advance_copy(cont.begin(), 1));
		yggr_test_assert(cont.size() == 2);

		cont.erase(cont.begin(), yggr::advance_copy(cont.cbegin(), 1));
		yggr_test_assert(cont.size() == 1);

		cont.erase(cont.cbegin(), yggr::advance_copy(cont.begin(), 1));
		yggr_test_assert(cont.size() == 0);

		cont.erase(cont.cbegin(), cont.cend());
		yggr_test_assert(cont.size() == 0);
	}

	{
		wrap_cont_type cont(arr + 0, arr + 5);
		cont.insert(cont.end(), arr + 0, arr + 5);
		cont.insert(cont.end(), arr + 0, arr + 5);
		cont.insert(cont.end(), arr + 0, arr + 5);

		yggr_test_assert(cont.size() == 20);

		typename wrap_cont_type::wrap_iterator_type ret;

		ret = cont.wraped_erase(cont.begin());
		yggr_test_assert(cont.size() == 19);

		ret = cont.wraped_erase(cont.cbegin());
		yggr_test_assert(cont.size() == 18);

		ret = cont.wraped_erase(cont.wraped_begin());
		yggr_test_assert(cont.size() == 17);

		ret = cont.wraped_erase(cont.wraped_cbegin());
		yggr_test_assert(cont.size() == 16);

		ret = cont.wraped_erase(cont.begin(), yggr::advance_copy(cont.begin(), 1));
		yggr_test_assert(cont.size() == 15);

		ret = cont.wraped_erase(cont.begin(), yggr::advance_copy(cont.wraped_begin(), 1));
		yggr_test_assert(cont.size() == 14);

		ret = cont.wraped_erase(cont.begin(), yggr::advance_copy(cont.cbegin(), 1));
		yggr_test_assert(cont.size() == 13);

		ret = cont.wraped_erase(cont.begin(), yggr::advance_copy(cont.wraped_cbegin(), 1));
		yggr_test_assert(cont.size() == 12);

		ret = cont.wraped_erase(cont.cbegin(), yggr::advance_copy(cont.begin(), 1));
		yggr_test_assert(cont.size() == 11);

		ret = cont.wraped_erase(cont.cbegin(), yggr::advance_copy(cont.wraped_begin(), 1));
		yggr_test_assert(cont.size() == 10);

		ret = cont.wraped_erase(cont.cbegin(), yggr::advance_copy(cont.cbegin(), 1));
		yggr_test_assert(cont.size() == 9);

		ret = cont.wraped_erase(cont.cbegin(), yggr::advance_copy(cont.wraped_cbegin(), 1));
		yggr_test_assert(cont.size() == 8);

		ret = cont.wraped_erase(cont.wraped_begin(), yggr::advance_copy(cont.begin(), 1));
		yggr_test_assert(cont.size() == 7);

		ret = cont.wraped_erase(cont.wraped_begin(), yggr::advance_copy(cont.wraped_begin(), 1));
		yggr_test_assert(cont.size() == 6);

		ret = cont.wraped_erase(cont.wraped_begin(), yggr::advance_copy(cont.cbegin(), 1));
		yggr_test_assert(cont.size() == 5);

		ret = cont.wraped_erase(cont.wraped_begin(), yggr::advance_copy(cont.wraped_cbegin(), 1));
		yggr_test_assert(cont.size() == 4);

		ret = cont.wraped_erase(cont.wraped_cbegin(), yggr::advance_copy(cont.begin(), 1));
		yggr_test_assert(cont.size() == 3);

		ret = cont.wraped_erase(cont.wraped_cbegin(), yggr::advance_copy(cont.wraped_begin(), 1));
		yggr_test_assert(cont.size() == 2);

		ret = cont.wraped_erase(cont.wraped_cbegin(), yggr::advance_copy(cont.cbegin(), 1));
		yggr_test_assert(cont.size() == 1);

		ret = cont.wraped_erase(cont.wraped_cbegin(), yggr::advance_copy(cont.wraped_cbegin(), 1));
		yggr_test_assert(cont.size() == 0);
	}

	{
		wrap_cont_type cont;
		cont.push_back(boost::move(arr[0]));
		yggr_test_assert(cont.back() == arr[0]);

		cont.push_back(10);
		yggr_test_assert(cont.back() == 10);

		cont.emplace_back();
		yggr_test_assert(cont.back() == 0);

		cont.emplace_back(boost::move(arr[1]));
		yggr_test_assert(cont.back() == arr[1]);

		cont.pop_back();
		yggr_test_assert(cont.back() == 0);
	}

	{
		wrap_cont_type cont;
		cont.resize(10);
		yggr_test_assert(cont.size() == 10);

		cont.resize(20, 100);
		yggr_test_assert(cont.size() == 20);
		yggr_test_assert(cont.front() == 0);
		yggr_test_assert(cont.back() == 100);
	}

	{
		wrap_cont_type cont;
		cont.resize(size_wrap_base_t(10));
		yggr_test_assert(cont.size() == 10);

		cont.resize(size_wrap_base_t(20), 100);
		yggr_test_assert(cont.size() == 20);
		yggr_test_assert(cont.front() == 0);
		yggr_test_assert(cont.back() == 100);
	}


	std::cout << "------test_modify [" << typeid(wrap_cont_type).name() << "] end------" << std::endl;
}

template<typename Cont>
void test_operator_op(void)
{
	typedef Cont wrap_cont_type;
	typedef typename wrap_cont_type::base_type cont_type;
	typedef typename wrap_cont_type::value_type value_type;
	typedef typename wrap_cont_type::size_type size_type;
	typedef typename yggr::lua_wrap::wrap_base_t_maker<size_type>::type size_wrap_base_t;

	value_type arr1[] = {1, 2, 3, 4, 5};
	value_type arr2[] = {6, 7, 8, 9, 10};
	value_type arr3[] = {1, 2, 3, 4, 5};

	wrap_cont_type cont1(arr1 + 0, arr1 + 5);
	wrap_cont_type cont2(arr2 + 0, arr2 + 5);
	wrap_cont_type cont3(arr3, arr3 + 5);

	yggr_test_assert(cont1 == cont3);

	yggr_test_assert(cont1.unwrap() == cont3);
	yggr_test_assert(cont1 == cont3.unwrap());
	yggr_test_assert(cont1 == cont3);

	yggr_test_assert(cont1.unwrap() != cont2);
	yggr_test_assert(cont1 != cont2.unwrap());
	yggr_test_assert(cont1 != cont2);

	yggr_test_assert(cont1.unwrap() < cont2);
	yggr_test_assert(cont1 < cont2.unwrap());
	yggr_test_assert(cont1 < cont2);

	yggr_test_assert(cont1.unwrap() <= cont2);
	yggr_test_assert(cont1 <= cont2.unwrap());
	yggr_test_assert(cont1 <= cont2);

	yggr_test_assert(cont1.unwrap() <= cont3);
	yggr_test_assert(cont1 <= cont3.unwrap());
	yggr_test_assert(cont1 <= cont3);

	yggr_test_assert(cont2.unwrap() > cont1);
	yggr_test_assert(cont2 > cont1.unwrap());
	yggr_test_assert(cont2 > cont1);

	yggr_test_assert(cont2.unwrap() >= cont1);
	yggr_test_assert(cont2 >= cont1.unwrap());
	yggr_test_assert(cont2 >= cont1);

	yggr_test_assert(cont3.unwrap() >= cont1);
	yggr_test_assert(cont3 >= cont1.unwrap());
	yggr_test_assert(cont3 >= cont1);

	std::cout << "cont1:\t" << cont1 << std::endl;
	std::cout << "cont2:\t" << cont2 << std::endl;
	std::cout << "cont3:\t" << cont3 << std::endl;

	std::cout << "------test_operator_op [" << typeid(wrap_cont_type).name() << "] end------" << std::endl;
}

int main(int argc, char* argv[])
{
	test_construct<yggr::lua_wrap::wrap_vector<yggr::s64> >();
	test_construct<yggr::lua_wrap::wrap_vector<yggr::u64> >();

	test_operator_set< yggr::lua_wrap::wrap_vector<yggr::s64> >();
	test_operator_set< yggr::lua_wrap::wrap_vector<yggr::u64> >();

	test_swap< yggr::lua_wrap::wrap_vector<yggr::s64> >();
	test_swap< yggr::lua_wrap::wrap_vector<yggr::u64> >();

	test_assign< yggr::lua_wrap::wrap_vector<yggr::s64> >();
	test_assign< yggr::lua_wrap::wrap_vector<yggr::u64> >();

	test_element_access< yggr::lua_wrap::wrap_vector<yggr::s64> >();
	test_element_access< yggr::lua_wrap::wrap_vector<yggr::u64> >();

	test_iterators< yggr::lua_wrap::wrap_vector<yggr::s64> >();
	test_iterators< yggr::lua_wrap::wrap_vector<yggr::u64> >();

	test_capacity< yggr::lua_wrap::wrap_vector<yggr::s64> >();
	test_capacity< yggr::lua_wrap::wrap_vector<yggr::u64> >();

	test_modify< yggr::lua_wrap::wrap_vector<yggr::s64> >();
	test_modify< yggr::lua_wrap::wrap_vector<yggr::u64> >();

	test_operator_op< yggr::lua_wrap::wrap_vector<yggr::s64> >();
	test_operator_op< yggr::lua_wrap::wrap_vector<yggr::u64> >();

	wait_any_key(argc, argv);
	return 0;
}
