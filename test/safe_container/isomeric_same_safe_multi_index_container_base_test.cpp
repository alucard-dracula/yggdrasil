//safe_multi_index_indexes_base_test.cpp

//#define YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION

#include <test/wait_any_key/wait_any_key.hpp>
#include <string>
#include <cassert>

#include <boost/mpl/size_t.hpp>

#include <boost/multi_index/member.hpp>
#include <boost/bind.hpp>

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

template<typename Container, typename Isomeric_Same_Container>
void test_construct(void)
{
	typedef Container mi_cont_type;
	typedef Isomeric_Same_Container isomeric_same_mi_cont_type;

	BOOST_MPL_ASSERT((boost::is_same<typename mi_cont_type::base_type, typename isomeric_same_mi_cont_type::base_type>));

	A arr[10] = {A(1, 1, "aaa"), A(2, 2, "bbb"), A(3, 3, "ccc"), A(4, 3, "ddd")};

	{
		mi_cont_type chk_cont(arr + 0, arr + 10);
		isomeric_same_mi_cont_type isomeric_same_cont(arr + 0, arr + 10);
		mi_cont_type cont(boost::move(isomeric_same_cont));
		yggr_test_assert(isomeric_same_cont.size() == 0); // ub
		yggr_test_assert(cont == chk_cont);
	}

	{
		mi_cont_type chk_cont(arr + 0, arr + 10);
		isomeric_same_mi_cont_type isomeric_same_cont(arr + 0, arr + 10);
		mi_cont_type cont(isomeric_same_cont);
		yggr_test_assert(isomeric_same_cont.size());
		yggr_test_assert(cont == chk_cont);
	}

	{
		mi_cont_type chk_cont(arr + 0, arr + 10);
		isomeric_same_mi_cont_type isomeric_same_cont(arr + 0, arr + 10);
		mi_cont_type cont(boost::move(isomeric_same_cont), isomeric_same_cont.get_allocator());
		yggr_test_assert(isomeric_same_cont.size() == 0); // ub
		yggr_test_assert(cont == chk_cont);
	}

	{
		mi_cont_type chk_cont(arr + 0, arr + 10);
		isomeric_same_mi_cont_type isomeric_same_cont(arr + 0, arr + 10);
		mi_cont_type cont(isomeric_same_cont, isomeric_same_cont.get_allocator());
		yggr_test_assert(isomeric_same_cont.size());
		yggr_test_assert(cont == chk_cont);
	}
}

template<typename Container, typename Isomeric_Same_Container>
void test_swap(void)
{
	typedef Container mi_cont_type;
	typedef Isomeric_Same_Container isomeric_same_mi_cont_type;

	A arr1[10] = {A(1, 1, "aaa"), A(2, 2, "bbb"), A(3, 3, "ccc"), A(4, 3, "ddd")};
	A arr2[10] = {A(6, 6, "aaa"), A(7, 7, "bbb"), A(8, 8, "ccc"), A(9, 8, "ddd")};

	mi_cont_type cont1(arr1 + 0, arr1 + 10);
	isomeric_same_mi_cont_type cont2(arr2 + 0, arr2 + 5);

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

	std::swap(cont1.template get<0>(), cont2.template get<0>());
	std::swap(cont2.template get<1>(), cont1.template get<1>());
	boost::swap(cont1.template get<0>(), cont2.template get<0>());
	boost::swap(cont2.template get<1>(), cont1.template get<1>());
	yggr::swap(cont1.template get<0>(), cont2.template get<0>());
	yggr::swap(cont2.template get<1>(), cont1.template get<1>());


	// this_type& this_type&
	boost::swap(cont1, cont2);
	boost::swap(cont1, cont2);

	// this_type&& this_type&
	boost::swap(boost::move(cont1), cont2);

	// this_type& this_type&&
	boost::swap(cont1, boost::move(cont2));

	// ------------------ std::swap test------------------------
	// this_type& this_type&
	std::swap(cont1, cont2);
	std::swap(cont1, cont2);

	// this_type&& this_type&
	std::swap(boost::move(cont1), cont2);

	// this_type& this_type&&
	std::swap(cont1, boost::move(cont2));

	// ------------------ yggr::swap test------------------------
	// this_type& this_type&
	yggr::swap(cont1, cont2);
	yggr::swap(cont1, cont2);

	// this_type&& this_type&
	yggr::swap(boost::move(cont1), cont2);

	// this_type& this_type&&
	yggr::swap(cont1, boost::move(cont2));

}

template<typename Container, typename Isomeric_Same_Container>
void test_operator_set(void)
{
	typedef Container safe_cont_type;
	typedef Isomeric_Same_Container isomeric_same_mi_cont_type;
	typedef typename safe_cont_type::base_type unsafe_cont_type;
	typedef typename isomeric_same_mi_cont_type::base_type unsafe_isomeric_same_cont_type;

	BOOST_MPL_ASSERT((boost::is_same<unsafe_cont_type, unsafe_isomeric_same_cont_type>));

	A arr1[10] = {A(1, 1, "aaa"), A(2, 2, "bbb"), A(3, 3, "ccc"), A(4, 3, "ddd")};
	A arr2[10] = {A(6, 6, "aaa"), A(7, 7, "bbb"), A(8, 8, "ccc"), A(9, 8, "ddd")};

	{
		// container this other
		//inline this_type& operator=(BOOST_RV_REF(this_type) right);

		safe_cont_type cont_safe1(arr1 + 0, arr1 + 10);
		isomeric_same_mi_cont_type cont_safe2(arr2 + 0, arr2 + 10);
		safe_cont_type cont_safe_chk(arr1 + 0, arr1 + 10);

		cont_safe2 = boost::move(cont_safe1);

		yggr_test_assert(cont_safe2 == cont_safe_chk);
		yggr_test_assert(cont_safe1 != cont_safe_chk);
	}

	{
		// container this other
		//this_type& operator=(const this_type& right);
		safe_cont_type cont_safe1(arr1 + 0, arr1 + 10);
		isomeric_same_mi_cont_type cont_safe2(arr2 + 0, arr2 + 10);

		cont_safe2 = cont_safe1;

		yggr_test_assert(cont_safe2 == cont_safe1);
	}

	{
		// index this other
		//inline this_type& operator=(BOOST_RV_REF(this_type) right);

		safe_cont_type cont_safe1;
		isomeric_same_mi_cont_type cont_safe2(arr1 + 0, arr1 + 10);
		safe_cont_type cont_safe_chk(arr1 + 0, arr1 + 10);

		cont_safe1.template get<0>() = boost::move(cont_safe2.template get<0>());

		yggr_test_assert(cont_safe1 == cont_safe_chk);
	}

	{
		// index this other
		//this_type& operator=(const this_type& right);

		safe_cont_type cont_safe1;
		isomeric_same_mi_cont_type cont_safe2(arr1 + 0, arr1 + 10);

		cont_safe1.template get<0>() = cont_safe2.template get<0>();
		cont_safe1.template get<1>() = cont_safe2.template get<1>();

		yggr_test_assert(!cont_safe1.empty());
		yggr_test_assert(cont_safe1 == cont_safe2);
	}

	{
		// index this this
		//inline this_type& operator=(BOOST_RV_REF(this_type) right);

		safe_cont_type cont_safe(arr1 + 0, arr1 + 10);
		isomeric_same_mi_cont_type cont_safe_chk(arr1 + 0, arr1 + 10);

		cont_safe.template get<0>() = boost::move(cont_safe.template get<0>());
		//cont_safe.get<1>() = boost::move(cont_safe.get<1>());

		yggr_test_assert(!cont_safe.empty());
		yggr_test_assert(cont_safe == cont_safe_chk);
	}

	{
		// index this this
		//this_type& operator=(const this_type& right);

		safe_cont_type cont_safe(arr1 + 0, arr1 + 10);
		isomeric_same_mi_cont_type cont_safe_chk(arr1 + 0, arr1 + 10);

		cont_safe.template get<0>() = cont_safe.template get<0>();
		cont_safe.template get<1>() = cont_safe.template get<1>();

		yggr_test_assert(!cont_safe.empty());
		yggr_test_assert(cont_safe == cont_safe_chk);
	}

	{
		// index this other
		//this_type& operator=(BOOST_RV_REF(safe_multi_index_containe_this) right);

		safe_cont_type cont_safe1;
		isomeric_same_mi_cont_type cont_safe2(arr1 + 0, arr1 + 10);
		safe_cont_type cont_safe_chk(arr1 + 0, arr1 + 10);

		cont_safe1.template get<0>() = boost::move(cont_safe2);

		yggr_test_assert(cont_safe1 == cont_safe_chk);
	}

	{
		// index this other
		//this_type& operator=(const safe_multi_index_containe_this& right);

		safe_cont_type cont_safe1;
		isomeric_same_mi_cont_type cont_safe2(arr1 + 0, arr1 + 10);

		cont_safe1.template get<0>() = cont_safe2;

		yggr_test_assert(!cont_safe2.empty());
		yggr_test_assert(cont_safe1 == cont_safe2);
	}

	{
		// index this this
		//this_type& operator=(BOOST_RV_REF(safe_multi_index_containe_this) right);

		safe_cont_type cont_safe(arr1 + 0, arr1 + 10);
		isomeric_same_mi_cont_type cont_safe_chk(arr1 + 0, arr1 + 10);

		cont_safe.template get<0>() = boost::move(cont_safe);

		yggr_test_assert(!cont_safe.empty());
		yggr_test_assert(cont_safe == cont_safe_chk);
	}

	{
		// index this this
		//this_type& operator=(const safe_multi_index_containe_this& right);

		safe_cont_type cont_safe(arr1 + 0, arr1 + 10);
		isomeric_same_mi_cont_type cont_safe_chk(arr1 + 0, arr1 + 10);

		cont_safe.template get<0>() = cont_safe;

		yggr_test_assert(!cont_safe.empty());
		yggr_test_assert(cont_safe == cont_safe_chk);
	}


#ifndef BOOST_NO_CXX11_HDR_INITIALIZER_LIST
	{
		//this_type& operator=(std::initializer_list<value_type> list);
		safe_cont_type cont_safe;
		isomeric_same_mi_cont_type cont_safe_chk(arr1 + 0, arr1 + 4);

		cont_safe.template get<0>() = {A(1, 1, "aaa"), A(2, 2, "bbb"), A(3, 3, "ccc"), A(4, 3, "ddd")};

		yggr_test_assert(cont_safe == cont_safe_chk);
	}
#endif // BOOST_NO_CXX11_HDR_INITIALIZER_LIST

}

template<typename Container, typename Isomeric_Same_Container>
void test_compare_of_equal_to_and_not_equal_to(void)
{
	typedef Container safe_cont_type;
	typedef Isomeric_Same_Container isomeric_same_cont_type;
	//typedef typename safe_cont_type::base_type unsafe_cont_type;

	A arr1[10] = {A(1, 1, "aaa"), A(2, 2, "bbb"), A(3, 3, "ccc"), A(4, 3, "ddd")};
	A arr2[10] = {A(6, 6, "aaa"), A(7, 7, "bbb"), A(8, 8, "ccc"), A(9, 8, "ddd")};

	safe_cont_type safe_cont11(arr1 + 0, arr1 + 10);
	isomeric_same_cont_type safe_cont12(arr1 + 0, arr1 + 10);

	safe_cont_type safe_cont21(arr2 + 0, arr2 + 10);
	isomeric_same_cont_type safe_cont22(arr2 + 0, arr2 + 10);

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

}

template<typename Container, typename Isomeric_Same_Container>
void test_compare_of_full(void)
{
	test_compare_of_equal_to_and_not_equal_to<Container, Isomeric_Same_Container>();

	typedef Container safe_cont_type;
	typedef Isomeric_Same_Container isomeric_same_cont_type;

	A arr1[10] = {A(1, 1, "aaa"), A(2, 2, "bbb"), A(3, 3, "ccc"), A(4, 3, "ddd")};
	A arr2[10] = {A(6, 6, "aaa"), A(7, 7, "bbb"), A(8, 8, "ccc"), A(9, 8, "ddd")};

	safe_cont_type safe_cont11(arr1 + 0, arr1 + 10);
	safe_cont_type safe_cont12(arr1 + 0, arr1 + 10);

	safe_cont_type safe_cont21(arr2 + 0, arr2 + 10);
	safe_cont_type safe_cont22(arr2 + 0, arr2 + 10);

	// <
	yggr_test_assert(!(safe_cont11 < safe_cont11));
	yggr_test_assert(!(safe_cont11 < safe_cont12));
	yggr_test_assert(safe_cont11 < safe_cont21);

	yggr_test_assert(!(safe_cont11.template get<0>() < safe_cont11.template get<0>()));
	yggr_test_assert(!(safe_cont11.template get<1>() < safe_cont11.template get<1>()));
	yggr_test_assert(!(safe_cont11.template get<0>() < safe_cont12.template get<0>()));
	yggr_test_assert(!(safe_cont11.template get<1>() < safe_cont12.template get<1>()));
	yggr_test_assert(safe_cont11.template get<0>() < safe_cont21.template get<0>());
	yggr_test_assert(safe_cont11.template get<1>() < safe_cont21.template get<1>());

	// <=
	yggr_test_assert(safe_cont11 <= safe_cont11);
	yggr_test_assert(safe_cont11 <= safe_cont12);
	yggr_test_assert(safe_cont11 <= safe_cont21);

	// >
	yggr_test_assert(!(safe_cont11 > safe_cont11));
	yggr_test_assert(!(safe_cont11 > safe_cont12));
	yggr_test_assert(safe_cont21 > safe_cont11);

	// >=
	yggr_test_assert(safe_cont11 >= safe_cont11);
	yggr_test_assert(safe_cont11 >= safe_cont12);
	yggr_test_assert(safe_cont21 >= safe_cont11);
}

//--------------------hash------------------------------------------

void test_construct_of_hash(void)
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
			>,
			boost::mutex
		> isomeric_same_mi_cont_type;

	test_construct<mi_cont_type, isomeric_same_mi_cont_type>();

	std::cout << "---------------test_construct_of_hash end-------------------------" << std::endl;
}

void test_swap_of_hash(void)
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
			>,
			boost::mutex
		> isomeric_same_mi_cont_type;

	test_swap<mi_cont_type, isomeric_same_mi_cont_type>();
	std::cout << "---------------test_swap_of_hash end-----------------" << std::endl;
}

void test_operator_set_of_hash(void)
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
			>,
			boost::mutex
		> isomeric_same_mi_cont_type;

	test_operator_set<mi_cont_type, isomeric_same_mi_cont_type>();
	std::cout << "---------------test_operator_set_of_hash end-----------------" << std::endl;
}

void test_comparer_of_hash(void)
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
			>,
			boost::mutex
		> isomeric_same_mi_cont_type;

	test_compare_of_equal_to_and_not_equal_to<mi_cont_type, isomeric_same_mi_cont_type>();

	std::cout << "---------------test_comparer_of_hash end-----------------" << std::endl;
}

//------------------------------ordered--------------------------------
void test_construct_of_ordered(void)
{
	typedef
		yggr::safe_container::safe_multi_index_container<
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

	typedef
		yggr::safe_container::safe_multi_index_container<
			A,
			boost::multi_index::indexed_by<
				boost::multi_index::ordered_unique<
					boost::multi_index::tag<A::tag_prm1>,
					BOOST_MULTI_INDEX_MEMBER(A, int, prm1) >,

				boost::multi_index::ordered_non_unique<
					boost::multi_index::tag<A::tag_prm2>,
					BOOST_MULTI_INDEX_MEMBER(A, int, prm2)>
			>,
			boost::mutex
		> isomeric_same_mi_cont_type;

	test_construct<mi_cont_type, isomeric_same_mi_cont_type>();

	std::cout << "---------------test_construct_of_ordered end-------------------------" << std::endl;
}

void test_swap_of_ordered(void)
{
	typedef
		yggr::safe_container::safe_multi_index_container<
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

	typedef
		yggr::safe_container::safe_multi_index_container<
			A,
			boost::multi_index::indexed_by<
				boost::multi_index::ordered_unique<
					boost::multi_index::tag<A::tag_prm1>,
					BOOST_MULTI_INDEX_MEMBER(A, int, prm1) >,

				boost::multi_index::ordered_non_unique<
					boost::multi_index::tag<A::tag_prm2>,
					BOOST_MULTI_INDEX_MEMBER(A, int, prm2)>
			>,
			boost::mutex
		> isomeric_same_mi_cont_type;

	test_swap<mi_cont_type, isomeric_same_mi_cont_type>();
	std::cout << "---------------test_swap_of_ordered end-----------------" << std::endl;
}

void test_operator_set_of_ordered(void)
{
	typedef
		yggr::safe_container::safe_multi_index_container<
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

	typedef
		yggr::safe_container::safe_multi_index_container<
			A,
			boost::multi_index::indexed_by<
				boost::multi_index::ordered_unique<
					boost::multi_index::tag<A::tag_prm1>,
					BOOST_MULTI_INDEX_MEMBER(A, int, prm1) >,

				boost::multi_index::ordered_non_unique<
					boost::multi_index::tag<A::tag_prm2>,
					BOOST_MULTI_INDEX_MEMBER(A, int, prm2)>
			>,
			boost::mutex
		> isomeric_same_mi_cont_type;

	test_operator_set<mi_cont_type, isomeric_same_mi_cont_type>();
	std::cout << "---------------test_operator_set_of_ordered end-----------------" << std::endl;
}

void test_comparer_of_ordered(void)
{
	typedef
		yggr::safe_container::safe_multi_index_container<
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

	typedef
		yggr::safe_container::safe_multi_index_container<
			A,
			boost::multi_index::indexed_by<
				boost::multi_index::ordered_unique<
					boost::multi_index::tag<A::tag_prm1>,
					BOOST_MULTI_INDEX_MEMBER(A, int, prm1) >,

				boost::multi_index::ordered_non_unique<
					boost::multi_index::tag<A::tag_prm2>,
					BOOST_MULTI_INDEX_MEMBER(A, int, prm2)>
			>,
			boost::mutex
		> isomeric_same_mi_cont_type;

	test_compare_of_full<mi_cont_type, isomeric_same_mi_cont_type>();
	std::cout << "---------------test_comparer_of_ordered end-----------------" << std::endl;
}

//------------------------------ranked--------------------------------

#ifdef YGGR_BOOST_MULTI_INDEX_HAS_RANKED_INDEX

void test_construct_of_ranked(void)
{
	typedef
		yggr::safe_container::safe_multi_index_container<
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

	typedef
		yggr::safe_container::safe_multi_index_container<
			A,
			boost::multi_index::indexed_by<
				boost::multi_index::ranked_unique<
					boost::multi_index::tag<A::tag_prm1>,
					BOOST_MULTI_INDEX_MEMBER(A, int, prm1) >,

				boost::multi_index::ranked_non_unique<
					boost::multi_index::tag<A::tag_prm2>,
					BOOST_MULTI_INDEX_MEMBER(A, int, prm2)>
			>,
			boost::mutex
		> isomeric_same_mi_cont_type;

	test_construct<mi_cont_type, isomeric_same_mi_cont_type>();

	std::cout << "---------------test_construct_of_ranked end-------------------------" << std::endl;
}

void test_swap_of_ranked(void)
{
	typedef
		yggr::safe_container::safe_multi_index_container<
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

	typedef
		yggr::safe_container::safe_multi_index_container<
			A,
			boost::multi_index::indexed_by<
				boost::multi_index::ranked_unique<
					boost::multi_index::tag<A::tag_prm1>,
					BOOST_MULTI_INDEX_MEMBER(A, int, prm1) >,

				boost::multi_index::ranked_non_unique<
					boost::multi_index::tag<A::tag_prm2>,
					BOOST_MULTI_INDEX_MEMBER(A, int, prm2)>
			>,
			boost::mutex
		> isomeric_same_mi_cont_type;

	test_swap<mi_cont_type, isomeric_same_mi_cont_type>();

	std::cout << "---------------test_swap_of_ranked end-----------------" << std::endl;
}

void test_operator_set_of_ranked(void)
{
	typedef
		yggr::safe_container::safe_multi_index_container<
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

	typedef
		yggr::safe_container::safe_multi_index_container<
			A,
			boost::multi_index::indexed_by<
				boost::multi_index::ranked_unique<
					boost::multi_index::tag<A::tag_prm1>,
					BOOST_MULTI_INDEX_MEMBER(A, int, prm1) >,

				boost::multi_index::ranked_non_unique<
					boost::multi_index::tag<A::tag_prm2>,
					BOOST_MULTI_INDEX_MEMBER(A, int, prm2)>
			>,
			boost::mutex
		> isomeric_same_mi_cont_type;

	test_operator_set<mi_cont_type, isomeric_same_mi_cont_type>();

	std::cout << "---------------test_operator_set_of_ranked end-----------------" << std::endl;
}

void test_comparer_of_ranked(void)
{
	typedef
		yggr::safe_container::safe_multi_index_container<
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

	typedef
		yggr::safe_container::safe_multi_index_container<
			A,
			boost::multi_index::indexed_by<
				boost::multi_index::ranked_unique<
					boost::multi_index::tag<A::tag_prm1>,
					BOOST_MULTI_INDEX_MEMBER(A, int, prm1) >,

				boost::multi_index::ranked_non_unique<
					boost::multi_index::tag<A::tag_prm2>,
					BOOST_MULTI_INDEX_MEMBER(A, int, prm2)>
			>,
			boost::mutex
		> isomeric_same_mi_cont_type;

	test_compare_of_full<mi_cont_type, isomeric_same_mi_cont_type>();
	std::cout << "---------------test_comparer_of_ranked end-----------------" << std::endl;
}

#endif // YGGR_BOOST_MULTI_INDEX_HAS_RANKED_INDEX

//---------------------sequenced------------------------------------

void test_construct_of_sequenced(void)
{
	typedef
		yggr::safe_container::safe_multi_index_container<
			A,
			boost::multi_index::indexed_by<
				boost::multi_index::sequenced< boost::multi_index::tag<A::tag_prm1> >,
				boost::multi_index::sequenced< boost::multi_index::tag<A::tag_prm2> >
			>
		> mi_cont_type;

	typedef
		yggr::safe_container::safe_multi_index_container<
			A,
			boost::multi_index::indexed_by<
				boost::multi_index::sequenced< boost::multi_index::tag<A::tag_prm1> >,
				boost::multi_index::sequenced< boost::multi_index::tag<A::tag_prm2> >
			>,
			boost::mutex
		> isomeric_same_mi_cont_type;

	test_construct<mi_cont_type, isomeric_same_mi_cont_type>();

	std::cout << "---------------test_construct_of_sequenced end-------------------------" << std::endl;
}

void test_swap_of_sequenced(void)
{
	typedef
		yggr::safe_container::safe_multi_index_container<
			A,
			boost::multi_index::indexed_by<
				boost::multi_index::sequenced< boost::multi_index::tag<A::tag_prm1> >,
				boost::multi_index::sequenced< boost::multi_index::tag<A::tag_prm2> >
			>
		> mi_cont_type;

	typedef
		yggr::safe_container::safe_multi_index_container<
			A,
			boost::multi_index::indexed_by<
				boost::multi_index::sequenced< boost::multi_index::tag<A::tag_prm1> >,
				boost::multi_index::sequenced< boost::multi_index::tag<A::tag_prm2> >
			>,
			boost::mutex
		> isomeric_same_mi_cont_type;

	test_swap<mi_cont_type, isomeric_same_mi_cont_type>();
	std::cout << "---------------test_swap_of_sequenced end-----------------" << std::endl;
}

void test_operator_set_of_sequenced(void)
{
	typedef
		yggr::safe_container::safe_multi_index_container<
			A,
			boost::multi_index::indexed_by<
				boost::multi_index::sequenced< boost::multi_index::tag<A::tag_prm1> >,
				boost::multi_index::sequenced< boost::multi_index::tag<A::tag_prm2> >
			>
		> mi_cont_type;

	typedef
		yggr::safe_container::safe_multi_index_container<
			A,
			boost::multi_index::indexed_by<
				boost::multi_index::sequenced< boost::multi_index::tag<A::tag_prm1> >,
				boost::multi_index::sequenced< boost::multi_index::tag<A::tag_prm2> >
			>,
			boost::mutex
		> isomeric_same_mi_cont_type;

	test_operator_set<mi_cont_type, isomeric_same_mi_cont_type>();

	std::cout << "---------------test_operator_set_of_sequenced end-----------------" << std::endl;
}

void test_comparer_of_sequenced(void)
{
	typedef
		yggr::safe_container::safe_multi_index_container<
			A,
			boost::multi_index::indexed_by<
				boost::multi_index::sequenced< boost::multi_index::tag<A::tag_prm1> >,
				boost::multi_index::sequenced< boost::multi_index::tag<A::tag_prm2> >
			>
		> mi_cont_type;

	typedef
		yggr::safe_container::safe_multi_index_container<
			A,
			boost::multi_index::indexed_by<
				boost::multi_index::sequenced< boost::multi_index::tag<A::tag_prm1> >,
				boost::multi_index::sequenced< boost::multi_index::tag<A::tag_prm2> >
			>,
			boost::mutex
		> isomeric_same_mi_cont_type;

	test_compare_of_full<mi_cont_type, isomeric_same_mi_cont_type>();
	std::cout << "---------------test_comparer_of_sequenced end-----------------" << std::endl;
}

// -------------------------------random access----------------------------------

void test_construct_of_random_access(void)
{
	typedef
		yggr::safe_container::safe_multi_index_container<
			A,
			boost::multi_index::indexed_by<
				boost::multi_index::random_access< boost::multi_index::tag<A::tag_prm1> >,
				boost::multi_index::random_access< boost::multi_index::tag<A::tag_prm2> >
			>
		> mi_cont_type;

	typedef
		yggr::safe_container::safe_multi_index_container<
			A,
			boost::multi_index::indexed_by<
				boost::multi_index::random_access< boost::multi_index::tag<A::tag_prm1> >,
				boost::multi_index::random_access< boost::multi_index::tag<A::tag_prm2> >
			>,
			boost::mutex
		> isomeric_same_mi_cont_type;

	test_construct<mi_cont_type, isomeric_same_mi_cont_type>();

	std::cout << "---------------test_construct_of_random_access end-------------------------" << std::endl;
}

void test_swap_of_random_access(void)
{
	typedef
		yggr::safe_container::safe_multi_index_container<
			A,
			boost::multi_index::indexed_by<
				boost::multi_index::random_access< boost::multi_index::tag<A::tag_prm1> >,
				boost::multi_index::random_access< boost::multi_index::tag<A::tag_prm2> >
			>
		> mi_cont_type;

	typedef
		yggr::safe_container::safe_multi_index_container<
			A,
			boost::multi_index::indexed_by<
				boost::multi_index::random_access< boost::multi_index::tag<A::tag_prm1> >,
				boost::multi_index::random_access< boost::multi_index::tag<A::tag_prm2> >
			>,
			boost::mutex
		> isomeric_same_mi_cont_type;

	test_swap<mi_cont_type, isomeric_same_mi_cont_type>();

	std::cout << "---------------test_swap_of_random_access end-----------------" << std::endl;
}

void test_operator_set_of_random_access(void)
{
	typedef
		yggr::safe_container::safe_multi_index_container<
			A,
			boost::multi_index::indexed_by<
				boost::multi_index::random_access< boost::multi_index::tag<A::tag_prm1> >,
				boost::multi_index::random_access< boost::multi_index::tag<A::tag_prm2> >
			>
		> mi_cont_type;

	typedef
		yggr::safe_container::safe_multi_index_container<
			A,
			boost::multi_index::indexed_by<
				boost::multi_index::random_access< boost::multi_index::tag<A::tag_prm1> >,
				boost::multi_index::random_access< boost::multi_index::tag<A::tag_prm2> >
			>,
			boost::mutex
		> isomeric_same_mi_cont_type;

	test_operator_set<mi_cont_type, isomeric_same_mi_cont_type>();

	std::cout << "---------------test_operator_set_of_random_access end-----------------" << std::endl;
}

void test_comparer_of_random_access(void)
{
	typedef
		yggr::safe_container::safe_multi_index_container<
			A,
			boost::multi_index::indexed_by<
				boost::multi_index::random_access< boost::multi_index::tag<A::tag_prm1> >,
				boost::multi_index::random_access< boost::multi_index::tag<A::tag_prm2> >
			>
		> mi_cont_type;

	typedef
		yggr::safe_container::safe_multi_index_container<
			A,
			boost::multi_index::indexed_by<
				boost::multi_index::random_access< boost::multi_index::tag<A::tag_prm1> >,
				boost::multi_index::random_access< boost::multi_index::tag<A::tag_prm2> >
			>,
			boost::mutex
		> isomeric_same_mi_cont_type;

	test_compare_of_full<mi_cont_type, isomeric_same_mi_cont_type>();

	std::cout << "---------------test_comparer_of_random_access end-----------------" << std::endl;
}

void debug_test(void)
{
	test_construct_of_hash();
	test_swap_of_hash();
	test_operator_set_of_hash();
	test_comparer_of_hash();

	std::cout << "///////////////////////////////////////" << std::endl;

	test_construct_of_ordered();
	test_swap_of_ordered();
	test_operator_set_of_ordered();
	test_comparer_of_ordered();

	std::cout << "////////////////////////////////////" << std::endl;

#ifdef YGGR_BOOST_MULTI_INDEX_HAS_RANKED_INDEX

	test_construct_of_ranked();
	test_swap_of_ranked();
	test_operator_set_of_ranked();
	test_comparer_of_ranked();

#else

	std::cout << "!!!test randed_index need boost >= 1.59!!!" << std::endl;

#endif // YGGR_BOOST_MULTI_INDEX_HAS_RANKED_INDEX

	std::cout << "////////////////////////////////////" << std::endl;

	test_construct_of_sequenced();
	test_swap_of_sequenced();
	test_operator_set_of_sequenced();
	test_comparer_of_sequenced();

	std::cout << "////////////////////////////////////" << std::endl;

	test_construct_of_random_access();
	test_swap_of_random_access();
	test_operator_set_of_random_access();
	test_comparer_of_random_access();

	std::cout << "////////////////////////////////////" << std::endl;

	std::cout << "all test end" << std::endl;
}

int main(int argc, char* argv[])
{
	debug_test();

	wait_any_key(argc, argv);
	return 0;
}
