//safe_multi_index_indexes_base_test.cpp

//#define YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION

#include <test/wait_any_key/wait_any_key.hpp>
#include <string>
#include <cassert>

#include <yggr/bind/bind.hpp>
#include <boost/mpl/size_t.hpp>

#include <boost/multi_index/member.hpp>

#include <yggr/safe_container/safe_multi_index_container.hpp>
#include <yggr/safe_container/safe_multi_index_hashed_index.hpp>
#include <yggr/safe_container/safe_multi_index_ordered_index.hpp>

#ifdef YGGR_BOOST_MULTI_INDEX_HAS_RANKED_INDEX
#	include <yggr/safe_container/safe_multi_index_ranked_index.hpp>
#endif // YGGR_BOOST_MULTI_INDEX_HAS_RANKED_INDEX

#include <yggr/safe_container/safe_multi_index_sequenced_index.hpp>
#include <yggr/safe_container/safe_multi_index_random_access_index.hpp>

#include <yggr/safe_container/safe_insert_return_type_base.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

#include YGGR_PP_LINK_LIB(base)

struct A
{
	struct tag_prm1{};
	struct tag_prm2{};

	struct tag_prm3{};
	struct tag_prm4{};
	struct tag_prm5{};
	A(void)
		: prm1(), prm2()
	{
	}

	A(int a, int b, const std::string& c)
		: prm1(a), prm2(b), prm3(c)
	{
	}

	A(const A& right)
		: prm1(right.prm1), prm2(right.prm2), prm3(right.prm3)
	{
	}

	~A(void)
	{
	}

	operator const int&(void) const
	{
		return prm1;
	}

	A& operator=(const std::string& str)
	{
		prm3 = str;
		return *this;
	}

	A& operator=(const A& right)
	{
		if(this == &right)
		{
			return *this;
		}

		prm1 = right.prm1;
		prm2 = right.prm2;
		prm3 = right.prm3;

		return *this;
	}

	int prm1;
	int prm2;
	std::string prm3;
};

bool operator==(const A& l, const A& r)
{
	return l.prm1 == r.prm1 && l.prm2 == r.prm2 && l.prm3 == r.prm3;
}

bool operator<(const A& l, const A& r)
{
	return l.prm1 < r.prm1;
}

template<typename Container>
void test_construct(void)
{
	typedef Container mi_cont_type;

	mi_cont_type cont1;
	yggr_test_assert(cont1.empty());

	{
		typedef typename mi_cont_type::ctor_args_list ctor_args_list_type;
		ctor_args_list_type ctor_args_list;
		mi_cont_type cont(ctor_args_list);
		yggr_test_assert(cont.empty());
	}

	mi_cont_type cont2(cont1.get_allocator());
	yggr_test_assert(cont2.empty());

	{
		mi_cont_type cont(typename mi_cont_type::ctor_args_list(), cont1.get_allocator());
		yggr_test_assert(cont.empty());
	}

	A arr[10] = {A(1, 1, "aaa"), A(2, 2, "bbb"), A(3, 3, "ccc"), A(4, 3, "ddd")};

	mi_cont_type cont3(arr + 0, arr + 10);
	typename mi_cont_type::base_type base_cont3(arr + 0, arr + 10);
	yggr_test_assert(cont3.size() == base_cont3.size());

	{
		mi_cont_type cont(arr + 0, arr + 10, typename mi_cont_type::ctor_args_list());
		typename mi_cont_type::base_type base_cont(arr + 0, arr + 10);
		yggr_test_assert(cont.size() == base_cont.size());
	}

	mi_cont_type cont4(arr + 0, arr + 10, cont1.get_allocator());
	typename mi_cont_type::base_type base_cont4(arr + 0, arr + 10);
	yggr_test_assert(cont4.size() == base_cont4.size());

	mi_cont_type cont5(arr + 0, arr + 10, typename mi_cont_type::ctor_args_list(), cont1.get_allocator());
	typename mi_cont_type::base_type base_cont5(arr + 0, arr + 10);
	yggr_test_assert(cont5.size() == base_cont5.size());

	{
		mi_cont_type cont6(arr + 0, arr + 10, typename mi_cont_type::ctor_args_list(), cont1.get_allocator());
		typename mi_cont_type::base_type base_cont6(arr + 0, arr + 10);
		yggr_test_assert(cont6.size() == base_cont6.size());

		{
			mi_cont_type cont7(cont6);
			yggr_test_assert(cont7.size() == base_cont6.size());

			mi_cont_type cont8(boost::move(cont7));
			yggr_test_assert(cont8.size() == base_cont6.size());
		}

		{
			mi_cont_type cont7(cont6, cont6.get_allocator());
			yggr_test_assert(cont7.size() == base_cont6.size());

			mi_cont_type cont8(boost::move(cont7), cont7.get_allocator());
			yggr_test_assert(cont8.size() == base_cont6.size());
		}
	}
}

template<typename Container>
void test_swap(void)
{
	typedef Container mi_cont_type;

	A arr1[10] = {A(1, 1, "aaa"), A(2, 2, "bbb"), A(3, 3, "ccc"), A(4, 3, "ddd")};
	A arr2[10] = {A(6, 6, "aaa"), A(7, 7, "bbb"), A(8, 8, "ccc"), A(9, 8, "ddd")};

	mi_cont_type cont1(arr1 + 0, arr1 + 10);
	mi_cont_type cont2(arr2 + 0, arr2 + 5);

	typename mi_cont_type::base_type base_cont1(arr1 + 0, arr1 + 10);
	typename mi_cont_type::base_type base_cont2(arr2 + 0, arr2 + 5);
	yggr_test_assert(cont1.size() == base_cont1.size());

	typename mi_cont_type::base_type base_cont3(base_cont1);
	typename mi_cont_type::base_type base_cont4(base_cont2);

	std::swap(cont1, cont2);
	std::swap(cont1, cont2);
	boost::swap(cont1, cont2);
	boost::swap(cont1, cont2);
	yggr::swap(cont1, cont2);
	yggr::swap(cont1, cont2);

	std::swap(cont1.template get<1>(), cont2);
	std::swap(cont1, cont2.template get<1>());
	boost::swap(cont1.template get<1>(), cont2);
	boost::swap(cont1, cont2.template get<1>());
	yggr::swap(cont1.template get<1>(), cont2);
	yggr::swap(cont1, cont2.template get<1>());

	std::swap(cont1.template get<0>(), cont2);
	std::swap(cont1, cont2.template get<0>());
	boost::swap(cont1.template get<0>(), cont2);
	boost::swap(cont1, cont2.template get<1>());
	yggr::swap(cont1.template get<0>(), cont2);
	yggr::swap(cont1, cont2.template get<0>());

	std::swap(cont1, base_cont3);
	std::swap(base_cont3, cont1);
	boost::swap(cont1, base_cont3);
	boost::swap(base_cont3, cont1);
	yggr::swap(cont1, base_cont3);
	yggr::swap(base_cont3, cont1);

	// !!! cont1.swap(base_cont3.get<0>()) swap foo style only supports the first index !!!
	cont1.swap(base_cont3.template get<0>());
	cont1.template get<0>().swap(base_cont3.template get<0>());
	cont1.template get<1>().swap(base_cont3.template get<1>());
	cont1.template get<1>().swap(base_cont3.template get<1>());

	// !!! std::swap(cont1, base_cont3.get<0>()) swap foo style only supports the first index !!!
	std::swap(cont1, base_cont3.template get<0>());
	std::swap(base_cont3.template get<0>(), cont1);
	boost::swap(cont1, base_cont3.template get<0>());
	boost::swap(base_cont3.template get<0>(), cont1);
	yggr::swap(cont1, base_cont3.template get<0>());
	yggr::swap(base_cont3.template get<0>(), cont1);

	std::swap(cont1.template get<0>(), base_cont3.template get<0>());
	std::swap(base_cont3.template get<1>(), cont1.template get<1>());
	boost::swap(cont1.template get<0>(), base_cont3.template get<0>());
	boost::swap(base_cont3.template get<1>(), cont1.template get<1>());
	yggr::swap(cont1.template get<0>(), base_cont3.template get<0>());
	yggr::swap(base_cont3.template get<1>(), cont1.template get<1>());

	std::swap(cont1.template get<0>(), cont2.template get<0>());
	std::swap(cont2.template get<1>(), cont1.template get<1>());
	boost::swap(cont1.template get<0>(), cont2.template get<0>());
	boost::swap(cont2.template get<1>(), cont1.template get<1>());
	yggr::swap(cont1.template get<0>(), cont2.template get<0>());
	yggr::swap(cont2.template get<1>(), cont1.template get<1>());


	// ------------------ boost::swap test------------------------
	// base_type&& this_type&
	boost::swap(boost::move(base_cont4), cont1);
	boost::swap(boost::move(base_cont4.template get<0>()), cont1.template get<0>());
	boost::swap(boost::move(base_cont4.template get<0>()), cont1.template get<0>());
	boost::swap(base_cont4, cont1.template get<0>());
	boost::swap(base_cont4.template get<0>(), cont1.template get<0>());
	yggr_test_assert(base_cont4.size() == base_cont1.size());

	// base_type& this_type&
	boost::swap(base_cont3, cont1);
	yggr_test_assert(base_cont3.size() == base_cont2.size());

	// this_type& base_type&&
	boost::swap(cont1, boost::move(base_cont3));
	yggr_test_assert(base_cont3.size() == base_cont1.size());

	// this_type& base_type&
	boost::swap(cont1, boost::move(base_cont4));
	yggr_test_assert(base_cont4.size() == base_cont2.size());

	// this_type& this_type&
	boost::swap(cont1, cont2);
	boost::swap(cont1, cont2);

	// this_type&& this_type&
	boost::swap(boost::move(cont1), cont2);

	// this_type& this_type&&
	boost::swap(cont1, boost::move(cont2));

	// ------------------ std::swap test------------------------
	// base_type&& this_type&
	std::swap(boost::move(base_cont4), cont1);
	yggr_test_assert(base_cont4.size() == base_cont1.size());

	// base_type& this_type&
	std::swap(base_cont3, cont1);
	yggr_test_assert(base_cont3.size() == base_cont2.size());

	// this_type& base_type&&
	std::swap(cont1, boost::move(base_cont3));
	yggr_test_assert(base_cont3.size() == base_cont1.size());

	// this_type& base_type&
	std::swap(cont1, boost::move(base_cont4));
	yggr_test_assert(base_cont4.size() == base_cont2.size());

	// this_type& this_type&
	std::swap(cont1, cont2);
	std::swap(cont1, cont2);

	// this_type&& this_type&
	std::swap(boost::move(cont1), cont2);

	// this_type& this_type&&
	std::swap(cont1, boost::move(cont2));

	// ------------------ yggr::swap test------------------------
	// base_type&& this_type&
	yggr::swap(boost::move(base_cont4), cont1);
	yggr_test_assert(base_cont4.size() == base_cont1.size());

	// base_type& this_type&
	yggr::swap(base_cont3, cont1);
	yggr_test_assert(base_cont3.size() == base_cont2.size());

	// this_type& base_type&&
	yggr::swap(cont1, boost::move(base_cont3));
	yggr_test_assert(base_cont3.size() == base_cont1.size());

	// this_type& base_type&
	yggr::swap(cont1, boost::move(base_cont4));
	yggr_test_assert(base_cont4.size() == base_cont2.size());

	// this_type& this_type&
	yggr::swap(cont1, cont2);
	yggr::swap(cont1, cont2);

	// this_type&& this_type&
	yggr::swap(boost::move(cont1), cont2);

	// this_type& this_type&&
	yggr::swap(cont1, boost::move(cont2));

}

template<typename Container>
void test_operator_set(void)
{
	typedef Container safe_cont_type;
	typedef typename safe_cont_type::base_type unsafe_cont_type;

	A arr1[10] = {A(1, 1, "aaa"), A(2, 2, "bbb"), A(3, 3, "ccc"), A(4, 3, "ddd")};
	A arr2[10] = {A(6, 6, "aaa"), A(7, 7, "bbb"), A(8, 8, "ccc"), A(9, 8, "ddd")};

	{
		// container
		//inline this_type& operator=(BOOST_RV_REF(typename first_index_type::base_type) right);
		unsafe_cont_type cont_unsafe(arr1 + 0, arr1 + 10);
		safe_cont_type cont_safe(arr2 + 0, arr2 + 10);
		unsafe_cont_type cont_unsafe_chk = cont_unsafe;

		// !!!the operator= styple only supports the first index !!!!
		cont_safe = boost::move(cont_unsafe.template get<0>());
		yggr_test_assert(cont_safe == cont_unsafe_chk);
	}

	{
		// container
		//this_type& operator=(const base_type& right);
		unsafe_cont_type cont_unsafe(arr1 + 0, arr1 + 10);
		safe_cont_type cont_safe(arr2 + 0, arr2 + 10);

		// !!!the operator= styple only supports the first index !!!!
		cont_safe = cont_unsafe.template get<0>();

		yggr_test_assert(cont_safe == cont_unsafe);
	}

	{
		// container
		//inline this_type& operator=(BOOST_RV_REF(base_type) right);
		unsafe_cont_type cont_unsafe(arr1 + 0, arr1 + 10);
		safe_cont_type cont_safe(arr2 + 0, arr2 + 10);
		unsafe_cont_type cont_unsafe_chk = cont_unsafe;

		cont_safe = boost::move(cont_unsafe);
		yggr_test_assert(cont_safe == cont_unsafe_chk);
	}

	{
		// container
		//this_type& operator=(const base_type& right);
		unsafe_cont_type cont_unsafe(arr1 + 0, arr1 + 10);
		safe_cont_type cont_safe(arr2 + 0, arr2 + 10);

		cont_safe = cont_unsafe;

		yggr_test_assert(cont_safe == cont_unsafe);
	}

	{
		// container this other
		//inline this_type& operator=(BOOST_RV_REF(this_type) right);

		safe_cont_type cont_safe1(arr1 + 0, arr1 + 10);
		safe_cont_type cont_safe2(arr2 + 0, arr2 + 10);
		safe_cont_type cont_safe_chk(arr1 + 0, arr1 + 10);

		cont_safe2 = boost::move(cont_safe1);

		yggr_test_assert(cont_safe2 == cont_safe_chk);
		yggr_test_assert(cont_safe1 != cont_safe_chk);
	}

	{
		// container this other
		//this_type& operator=(const this_type& right);
		safe_cont_type cont_safe1(arr1 + 0, arr1 + 10);
		safe_cont_type cont_safe2(arr2 + 0, arr2 + 10);

		cont_safe2 = cont_safe1;

		yggr_test_assert(cont_safe2 == cont_safe1);
	}

	{
		// container this this
		//inline this_type& operator=(BOOST_RV_REF(this_type) right);
		safe_cont_type cont1(arr1 + 0, arr1 + 10);

		cont1 = boost::move(cont1);

		yggr_test_assert(cont1.size());
		yggr_test_assert(cont1 == cont1);
	}


	{
		// container this this
		//this_type& operator=(const this_type& right);
		safe_cont_type cont1(arr1 + 0, arr1 + 10);

		cont1 = cont1;

		yggr_test_assert(cont1.size());
		yggr_test_assert(cont1 == cont1);
	}

	{
		// index
		//inline this_type& operator=(BOOST_RV_REF(base_type) right);

		safe_cont_type cont_safe;
		unsafe_cont_type cont_unsafe(arr1 + 0, arr1 + 10);
		safe_cont_type cont_safe_chk(arr1 + 0, arr1 + 10);

		cont_safe.template get<0>() = boost::move(cont_unsafe.template get<0>());

		yggr_test_assert(cont_safe == cont_safe_chk);
	}

	{
		// index
		//this_type& operator=(const base_type& right);

		safe_cont_type cont_safe;
		unsafe_cont_type cont_unsafe(arr1 + 0, arr1 + 10);

		cont_safe.template get<0>() = cont_unsafe.template get<0>();

		yggr_test_assert(!cont_unsafe.empty());
		yggr_test_assert(cont_safe == cont_unsafe);
	}

	{
		// index
		//inline this_type& operator=(BOOST_RV_REF(final_type) right);

		safe_cont_type cont_safe;
		unsafe_cont_type cont_unsafe(arr1 + 0, arr1 + 10);
		safe_cont_type cont_safe_chk(arr1 + 0, arr1 + 10);

		cont_safe.template get<0>() = boost::move(cont_unsafe);
		//cont_safe.get<1>() = boost::move(cont_unsafe);

		yggr_test_assert(cont_safe == cont_safe_chk);
	}

	{
		// index
		//this_type& operator=(const final_type& right);

		safe_cont_type cont_safe;
		unsafe_cont_type cont_unsafe(arr1 + 0, arr1 + 10);

		cont_safe.template get<0>() = cont_unsafe;
		cont_safe.template get<1>() = cont_unsafe;

		yggr_test_assert(!cont_unsafe.empty());
		yggr_test_assert(cont_safe == cont_unsafe);
	}

	{
		// index this other
		//inline this_type& operator=(BOOST_RV_REF(this_type) right);

		safe_cont_type cont_safe1;
		safe_cont_type cont_safe2(arr1 + 0, arr1 + 10);
		safe_cont_type cont_safe_chk(arr1 + 0, arr1 + 10);

		cont_safe1.template get<0>() = boost::move(cont_safe2.template get<0>());
		//cont_safe1.get<1>() = boost::move(cont_safe2.get<1>());

		yggr_test_assert(cont_safe1 == cont_safe_chk);
	}

	{
		// index this other
		//this_type& operator=(const this_type& right);

		safe_cont_type cont_safe1;
		safe_cont_type cont_safe2(arr1 + 0, arr1 + 10);

		cont_safe1.template get<0>() = cont_safe2.template get<0>();
		cont_safe1.template get<1>() = cont_safe2.template get<1>();

		yggr_test_assert(!cont_safe1.empty());
		yggr_test_assert(cont_safe1 == cont_safe2);
	}

	{
		// index this this
		//inline this_type& operator=(BOOST_RV_REF(this_type) right);

		safe_cont_type cont_safe(arr1 + 0, arr1 + 10);
		safe_cont_type cont_safe_chk(arr1 + 0, arr1 + 10);

		cont_safe.template get<0>() = boost::move(cont_safe.template get<0>());
		//cont_safe.get<1>() = boost::move(cont_safe.get<1>());

		yggr_test_assert(!cont_safe.empty());
		yggr_test_assert(cont_safe == cont_safe_chk);
	}

	{
		// index this this
		//this_type& operator=(const this_type& right);

		safe_cont_type cont_safe(arr1 + 0, arr1 + 10);
		safe_cont_type cont_safe_chk(arr1 + 0, arr1 + 10);

		cont_safe.template get<0>() = cont_safe.template get<0>();
		cont_safe.template get<1>() = cont_safe.template get<1>();

		yggr_test_assert(!cont_safe.empty());
		yggr_test_assert(cont_safe == cont_safe_chk);
	}

	{
		// index this other
		//this_type& operator=(BOOST_RV_REF(safe_multi_index_containe_this) right);

		safe_cont_type cont_safe1;
		safe_cont_type cont_safe2(arr1 + 0, arr1 + 10);
		safe_cont_type cont_safe_chk(arr1 + 0, arr1 + 10);

		cont_safe1.template get<0>() = boost::move(cont_safe2);

		yggr_test_assert(cont_safe1 == cont_safe_chk);
	}

	{
		// index this other
		//this_type& operator=(const safe_multi_index_containe_this& right);

		safe_cont_type cont_safe1;
		safe_cont_type cont_safe2(arr1 + 0, arr1 + 10);

		cont_safe1.template get<0>() = cont_safe2;

		yggr_test_assert(!cont_safe2.empty());
		yggr_test_assert(cont_safe1 == cont_safe2);
	}

	{
		// index this this
		//this_type& operator=(BOOST_RV_REF(safe_multi_index_containe_this) right);

		safe_cont_type cont_safe(arr1 + 0, arr1 + 10);
		safe_cont_type cont_safe_chk(arr1 + 0, arr1 + 10);

		cont_safe.template get<0>() = boost::move(cont_safe);

		yggr_test_assert(!cont_safe.empty());
		yggr_test_assert(cont_safe == cont_safe_chk);
	}

	{
		// index this this
		//this_type& operator=(const safe_multi_index_containe_this& right);

		safe_cont_type cont_safe(arr1 + 0, arr1 + 10);
		safe_cont_type cont_safe_chk(arr1 + 0, arr1 + 10);

		cont_safe.template get<0>() = cont_safe;

		yggr_test_assert(!cont_safe.empty());
		yggr_test_assert(cont_safe == cont_safe_chk);
	}


#ifndef BOOST_NO_CXX11_HDR_INITIALIZER_LIST
	{
		//this_type& operator=(std::initializer_list<value_type> list);
		safe_cont_type cont_safe;
		safe_cont_type cont_safe_chk(arr1 + 0, arr1 + 4);

		cont_safe.template get<0>() = {A(1, 1, "aaa"), A(2, 2, "bbb"), A(3, 3, "ccc"), A(4, 3, "ddd")};

		yggr_test_assert(cont_safe.size() == 4);
	}
#endif // BOOST_NO_CXX11_HDR_INITIALIZER_LIST

}

template<typename Container>
void test_compare_of_equal_to_and_not_equal_to(void)
{
	typedef Container safe_cont_type;
	typedef typename safe_cont_type::base_type unsafe_cont_type;

	A arr1[10] = {A(1, 1, "aaa"), A(2, 2, "bbb"), A(3, 3, "ccc"), A(4, 3, "ddd")};
	A arr2[10] = {A(6, 6, "aaa"), A(7, 7, "bbb"), A(8, 8, "ccc"), A(9, 8, "ddd")};

	safe_cont_type safe_cont11(arr1 + 0, arr1 + 10);
	safe_cont_type safe_cont12(arr1 + 0, arr1 + 10);

	safe_cont_type safe_cont21(arr2 + 0, arr2 + 10);
	safe_cont_type safe_cont22(arr2 + 0, arr2 + 10);

	unsafe_cont_type unsafe_cont11(arr1 + 0, arr1 + 10);
	unsafe_cont_type unsafe_cont12(arr1 + 0, arr1 + 10);

	unsafe_cont_type unsafe_cont21(arr2 + 0, arr2 + 10);
	unsafe_cont_type unsafe_cont22(arr2 + 0, arr2 + 10);

	// safe_this op safe_this
	yggr_test_assert(safe_cont11 == safe_cont11);
	yggr_test_assert(safe_cont11 != safe_cont21);

	// safe_this op safe_this_index
	yggr_test_assert(safe_cont11 == safe_cont11.template get<0>());
	yggr_test_assert(safe_cont11 == safe_cont11.template get<1>());
	yggr_test_assert(safe_cont11 != safe_cont21.template get<0>());
	yggr_test_assert(safe_cont11 != safe_cont21.template get<1>());

	//safe_this_index op safe_this
	yggr_test_assert(safe_cont11.template get<0>() == safe_cont11);
	yggr_test_assert(safe_cont11.template get<1>() == safe_cont11);
	yggr_test_assert(safe_cont21.template get<0>() != safe_cont11);
	yggr_test_assert(safe_cont21.template get<1>() != safe_cont11);

	// safe op unsafe
	yggr_test_assert(safe_cont11 == unsafe_cont11);
	yggr_test_assert(unsafe_cont11 == safe_cont11);
	yggr_test_assert(safe_cont11 != unsafe_cont21);
	yggr_test_assert(unsafe_cont11 != safe_cont21);


	// !!! safe_cont11 == unsafe_cont11.get<0>() only supports the first index !!!
	// safe op unsafe_index_0
	yggr_test_assert(safe_cont11 == unsafe_cont11.template get<0>());
	yggr_test_assert(safe_cont11 != unsafe_cont21.template get<0>());

	// unsafe_index op safe
	yggr_test_assert(unsafe_cont11.template get<0>() == safe_cont11);
	yggr_test_assert(unsafe_cont21.template get<0>() != safe_cont11);

	// safe op safe_index
	yggr_test_assert(safe_cont11 == safe_cont12.template get<0>());
	yggr_test_assert(safe_cont11 == safe_cont12.template get<1>());
	yggr_test_assert(safe_cont11 != safe_cont21.template get<0>());
	yggr_test_assert(safe_cont11 != safe_cont21.template get<1>());

	//safe_index op safe_index
	yggr_test_assert(safe_cont11.template get<0>() == safe_cont12.template get<0>());
	yggr_test_assert(safe_cont11.template get<1>() == safe_cont12.template get<1>());
	yggr_test_assert(safe_cont11.template get<0>() != safe_cont21.template get<0>());
	yggr_test_assert(safe_cont11.template get<1>() != safe_cont21.template get<1>());

	//safe_index op unsafe_index
	yggr_test_assert(safe_cont11.template get<0>() == unsafe_cont12.template get<0>());
	yggr_test_assert(safe_cont11.template get<1>() == unsafe_cont12.template get<1>());
	yggr_test_assert(safe_cont11.template get<0>() != unsafe_cont21.template get<0>());
	yggr_test_assert(safe_cont11.template get<1>() != unsafe_cont21.template get<1>());

	//unsafe_index op safe_index
	yggr_test_assert(unsafe_cont11.template get<0>() == safe_cont12.template get<0>());
	yggr_test_assert(unsafe_cont11.template get<1>() == safe_cont12.template get<1>());
	yggr_test_assert(unsafe_cont11.template get<0>() != safe_cont21.template get<0>());
	yggr_test_assert(unsafe_cont11.template get<1>() != safe_cont21.template get<1>());
}

template<typename Container>
void test_compare_of_full(void)
{
	test_compare_of_equal_to_and_not_equal_to<Container>();

	typedef Container safe_cont_type;
	typedef typename safe_cont_type::base_type unsafe_cont_type;

	A arr1[10] = {A(1, 1, "aaa"), A(2, 2, "bbb"), A(3, 3, "ccc"), A(4, 3, "ddd")};
	A arr2[10] = {A(6, 6, "aaa"), A(7, 7, "bbb"), A(8, 8, "ccc"), A(9, 8, "ddd")};

	safe_cont_type safe_cont11(arr1 + 0, arr1 + 10);
	safe_cont_type safe_cont12(arr1 + 0, arr1 + 10);

	safe_cont_type safe_cont21(arr2 + 0, arr2 + 10);
	safe_cont_type safe_cont22(arr2 + 0, arr2 + 10);

	unsafe_cont_type unsafe_cont11(arr1 + 0, arr1 + 10);
	unsafe_cont_type unsafe_cont12(arr1 + 0, arr1 + 10);

	unsafe_cont_type unsafe_cont21(arr2 + 0, arr2 + 10);
	unsafe_cont_type unsafe_cont22(arr2 + 0, arr2 + 10);

	// <
	yggr_test_assert(!(safe_cont11 < safe_cont11));
	yggr_test_assert(!(safe_cont11 < safe_cont12));
	yggr_test_assert(safe_cont11 < safe_cont21);

	yggr_test_assert(!(safe_cont11 < unsafe_cont11));
	yggr_test_assert(!(safe_cont11 < unsafe_cont12));
	yggr_test_assert(safe_cont11 < unsafe_cont21);

	yggr_test_assert(!(unsafe_cont11 < safe_cont11));
	yggr_test_assert(!(unsafe_cont11 < safe_cont12));
	yggr_test_assert(unsafe_cont11 < safe_cont21);

	//!!! the style only supports the first index !!!
	yggr_test_assert(!(safe_cont11 < unsafe_cont11.template get<0>()));
	yggr_test_assert(!(safe_cont11 < unsafe_cont12.template get<0>()));
	yggr_test_assert(safe_cont11 < unsafe_cont21.template get<0>());

	yggr_test_assert(!(unsafe_cont11.template get<0>() < safe_cont11));
	yggr_test_assert(!(unsafe_cont11.template get<0>() < safe_cont12));
	yggr_test_assert(unsafe_cont11.template get<0>() < safe_cont21);

	yggr_test_assert(!(safe_cont11.template get<0>() < safe_cont11.template get<0>()));
	yggr_test_assert(!(safe_cont11.template get<1>() < safe_cont11.template get<1>()));
	yggr_test_assert(!(safe_cont11.template get<0>() < safe_cont12.template get<0>()));
	yggr_test_assert(!(safe_cont11.template get<1>() < safe_cont12.template get<1>()));
	yggr_test_assert(safe_cont11.template get<0>() < safe_cont21.template get<0>());
	yggr_test_assert(safe_cont11.template get<1>() < safe_cont21.template get<1>());

	yggr_test_assert(!(safe_cont11.template get<0>() < unsafe_cont11.template get<0>()));
	yggr_test_assert(!(safe_cont11.template get<1>() < unsafe_cont11.template get<1>()));
	yggr_test_assert(!(safe_cont11.template get<0>() < unsafe_cont12.template get<0>()));
	yggr_test_assert(!(safe_cont11.template get<1>() < unsafe_cont12.template get<1>()));
	yggr_test_assert(safe_cont11.template get<0>() < unsafe_cont21.template get<0>());
	yggr_test_assert(safe_cont11.template get<1>() < unsafe_cont21.template get<1>());

	yggr_test_assert(!(unsafe_cont11.template get<0>() < safe_cont11.template get<0>()));
	yggr_test_assert(!(unsafe_cont11.template get<1>() < safe_cont11.template get<1>()));
	yggr_test_assert(!(unsafe_cont11.template get<0>() < safe_cont12.template get<0>()));
	yggr_test_assert(!(unsafe_cont11.template get<1>() < safe_cont12.template get<1>()));
	yggr_test_assert(unsafe_cont11.template get<0>() < safe_cont21.template get<0>());
	yggr_test_assert(unsafe_cont11.template get<1>() < safe_cont21.template get<1>());

	// <=
	yggr_test_assert(safe_cont11 <= safe_cont11);
	yggr_test_assert(safe_cont11 <= safe_cont12);
	yggr_test_assert(safe_cont11 <= safe_cont21);

	yggr_test_assert(safe_cont11 <= unsafe_cont11);
	yggr_test_assert(safe_cont11 <= unsafe_cont12);
	yggr_test_assert(safe_cont11 <= unsafe_cont21);

	yggr_test_assert(unsafe_cont11 <= safe_cont11);
	yggr_test_assert(unsafe_cont11 <= safe_cont12);
	yggr_test_assert(unsafe_cont11 <= safe_cont21);

	//!!! the style only supports the first index !!!
	yggr_test_assert(safe_cont11 <= unsafe_cont11.template get<0>());
	yggr_test_assert(safe_cont11 <= unsafe_cont12.template get<0>());
	yggr_test_assert(safe_cont11 <= unsafe_cont21.template get<0>());

	yggr_test_assert(unsafe_cont11.template get<0>() <= safe_cont11);
	yggr_test_assert(unsafe_cont11.template get<0>() <= safe_cont12);
	yggr_test_assert(unsafe_cont11.template get<0>() <= safe_cont21);

	yggr_test_assert(safe_cont11.template get<0>() <= safe_cont11.template get<0>());
	yggr_test_assert(safe_cont11.template get<1>() <= safe_cont11.template get<1>());
	yggr_test_assert(safe_cont11.template get<0>() <= safe_cont12.template get<0>());
	yggr_test_assert(safe_cont11.template get<1>() <= safe_cont12.template get<1>());
	yggr_test_assert(safe_cont11.template get<0>() <= safe_cont21.template get<0>());
	yggr_test_assert(safe_cont11.template get<1>() <= safe_cont21.template get<1>());

	yggr_test_assert(safe_cont11.template get<0>() <= unsafe_cont11.template get<0>());
	yggr_test_assert(safe_cont11.template get<1>() <= unsafe_cont11.template get<1>());
	yggr_test_assert(safe_cont11.template get<0>() <= unsafe_cont12.template get<0>());
	yggr_test_assert(safe_cont11.template get<1>() <= unsafe_cont12.template get<1>());
	yggr_test_assert(safe_cont11.template get<0>() <= unsafe_cont21.template get<0>());
	yggr_test_assert(safe_cont11.template get<1>() <= unsafe_cont21.template get<1>());

	yggr_test_assert(unsafe_cont11.template get<0>() <= safe_cont11.template get<0>());
	yggr_test_assert(unsafe_cont11.template get<1>() <= safe_cont11.template get<1>());
	yggr_test_assert(unsafe_cont11.template get<0>() <= safe_cont12.template get<0>());
	yggr_test_assert(unsafe_cont11.template get<1>() <= safe_cont12.template get<1>());
	yggr_test_assert(unsafe_cont11.template get<0>() <= safe_cont21.template get<0>());
	yggr_test_assert(unsafe_cont11.template get<1>() <= safe_cont21.template get<1>());

	// >

	yggr_test_assert(!(safe_cont11 > safe_cont11));
	yggr_test_assert(!(safe_cont11 > safe_cont12));
	yggr_test_assert(safe_cont21 > safe_cont11);

	yggr_test_assert(!(safe_cont11 > unsafe_cont11));
	yggr_test_assert(!(safe_cont11 > unsafe_cont12));
	yggr_test_assert(safe_cont21 > unsafe_cont11);

	yggr_test_assert(!(unsafe_cont11 > safe_cont11));
	yggr_test_assert(!(unsafe_cont11 > safe_cont12));
	yggr_test_assert(unsafe_cont21 > safe_cont11);

	//!!! the style only supports the first index !!!
	yggr_test_assert(!(safe_cont11 > unsafe_cont11.template get<0>()));
	yggr_test_assert(!(safe_cont11 > unsafe_cont12.template get<0>()));
	yggr_test_assert(safe_cont21 > unsafe_cont11.template get<0>());

	yggr_test_assert(!(unsafe_cont11.template get<0>() > safe_cont11));
	yggr_test_assert(!(unsafe_cont11.template get<0>() > safe_cont12));
	yggr_test_assert(unsafe_cont21.template get<0>() > safe_cont11);

	yggr_test_assert(!(safe_cont11.template get<0>() > safe_cont11.template get<0>()));
	yggr_test_assert(!(safe_cont11.template get<1>() > safe_cont11.template get<1>()));
	yggr_test_assert(!(safe_cont11.template get<0>() > safe_cont12.template get<0>()));
	yggr_test_assert(!(safe_cont11.template get<1>() > safe_cont12.template get<1>()));
	yggr_test_assert(safe_cont21.template get<0>() > safe_cont11.template get<0>());
	yggr_test_assert(safe_cont21.template get<1>() > safe_cont11.template get<1>());

	yggr_test_assert(!(safe_cont11.template get<0>() > unsafe_cont11.template get<0>()));
	yggr_test_assert(!(safe_cont11.template get<1>() > unsafe_cont11.template get<1>()));
	yggr_test_assert(!(safe_cont11.template get<0>() > unsafe_cont12.template get<0>()));
	yggr_test_assert(!(safe_cont11.template get<1>() > unsafe_cont12.template get<1>()));
	yggr_test_assert(safe_cont21.template get<0>() > unsafe_cont11.template get<0>());
	yggr_test_assert(safe_cont21.template get<1>() > unsafe_cont11.template get<1>());

	yggr_test_assert(!(unsafe_cont11.template get<0>() > safe_cont11.template get<0>()));
	yggr_test_assert(!(unsafe_cont11.template get<1>() > safe_cont11.template get<1>()));
	yggr_test_assert(!(unsafe_cont11.template get<0>() > safe_cont12.template get<0>()));
	yggr_test_assert(!(unsafe_cont11.template get<1>() > safe_cont12.template get<1>()));
	yggr_test_assert(unsafe_cont21.template get<0>() > safe_cont11.template get<0>());
	yggr_test_assert(unsafe_cont21.template get<1>() > safe_cont11.template get<1>());


	// >=
	yggr_test_assert(safe_cont11 >= safe_cont11);
	yggr_test_assert(safe_cont11 >= safe_cont12);
	yggr_test_assert(safe_cont21 >= safe_cont11);

	yggr_test_assert(safe_cont11 >= unsafe_cont11);
	yggr_test_assert(safe_cont11 >= unsafe_cont12);
	yggr_test_assert(safe_cont21 >= unsafe_cont11);

	yggr_test_assert(unsafe_cont11 >= safe_cont11);
	yggr_test_assert(unsafe_cont11 >= safe_cont12);
	yggr_test_assert(unsafe_cont21 >= safe_cont11);

	//!!! the style only supports the first index !!!
	yggr_test_assert(safe_cont11 >= unsafe_cont11.template get<0>());
	yggr_test_assert(safe_cont11 >= unsafe_cont12.template get<0>());
	yggr_test_assert(safe_cont21 >= unsafe_cont11.template get<0>());

	yggr_test_assert(unsafe_cont11.template get<0>() >= safe_cont11);
	yggr_test_assert(unsafe_cont11.template get<0>() >= safe_cont12);
	yggr_test_assert(unsafe_cont21.template get<0>() >= safe_cont11);

	yggr_test_assert(safe_cont11.template get<0>() >= safe_cont11.template get<0>());
	yggr_test_assert(safe_cont11.template get<1>() >= safe_cont11.template get<1>());
	yggr_test_assert(safe_cont11.template get<0>() >= safe_cont12.template get<0>());
	yggr_test_assert(safe_cont11.template get<1>() >= safe_cont12.template get<1>());
	yggr_test_assert(safe_cont21.template get<0>() >= safe_cont11.template get<0>());
	yggr_test_assert(safe_cont21.template get<1>() >= safe_cont11.template get<1>());

	yggr_test_assert(safe_cont11.template get<0>() >= unsafe_cont11.template get<0>());
	yggr_test_assert(safe_cont11.template get<1>() >= unsafe_cont11.template get<1>());
	yggr_test_assert(safe_cont11.template get<0>() >= unsafe_cont12.template get<0>());
	yggr_test_assert(safe_cont11.template get<1>() >= unsafe_cont12.template get<1>());
	yggr_test_assert(safe_cont21.template get<0>() >= unsafe_cont11.template get<0>());
	yggr_test_assert(safe_cont21.template get<1>() >= unsafe_cont11.template get<1>());

	yggr_test_assert(unsafe_cont11.template get<0>() >= safe_cont11.template get<0>());
	yggr_test_assert(unsafe_cont11.template get<1>() >= safe_cont11.template get<1>());
	yggr_test_assert(unsafe_cont11.template get<0>() >= safe_cont12.template get<0>());
	yggr_test_assert(unsafe_cont11.template get<1>() >= safe_cont12.template get<1>());
	yggr_test_assert(unsafe_cont21.template get<0>() >= safe_cont11.template get<0>());
	yggr_test_assert(unsafe_cont21.template get<1>() >= safe_cont11.template get<1>());

}

template<typename Val, typename ModVal>
void modifier(Val& left, const ModVal& right)
{
	left = right;
}

template<typename Val, typename ModeVal>
void modifier(Val& left, const ModeVal& right, Val& tmp)
{
	tmp = left;
	left = right;
}

template<typename Val>
void rollback(Val& left, const Val& tmp)
{
	left = tmp;
}

bool less_foo(const A& l, const A& r)
{
	return l.prm1 < r.prm1;
};

bool greater_foo(const A& l, const A& r)
{
	return l.prm1 > r.prm1;
};


template<typename Base>
bool handler_using_handler(Base& base, int chk_size)
{
	return base.size() == chk_size;
}

template<typename T>
void handler_for_each(T iter, int& count)
{
	++count;
}


template<typename T>
bool handler_filter(T iter, bool bfilter)
{
	return bfilter;
}

template<typename Index, typename IndexID>
const typename Index::value_type&
	handler_value_of_begin(Index& base, IndexID)
{
	yggr_test_assert(!base.template get<1>().empty());
	return *base.template get<1>().begin();
}

template<typename Base, typename IndexID, typename Iterator>
bool handler_iterator_to(Base& base,
							boost::fusion::pair
							<
								IndexID,
								Iterator
							> rst)
{
	return rst.second == base.template get<0>().begin();
}

template<typename Base, typename IndexID, typename Iterator>
bool handler_erase(Base& base,
					boost::fusion::pair
					<
						IndexID,
						std::pair<Iterator, bool>
					> rst, bool chk)
{
	return rst.second.second == chk;
}

template<typename Base>
std::pair<typename Base::iterator, typename Base::iterator>
	begin_and_end_iter(Base& base)
{
	return std::make_pair(base.begin(), base.end());
}


template<typename Container>
void test_safe_other_gernel(void)
{
	typedef Container mi_cont_type;
	typedef typename mi_cont_type::base_type unsafe_midx_type;

	typedef typename boost::multi_index::nth_index<mi_cont_type, 0>::type safe_index0_type;
	typedef typename safe_index0_type::index_id_type index_id0_type;
	typedef typename safe_index0_type::iterator iterator0_type;

	typedef typename boost::multi_index::nth_index<mi_cont_type, 1>::type safe_index1_type;
	typedef typename safe_index1_type::index_id_type index_id1_type;
	typedef typename safe_index1_type::iterator iterator1_type;

	A arr[10] = {A(1, 1, "aaa"), A(2, 2, "bbb"), A(3, 3, "ccc"), A(4, 3, "ddd")};

	mi_cont_type cont(arr + 0, arr + 10);

	yggr_test_assert(cont.iterator_to(boost::bind(&handler_value_of_begin<unsafe_midx_type, index_id0_type>, _1, _2),
							boost::bind(&handler_iterator_to<unsafe_midx_type, index_id0_type, iterator0_type>, _1, _2)));


	// operator final_type
	unsafe_midx_type unsafe_cont = cont;

	yggr_test_assert(unsafe_cont == cont);

	unsafe_cont = cont;
	yggr_test_assert(unsafe_cont == cont);

	// erase iterator
	{
		unsafe_midx_type tmp(arr + 0, arr + 10);

		mi_cont_type cont(tmp);

		yggr_test_assert(cont.size() == tmp.size());

		yggr_test_assert(cont.erase(boost::bind(&safe_index0_type::base_type::cbegin, _1)));

		yggr_test_assert(cont.size() == tmp.size() - 1);

		yggr_test_assert(cont.erase(boost::bind(&safe_index0_type::base_type::cbegin, _1),
							boost::bind(&safe_index0_type::base_type::cend, _1)));
		yggr_test_assert(cont.empty());

		cont = tmp;
		yggr_test_assert(cont.size() == tmp.size());

		yggr_test_assert(cont.erase(boost::bind(&safe_index0_type::base_type::cbegin, _1),
							boost::bind(&handler_erase<unsafe_midx_type, index_id0_type, iterator0_type>, _1, _2, true)));

		yggr_test_assert(cont.size() == tmp.size() - 1);

		yggr_test_assert(cont.erase(boost::bind(&safe_index0_type::base_type::cbegin, _1),
							boost::bind(&safe_index0_type::base_type::cend, _1),
							boost::bind(&handler_erase<unsafe_midx_type, index_id0_type, iterator0_type>, _1, _2, true)));

		yggr_test_assert(cont.empty());

		cont = tmp;
		yggr_test_assert(cont.size() == tmp.size());

		yggr_test_assert(cont.erase(boost::bind(&begin_and_end_iter<typename safe_index0_type::base_type>, _1)));

		yggr_test_assert(cont.empty());

		cont = tmp;
		yggr_test_assert(cont.size() == tmp.size());

		yggr_test_assert(cont.erase(boost::bind(&begin_and_end_iter<typename safe_index0_type::base_type>, _1),
							boost::bind(&handler_erase<unsafe_midx_type, index_id0_type, iterator0_type>, _1, _2, true)));

		yggr_test_assert(cont.empty());
	}

	{
		unsafe_midx_type tmp;
		cont.copy_to_base(tmp);
		yggr_test_assert(tmp == cont);
	}

	{
		unsafe_midx_type tmp;
		cont.copy_to_base(tmp.template get<0>());
		yggr_test_assert(tmp == cont);
	}

	{
		unsafe_midx_type tmp;
		tmp = cont.load();
		yggr_test_assert(tmp == cont);

		tmp = cont.template get<0>().load();
		yggr_test_assert(tmp == cont);

		tmp = cont.template get<1>().load();
		yggr_test_assert(tmp == cont);
	}

	{
		unsafe_midx_type store_cont(arr + 0, arr + 10);
		mi_cont_type tmp;

		tmp.store(store_cont);
		yggr_test_assert(tmp == store_cont);

		tmp.store(store_cont.template get<0>());
		yggr_test_assert(tmp == store_cont);
	}

	yggr_test_assert(cont.using_handler(boost::bind(&handler_using_handler<typename mi_cont_type::base_type>, _1, cont.size())));
	yggr_test_assert(cont.template using_handler<0>(boost::bind(&handler_using_handler<typename safe_index0_type::base_type>, _1, cont.size())));

	yggr_test_assert(cont.template get<0>().using_handler(boost::bind(&handler_using_handler<typename safe_index0_type::base_type>, _1, cont.size())));
	yggr_test_assert(cont.template get<0>().using_handler_final(boost::bind(&handler_using_handler<typename mi_cont_type::base_type>, _1, cont.size())));

	{
		int call_count = 0;
		cont.for_each_iterator(boost::bind(&handler_for_each<iterator0_type>, _1, boost::ref(call_count)));
		yggr_test_assert(call_count == cont.size());

		call_count = 0;
		cont.for_each_iterator(boost::bind(&handler_for_each<iterator0_type>, _1, boost::ref(call_count)),
								boost::bind(&handler_filter<iterator0_type>, _1, false));
		yggr_test_assert(call_count == 0);

		call_count = 0;
		cont.for_each_iterator(boost::bind(&handler_for_each<iterator0_type>, _1, boost::ref(call_count)),
								boost::bind(&handler_filter<iterator0_type>, _1, true));
		yggr_test_assert(call_count == cont.size());
	}

	{
		int call_count = 0;
		cont.for_each(boost::bind(&handler_for_each<const typename mi_cont_type::value_type&>, _1, boost::ref(call_count)));
		yggr_test_assert(call_count == cont.size());

		call_count = 0;
		cont.for_each(boost::bind(&handler_for_each<const typename mi_cont_type::value_type&>, _1, boost::ref(call_count)),
								boost::bind(&handler_filter<const typename mi_cont_type::value_type&>, _1, false));
		yggr_test_assert(call_count == 0);

		call_count = 0;
		cont.for_each(boost::bind(&handler_for_each<const typename mi_cont_type::value_type&>, _1, boost::ref(call_count)),
								boost::bind(&handler_filter<const typename mi_cont_type::value_type&>, _1, true));
		yggr_test_assert(call_count == cont.size());
	}
}

//--------------------hash------------------------------------------

void test_construct_of_hash(void)
{
	typedef yggr::safe_container::safe_multi_index_container<
			A,
			boost::multi_index::indexed_by<
				boost::multi_index::hashed_unique<
					boost::multi_index::tag<A::tag_prm1>,
					BOOST_MULTI_INDEX_MEMBER(A, int, prm1) >,

				boost::multi_index::hashed_non_unique<
					boost::multi_index::tag<A::tag_prm2>,
					BOOST_MULTI_INDEX_MEMBER(A, int, prm2)>
			>
		> mi_cont_type;

	test_construct<mi_cont_type>();

	std::cout << "---------------test_construct_of_hash end-------------------------" << std::endl;
}

void test_observers_of_hash(void)
{
	typedef yggr::safe_container::safe_multi_index_container<
			A,
			boost::multi_index::indexed_by<
				boost::multi_index::hashed_unique<
					boost::multi_index::tag<A::tag_prm1, A::tag_prm3>,
					BOOST_MULTI_INDEX_MEMBER(A, int, prm1) >,

				boost::multi_index::hashed_non_unique<
					boost::multi_index::tag<A::tag_prm2>,
					BOOST_MULTI_INDEX_MEMBER(A, int, prm2)>
			>
		> mi_cont_type;

	A arr[10] = {A(1, 1, "aaa"), A(2, 2, "bbb"), A(3, 3, "ccc"), A(4, 3, "ddd")};
	mi_cont_type cont(arr + 0, arr + 10);
	mi_cont_type cont2(cont.get_allocator());

	cont.get<0>().get_allocator();
	cont.get<1>().get_allocator();

	cont.get<A::tag_prm1>().get_allocator();
	cont.get<A::tag_prm2>().get_allocator();

	cont.get<0>().key_extractor();
	cont.get<1>().key_extractor();

	cont.get<A::tag_prm1>().key_extractor();
	cont.get<A::tag_prm2>().key_extractor();

	cont.get<0>().hash_function();
	cont.get<1>().hash_function();

	cont.get<A::tag_prm1>().hash_function();
	cont.get<A::tag_prm2>().hash_function();

	cont.get<0>().key_eq();
	cont.get<1>().key_eq();

	cont.get<A::tag_prm1>().key_eq();
	cont.get<A::tag_prm2>().key_eq();

	std::cout << "---------------test_observers_of_hash end-------------------------" << std::endl;
}

void test_capacity_of_hash(void)
{
	typedef yggr::safe_container::safe_multi_index_container<
			A,
			boost::multi_index::indexed_by<
				boost::multi_index::hashed_unique<
					boost::multi_index::tag<A::tag_prm1>,
					BOOST_MULTI_INDEX_MEMBER(A, int, prm1) >,

				boost::multi_index::hashed_non_unique<
					boost::multi_index::tag<A::tag_prm2>,
					BOOST_MULTI_INDEX_MEMBER(A, int, prm2)>
			>
		> mi_cont_type;

	A arr[10] = {A(1, 1, "aaa"), A(2, 2, "bbb"), A(3, 3, "ccc"), A(4, 3, "ddd")};

	mi_cont_type cont(arr + 0, arr + 10);
	mi_cont_type::base_type base_cont(arr + 0, arr + 10);
	yggr_test_assert(cont.size() == 5);
	yggr_test_assert(!cont.empty());
	yggr_test_assert(base_cont.size() == cont.size());
	yggr_test_assert(base_cont.max_size() == cont.max_size());

	yggr_test_assert(cont.get<0>().empty() == cont.get<A::tag_prm1>().empty());
	yggr_test_assert(cont.get<0>().empty() == cont.get<A::tag_prm2>().empty());

	yggr_test_assert(cont.get<1>().empty() == cont.get<A::tag_prm1>().empty());
	yggr_test_assert(cont.get<1>().empty() == cont.get<A::tag_prm2>().empty());

	yggr_test_assert(cont.get<0>().size() == cont.get<A::tag_prm1>().size());
	yggr_test_assert(cont.get<0>().size() == cont.get<A::tag_prm2>().size());

	yggr_test_assert(cont.get<1>().size() == cont.get<A::tag_prm1>().size());
	yggr_test_assert(cont.get<1>().size() == cont.get<A::tag_prm2>().size());

	yggr_test_assert(cont.get<0>().max_size() == cont.get<A::tag_prm1>().max_size());
	yggr_test_assert(cont.get<0>().max_size() == cont.get<A::tag_prm2>().max_size());

	yggr_test_assert(cont.get<1>().max_size() == cont.get<A::tag_prm1>().max_size());
	yggr_test_assert(cont.get<1>().max_size() == cont.get<A::tag_prm2>().max_size());

	std::cout << "---------------test_capacity_of_hash end-------------------------" << std::endl;
}

template<typename Final, typename IndexID, typename Iterator>
bool handler_insert_value_of_hash(Final& final,
									const
										boost::fusion::pair
										<
											IndexID,
											std::pair<Iterator, bool>
										>& rst, int& count)
{
	typedef boost::fusion::pair<IndexID, std::pair<Iterator, bool> > rst_type;
	typedef typename rst_type::first_type index_id_type;
	typedef Final midx_type;
	typedef typename
		boost::multi_index::nth_index
		<
			midx_type, index_id_type::first::value
		>::type now_index_type;

	typedef typename
		boost::mpl::if_
		<
			boost::is_const<Final>,
			const now_index_type&,
			now_index_type&
		>::type now_index_ref_type;

	++count;
	if(rst.second.second)
	{
		now_index_ref_type now_index = final.template get<index_id_type::first::value>();

		yggr_test_assert((now_index.find((now_index.key_extractor())(*rst.second.first)) != now_index.end()));
	}
	return rst.second.second;
}

template<typename Final, typename IndexID, typename InsRet>
yggr::safe_container::safe_insert_return_type_base
<
	typename
		yggr::container::node_handle_traits
		<
			typename
				boost::multi_index::nth_index
				<
					Final, IndexID::first::value
				>::type
		>::node_type
>
	handler_insert_node_of_kv(Final& final,
								boost::fusion::pair
								<
									IndexID,
									InsRet
								>& rst)
{
	typedef boost::fusion::pair<IndexID, InsRet > rst_type;
	typedef typename rst_type::first_type index_id_type;
	typedef Final midx_type;
	typedef typename
		boost::multi_index::nth_index
		<
			midx_type, index_id_type::first::value
		>::type now_index_type;

	typedef typename
		boost::mpl::if_
		<
			boost::is_const<Final>,
			const now_index_type&,
			now_index_type&
		>::type now_index_ref_type;

	typedef typename
		yggr::container::node_handle_traits
		<
			now_index_type
		>::node_type node_type;

	typedef yggr::safe_container::safe_insert_return_type_base<node_type> ret_type;

	if(rst.second.inserted)
	{
		now_index_ref_type now_index = final.template get<index_id_type::first::value>();
		yggr_test_assert((now_index.find((now_index.key_extractor())(*rst.second.position)) != now_index.end()));
		return ret_type(boost::move(rst.second));
	}

	return ret_type(boost::move(rst.second));
}

template<typename Final, typename IndexID, typename Iterator>
bool handler_insert_node_of_kv_hint(Final& final,
									const
										boost::fusion::pair
										<
											IndexID,
											Iterator
										>& rst)
{
	typedef boost::fusion::pair<IndexID, Iterator > rst_type;
	typedef typename rst_type::first_type index_id_type;
	typedef Final midx_type;
	typedef typename
		boost::multi_index::nth_index
		<
			midx_type, index_id_type::first::value
		>::type now_index_type;

	typedef typename
		boost::mpl::if_
		<
			boost::is_const<Final>,
			const now_index_type&,
			now_index_type&
		>::type now_index_ref_type;

	if(rst.second != final.template get<index_id_type::first::value>().end())
	{
		now_index_ref_type now_index = final.template get<index_id_type::first::value>();
		yggr_test_assert((now_index.find((now_index.key_extractor())(*rst.second)) != now_index.end()));
		return true;
	}
	else
	{
		return false;
	}
}

void test_modifiers_of_hash(void)
{
	typedef
		yggr::safe_container::safe_multi_index_container<
			A,
			boost::multi_index::indexed_by<
				boost::multi_index::hashed_unique<
					boost::multi_index::tag<A::tag_prm1>,
					BOOST_MULTI_INDEX_MEMBER(A, int, prm1) >,

				boost::multi_index::hashed_non_unique<
					boost::multi_index::tag<A::tag_prm2>,
					BOOST_MULTI_INDEX_MEMBER(A, int, prm2)>
			>
		> mi_cont_type;

	typedef mi_cont_type::base_type unsafe_midx_type;
	typedef boost::multi_index::nth_index<unsafe_midx_type, 0>::type unsafe_hash_unique_type;

	A arr[10] = {A(1, 1, "aaa"), A(2, 2, "bbb"), A(3, 3, "ccc"), A(4, 3, "ddd")};
	mi_cont_type cont(arr + 0, arr + 10);

	yggr_test_assert(cont.size() == 5);

// initializer_list
#ifndef BOOST_NO_CXX11_HDR_INITIALIZER_LIST

	{
		mi_cont_type cont1 = {A(1, 1, "aaa"), A(2, 2, "bbb"), A(3, 3, "ccc"), A(4, 3, "ddd")};
		yggr_test_assert(cont1.size() == 4);
	}

	{
		mi_cont_type cont1;
		cont1.insert({A(1, 1, "aaa"), A(2, 2, "bbb"), A(3, 3, "ccc"), A(4, 3, "ddd")});
		yggr_test_assert(cont1.size() == 4);
	}

#endif // BOOST_NO_CXX11_HDR_INITIALIZER_LIST

	// insert
	yggr_test_assert(cont.get<0>().insert(A(5, 3, "eee")));
	yggr_test_assert(cont.size() == 6);
	yggr_test_assert(!cont.get<A::tag_prm1>().insert(A(3, 5, "cccc")));
	yggr_test_assert(cont.size() == 6);

	A arr2[10] = {A(10, 10, "a10"), A(20, 20, "b20"), A(30, 30, "c30"), A(40, 30, "d40")};
	yggr_test_assert(cont.get<A::tag_prm1>().insert(arr2 + 0, arr2 + 10));
	yggr_test_assert(cont.size() == 10);
	yggr_test_assert(cont.get<0>().insert(arr + 0, arr + 10));
	yggr_test_assert(cont.size() == 10);

	yggr_test_assert(cont.get<1>().insert(A(6, 3, "eee")));
	yggr_test_assert(cont.size() == 11);
	yggr_test_assert(!cont.get<A::tag_prm2>().insert(A(6, 3, "cccc")));
	yggr_test_assert(cont.size() == 11);

	A arr3[10] = {A(11, 11, "a11"), A(21, 21, "b21"), A(31, 31, "c31"), A(41, 31, "d41")};
	yggr_test_assert(cont.get<A::tag_prm2>().insert(arr3, arr3 + 10));
	yggr_test_assert(cont.size() == 15);
	yggr_test_assert(cont.get<1>().insert(arr3, arr3 + 10));
	yggr_test_assert(cont.size() == 15);

	{
		typedef yggr::safe_container::nth_index<mi_cont_type, 0>::type safe_index0_type;
		typedef safe_index0_type::index_id_type index_id0_type;
		typedef safe_index0_type::iterator iterator0_type;


		typedef yggr::safe_container::nth_index<mi_cont_type, 1>::type safe_index1_type;
		typedef safe_index1_type::index_id_type index_id1_type;
		typedef safe_index1_type::iterator iterator1_type;

		mi_cont_type cont;
		int call_count = 0;
		bool bright =
			cont.insert(arr[0],
						boost::bind(&handler_insert_value_of_hash<unsafe_midx_type, index_id0_type, iterator0_type>,
									_1, _2, boost::ref(call_count)));

		yggr_test_assert(call_count == 1);

		A ins_data = arr[1];
		 bright = bright &&
			cont.get<1>().
				insert(boost::move(ins_data),
						boost::bind(&handler_insert_value_of_hash<unsafe_midx_type, index_id1_type, iterator1_type>,
									_1, _2, boost::ref(call_count)));

		 yggr_test_assert(call_count == 2);

		yggr_test_assert(bright);
		yggr_test_assert(cont.size() == 2);

		bright = bright &&
			cont.get<0>().
				insert(arr[0],
						boost::bind(&handler_insert_value_of_hash<unsafe_midx_type, index_id0_type, iterator0_type>,
									_1, _2, boost::ref(call_count)));

		yggr_test_assert(call_count == 3);

		yggr_test_assert(!bright);
		yggr_test_assert(cont.size() == 2);
	}

	{
		typedef yggr::safe_container::nth_index<mi_cont_type, 0>::type safe_index0_type;
		typedef safe_index0_type::index_id_type index_id0_type;
		typedef safe_index0_type::iterator iterator0_type;
		typedef safe_index0_type::const_iterator const_iterator0_type;


		typedef yggr::safe_container::nth_index<mi_cont_type, 1>::type safe_index1_type;
		typedef safe_index1_type::index_id_type index_id1_type;
		typedef safe_index1_type::iterator iterator1_type;
		typedef safe_index1_type::const_iterator const_iterator1_type;





		mi_cont_type cont;
		bool bright =
			cont.insert(boost::bind(&safe_index0_type::base_type::cend, _1), arr[0]);


		A ins_data = arr[1];
		 bright = bright &&
			cont.get<1>().
				insert(boost::bind(&safe_index1_type::base_type::cend, _1),
						boost::move(ins_data));


		yggr_test_assert(bright);
		yggr_test_assert(cont.size() == 2);

		bright = bright &&
			cont.get<0>().insert(boost::bind(&safe_index0_type::base_type::cend, _1), arr[0]);

		yggr_test_assert(bright);
	}

	{
		typedef yggr::safe_container::nth_index<mi_cont_type, 0>::type safe_index0_type;
		typedef safe_index0_type::index_id_type index_id0_type;
		typedef safe_index0_type::iterator iterator0_type;


		typedef yggr::safe_container::nth_index<mi_cont_type, 1>::type safe_index1_type;
		typedef safe_index1_type::index_id_type index_id1_type;
		typedef safe_index1_type::iterator iterator1_type;

		mi_cont_type cont;
		int call_count = 0;
		bool bright =
			cont.insert(boost::bind(&safe_index0_type::base_type::cend, _1),
						arr[0],
						boost::bind(&handler_insert_value_of_hash<unsafe_midx_type, index_id0_type, iterator0_type>,
									_1, _2, boost::ref(call_count)));

		yggr_test_assert(call_count == 1);

		A ins_data = arr[1];
		 bright = bright &&
			cont.get<1>().
				insert(boost::bind(&safe_index1_type::base_type::cend, _1),
						boost::move(ins_data),
						boost::bind(&handler_insert_value_of_hash<unsafe_midx_type, index_id1_type, iterator1_type>,
									_1, _2, boost::ref(call_count)));

		yggr_test_assert(call_count == 2);

		yggr_test_assert(bright);
		yggr_test_assert(cont.size() == 2);

		bright = bright &&
			cont.get<0>().
				insert(boost::bind(&safe_index0_type::base_type::cend, _1),
						arr[0],
						boost::bind(&handler_insert_value_of_hash<unsafe_midx_type, index_id0_type, iterator0_type>,
									_1, _2, boost::ref(call_count)));

		yggr_test_assert(call_count == 3);
		yggr_test_assert(bright);
		yggr_test_assert(cont.size() == 2);
	}

	// extract and insert
	{
		typedef yggr::safe_container::nth_index<mi_cont_type, 0>::type safe_index0_type;
		typedef safe_index0_type::index_id_type index_id0_type;
		typedef safe_index0_type::iterator iterator0_type;
		typedef safe_index0_type::node_type node0_type;
		typedef safe_index0_type::insert_return_type insert_return0_type;
		typedef safe_index0_type::base_insert_return_type base_insert_return0_type;

		typedef yggr::safe_container::nth_index<mi_cont_type, 1>::type safe_index1_type;
		typedef safe_index1_type::index_id_type index_id1_type;
		typedef safe_index1_type::iterator iterator1_type;
		typedef safe_index1_type::node_type node1_type;
		typedef safe_index1_type::insert_return_type insert_return1_type;
		typedef safe_index1_type::base_insert_return_type base_insert_return1_type;

		mi_cont_type cont;
		cont.insert(arr[0]);
		cont.insert(arr[1]);
		cont.insert(arr[2]);

		// 0, 0
		{
			std::size_t old_size = cont.size();
			node0_type node = cont.extract(1);
			yggr_test_assert(!node.empty());
			yggr_test_assert(cont.size() == old_size - 1);
			yggr_test_assert(node.value() == arr[0]);

			insert_return0_type ins_ret = cont.insert(boost::move(node));
			yggr_test_assert(ins_ret.inserted);
			yggr_test_assert(ins_ret.node.empty());
			yggr_test_assert(cont.size() == old_size);

			node0_type node2 = cont.extract(5);
			yggr_test_assert(node2.empty());
			yggr_test_assert(cont.size() == old_size);
		}

		{
			std::size_t old_size = cont.size();
			node0_type node = cont.extract(1);
			yggr_test_assert(!node.empty());
			yggr_test_assert(cont.size() == old_size - 1);
			yggr_test_assert(node.value() == arr[0]);

			insert_return0_type ins_ret =
				cont.insert(
					boost::move(node),
					boost::bind(
						&handler_insert_node_of_kv<unsafe_midx_type, index_id0_type, base_insert_return0_type>,
						_1, _2));
			yggr_test_assert(ins_ret.inserted);
			yggr_test_assert(ins_ret.node.empty());
			yggr_test_assert(cont.size() == old_size);

			node0_type node2 = cont.extract(5);
			yggr_test_assert(node2.empty());
			yggr_test_assert(cont.size() == old_size);
		}

		{
			std::size_t old_size = cont.size();
			node0_type node = cont.extract(2);
			yggr_test_assert(!node.empty());
			yggr_test_assert(cont.size() == old_size - 1);
			yggr_test_assert(node.value() == arr[1]);

			bool ins_ret = cont.insert(boost::bind(&safe_index0_type::base_type::cbegin, _1), boost::move(node));
			yggr_test_assert(ins_ret);
			yggr_test_assert(cont.size() == old_size);
		}

		{
			std::size_t old_size = cont.size();
			node0_type node = cont.extract(3);
			yggr_test_assert(!node.empty());
			yggr_test_assert(cont.size() == old_size - 1);
			yggr_test_assert(node.value() == arr[2]);

			bool ins_ret =
				cont.insert(
					boost::bind(&safe_index0_type::base_type::cbegin, _1),
					boost::move(node),
					boost::bind(
						&handler_insert_node_of_kv_hint<unsafe_midx_type, index_id0_type, iterator0_type>,
						_1, _2));
			yggr_test_assert(ins_ret);
			yggr_test_assert(cont.size() == old_size);
		}

		{
			node0_type node = cont.extract(1);
			yggr_test_assert(!node.empty());
			yggr_test_assert(node.value() == arr[0]);

			cont.insert(arr[0]);

			insert_return0_type ins_ret = cont.insert(boost::move(node));
			yggr_test_assert(!ins_ret.inserted);
			yggr_test_assert(!ins_ret.node.empty());
		}

		{
			std::size_t old_size = cont.size();
			node0_type node = cont.extract(boost::bind(&safe_index0_type::base_type::cbegin, _1));
			yggr_test_assert(!node.empty());
			yggr_test_assert(cont.size() == old_size - 1);

			bool ins_ret = cont.insert(boost::bind(&safe_index0_type::base_type::cbegin, _1), boost::move(node));
			yggr_test_assert(ins_ret);
			yggr_test_assert(cont.size() == old_size);
		}

		// 0, 1
		{
			std::size_t old_size = cont.size();
			node0_type node = cont.get<0>().extract(1);
			yggr_test_assert(!node.empty());
			yggr_test_assert(cont.size() == old_size - 1);
			yggr_test_assert(node.value() == arr[0]);

			insert_return1_type ins_ret = cont.get<1>().insert(boost::move(node));
			yggr_test_assert(ins_ret.inserted);
			yggr_test_assert(ins_ret.node.empty());
			yggr_test_assert(cont.size() == old_size);
		}

		{
			std::size_t old_size = cont.size();
			node0_type node = cont.get<0>().extract(1);
			yggr_test_assert(!node.empty());
			yggr_test_assert(cont.size() == old_size - 1);
			yggr_test_assert(node.value() == arr[0]);

			insert_return1_type ins_ret =
				cont.get<1>().insert(
					boost::move(node),
					boost::bind(
						&handler_insert_node_of_kv<unsafe_midx_type, index_id1_type, base_insert_return1_type>,
						_1, _2));
			yggr_test_assert(ins_ret.inserted);
			yggr_test_assert(ins_ret.node.empty());
			yggr_test_assert(cont.size() == old_size);
		}

		{
			std::size_t old_size = cont.size();
			node0_type node = cont.get<0>().extract(2);
			yggr_test_assert(!node.empty());
			yggr_test_assert(cont.size() == old_size - 1);
			yggr_test_assert(node.value() == arr[1]);

			bool ins_ret = cont.get<1>().insert(boost::bind(&safe_index1_type::base_type::cbegin, _1), boost::move(node));
			yggr_test_assert(ins_ret);
			yggr_test_assert(cont.size() == old_size);
		}

		{
			std::size_t old_size = cont.size();
			node0_type node = cont.get<0>().extract(3);
			yggr_test_assert(!node.empty());
			yggr_test_assert(cont.size() == old_size - 1);
			yggr_test_assert(node.value() == arr[2]);

			bool ins_ret =
				cont.get<1>().insert(
					boost::bind(&safe_index1_type::base_type::cbegin, _1),
					boost::move(node),
					boost::bind(
						&handler_insert_node_of_kv_hint<unsafe_midx_type, index_id1_type, iterator1_type>,
						_1, _2));
			yggr_test_assert(ins_ret);
			yggr_test_assert(cont.size() == old_size);
		}

		{
			node0_type node = cont.get<0>().extract(1);
			yggr_test_assert(!node.empty());
			yggr_test_assert(node.value() == arr[0]);

			cont.insert(arr[0]);

			insert_return1_type ins_ret = cont.get<1>().insert(boost::move(node));
			yggr_test_assert(!ins_ret.inserted);
			yggr_test_assert(!ins_ret.node.empty());
		}

		{
			std::size_t old_size = cont.size();
			node0_type node = cont.get<0>().extract(boost::bind(&safe_index0_type::base_type::cbegin, _1));
			yggr_test_assert(!node.empty());
			yggr_test_assert(cont.size() == old_size - 1);

			bool ins_ret = cont.get<1>().insert(boost::bind(&safe_index1_type::base_type::cbegin, _1), boost::move(node));
			yggr_test_assert(ins_ret);
			yggr_test_assert(cont.size() == old_size);
		}

		// 1, 1
		{
			std::size_t old_size = cont.size();
			node1_type node = cont.get<1>().extract(1);
			yggr_test_assert(!node.empty());
			yggr_test_assert(cont.size() == old_size - 1);
			yggr_test_assert(node.value() == arr[0]);

			insert_return1_type ins_ret = cont.get<1>().insert(boost::move(node));
			yggr_test_assert(ins_ret.inserted);
			yggr_test_assert(ins_ret.node.empty());
			yggr_test_assert(cont.size() == old_size);

			node1_type node2 = cont.get<1>().extract(5);
			yggr_test_assert(node2.empty());
			yggr_test_assert(cont.size() == old_size);
		}

		{
			std::size_t old_size = cont.size();
			node1_type node = cont.get<1>().extract(1);
			yggr_test_assert(!node.empty());
			yggr_test_assert(cont.size() == old_size - 1);
			yggr_test_assert(node.value() == arr[0]);

			insert_return1_type ins_ret =
				cont.get<1>().insert(
					boost::move(node),
					boost::bind(
						&handler_insert_node_of_kv<unsafe_midx_type, index_id1_type, base_insert_return1_type>,
						_1, _2));
			yggr_test_assert(ins_ret.inserted);
			yggr_test_assert(ins_ret.node.empty());
			yggr_test_assert(cont.size() == old_size);

			node1_type node2 = cont.extract(5);
			yggr_test_assert(node2.empty());
			yggr_test_assert(cont.size() == old_size);
		}

		{
			std::size_t old_size = cont.size();
			node1_type node = cont.get<1>().extract(2);
			yggr_test_assert(!node.empty());
			yggr_test_assert(cont.size() == old_size - 1);
			yggr_test_assert(node.value() == arr[1]);

			bool ins_ret = cont.get<1>().insert(boost::bind(&safe_index1_type::base_type::cbegin, _1), boost::move(node));
			yggr_test_assert(ins_ret);
			yggr_test_assert(cont.size() == old_size);
		}

		{
			std::size_t old_size = cont.size();
			node1_type node = cont.get<1>().extract(3);
			yggr_test_assert(!node.empty());
			yggr_test_assert(cont.size() == old_size - 1);
			yggr_test_assert(node.value() == arr[2]);

			bool ins_ret =
				cont.get<1>().insert(
					boost::bind(&safe_index1_type::base_type::cbegin, _1),
					boost::move(node),
					boost::bind(
						&handler_insert_node_of_kv_hint<unsafe_midx_type, index_id1_type, iterator1_type>,
						_1, _2));
			yggr_test_assert(ins_ret);
			yggr_test_assert(cont.size() == old_size);
		}

		{
			node1_type node = cont.get<1>().extract(1);
			yggr_test_assert(!node.empty());
			yggr_test_assert(node.value() == arr[0]);

			cont.insert(arr[0]);

			insert_return1_type ins_ret = cont.get<1>().insert(boost::move(node));
			yggr_test_assert(!ins_ret.inserted);
			yggr_test_assert(!ins_ret.node.empty());
		}

		{
			std::size_t old_size = cont.size();
			node1_type node = cont.get<1>().extract(boost::bind(&safe_index1_type::base_type::cbegin, _1));
			yggr_test_assert(!node.empty());
			yggr_test_assert(cont.size() == old_size - 1);

			bool ins_ret = cont.get<1>().insert(boost::bind(&safe_index1_type::base_type::cbegin, _1), boost::move(node));
			yggr_test_assert(ins_ret);
			yggr_test_assert(cont.size() == old_size);
		}
	}

	// erase
	yggr_test_assert(cont.get<0>().insert(A(100, 100, "100")));
	yggr_test_assert(cont.get<0>().insert(A(200, 100, "100")));
	yggr_test_assert(cont.get<0>().insert(A(300, 100, "100")));
	yggr_test_assert(cont.get<0>().insert(A(400, 100, "100")));
	yggr_test_assert(cont.size() == 19);

	yggr_test_assert(1 == cont.get<0>().erase(100));
	yggr_test_assert(3 == cont.get<A::tag_prm2>().erase(100));
	yggr_test_assert(cont.size() == 15);

	// replace
	yggr_test_assert(cont.get<0>().replace(boost::bind(&unsafe_hash_unique_type::find<int>, _1, 0), A(0, 100, "0")));
	yggr_test_assert(cont.get<0>().replace(boost::bind(&unsafe_hash_unique_type::find<int>, _1, 0), A(100, 100, "0")));
	yggr_test_assert(cont.get<A::tag_prm1>().replace(boost::bind(&unsafe_hash_unique_type::find<int>, _1, 100), A(0, 100, "0")));
	yggr_test_assert(!cont.get<A::tag_prm1>().replace(boost::bind(&unsafe_hash_unique_type::find<int>, _1, 0), A(1, 100, "0")));
	yggr_test_assert(!cont.get<A::tag_prm1>().replace(boost::bind(&unsafe_hash_unique_type::cend, _1), A(1, 100, "0")));

	yggr_test_assert(cont.size() == 15);

	yggr_test_assert(cont.get<1>().find_and_replace(100, A(100, 100, "0")));
	yggr_test_assert(cont.get<A::tag_prm2>().find_and_replace(100, cont.get<A::tag_prm2>().hash_function(),
															cont.get<A::tag_prm2>().key_eq(), A(0, 1, "0")));
	yggr_test_assert(cont.get<A::tag_prm2>().find_and_replace(1, A(100, 1, "0")));
	yggr_test_assert(!cont.get<A::tag_prm2>().find_and_replace(1, cont.get<A::tag_prm2>().hash_function(),
															cont.get<A::tag_prm2>().key_eq(), A(2, 1, "0")));

	yggr_test_assert(cont.size() == 15);


	// modify

	A tmp1;
	yggr_test_assert(cont.get<0>().modify(boost::bind(&unsafe_hash_unique_type::find<int>, _1, 2),
									boost::bind(&modifier<A, A>, _1, A(2, 100, "100"))));

	yggr_test_assert(cont.get<0>().modify(boost::bind(&unsafe_hash_unique_type::find<int>, _1, 1),
									boost::bind(&modifier<A, A>, _1, A(1, 100, "100"), boost::ref(tmp1)),
									boost::bind(&rollback<A>, _1, boost::cref(tmp1)) ) );

	yggr_test_assert(cont.get<0>().modify(boost::bind(&unsafe_hash_unique_type::find<int>, _1, 2),
									yggr::safe_container::make_optimization_multi_index_modifier<unsafe_midx_type>(
										A(2, 100, "100"),
										boost::bind(&modifier<A, A>, _1, A(2, 100, "100")))));

	yggr_test_assert(cont.get<0>().modify(boost::bind(&unsafe_hash_unique_type::find<int>, _1, 1),
									yggr::safe_container::make_optimization_multi_index_modifier<unsafe_midx_type>(
										A(1, 100, "100"),
										boost::bind(&modifier<A, A>, _1, A(1, 100, "100"), boost::ref(tmp1))),
									boost::bind(&rollback<A>, _1, boost::cref(tmp1)) ) );

	yggr_test_assert(cont.size() == 15);

	yggr_test_assert(!cont.get<A::tag_prm1>().find_and_modify(2000,
													boost::bind(&modifier<A, A>, _1, A(1, 100, "100"), boost::ref(tmp1))));

	yggr_test_assert(!cont.get<A::tag_prm1>().find_and_modify(2000, cont.get<A::tag_prm1>().hash_function(), cont.get<A::tag_prm1>().key_eq(),
													boost::bind(&modifier<A, A>, _1, A(1, 100, "100"), boost::ref(tmp1))));

	yggr_test_assert(!cont.get<A::tag_prm1>().find_and_modify(2000,
													boost::bind(&modifier<A, A>, _1, A(1, 100, "100"), boost::ref(tmp1)),
													boost::bind(&rollback<A>, _1, boost::cref(tmp1))));

	yggr_test_assert(!cont.get<A::tag_prm1>().find_and_modify(2, cont.get<A::tag_prm1>().hash_function(), cont.get<A::tag_prm1>().key_eq(),
													boost::bind(&modifier<A, A>, _1, A(1, 100, "100"), boost::ref(tmp1)),
													boost::bind(&rollback<A>, _1, boost::cref(tmp1))));
	yggr_test_assert(cont.size() == 15);

	// modify key
	int tmp2;
	yggr_test_assert(cont.get<0>().modify_key(boost::bind(&unsafe_hash_unique_type::find<int>, _1, 2),
									boost::bind(&modifier<int, int>, _1, 200)));

	yggr_test_assert(cont.get<0>().modify_key(boost::bind(&unsafe_hash_unique_type::find<int>, _1, 200),
									boost::bind(&modifier<int, int>, _1, 200),
									boost::bind(&rollback<int>, _1, boost::cref(tmp2))));

	yggr_test_assert(cont.get<0>().modify_key(boost::bind(&unsafe_hash_unique_type::find<int>, _1, 200),
										yggr::safe_container::make_optimization_multi_index_modifier<unsafe_midx_type>(
											A(200, 100, "100"),
											boost::bind(&modifier<int, int>, _1, 200))));

	yggr_test_assert(cont.get<0>().modify_key(boost::bind(&unsafe_hash_unique_type::find<int>, _1, 200),
										yggr::safe_container::make_optimization_multi_index_modifier<unsafe_midx_type>(
											A(200, 100, "100"),
											boost::bind(&modifier<int, int>, _1, 200)),
									boost::bind(&rollback<int>, _1, boost::cref(tmp2))));


	yggr_test_assert(cont.size() == 15);

	yggr_test_assert(!cont.get<A::tag_prm1>().find_and_modify_key(2000,
														boost::bind(&modifier<int, int>, _1, 3, boost::ref(tmp2))));

	yggr_test_assert(!cont.get<A::tag_prm1>().find_and_modify_key(2000, cont.get<A::tag_prm1>().hash_function(), cont.get<A::tag_prm1>().key_eq(),
														boost::bind(&modifier<int, int>, _1, 3, boost::ref(tmp2))));

	yggr_test_assert(!cont.get<A::tag_prm1>().find_and_modify_key(200,
														boost::bind(&modifier<int, int>, _1, 3, boost::ref(tmp2)),
														boost::bind(&rollback<int>, _1, boost::cref(tmp2))));

	yggr_test_assert(!cont.get<A::tag_prm1>().find_and_modify_key(200, cont.get<A::tag_prm1>().hash_function(), cont.get<A::tag_prm1>().key_eq(),
														boost::bind(&modifier<int, int>, _1, 3, boost::ref(tmp2)),
														boost::bind(&rollback<int>, _1, boost::cref(tmp2))));
	yggr_test_assert(cont.size() == 15);

	tmp2 = 0;

	mi_cont_type::base_type base_cont(arr + 0, arr + 10);
	yggr_test_assert(cont.size() == 15);
	yggr_test_assert(base_cont.size() == 5);

	cont.get<0>().swap(base_cont.get<0>());
	yggr_test_assert(cont.size() == 5);
	yggr_test_assert(base_cont.size() == 15);

	cont.get<A::tag_prm1>().swap(base_cont);
	yggr_test_assert(cont.size() == 15);
	yggr_test_assert(base_cont.size() == 5);

	yggr_test_assert(cont.erase(boost::bind(&unsafe_hash_unique_type::cbegin, _1)));
	yggr_test_assert(cont.size() == 14);
	yggr_test_assert(cont.erase(boost::bind(&unsafe_hash_unique_type::cbegin, _1),
						boost::bind(&unsafe_hash_unique_type::cend, _1)));

	yggr_test_assert(cont.empty());

	{
		typedef yggr::safe_container::nth_index<mi_cont_type, 0>::type safe_index0_type;
		typedef safe_index0_type::index_id_type index_id0_type;
		typedef safe_index0_type::iterator iterator0_type;

		typedef yggr::safe_container::nth_index<mi_cont_type, 1>::type safe_index1_type;
		typedef safe_index1_type::index_id_type index_id1_type;
		typedef safe_index1_type::iterator iterator1_type;

		yggr_test_assert(cont.emplace(yggr::tag_non_handler()));
		yggr_test_assert(cont.emplace(yggr::tag_non_handler(), 1, 1, "1"));
		yggr_test_assert(cont.emplace_hint(boost::bind(&safe_index0_type::base_type::cend, _1), yggr::tag_non_handler()));
		yggr_test_assert(cont.emplace_hint(boost::bind(&safe_index0_type::base_type::cend, _1), yggr::tag_non_handler(), 1, 1, "1"));

		int call_count = 0;

		yggr_test_assert(!(cont.emplace(boost::bind(&handler_insert_value_of_hash<unsafe_midx_type, index_id0_type, iterator0_type>,
											_1, _2, boost::ref(call_count)))));

		yggr_test_assert(!cont.get<1>().
					emplace(boost::bind(&handler_insert_value_of_hash<unsafe_midx_type, index_id1_type, iterator1_type>,
											_1, _2, boost::ref(call_count)), 1, 1, "1"));

		yggr_test_assert(cont.emplace_hint(boost::bind(&safe_index0_type::base_type::cend, _1),
									boost::bind(&handler_insert_value_of_hash<unsafe_midx_type, index_id0_type, iterator0_type>,
													_1, _2, boost::ref(call_count))));

		yggr_test_assert(cont.get<1>().
				emplace_hint(boost::bind(&safe_index1_type::base_type::cend, _1),
								boost::bind(&handler_insert_value_of_hash<unsafe_midx_type, index_id1_type, iterator1_type>,
											_1, _2, boost::ref(call_count)), 1, 1, "1"));


		yggr_test_assert(call_count == 4);
		yggr_test_assert(cont.size() == 2);
	}

	cont.get<0>().clear();
	yggr_test_assert(cont.empty());

	cont.clear();
	yggr_test_assert(cont.empty());

	std::cout << "---------------test_modifiers_of_hash end-------------------------" << std::endl;
}


template<typename Final, typename IndexID, typename Iterator>
bool handler_find_of_hash(Final& final,
							const
								boost::fusion::pair
								<
									IndexID,
									Iterator
								>& rst, int& count)
{
	typedef boost::fusion::pair<IndexID, Iterator> rst_type;
	typedef typename rst_type::first_type index_id_type;
	typedef Final midx_type;
	typedef typename
		boost::multi_index::nth_index
		<
			midx_type, index_id_type::first::value
		>::type now_index_type;

	++count;
	return rst.second != final.template get<index_id_type::first::value>().end();
}

template<typename Final, typename IndexID, typename Iterator>
std::size_t handler_equal_range_of_hash(Final& final,
										const
											boost::fusion::pair
											<
												IndexID,
												std::pair<Iterator, Iterator>
											>& rst, int& count)
{
	typedef boost::fusion::pair<IndexID, Iterator> rst_type;
	typedef typename rst_type::first_type index_id_type;
	typedef Final midx_type;
	typedef typename
		boost::multi_index::nth_index
		<
			midx_type, index_id_type::first::value
		>::type now_index_type;

	++count;
	return std::distance(rst.second.first, rst.second.second);
}

void test_lookup_of_hash(void)
{
	typedef yggr::safe_container::safe_multi_index_container<
			A,
			boost::multi_index::indexed_by<
				boost::multi_index::hashed_unique<
					boost::multi_index::tag<A::tag_prm1>,
					BOOST_MULTI_INDEX_MEMBER(A, int, prm1) >,

				boost::multi_index::hashed_non_unique<
					boost::multi_index::tag<A::tag_prm2>,
					BOOST_MULTI_INDEX_MEMBER(A, int, prm2)>
			>
		> mi_cont_type;

	typedef mi_cont_type::base_type unsafe_midx_type;

	typedef boost::multi_index::nth_index<mi_cont_type, 0>::type safe_index0_type;
	typedef safe_index0_type::index_id_type index_id0_type;
	typedef safe_index0_type::iterator iterator0_type;

	typedef boost::multi_index::nth_index<mi_cont_type, 1>::type safe_index1_type;
	typedef safe_index1_type::index_id_type index_id1_type;
	typedef safe_index1_type::iterator iterator1_type;

	A arr[10] = {A(1, 1, "aaa"), A(2, 2, "bbb"), A(3, 3, "ccc"), A(4, 3, "ddd")};
	mi_cont_type cont(arr + 0, arr + 10);


	yggr_test_assert(cont.find(1));
	yggr_test_assert(!cont.find(100));

	{
		int call_count = 0;

		yggr_test_assert(cont.find(1,
							boost::bind(
								&handler_find_of_hash<
									unsafe_midx_type, index_id0_type, iterator0_type>, _1, _2, boost::ref(call_count)) ));

		const safe_index0_type& idx0_cref = cont.get<0>();

		yggr_test_assert(call_count == 1);

		yggr_test_assert(idx0_cref.find(1,
							boost::bind(
								&handler_find_of_hash<
									const unsafe_midx_type, index_id0_type, iterator0_type>, _1, _2, boost::ref(call_count)) ));

		yggr_test_assert(call_count == 2);

		yggr_test_assert(!cont.find(100,
							boost::bind(
								&handler_find_of_hash<
									unsafe_midx_type, index_id0_type, iterator0_type>, _1, _2, boost::ref(call_count)) ));

		yggr_test_assert(call_count == 3);
	}

	yggr_test_assert(cont.get<1>().find(1, cont.hash_function(), cont.key_eq()));
	yggr_test_assert(!cont.get<1>().find(100, cont.hash_function(), cont.key_eq()));

	{
		int call_count = 0;

		yggr_test_assert(cont.find(1, cont.hash_function(), cont.key_eq(),
							boost::bind(
								&handler_find_of_hash<
									unsafe_midx_type, index_id0_type, iterator0_type>, _1, _2, boost::ref(call_count)) ));

		const safe_index0_type& idx0_cref = cont.get<0>();

		yggr_test_assert(call_count == 1);

		yggr_test_assert(idx0_cref.find(1, cont.hash_function(), cont.key_eq(),
							boost::bind(
								&handler_find_of_hash<
									const unsafe_midx_type, index_id0_type, iterator0_type>, _1, _2, boost::ref(call_count)) ));

		yggr_test_assert(call_count == 2);

		yggr_test_assert(!cont.find(100, cont.hash_function(), cont.key_eq(),
							boost::bind(
								&handler_find_of_hash<
									unsafe_midx_type, index_id0_type, iterator0_type>, _1, _2, boost::ref(call_count)) ));

		yggr_test_assert(call_count == 3);
	}

	yggr_test_assert(cont.count(1) == 1);
	yggr_test_assert(cont.count(100) == 0);

	yggr_test_assert(cont.count(1, cont.hash_function(), cont.key_eq()) == 1);
	yggr_test_assert(cont.count(100, cont.hash_function(), cont.key_eq()) == 0);

	{
		int call_count = 0;
		yggr_test_assert(1 == cont.equal_range(1,
								boost::bind(
									&handler_equal_range_of_hash<
										unsafe_midx_type, index_id0_type, iterator0_type>, _1, _2, boost::ref(call_count))));

		yggr_test_assert(call_count == 1);

		yggr_test_assert(0 == cont.equal_range(100,
								boost::bind(
									&handler_equal_range_of_hash<
										unsafe_midx_type, index_id0_type, iterator0_type>, _1, _2, boost::ref(call_count))));

		yggr_test_assert(call_count == 2);
	}

	{
		int call_count = 0;
		yggr_test_assert(1 == cont.equal_range(1, cont.hash_function(), cont.key_eq(),
								boost::bind(
									&handler_equal_range_of_hash<
										unsafe_midx_type, index_id0_type, iterator0_type>, _1, _2, boost::ref(call_count))));

		yggr_test_assert(call_count == 1);

		yggr_test_assert(0 == cont.equal_range(100, cont.hash_function(), cont.key_eq(),
								boost::bind(
									&handler_equal_range_of_hash<
										unsafe_midx_type, index_id0_type, iterator0_type>, _1, _2, boost::ref(call_count))));

		yggr_test_assert(call_count == 2);
	}

	{
		A a;

		bool bret = cont.get_value(1, a);
		yggr_test_assert(bret);
		yggr_test_assert(a == arr[0]);

		yggr_test_assert(!cont.get_value(100, a));
	}

	{
		A a;

		bool bret = cont.get<1>().get_value(1, cont.hash_function(), cont.key_eq(), a);
		yggr_test_assert(bret);
		yggr_test_assert(a == arr[0]);

		yggr_test_assert(!cont.get<1>().get_value(100, cont.hash_function(), cont.key_eq(), a));
	}

	std::cout << "---------------test_lookup_of_hash end-------------------------" << std::endl;

}

void test_set_operations_of_hash(void)
{
	typedef yggr::safe_container::safe_multi_index_container<
			A,
			boost::multi_index::indexed_by<
				boost::multi_index::hashed_unique<
					boost::multi_index::tag<A::tag_prm1>,
					BOOST_MULTI_INDEX_MEMBER(A, int, prm1) >,

				boost::multi_index::hashed_non_unique<
					boost::multi_index::tag<A::tag_prm2>,
					BOOST_MULTI_INDEX_MEMBER(A, int, prm2)>
			>
		> mi_cont_type;

	A arr[10] = {A(1, 1, "aaa"), A(2, 2, "bbb"), A(3, 3, "ccc"), A(4, 3, "ddd")};
	mi_cont_type cont(arr + 0, arr + 10);

	yggr_test_assert(1 == cont.get<A::tag_prm1>().count(1));
	yggr_test_assert(1 == cont.get<0>().count(2));
	yggr_test_assert(0 == cont.get<0>().count(100));

	yggr_test_assert(1 == cont.get<A::tag_prm2>().count(2));
	yggr_test_assert(2 == cont.get<1>().count(3));
	yggr_test_assert(0 == cont.get<1>().count(100));

	yggr_test_assert(cont.size() == 5);

	mi_cont_type::base_type base_cont(arr + 0, arr + 10);
	yggr_test_assert(cont.size() == 5);

	yggr_test_assert(1 == cont.get<A::tag_prm1>().count(1, cont.get<0>().hash_function(), std::equal_to<int>()));

	yggr_test_assert(1 == cont.get<0>().count(2, cont.get<0>().hash_function(), std::equal_to<int>()));

	yggr_test_assert(0 == cont.get<0>().count(100, cont.get<0>().hash_function(), std::equal_to<int>()));

	yggr_test_assert(1 == cont.get<A::tag_prm2>().count(2, cont.get<1>().hash_function(), std::equal_to<int>()));

	yggr_test_assert(2 == cont.get<1>().count(3, cont.get<1>().hash_function(), std::equal_to<int>()));

	yggr_test_assert(0 == cont.get<1>().count(100, cont.get<1>().hash_function(), std::equal_to<int>()));


	std::cout << "---------------test_set_operations_of_hash end-------------------" << std::endl;
}

void test_bucket_of_hash(void)
{
	typedef yggr::safe_container::safe_multi_index_container<
			A,
			boost::multi_index::indexed_by<
				boost::multi_index::hashed_unique<
					boost::multi_index::tag<A::tag_prm1>,
					BOOST_MULTI_INDEX_MEMBER(A, int, prm1) >,

				boost::multi_index::hashed_non_unique<
					boost::multi_index::tag<A::tag_prm2>,
					BOOST_MULTI_INDEX_MEMBER(A, int, prm2)>
			>
		> mi_cont_type;

	A arr[10] = {A(1, 1, "aaa"), A(2, 2, "bbb"), A(3, 3, "ccc"), A(4, 3, "ddd")};

	mi_cont_type cont(arr + 0, arr + 10);
	mi_cont_type::base_type base_cont(arr + 0, arr + 10);


	yggr_test_assert(cont.get<0>().bucket_count() == base_cont.get<0>().bucket_count());
	yggr_test_assert(cont.get<0>().max_bucket_count() == base_cont.get<0>().max_bucket_count());
	yggr_test_assert(cont.get<0>().bucket_size(0) == base_cont.get<0>().bucket_size(0));
	yggr_test_assert(cont.get<1>().bucket_size(1) == base_cont.get<1>().bucket_size(1));

	yggr_test_assert(cont.get<0>().bucket(0) == base_cont.get<0>().bucket(0));
	yggr_test_assert(cont.get<1>().bucket(1) == base_cont.get<1>().bucket(1));

	std::cout << "---------------test_bucket_of_hash end-------------------" << std::endl;
}

void test_hash_policy_of_hash(void)
{
	typedef yggr::safe_container::safe_multi_index_container<
			A,
			boost::multi_index::indexed_by<
				boost::multi_index::hashed_unique<
					boost::multi_index::tag<A::tag_prm1>,
					BOOST_MULTI_INDEX_MEMBER(A, int, prm1) >,

				boost::multi_index::hashed_non_unique<
					boost::multi_index::tag<A::tag_prm2>,
					BOOST_MULTI_INDEX_MEMBER(A, int, prm2)>
			>
		> mi_cont_type;

	A arr[10] = {A(1, 1, "aaa"), A(2, 2, "bbb"), A(3, 3, "ccc"), A(4, 3, "ddd")};

	mi_cont_type cont(arr + 0, arr + 10);
	mi_cont_type::base_type base_cont(arr + 0, arr + 10);

	yggr_test_assert(cont.get<0>().load_factor() == base_cont.get<0>().load_factor());
	yggr_test_assert(cont.get<1>().load_factor() == base_cont.get<1>().load_factor());

	yggr_test_assert(cont.get<0>().max_load_factor() == base_cont.get<0>().max_load_factor());
	yggr_test_assert(cont.get<1>().max_load_factor() == base_cont.get<1>().max_load_factor());

	cont.get<0>().max_load_factor(cont.get<0>().max_load_factor());

	cont.get<1>().max_load_factor(base_cont.get<1>().max_load_factor());

	yggr_test_assert(cont.get<0>().max_load_factor() == base_cont.get<0>().max_load_factor());
	yggr_test_assert(cont.get<1>().max_load_factor() == base_cont.get<1>().max_load_factor());

	cont.get<0>().rehash(100);
	base_cont.get<0>().rehash(100);

	yggr_test_assert(cont.get<0>().max_load_factor() == base_cont.get<0>().max_load_factor());

	std::cout << "---------------test_hash_policy_of_hash end-------------------" << std::endl;
}

void test_swap_of_hash(void)
{
	typedef yggr::safe_container::safe_multi_index_container<
			A,
			boost::multi_index::indexed_by<
				boost::multi_index::hashed_unique<
					boost::multi_index::tag<A::tag_prm1>,
					BOOST_MULTI_INDEX_MEMBER(A, int, prm1) >,

				boost::multi_index::hashed_non_unique<
					boost::multi_index::tag<A::tag_prm2>,
					BOOST_MULTI_INDEX_MEMBER(A, int, prm2)>
			>
		> mi_cont_type;
	test_swap<mi_cont_type>();
	std::cout << "---------------test_swap_of_hash end-----------------" << std::endl;
}

void test_operator_set_of_hash(void)
{
	typedef yggr::safe_container::safe_multi_index_container<
			A,
			boost::multi_index::indexed_by<
				boost::multi_index::hashed_unique<
					boost::multi_index::tag<A::tag_prm1>,
					BOOST_MULTI_INDEX_MEMBER(A, int, prm1) >,

				boost::multi_index::hashed_non_unique<
					boost::multi_index::tag<A::tag_prm2>,
					BOOST_MULTI_INDEX_MEMBER(A, int, prm2)>
			>
		> mi_cont_type;
	test_operator_set<mi_cont_type>();
	std::cout << "---------------test_operator_set_of_hash end-----------------" << std::endl;
}

void test_comparer_of_hash(void)
{
	typedef yggr::safe_container::safe_multi_index_container<
			A,
			boost::multi_index::indexed_by<
				boost::multi_index::hashed_unique<
					boost::multi_index::tag<A::tag_prm1>,
					BOOST_MULTI_INDEX_MEMBER(A, int, prm1) >,

				boost::multi_index::hashed_non_unique<
					boost::multi_index::tag<A::tag_prm2>,
					BOOST_MULTI_INDEX_MEMBER(A, int, prm2)>
			>
		> mi_cont_type;
	test_compare_of_equal_to_and_not_equal_to<mi_cont_type>();

	std::cout << "---------------test_comparer_of_hash end-----------------" << std::endl;
}

void test_safe_other_of_hash(void)
{
	typedef
		yggr::safe_container::safe_multi_index_container
		<
			A,
			boost::multi_index::indexed_by<
				boost::multi_index::hashed_unique<
					boost::multi_index::tag<A::tag_prm1>,
					BOOST_MULTI_INDEX_MEMBER(A, int, prm1) >,

				boost::multi_index::hashed_non_unique<
					boost::multi_index::tag<A::tag_prm2>,
					BOOST_MULTI_INDEX_MEMBER(A, int, prm2)>
			>
		> mi_cont_type;

	typedef mi_cont_type::base_type unsafe_midx_type;

	typedef boost::multi_index::nth_index<mi_cont_type, 0>::type safe_index0_type;
	typedef safe_index0_type::index_id_type index_id0_type;
	typedef safe_index0_type::iterator iterator0_type;

	typedef boost::multi_index::nth_index<mi_cont_type, 1>::type safe_index1_type;
	typedef safe_index1_type::index_id_type index_id1_type;
	typedef safe_index1_type::iterator iterator1_type;

	test_safe_other_gernel<mi_cont_type>();

	A arr[10] = {A(1, 1, "aaa"), A(2, 2, "bbb"), A(3, 3, "ccc"), A(4, 3, "ddd")};

	mi_cont_type cont(arr + 0, arr + 10);

	yggr_test_assert(cont.is_exists(1));
	yggr_test_assert(!cont.is_exists(100));
	yggr_test_assert(cont.is_exists(1, cont.hash_function(), cont.key_eq()));
	yggr_test_assert(!cont.is_exists(100, cont.hash_function(), cont.key_eq()));

	{
		A beg_val;

		mi_cont_type cont_empty;
		yggr_test_assert(!cont_empty.get_begin_value(beg_val));

		try
		{
			cont_empty.begin_value();
			yggr_test_assert(false);
		}
		catch(const mi_cont_type::error_type&)
		{
		}


		cont.get_begin_value(beg_val);
		yggr_test_assert(beg_val == cont.begin_value());
	}

	{
		int call_count = 0;
		cont.for_each_local_iterator(0, boost::bind(&handler_for_each<typename safe_index0_type::local_iterator>,
														_1, boost::ref(call_count)));
		yggr_test_assert(call_count == cont.bucket_size(0));

		call_count = 0;
		cont.for_each_local_iterator(0, boost::bind(&handler_for_each<typename safe_index0_type::local_iterator>,
														_1, boost::ref(call_count)),
										boost::bind(&handler_filter<typename safe_index0_type::local_iterator>,
														_1, false));
		yggr_test_assert(call_count == 0);

		call_count = 0;
		cont.for_each_local_iterator(0, boost::bind(&handler_for_each<typename safe_index0_type::local_iterator>,
														_1, boost::ref(call_count)),
										boost::bind(&handler_filter<typename safe_index0_type::local_iterator>,
														_1, true));
		yggr_test_assert(call_count == cont.bucket_size(0));
	}

	{
		int call_count = 0;
		cont.for_each_local(0, boost::bind(&handler_for_each<typename safe_index0_type::value_type>,
											_1, boost::ref(call_count)));
		yggr_test_assert(call_count == cont.bucket_size(0));

		call_count = 0;
		cont.for_each_local(0, boost::bind(&handler_for_each<typename safe_index0_type::value_type>,
											_1, boost::ref(call_count)),
								boost::bind(&handler_filter<typename safe_index0_type::value_type>,
											_1, false));
		yggr_test_assert(call_count == 0);

		call_count = 0;
		cont.for_each_local(0, boost::bind(&handler_for_each<typename safe_index0_type::value_type>,
											_1, boost::ref(call_count)),
								boost::bind(&handler_filter<typename safe_index0_type::value_type>,
											_1, true));
		yggr_test_assert(call_count == cont.bucket_size(0));
	}

	std::cout << "---------------test_safe_other_of_hash end-----------------" << std::endl;
}

//------------------------------ordered--------------------------------
void test_construct_of_ordered(void)
{
	typedef yggr::safe_container::safe_multi_index_container<
			A,
			boost::multi_index::indexed_by<
				boost::multi_index::ordered_unique<
					boost::multi_index::tag<A::tag_prm1>,
					BOOST_MULTI_INDEX_MEMBER(A, int, prm1) >,

				boost::multi_index::ordered_non_unique<
					boost::multi_index::tag<A::tag_prm2>,
					BOOST_MULTI_INDEX_MEMBER(A, int, prm2)>
			>
		> mi_cont_type;
	test_construct<mi_cont_type>();

	std::cout << "---------------test_construct_of_ordered end-------------------------" << std::endl;
}

void test_observers_of_ordered(void)
{
	typedef yggr::safe_container::safe_multi_index_container<
			A,
			boost::multi_index::indexed_by<
				boost::multi_index::ordered_unique<
					boost::multi_index::tag<A::tag_prm1>,
					BOOST_MULTI_INDEX_MEMBER(A, int, prm1) >,

				boost::multi_index::ordered_non_unique<
					boost::multi_index::tag<A::tag_prm2>,
					BOOST_MULTI_INDEX_MEMBER(A, int, prm2)>
			>
		> mi_cont_type;

	A arr[10] = {A(1, 1, "aaa"), A(2, 2, "bbb"), A(3, 3, "ccc"), A(4, 3, "ddd")};
	mi_cont_type cont(arr + 0, arr + 10);
	mi_cont_type cont2(cont.get_allocator());

	cont.get<0>().get_allocator();
	cont.get<1>().get_allocator();
	cont.get<A::tag_prm1>().get_allocator();
	cont.get<A::tag_prm2>().get_allocator();

	cont.get<0>().key_extractor();
	cont.get<1>().key_extractor();
	cont.get<A::tag_prm1>().key_extractor();
	cont.get<A::tag_prm2>().key_extractor();

	yggr_test_assert(cont.get<0>().key_extractor()(arr[3]) == 4);
	yggr_test_assert(cont.get<1>().key_extractor()(arr[3]) == 3);

	cont.get<0>().key_comp();
	cont.get<1>().key_comp();
	cont.get<A::tag_prm1>().key_comp();
	cont.get<A::tag_prm2>().key_comp();

	cont.get<0>().value_comp();
	cont.get<1>().value_comp();
	cont.get<A::tag_prm1>().value_comp();
	cont.get<A::tag_prm2>().value_comp();

	std::cout << "---------------test_observers_of_ordered end-------------------------" << std::endl;
}

void test_capacity_of_ordered(void)
{
	typedef yggr::safe_container::safe_multi_index_container<
			A,
			boost::multi_index::indexed_by<
				boost::multi_index::ordered_unique<
					boost::multi_index::tag<A::tag_prm1>,
					BOOST_MULTI_INDEX_MEMBER(A, int, prm1) >,

				boost::multi_index::ordered_non_unique<
					boost::multi_index::tag<A::tag_prm2>,
					BOOST_MULTI_INDEX_MEMBER(A, int, prm2)>
			>
		> mi_cont_type;

	A arr[10] = {A(1, 1, "aaa"), A(2, 2, "bbb"), A(3, 3, "ccc"), A(4, 3, "ddd")};
	mi_cont_type cont(arr + 0, arr + 10);
	mi_cont_type::base_type base_cont(arr + 0, arr + 10);
	yggr_test_assert(cont.size() == 5);
	yggr_test_assert(!cont.empty());
	yggr_test_assert(base_cont.size() == cont.size());
	yggr_test_assert(base_cont.max_size() == cont.max_size());

	yggr_test_assert(cont.get<0>().empty() == cont.get<A::tag_prm1>().empty());
	yggr_test_assert(cont.get<0>().empty() == cont.get<A::tag_prm2>().empty());

	yggr_test_assert(cont.get<1>().empty() == cont.get<A::tag_prm2>().empty());

	yggr_test_assert(cont.get<1>().empty() == base_cont.empty());

	yggr_test_assert(cont.get<0>().size() == cont.get<A::tag_prm1>().size());

	yggr_test_assert(cont.get<0>().size() == base_cont.size());

	yggr_test_assert(cont.get<0>().max_size() == base_cont.max_size());

	yggr_test_assert(cont.get<0>().max_size() == base_cont.get<0>().max_size());

	yggr_test_assert(cont.get<A::tag_prm2>().max_size() == base_cont.get<1>().max_size());

	std::cout << "---------------test_capacity_of_ordered end-------------------------" << std::endl;
}

template<typename Final, typename IndexID, typename Iterator>
bool handler_insert_value_of_ordered(Final& final,
										const
											boost::fusion::pair
											<
												IndexID,
												std::pair<Iterator, bool>
											>& rst, int& count)
{
	typedef boost::fusion::pair<IndexID, std::pair<Iterator, bool> > rst_type;
	typedef typename rst_type::first_type index_id_type;
	typedef Final midx_type;
	typedef typename
		boost::multi_index::nth_index
		<
			midx_type, index_id_type::first::value
		>::type now_index_type;

	typedef typename
		boost::mpl::if_
		<
			boost::is_const<Final>,
			const now_index_type&,
			now_index_type&
		>::type now_index_ref_type;

	++count;
	if(rst.second.second)
	{
		now_index_ref_type now_index = final.template get<index_id_type::first::value>();

		yggr_test_assert((now_index.find((now_index.key_extractor())(*rst.second.first)) != now_index.end()));
	}
	return rst.second.second;
}

void test_modifiers_of_ordered(void)
{
	typedef
		yggr::safe_container::safe_multi_index_container
		<
			A,
			boost::multi_index::indexed_by
			<
				boost::multi_index::ordered_unique
				<
					boost::multi_index::tag<A::tag_prm1>,
					BOOST_MULTI_INDEX_MEMBER(A, int, prm1)
				>,

				boost::multi_index::ordered_non_unique
				<
					boost::multi_index::tag<A::tag_prm2>,
					BOOST_MULTI_INDEX_MEMBER(A, int, prm2)
				>
			>
		> mi_cont_type;

	typedef mi_cont_type::base_type unsafe_midx_type;
	typedef boost::multi_index::nth_index<unsafe_midx_type, 0>::type unsafe_ordered_unique_type;
	typedef boost::multi_index::nth_index<unsafe_midx_type, 1>::type unsafe_ordered_non_unique_type;

	A arr[10] = {A(1, 1, "aaa"), A(2, 2, "bbb"), A(3, 3, "ccc"), A(4, 3, "ddd")};
	mi_cont_type cont(arr + 0, arr + 10);

	yggr_test_assert(cont.size() == 5);

// initializer_list
#ifndef BOOST_NO_CXX11_HDR_INITIALIZER_LIST
	{
		mi_cont_type cont1 = {A(1, 1, "aaa"), A(2, 2, "bbb"), A(3, 3, "ccc"), A(4, 3, "ddd")};
		yggr_test_assert(cont1.size() == 4);
	}

	{
		mi_cont_type cont1;
		cont1.insert({A(1, 1, "aaa"), A(2, 2, "bbb"), A(3, 3, "ccc"), A(4, 3, "ddd")});
		yggr_test_assert(cont1.size() == 4);
	}
#endif // BOOST_NO_CXX11_HDR_INITIALIZER_LIST

	// insert
	yggr_test_assert(cont.get<0>().insert(A(5, 3, "eee")));
	yggr_test_assert(cont.size() == 6);
	yggr_test_assert(!cont.get<A::tag_prm1>().insert(A(3, 5, "cccc")));
	yggr_test_assert(cont.size() == 6);

	A arr2[10] = {A(10, 10, "a10"), A(20, 20, "b20"), A(30, 30, "c30"), A(40, 30, "d40")};
	yggr_test_assert(cont.get<A::tag_prm1>().insert(arr2 + 0, arr2 + 10));
	yggr_test_assert(cont.size() == 10);
	yggr_test_assert(cont.get<0>().insert(arr + 0, arr + 10));
	yggr_test_assert(cont.size() == 10);

	yggr_test_assert(cont.get<1>().insert(A(6, 3, "eee")));
	yggr_test_assert(cont.size() == 11);
	yggr_test_assert(!cont.get<A::tag_prm2>().insert(A(6, 3, "cccc")));
	yggr_test_assert(cont.size() == 11);

	A arr3[10] = {A(11, 11, "a11"), A(21, 21, "b21"), A(31, 31, "c31"), A(41, 31, "d41")};
	yggr_test_assert(cont.get<A::tag_prm2>().insert(arr3, arr3 + 10));
	yggr_test_assert(cont.size() == 15);
	yggr_test_assert(cont.get<1>().insert(arr3, arr3 + 10));
	yggr_test_assert(cont.size() == 15);

	{
		typedef yggr::safe_container::nth_index<mi_cont_type, 0>::type safe_index0_type;
		typedef safe_index0_type::index_id_type index_id0_type;
		typedef safe_index0_type::iterator iterator0_type;


		typedef yggr::safe_container::nth_index<mi_cont_type, 1>::type safe_index1_type;
		typedef safe_index1_type::index_id_type index_id1_type;
		typedef safe_index1_type::iterator iterator1_type;

		mi_cont_type cont;
		int call_count = 0;
		bool bright =
			cont.insert(arr[0],
						boost::bind(&handler_insert_value_of_ordered<unsafe_midx_type, index_id0_type, iterator0_type>,
									_1, _2, boost::ref(call_count)));

		yggr_test_assert(call_count == 1);

		A ins_data = arr[1];
		 bright = bright &&
			cont.get<1>().
				insert(boost::move(ins_data),
						boost::bind(&handler_insert_value_of_ordered<unsafe_midx_type, index_id1_type, iterator1_type>,
									_1, _2, boost::ref(call_count)));

		yggr_test_assert(call_count == 2);

		yggr_test_assert(bright);
		yggr_test_assert(cont.size() == 2);

		bright = bright &&
			cont.get<0>().
				insert(arr[0],
						boost::bind(&handler_insert_value_of_ordered<unsafe_midx_type, index_id0_type, iterator0_type>,
									_1, _2, boost::ref(call_count)));

		yggr_test_assert(call_count == 3);

		yggr_test_assert(!bright);
		yggr_test_assert(cont.size() == 2);
	}

	{
		typedef yggr::safe_container::nth_index<mi_cont_type, 0>::type safe_index0_type;
		typedef safe_index0_type::index_id_type index_id0_type;
		typedef safe_index0_type::iterator iterator0_type;


		typedef yggr::safe_container::nth_index<mi_cont_type, 1>::type safe_index1_type;
		typedef safe_index1_type::index_id_type index_id1_type;
		typedef safe_index1_type::iterator iterator1_type;

		mi_cont_type cont;
		bool bright =
			cont.insert(boost::bind(&safe_index0_type::base_type::cend, _1), arr[0]);


		A ins_data = arr[1];
		 bright = bright &&
			cont.get<1>().
				insert(boost::bind(&safe_index1_type::base_type::cend, _1),
						boost::move(ins_data));


		yggr_test_assert(bright);
		yggr_test_assert(cont.size() == 2);

		bright = bright &&
			cont.get<0>().insert(boost::bind(&safe_index0_type::base_type::cend, _1), arr[0]);

		yggr_test_assert(bright);
	}

	{
		typedef yggr::safe_container::nth_index<mi_cont_type, 0>::type safe_index0_type;
		typedef safe_index0_type::index_id_type index_id0_type;
		typedef safe_index0_type::iterator iterator0_type;


		typedef yggr::safe_container::nth_index<mi_cont_type, 1>::type safe_index1_type;
		typedef safe_index1_type::index_id_type index_id1_type;
		typedef safe_index1_type::iterator iterator1_type;

		mi_cont_type cont;
		int call_count = 0;
		bool bright =
			cont.insert(boost::bind(&safe_index0_type::base_type::cend, _1),
						arr[0],
						boost::bind(&handler_insert_value_of_ordered<unsafe_midx_type, index_id0_type, iterator0_type>,
									_1, _2, boost::ref(call_count)));

		yggr_test_assert(call_count == 1);

		A ins_data = arr[1];
		 bright = bright &&
			cont.get<1>().
				insert(boost::bind(&safe_index1_type::base_type::cend, _1),
						boost::move(ins_data),
						boost::bind(&handler_insert_value_of_ordered<unsafe_midx_type, index_id1_type, iterator1_type>,
									_1, _2, boost::ref(call_count)));

		yggr_test_assert(call_count == 2);

		yggr_test_assert(bright);
		yggr_test_assert(cont.size() == 2);

		bright = bright &&
			cont.get<0>().
				insert(boost::bind(&safe_index0_type::base_type::cend, _1),
						arr[0],
						boost::bind(&handler_insert_value_of_ordered<unsafe_midx_type, index_id0_type, iterator0_type>,
									_1, _2, boost::ref(call_count)));

		yggr_test_assert(call_count == 3);
		yggr_test_assert(bright);
		yggr_test_assert(cont.size() == 2);
	}

	// extract and insert
	{
		typedef yggr::safe_container::nth_index<mi_cont_type, 0>::type safe_index0_type;
		typedef safe_index0_type::index_id_type index_id0_type;
		typedef safe_index0_type::iterator iterator0_type;
		typedef safe_index0_type::node_type node0_type;
		typedef safe_index0_type::insert_return_type insert_return0_type;
		typedef safe_index0_type::base_insert_return_type base_insert_return0_type;

		typedef yggr::safe_container::nth_index<mi_cont_type, 1>::type safe_index1_type;
		typedef safe_index1_type::index_id_type index_id1_type;
		typedef safe_index1_type::iterator iterator1_type;
		typedef safe_index1_type::node_type node1_type;
		typedef safe_index1_type::insert_return_type insert_return1_type;
		typedef safe_index1_type::base_insert_return_type base_insert_return1_type;

		mi_cont_type cont;
		cont.insert(arr[0]);
		cont.insert(arr[1]);
		cont.insert(arr[2]);

		// 0, 0
		{
			std::size_t old_size = cont.size();
			node0_type node = cont.extract(1);
			yggr_test_assert(!node.empty());
			yggr_test_assert(cont.size() == old_size - 1);
			yggr_test_assert(node.value() == arr[0]);

			insert_return0_type ins_ret = cont.insert(boost::move(node));
			yggr_test_assert(ins_ret.inserted);
			yggr_test_assert(ins_ret.node.empty());
			yggr_test_assert(cont.size() == old_size);

			node0_type node2 = cont.extract(5);
			yggr_test_assert(node2.empty());
			yggr_test_assert(cont.size() == old_size);
		}

		{
			std::size_t old_size = cont.size();
			node0_type node = cont.extract(1);
			yggr_test_assert(!node.empty());
			yggr_test_assert(cont.size() == old_size - 1);
			yggr_test_assert(node.value() == arr[0]);

			insert_return0_type ins_ret =
				cont.insert(
					boost::move(node),
					boost::bind(
						&handler_insert_node_of_kv<unsafe_midx_type, index_id0_type, base_insert_return0_type>,
						_1, _2));
			yggr_test_assert(ins_ret.inserted);
			yggr_test_assert(ins_ret.node.empty());
			yggr_test_assert(cont.size() == old_size);

			node0_type node2 = cont.extract(5);
			yggr_test_assert(node2.empty());
			yggr_test_assert(cont.size() == old_size);
		}

		{
			std::size_t old_size = cont.size();
			node0_type node = cont.extract(2);
			yggr_test_assert(!node.empty());
			yggr_test_assert(cont.size() == old_size - 1);
			yggr_test_assert(node.value() == arr[1]);

			bool ins_ret = cont.insert(boost::bind(&safe_index0_type::base_type::cbegin, _1), boost::move(node));
			yggr_test_assert(ins_ret);
			yggr_test_assert(cont.size() == old_size);
		}

		{
			std::size_t old_size = cont.size();
			node0_type node = cont.extract(3);
			yggr_test_assert(!node.empty());
			yggr_test_assert(cont.size() == old_size - 1);
			yggr_test_assert(node.value() == arr[2]);

			bool ins_ret =
				cont.insert(
					boost::bind(&safe_index0_type::base_type::cbegin, _1),
					boost::move(node),
					boost::bind(
						&handler_insert_node_of_kv_hint<unsafe_midx_type, index_id0_type, iterator0_type>,
						_1, _2));
			yggr_test_assert(ins_ret);
			yggr_test_assert(cont.size() == old_size);
		}

		{
			node0_type node = cont.extract(1);
			yggr_test_assert(!node.empty());
			yggr_test_assert(node.value() == arr[0]);

			cont.insert(arr[0]);

			insert_return0_type ins_ret = cont.insert(boost::move(node));
			yggr_test_assert(!ins_ret.inserted);
			yggr_test_assert(!ins_ret.node.empty());
		}

		{
			std::size_t old_size = cont.size();
			node0_type node = cont.extract(boost::bind(&safe_index0_type::base_type::cbegin, _1));
			yggr_test_assert(!node.empty());
			yggr_test_assert(cont.size() == old_size - 1);

			bool ins_ret = cont.insert(boost::bind(&safe_index0_type::base_type::cbegin, _1), boost::move(node));
			yggr_test_assert(ins_ret);
			yggr_test_assert(cont.size() == old_size);
		}

		// 0, 1
		{
			std::size_t old_size = cont.size();
			node0_type node = cont.get<0>().extract(1);
			yggr_test_assert(!node.empty());
			yggr_test_assert(cont.size() == old_size - 1);
			yggr_test_assert(node.value() == arr[0]);

			insert_return1_type ins_ret = cont.get<1>().insert(boost::move(node));
			yggr_test_assert(ins_ret.inserted);
			yggr_test_assert(ins_ret.node.empty());
			yggr_test_assert(cont.size() == old_size);
		}

		{
			std::size_t old_size = cont.size();
			node0_type node = cont.get<0>().extract(1);
			yggr_test_assert(!node.empty());
			yggr_test_assert(cont.size() == old_size - 1);
			yggr_test_assert(node.value() == arr[0]);

			insert_return1_type ins_ret =
				cont.get<1>().insert(
					boost::move(node),
					boost::bind(
						&handler_insert_node_of_kv<unsafe_midx_type, index_id1_type, base_insert_return1_type>,
						_1, _2));
			yggr_test_assert(ins_ret.inserted);
			yggr_test_assert(ins_ret.node.empty());
			yggr_test_assert(cont.size() == old_size);
		}

		{
			std::size_t old_size = cont.size();
			node0_type node = cont.get<0>().extract(2);
			yggr_test_assert(!node.empty());
			yggr_test_assert(cont.size() == old_size - 1);
			yggr_test_assert(node.value() == arr[1]);

			bool ins_ret = cont.get<1>().insert(boost::bind(&safe_index1_type::base_type::cbegin, _1), boost::move(node));
			yggr_test_assert(ins_ret);
			yggr_test_assert(cont.size() == old_size);
		}

		{
			std::size_t old_size = cont.size();
			node0_type node = cont.get<0>().extract(3);
			yggr_test_assert(!node.empty());
			yggr_test_assert(cont.size() == old_size - 1);
			yggr_test_assert(node.value() == arr[2]);

			bool ins_ret =
				cont.get<1>().insert(
					boost::bind(&safe_index1_type::base_type::cbegin, _1),
					boost::move(node),
					boost::bind(
						&handler_insert_node_of_kv_hint<unsafe_midx_type, index_id1_type, iterator1_type>,
						_1, _2));
			yggr_test_assert(ins_ret);
			yggr_test_assert(cont.size() == old_size);
		}

		{
			node0_type node = cont.get<0>().extract(1);
			yggr_test_assert(!node.empty());
			yggr_test_assert(node.value() == arr[0]);

			cont.get<0>().insert(arr[0]);

			insert_return1_type ins_ret = cont.get<1>().insert(boost::move(node));
			yggr_test_assert(!ins_ret.inserted);
			yggr_test_assert(!ins_ret.node.empty());
		}

		{
			std::size_t old_size = cont.size();
			node0_type node = cont.get<0>().extract(boost::bind(&safe_index0_type::base_type::cbegin, _1));
			yggr_test_assert(!node.empty());
			yggr_test_assert(cont.size() == old_size - 1);

			bool ins_ret = cont.get<1>().insert(boost::bind(&safe_index1_type::base_type::cbegin, _1), boost::move(node));
			yggr_test_assert(ins_ret);
			yggr_test_assert(cont.size() == old_size);
		}

		// 1, 1
		{
			std::size_t old_size = cont.size();
			node1_type node = cont.get<1>().extract(1);
			yggr_test_assert(!node.empty());
			yggr_test_assert(cont.size() == old_size - 1);
			yggr_test_assert(node.value() == arr[0]);

			insert_return1_type ins_ret = cont.get<1>().insert(boost::move(node));
			yggr_test_assert(ins_ret.inserted);
			yggr_test_assert(ins_ret.node.empty());
			yggr_test_assert(cont.size() == old_size);

			node1_type node2 = cont.get<1>().extract(5);
			yggr_test_assert(node2.empty());
			yggr_test_assert(cont.size() == old_size);
		}

		{
			std::size_t old_size = cont.size();
			node1_type node = cont.get<1>().extract(1);
			yggr_test_assert(!node.empty());
			yggr_test_assert(cont.size() == old_size - 1);
			yggr_test_assert(node.value() == arr[0]);

			insert_return1_type ins_ret =
				cont.get<1>().insert(
					boost::move(node),
					boost::bind(
						&handler_insert_node_of_kv<unsafe_midx_type, index_id1_type, base_insert_return1_type>,
						_1, _2));
			yggr_test_assert(ins_ret.inserted);
			yggr_test_assert(ins_ret.node.empty());
			yggr_test_assert(cont.size() == old_size);

			node1_type node2 = cont.get<1>().extract(5);
			yggr_test_assert(node2.empty());
			yggr_test_assert(cont.size() == old_size);
		}

		{
			std::size_t old_size = cont.size();
			node1_type node = cont.get<1>().extract(2);
			yggr_test_assert(!node.empty());
			yggr_test_assert(cont.size() == old_size - 1);
			yggr_test_assert(node.value() == arr[1]);

			bool ins_ret = cont.get<1>().insert(boost::bind(&safe_index1_type::base_type::cbegin, _1), boost::move(node));
			yggr_test_assert(ins_ret);
			yggr_test_assert(cont.size() == old_size);
		}

		{
			std::size_t old_size = cont.size();
			node1_type node = cont.get<1>().extract(3);
			yggr_test_assert(!node.empty());
			yggr_test_assert(cont.size() == old_size - 1);
			yggr_test_assert(node.value() == arr[2]);

			bool ins_ret =
				cont.get<1>().insert(
					boost::bind(&safe_index1_type::base_type::cbegin, _1),
					boost::move(node),
					boost::bind(
						&handler_insert_node_of_kv_hint<unsafe_midx_type, index_id1_type, iterator1_type>,
						_1, _2));
			yggr_test_assert(ins_ret);
			yggr_test_assert(cont.size() == old_size);
		}

		{
			node1_type node = cont.get<1>().extract(1);
			yggr_test_assert(!node.empty());
			yggr_test_assert(node.value() == arr[0]);

			cont.get<1>().insert(arr[0]);

			insert_return1_type ins_ret = cont.get<1>().insert(boost::move(node));
			yggr_test_assert(!ins_ret.inserted);
			yggr_test_assert(!ins_ret.node.empty());
		}

		{
			std::size_t old_size = cont.size();
			node1_type node = cont.get<1>().extract(boost::bind(&safe_index1_type::base_type::cbegin, _1));
			yggr_test_assert(!node.empty());
			yggr_test_assert(cont.size() == old_size - 1);

			bool ins_ret = cont.get<1>().insert(boost::bind(&safe_index1_type::base_type::cbegin, _1), boost::move(node));
			yggr_test_assert(ins_ret);
			yggr_test_assert(cont.size() == old_size);
		}
	}

	// erase
	yggr_test_assert(cont.get<0>().insert(A(100, 100, "100")));
	yggr_test_assert(cont.get<0>().insert(A(200, 100, "100")));
	yggr_test_assert(cont.get<0>().insert(A(300, 100, "100")));
	yggr_test_assert(cont.get<0>().insert(A(400, 100, "100")));
	yggr_test_assert(cont.size() == 19);

	yggr_test_assert(1 == cont.get<0>().erase(100));
	yggr_test_assert(3 == cont.get<A::tag_prm2>().erase(100));
	yggr_test_assert(cont.size() == 15);

	// replace
	yggr_test_assert(cont.get<0>().replace(boost::bind(&unsafe_ordered_unique_type::find<int>, _1, 0), A(0, 100, "0")));
	yggr_test_assert(cont.get<0>().replace(boost::bind(&unsafe_ordered_unique_type::find<int>, _1, 0), A(100, 100, "0")));
	yggr_test_assert(cont.get<A::tag_prm1>().replace(boost::bind(&unsafe_ordered_unique_type::find<int>, _1, 100), A(0, 100, "0")));
	yggr_test_assert(!cont.get<A::tag_prm1>().replace(boost::bind(&unsafe_ordered_unique_type::find<int>, _1, 0), A(1, 100, "0")));
	yggr_test_assert(!cont.get<A::tag_prm1>().replace(boost::bind(&unsafe_ordered_unique_type::cend, _1), A(1, 100, "0")));

	yggr_test_assert(cont.size() == 15);

	yggr_test_assert(cont.get<1>().find_and_replace(100, A(100, 100, "0")));
	yggr_test_assert(cont.get<A::tag_prm2>().find_and_replace(100, cont.get<A::tag_prm2>().key_comp(), A(0, 1, "0")));
	yggr_test_assert(cont.get<A::tag_prm2>().find_and_replace(1, A(100, 1, "0")));
	yggr_test_assert(!cont.get<A::tag_prm2>().find_and_replace(1, cont.get<A::tag_prm2>().key_comp(), A(2, 1, "0")));

	yggr_test_assert(cont.size() == 15);

	// modify

	A tmp1;

	yggr_test_assert(cont.get<0>().modify(boost::bind(&unsafe_ordered_unique_type::find<int>, _1, 2),
									boost::bind(&modifier<A, A>, _1, A(1, 100, "100"))));

	yggr_test_assert(cont.get<0>().modify(boost::bind(&unsafe_ordered_unique_type::find<int>, _1, 1),
									boost::bind(&modifier<A, A>, _1, A(1, 100, "100"), boost::ref(tmp1)),
									boost::bind(&rollback<A>, _1, boost::cref(tmp1)) ) );

	yggr_test_assert(cont.get<0>().modify(boost::bind(&unsafe_ordered_unique_type::find<int>, _1, 1),
									yggr::safe_container::make_optimization_multi_index_modifier<unsafe_midx_type>(
										A(2, 100, "100"),
										boost::bind(&modifier<A, A>, _1, A(1, 100, "100")))));

	yggr_test_assert(cont.get<0>().modify(boost::bind(&unsafe_ordered_unique_type::find<int>, _1, 1),
									yggr::safe_container::make_optimization_multi_index_modifier<unsafe_midx_type>(
										A(1, 100, "100"),
										boost::bind(&modifier<A, A>, _1, A(1, 100, "100"), boost::ref(tmp1))),
									boost::bind(&rollback<A>, _1, boost::cref(tmp1)) ) );

	yggr_test_assert(cont.size() == 15);

	yggr_test_assert(!cont.get<A::tag_prm1>().find_and_modify(2000,
													boost::bind(&modifier<A, A>, _1, A(1, 100, "100"), boost::ref(tmp1))));

	yggr_test_assert(!cont.get<A::tag_prm1>().find_and_modify(2000, cont.get<A::tag_prm1>().key_comp(),
													boost::bind(&modifier<A, A>, _1, A(1, 100, "100"), boost::ref(tmp1))));

	yggr_test_assert(!cont.get<A::tag_prm1>().find_and_modify(2000,
													boost::bind(&modifier<A, A>, _1, A(1, 100, "100"), boost::ref(tmp1)),
													boost::bind(&rollback<A>, _1, boost::cref(tmp1))));

	yggr_test_assert(!cont.get<A::tag_prm1>().find_and_modify(2, cont.get<A::tag_prm1>().key_comp(),
													boost::bind(&modifier<A, A>, _1, A(1, 100, "100"), boost::ref(tmp1)),
													boost::bind(&rollback<A>, _1, boost::cref(tmp1))));

	yggr_test_assert(cont.size() == 15);

	// modify key
	int tmp2;

	yggr_test_assert(cont.get<0>().modify_key(boost::bind(&unsafe_ordered_unique_type::find<int>, _1, 1),
									boost::bind(&modifier<int, int>, _1, 200)));

	yggr_test_assert(cont.get<0>().modify_key(boost::bind(&unsafe_ordered_unique_type::find<int>, _1, 200),
									boost::bind(&modifier<int, int>, _1, 200),
									boost::bind(&rollback<int>, _1, boost::cref(tmp2))));

	yggr_test_assert(cont.get<0>().modify_key(boost::bind(&unsafe_ordered_unique_type::find<int>, _1, 200),
									yggr::safe_container::make_optimization_multi_index_modifier<unsafe_midx_type>(
										A(200, 100, "100"),
										boost::bind(&modifier<int, int>, _1, 200))));

	yggr_test_assert(cont.get<0>().modify_key(boost::bind(&unsafe_ordered_unique_type::find<int>, _1, 200),
										yggr::safe_container::make_optimization_multi_index_modifier<unsafe_midx_type>(
											A(200, 100, "100"),
											boost::bind(&modifier<int, int>, _1, 200)),
									boost::bind(&rollback<int>, _1, boost::cref(tmp2))));

	yggr_test_assert(cont.size() == 15);

	yggr_test_assert(!cont.get<A::tag_prm1>().find_and_modify_key(2000,
														boost::bind(&modifier<int, int>, _1, 3, boost::ref(tmp2))));

	yggr_test_assert(!cont.get<A::tag_prm1>().find_and_modify_key(2000, cont.get<A::tag_prm1>().key_comp(),
														boost::bind(&modifier<int, int>, _1, 3, boost::ref(tmp2))));

	yggr_test_assert(!cont.get<A::tag_prm1>().find_and_modify_key(200,
														boost::bind(&modifier<int, int>, _1, 3, boost::ref(tmp2)),
														boost::bind(&rollback<int>, _1, boost::cref(tmp2))));

	yggr_test_assert(!cont.get<A::tag_prm1>().find_and_modify_key(200, cont.get<A::tag_prm1>().key_comp(),
														boost::bind(&modifier<int, int>, _1, 3, boost::ref(tmp2)),
														boost::bind(&rollback<int>, _1, boost::cref(tmp2))));
	yggr_test_assert(cont.size() == 15);

	tmp2 = 0;

	mi_cont_type::base_type base_cont(arr + 0, arr + 10);
	yggr_test_assert(cont.size() == 15);
	yggr_test_assert(base_cont.size() == 5);

	cont.get<0>().swap(base_cont.get<0>());
	yggr_test_assert(cont.size() == 5);
	yggr_test_assert(base_cont.size() == 15);

	cont.get<A::tag_prm1>().swap(base_cont.get<A::tag_prm1>());
	yggr_test_assert(cont.size() == 15);
	yggr_test_assert(base_cont.size() == 5);

	yggr_test_assert(!cont.erase(boost::bind(&unsafe_ordered_unique_type::cend, _1)));
	yggr_test_assert(cont.erase(boost::bind(&unsafe_ordered_unique_type::cbegin, _1)));
	yggr_test_assert(cont.get<1>().erase(boost::bind(&unsafe_ordered_non_unique_type::cbegin, _1),
								boost::bind(&unsafe_ordered_non_unique_type::cend, _1)));

	yggr_test_assert(cont.empty());

	{
		typedef yggr::safe_container::nth_index<mi_cont_type, 0>::type safe_index0_type;
		typedef safe_index0_type::index_id_type index_id0_type;
		typedef safe_index0_type::iterator iterator0_type;


		typedef yggr::safe_container::nth_index<mi_cont_type, 1>::type safe_index1_type;
		typedef safe_index1_type::index_id_type index_id1_type;
		typedef safe_index1_type::iterator iterator1_type;

		yggr_test_assert(cont.emplace(yggr::tag_non_handler()));
		yggr_test_assert(cont.emplace(yggr::tag_non_handler(), 1, 1, "1"));
		yggr_test_assert(cont.emplace_hint(boost::bind(&safe_index0_type::base_type::cend, _1), yggr::tag_non_handler()));
		yggr_test_assert(cont.emplace_hint(boost::bind(&safe_index0_type::base_type::cend, _1), yggr::tag_non_handler(), 1, 1, "1"));

		int call_count = 0;

		yggr_test_assert(!(cont.emplace(boost::bind(&handler_insert_value_of_ordered<unsafe_midx_type, index_id0_type, iterator0_type>,
											_1, _2, boost::ref(call_count)))));

		yggr_test_assert(!cont.get<1>().
					emplace(boost::bind(&handler_insert_value_of_ordered<unsafe_midx_type, index_id1_type, iterator1_type>,
											_1, _2, boost::ref(call_count)), 1, 1, "1"));

		yggr_test_assert(cont.emplace_hint(boost::bind(&safe_index0_type::base_type::cend, _1),
									boost::bind(&handler_insert_value_of_ordered<unsafe_midx_type, index_id0_type, iterator0_type>,
													_1, _2, boost::ref(call_count))));

		yggr_test_assert(cont.get<1>().
				emplace_hint(boost::bind(&safe_index1_type::base_type::cend, _1),
								boost::bind(&handler_insert_value_of_ordered<unsafe_midx_type, index_id1_type, iterator1_type>,
											_1, _2, boost::ref(call_count)), 1, 1, "1"));


		yggr_test_assert(call_count == 4);
		yggr_test_assert(cont.size() == 2);
	}

	cont.get<0>().clear();
	yggr_test_assert(cont.empty());

	cont.clear();
	yggr_test_assert(cont.empty());

	std::cout << "---------------test_modifiers_of_ordered end-------------------------" << std::endl;
}

template<typename Final, typename IndexID, typename Iterator>
bool handler_find_of_ordered(Final& final,
								const
									boost::fusion::pair
									<
										IndexID,
										Iterator
									>& rst, int& count)
{
	typedef boost::fusion::pair<IndexID, Iterator> rst_type;
	typedef typename rst_type::first_type index_id_type;
	typedef Final midx_type;
	typedef typename
		boost::multi_index::nth_index
		<
			midx_type, index_id_type::first::value
		>::type now_index_type;

	++count;
	return rst.second != final.template get<index_id_type::first::value>().end();
}

template<typename Final, typename IndexID, typename Iterator>
int handler_equal_range_of_ordered(Final& final,
									const
										boost::fusion::pair
										<
											IndexID,
											std::pair<Iterator, Iterator>
										>& rst, int& count)
{
	typedef boost::fusion::pair<IndexID, Iterator> rst_type;
	typedef typename rst_type::first_type index_id_type;
	typedef Final midx_type;
	typedef typename
		boost::multi_index::nth_index
		<
			midx_type, index_id_type::first::value
		>::type now_index_type;

	++count;
	return std::distance(rst.second.first, rst.second.second);
}

template<typename Final, typename IndexID, typename Iterator>
bool handler_bound_of_ordered(Final& final,
								const
									boost::fusion::pair
									<
										IndexID,
										Iterator
									>& rst, int chk)
{
	typedef boost::fusion::pair<IndexID, Iterator> rst_type;
	typedef typename rst_type::first_type index_id_type;
	typedef Final midx_type;
	typedef typename
		boost::multi_index::nth_index
		<
			midx_type, index_id_type::first::value
		>::type now_index_type;

	return (rst.second != final.template get<index_id_type::first::value>().end())
			&&  ((final.template get<index_id_type::first::value>().key_extractor())(*(rst.second)) == chk);
}

template<typename Final, typename IndexID, typename Iterator>
bool handler_range_of_ordered(Final& final,
								const
									boost::fusion::pair
									<
										IndexID,
										std::pair<Iterator, Iterator>
									>& rst, std::size_t chk)
{
	typedef boost::fusion::pair<IndexID, std::pair<Iterator, Iterator> > rst_type;
	typedef typename rst_type::first_type index_id_type;
	typedef Final midx_type;
	typedef typename
		boost::multi_index::nth_index
		<
			midx_type, index_id_type::first::value
		>::type now_index_type;

	return std::distance(rst.second.first, rst.second.second) == chk;
}

void test_lookup_of_ordered(void)
{
	typedef yggr::safe_container::safe_multi_index_container<
			A,
			boost::multi_index::indexed_by<
				boost::multi_index::ordered_unique<
					boost::multi_index::tag<A::tag_prm1>,
					BOOST_MULTI_INDEX_MEMBER(A, int, prm1) >,

				boost::multi_index::ordered_non_unique<
					boost::multi_index::tag<A::tag_prm2>,
					BOOST_MULTI_INDEX_MEMBER(A, int, prm2)>
			>
		> mi_cont_type;

	typedef mi_cont_type::base_type unsafe_midx_type;

	typedef boost::multi_index::nth_index<mi_cont_type, 0>::type safe_index0_type;
	typedef safe_index0_type::index_id_type index_id0_type;
	typedef safe_index0_type::iterator iterator0_type;

	typedef boost::multi_index::nth_index<mi_cont_type, 1>::type safe_index1_type;
	typedef safe_index1_type::index_id_type index_id1_type;
	typedef safe_index1_type::iterator iterator1_type;

	A arr[10] = {A(1, 1, "aaa"), A(2, 2, "bbb"), A(3, 3, "ccc"), A(4, 4, "ddd")};
	mi_cont_type cont(arr + 0, arr + 10);


	yggr_test_assert(cont.find(1));
	yggr_test_assert(!cont.find(100));

	{
		int call_count = 0;

		yggr_test_assert(cont.find(1,
							boost::bind(
								&handler_find_of_ordered<
									unsafe_midx_type, index_id0_type, iterator0_type>, _1, _2, boost::ref(call_count)) ));

		const safe_index0_type& idx0_cref = cont.get<0>();

		yggr_test_assert(call_count == 1);

		yggr_test_assert(idx0_cref.find(1,
							boost::bind(
								&handler_find_of_ordered<
									const unsafe_midx_type, index_id0_type, iterator0_type>, _1, _2, boost::ref(call_count)) ));

		yggr_test_assert(call_count == 2);

		yggr_test_assert(!cont.find(100,
							boost::bind(
								&handler_find_of_ordered<
									unsafe_midx_type, index_id0_type, iterator0_type>, _1, _2, boost::ref(call_count)) ));

		yggr_test_assert(call_count == 3);
	}

	yggr_test_assert(cont.get<1>().find(1, cont.key_comp()));
	yggr_test_assert(!cont.get<1>().find(100, cont.key_comp()));

	{
		int call_count = 0;

		yggr_test_assert(cont.find(1, cont.key_comp(),
							boost::bind(
								&handler_find_of_ordered<
									unsafe_midx_type, index_id0_type, iterator0_type>, _1, _2, boost::ref(call_count)) ));

		const safe_index0_type& idx0_cref = cont.get<0>();

		yggr_test_assert(call_count == 1);

		yggr_test_assert(idx0_cref.find(1, cont.key_comp(),
							boost::bind(
								&handler_find_of_ordered<
									const unsafe_midx_type, index_id0_type, iterator0_type>, _1, _2, boost::ref(call_count)) ));

		yggr_test_assert(call_count == 2);

		yggr_test_assert(!cont.find(100, cont.key_comp(),
							boost::bind(
								&handler_find_of_ordered<
									unsafe_midx_type, index_id0_type, iterator0_type>, _1, _2, boost::ref(call_count)) ));

		yggr_test_assert(call_count == 3);
	}

	yggr_test_assert(cont.count(1) == 1);
	yggr_test_assert(cont.count(100) == 0);

	yggr_test_assert(cont.count(1, cont.key_comp()) == 1);
	yggr_test_assert(cont.count(100, cont.key_comp()) == 0);

	{
		int call_count = 0;
		yggr_test_assert(1 == cont.equal_range(1,
								boost::bind(
									&handler_equal_range_of_ordered<
										unsafe_midx_type, index_id0_type, iterator0_type>, _1, _2, boost::ref(call_count))));

		yggr_test_assert(call_count == 1);

		yggr_test_assert(0 == cont.equal_range(100,
								boost::bind(
									&handler_equal_range_of_ordered<
										unsafe_midx_type, index_id0_type, iterator0_type>, _1, _2, boost::ref(call_count))));

		yggr_test_assert(call_count == 2);
	}

	{
		int call_count = 0;
		yggr_test_assert(1 == cont.equal_range(1, cont.key_comp(),
								boost::bind(
									&handler_equal_range_of_ordered<
										unsafe_midx_type, index_id0_type, iterator0_type>, _1, _2, boost::ref(call_count))));

		yggr_test_assert(call_count == 1);

		yggr_test_assert(0 == cont.equal_range(100, cont.key_comp(),
								boost::bind(
									&handler_equal_range_of_ordered<
										unsafe_midx_type, index_id0_type, iterator0_type>, _1, _2, boost::ref(call_count))));

		yggr_test_assert(call_count == 2);
	}

	{
		yggr_test_assert(cont.lower_bound(1,
								boost::bind(
									&handler_bound_of_ordered<
										unsafe_midx_type, index_id0_type, iterator0_type>, _1, _2, 1)));

		yggr_test_assert(cont.get<1>().lower_bound(1, cont.get<1>().key_comp(),
								boost::bind(
									&handler_bound_of_ordered<
										unsafe_midx_type, index_id1_type, iterator1_type>, _1, _2, 1)));

		yggr_test_assert(!cont.lower_bound(100,
								boost::bind(
									&handler_bound_of_ordered<
										unsafe_midx_type, index_id0_type, iterator0_type>, _1, _2, 100)));

		yggr_test_assert(!cont.get<1>().lower_bound(100, cont.get<1>().key_comp(),
								boost::bind(
									&handler_bound_of_ordered<
										unsafe_midx_type, index_id1_type, iterator1_type>, _1, _2, 100)));
	}

	{
		yggr_test_assert(cont.upper_bound(1,
								boost::bind(
									&handler_bound_of_ordered<
										unsafe_midx_type, index_id0_type, iterator0_type>, _1, _2, 2)));

		yggr_test_assert(cont.get<1>().upper_bound(1, cont.get<1>().key_comp(),
								boost::bind(
									&handler_bound_of_ordered<
										unsafe_midx_type, index_id1_type, iterator1_type>, _1, _2, 2)));

		yggr_test_assert(!cont.upper_bound(100,
								boost::bind(
									&handler_bound_of_ordered<
										unsafe_midx_type, index_id0_type, iterator0_type>, _1, _2, 100)));

		yggr_test_assert(!cont.get<1>().upper_bound(100, cont.get<1>().key_comp(),
								boost::bind(
									&handler_bound_of_ordered<
										unsafe_midx_type, index_id1_type, iterator1_type>, _1, _2, 100)));
	}

	{
		// 1 < x < 4 -> A(2, 2, "bbb"), A(3, 3, "ccc")
		yggr_test_assert(cont.range(boost::bind(cont.key_comp(), 1, _1), boost::bind(cont.key_comp(), _1, 4),
								boost::bind(
									&handler_range_of_ordered<
										unsafe_midx_type, index_id0_type, iterator0_type>, _1, _2, 2)));

		// -1 < x < 100 -> A(0, 0, ""), A(1, 1, "aaa"), A(2, 2, "bbb"), A(3, 3, "ccc"), A(4, 4, "ddd")
		yggr_test_assert(cont.range(boost::bind(cont.key_comp(), -1, _1), boost::bind(cont.key_comp(), _1, 100),
								boost::bind(
									&handler_range_of_ordered<
										unsafe_midx_type, index_id0_type, iterator0_type>, _1, _2, 5)));
	}

	{
		A a;

		bool bret = cont.get_value(1, a);
		yggr_test_assert(bret);
		yggr_test_assert(a == arr[0]);

		yggr_test_assert(!cont.get_value(100, a));
	}

	{
		A a;

		bool bret = cont.get<1>().get_value(1, cont.key_comp(), a);
		yggr_test_assert(bret);
		yggr_test_assert(a == arr[0]);

		yggr_test_assert(!cont.get<1>().get_value(100, cont.key_comp(), a));
	}

	std::cout << "---------------test_lookup_of_ordered end-------------------------" << std::endl;

}

void test_set_operations_of_ordered(void)
{
	typedef yggr::safe_container::safe_multi_index_container<
			A,
			boost::multi_index::indexed_by<
				boost::multi_index::ordered_unique<
					boost::multi_index::tag<A::tag_prm1>,
					BOOST_MULTI_INDEX_MEMBER(A, int, prm1) >,

				boost::multi_index::ordered_non_unique<
					boost::multi_index::tag<A::tag_prm2>,
					BOOST_MULTI_INDEX_MEMBER(A, int, prm2)>
			>
		> mi_cont_type;

	A arr[10] = {A(1, 1, "aaa"), A(2, 2, "bbb"), A(3, 3, "ccc"), A(4, 3, "ddd")};
	mi_cont_type cont(arr + 0, arr + 10);

	yggr_test_assert(1 == cont.get<A::tag_prm1>().count(1));
	yggr_test_assert(1 == cont.get<0>().count(2));
	yggr_test_assert(0 == cont.get<0>().count(100));

	yggr_test_assert(1 == cont.get<A::tag_prm2>().count(2));
	yggr_test_assert(2 == cont.get<1>().count(3));
	yggr_test_assert(0 == cont.get<1>().count(100));

	yggr_test_assert(cont.size() == 5);

	mi_cont_type::base_type base_cont(arr + 0, arr + 10);
	yggr_test_assert(cont.size() == 5);

	yggr_test_assert(1 == cont.get<A::tag_prm1>().count(1, std::less<int>()));

	yggr_test_assert(1 == cont.get<0>().count(2, std::less<int>()));

	yggr_test_assert(0 == cont.get<0>().count(100, std::less<int>()));

	yggr_test_assert(1 == cont.get<A::tag_prm2>().count(2, std::less<int>()));

	yggr_test_assert(2 == cont.get<1>().count(3, std::less<int>()));

	yggr_test_assert(0 == cont.get<1>().count(100, std::less<int>()));

	std::cout << "---------------test_set_operations_of_ordered end-------------------" << std::endl;
}

void test_swap_of_ordered(void)
{
	typedef yggr::safe_container::safe_multi_index_container<
			A,
			boost::multi_index::indexed_by<
				boost::multi_index::ordered_unique<
					boost::multi_index::tag<A::tag_prm1>,
					BOOST_MULTI_INDEX_MEMBER(A, int, prm1) >,

				boost::multi_index::ordered_non_unique<
					boost::multi_index::tag<A::tag_prm2>,
					BOOST_MULTI_INDEX_MEMBER(A, int, prm2)>
			>
		> mi_cont_type;
	test_swap<mi_cont_type>();
	std::cout << "---------------test_swap_of_ordered end-----------------" << std::endl;
}

void test_operator_set_of_ordered(void)
{
	typedef yggr::safe_container::safe_multi_index_container<
			A,
			boost::multi_index::indexed_by<
				boost::multi_index::ordered_unique<
					boost::multi_index::tag<A::tag_prm1>,
					BOOST_MULTI_INDEX_MEMBER(A, int, prm1) >,

				boost::multi_index::ordered_non_unique<
					boost::multi_index::tag<A::tag_prm2>,
					BOOST_MULTI_INDEX_MEMBER(A, int, prm2)>
			>
		> mi_cont_type;
	test_operator_set<mi_cont_type>();
	std::cout << "---------------test_operator_set_of_ordered end-----------------" << std::endl;
}

void test_comparer_of_ordered(void)
{
	typedef yggr::safe_container::safe_multi_index_container<
			A,
			boost::multi_index::indexed_by<
				boost::multi_index::ordered_unique<
					boost::multi_index::tag<A::tag_prm1>,
					BOOST_MULTI_INDEX_MEMBER(A, int, prm1) >,

				boost::multi_index::ordered_non_unique<
					boost::multi_index::tag<A::tag_prm2>,
					BOOST_MULTI_INDEX_MEMBER(A, int, prm2)>
			>
		> mi_cont_type;
	test_compare_of_full<mi_cont_type>();
	std::cout << "---------------test_comparer_of_ordered end-----------------" << std::endl;
}


void test_safe_other_of_ordered(void)
{
	typedef
		yggr::safe_container::safe_multi_index_container
		<
			A,
			boost::multi_index::indexed_by<
				boost::multi_index::ordered_unique<
					boost::multi_index::tag<A::tag_prm1>,
					BOOST_MULTI_INDEX_MEMBER(A, int, prm1) >,

				boost::multi_index::ordered_non_unique<
					boost::multi_index::tag<A::tag_prm2>,
					BOOST_MULTI_INDEX_MEMBER(A, int, prm2)>
			>
		> mi_cont_type;

	typedef mi_cont_type::base_type unsafe_midx_type;

	typedef boost::multi_index::nth_index<mi_cont_type, 0>::type safe_index0_type;
	typedef safe_index0_type::index_id_type index_id0_type;
	typedef safe_index0_type::iterator iterator0_type;

	typedef boost::multi_index::nth_index<mi_cont_type, 1>::type safe_index1_type;
	typedef safe_index1_type::index_id_type index_id1_type;
	typedef safe_index1_type::iterator iterator1_type;

	test_safe_other_gernel<mi_cont_type>();

	A arr[10] = {A(1, 1, "aaa"), A(2, 2, "bbb"), A(3, 3, "ccc"), A(4, 3, "ddd")};

	mi_cont_type cont(arr + 0, arr + 10);

	yggr_test_assert(cont.is_exists(1));
	yggr_test_assert(!cont.is_exists(100));
	yggr_test_assert(cont.is_exists(1, cont.key_comp()));
	yggr_test_assert(!cont.is_exists(100, cont.key_comp()));

	{
		A beg_val, rbeg_val;

		mi_cont_type cont_empty;
		yggr_test_assert(!cont_empty.get_begin_value(beg_val));
		yggr_test_assert(!cont_empty.get_rbegin_value(rbeg_val));

		try
		{
			cont_empty.begin_value();
			yggr_test_assert(false);
		}
		catch(const mi_cont_type::error_type&)
		{
		}

		try
		{
			cont_empty.rbegin_value();
			yggr_test_assert(false);
		}
		catch(const mi_cont_type::error_type&)
		{
		}

		cont.get_begin_value(beg_val);
		cont.get_rbegin_value(rbeg_val);

		yggr_test_assert(beg_val == cont.begin_value());
		yggr_test_assert(rbeg_val == cont.rbegin_value());
	}

	{
		int call_count = 0;
		cont.for_each_iterator_reverse(boost::bind(&handler_for_each<typename safe_index0_type::reverse_iterator>,
														_1, boost::ref(call_count)));
		yggr_test_assert(call_count == cont.size());

		call_count = 0;
		cont.for_each_iterator_reverse(boost::bind(&handler_for_each<typename safe_index0_type::reverse_iterator>,
														_1, boost::ref(call_count)),
										boost::bind(&handler_filter<typename safe_index0_type::reverse_iterator>,
														_1, false));
		yggr_test_assert(call_count == 0);

		call_count = 0;
		cont.for_each_iterator_reverse(boost::bind(&handler_for_each<typename safe_index0_type::reverse_iterator>,
														_1, boost::ref(call_count)),
										boost::bind(&handler_filter<typename safe_index0_type::reverse_iterator>,
														_1, true));
		yggr_test_assert(call_count == cont.size());
	}

	{
		int call_count = 0;
		cont.for_each_reverse(boost::bind(&handler_for_each<typename safe_index0_type::value_type>,
											_1, boost::ref(call_count)));
		yggr_test_assert(call_count == cont.size());

		call_count = 0;
		cont.for_each_reverse(boost::bind(&handler_for_each<typename safe_index0_type::value_type>,
											_1, boost::ref(call_count)),
								boost::bind(&handler_filter<typename safe_index0_type::value_type>,
											_1, false));
		yggr_test_assert(call_count == 0);

		call_count = 0;
		cont.for_each_reverse(boost::bind(&handler_for_each<typename safe_index0_type::value_type>,
											_1, boost::ref(call_count)),
								boost::bind(&handler_filter<typename safe_index0_type::value_type>,
											_1, true));
		yggr_test_assert(call_count == cont.size());
	}

	std::cout << "---------------test_safe_other_of_ordered end-----------------" << std::endl;
}

//------------------------------ranked--------------------------------

#ifdef YGGR_BOOST_MULTI_INDEX_HAS_RANKED_INDEX

void test_construct_of_ranked(void)
{
	typedef yggr::safe_container::safe_multi_index_container<
			A,
			boost::multi_index::indexed_by<
				boost::multi_index::ranked_unique<
					boost::multi_index::tag<A::tag_prm1>,
					BOOST_MULTI_INDEX_MEMBER(A, int, prm1) >,

				boost::multi_index::ranked_non_unique<
					boost::multi_index::tag<A::tag_prm2>,
					BOOST_MULTI_INDEX_MEMBER(A, int, prm2)>
			>
		> mi_cont_type;
	test_construct<mi_cont_type>();

	std::cout << "---------------test_construct_of_ranked end-------------------------" << std::endl;
}

void test_observers_of_ranked(void)
{
	typedef yggr::safe_container::safe_multi_index_container<
			A,
			boost::multi_index::indexed_by<
				boost::multi_index::ranked_unique<
					boost::multi_index::tag<A::tag_prm1>,
					BOOST_MULTI_INDEX_MEMBER(A, int, prm1) >,

				boost::multi_index::ranked_non_unique<
					boost::multi_index::tag<A::tag_prm2>,
					BOOST_MULTI_INDEX_MEMBER(A, int, prm2)>
			>
		> mi_cont_type;

	A arr[10] = {A(1, 1, "aaa"), A(2, 2, "bbb"), A(3, 3, "ccc"), A(4, 3, "ddd")};
	mi_cont_type cont(arr + 0, arr + 10);
	mi_cont_type cont2(cont.get_allocator());

	cont.get<0>().get_allocator();
	cont.get<1>().get_allocator();
	cont.get<A::tag_prm1>().get_allocator();
	cont.get<A::tag_prm2>().get_allocator();

	cont.get<0>().key_extractor();
	cont.get<1>().key_extractor();
	cont.get<A::tag_prm1>().key_extractor();
	cont.get<A::tag_prm2>().key_extractor();

	yggr_test_assert(cont.get<0>().key_extractor()(arr[3]) == 4);
	yggr_test_assert(cont.get<1>().key_extractor()(arr[3]) == 3);

	cont.get<0>().key_comp();
	cont.get<1>().key_comp();
	cont.get<A::tag_prm1>().key_comp();
	cont.get<A::tag_prm2>().key_comp();

	cont.get<0>().value_comp();
	cont.get<1>().value_comp();
	cont.get<A::tag_prm1>().value_comp();
	cont.get<A::tag_prm2>().value_comp();

	std::cout << "---------------test_observers_of_ranked end-------------------------" << std::endl;
}

void test_capacity_of_ranked(void)
{
	typedef yggr::safe_container::safe_multi_index_container<
			A,
			boost::multi_index::indexed_by<
				boost::multi_index::ranked_unique<
					boost::multi_index::tag<A::tag_prm1>,
					BOOST_MULTI_INDEX_MEMBER(A, int, prm1) >,

				boost::multi_index::ranked_non_unique<
					boost::multi_index::tag<A::tag_prm2>,
					BOOST_MULTI_INDEX_MEMBER(A, int, prm2)>
			>
		> mi_cont_type;

	A arr[10] = {A(1, 1, "aaa"), A(2, 2, "bbb"), A(3, 3, "ccc"), A(4, 3, "ddd")};
	mi_cont_type cont(arr + 0, arr + 10);
	mi_cont_type::base_type base_cont(arr + 0, arr + 10);
	yggr_test_assert(cont.size() == 5);
	yggr_test_assert(!cont.empty());
	yggr_test_assert(base_cont.size() == cont.size());
	yggr_test_assert(base_cont.max_size() == cont.max_size());

	yggr_test_assert(cont.get<0>().empty() == cont.get<A::tag_prm1>().empty());
	yggr_test_assert(cont.get<0>().empty() == cont.get<A::tag_prm2>().empty());

	yggr_test_assert(cont.get<1>().empty() == cont.get<A::tag_prm2>().empty());

	yggr_test_assert(cont.get<1>().empty() == base_cont.empty());

	yggr_test_assert(cont.get<0>().size() == cont.get<A::tag_prm1>().size());

	yggr_test_assert(cont.get<0>().size() == base_cont.size());

	yggr_test_assert(cont.get<0>().max_size() == base_cont.max_size());

	yggr_test_assert(cont.get<0>().max_size() == base_cont.get<0>().max_size());

	yggr_test_assert(cont.get<A::tag_prm2>().max_size() == base_cont.get<1>().max_size());

	std::cout << "---------------test_capacity_of_ranked end-------------------------" << std::endl;
}

template<typename Final, typename IndexID, typename Iterator>
bool handler_insert_value_of_ranked(Final& final,
										const
											boost::fusion::pair
											<
												IndexID,
												std::pair<Iterator, bool>
											>& rst, int& count)
{
	typedef boost::fusion::pair<IndexID, std::pair<Iterator, bool> > rst_type;
	typedef typename rst_type::first_type index_id_type;
	typedef Final midx_type;
	typedef typename
		boost::multi_index::nth_index
		<
			midx_type, index_id_type::first::value
		>::type now_index_type;

	typedef typename
		boost::mpl::if_
		<
			boost::is_const<Final>,
			const now_index_type&,
			now_index_type&
		>::type now_index_ref_type;

	++count;
	if(rst.second.second)
	{
		now_index_ref_type now_index = final.template get<index_id_type::first::value>();

		yggr_test_assert((now_index.find((now_index.key_extractor())(*rst.second.first)) != now_index.end()));
	}
	return rst.second.second;
}

void test_modifiers_of_ranked(void)
{
	typedef yggr::safe_container::safe_multi_index_container<
			A,
			boost::multi_index::indexed_by<
				boost::multi_index::ranked_unique<
					boost::multi_index::tag<A::tag_prm1>,
					BOOST_MULTI_INDEX_MEMBER(A, int, prm1) >,

				boost::multi_index::ranked_non_unique<
					boost::multi_index::tag<A::tag_prm2>,
					BOOST_MULTI_INDEX_MEMBER(A, int, prm2)>
			>
		> mi_cont_type;

	typedef mi_cont_type::base_type unsafe_midx_type;
	typedef boost::multi_index::nth_index<unsafe_midx_type, 0>::type unsafe_ranked_unique_type;
	typedef boost::multi_index::nth_index<unsafe_midx_type, 1>::type unsafe_ranked_non_unique_type;

	A arr[10] = {A(1, 1, "aaa"), A(2, 2, "bbb"), A(3, 3, "ccc"), A(4, 3, "ddd")};
	mi_cont_type cont(arr + 0, arr + 10);

	yggr_test_assert(cont.size() == 5);

// initializer_list
#ifndef BOOST_NO_CXX11_HDR_INITIALIZER_LIST
	{
		mi_cont_type cont1 = {A(1, 1, "aaa"), A(2, 2, "bbb"), A(3, 3, "ccc"), A(4, 3, "ddd")};
		yggr_test_assert(cont1.size() == 4);
	}

	{
		mi_cont_type cont1;
		cont1.insert({A(1, 1, "aaa"), A(2, 2, "bbb"), A(3, 3, "ccc"), A(4, 3, "ddd")});
		yggr_test_assert(cont1.size() == 4);
	}
#endif // BOOST_NO_CXX11_HDR_INITIALIZER_LIST

	// insert
	yggr_test_assert(cont.get<0>().insert(A(5, 3, "eee")));
	yggr_test_assert(cont.size() == 6);
	yggr_test_assert(!cont.get<A::tag_prm1>().insert(A(3, 5, "cccc")));
	yggr_test_assert(cont.size() == 6);

	A arr2[10] = {A(10, 10, "a10"), A(20, 20, "b20"), A(30, 30, "c30"), A(40, 30, "d40")};
	yggr_test_assert(cont.get<A::tag_prm1>().insert(arr2 + 0, arr2 + 10));
	yggr_test_assert(cont.size() == 10);
	yggr_test_assert(cont.get<0>().insert(arr + 0, arr + 10));
	yggr_test_assert(cont.size() == 10);

	yggr_test_assert(cont.get<1>().insert(A(6, 3, "eee")));
	yggr_test_assert(cont.size() == 11);
	yggr_test_assert(!cont.get<A::tag_prm2>().insert(A(6, 3, "cccc")));
	yggr_test_assert(cont.size() == 11);

	A arr3[10] = {A(11, 11, "a11"), A(21, 21, "b21"), A(31, 31, "c31"), A(41, 31, "d41")};
	yggr_test_assert(cont.get<A::tag_prm2>().insert(arr3, arr3 + 10));
	yggr_test_assert(cont.size() == 15);
	yggr_test_assert(cont.get<1>().insert(arr3, arr3 + 10));
	yggr_test_assert(cont.size() == 15);

	{
		typedef yggr::safe_container::nth_index<mi_cont_type, 0>::type safe_index0_type;
		typedef safe_index0_type::index_id_type index_id0_type;
		typedef safe_index0_type::iterator iterator0_type;


		typedef yggr::safe_container::nth_index<mi_cont_type, 1>::type safe_index1_type;
		typedef safe_index1_type::index_id_type index_id1_type;
		typedef safe_index1_type::iterator iterator1_type;

		mi_cont_type cont;
		int call_count = 0;
		bool bright =
			cont.insert(arr[0],
						boost::bind(&handler_insert_value_of_ranked<unsafe_midx_type, index_id0_type, iterator0_type>,
									_1, _2, boost::ref(call_count)));

		yggr_test_assert(call_count == 1);

		A ins_data = arr[1];
		 bright = bright &&
			cont.get<1>().
				insert(boost::move(ins_data),
						boost::bind(&handler_insert_value_of_ranked<unsafe_midx_type, index_id1_type, iterator1_type>,
									_1, _2, boost::ref(call_count)));

		yggr_test_assert(call_count == 2);

		yggr_test_assert(bright);
		yggr_test_assert(cont.size() == 2);

		bright = bright &&
			cont.get<0>().
				insert(arr[0],
						boost::bind(&handler_insert_value_of_ranked<unsafe_midx_type, index_id0_type, iterator0_type>,
									_1, _2, boost::ref(call_count)));

		yggr_test_assert(call_count == 3);

		yggr_test_assert(!bright);
		yggr_test_assert(cont.size() == 2);
	}

	{
		typedef yggr::safe_container::nth_index<mi_cont_type, 0>::type safe_index0_type;
		typedef safe_index0_type::index_id_type index_id0_type;
		typedef safe_index0_type::iterator iterator0_type;


		typedef yggr::safe_container::nth_index<mi_cont_type, 1>::type safe_index1_type;
		typedef safe_index1_type::index_id_type index_id1_type;
		typedef safe_index1_type::iterator iterator1_type;

		mi_cont_type cont;
		bool bright =
			cont.insert(boost::bind(&safe_index0_type::base_type::cend, _1), arr[0]);


		A ins_data = arr[1];
		 bright = bright &&
			cont.get<1>().
				insert(boost::bind(&safe_index1_type::base_type::cend, _1),
						boost::move(ins_data));


		yggr_test_assert(bright);
		yggr_test_assert(cont.size() == 2);

		bright = bright &&
			cont.get<0>().insert(boost::bind(&safe_index0_type::base_type::cend, _1), arr[0]);

		yggr_test_assert(bright);
	}

	{
		typedef yggr::safe_container::nth_index<mi_cont_type, 0>::type safe_index0_type;
		typedef safe_index0_type::index_id_type index_id0_type;
		typedef safe_index0_type::iterator iterator0_type;


		typedef yggr::safe_container::nth_index<mi_cont_type, 1>::type safe_index1_type;
		typedef safe_index1_type::index_id_type index_id1_type;
		typedef safe_index1_type::iterator iterator1_type;

		mi_cont_type cont;
		int call_count = 0;
		bool bright =
			cont.insert(boost::bind(&safe_index0_type::base_type::cend, _1),
						arr[0],
						boost::bind(&handler_insert_value_of_ranked<unsafe_midx_type, index_id0_type, iterator0_type>,
									_1, _2, boost::ref(call_count)));

		yggr_test_assert(call_count == 1);

		A ins_data = arr[1];
		 bright = bright &&
			cont.get<1>().
				insert(boost::bind(&safe_index1_type::base_type::cend, _1),
						boost::move(ins_data),
						boost::bind(&handler_insert_value_of_ranked<unsafe_midx_type, index_id1_type, iterator1_type>,
									_1, _2, boost::ref(call_count)));

		yggr_test_assert(call_count == 2);

		yggr_test_assert(bright);
		yggr_test_assert(cont.size() == 2);

		bright = bright &&
			cont.get<0>().
				insert(boost::bind(&safe_index0_type::base_type::cend, _1),
						arr[0],
						boost::bind(&handler_insert_value_of_ranked<unsafe_midx_type, index_id0_type, iterator0_type>,
									_1, _2, boost::ref(call_count)));

		yggr_test_assert(call_count == 3);
		yggr_test_assert(bright);
		yggr_test_assert(cont.size() == 2);
	}

	// extract and insert
	{
		typedef yggr::safe_container::nth_index<mi_cont_type, 0>::type safe_index0_type;
		typedef safe_index0_type::index_id_type index_id0_type;
		typedef safe_index0_type::iterator iterator0_type;
		typedef safe_index0_type::node_type node0_type;
		typedef safe_index0_type::insert_return_type insert_return0_type;
		typedef safe_index0_type::base_insert_return_type base_insert_return0_type;

		typedef yggr::safe_container::nth_index<mi_cont_type, 1>::type safe_index1_type;
		typedef safe_index1_type::index_id_type index_id1_type;
		typedef safe_index1_type::iterator iterator1_type;
		typedef safe_index1_type::node_type node1_type;
		typedef safe_index1_type::insert_return_type insert_return1_type;
		typedef safe_index1_type::base_insert_return_type base_insert_return1_type;

		mi_cont_type cont;
		cont.insert(arr[0]);
		cont.insert(arr[1]);
		cont.insert(arr[2]);

		// 0, 0
		{
			std::size_t old_size = cont.size();
			node0_type node = cont.extract(1);
			yggr_test_assert(!node.empty());
			yggr_test_assert(cont.size() == old_size - 1);
			yggr_test_assert(node.value() == arr[0]);

			insert_return0_type ins_ret = cont.insert(boost::move(node));
			yggr_test_assert(ins_ret.inserted);
			yggr_test_assert(ins_ret.node.empty());
			yggr_test_assert(cont.size() == old_size);

			node0_type node2 = cont.extract(5);
			yggr_test_assert(node2.empty());
			yggr_test_assert(cont.size() == old_size);
		}

		{
			std::size_t old_size = cont.size();
			node0_type node = cont.extract(1);
			yggr_test_assert(!node.empty());
			yggr_test_assert(cont.size() == old_size - 1);
			yggr_test_assert(node.value() == arr[0]);

			insert_return0_type ins_ret =
				cont.insert(
					boost::move(node),
					boost::bind(
						&handler_insert_node_of_kv<unsafe_midx_type, index_id0_type, base_insert_return0_type>,
						_1, _2));
			yggr_test_assert(ins_ret.inserted);
			yggr_test_assert(ins_ret.node.empty());
			yggr_test_assert(cont.size() == old_size);

			node0_type node2 = cont.extract(5);
			yggr_test_assert(node2.empty());
			yggr_test_assert(cont.size() == old_size);
		}

		{
			std::size_t old_size = cont.size();
			node0_type node = cont.extract(2);
			yggr_test_assert(!node.empty());
			yggr_test_assert(cont.size() == old_size - 1);
			yggr_test_assert(node.value() == arr[1]);

			bool ins_ret = cont.insert(boost::bind(&safe_index0_type::base_type::cbegin, _1), boost::move(node));
			yggr_test_assert(ins_ret);
			yggr_test_assert(cont.size() == old_size);
		}

		{
			std::size_t old_size = cont.size();
			node0_type node = cont.extract(3);
			yggr_test_assert(!node.empty());
			yggr_test_assert(cont.size() == old_size - 1);
			yggr_test_assert(node.value() == arr[2]);

			bool ins_ret =
				cont.insert(
					boost::bind(&safe_index0_type::base_type::cbegin, _1),
					boost::move(node),
					boost::bind(
						&handler_insert_node_of_kv_hint<unsafe_midx_type, index_id0_type, iterator0_type>,
						_1, _2));
			yggr_test_assert(ins_ret);
			yggr_test_assert(cont.size() == old_size);
		}

		{
			node0_type node = cont.extract(1);
			yggr_test_assert(!node.empty());
			yggr_test_assert(node.value() == arr[0]);

			cont.insert(arr[0]);

			insert_return0_type ins_ret = cont.insert(boost::move(node));
			yggr_test_assert(!ins_ret.inserted);
			yggr_test_assert(!ins_ret.node.empty());
		}

		{
			std::size_t old_size = cont.size();
			node0_type node = cont.extract(boost::bind(&safe_index0_type::base_type::cbegin, _1));
			yggr_test_assert(!node.empty());
			yggr_test_assert(cont.size() == old_size - 1);

			bool ins_ret = cont.insert(boost::bind(&safe_index0_type::base_type::cbegin, _1), boost::move(node));
			yggr_test_assert(ins_ret);
			yggr_test_assert(cont.size() == old_size);
		}

		// 0, 1
		{
			std::size_t old_size = cont.size();
			node0_type node = cont.get<0>().extract(1);
			yggr_test_assert(!node.empty());
			yggr_test_assert(cont.size() == old_size - 1);
			yggr_test_assert(node.value() == arr[0]);

			insert_return1_type ins_ret = cont.get<1>().insert(boost::move(node));
			yggr_test_assert(ins_ret.inserted);
			yggr_test_assert(ins_ret.node.empty());
			yggr_test_assert(cont.size() == old_size);
		}

		{
			std::size_t old_size = cont.size();
			node0_type node = cont.get<0>().extract(1);
			yggr_test_assert(!node.empty());
			yggr_test_assert(cont.size() == old_size - 1);
			yggr_test_assert(node.value() == arr[0]);

			insert_return1_type ins_ret =
				cont.get<1>().insert(
					boost::move(node),
					boost::bind(
						&handler_insert_node_of_kv<unsafe_midx_type, index_id1_type, base_insert_return1_type>,
						_1, _2));
			yggr_test_assert(ins_ret.inserted);
			yggr_test_assert(ins_ret.node.empty());
			yggr_test_assert(cont.size() == old_size);
		}

		{
			std::size_t old_size = cont.size();
			node0_type node = cont.get<0>().extract(2);
			yggr_test_assert(!node.empty());
			yggr_test_assert(cont.size() == old_size - 1);
			yggr_test_assert(node.value() == arr[1]);

			bool ins_ret = cont.get<1>().insert(boost::bind(&safe_index1_type::base_type::cbegin, _1), boost::move(node));
			yggr_test_assert(ins_ret);
			yggr_test_assert(cont.size() == old_size);
		}

		{
			std::size_t old_size = cont.size();
			node0_type node = cont.get<0>().extract(3);
			yggr_test_assert(!node.empty());
			yggr_test_assert(cont.size() == old_size - 1);
			yggr_test_assert(node.value() == arr[2]);

			bool ins_ret =
				cont.get<1>().insert(
					boost::bind(&safe_index1_type::base_type::cbegin, _1),
					boost::move(node),
					boost::bind(
						&handler_insert_node_of_kv_hint<unsafe_midx_type, index_id1_type, iterator1_type>,
						_1, _2));
			yggr_test_assert(ins_ret);
			yggr_test_assert(cont.size() == old_size);
		}

		{
			node0_type node = cont.get<0>().extract(1);
			yggr_test_assert(!node.empty());
			yggr_test_assert(node.value() == arr[0]);

			cont.get<0>().insert(arr[0]);

			insert_return1_type ins_ret = cont.get<1>().insert(boost::move(node));
			yggr_test_assert(!ins_ret.inserted);
			yggr_test_assert(!ins_ret.node.empty());
		}

		{
			std::size_t old_size = cont.size();
			node0_type node = cont.get<0>().extract(boost::bind(&safe_index0_type::base_type::cbegin, _1));
			yggr_test_assert(!node.empty());
			yggr_test_assert(cont.size() == old_size - 1);

			bool ins_ret = cont.get<1>().insert(boost::bind(&safe_index1_type::base_type::cbegin, _1), boost::move(node));
			yggr_test_assert(ins_ret);
			yggr_test_assert(cont.size() == old_size);
		}

		// 1, 1
		{
			std::size_t old_size = cont.size();
			node1_type node = cont.get<1>().extract(1);
			yggr_test_assert(!node.empty());
			yggr_test_assert(cont.size() == old_size - 1);
			yggr_test_assert(node.value() == arr[0]);

			insert_return1_type ins_ret = cont.get<1>().insert(boost::move(node));
			yggr_test_assert(ins_ret.inserted);
			yggr_test_assert(ins_ret.node.empty());
			yggr_test_assert(cont.size() == old_size);

			node1_type node2 = cont.get<1>().extract(5);
			yggr_test_assert(node2.empty());
			yggr_test_assert(cont.size() == old_size);
		}

		{
			std::size_t old_size = cont.size();
			node1_type node = cont.get<1>().extract(1);
			yggr_test_assert(!node.empty());
			yggr_test_assert(cont.size() == old_size - 1);
			yggr_test_assert(node.value() == arr[0]);

			insert_return1_type ins_ret =
				cont.get<1>().insert(
					boost::move(node),
					boost::bind(
						&handler_insert_node_of_kv<unsafe_midx_type, index_id1_type, base_insert_return1_type>,
						_1, _2));
			yggr_test_assert(ins_ret.inserted);
			yggr_test_assert(ins_ret.node.empty());
			yggr_test_assert(cont.size() == old_size);

			node1_type node2 = cont.get<1>().extract(5);
			yggr_test_assert(node2.empty());
			yggr_test_assert(cont.size() == old_size);
		}

		{
			std::size_t old_size = cont.size();
			node1_type node = cont.get<1>().extract(2);
			yggr_test_assert(!node.empty());
			yggr_test_assert(cont.size() == old_size - 1);
			yggr_test_assert(node.value() == arr[1]);

			bool ins_ret = cont.get<1>().insert(boost::bind(&safe_index1_type::base_type::cbegin, _1), boost::move(node));
			yggr_test_assert(ins_ret);
			yggr_test_assert(cont.size() == old_size);
		}

		{
			std::size_t old_size = cont.size();
			node1_type node = cont.get<1>().extract(3);
			yggr_test_assert(!node.empty());
			yggr_test_assert(cont.size() == old_size - 1);
			yggr_test_assert(node.value() == arr[2]);

			bool ins_ret =
				cont.get<1>().insert(
					boost::bind(&safe_index1_type::base_type::cbegin, _1),
					boost::move(node),
					boost::bind(
						&handler_insert_node_of_kv_hint<unsafe_midx_type, index_id1_type, iterator1_type>,
						_1, _2));
			yggr_test_assert(ins_ret);
			yggr_test_assert(cont.size() == old_size);
		}

		{
			node1_type node = cont.get<1>().extract(1);
			yggr_test_assert(!node.empty());
			yggr_test_assert(node.value() == arr[0]);

			cont.get<1>().insert(arr[0]);

			insert_return1_type ins_ret = cont.get<1>().insert(boost::move(node));
			yggr_test_assert(!ins_ret.inserted);
			yggr_test_assert(!ins_ret.node.empty());
		}

		{
			std::size_t old_size = cont.size();
			node1_type node = cont.get<1>().extract(boost::bind(&safe_index1_type::base_type::cbegin, _1));
			yggr_test_assert(!node.empty());
			yggr_test_assert(cont.size() == old_size - 1);

			bool ins_ret = cont.get<1>().insert(boost::bind(&safe_index1_type::base_type::cbegin, _1), boost::move(node));
			yggr_test_assert(ins_ret);
			yggr_test_assert(cont.size() == old_size);
		}
	}

	// erase
	yggr_test_assert(cont.get<0>().insert(A(100, 100, "100")));
	yggr_test_assert(cont.get<0>().insert(A(200, 100, "100")));
	yggr_test_assert(cont.get<0>().insert(A(300, 100, "100")));
	yggr_test_assert(cont.get<0>().insert(A(400, 100, "100")));
	yggr_test_assert(cont.size() == 19);

	yggr_test_assert(1 == cont.get<0>().erase(100));
	yggr_test_assert(3 == cont.get<A::tag_prm2>().erase(100));
	yggr_test_assert(cont.size() == 15);

	// replace
	yggr_test_assert(cont.get<0>().replace(boost::bind(&unsafe_ranked_unique_type::find<int>, _1, 0), A(0, 100, "0")));
	yggr_test_assert(cont.get<0>().replace(boost::bind(&unsafe_ranked_unique_type::find<int>, _1, 0), A(100, 100, "0")));
	yggr_test_assert(cont.get<A::tag_prm1>().replace(boost::bind(&unsafe_ranked_unique_type::find<int>, _1, 100), A(0, 100, "0")));
	yggr_test_assert(!cont.get<A::tag_prm1>().replace(boost::bind(&unsafe_ranked_unique_type::find<int>, _1, 0), A(1, 100, "0")));
	yggr_test_assert(!cont.get<A::tag_prm1>().replace(boost::bind(&unsafe_ranked_unique_type::cend, _1), A(1, 100, "0")));

	yggr_test_assert(cont.size() == 15);

	yggr_test_assert(cont.get<1>().find_and_replace(100, A(100, 100, "0")));
	yggr_test_assert(cont.get<A::tag_prm2>().find_and_replace(100, cont.get<A::tag_prm2>().key_comp(), A(0, 1, "0")));
	yggr_test_assert(cont.get<A::tag_prm2>().find_and_replace(1, A(100, 1, "0")));
	yggr_test_assert(!cont.get<A::tag_prm2>().find_and_replace(1, cont.get<A::tag_prm2>().key_comp(), A(2, 1, "0")));

	yggr_test_assert(cont.size() == 15);

	// modify

	A tmp1;

	yggr_test_assert(cont.get<0>().modify(boost::bind(&unsafe_ranked_unique_type::find<int>, _1, 2),
									boost::bind(&modifier<A, A>, _1, A(1, 100, "100"))));

	yggr_test_assert(cont.get<0>().modify(boost::bind(&unsafe_ranked_unique_type::find<int>, _1, 1),
									boost::bind(&modifier<A, A>, _1, A(1, 100, "100"), boost::ref(tmp1)),
									boost::bind(&rollback<A>, _1, boost::cref(tmp1)) ) );

	yggr_test_assert(!cont.get<0>().modify(boost::bind(&unsafe_ranked_unique_type::find<int>, _1, 2),
									yggr::safe_container::make_optimization_multi_index_modifier<unsafe_midx_type>(
										A(2, 100, "100"),
										boost::bind(&modifier<A, A>, _1, A(2, 100, "100")))));

	yggr_test_assert(cont.get<0>().modify(boost::bind(&unsafe_ranked_unique_type::find<int>, _1, 1),
									yggr::safe_container::make_optimization_multi_index_modifier<unsafe_midx_type>(
										A(1, 100, "100"),
										boost::bind(&modifier<A, A>, _1, A(1, 100, "100"), boost::ref(tmp1))),
									boost::bind(&rollback<A>, _1, boost::cref(tmp1)) ) );

	yggr_test_assert(cont.size() == 15);

	yggr_test_assert(!cont.get<A::tag_prm1>().find_and_modify(2000,
													boost::bind(&modifier<A, A>, _1, A(1, 100, "100"), boost::ref(tmp1))));

	yggr_test_assert(!cont.get<A::tag_prm1>().find_and_modify(2000, cont.get<A::tag_prm1>().key_comp(),
													boost::bind(&modifier<A, A>, _1, A(1, 100, "100"), boost::ref(tmp1))));

	yggr_test_assert(!cont.get<A::tag_prm1>().find_and_modify(2000,
													boost::bind(&modifier<A, A>, _1, A(1, 100, "100"), boost::ref(tmp1)),
													boost::bind(&rollback<A>, _1, boost::cref(tmp1))));

	yggr_test_assert(!cont.get<A::tag_prm1>().find_and_modify(2, cont.get<A::tag_prm1>().key_comp(),
													boost::bind(&modifier<A, A>, _1, A(1, 100, "100"), boost::ref(tmp1)),
													boost::bind(&rollback<A>, _1, boost::cref(tmp1))));

	yggr_test_assert(cont.size() == 15);

	// modify key
	int tmp2;

	yggr_test_assert(cont.get<0>().modify_key(boost::bind(&unsafe_ranked_unique_type::find<int>, _1, 1),
									boost::bind(&modifier<int, int>, _1, 200)));

	yggr_test_assert(cont.get<0>().modify_key(boost::bind(&unsafe_ranked_unique_type::find<int>, _1, 200),
									boost::bind(&modifier<int, int>, _1, 200),
									boost::bind(&rollback<int>, _1, boost::cref(tmp2))));

	yggr_test_assert(cont.get<0>().modify_key(boost::bind(&unsafe_ranked_unique_type::find<int>, _1, 200),
									yggr::safe_container::make_optimization_multi_index_modifier<unsafe_midx_type>(
										A(200, 100, "100"),
										boost::bind(&modifier<int, int>, _1, 200))));

	yggr_test_assert(cont.get<0>().modify_key(boost::bind(&unsafe_ranked_unique_type::find<int>, _1, 200),
										yggr::safe_container::make_optimization_multi_index_modifier<unsafe_midx_type>(
											A(200, 100, "100"),
											boost::bind(&modifier<int, int>, _1, 200)),
									boost::bind(&rollback<int>, _1, boost::cref(tmp2))));



	yggr_test_assert(cont.size() == 15);

	yggr_test_assert(!cont.get<A::tag_prm1>().find_and_modify_key(2000,
														boost::bind(&modifier<int, int>, _1, 3, boost::ref(tmp2))));

	yggr_test_assert(!cont.get<A::tag_prm1>().find_and_modify_key(2000, cont.get<A::tag_prm1>().key_comp(),
														boost::bind(&modifier<int, int>, _1, 3, boost::ref(tmp2))));

	yggr_test_assert(!cont.get<A::tag_prm1>().find_and_modify_key(200,
														boost::bind(&modifier<int, int>, _1, 3, boost::ref(tmp2)),
														boost::bind(&rollback<int>, _1, boost::cref(tmp2))));

	yggr_test_assert(!cont.get<A::tag_prm1>().find_and_modify_key(200, cont.get<A::tag_prm1>().key_comp(),
														boost::bind(&modifier<int, int>, _1, 3, boost::ref(tmp2)),
														boost::bind(&rollback<int>, _1, boost::cref(tmp2))));
	yggr_test_assert(cont.size() == 15);

	tmp2 = 0;

	mi_cont_type::base_type base_cont(arr + 0, arr + 10);
	yggr_test_assert(cont.size() == 15);
	yggr_test_assert(base_cont.size() == 5);

	cont.get<0>().swap(base_cont.get<0>());
	yggr_test_assert(cont.size() == 5);
	yggr_test_assert(base_cont.size() == 15);

	cont.get<A::tag_prm1>().swap(base_cont.get<A::tag_prm1>());
	yggr_test_assert(cont.size() == 15);
	yggr_test_assert(base_cont.size() == 5);

	yggr_test_assert(!cont.erase(boost::bind(&unsafe_ranked_unique_type::cend, _1)));
	yggr_test_assert(cont.erase(boost::bind(&unsafe_ranked_unique_type::cbegin, _1)));
	yggr_test_assert(cont.get<1>().erase(boost::bind(&unsafe_ranked_non_unique_type::cbegin, _1),
								boost::bind(&unsafe_ranked_non_unique_type::cend, _1)));

	yggr_test_assert(cont.empty());

	{
		typedef yggr::safe_container::nth_index<mi_cont_type, 0>::type safe_index0_type;
		typedef safe_index0_type::index_id_type index_id0_type;
		typedef safe_index0_type::iterator iterator0_type;


		typedef yggr::safe_container::nth_index<mi_cont_type, 1>::type safe_index1_type;
		typedef safe_index1_type::index_id_type index_id1_type;
		typedef safe_index1_type::iterator iterator1_type;

		yggr_test_assert(cont.emplace(yggr::tag_non_handler()));
		yggr_test_assert(cont.emplace(yggr::tag_non_handler(), 1, 1, "1"));
		yggr_test_assert(cont.emplace_hint(boost::bind(&safe_index0_type::base_type::cend, _1), yggr::tag_non_handler()));
		yggr_test_assert(cont.emplace_hint(boost::bind(&safe_index0_type::base_type::cend, _1), yggr::tag_non_handler(), 1, 1, "1"));

		int call_count = 0;

		yggr_test_assert(!(cont.emplace(boost::bind(&handler_insert_value_of_ordered<unsafe_midx_type, index_id0_type, iterator0_type>,
											_1, _2, boost::ref(call_count)))));

		yggr_test_assert(!cont.get<1>().
					emplace(boost::bind(&handler_insert_value_of_ordered<unsafe_midx_type, index_id1_type, iterator1_type>,
											_1, _2, boost::ref(call_count)), 1, 1, "1"));

		yggr_test_assert(cont.emplace_hint(boost::bind(&safe_index0_type::base_type::cend, _1),
									boost::bind(&handler_insert_value_of_ordered<unsafe_midx_type, index_id0_type, iterator0_type>,
													_1, _2, boost::ref(call_count))));

		yggr_test_assert(cont.get<1>().
				emplace_hint(boost::bind(&safe_index1_type::base_type::cend, _1),
								boost::bind(&handler_insert_value_of_ordered<unsafe_midx_type, index_id1_type, iterator1_type>,
											_1, _2, boost::ref(call_count)), 1, 1, "1"));


		yggr_test_assert(call_count == 4);
		yggr_test_assert(cont.size() == 2);
	}

	cont.get<0>().clear();
	yggr_test_assert(cont.empty());

	cont.clear();
	yggr_test_assert(cont.empty());

	std::cout << "---------------test_modifiers_of_ranked end-------------------------" << std::endl;
}

template<typename Final, typename IndexID, typename Iterator>
bool handler_find_of_ranked(Final& final,
								const
									boost::fusion::pair
									<
										IndexID,
										Iterator
									>& rst, int& count)
{
	typedef boost::fusion::pair<IndexID, Iterator> rst_type;
	typedef typename rst_type::first_type index_id_type;
	typedef Final midx_type;
	typedef typename
		boost::multi_index::nth_index
		<
			midx_type, index_id_type::first::value
		>::type now_index_type;

	++count;
	return rst.second != final.template get<index_id_type::first::value>().end();
}

template<typename Final, typename IndexID, typename Iterator>
int handler_equal_range_of_ranked(Final& final,
									const
										boost::fusion::pair
										<
											IndexID,
											std::pair<Iterator, Iterator>
										>& rst, int& count)
{
	typedef boost::fusion::pair<IndexID, Iterator> rst_type;
	typedef typename rst_type::first_type index_id_type;
	typedef Final midx_type;
	typedef typename
		boost::multi_index::nth_index
		<
			midx_type, index_id_type::first::value
		>::type now_index_type;

	++count;
	return std::distance(rst.second.first, rst.second.second);
}

template<typename Final, typename IndexID, typename Iterator>
bool handler_bound_of_ranked(Final& final,
								const
									boost::fusion::pair
									<
										IndexID,
										Iterator
									>& rst, int chk)
{
	typedef boost::fusion::pair<IndexID, Iterator> rst_type;
	typedef typename rst_type::first_type index_id_type;
	typedef Final midx_type;
	typedef typename
		boost::multi_index::nth_index
		<
			midx_type, index_id_type::first::value
		>::type now_index_type;

	return (rst.second != final.template get<index_id_type::first::value>().end())
			&&  ((final.template get<index_id_type::first::value>().key_extractor())(*(rst.second)) == chk);
}

template<typename Final, typename IndexID, typename Iterator>
bool handler_range_of_ranked(Final& final,
								const
									boost::fusion::pair
									<
										IndexID,
										std::pair<Iterator, Iterator>
									>& rst, std::size_t chk)
{
	typedef boost::fusion::pair<IndexID, Iterator> rst_type;
	typedef typename rst_type::first_type index_id_type;
	typedef Final midx_type;
	typedef typename
		boost::multi_index::nth_index
		<
			midx_type, index_id_type::first::value
		>::type now_index_type;

	return std::distance(rst.second.first, rst.second.second) == chk;
}

void test_lookup_of_ranked(void)
{
	typedef yggr::safe_container::safe_multi_index_container<
			A,
			boost::multi_index::indexed_by<
				boost::multi_index::ranked_unique<
					boost::multi_index::tag<A::tag_prm1>,
					BOOST_MULTI_INDEX_MEMBER(A, int, prm1) >,

				boost::multi_index::ranked_non_unique<
					boost::multi_index::tag<A::tag_prm2>,
					BOOST_MULTI_INDEX_MEMBER(A, int, prm2)>
			>
		> mi_cont_type;

	typedef mi_cont_type::base_type unsafe_midx_type;

	typedef boost::multi_index::nth_index<mi_cont_type, 0>::type safe_index0_type;
	typedef safe_index0_type::index_id_type index_id0_type;
	typedef safe_index0_type::iterator iterator0_type;

	typedef boost::multi_index::nth_index<mi_cont_type, 1>::type safe_index1_type;
	typedef safe_index1_type::index_id_type index_id1_type;
	typedef safe_index1_type::iterator iterator1_type;

	A arr[10] = {A(1, 1, "aaa"), A(2, 2, "bbb"), A(3, 3, "ccc"), A(4, 4, "ddd")};
	mi_cont_type cont(arr + 0, arr + 10);


	yggr_test_assert(cont.find(1));
	yggr_test_assert(!cont.find(100));

	{
		int call_count = 0;

		yggr_test_assert(cont.find(1,
							boost::bind(
								&handler_find_of_ranked<
									unsafe_midx_type, index_id0_type, iterator0_type>, _1, _2, boost::ref(call_count)) ));

		const safe_index0_type& idx0_cref = cont.get<0>();

		yggr_test_assert(call_count == 1);

		yggr_test_assert(idx0_cref.find(1,
							boost::bind(
								&handler_find_of_ranked<
									const unsafe_midx_type, index_id0_type, iterator0_type>, _1, _2, boost::ref(call_count)) ));

		yggr_test_assert(call_count == 2);

		yggr_test_assert(!cont.find(100,
							boost::bind(
								&handler_find_of_ranked<
									unsafe_midx_type, index_id0_type, iterator0_type>, _1, _2, boost::ref(call_count)) ));

		yggr_test_assert(call_count == 3);
	}

	yggr_test_assert(cont.get<1>().find(1, cont.key_comp()));
	yggr_test_assert(!cont.get<1>().find(100, cont.key_comp()));

	{
		int call_count = 0;

		yggr_test_assert(cont.find(1, cont.key_comp(),
							boost::bind(
								&handler_find_of_ranked<
									unsafe_midx_type, index_id0_type, iterator0_type>, _1, _2, boost::ref(call_count)) ));

		const safe_index0_type& idx0_cref = cont.get<0>();

		yggr_test_assert(call_count == 1);

		yggr_test_assert(idx0_cref.find(1, cont.key_comp(),
							boost::bind(
								&handler_find_of_ranked<
									const unsafe_midx_type, index_id0_type, iterator0_type>, _1, _2, boost::ref(call_count)) ));

		yggr_test_assert(call_count == 2);

		yggr_test_assert(!cont.find(100, cont.key_comp(),
							boost::bind(
								&handler_find_of_ranked<
									unsafe_midx_type, index_id0_type, iterator0_type>, _1, _2, boost::ref(call_count)) ));

		yggr_test_assert(call_count == 3);
	}

	yggr_test_assert(cont.count(1) == 1);
	yggr_test_assert(cont.count(100) == 0);

	yggr_test_assert(cont.count(1, cont.key_comp()) == 1);
	yggr_test_assert(cont.count(100, cont.key_comp()) == 0);

	{
		int call_count = 0;
		yggr_test_assert(1 == cont.equal_range(1,
								boost::bind(
									&handler_equal_range_of_ranked<
										unsafe_midx_type, index_id0_type, iterator0_type>, _1, _2, boost::ref(call_count))));

		yggr_test_assert(call_count == 1);

		yggr_test_assert(0 == cont.equal_range(100,
								boost::bind(
									&handler_equal_range_of_ranked<
										unsafe_midx_type, index_id0_type, iterator0_type>, _1, _2, boost::ref(call_count))));

		yggr_test_assert(call_count == 2);
	}

	{
		int call_count = 0;
		yggr_test_assert(1 == cont.equal_range(1, cont.key_comp(),
								boost::bind(
									&handler_equal_range_of_ranked<
										unsafe_midx_type, index_id0_type, iterator0_type>, _1, _2, boost::ref(call_count))));

		yggr_test_assert(call_count == 1);

		yggr_test_assert(0 == cont.equal_range(100, cont.key_comp(),
								boost::bind(
									&handler_equal_range_of_ranked<
										unsafe_midx_type, index_id0_type, iterator0_type>, _1, _2, boost::ref(call_count))));

		yggr_test_assert(call_count == 2);
	}

	{
		yggr_test_assert(cont.lower_bound(1,
								boost::bind(
									&handler_bound_of_ranked<
										unsafe_midx_type, index_id0_type, iterator0_type>, _1, _2, 1)));

		yggr_test_assert(cont.get<1>().lower_bound(1, cont.get<1>().key_comp(),
								boost::bind(
									&handler_bound_of_ranked<
										unsafe_midx_type, index_id1_type, iterator1_type>, _1, _2, 1)));

		yggr_test_assert(!cont.lower_bound(100,
								boost::bind(
									&handler_bound_of_ranked<
										unsafe_midx_type, index_id0_type, iterator0_type>, _1, _2, 100)));

		yggr_test_assert(!cont.get<1>().lower_bound(100, cont.get<1>().key_comp(),
								boost::bind(
									&handler_bound_of_ranked<
										unsafe_midx_type, index_id1_type, iterator1_type>, _1, _2, 100)));
	}

	{
		yggr_test_assert(cont.upper_bound(1,
								boost::bind(
									&handler_bound_of_ranked<
										unsafe_midx_type, index_id0_type, iterator0_type>, _1, _2, 2)));

		yggr_test_assert(cont.get<1>().upper_bound(1, cont.get<1>().key_comp(),
								boost::bind(
									&handler_bound_of_ranked<
										unsafe_midx_type, index_id1_type, iterator1_type>, _1, _2, 2)));

		yggr_test_assert(!cont.upper_bound(100,
								boost::bind(
									&handler_bound_of_ranked<
										unsafe_midx_type, index_id0_type, iterator0_type>, _1, _2, 100)));

		yggr_test_assert(!cont.get<1>().upper_bound(100, cont.get<1>().key_comp(),
								boost::bind(
									&handler_bound_of_ranked<
										unsafe_midx_type, index_id1_type, iterator1_type>, _1, _2, 100)));
	}

	{
		// 1 < x < 4 -> A(2, 2, "bbb"), A(3, 3, "ccc")
		yggr_test_assert(cont.range(boost::bind(cont.key_comp(), 1, _1), boost::bind(cont.key_comp(), _1, 4),
								boost::bind(
									&handler_range_of_ranked<
										unsafe_midx_type, index_id0_type, iterator0_type>, _1, _2, 2)));

		// -1 < x < 100 -> A(0, 0, ""), A(1, 1, "aaa"), A(2, 2, "bbb"), A(3, 3, "ccc"), A(4, 4, "ddd")
		yggr_test_assert(cont.range(boost::bind(cont.key_comp(), -1, _1), boost::bind(cont.key_comp(), _1, 100),
								boost::bind(
									&handler_range_of_ranked<
										unsafe_midx_type, index_id0_type, iterator0_type>, _1, _2, 5)));
	}

	{
		A a;

		bool bret = cont.get_value(1, a);
		yggr_test_assert(bret);
		yggr_test_assert(a == arr[0]);

		yggr_test_assert(!cont.get_value(100, a));
	}

	{
		A a;

		bool bret = cont.get<1>().get_value(1, cont.key_comp(), a);
		yggr_test_assert(bret);
		yggr_test_assert(a == arr[0]);

		yggr_test_assert(!cont.get<1>().get_value(100, cont.key_comp(), a));
	}

	std::cout << "---------------test_lookup_of_ranked end-------------------------" << std::endl;

}

template<typename Base, typename IndexID, typename Iterator>
bool handler_nth_of_rank(Base& base, boost::fusion::pair<IndexID, Iterator> rst, A chk)
{
	return base.end() != rst.second && (*rst.second == chk);
}

void test_rank_operations_of_randked(void)
{
	typedef yggr::safe_container::safe_multi_index_container<
		A,
		boost::multi_index::indexed_by<
			boost::multi_index::ranked_unique<
				boost::multi_index::tag<A::tag_prm1>,
				BOOST_MULTI_INDEX_MEMBER(A, int, prm1) >,

			boost::multi_index::ranked_non_unique<
				boost::multi_index::tag<A::tag_prm2>,
				BOOST_MULTI_INDEX_MEMBER(A, int, prm2)>
		>
	> mi_cont_type;

	typedef mi_cont_type::base_type unsafe_midx_type;

	typedef boost::multi_index::nth_index<mi_cont_type, 0>::type safe_index0_type;
	typedef safe_index0_type::index_id_type index_id0_type;
	typedef safe_index0_type::iterator iterator0_type;

	typedef boost::multi_index::nth_index<mi_cont_type, 1>::type safe_index1_type;
	typedef safe_index1_type::index_id_type index_id1_type;
	typedef safe_index1_type::iterator iterator1_type;

	A arr[10] = {A(1, 1, "aaa"), A(2, 2, "bbb"), A(3, 3, "ccc"), A(4, 4, "ddd")};
	mi_cont_type cont(arr + 0, arr + 10);

	yggr_test_assert(cont.nth(0, boost::bind(&handler_nth_of_rank<unsafe_midx_type, index_id0_type, iterator0_type>, _1, _2, A(0, 0, ""))));
	yggr_test_assert(cont.nth(1, boost::bind(&handler_nth_of_rank<unsafe_midx_type, index_id0_type, iterator0_type>, _1, _2, A(1, 1, "aaa"))));

	yggr_test_assert(0 == cont.rank(boost::bind(&safe_index0_type::base_type::cbegin, _1)));

	yggr_test_assert(cont.size() == cont.rank(boost::bind(&safe_index0_type::base_type::cend, _1)));

	yggr_test_assert(1 == cont.find_rank(1));
	yggr_test_assert(1 == cont.find_rank(1, std::less<int>()));

	yggr_test_assert(1 == cont.lower_bound_rank(1));
	yggr_test_assert(1 == cont.lower_bound_rank(1, std::less<int>()));

	yggr_test_assert(2 == cont.upper_bound_rank(1));
	yggr_test_assert(2 == cont.upper_bound_rank(1, std::less<int>()));

	{
		std::pair<std::size_t, std::size_t> rst
			= cont.equal_range_rank(1);

		yggr_test_assert((rst.first == 1 && rst.second == 2));
	}

	{
		std::pair<std::size_t, std::size_t> rst
			= cont.equal_range_rank(1, std::less<int>());

		yggr_test_assert((rst.first == 1 && rst.second == 2));
	}

	{
		// 1 < x < 4
		std::pair<std::size_t, std::size_t> rst
			= cont.range_rank(boost::bind(std::less<int>(), 1, _1), boost::bind(std::less<int>(), _1, 4));

		yggr_test_assert((rst.first == 2 && rst.second == 4));
	}


	std::cout << "---------------test_rank_operations_of_randked end-------------------------" << std::endl;
}

void test_set_operations_of_ranked(void)
{
	typedef yggr::safe_container::safe_multi_index_container<
			A,
			boost::multi_index::indexed_by<
				boost::multi_index::ranked_unique<
					boost::multi_index::tag<A::tag_prm1>,
					BOOST_MULTI_INDEX_MEMBER(A, int, prm1) >,

				boost::multi_index::ranked_non_unique<
					boost::multi_index::tag<A::tag_prm2>,
					BOOST_MULTI_INDEX_MEMBER(A, int, prm2)>
			>
		> mi_cont_type;

	A arr[10] = {A(1, 1, "aaa"), A(2, 2, "bbb"), A(3, 3, "ccc"), A(4, 3, "ddd")};
	mi_cont_type cont(arr + 0, arr + 10);

	yggr_test_assert(1 == cont.get<A::tag_prm1>().count(1));
	yggr_test_assert(1 == cont.get<0>().count(2));
	yggr_test_assert(0 == cont.get<0>().count(100));

	yggr_test_assert(1 == cont.get<A::tag_prm2>().count(2));
	yggr_test_assert(2 == cont.get<1>().count(3));
	yggr_test_assert(0 == cont.get<1>().count(100));

	yggr_test_assert(cont.size() == 5);

	mi_cont_type::base_type base_cont(arr + 0, arr + 10);
	yggr_test_assert(cont.size() == 5);

	yggr_test_assert(1 == cont.get<A::tag_prm1>().count(1, std::less<int>()));

	yggr_test_assert(1 == cont.get<0>().count(2, std::less<int>()));

	yggr_test_assert(0 == cont.get<0>().count(100, std::less<int>()));

	yggr_test_assert(1 == cont.get<A::tag_prm2>().count(2, std::less<int>()));

	yggr_test_assert(2 == cont.get<1>().count(3, std::less<int>()));

	yggr_test_assert(0 == cont.get<1>().count(100, std::less<int>()));

	std::cout << "---------------test_set_operations_of_ranked end-------------------" << std::endl;
}

void test_swap_of_ranked(void)
{
	typedef yggr::safe_container::safe_multi_index_container<
			A,
			boost::multi_index::indexed_by<
				boost::multi_index::ranked_unique<
					boost::multi_index::tag<A::tag_prm1>,
					BOOST_MULTI_INDEX_MEMBER(A, int, prm1) >,

				boost::multi_index::ranked_non_unique<
					boost::multi_index::tag<A::tag_prm2>,
					BOOST_MULTI_INDEX_MEMBER(A, int, prm2)>
			>
		> mi_cont_type;
	test_swap<mi_cont_type>();
	std::cout << "---------------test_swap_of_ranked end-----------------" << std::endl;
}

void test_operator_set_of_ranked(void)
{
	typedef yggr::safe_container::safe_multi_index_container<
			A,
			boost::multi_index::indexed_by<
				boost::multi_index::ranked_unique<
					boost::multi_index::tag<A::tag_prm1>,
					BOOST_MULTI_INDEX_MEMBER(A, int, prm1) >,

				boost::multi_index::ranked_non_unique<
					boost::multi_index::tag<A::tag_prm2>,
					BOOST_MULTI_INDEX_MEMBER(A, int, prm2)>
			>
		> mi_cont_type;
	test_operator_set<mi_cont_type>();
	std::cout << "---------------test_operator_set_of_ranked end-----------------" << std::endl;
}

void test_comparer_of_ranked(void)
{
	typedef yggr::safe_container::safe_multi_index_container<
			A,
			boost::multi_index::indexed_by<
				boost::multi_index::ranked_unique<
					boost::multi_index::tag<A::tag_prm1>,
					BOOST_MULTI_INDEX_MEMBER(A, int, prm1) >,

				boost::multi_index::ranked_non_unique<
					boost::multi_index::tag<A::tag_prm2>,
					BOOST_MULTI_INDEX_MEMBER(A, int, prm2)>
			>
		> mi_cont_type;
	test_compare_of_full<mi_cont_type>();
	std::cout << "---------------test_comparer_of_ranked end-----------------" << std::endl;
}


void test_safe_other_of_ranked(void)
{
	typedef
		yggr::safe_container::safe_multi_index_container
		<
			A,
			boost::multi_index::indexed_by<
				boost::multi_index::ranked_unique<
					boost::multi_index::tag<A::tag_prm1>,
					BOOST_MULTI_INDEX_MEMBER(A, int, prm1) >,

				boost::multi_index::ranked_non_unique<
					boost::multi_index::tag<A::tag_prm2>,
					BOOST_MULTI_INDEX_MEMBER(A, int, prm2)>
			>
		> mi_cont_type;

	typedef mi_cont_type::base_type unsafe_midx_type;

	typedef boost::multi_index::nth_index<mi_cont_type, 0>::type safe_index0_type;
	typedef safe_index0_type::index_id_type index_id0_type;
	typedef safe_index0_type::iterator iterator0_type;

	typedef boost::multi_index::nth_index<mi_cont_type, 1>::type safe_index1_type;
	typedef safe_index1_type::index_id_type index_id1_type;
	typedef safe_index1_type::iterator iterator1_type;

	test_safe_other_gernel<mi_cont_type>();

	A arr[10] = {A(1, 1, "aaa"), A(2, 2, "bbb"), A(3, 3, "ccc"), A(4, 3, "ddd")};

	mi_cont_type cont(arr + 0, arr + 10);

	yggr_test_assert(cont.is_exists(1));
	yggr_test_assert(!cont.is_exists(100));
	yggr_test_assert(cont.is_exists(1, cont.key_comp()));
	yggr_test_assert(!cont.is_exists(100, cont.key_comp()));

	{
		A beg_val, rbeg_val;

		mi_cont_type cont_empty;
		yggr_test_assert(!cont_empty.get_begin_value(beg_val));
		yggr_test_assert(!cont_empty.get_rbegin_value(rbeg_val));

		try
		{
			cont_empty.begin_value();
			yggr_test_assert(false);
		}
		catch(const mi_cont_type::error_type&)
		{
		}

		try
		{
			cont_empty.rbegin_value();
			yggr_test_assert(false);
		}
		catch(const mi_cont_type::error_type&)
		{
		}

		cont.get_begin_value(beg_val);
		cont.get_rbegin_value(rbeg_val);

		yggr_test_assert(beg_val == cont.begin_value());
		yggr_test_assert(rbeg_val == cont.rbegin_value());
	}

	{
		int call_count = 0;
		cont.for_each_iterator_reverse(boost::bind(&handler_for_each<typename safe_index0_type::reverse_iterator>,
														_1, boost::ref(call_count)));
		yggr_test_assert(call_count == cont.size());

		call_count = 0;
		cont.for_each_iterator_reverse(boost::bind(&handler_for_each<typename safe_index0_type::reverse_iterator>,
														_1, boost::ref(call_count)),
										boost::bind(&handler_filter<typename safe_index0_type::reverse_iterator>,
														_1, false));
		yggr_test_assert(call_count == 0);

		call_count = 0;
		cont.for_each_iterator_reverse(boost::bind(&handler_for_each<typename safe_index0_type::reverse_iterator>,
														_1, boost::ref(call_count)),
										boost::bind(&handler_filter<typename safe_index0_type::reverse_iterator>,
														_1, true));
		yggr_test_assert(call_count == cont.size());
	}

	{
		int call_count = 0;
		cont.for_each_reverse(boost::bind(&handler_for_each<typename safe_index0_type::value_type>,
											_1, boost::ref(call_count)));
		yggr_test_assert(call_count == cont.size());

		call_count = 0;
		cont.for_each_reverse(boost::bind(&handler_for_each<typename safe_index0_type::value_type>,
											_1, boost::ref(call_count)),
								boost::bind(&handler_filter<typename safe_index0_type::value_type>,
											_1, false));
		yggr_test_assert(call_count == 0);

		call_count = 0;
		cont.for_each_reverse(boost::bind(&handler_for_each<typename safe_index0_type::value_type>,
											_1, boost::ref(call_count)),
								boost::bind(&handler_filter<typename safe_index0_type::value_type>,
											_1, true));
		yggr_test_assert(call_count == cont.size());
	}

	std::cout << "---------------test_safe_other_of_ranked end-----------------" << std::endl;
}

#endif // YGGR_BOOST_MULTI_INDEX_HAS_RANKED_INDEX

//---------------------sequenced------------------------------------

void test_construct_of_sequenced(void)
{
	typedef yggr::safe_container::safe_multi_index_container<
			A,
			boost::multi_index::indexed_by<
				boost::multi_index::sequenced< boost::multi_index::tag<A::tag_prm1> >,
				boost::multi_index::sequenced< boost::multi_index::tag<A::tag_prm2> >
			>
		> mi_cont_type;
	test_construct<mi_cont_type>();

	std::cout << "---------------test_construct_of_sequenced end-------------------------" << std::endl;
}

void test_observers_of_sequenced(void)
{
	typedef yggr::safe_container::safe_multi_index_container<
			A,
			boost::multi_index::indexed_by<
				boost::multi_index::sequenced< boost::multi_index::tag<A::tag_prm1> >,
				boost::multi_index::sequenced< boost::multi_index::tag<A::tag_prm2> >
			>
		> mi_cont_type;

	A arr[10] = {A(1, 1, "aaa"), A(2, 2, "bbb"), A(3, 3, "ccc"), A(4, 3, "ddd")};
	mi_cont_type cont(arr + 0, arr + 10);
	mi_cont_type cont2(cont.get_allocator());

	cont.get<0>().get_allocator();
	cont.get<1>().get_allocator();
	cont.get<A::tag_prm1>().get_allocator();
	cont.get<A::tag_prm2>().get_allocator();

	std::cout << "---------------test_observers_of_sequenced end-------------------------" << std::endl;
}

void test_capacity_of_sequenced(void)
{
	typedef yggr::safe_container::safe_multi_index_container<
			A,
			boost::multi_index::indexed_by<
				boost::multi_index::sequenced< boost::multi_index::tag<A::tag_prm1> >,
				boost::multi_index::sequenced< boost::multi_index::tag<A::tag_prm2> >
			>
		> mi_cont_type;

	A arr[10] = {A(1, 1, "aaa"), A(2, 2, "bbb"), A(3, 3, "ccc"), A(4, 3, "ddd")};
	mi_cont_type cont(arr + 0, arr + 10);
	mi_cont_type::base_type base_cont(arr + 0, arr + 10);
	yggr_test_assert(cont.size() == 10);
	yggr_test_assert(!cont.empty());
	yggr_test_assert(base_cont.size() == cont.size());
	yggr_test_assert(base_cont.max_size() == cont.max_size());

	yggr_test_assert(cont.get<0>().empty() == cont.get<A::tag_prm1>().empty());
	yggr_test_assert(cont.get<0>().empty() == cont.get<A::tag_prm2>().empty());

	yggr_test_assert(cont.get<1>().empty() == cont.get<A::tag_prm2>().empty());
	yggr_test_assert(cont.get<1>().empty() == base_cont.empty());

	yggr_test_assert(cont.get<0>().size() == cont.get<A::tag_prm1>().size());
	yggr_test_assert(cont.get<0>().size() == base_cont.size());

	yggr_test_assert(cont.get<0>().max_size() == base_cont.max_size());
	yggr_test_assert(cont.get<0>().max_size() == base_cont.get<0>().max_size());

	yggr_test_assert(cont.get<A::tag_prm2>().max_size() == base_cont.get<1>().max_size());

	cont.resize(100);
	yggr_test_assert(cont.get<0>().size() == cont.get<1>().size());

	cont.get<1>().resize(100, A(700, 700, "700"));
	yggr_test_assert(cont.get<0>().size() == cont.get<1>().size());

	std::cout << "---------------test_capacity_of_sequenced end-------------------------" << std::endl;
}

template<typename Final, typename IndexID, typename Iterator>
bool handler_insert_value_of_linear(Final& final,
									const
										boost::fusion::pair
										<
											IndexID,
											std::pair<Iterator, bool>
										>& rst, int& count)
{
	typedef boost::fusion::pair<IndexID, std::pair<Iterator, bool> > rst_type;
	typedef typename rst_type::first_type index_id_type;
	typedef Final midx_type;
	typedef typename
		boost::multi_index::nth_index
		<
			midx_type, index_id_type::first::value
		>::type now_index_type;

	typedef typename
		boost::mpl::if_
		<
			boost::is_const<Final>,
			const now_index_type&,
			now_index_type&
		>::type now_index_ref_type;

	++count;
	if(rst.second.second)
	{
		now_index_ref_type now_index = final.template get<index_id_type::first::value>();

		yggr_test_assert((std::find(now_index.begin(), now_index.end(), *rst.second.first) != now_index.end()));
	}
	return rst.second.second;
}

template<typename Final, typename IndexID, typename InsRet>
yggr::safe_container::safe_insert_return_type_base
<
	typename
		yggr::container::node_handle_traits
		<
			typename
				boost::multi_index::nth_index
				<
					Final, IndexID::first::value
				>::type
		>::node_type
>
	handler_insert_node_of_linear(Final& final,
									boost::fusion::pair
									<
										IndexID,
										InsRet
									>& rst)
{
	typedef boost::fusion::pair<IndexID, InsRet > rst_type;
	typedef typename rst_type::first_type index_id_type;
	typedef Final midx_type;
	typedef typename
		boost::multi_index::nth_index
		<
			midx_type, index_id_type::first::value
		>::type now_index_type;

	typedef typename
		boost::mpl::if_
		<
			boost::is_const<Final>,
			const now_index_type&,
			now_index_type&
		>::type now_index_ref_type;

	typedef typename
		yggr::container::node_handle_traits
		<
			now_index_type
		>::node_type node_type;

	typedef yggr::safe_container::safe_insert_return_type_base<node_type> ret_type;

	if(rst.second.inserted)
	{
		now_index_ref_type now_index = final.template get<index_id_type::first::value>();
		yggr_test_assert((std::find(now_index.begin(), now_index.end(), *(rst.second.position)) != now_index.end()));
	}

	return ret_type(boost::move(rst.second));
}

void test_modifiers_of_sequenced(void)
{
	typedef yggr::safe_container::safe_multi_index_container<
			A,
			boost::multi_index::indexed_by<
				boost::multi_index::sequenced< boost::multi_index::tag<A::tag_prm1> >,
				boost::multi_index::sequenced< boost::multi_index::tag<A::tag_prm2> >
			>
		> mi_cont_type;

	typedef mi_cont_type::base_type unsafe_midx_type;
	typedef boost::multi_index::nth_index<unsafe_midx_type, 0>::type unsafe_sequenced_type;

	typedef yggr::safe_container::nth_index<mi_cont_type, 0>::type safe_index0_type;
	typedef safe_index0_type::index_id_type index_id0_type;
	typedef safe_index0_type::iterator iterator0_type;


	typedef yggr::safe_container::nth_index<mi_cont_type, 1>::type safe_index1_type;
	typedef safe_index1_type::index_id_type index_id1_type;
	typedef safe_index1_type::iterator iterator1_type;

	mi_cont_type cont2;
	A tmp;
	yggr_test_assert(cont2.empty());
	yggr_test_assert(!cont2.get<0>().get_front(tmp));
	try
	{
		cont2.get<0>().front();
		yggr_test_assert(false);
	}
	catch(...)
	{
		yggr_test_assert(true);
	}

	yggr_test_assert(!cont2.get<0>().get_back(tmp));

	try
	{
		cont2.get<0>().back();
		yggr_test_assert(false);
	}
	catch(...)
	{
		yggr_test_assert(true);
	}

	yggr_test_assert(!cont2.get<A::tag_prm2>().get_front(tmp));
	yggr_test_assert(!cont2.get<A::tag_prm2>().get_back(tmp));

	yggr_test_assert(!cont2.get<A::tag_prm2>().pop_front(tmp));
	yggr_test_assert(!cont2.get<A::tag_prm2>().pop_back(tmp));
	yggr_test_assert(!cont2.get<A::tag_prm2>().pop_front());
	yggr_test_assert(!cont2.get<A::tag_prm2>().pop_back());

	A arr[10] = {A(1, 1, "aaa"), A(2, 2, "bbb"), A(3, 3, "ccc"), A(4, 3, "ddd")};
	mi_cont_type cont(arr + 0, arr + 10);
	yggr_test_assert(cont.size() == 10);

	// initializer_list
#ifndef BOOST_NO_CXX11_HDR_INITIALIZER_LIST
	{
		mi_cont_type cont1 = {A(1, 1, "aaa"), A(2, 2, "bbb"), A(3, 3, "ccc"), A(4, 3, "ddd")};
		yggr_test_assert(cont1.size() == 4);
	}

	{
		mi_cont_type cont1;
		cont1.insert(boost::bind(&safe_index0_type::base_type::cend, _1),
						{A(1, 1, "aaa"), A(2, 2, "bbb"), A(3, 3, "ccc"), A(4, 3, "ddd")});
		yggr_test_assert(cont1.size() == 4);
	}
#endif // BOOST_NO_CXX11_HDR_INITIALIZER_LIST

	cont.get<0>().push_back(A(10, 10, "10"));
	A back;
	yggr_test_assert(cont.get<0>().get_back(back));
	yggr_test_assert(back.prm1 == 10 && back.prm2 == 10);

	yggr_test_assert(cont.size() == 11);

	cont.get<0>().push_front(A(20, 20, "20"));
	A front;
	yggr_test_assert(cont.get<0>().get_front(front));
	yggr_test_assert(front.prm1 == 20 && front.prm2 == 20);
	yggr_test_assert(cont.size() == 12);

	cont.get<0>().push_back(A(40, 40, "40"));
	cont.get<0>().push_front(A(60, 60, "60"));

	yggr_test_assert(cont.size() == 14);
	A back2;
	A front2;

	yggr_test_assert(cont.get<0>().pop_front(front2));
	yggr_test_assert(cont.get<0>().pop_back(back2));

	yggr_test_assert(back2.prm1 == 40);
	yggr_test_assert(front2.prm1 == 60);
	yggr_test_assert(cont.size() == 12);

	yggr_test_assert(cont.get<0>().pop_front());
	yggr_test_assert(cont.get<0>().pop_back());

	yggr_test_assert(cont.size() == 10);

	{
		int call_count = 0;
		bool bright =
			cont.push_back(A(100, 100, "100"),
							boost::bind(&handler_insert_value_of_linear
										<
											unsafe_midx_type, index_id0_type, iterator0_type
										>, _1, _2, boost::ref(call_count)));
		yggr_test_assert(bright);
		yggr_test_assert(call_count == 1);
		yggr_test_assert(cont.back() == A(100, 100, "100"));

		bright =
			cont.emplace_back(yggr::tag_non_handler());

		yggr_test_assert(bright);
		yggr_test_assert(cont.back() == A(0, 0, ""));

		bright =
			cont.emplace_back(yggr::tag_non_handler(), 200, 200, "200");

		yggr_test_assert(bright);
		yggr_test_assert(cont.back() == A(200, 200, "200"));

		bright =
			cont.emplace_back(boost::bind(&handler_insert_value_of_linear
										<
											unsafe_midx_type, index_id0_type, iterator0_type
										>, _1, _2, boost::ref(call_count)) );

		yggr_test_assert(bright);
		yggr_test_assert(call_count == 2);
		yggr_test_assert(cont.back() == A(0, 0, ""));

		bright =
			cont.emplace_back(boost::bind(&handler_insert_value_of_linear
										<
											unsafe_midx_type, index_id0_type, iterator0_type
										>, _1, _2, boost::ref(call_count)),
								300, 300, "200");

		yggr_test_assert(bright);
		yggr_test_assert(call_count == 3);
		yggr_test_assert(cont.back() == A(300, 300, "200"));

		cont.pop_back();
		cont.pop_back();
		cont.pop_back();
		cont.pop_back();
		cont.pop_back();
	}

	{
		int call_count = 0;
		bool bright =
			cont.push_front(A(100, 100, "100"),
							boost::bind(&handler_insert_value_of_linear
										<
											unsafe_midx_type, index_id0_type, iterator0_type
										>, _1, _2, boost::ref(call_count)));

		yggr_test_assert(bright);
		yggr_test_assert(call_count == 1);
		yggr_test_assert(cont.front() == A(100, 100, "100"));

		bright =
			cont.emplace_front(yggr::tag_non_handler());

		yggr_test_assert(bright);
		yggr_test_assert(cont.front() == A(0, 0, ""));

		bright =
			cont.emplace_front(yggr::tag_non_handler(), 200, 200, "200");

		yggr_test_assert(bright);
		yggr_test_assert(cont.front() == A(200, 200, "200"));

		bright =
			cont.emplace_front(boost::bind(&handler_insert_value_of_linear
										<
											unsafe_midx_type, index_id0_type, iterator0_type
										>, _1, _2, boost::ref(call_count)) );

		yggr_test_assert(bright);
		yggr_test_assert(call_count == 2);
		yggr_test_assert(cont.front() == A(0, 0, ""));

		bright =
			cont.emplace_front(boost::bind(&handler_insert_value_of_linear
										<
											unsafe_midx_type, index_id0_type, iterator0_type
										>, _1, _2, boost::ref(call_count)),
								300, 300, "200");

		yggr_test_assert(bright);
		yggr_test_assert(call_count == 3);
		yggr_test_assert(cont.front() == A(300, 300, "200"));

		cont.pop_front();
		cont.pop_front();
		cont.pop_front();
		cont.pop_front();
		cont.pop_front();
	}

	{
		mi_cont_type cont;

		yggr_test_assert(cont.insert(boost::bind(&safe_index0_type::base_type::cbegin, _1), A(10, 10, "10")));
		A a(10, 10, "10");
		yggr_test_assert(cont.insert(boost::bind(&safe_index0_type::base_type::cbegin, _1), boost::move(a)));
		yggr_test_assert(cont.size() == 2);

		int call_count = 0;

		yggr_test_assert((cont.insert(boost::bind(&safe_index0_type::base_type::cbegin, _1),
							A(10, 10, "10"),
							boost::bind(&handler_insert_value_of_linear
										<
											unsafe_midx_type, index_id0_type, iterator0_type
										>, _1, _2, boost::ref(call_count)) ) ));

		yggr_test_assert(call_count == 1);
		yggr_test_assert(cont.size() == 3);

		yggr_test_assert((cont.insert(boost::bind(&safe_index0_type::base_type::cbegin, _1),
							3, A(10, 10, "10")) ));

		yggr_test_assert(cont.size() == 6);

		yggr_test_assert(cont.insert(boost::bind(&safe_index0_type::base_type::cbegin, _1), arr + 0, arr + 10));
		yggr_test_assert(cont.size() == 16);


		yggr_test_assert(cont.replace(boost::bind(&safe_index0_type::base_type::cbegin, _1), A(100, 100, "100")));

		A tmp1;

		yggr_test_assert(cont.modify(boost::bind(&safe_index0_type::base_type::cbegin, _1),
							boost::bind(&modifier<A, A>, _1, A(1, 100, "100"), boost::ref(tmp1)) ));

		yggr_test_assert(!cont.modify(boost::bind(&safe_index0_type::base_type::cend, _1),
							boost::bind(&modifier<A, A>, _1, A(1, 100, "100"), boost::ref(tmp1)) ));

		yggr_test_assert(cont.modify(boost::bind(&safe_index0_type::base_type::cbegin, _1),
							boost::bind(&modifier<A, A>, _1, A(1, 100, "100"), boost::ref(tmp1)),
							boost::bind(&rollback<A>, _1, boost::cref(tmp1)) ));

		yggr_test_assert(cont.modify(boost::bind(&safe_index0_type::base_type::cbegin, _1),
							yggr::safe_container::make_optimization_multi_index_modifier<unsafe_midx_type>(
								A(1, 100, "100"),
								boost::bind(&modifier<A, A>, _1, A(1, 100, "100")))));

		yggr_test_assert(cont.modify(boost::bind(&safe_index0_type::base_type::cbegin, _1),
							yggr::safe_container::make_optimization_multi_index_modifier<unsafe_midx_type>(
								A(1, 100, "100"),
								boost::bind(&modifier<A, A>, _1, A(1, 100, "100"), boost::ref(tmp1))),
							boost::bind(&rollback<A>, _1, boost::cref(tmp1)) ) );


		yggr_test_assert(cont.size() == 16);

		yggr_test_assert(cont.emplace(boost::bind(&safe_index0_type::base_type::cbegin, _1),
								yggr::tag_non_handler()));

		yggr_test_assert(cont.size() == 17);

		yggr_test_assert(cont.emplace(boost::bind(&safe_index0_type::base_type::cbegin, _1),
								yggr::tag_non_handler(), 200, 200, "200"));

		yggr_test_assert(cont.size() == 18);

		call_count = 0;

		yggr_test_assert(cont.emplace(boost::bind(&safe_index0_type::base_type::cbegin, _1),
								boost::bind(&handler_insert_value_of_linear
										<
											unsafe_midx_type, index_id0_type, iterator0_type
										>, _1, _2, boost::ref(call_count)) ));

		yggr_test_assert(cont.size() == 19);
		yggr_test_assert(call_count == 1);

		yggr_test_assert(cont.emplace(boost::bind(&safe_index0_type::base_type::cbegin, _1),
								boost::bind(&handler_insert_value_of_linear
										<
											unsafe_midx_type, index_id0_type, iterator0_type
										>, _1, _2, boost::ref(call_count)), 200, 200, "200" ));

		yggr_test_assert(cont.size() == 20);
		yggr_test_assert(call_count == 2);
	}

	// extract and insert
	{
		typedef yggr::safe_container::nth_index<mi_cont_type, 0>::type safe_index0_type;
		typedef safe_index0_type::index_id_type index_id0_type;
		typedef safe_index0_type::iterator iterator0_type;
		typedef safe_index0_type::node_type node0_type;
		typedef safe_index0_type::base_insert_return_type base_insert_return0_type;
		typedef safe_index0_type::insert_return_type insert_return0_type;

		typedef yggr::safe_container::nth_index<mi_cont_type, 1>::type safe_index1_type;
		typedef safe_index1_type::index_id_type index_id1_type;
		typedef safe_index1_type::iterator iterator1_type;
		typedef safe_index1_type::node_type node1_type;
		typedef safe_index1_type::base_insert_return_type base_insert_return1_type;
		typedef safe_index1_type::insert_return_type insert_return1_type;


		mi_cont_type cont;
		cont.push_back(arr[0]);
		cont.push_back(arr[1]);
		cont.push_back(arr[2]);

		// 0, 0
		{
			std::size_t old_size = cont.size();
			node0_type node = cont.extract(boost::bind(&safe_index0_type::base_type::cbegin, _1));
			yggr_test_assert(!node.empty());
			yggr_test_assert(cont.size() == old_size - 1);
			yggr_test_assert(node.value() == arr[0]);

			insert_return0_type ins_ret = cont.insert(boost::bind(&safe_index0_type::base_type::cend, _1), boost::move(node));
			yggr_test_assert(ins_ret.inserted);
			yggr_test_assert(ins_ret.node.empty());
			yggr_test_assert(cont.size() == old_size);
		}

		{
			std::size_t old_size = cont.size();
			node0_type node = cont.extract(boost::bind(&safe_index0_type::base_type::cbegin, _1));
			yggr_test_assert(!node.empty());
			yggr_test_assert(cont.size() == old_size - 1);
			yggr_test_assert(node.value() == arr[1]);

			insert_return0_type ins_ret =
				cont.insert(
					boost::bind(&safe_index0_type::base_type::cend, _1),
					boost::move(node),
					boost::bind(
						&handler_insert_node_of_linear<unsafe_midx_type, index_id0_type, base_insert_return0_type>,
						_1, _2) );
			yggr_test_assert(ins_ret.inserted);
			yggr_test_assert(ins_ret.node.empty());
			yggr_test_assert(cont.size() == old_size);
		}

		{
			std::size_t old_size = cont.size();
			node0_type node = cont.extract(boost::bind(&safe_index0_type::base_type::cbegin, _1));
			yggr_test_assert(!node.empty());
			yggr_test_assert(cont.size() == old_size - 1);
			yggr_test_assert(node.value() == arr[2]);

			insert_return0_type ins_ret = cont.insert(boost::bind(&safe_index0_type::base_type::cend, _1), boost::move(node));
			yggr_test_assert(ins_ret.inserted);
			yggr_test_assert(ins_ret.node.empty());
			yggr_test_assert(cont.size() == old_size);
		}

		// 0, 1
		{
			std::size_t old_size = cont.size();
			node0_type node = cont.get<0>().extract(boost::bind(&safe_index0_type::base_type::cbegin, _1));
			yggr_test_assert(!node.empty());
			yggr_test_assert(cont.size() == old_size - 1);
			yggr_test_assert(node.value() == arr[0]);

			insert_return1_type ins_ret = cont.get<1>().insert(boost::bind(&safe_index1_type::base_type::cend, _1), boost::move(node));
			yggr_test_assert(ins_ret.inserted);
			yggr_test_assert(ins_ret.node.empty());
			yggr_test_assert(cont.size() == old_size);
		}

		{
			std::size_t old_size = cont.size();
			node0_type node = cont.get<0>().extract(boost::bind(&safe_index0_type::base_type::cbegin, _1));
			yggr_test_assert(!node.empty());
			yggr_test_assert(cont.size() == old_size - 1);
			yggr_test_assert(node.value() == arr[1]);

			insert_return1_type ins_ret =
				cont.get<1>().insert(
					boost::bind(&safe_index1_type::base_type::cend, _1),
					boost::move(node),
					boost::bind(
						&handler_insert_node_of_linear<unsafe_midx_type, index_id1_type, base_insert_return1_type>,
						_1, _2) );
			yggr_test_assert(ins_ret.inserted);
			yggr_test_assert(ins_ret.node.empty());
			yggr_test_assert(cont.size() == old_size);
		}

		{
			std::size_t old_size = cont.size();
			node0_type node = cont.get<0>().extract(boost::bind(&safe_index0_type::base_type::cbegin, _1));
			yggr_test_assert(!node.empty());
			yggr_test_assert(cont.size() == old_size - 1);
			yggr_test_assert(node.value() == arr[2]);

			insert_return1_type ins_ret = cont.get<1>().insert(boost::bind(&safe_index1_type::base_type::cend, _1), boost::move(node));
			yggr_test_assert(ins_ret.inserted);
			yggr_test_assert(ins_ret.node.empty());
			yggr_test_assert(cont.size() == old_size);
		}

		// 1, 1
		{
			std::size_t old_size = cont.size();
			node1_type node = cont.get<1>().extract(boost::bind(&safe_index1_type::base_type::cbegin, _1));
			yggr_test_assert(!node.empty());
			yggr_test_assert(cont.size() == old_size - 1);
			yggr_test_assert(node.value() == arr[0]);

			insert_return1_type ins_ret = cont.get<1>().insert(boost::bind(&safe_index1_type::base_type::cend, _1), boost::move(node));
			yggr_test_assert(ins_ret.inserted);
			yggr_test_assert(ins_ret.node.empty());
			yggr_test_assert(cont.size() == old_size);
		}

		{
			std::size_t old_size = cont.size();
			node1_type node = cont.get<1>().extract(boost::bind(&safe_index1_type::base_type::cbegin, _1));
			yggr_test_assert(!node.empty());
			yggr_test_assert(cont.size() == old_size - 1);
			yggr_test_assert(node.value() == arr[1]);

			insert_return1_type ins_ret =
				cont.get<1>().insert(
					boost::bind(&safe_index1_type::base_type::cend, _1),
					boost::move(node),
					boost::bind(
						&handler_insert_node_of_linear<unsafe_midx_type, index_id1_type, base_insert_return1_type>,
						_1, _2) );
			yggr_test_assert(ins_ret.inserted);
			yggr_test_assert(ins_ret.node.empty());
			yggr_test_assert(cont.size() == old_size);
		}

		{
			std::size_t old_size = cont.size();
			node1_type node = cont.get<1>().extract(boost::bind(&safe_index1_type::base_type::cbegin, _1));
			yggr_test_assert(!node.empty());
			yggr_test_assert(cont.size() == old_size - 1);
			yggr_test_assert(node.value() == arr[2]);

			insert_return1_type ins_ret = cont.get<1>().insert(boost::bind(&safe_index1_type::base_type::cend, _1), boost::move(node));
			yggr_test_assert(ins_ret.inserted);
			yggr_test_assert(ins_ret.node.empty());
			yggr_test_assert(cont.size() == old_size);
		}
	}

	struct iter_range_t
	{
		static
		std::pair<safe_index0_type::iterator, safe_index0_type::iterator>
			iter_range(safe_index0_type::base_type& base)
		{
			return std::make_pair(base.begin(), base.end());
		}

		static
		std::pair<safe_index0_type::iterator, safe_index0_type::iterator>
			iter_range_1(safe_index0_type::base_type& base)
		{
			yggr_test_assert(base.size() > 1);
			safe_index0_type::iterator iter = base.begin();
			return std::make_pair(++iter, base.end());
		}

		static
		safe_index0_type::iterator
			iter_pos_1(safe_index0_type::base_type& base)
		{
			yggr_test_assert(base.size() > 1);
			safe_index0_type::iterator iter = base.begin();
			return ++iter;
		}
	};

	// splice(base_type&)
	{
		mi_cont_type cont(arr + 0, arr + 10);
		unsafe_midx_type right(arr + 0, arr + 10);

		cont.splice(boost::move(right));

		yggr_test_assert(cont.size() == 20);
		yggr_test_assert(right.size() == 0);
	}

	{
		mi_cont_type cont(arr + 0, arr + 10);
		unsafe_midx_type right(arr + 0, arr + 10);

		cont.splice(boost::move(right), right.begin());

		yggr_test_assert(cont.size() == 11);
		yggr_test_assert(right.size() == 9);
	}

	{
		mi_cont_type cont(arr + 0, arr + 10);
		unsafe_midx_type right(arr + 0, arr + 10);

		cont.splice(boost::move(right), right.begin(), right.end());

		yggr_test_assert(cont.size() == 20);
		yggr_test_assert(right.size() == 0);
	}

	// splice(iter_handler, base)
	{
		mi_cont_type cont(arr + 0, arr + 10);
		unsafe_midx_type right(arr + 0, arr + 10);

		cont.splice(boost::bind(&safe_index0_type::base_type::cbegin, _1), boost::move(right));

		yggr_test_assert(cont.size() == 20);
		yggr_test_assert(right.size() == 0);
	}

	{
		mi_cont_type cont(arr + 0, arr + 10);
		unsafe_midx_type right(arr + 0, arr + 10);

		cont.splice(boost::bind(&safe_index0_type::base_type::cbegin, _1),
						boost::move(right), right.begin());

		yggr_test_assert(cont.size() == 11);
		yggr_test_assert(right.size() == 9);
	}

	{
		mi_cont_type cont(arr + 0, arr + 10);
		unsafe_midx_type right(arr + 0, arr + 10);

		cont.splice(boost::bind(&safe_index0_type::base_type::cbegin, _1),
						boost::move(right), right.begin(), right.end());

		yggr_test_assert(cont.size() == 20);
		yggr_test_assert(right.size() == 0);
	}

	// splice(this_type&)
	{
		mi_cont_type cont(arr + 0, arr + 10);
		mi_cont_type right(arr + 0, arr + 10);

		cont.splice(boost::move(right));

		yggr_test_assert(cont.size() == 20);
		yggr_test_assert(right.size() == 0);
	}

	{
		mi_cont_type cont(arr + 0, arr + 10);
		mi_cont_type right(arr + 0, arr + 10);

		cont.splice(boost::move(right), boost::bind(&safe_index0_type::base_type::cbegin, _1));

		yggr_test_assert(cont.size() == 11);
		yggr_test_assert(right.size() == 9);
	}

	{
		mi_cont_type cont(arr + 0, arr + 10);
		mi_cont_type right(arr + 0, arr + 10);

		cont.splice(boost::move(right), boost::bind(&iter_range_t::iter_range, _1));

		yggr_test_assert(cont.size() == 20);
		yggr_test_assert(right.size() == 0);
	}

	{
		mi_cont_type cont(arr + 0, arr + 10);
		mi_cont_type right(arr + 0, arr + 10);

		cont.splice(boost::move(right), boost::bind(&safe_index0_type::base_type::cbegin, _1),
							boost::bind(&safe_index0_type::base_type::cend, _1) );

		yggr_test_assert(cont.size() == 20);
		yggr_test_assert(right.size() == 0);
	}

	// splice(iter_handler, this_type&)
	{
		mi_cont_type cont(arr + 0, arr + 10);
		mi_cont_type right(arr + 0, arr + 10);

		cont.splice(boost::bind(&safe_index0_type::base_type::cend, _1),
						boost::move(right));

		yggr_test_assert(cont.size() == 20);
		yggr_test_assert(right.size() == 0);
	}

	{
		mi_cont_type cont(arr + 0, arr + 10);
		mi_cont_type right(arr + 0, arr + 10);

		cont.splice(boost::bind(&safe_index0_type::base_type::cend, _1),
						boost::move(right),
						boost::bind(&safe_index0_type::base_type::cbegin, _1));

		yggr_test_assert(cont.size() == 11);
		yggr_test_assert(right.size() == 9);
	}

	{
		mi_cont_type cont(arr + 0, arr + 10);
		mi_cont_type right(arr + 0, arr + 10);

		cont.splice(boost::bind(&safe_index0_type::base_type::cend, _1),
						boost::move(right),
						boost::bind(&iter_range_t::iter_range, _1));

		yggr_test_assert(cont.size() == 20);
		yggr_test_assert(right.size() == 0);
	}

	{
		mi_cont_type cont(arr + 0, arr + 10);
		mi_cont_type right(arr + 0, arr + 10);

		cont.splice(boost::bind(&safe_index0_type::base_type::cend, _1),
						boost::move(right),
						boost::bind(&safe_index0_type::base_type::cbegin, _1),
						boost::bind(&safe_index0_type::base_type::cend, _1));

		yggr_test_assert(cont.size() == 20);
		yggr_test_assert(right.size() == 0);
	}

	//cont.get<0>().remove(A(1, 100, "aaa")); // warning:right
	cont.get<0>().remove(A(1, 1, "aaa"));

	yggr_test_assert(cont.size() == 9);

	A rm_cmp(3, 0, "");
	cont.get<0>().remove_if(boost::bind(&less_foo, _1, boost::cref(rm_cmp)));
	yggr_test_assert(cont.size() == 2);

	mi_cont_type::base_type base_cont(arr + 0, arr + 10);

	cont.get<0>().push_back(A(10, 10, "10"));
	cont.get<0>().push_back(A(10, 10, "10"));
	cont.get<0>().push_back(A(10, 10, "10"));
	cont.get<0>().push_back(A(10, 10, "10"));

	cont.get<0>().sort();

	yggr_test_assert(cont.size() == 6);

	cont.get<0>().unique();

	yggr_test_assert(cont.size() == 3);

	cont.get<0>().push_back(A(10, 10, "10"));
	cont.get<0>().push_back(A(10, 10, "10"));
	cont.get<0>().push_back(A(10, 10, "10"));

	yggr_test_assert(cont.size() == 6);

	cont.get<0>().sort();
	cont.get<0>().unique(boost::bind(&less_foo, _1, _2));
	yggr_test_assert(cont.size() == 6);

	cont.get<0>().reverse();

	cont.get<0>().unique(boost::bind(&less_foo, _1, _2));

	mi_cont_type::base_type base_cont2(arr + 0, arr + 10);
	cont.get<0>().merge(base_cont2);

	yggr_test_assert(base_cont2.size() == 0);

	yggr_test_assert(cont.size() == 14);
	yggr_test_assert(base_cont.size() == 10);

	cont.get<0>().swap(base_cont.get<0>());
	yggr_test_assert(cont.size() == 10);
	yggr_test_assert(base_cont.size() == 14);

	cont.get<A::tag_prm1>().swap(base_cont.get<A::tag_prm1>());
	yggr_test_assert(cont.size() == 14);
	yggr_test_assert(base_cont.size() == 10);

	// erase
	yggr_test_assert(cont.erase(boost::bind(&unsafe_sequenced_type::cbegin, _1)));
	yggr_test_assert(cont.erase(boost::bind(&unsafe_sequenced_type::cbegin, _1),
						boost::bind(&unsafe_sequenced_type::cend, _1)));

	yggr_test_assert(cont.empty());

	// remove
	{
		cont.get<0>().push_back(A(10, 10, "10"));
		cont.get<0>().push_back(A(10, 10, "10"));
		cont.get<0>().push_back(A(10, 10, "10"));
		cont.get<0>().push_back(A(10, 10, "10"));
		cont.get<0>().push_back(A(11, 11, "11"));

		cont.remove(A(10, 10, "10"));

		yggr_test_assert(cont.size() == 1);
		cont.clear();
	}

	// remove_if
	{
		cont.get<0>().push_back(A(10, 10, "10"));
		cont.get<0>().push_back(A(10, 10, "10"));
		cont.get<0>().push_back(A(10, 10, "10"));
		cont.get<0>().push_back(A(10, 10, "10"));
		cont.get<0>().push_back(A(11, 11, "11"));

		cont.remove_if(boost::bind(std::equal_to<A>(), _1, A(10, 10, "10")));

		yggr_test_assert(cont.size() == 1);
		cont.clear();
	}

	// unique
	{
		cont.get<0>().push_back(A(10, 10, "10"));
		cont.get<0>().push_back(A(10, 10, "10"));
		cont.get<0>().push_back(A(10, 10, "10"));
		cont.get<0>().push_back(A(10, 10, "10"));
		cont.get<0>().push_back(A(11, 11, "11"));

		cont.unique();

		yggr_test_assert(cont.size() == 2);
		cont.clear();
	}

	{
		cont.get<0>().push_back(A(10, 10, "10"));
		cont.get<0>().push_back(A(10, 10, "10"));
		cont.get<0>().push_back(A(10, 10, "10"));
		cont.get<0>().push_back(A(10, 10, "10"));
		cont.get<0>().push_back(A(11, 11, "11"));

		cont.unique(std::equal_to<A>());

		yggr_test_assert(cont.size() == 2);
		cont.clear();
	}

	// merge
	{
		mi_cont_type cont1(arr + 0, arr + 10);
		mi_cont_type::base_type cont2(arr + 0, arr + 10);

		cont1.merge(boost::move(cont2));

		yggr_test_assert(cont1.size() == 20);
		yggr_test_assert(cont2.size() == 0);
	}

	{
		mi_cont_type cont1(arr + 0, arr + 10);
		mi_cont_type::base_type cont2(arr + 0, arr + 10);

		cont1.merge(boost::move(cont2), std::less<A>());

		yggr_test_assert(cont1.size() == 20);
		yggr_test_assert(cont2.size() == 0);
	}

	{
		mi_cont_type cont1(arr + 0, arr + 10);
		mi_cont_type cont2(arr + 0, arr + 10);

		cont1.merge(boost::move(cont2));

		yggr_test_assert(cont1.size() == 20);
		yggr_test_assert(cont2.size() == 0);
	}

	{
		mi_cont_type cont1(arr + 0, arr + 10);
		mi_cont_type cont2(arr + 0, arr + 10);

		cont1.merge(boost::move(cont2), std::less<A>());

		yggr_test_assert(cont1.size() == 20);
		yggr_test_assert(cont2.size() == 0);
	}

	{
		mi_cont_type cont1(arr + 0, arr + 10);

		cont1.sort();

		yggr_test_assert(cont1.front() < cont1.back());
	}

	{
		mi_cont_type cont1(arr + 0, arr + 10);

		cont1.sort(std::less<A>());

		yggr_test_assert(cont1.front() < cont1.back());

		cont1.reverse();

		yggr_test_assert(!(cont1.front() < cont1.back()));
	}

	// relocate
	{
		mi_cont_type cont1(arr + 0, arr + 10);

		yggr_test_assert(cont1.size() == 10);
		yggr_test_assert(cont1.front() == arr[0]);

		cont1.relocate(boost::bind(&safe_index0_type::base_type::cbegin, _1),
						boost::bind(&iter_range_t::iter_pos_1, _1));

		yggr_test_assert(cont1.front() == arr[1]);
	}

	{
		mi_cont_type cont1(arr + 0, arr + 10);

		yggr_test_assert(cont1.size() == 10);
		yggr_test_assert(cont1.front() == arr[0]);

		cont1.relocate(boost::bind(&safe_index0_type::base_type::cbegin, _1),
						boost::bind(&safe_index0_type::base_type::cend, _1));

		yggr_test_assert(cont1.front() == arr[0]);
		yggr_test_assert(cont1.back() == arr[9]);
		yggr_test_assert(cont1.size() == 10);
	}

	{
		mi_cont_type cont1(arr + 0, arr + 10);

		yggr_test_assert(cont1.size() == 10);
		yggr_test_assert(cont1.front() == arr[0]);

		cont1.relocate(boost::bind(&safe_index0_type::base_type::cbegin, _1),
						boost::bind(&iter_range_t::iter_range_1, _1));

		yggr_test_assert(cont1.front() == arr[1]);
		yggr_test_assert(cont1.back() == arr[0]);
	}

	{
		mi_cont_type cont1(arr + 0, arr + 10);

		yggr_test_assert(cont1.size() == 10);
		yggr_test_assert(cont1.front() == arr[0]);

		cont1.relocate(boost::bind(&safe_index0_type::base_type::cbegin, _1),
						boost::bind(&iter_range_t::iter_pos_1, _1),
						boost::bind(&safe_index0_type::base_type::cend, _1));

		yggr_test_assert(cont1.front() == arr[1]);
		yggr_test_assert(cont1.back() == arr[0]);
	}

	//rearrange

	// basic_test
	{
		mi_cont_type::base_type cont1(arr + 0, arr + 10);

		yggr_test_assert(cont1.get<0>().front() == arr[0]);
		yggr_test_assert(cont1.get<0>().back() == arr[9]);

		yggr_test_assert(cont1.get<1>().front() == arr[0]);
		yggr_test_assert(cont1.get<1>().back() == arr[9]);

		cont1.get<1>().reverse();

		yggr_test_assert(cont1.get<0>().front() == arr[0]);
		yggr_test_assert(cont1.get<0>().back() == arr[9]);

		yggr_test_assert(cont1.get<1>().front() == arr[9]);
		yggr_test_assert(cont1.get<1>().back() == arr[0]);
	}

	struct view_foo_t
	{
		typedef boost::reference_wrapper<A const> a_ref_type;
		typedef std::vector<a_ref_type> aref_vt_type;

		static
		safe_index1_type::iterator rearrange_reverse_1(mi_cont_type::base_type& base)
		{
			base.get<1>().reverse();
			return base.get<1>().begin();
		}

		static
		safe_index0_type::iterator non_rearrange_0(mi_cont_type::base_type& base)
		{
			return base.get<0>().begin();
		}

		static
		aref_vt_type::iterator rearrange_vt(mi_cont_type::base_type& base, aref_vt_type& aref_vt)
		{
			for(safe_index0_type::iterator i = base.get<0>().begin(), isize = base.get<0>().end();
				i != isize; ++i)
			{
				aref_vt.push_back(boost::cref(*i));
			}

			std::reverse(aref_vt.begin(), aref_vt.end());

			return aref_vt.begin();
		}
	};

	{
		bool bright = false;
		mi_cont_type cont1(arr + 0, arr + 10);

		yggr_test_assert(cont1.get<0>().front() == arr[0]);
		yggr_test_assert(cont1.get<0>().back() == arr[9]);

		yggr_test_assert(cont1.get<1>().front() == arr[0]);
		yggr_test_assert(cont1.get<1>().back() == arr[9]);

		bright = cont1.rearrange(boost::bind(&view_foo_t::non_rearrange_0, _1));
		yggr_test_assert(bright);

		yggr_test_assert(cont1.get<0>().front() == arr[0]);
		yggr_test_assert(cont1.get<0>().back() == arr[9]);

		yggr_test_assert(cont1.get<1>().front() == arr[0]);
		yggr_test_assert(cont1.get<1>().back() == arr[9]);

		bright = cont1.rearrange(boost::bind(&view_foo_t::rearrange_reverse_1, _1));
		yggr_test_assert(bright);

		yggr_test_assert(cont1.get<0>().front() == arr[9]);
		yggr_test_assert(cont1.get<0>().back() == arr[0]);

		yggr_test_assert(cont1.get<1>().front() == arr[9]);
		yggr_test_assert(cont1.get<1>().back() == arr[0]);
	}

	{
		bool bright = false;
		// deliberately assigning afref_vt to release after cont1 to test safety
		view_foo_t::aref_vt_type helper_vt;
		mi_cont_type cont1(arr + 0, arr + 10);

		yggr_test_assert(cont1.get<0>().front() == arr[0]);
		yggr_test_assert(cont1.get<0>().back() == arr[9]);

		yggr_test_assert(cont1.get<1>().front() == arr[0]);
		yggr_test_assert(cont1.get<1>().back() == arr[9]);

		bright = cont1.rearrange(boost::bind(&view_foo_t::rearrange_vt, _1, boost::ref(helper_vt)));
		yggr_test_assert(bright);

		yggr_test_assert(cont1.get<0>().front() == arr[9]);
		yggr_test_assert(cont1.get<0>().back() == arr[0]);

		yggr_test_assert(cont1.get<1>().front() == arr[0]);
		yggr_test_assert(cont1.get<1>().back() == arr[9]);
	}

	cont.clear();
	yggr_test_assert(cont.empty());

	std::cout << "---------------test_modifiers_of_sequenced end-------------------------" << std::endl;
}

void test_swap_of_sequenced(void)
{
	typedef yggr::safe_container::safe_multi_index_container<
			A,
			boost::multi_index::indexed_by<
				boost::multi_index::sequenced< boost::multi_index::tag<A::tag_prm1> >,
				boost::multi_index::sequenced< boost::multi_index::tag<A::tag_prm2> >
			>
		> mi_cont_type;
	test_swap<mi_cont_type>();
	std::cout << "---------------test_swap_of_sequenced end-----------------" << std::endl;
}

void test_operator_set_of_sequenced(void)
{
	typedef yggr::safe_container::safe_multi_index_container<
			A,
			boost::multi_index::indexed_by<
				boost::multi_index::sequenced< boost::multi_index::tag<A::tag_prm1> >,
				boost::multi_index::sequenced< boost::multi_index::tag<A::tag_prm2> >
			>
		> mi_cont_type;
	test_operator_set<mi_cont_type>();
	std::cout << "---------------test_operator_set_of_sequenced end-----------------" << std::endl;
}

void test_comparer_of_sequenced(void)
{
	typedef yggr::safe_container::safe_multi_index_container<
			A,
			boost::multi_index::indexed_by<
				boost::multi_index::sequenced< boost::multi_index::tag<A::tag_prm1> >,
				boost::multi_index::sequenced< boost::multi_index::tag<A::tag_prm2> >
			>
		> mi_cont_type;
	test_compare_of_full<mi_cont_type>();
	std::cout << "---------------test_comparer_of_sequenced end-----------------" << std::endl;
}

void test_safe_other_of_sequenced(void)
{
	typedef
		yggr::safe_container::safe_multi_index_container
		<
			A,
			boost::multi_index::indexed_by<
				boost::multi_index::sequenced< boost::multi_index::tag<A::tag_prm1> >,
				boost::multi_index::sequenced< boost::multi_index::tag<A::tag_prm2> >
			>
		> mi_cont_type;

	typedef mi_cont_type::base_type unsafe_midx_type;

	typedef boost::multi_index::nth_index<mi_cont_type, 0>::type safe_index0_type;
	typedef safe_index0_type::index_id_type index_id0_type;
	typedef safe_index0_type::iterator iterator0_type;

	typedef boost::multi_index::nth_index<mi_cont_type, 1>::type safe_index1_type;
	typedef safe_index1_type::index_id_type index_id1_type;
	typedef safe_index1_type::iterator iterator1_type;

	test_safe_other_gernel<mi_cont_type>();

	A arr[10] = {A(1, 1, "aaa"), A(2, 2, "bbb"), A(3, 3, "ccc"), A(4, 3, "ddd")};

	mi_cont_type cont(arr + 0, arr + 10);

	yggr_test_assert(cont.is_exists(A(1, 1, "aaa")));
	yggr_test_assert(!cont.is_exists(A(100, 100, "100")));

	yggr_test_assert(cont.is_exists(boost::bind(std::equal_to<A>(), A(1, 1, "aaa"), _1)));
	yggr_test_assert(!cont.is_exists(boost::bind(std::equal_to<A>(), A(100, 100, "100"), _1)));

	{
		A beg_val, rbeg_val;

		mi_cont_type cont_empty;
		yggr_test_assert(!cont_empty.get_begin_value(beg_val));
		yggr_test_assert(!cont_empty.get_rbegin_value(rbeg_val));

		try
		{
			cont_empty.begin_value();
			yggr_test_assert(false);
		}
		catch(const mi_cont_type::error_type&)
		{
		}

		try
		{
			cont_empty.rbegin_value();
			yggr_test_assert(false);
		}
		catch(const mi_cont_type::error_type&)
		{
		}

		cont.get_begin_value(beg_val);
		cont.get_rbegin_value(rbeg_val);

		yggr_test_assert(beg_val == cont.begin_value());
		yggr_test_assert(rbeg_val == cont.rbegin_value());
	}

	{
		int call_count = 0;
		cont.for_each_iterator_reverse(boost::bind(&handler_for_each<typename safe_index0_type::reverse_iterator>,
														_1, boost::ref(call_count)));
		yggr_test_assert(call_count == cont.size());

		call_count = 0;
		cont.for_each_iterator_reverse(boost::bind(&handler_for_each<typename safe_index0_type::reverse_iterator>,
														_1, boost::ref(call_count)),
										boost::bind(&handler_filter<typename safe_index0_type::reverse_iterator>,
														_1, false));
		yggr_test_assert(call_count == 0);

		call_count = 0;
		cont.for_each_iterator_reverse(boost::bind(&handler_for_each<typename safe_index0_type::reverse_iterator>,
														_1, boost::ref(call_count)),
										boost::bind(&handler_filter<typename safe_index0_type::reverse_iterator>,
														_1, true));
		yggr_test_assert(call_count == cont.size());
	}

	{
		int call_count = 0;
		cont.for_each_reverse(boost::bind(&handler_for_each<typename safe_index0_type::value_type>,
											_1, boost::ref(call_count)));
		yggr_test_assert(call_count == cont.size());

		call_count = 0;
		cont.for_each_reverse(boost::bind(&handler_for_each<typename safe_index0_type::value_type>,
											_1, boost::ref(call_count)),
								boost::bind(&handler_filter<typename safe_index0_type::value_type>,
											_1, false));
		yggr_test_assert(call_count == 0);

		call_count = 0;
		cont.for_each_reverse(boost::bind(&handler_for_each<typename safe_index0_type::value_type>,
											_1, boost::ref(call_count)),
								boost::bind(&handler_filter<typename safe_index0_type::value_type>,
											_1, true));
		yggr_test_assert(call_count == cont.size());
	}

	std::cout << "---------------test_safe_other_of_sequenced end-----------------" << std::endl;
}

void debug_test(void)
{
	test_construct_of_hash();
	test_observers_of_hash();
	test_capacity_of_hash();
	test_modifiers_of_hash();
	test_lookup_of_hash();
	test_set_operations_of_hash();
	test_bucket_of_hash();
	test_hash_policy_of_hash();
	test_swap_of_hash();
	test_operator_set_of_hash();
	test_comparer_of_hash();
	test_safe_other_of_hash();

	std::cout << "///////////////////////////////////////" << std::endl;

	test_construct_of_ordered();
	test_observers_of_ordered();
	test_capacity_of_ordered();
	test_modifiers_of_ordered();
	test_lookup_of_ordered();
	test_set_operations_of_ordered();
	test_swap_of_ordered();
	test_operator_set_of_ordered();
	test_comparer_of_ordered();
	test_safe_other_of_ordered();

	std::cout << "////////////////////////////////////" << std::endl;

#ifdef YGGR_BOOST_MULTI_INDEX_HAS_RANKED_INDEX

	test_construct_of_ranked();
	test_observers_of_ranked();
	test_capacity_of_ranked();
	test_modifiers_of_ranked();
	test_lookup_of_ranked();
	test_rank_operations_of_randked();
	test_set_operations_of_ranked();
	test_swap_of_ranked();
	test_operator_set_of_ranked();
	test_comparer_of_ranked();
	test_safe_other_of_ranked();

#else

	std::cout << "!!!test randed_index need boost >= 1.59!!!" << std::endl;

#endif // YGGR_BOOST_MULTI_INDEX_HAS_RANKED_INDEX

	std::cout << "////////////////////////////////////" << std::endl;

	test_construct_of_sequenced();
	test_observers_of_sequenced();
	test_capacity_of_sequenced();
	test_modifiers_of_sequenced();
	test_swap_of_sequenced();
	test_operator_set_of_sequenced();
	test_comparer_of_sequenced();
	test_safe_other_of_sequenced();

	std::cout << "////////////////////////////////////" << std::endl;

	std::cout << "all test end" << std::endl;
}

int main(int argc, char* argv[])
{
	debug_test();

	wait_any_key(argc, argv);
	return 0;
}
