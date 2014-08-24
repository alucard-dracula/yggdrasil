//safe_multi_index_container_base_test.cpp

#if defined(_MSC_VER) && defined(_DEBUG)
#	include <vld.h>
#endif // _MSC_VER && _DEBUG
#include <iostream>
#include <string>
#include <yggr/safe_container/safe_multi_index_container.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/bind.hpp>

#include <assert.h>

struct A
{
	struct tag_prm1{};
	struct tag_prm2{};
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

	int prm1;
	int prm2;
	std::string prm3;
};



#ifdef _DEBUG
template<typename Container>
void test_construct(void)
{
	typedef Container mi_cont_type;

	mi_cont_type cont1;
	assert(cont1.empty());

	mi_cont_type cont2(cont1.get_allocator());
	assert(cont2.empty());

	A arr[10] = {A(1, 1, "aaa"), A(2, 2, "bbb"), A(3, 3, "ccc"), A(4, 3, "ddd")};

	mi_cont_type cont3(arr, arr + 10);
	typename mi_cont_type::base_type base_cont3(arr, arr + 10);
	assert(cont3.size() == base_cont3.size());

	mi_cont_type cont4(arr, arr + 10, cont1.get_allocator());
	typename mi_cont_type::base_type base_cont4(arr, arr + 10);
	assert(cont4.size() == base_cont4.size());

	mi_cont_type cont5(arr, arr + 10, typename mi_cont_type::ctor_args_list(), cont1.get_allocator());
	typename mi_cont_type::base_type base_cont5(arr, arr + 10);
	assert(cont4.size() == base_cont5.size());
}

template<typename Container>
void test_swap(void)
{
	typedef Container mi_cont_type;

	A arr1[10] = {A(1, 1, "aaa"), A(2, 2, "bbb"), A(3, 3, "ccc"), A(4, 3, "ddd")};
	A arr2[10] = {A(6, 6, "aaa"), A(7, 7, "bbb"), A(8, 8, "ccc"), A(9, 8, "ddd")};

	mi_cont_type cont1(arr1, arr1 + 10);
	mi_cont_type cont2(arr2, arr2 + 5);

	typename mi_cont_type::base_type base_cont1(arr1, arr1 + 10);
	typename mi_cont_type::base_type base_cont2(arr2, arr2 + 5);
	assert(cont1.size() == base_cont1.size());

	typename mi_cont_type::base_type base_cont3(base_cont1);
	typename mi_cont_type::base_type base_cont4(base_cont2);

	// ------------------ boost::swap test------------------------
	// base_type&& this_type&
	boost::swap(boost::move(base_cont4), cont1);
	assert(base_cont4.size() == base_cont1.size());

	// base_type& this_type&
	boost::swap(base_cont3, cont1);
	assert(base_cont3.size() == base_cont2.size());

	// this_type& base_type&&
	boost::swap(cont1, boost::move(base_cont3));
	assert(base_cont3.size() == base_cont1.size());

	// this_type& base_type&
	boost::swap(cont1, boost::move(base_cont4));
	assert(base_cont4.size() == base_cont2.size());

	// this_type& this_type&
	boost::swap(cont1, cont2);
	boost::swap(cont1, cont2);

	// ------------------ std::swap test------------------------
	// base_type&& this_type&
	std::swap(boost::move(base_cont4), cont1);
	assert(base_cont4.size() == base_cont1.size());

	// base_type& this_type&
	std::swap(base_cont3, cont1);
	assert(base_cont3.size() == base_cont2.size());

	// this_type& base_type&&
	std::swap(cont1, boost::move(base_cont3));
	assert(base_cont3.size() == base_cont1.size());

	// this_type& base_type&
	std::swap(cont1, boost::move(base_cont4));
	assert(base_cont4.size() == base_cont2.size());

	// this_type& this_type&
	std::swap(cont1, cont2);

	std::swap(cont1, cont2);

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
					boost::multi_index::tag<A::tag_prm1>,
					BOOST_MULTI_INDEX_MEMBER(A, int, prm1) >,

				boost::multi_index::hashed_non_unique<
					boost::multi_index::tag<A::tag_prm2>,
					BOOST_MULTI_INDEX_MEMBER(A, int, prm2)>
			>
		> mi_cont_type;

	A arr[10] = {A(1, 1, "aaa"), A(2, 2, "bbb"), A(3, 3, "ccc"), A(4, 3, "ddd")};
	mi_cont_type cont(arr, arr + 10);
	mi_cont_type cont2(cont.get_allocator());

	cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).get_allocator());
	cont.use_method<1>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 1).get_allocator());
	cont.use_method<A::tag_prm1>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, A::tag_prm1).get_allocator());
	cont.use_method<A::tag_prm2>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, A::tag_prm2).get_allocator());

	cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).key_extractor());
	cont.use_method<1>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 1).key_extractor());
	cont.use_method<A::tag_prm1>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, A::tag_prm1).key_extractor());
	cont.use_method<A::tag_prm2>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, A::tag_prm2).key_extractor());

	cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).hash_function());
	cont.use_method<1>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 1).hash_function());
	cont.use_method<A::tag_prm1>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, A::tag_prm1).hash_function());
	cont.use_method<A::tag_prm2>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, A::tag_prm2).hash_function());

	cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).key_eq());
	cont.use_method<1>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 1).key_eq());
	cont.use_method<A::tag_prm1>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, A::tag_prm1).key_eq());
	cont.use_method<A::tag_prm2>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, A::tag_prm2).key_eq());


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
	mi_cont_type cont(arr, arr + 10);
	mi_cont_type::base_type base_cont(arr, arr + 10);
	assert(cont.size() == 5);
	assert(!cont.empty());
	assert(base_cont.size() == cont.size());
	assert(base_cont.max_size() == cont.max_size());

	assert(cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).empty())
			== cont.use_method<A::tag_prm1>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, A::tag_prm1).empty()));
	assert(cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).empty())
				== cont.use_method<A::tag_prm2>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, A::tag_prm2).empty()));

	assert(cont.use_method<1>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 1).empty())
			== cont.use_method<A::tag_prm2>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, A::tag_prm2).empty()));

	assert(cont.use_method<1>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 1).empty())
			== base_cont.empty());

	assert(cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).size())
			== cont.use_method<A::tag_prm1>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, A::tag_prm1).size()));

	assert(cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).size())
			== base_cont.size());

	assert(cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).max_size())
			== base_cont.max_size());

	assert(cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).max_size())
			== base_cont.get<0>().max_size());

	assert(cont.use_method<A::tag_prm2>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, A::tag_prm2).max_size())
			== base_cont.get<1>().max_size());

	std::cout << "---------------test_capacity_of_hash end-------------------------" << std::endl;
}

void test_modifiers_of_hash(void)
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
	mi_cont_type cont(arr, arr + 10);

	assert(cont.size() == 5);

	// insert
	assert(cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, A::tag_prm1).insert(A(5, 3, "eee"))));
	assert(cont.size() == 6);
	assert(!cont.use_method<A::tag_prm1>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).insert(A(3, 5, "cccc"))));
	assert(cont.size() == 6);

	A arr2[10] = {A(10, 10, "a10"), A(20, 20, "b20"), A(30, 30, "c30"), A(40, 30, "d40")};
	assert(cont.use_method<A::tag_prm1>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).insert(arr2, arr2 + 10)));
	assert(cont.size() == 10);
	assert(!cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, A::tag_prm1).insert(arr, arr + 10)));
	assert(cont.size() == 10);


	assert(cont.use_method<1>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, A::tag_prm2).insert(A(6, 3, "eee"))));
	assert(cont.size() == 11);
	assert(!cont.use_method<A::tag_prm2>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 1).insert(A(6, 3, "cccc"))));
	assert(cont.size() == 11);

	A arr3[10] = {A(11, 11, "a11"), A(21, 21, "b21"), A(31, 31, "c31"), A(41, 31, "d41")};
	assert(cont.use_method<A::tag_prm2>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 1).insert(arr3, arr3 + 10)));
	assert(cont.size() == 15);
	assert(!cont.use_method<1>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, A::tag_prm2).insert(arr3, arr3 + 10)));
	assert(cont.size() == 15);

	// erase
	assert(cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, A::tag_prm1).insert(A(100, 100, "100"))));
	assert(cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, A::tag_prm1).insert(A(200, 100, "100"))));
	assert(cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, A::tag_prm1).insert(A(300, 100, "100"))));
	assert(cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, A::tag_prm1).insert(A(400, 100, "100"))));
	assert(cont.size() == 19);

	assert(1 == cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).erase(100)));
	assert(3 == cont.use_method<A::tag_prm2>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, A::tag_prm2).erase(100)));
	assert(cont.size() == 15);

	// replace
	assert(cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, A::tag_prm1).replace(0, A(0, 100, "0"))));
	assert(cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, A::tag_prm1).replace(0, A(100, 100, "0"))));
	assert(cont.use_method<A::tag_prm1>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, A::tag_prm1).replace(100, A(0, 100, "0"))));
	assert(!cont.use_method<A::tag_prm1>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, A::tag_prm1).replace(0, A(1, 100, "0"))));

	assert(cont.size() == 15);

	assert(cont.use_method<1>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, A::tag_prm2).replace(100, A(100, 100, "0"))));
	assert(cont.use_method<A::tag_prm2>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, A::tag_prm2).replace(100, A(0, 1, "0"))));
	assert(cont.use_method<A::tag_prm2>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, A::tag_prm2).replace(1, A(100, 1, "0"))));
	assert(!cont.use_method<A::tag_prm2>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, A::tag_prm2).replace(1, A(2, 1, "0"))));

	assert(cont.size() == 15);

	// modify

	assert(cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, A::tag_prm1).modify(2, boost::bind(&modifier<A, A>, _1, A(2, 100, "100")))));
	assert(cont.size() == 15);
	A tmp1;
	assert(!cont.use_method<A::tag_prm1>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, A::tag_prm1).modify(2,
																							boost::bind(&modifier<A, A>, _1, A(1, 100, "100"), boost::ref(tmp1)),
																							boost::bind(&rollback<A>, _1, boost::cref(tmp1)))));
	assert(cont.size() == 15);

	// modify key
	assert(cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, A::tag_prm1).modify_key(2, boost::bind(&modifier<int, int>, _1, 200))));
	assert(cont.size() == 15);
	int tmp2;
	assert(!cont.use_method<A::tag_prm1>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, A::tag_prm1).modify_key(200,
																							boost::bind(&modifier<int, int>, _1, 3, boost::ref(tmp2)),
																							boost::bind(&rollback<int>, _1, boost::cref(tmp2)))));
	assert(cont.size() == 15);

	mi_cont_type::base_type base_cont(arr, arr + 10);
	assert(cont.size() == 15);
	assert(base_cont.size() == 5);

	cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, A::tag_prm1).swap(base_cont.get<0>()));
	assert(cont.size() == 5);
	assert(base_cont.size() == 15);

	cont.use_method<A::tag_prm1>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).swap(base_cont.get<A::tag_prm1>()));
	assert(cont.size() == 15);
	assert(base_cont.size() == 5);

	cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).clear());
	assert(cont.empty());

	cont.clear();
	assert(cont.empty());

	std::cout << "---------------test_modifiers_of_hash end-------------------------" << std::endl;
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
	mi_cont_type cont(arr, arr + 10);

	assert(1 == cont.use_method<A::tag_prm1>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, A::tag_prm1).count(1)));
	assert(1 == cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).count(2)));
	assert(0 == cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).count(100)));

	assert(1 == cont.use_method<A::tag_prm2>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, A::tag_prm2).count(2)));
	assert(2 == cont.use_method<1>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 1).count(3)));
	assert(0 == cont.use_method<1>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 1).count(100)));

	assert(cont.size() == 5);

	mi_cont_type::base_type base_cont(arr, arr + 10);
	assert(cont.size() == 5);

	assert(1 == cont.use_method<A::tag_prm1>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, A::tag_prm1)
						.count(1,
								cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).hash_function()),
								std::equal_to<int>())));

	assert(1 == cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0)
						.count(2,
								cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).hash_function()),
								std::equal_to<int>())));

	assert(0 == cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0)
									.count(100,
											cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).hash_function()),
											std::equal_to<int>())));

	assert(1 == cont.use_method<A::tag_prm2>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, A::tag_prm2)
												.count(2,
														cont.use_method<1>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 1).hash_function()),
														std::equal_to<int>())));

	assert(2 == cont.use_method<1>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 1)
												.count(3,
														cont.use_method<1>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 1).hash_function()),
														std::equal_to<int>())));

	assert(0 == cont.use_method<1>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 1)
												.count(100,
														 cont.use_method<1>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 1).hash_function()),
														 std::equal_to<int>())));


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

	mi_cont_type cont(arr, arr + 10);
	mi_cont_type::base_type base_cont(arr, arr + 10);


	assert(cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).bucket_count()) == base_cont.get<0>().bucket_count());
	assert(cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).max_bucket_count()) == base_cont.get<0>().max_bucket_count());
	assert(cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).bucket_size(0)) == base_cont.get<0>().bucket_size(0));
	assert(cont.use_method<1>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 1).bucket_size(1)) == base_cont.get<1>().bucket_size(1));

	assert(cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).bucket(0)) == base_cont.get<0>().bucket(0));
	assert(cont.use_method<1>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 1).bucket(1)) == base_cont.get<1>().bucket(1));


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

	mi_cont_type cont(arr, arr + 10);
	mi_cont_type::base_type base_cont(arr, arr + 10);

	assert(cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).load_factor()) == base_cont.get<0>().load_factor());
	assert(cont.use_method<1>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 1).load_factor()) == base_cont.get<1>().load_factor());

	assert(cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).max_load_factor()) == base_cont.get<0>().max_load_factor());
	assert(cont.use_method<1>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 1).max_load_factor()) == base_cont.get<1>().max_load_factor());

	cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0)
						.max_load_factor(cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0)
							.max_load_factor())));

	cont.use_method<1>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 1).max_load_factor(base_cont.get<1>().max_load_factor()));

	assert(cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).max_load_factor()) == base_cont.get<0>().max_load_factor());
	assert(cont.use_method<1>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 1).max_load_factor()) == base_cont.get<1>().max_load_factor());

    cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).rehash(100));
	base_cont.get<0>().rehash(100);

	assert(cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).max_load_factor()) == base_cont.get<0>().max_load_factor());



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
	mi_cont_type cont(arr, arr + 10);
	mi_cont_type cont2(cont.get_allocator());

	cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).get_allocator());
	cont.use_method<1>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 1).get_allocator());
	cont.use_method<A::tag_prm1>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, A::tag_prm1).get_allocator());
	cont.use_method<A::tag_prm2>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, A::tag_prm2).get_allocator());

	cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).key_extractor());
	cont.use_method<1>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 1).key_extractor());
	cont.use_method<A::tag_prm1>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, A::tag_prm1).key_extractor());
	cont.use_method<A::tag_prm2>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, A::tag_prm2).key_extractor());

	cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).key_comp());
	cont.use_method<1>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 1).key_comp());
	cont.use_method<A::tag_prm1>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, A::tag_prm1).key_comp());
	cont.use_method<A::tag_prm2>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, A::tag_prm2).key_comp());

	cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).value_comp());
	cont.use_method<1>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 1).value_comp());
	cont.use_method<A::tag_prm1>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, A::tag_prm1).value_comp());
	cont.use_method<A::tag_prm2>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, A::tag_prm2).value_comp());


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
	mi_cont_type cont(arr, arr + 10);
	mi_cont_type::base_type base_cont(arr, arr + 10);
	assert(cont.size() == 5);
	assert(!cont.empty());
	assert(base_cont.size() == cont.size());
	assert(base_cont.max_size() == cont.max_size());

	assert(cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).empty())
			== cont.use_method<A::tag_prm1>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, A::tag_prm1).empty()));
	assert(cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).empty())
				== cont.use_method<A::tag_prm2>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, A::tag_prm2).empty()));

	assert(cont.use_method<1>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 1).empty())
			== cont.use_method<A::tag_prm2>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, A::tag_prm2).empty()));

	assert(cont.use_method<1>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 1).empty())
			== base_cont.empty());

	assert(cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).size())
			== cont.use_method<A::tag_prm1>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, A::tag_prm1).size()));

	assert(cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).size())
			== base_cont.size());

	assert(cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).max_size())
			== base_cont.max_size());

	assert(cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).max_size())
			== base_cont.get<0>().max_size());

	assert(cont.use_method<A::tag_prm2>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, A::tag_prm2).max_size())
			== base_cont.get<1>().max_size());

	std::cout << "---------------test_capacity_of_ordered end-------------------------" << std::endl;
}

void test_modifiers_of_ordered(void)
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
	mi_cont_type cont(arr, arr + 10);

	assert(cont.size() == 5);

	// insert
	assert(cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, A::tag_prm1).insert(A(5, 3, "eee"))));
	assert(cont.size() == 6);
	assert(!cont.use_method<A::tag_prm1>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).insert(A(3, 5, "cccc"))));
	assert(cont.size() == 6);

	A arr2[10] = {A(10, 10, "a10"), A(20, 20, "b20"), A(30, 30, "c30"), A(40, 30, "d40")};
	assert(cont.use_method<A::tag_prm1>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).insert(arr2, arr2 + 10)));
	assert(cont.size() == 10);
	assert(!cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, A::tag_prm1).insert(arr, arr + 10)));
	assert(cont.size() == 10);


	assert(cont.use_method<1>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, A::tag_prm2).insert(A(6, 3, "eee"))));
	assert(cont.size() == 11);
	assert(!cont.use_method<A::tag_prm2>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 1).insert(A(6, 3, "cccc"))));
	assert(cont.size() == 11);

	A arr3[10] = {A(11, 11, "a11"), A(21, 21, "b21"), A(31, 31, "c31"), A(41, 31, "d41")};
	assert(cont.use_method<A::tag_prm2>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 1).insert(arr3, arr3 + 10)));
	assert(cont.size() == 15);
	assert(!cont.use_method<1>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, A::tag_prm2).insert(arr3, arr3 + 10)));
	assert(cont.size() == 15);

	// erase
	assert(cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, A::tag_prm1).insert(A(100, 100, "100"))));
	assert(cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, A::tag_prm1).insert(A(200, 100, "100"))));
	assert(cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, A::tag_prm1).insert(A(300, 100, "100"))));
	assert(cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, A::tag_prm1).insert(A(400, 100, "100"))));
	assert(cont.size() == 19);

	assert(1 == cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).erase(100)));
	assert(3 == cont.use_method<A::tag_prm2>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, A::tag_prm2).erase(100)));
	assert(cont.size() == 15);

	// replace
	assert(cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, A::tag_prm1).replace(0, A(0, 100, "0"))));
	assert(cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, A::tag_prm1).replace(0, A(100, 100, "0"))));
	assert(cont.use_method<A::tag_prm1>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, A::tag_prm1).replace(100, A(0, 100, "0"))));
	assert(!cont.use_method<A::tag_prm1>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, A::tag_prm1).replace(0, A(1, 100, "0"))));

	assert(cont.size() == 15);

	assert(cont.use_method<1>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, A::tag_prm2).replace(100, A(100, 100, "0"))));
	assert(cont.use_method<A::tag_prm2>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, A::tag_prm2).replace(100, A(0, 1, "0"))));
	assert(cont.use_method<A::tag_prm2>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, A::tag_prm2).replace(1, A(100, 1, "0"))));
	assert(!cont.use_method<A::tag_prm2>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, A::tag_prm2).replace(1, A(2, 1, "0"))));

	assert(cont.size() == 15);

	// modify

	assert(cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, A::tag_prm1).modify(2, boost::bind(&modifier<A, A>, _1, A(1, 100, "100")))));
	assert(cont.size() == 15);
	A tmp1;
	assert(!cont.use_method<A::tag_prm1>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, A::tag_prm1).modify(2,
																							boost::bind(&modifier<A, A>, _1, A(1, 100, "100"), boost::ref(tmp1)),
																							boost::bind(&rollback<A>, _1, boost::cref(tmp1)))));
	assert(cont.size() == 15);

	// modify key
	assert(cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, A::tag_prm1).modify_key(1, boost::bind(&modifier<int, int>, _1, 200))));
	assert(cont.size() == 15);
	int tmp2;
	assert(!cont.use_method<A::tag_prm1>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, A::tag_prm1).modify_key(200,
																							boost::bind(&modifier<int, int>, _1, 3, boost::ref(tmp2)),
																							boost::bind(&rollback<int>, _1, boost::cref(tmp2)))));
	assert(cont.size() == 15);

	mi_cont_type::base_type base_cont(arr, arr + 10);
	assert(cont.size() == 15);
	assert(base_cont.size() == 5);

	cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, A::tag_prm1).swap(base_cont.get<0>()));
	assert(cont.size() == 5);
	assert(base_cont.size() == 15);

	cont.use_method<A::tag_prm1>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).swap(base_cont.get<A::tag_prm1>()));
	assert(cont.size() == 15);
	assert(base_cont.size() == 5);

	cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).clear());
	assert(cont.empty());

	cont.clear();
	assert(cont.empty());

	std::cout << "---------------test_modifiers_of_ordered end-------------------------" << std::endl;
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
	mi_cont_type cont(arr, arr + 10);

	assert(1 == cont.use_method<A::tag_prm1>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, A::tag_prm1).count(1)));
	assert(1 == cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).count(2)));
	assert(0 == cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).count(100)));

	assert(1 == cont.use_method<A::tag_prm2>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, A::tag_prm2).count(2)));
	assert(2 == cont.use_method<1>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 1).count(3)));
	assert(0 == cont.use_method<1>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 1).count(100)));

	assert(cont.size() == 5);

	mi_cont_type::base_type base_cont(arr, arr + 10);
	assert(cont.size() == 5);

	assert(1 == cont.use_method<A::tag_prm1>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, A::tag_prm1)
						.count(1, std::less<int>())));

	assert(1 == cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0)
						.count(2, std::less<int>())));

	assert(0 == cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0)
									.count(100, std::less<int>())));

	assert(1 == cont.use_method<A::tag_prm2>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, A::tag_prm2)
												.count(2, std::less<int>())));

	assert(2 == cont.use_method<1>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 1)
												.count(3, std::less<int>())));

	assert(0 == cont.use_method<1>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 1)
												.count(100, std::less<int>())));


	std::cout << "---------------test_set_operations_of_hash end-------------------" << std::endl;
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
	mi_cont_type cont(arr, arr + 10);
	mi_cont_type cont2(cont.get_allocator());

	cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).get_allocator());
	cont.use_method<1>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 1).get_allocator());
	cont.use_method<A::tag_prm1>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, A::tag_prm1).get_allocator());
	cont.use_method<A::tag_prm2>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, A::tag_prm2).get_allocator());

	std::cout << "---------------test_observers_of_sequenced end-------------------------" << std::endl;
}

void test_capacity_of_sequecned(void)
{
	typedef yggr::safe_container::safe_multi_index_container<
			A,
			boost::multi_index::indexed_by<
				boost::multi_index::sequenced< boost::multi_index::tag<A::tag_prm1> >,
				boost::multi_index::sequenced< boost::multi_index::tag<A::tag_prm2> >
			>
		> mi_cont_type;

	A arr[10] = {A(1, 1, "aaa"), A(2, 2, "bbb"), A(3, 3, "ccc"), A(4, 3, "ddd")};
	mi_cont_type cont(arr, arr + 10);
	mi_cont_type::base_type base_cont(arr, arr + 10);
	assert(cont.size() == 10);
	assert(!cont.empty());
	assert(base_cont.size() == cont.size());
	assert(base_cont.max_size() == cont.max_size());

	assert(cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).empty())
			== cont.use_method<A::tag_prm1>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, A::tag_prm1).empty()));
	assert(cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).empty())
				== cont.use_method<A::tag_prm2>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, A::tag_prm2).empty()));

	assert(cont.use_method<1>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 1).empty())
			== cont.use_method<A::tag_prm2>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, A::tag_prm2).empty()));

	assert(cont.use_method<1>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 1).empty())
			== base_cont.empty());

	assert(cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).size())
			== cont.use_method<A::tag_prm1>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, A::tag_prm1).size()));

	assert(cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).size())
			== base_cont.size());

	assert(cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).max_size())
			== base_cont.max_size());

	assert(cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).max_size())
			== base_cont.get<0>().max_size());

	assert(cont.use_method<A::tag_prm2>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, A::tag_prm2).max_size())
			== base_cont.get<1>().max_size());

	cont.use_method(YGGR_PP_MIDX_MAKE_METHOD_NONTAG(mi_cont_type).resize(100));
	assert(cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).size())
			== cont.use_method<1>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 1).size()));

	cont.use_method<1>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 1).resize(100, A(700, 700, "700")));
	assert(cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).size())
			== cont.use_method<1>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 1).size()));

	std::cout << "---------------test_capacity_of_sequecned end-------------------------" << std::endl;
}

void test_modifiers_of_sequecned(void)
{
	typedef yggr::safe_container::safe_multi_index_container<
			A,
			boost::multi_index::indexed_by<
				boost::multi_index::sequenced< boost::multi_index::tag<A::tag_prm1> >,
				boost::multi_index::sequenced< boost::multi_index::tag<A::tag_prm2> >
			>
		> mi_cont_type;

	mi_cont_type cont2;
	A tmp;
	assert(cont2.empty());
	assert(!cont2.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).front(tmp)));
	assert(!cont2.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).back(tmp)));

	assert(!cont2.use_method<A::tag_prm2>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, A::tag_prm2).front(tmp)));
	assert(!cont2.use_method<A::tag_prm2>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, A::tag_prm2).back(tmp)));

	assert(!cont2.use_method<A::tag_prm2>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, A::tag_prm2).pop_front(tmp)));
	assert(!cont2.use_method<A::tag_prm2>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, A::tag_prm2).pop_back(tmp)));
	assert(!cont2.use_method<A::tag_prm2>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, A::tag_prm2).pop_front()));
	assert(!cont2.use_method<A::tag_prm2>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, A::tag_prm2).pop_back()));

	A arr[10] = {A(1, 1, "aaa"), A(2, 2, "bbb"), A(3, 3, "ccc"), A(4, 3, "ddd")};
	mi_cont_type cont(arr, arr + 10);
	assert(cont.size() == 10);

	cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).push_back(A(10, 10, "10")));
	A back;
	assert(cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).back(back)));
	assert(back.prm1 == 10 && back.prm2 == 10);

	assert(cont.size() == 11);

	cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).push_front(A(20, 20, "20")));
	A front;
	assert(cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).front(front)));
	assert(front.prm1 == 20 && front.prm2 == 20);
	assert(cont.size() == 12);

	cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).push_back(A(40, 40, "40")));
	cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).push_front(A(60, 60, "60")));

	assert(cont.size() == 14);
	A back2;
	A front2;

	assert(cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).pop_front(front2)));
	assert(cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).pop_back(back2)));

	assert(back2.prm1 == 40);
	assert(front2.prm1 == 60);
	assert(cont.size() == 12);

	assert(cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).pop_front()));
	assert(cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).pop_back()));

	assert(cont.size() == 10);

	//cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).remove(A(1, 100, "aaa"))); // warning:right
	cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).remove(A(1, 1, "aaa")));

	assert(cont.size() == 9);

	A rm_cmp(3, 0, "");
	cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).remove_if(boost::bind(&less_foo, _1, boost::cref(rm_cmp))));
	assert(cont.size() == 2);

	mi_cont_type::base_type base_cont(arr, arr + 10);

	cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).push_back(A(10, 10, "10")));
	cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).push_back(A(10, 10, "10")));
	cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).push_back(A(10, 10, "10")));
	cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).push_back(A(10, 10, "10")));

	cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).sort());

	assert(cont.size() == 6);

	cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).unique());

	assert(cont.size() == 3);

	cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).push_back(A(10, 10, "10")));
	cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).push_back(A(10, 10, "10")));
	cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).push_back(A(10, 10, "10")));

	assert(cont.size() == 6);

	cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).sort());
	cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).unique(boost::bind(&less_foo, _1, _2)));
	assert(cont.size() == 6);
	cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).reverse());

	cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).unique(boost::bind(&less_foo, _1, _2)));

	mi_cont_type::base_type base_cont2(arr, arr + 10);
	cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).merge(base_cont2));

	assert(base_cont2.size() == 0);

	assert(cont.size() == 14);
	assert(base_cont.size() == 10);

	cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, A::tag_prm1).swap(base_cont.get<0>()));
	assert(cont.size() == 10);
	assert(base_cont.size() == 14);

	cont.use_method<A::tag_prm1>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).swap(base_cont.get<A::tag_prm1>()));
	assert(cont.size() == 14);
	assert(base_cont.size() == 10);

	cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).clear());
	assert(cont.empty());

	cont.clear();
	assert(cont.empty());

	std::cout << "---------------test_modifiers_of_sequecned end-------------------------" << std::endl;
}

void test_swap_of_sequecned(void)
{
	typedef yggr::safe_container::safe_multi_index_container<
			A,
			boost::multi_index::indexed_by<
				boost::multi_index::sequenced< boost::multi_index::tag<A::tag_prm1> >,
				boost::multi_index::sequenced< boost::multi_index::tag<A::tag_prm2> >
			>
		> mi_cont_type;
	test_swap<mi_cont_type>();
	std::cout << "---------------test_swap_of_sequecned end-----------------" << std::endl;
}

// -------------------------------random access----------------------------------

void test_construct_of_random_access(void)
{
	typedef yggr::safe_container::safe_multi_index_container<
			A,
			boost::multi_index::indexed_by<
				boost::multi_index::random_access< boost::multi_index::tag<A::tag_prm1> >,
				boost::multi_index::random_access< boost::multi_index::tag<A::tag_prm2> >
			>
		> mi_cont_type;
	test_construct<mi_cont_type>();

	std::cout << "---------------test_construct_of_random_access end-------------------------" << std::endl;
}

void test_observers_of_random_access(void)
{
	typedef yggr::safe_container::safe_multi_index_container<
			A,
			boost::multi_index::indexed_by<
				boost::multi_index::random_access< boost::multi_index::tag<A::tag_prm1> >,
				boost::multi_index::random_access< boost::multi_index::tag<A::tag_prm2> >
			>
		> mi_cont_type;

	A arr[10] = {A(1, 1, "aaa"), A(2, 2, "bbb"), A(3, 3, "ccc"), A(4, 3, "ddd")};
	mi_cont_type cont(arr, arr + 10);
	mi_cont_type cont2(cont.get_allocator());

	cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).get_allocator());
	cont.use_method<1>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 1).get_allocator());
	cont.use_method<A::tag_prm1>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, A::tag_prm1).get_allocator());
	cont.use_method<A::tag_prm2>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, A::tag_prm2).get_allocator());

	std::cout << "---------------test_observers_of_random_access end-------------------------" << std::endl;
}

void test_capacity_of_random_access(void)
{
	typedef yggr::safe_container::safe_multi_index_container<
			A,
			boost::multi_index::indexed_by<
				boost::multi_index::random_access< boost::multi_index::tag<A::tag_prm1> >,
				boost::multi_index::random_access< boost::multi_index::tag<A::tag_prm2> >
			>
		> mi_cont_type;

	A arr[10] = {A(1, 1, "aaa"), A(2, 2, "bbb"), A(3, 3, "ccc"), A(4, 3, "ddd")};
	mi_cont_type cont(arr, arr + 10);
	mi_cont_type::base_type base_cont(arr, arr + 10);
	assert(cont.size() == 10);
	assert(!cont.empty());
	assert(base_cont.size() == cont.size());
	assert(base_cont.max_size() == cont.max_size());

	assert(cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).empty())
			== cont.use_method<A::tag_prm1>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, A::tag_prm1).empty()));
	assert(cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).empty())
				== cont.use_method<A::tag_prm2>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, A::tag_prm2).empty()));

	assert(cont.use_method<1>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 1).empty())
			== cont.use_method<A::tag_prm2>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, A::tag_prm2).empty()));

	assert(cont.use_method<1>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 1).empty())
			== base_cont.empty());

	assert(cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).size())
			== cont.use_method<A::tag_prm1>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, A::tag_prm1).size()));

	assert(cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).size())
			== base_cont.size());

	assert(cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).max_size())
			== base_cont.max_size());

	assert(cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).max_size())
			== base_cont.get<0>().max_size());

	assert(cont.use_method<A::tag_prm2>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, A::tag_prm2).max_size())
			== base_cont.get<1>().max_size());

	cont.use_method(YGGR_PP_MIDX_MAKE_METHOD_NONTAG(mi_cont_type).resize(100));
	assert(cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).size())
			== cont.use_method<1>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 1).size()));

	cont.use_method<1>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 1).resize(100, A(700, 700, "700")));
	assert(cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).size())
			== cont.use_method<1>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 1).size()));

	cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).reserve(500));
	base_cont.get<0>().reserve(500);

	assert(cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).capacity()) == base_cont.get<0>().capacity());
	assert(cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).capacity())
			!= cont.use_method<1>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 1).capacity()));


	std::cout << "---------------test_capacity_of_random_access end-------------------------" << std::endl;
}

void test_modifiers_of_random_access(void)
{
	typedef yggr::safe_container::safe_multi_index_container<
			A,
			boost::multi_index::indexed_by<
				boost::multi_index::random_access< boost::multi_index::tag<A::tag_prm1> >,
				boost::multi_index::random_access< boost::multi_index::tag<A::tag_prm2> >
			>
		> mi_cont_type;

	mi_cont_type cont2;
	A tmp;
	assert(cont2.empty());
	assert(!cont2.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).front(tmp)));
	assert(!cont2.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).back(tmp)));

	assert(!cont2.use_method<A::tag_prm2>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, A::tag_prm2).front(tmp)));
	assert(!cont2.use_method<A::tag_prm2>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, A::tag_prm2).back(tmp)));

	assert(!cont2.use_method<A::tag_prm2>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, A::tag_prm2).pop_front(tmp)));
	assert(!cont2.use_method<A::tag_prm2>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, A::tag_prm2).pop_back(tmp)));
	assert(!cont2.use_method<A::tag_prm2>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, A::tag_prm2).pop_front()));
	assert(!cont2.use_method<A::tag_prm2>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, A::tag_prm2).pop_back()));

	A arr[10] = {A(1, 1, "aaa"), A(2, 2, "bbb"), A(3, 3, "ccc"), A(4, 3, "ddd")};
	mi_cont_type cont(arr, arr + 10);
	assert(cont.size() == 10);

	cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).push_back(A(10, 10, "10")));
	A back;
	assert(cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).back(back)));
	assert(back.prm1 == 10 && back.prm2 == 10);

	assert(cont.size() == 11);

	cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).push_front(A(20, 20, "20")));
	A front;
	assert(cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).front(front)));
	assert(front.prm1 == 20 && front.prm2 == 20);
	assert(cont.size() == 12);

	cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).push_back(A(40, 40, "40")));
	cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).push_front(A(60, 60, "60")));

	assert(cont.size() == 14);
	A back2;
	A front2;

	assert(cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).pop_front(front2)));
	assert(cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).pop_back(back2)));

	assert(back2.prm1 == 40);
	assert(front2.prm1 == 60);
	assert(cont.size() == 12);

	assert(cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).pop_front()));
	assert(cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).pop_back()));

	assert(cont.size() == 10);

	//cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).remove(A(1, 100, "aaa"))); // warning:right
	cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).remove(A(1, 1, "aaa")));

	assert(cont.size() == 9);

	A rm_cmp(3, 0, "");
	cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).remove_if(boost::bind(&less_foo, _1, boost::cref(rm_cmp))));
	assert(cont.size() == 2);

	mi_cont_type::base_type base_cont(arr, arr + 10);

	cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).push_back(A(10, 10, "10")));
	cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).push_back(A(10, 10, "10")));
	cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).push_back(A(10, 10, "10")));
	cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).push_back(A(10, 10, "10")));

	cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).sort());

	assert(cont.size() == 6);

	cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).unique());

	assert(cont.size() == 3);

	cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).push_back(A(10, 10, "10")));
	cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).push_back(A(10, 10, "10")));
	cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).push_back(A(10, 10, "10")));

	assert(cont.size() == 6);

	cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).sort());
	cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).unique(boost::bind(&less_foo, _1, _2)));
	assert(cont.size() == 1);
	cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).reverse());
	assert(cont.size() == 1);
	cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).unique(boost::bind(&less_foo, _1, _2)));
	assert(cont.size() == 1);

	mi_cont_type::base_type base_cont2(arr, arr + 10);
	cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).merge(base_cont2));

	assert(base_cont2.size() == 0);

	assert(cont.size() == 11);
	assert(base_cont.size() == 10);

	cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, A::tag_prm1).swap(base_cont.get<0>()));
	assert(cont.size() == 10);
	assert(base_cont.size() == 11);

	cont.use_method<A::tag_prm1>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).swap(base_cont.get<A::tag_prm1>()));
	assert(cont.size() == 11);
	assert(base_cont.size() == 10);

	A at1;
	assert(cont.use_method<A::tag_prm1>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).at(1, at1)));
	assert(at1.prm1 == 2);

	cont.use_method<0>(YGGR_PP_MIDX_MAKE_METHOD(mi_cont_type, 0).clear());
	assert(cont.empty());

	cont.clear();
	assert(cont.empty());

	std::cout << "---------------test_modifiers_of_random_access end-------------------------" << std::endl;
}

void test_swap_of_random_access(void)
{
	typedef yggr::safe_container::safe_multi_index_container<
			A,
			boost::multi_index::indexed_by<
				boost::multi_index::random_access< boost::multi_index::tag<A::tag_prm1> >,
				boost::multi_index::random_access< boost::multi_index::tag<A::tag_prm2> >
			>
		> mi_cont_type;
	test_swap<mi_cont_type>();
	std::cout << "---------------test_swap_of_random_access end-----------------" << std::endl;
}

#include <boost/mpl/push_back.hpp>
#include <boost/mpl/push_front.hpp>
void test_swap_of_random_access_of_mpl(void)
{
	/*typedef boost::multi_index::indexed_by< boost::multi_index::random_access< boost::multi_index::tag<A::tag_prm1> > > by1_type;
	typedef boost::mpl::push_back<by1_type, boost::multi_index::random_access< boost::multi_index::tag<A::tag_prm2> > >::type by2_type;*/

	typedef boost::multi_index::indexed_by< boost::multi_index::random_access< boost::multi_index::tag<A::tag_prm2> > > by1_type;
	typedef boost::mpl::push_back<by1_type, boost::multi_index::random_access< boost::multi_index::tag<A::tag_prm1> > >::type by2_type;
	typedef yggr::safe_container::safe_multi_index_container<
			A,
			by2_type
		> mi_cont_type;
	test_swap<mi_cont_type>();
	std::cout << "---------------test_swap_of_random_access_of_mpl end-----------------" << std::endl;
}

#endif // _DEBUG

void debug_test(void)
{
#ifdef _DEBUG
	test_construct_of_hash();
	test_observers_of_hash();
	test_capacity_of_hash();
	test_modifiers_of_hash();
	test_set_operations_of_hash();
	test_bucket_of_hash();
	test_hash_policy_of_hash();
	test_swap_of_hash();

	std::cout << "///////////////////////////////////////" << std::endl;

	test_construct_of_ordered();
	test_observers_of_ordered();
	test_capacity_of_ordered();
	test_modifiers_of_ordered();
	test_set_operations_of_ordered();
	test_swap_of_ordered();

	std::cout << "////////////////////////////////////" << std::endl;

	test_construct_of_sequenced();
	test_observers_of_sequenced();
	test_capacity_of_sequecned();
	test_modifiers_of_sequecned();
	test_swap_of_sequecned();

	std::cout << "////////////////////////////////////" << std::endl;

	test_construct_of_random_access();
	test_observers_of_random_access();
	test_capacity_of_random_access();
	test_modifiers_of_random_access();
	test_swap_of_random_access();
	test_swap_of_random_access_of_mpl();

#endif // _DEBUG
}

int main(int argc, char* argv[])
{
	debug_test();

	char cc = 0;
	std::cin >> cc;
	return 0;
}
