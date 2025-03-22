// multimap_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <cassert>

#include <yggr/container/map.hpp>

#if defined(_MSC_VER)
#	include <vld.h>
#endif // _MSC_VER

#define TEST_COMPILE_ERROR 0

void test_construct(void)
{
	int arr[5] = {1, 1, 2, 2, 3};
	typedef yggr::multimap<int, int> multimap_type;
	typedef 
		yggr::multimap
		<
			multimap_type::key_type, 
			multimap_type::mapped_type, 
			multimap_type::key_compare, 
			multimap_type::allocator_type
		> isomeric_same_multimap_type;

#if !(BOOST_VERSION < 105600)
	typedef multimap_type::base_type base_multimap_type;
	typedef isomeric_same_multimap_type::base_type isomeric_same_base_multimap_type;
#else
	typedef multimap_type base_multimap_type;
	typedef isomeric_same_multimap_type isomeric_same_base_multimap_type;
#endif // #if !(BOOST_VERSION < 105600)

	yggr::multimap<int, int> multimap_sample;
	for(int i = 0, isize = 5; i != isize; ++i)
	{
		multimap_sample.insert(std::make_pair(arr[i], arr[i]));
	}
	yggr_test_assert(multimap_sample.size() == 5);

	multimap_type multimap0(multimap_sample.begin(), multimap_sample.end());
	yggr_test_assert(multimap0.size() == 5);

	{
		multimap_type multimap;
		yggr_test_assert(multimap.empty());
	}

#if !(BOOST_VERSION < 105600)
	{
		multimap_type multimap(multimap_sample.get_allocator());
		yggr_test_assert(multimap.empty());
	}

#endif // !(BOOST_VERSION < 105600)

	{
		multimap_type multimap(multimap_sample.key_comp(), multimap_sample.get_allocator());
		yggr_test_assert(multimap.size() == 0);
	}

	{
		multimap_type multimap(multimap_sample.begin(), multimap_sample.end());
		yggr_test_assert(multimap.size() == 5);
		yggr_test_assert(multimap.begin()->second == 1);
		yggr_test_assert(multimap.rbegin()->second == 3);
	}

	
#if !(BOOST_VERSION < 105600)
	{
		multimap_type multimap(multimap_sample.begin(), multimap_sample.end(), multimap_sample.get_allocator());
		yggr_test_assert(multimap.size() == 5);
		yggr_test_assert(multimap.begin()->second == 1);
		yggr_test_assert(multimap.rbegin()->second == 3);
	}
#endif // !(BOOST_VERSION < 105600)

	{
		multimap_type multimap(multimap_sample.begin(), multimap_sample.end(), multimap_sample.key_comp(), multimap_sample.get_allocator());
		yggr_test_assert(multimap.size() == 5);
		yggr_test_assert(multimap.begin()->second == 1);
		yggr_test_assert(multimap.rbegin()->second == 3);
	}

#if !defined(BOOST_NO_CXX11_HDR_INITIALIZER_LIST)

#	if !(BOOST_VERSION < 105700)
	{
		multimap_type multimap({std::make_pair(1, 1),
								std::make_pair(2, 2),
								std::make_pair(3, 3), 
								std::make_pair(4, 4), 
								std::make_pair(5, 5)});
		yggr_test_assert(multimap.size() == 5);
		yggr_test_assert(multimap.begin()->second == 1);
		yggr_test_assert(multimap.rbegin()->second == 5);
	}

	{
		multimap_type multimap({std::make_pair(1, 1), 
								std::make_pair(2, 2), 
								std::make_pair(3, 3), 
								std::make_pair(4, 4), 
								std::make_pair(5, 5)},
								multimap0.key_comp(), 
								multimap0.get_allocator());
		yggr_test_assert(multimap.size() == 5);
		yggr_test_assert(multimap.begin()->second == 1);
		yggr_test_assert(multimap.rbegin()->second == 5);
	}

#	endif // #if !(BOOST_VERSION < 105700)

#	if !(BOOST_VERSION < 105600)
	{
		multimap_type multimap({std::make_pair(1, 1), 
								std::make_pair(2, 2), 
								std::make_pair(3, 3), 
								std::make_pair(4, 4), 
								std::make_pair(5, 5)},
								multimap0.get_allocator());
		yggr_test_assert(multimap.size() == 5);
		yggr_test_assert(multimap.begin()->second == 1);
		yggr_test_assert(multimap.rbegin()->second == 5);
	}
#	endif // !(BOOST_VERSION < 105600)

#endif // BOOST_NO_CXX11_HDR_INITIALIZER_LIST

	{
		base_multimap_type tmp(multimap_sample.begin(), multimap_sample.end());
		multimap_type multimap(tmp);

		yggr_test_assert(multimap.size() == 5);
		yggr_test_assert(multimap == tmp);
		yggr_test_assert(tmp == multimap);
	}

	{
		multimap_type tmp(multimap_sample.begin(), multimap_sample.end());
		multimap_type multimap(tmp);

		yggr_test_assert(multimap.size() == 5);
		yggr_test_assert(multimap == tmp);
		yggr_test_assert(tmp == multimap);
	}

	{
		isomeric_same_base_multimap_type tmp(multimap_sample.begin(), multimap_sample.end());
		multimap_type multimap(tmp);

		yggr_test_assert(multimap.size() == 5);
		yggr_test_assert(multimap == tmp);
		yggr_test_assert(tmp == multimap);
	}

	{
		isomeric_same_multimap_type tmp(multimap_sample.begin(), multimap_sample.end());
		multimap_type multimap(tmp);

		yggr_test_assert(multimap.size() == 5);
		yggr_test_assert(multimap == tmp);
		yggr_test_assert(tmp == multimap);
	}

	{
		base_multimap_type tmp(multimap_sample.begin(), multimap_sample.end());
		multimap_type multimap(boost::move(tmp));

		yggr_test_assert(multimap.size() == 5);
		yggr_test_assert(multimap.begin()->second == 1);
		yggr_test_assert(multimap.rbegin()->second == 3);
	}

	{
		multimap_type tmp(multimap_sample.begin(), multimap_sample.end());
		multimap_type multimap(boost::move(tmp));

		yggr_test_assert(multimap.size() == 5);
		yggr_test_assert(multimap.begin()->second == 1);
		yggr_test_assert(multimap.rbegin()->second == 3);
	}

	{
		isomeric_same_base_multimap_type tmp(multimap_sample.begin(), multimap_sample.end());
		multimap_type multimap(boost::move(tmp));

		yggr_test_assert(multimap.size() == 5);
		yggr_test_assert(multimap.begin()->second == 1);
		yggr_test_assert(multimap.rbegin()->second == 3);
	}

	{
		isomeric_same_multimap_type tmp(multimap_sample.begin(), multimap_sample.end());
		multimap_type multimap(boost::move(tmp));

		yggr_test_assert(multimap.size() == 5);
		yggr_test_assert(multimap.begin()->second == 1);
		yggr_test_assert(multimap.rbegin()->second == 3);
	}

	{
		base_multimap_type tmp(multimap_sample.begin(), multimap_sample.end());
		multimap_type multimap(tmp, tmp.get_allocator());

		yggr_test_assert(multimap.size() == 5);
		yggr_test_assert(multimap == tmp);
		yggr_test_assert(tmp == multimap);
	}

	{
		multimap_type tmp(multimap_sample.begin(), multimap_sample.end());
		multimap_type multimap(tmp, tmp.get_allocator());

		yggr_test_assert(multimap.size() == 5);
		yggr_test_assert(multimap == tmp);
		yggr_test_assert(tmp == multimap);
	}

	{
		isomeric_same_base_multimap_type tmp(multimap_sample.begin(), multimap_sample.end());
		multimap_type multimap(tmp, tmp.get_allocator());

		yggr_test_assert(multimap.size() == 5);
		yggr_test_assert(multimap == tmp);
		yggr_test_assert(tmp == multimap);
	}

	{
		isomeric_same_multimap_type tmp(multimap_sample.begin(), multimap_sample.end());
		multimap_type multimap(tmp, tmp.get_allocator());

		yggr_test_assert(multimap.size() == 5);
		yggr_test_assert(multimap == tmp);
		yggr_test_assert(tmp == multimap);
	}

	{
		base_multimap_type tmp(multimap_sample.begin(), multimap_sample.end());
		multimap_type multimap(boost::move(tmp), tmp.get_allocator());

		yggr_test_assert(multimap.size() == 5);
		yggr_test_assert(multimap.begin()->second == 1);
		yggr_test_assert(multimap.rbegin()->second == 3);
	}


	{
		multimap_type tmp(multimap_sample.begin(), multimap_sample.end());
		multimap_type multimap(boost::move(tmp), tmp.get_allocator());

		yggr_test_assert(multimap.size() == 5);
		yggr_test_assert(multimap.begin()->second == 1);
		yggr_test_assert(multimap.rbegin()->second == 3);
	}

	{
		isomeric_same_base_multimap_type tmp(multimap_sample.begin(), multimap_sample.end());
		multimap_type multimap(boost::move(tmp), tmp.get_allocator());

		yggr_test_assert(multimap.size() == 5);
		yggr_test_assert(multimap.begin()->second == 1);
		yggr_test_assert(multimap.rbegin()->second == 3);
	}

	{
		isomeric_same_multimap_type tmp(multimap_sample.begin(), multimap_sample.end());
		multimap_type multimap(boost::move(tmp), tmp.get_allocator());

		yggr_test_assert(multimap.size() == 5);
		yggr_test_assert(multimap.begin()->second == 1);
		yggr_test_assert(multimap.rbegin()->second == 3);
	}

	std::cout << "------------test_construct end---------------" << std::endl;
}

void test_compare(void)
{
	int arr[5] = {1, 1, 2, 2, 3};
	int arr_less[5] = {1, 1, 2, 2, 2};
	int arr_greater[5] = {1, 1, 2, 2, 4};

	typedef yggr::multimap<int, int> multimap_type;
	typedef 
		yggr::multimap
		<
			multimap_type::key_type, 
			multimap_type::mapped_type, 
			multimap_type::key_compare, 
			multimap_type::allocator_type
		> isomeric_same_multimap_type;

#if !(BOOST_VERSION < 105600)
	typedef multimap_type::base_type base_multimap_type;
	typedef isomeric_same_multimap_type::base_type isomeric_same_base_multimap_type;
#else
	typedef multimap_type base_multimap_type;
	typedef isomeric_same_multimap_type isomeric_same_base_multimap_type;
#endif // #if !(BOOST_VERSION < 105600)

	yggr::multimap<int, int> multimap_equal_sample;
	yggr::multimap<int, int> multimap_less_sample;
	yggr::multimap<int, int> multimap_greater_sample;

	for(int i =0, isize = 5; i != isize; ++i)
	{
		multimap_equal_sample.insert(std::make_pair(arr[i], arr[i]));
		multimap_less_sample.insert(std::make_pair(arr_less[i], arr_less[i]));
		multimap_greater_sample.insert(std::make_pair(arr_greater[i], arr_greater[i]));
	}

	yggr_test_assert(multimap_equal_sample.size() == 5);
	yggr_test_assert(multimap_less_sample.size() == 5);
	yggr_test_assert(multimap_greater_sample.size() == 5);


	multimap_type multimap(multimap_equal_sample.begin(), multimap_equal_sample.end());

	multimap_type multimap_equal(multimap_equal_sample.begin(), multimap_equal_sample.end());
	isomeric_same_multimap_type multimap_same_equal(multimap_equal_sample.begin(), multimap_equal_sample.end());

	multimap_type multimap_less(multimap_less_sample.begin(), multimap_less_sample.end());
	isomeric_same_multimap_type multimap_same_less(multimap_less_sample.begin(), multimap_less_sample.end());

	multimap_type multimap_greater(multimap_greater_sample.begin(), multimap_greater_sample.end());
	isomeric_same_multimap_type multimap_same_greater(multimap_greater_sample.begin(), multimap_greater_sample.end());

	{
		// ==
		yggr_test_assert(static_cast<const base_multimap_type&>(multimap) == multimap);
		yggr_test_assert(multimap == static_cast<const base_multimap_type&>(multimap));
		yggr_test_assert(multimap == multimap);

		yggr_test_assert(static_cast<const base_multimap_type&>(multimap) == multimap_same_equal);
		yggr_test_assert(multimap == static_cast<const isomeric_same_base_multimap_type&>(multimap_same_equal));
		yggr_test_assert(multimap == multimap_same_equal);

		// != 
		yggr_test_assert(static_cast<const base_multimap_type&>(multimap) != multimap_less);
		yggr_test_assert(multimap != static_cast<const base_multimap_type&>(multimap_less));
		yggr_test_assert(multimap != multimap_less);

		yggr_test_assert(static_cast<const base_multimap_type&>(multimap) != multimap_same_less);
		yggr_test_assert(multimap != static_cast<const isomeric_same_base_multimap_type&>(multimap_same_less));
		yggr_test_assert(multimap != multimap_same_less);

		// <
		yggr_test_assert(static_cast<const base_multimap_type&>(multimap) < multimap_greater);
		yggr_test_assert(multimap < static_cast<const base_multimap_type&>(multimap_greater));
		yggr_test_assert(multimap < multimap_greater);

		yggr_test_assert(static_cast<const base_multimap_type&>(multimap) < multimap_same_greater);
		yggr_test_assert(multimap < static_cast<const isomeric_same_base_multimap_type&>(multimap_same_greater));
		yggr_test_assert(multimap < multimap_same_greater);

		// <=
		yggr_test_assert(static_cast<const base_multimap_type&>(multimap) <= multimap);
		yggr_test_assert(multimap <= static_cast<const base_multimap_type&>(multimap));
		yggr_test_assert(multimap <= multimap);

		yggr_test_assert(static_cast<const base_multimap_type&>(multimap) <= multimap_equal);
		yggr_test_assert(multimap <= static_cast<const base_multimap_type&>(multimap_equal));
		yggr_test_assert(multimap <= multimap_equal);

		yggr_test_assert(static_cast<const base_multimap_type&>(multimap) <= multimap_same_equal);
		yggr_test_assert(multimap <= static_cast<const isomeric_same_base_multimap_type&>(multimap_same_equal));
		yggr_test_assert(multimap <= multimap_same_equal);

		yggr_test_assert(static_cast<const base_multimap_type&>(multimap) <= multimap_greater);
		yggr_test_assert(multimap <= static_cast<const base_multimap_type&>(multimap_greater));
		yggr_test_assert(multimap <= multimap_greater);

		yggr_test_assert(static_cast<const base_multimap_type&>(multimap) <= multimap_same_greater);
		yggr_test_assert(multimap <= static_cast<const isomeric_same_base_multimap_type&>(multimap_same_greater));
		yggr_test_assert(multimap <= multimap_same_greater);

		// >
		yggr_test_assert(static_cast<const base_multimap_type&>(multimap) > multimap_less);
		yggr_test_assert(multimap > static_cast<const base_multimap_type&>(multimap_less));
		yggr_test_assert(multimap > multimap_less);

		yggr_test_assert(static_cast<const base_multimap_type&>(multimap) > multimap_same_less);
		yggr_test_assert(multimap > static_cast<const isomeric_same_base_multimap_type&>(multimap_same_less));
		yggr_test_assert(multimap > multimap_same_less);

		// >=
		yggr_test_assert(static_cast<const base_multimap_type&>(multimap) >= multimap);
		yggr_test_assert(multimap >= static_cast<const base_multimap_type&>(multimap));
		yggr_test_assert(multimap >= multimap);

		yggr_test_assert(static_cast<const base_multimap_type&>(multimap) >= multimap_equal);
		yggr_test_assert(multimap >= static_cast<const base_multimap_type&>(multimap_equal));
		yggr_test_assert(multimap >= multimap_equal);

		yggr_test_assert(static_cast<const base_multimap_type&>(multimap) >= multimap_same_equal);
		yggr_test_assert(multimap >= static_cast<const isomeric_same_base_multimap_type&>(multimap_same_equal));
		yggr_test_assert(multimap >= multimap_same_equal);

		yggr_test_assert(static_cast<const base_multimap_type&>(multimap) >= multimap_less);
		yggr_test_assert(multimap >= static_cast<const base_multimap_type&>(multimap_less));
		yggr_test_assert(multimap >= multimap_less);

		yggr_test_assert(static_cast<const base_multimap_type&>(multimap) >= multimap_same_less);
		yggr_test_assert(multimap >= static_cast<const isomeric_same_base_multimap_type&>(multimap_same_less));
		yggr_test_assert(multimap >= multimap_same_less);

	}

	std::cout << "------------test_compare end---------------" << std::endl;
}

#if !(BOOST_VERSION < 106200)
template<typename LCont, typename RCont>
void test_member_merge_ss(void)
{
	typedef LCont l_cont_type;
	typedef RCont r_cont_type;

	typedef typename l_cont_type::key_type val_type;

	val_type arr1[10] = {1, 1, 2, 2, 3, 3, 4, 4, 5, 5};
	val_type arr2[10] = {5, 5, 6, 6, 7, 7, 8, 8, 9, 9};

	std::map<val_type, val_type> simple1;
	std::map<val_type, val_type> simple2;

	for(int i = 0, isize = 10; i != isize; ++i)
	{
		simple1.insert(std::make_pair(arr1[i], arr1[i]));
		simple2.insert(std::make_pair(arr2[i], arr2[i]));
	}

	{
		l_cont_type cont_eins(simple1.begin(), simple1.end());
		r_cont_type cont_zwei(simple2.begin(), simple2.end());

		yggr_test_assert(cont_eins.size() == 5);
		yggr_test_assert(cont_zwei.size() == 5);
		cont_eins.merge(boost::move(cont_zwei));
		yggr_test_assert(cont_eins.size() == 9);
		yggr_test_assert(cont_zwei.size() == 1);
	}

	{
		l_cont_type cont_eins(simple1.begin(), simple1.end());
		r_cont_type cont_zwei(simple2.begin(), simple2.end());

		yggr_test_assert(cont_eins.size() == 5);
		yggr_test_assert(cont_zwei.size() == 5);
		cont_eins.merge(cont_zwei);
		yggr_test_assert(cont_eins.size() == 9);
		yggr_test_assert(cont_zwei.size() == 1);
	}
}

template<typename LCont, typename RCont>
void test_member_merge_sm(void)
{
	typedef LCont l_cont_type;
	typedef RCont r_cont_type;

	typedef typename l_cont_type::key_type val_type;

	val_type arr1[10] = {1, 1, 2, 2, 3, 3, 4, 4, 5, 5};
	val_type arr2[10] = {5, 5, 6, 6, 7, 7, 8, 8, 9, 9};

	std::map<val_type, val_type> simple1;
	std::multimap<val_type, val_type> simple2;

	for(int i = 0, isize = 10; i != isize; ++i)
	{
		simple1.insert(std::make_pair(arr1[i], arr1[i]));
		simple2.insert(std::make_pair(arr2[i], arr2[i]));
	}

	{
		l_cont_type cont_eins(simple1.begin(), simple1.end());
		r_cont_type cont_zwei(simple2.begin(), simple2.end());

		yggr_test_assert(cont_eins.size() == 5);
		yggr_test_assert(cont_zwei.size() == 10);
		cont_eins.merge(boost::move(cont_zwei));
		yggr_test_assert(cont_eins.size() == 9);
		yggr_test_assert(cont_zwei.size() == 6);
	}

	{
		l_cont_type cont_eins(simple1.begin(), simple1.end());
		r_cont_type cont_zwei(simple2.begin(), simple2.end());

		yggr_test_assert(cont_eins.size() == 5);
		yggr_test_assert(cont_zwei.size() == 10);
		cont_eins.merge(cont_zwei);
		yggr_test_assert(cont_eins.size() == 9);
		yggr_test_assert(cont_zwei.size() == 6);
	}
}

template<typename LCont, typename RCont>
void test_member_merge_ms(void)
{
	typedef LCont l_cont_type;
	typedef RCont r_cont_type;

	typedef typename l_cont_type::key_type val_type;

	val_type arr1[10] = {1, 1, 2, 2, 3, 3, 4, 4, 5, 5};
	val_type arr2[10] = {5, 5, 6, 6, 7, 7, 8, 8, 9, 9};

	std::multimap<val_type, val_type> simple1;
	std::map<val_type, val_type> simple2;

	for(int i = 0, isize = 10; i != isize; ++i)
	{
		simple1.insert(std::make_pair(arr1[i], arr1[i]));
		simple2.insert(std::make_pair(arr2[i], arr2[i]));
	}

	{
		l_cont_type cont_eins(simple1.begin(), simple1.end());
		r_cont_type cont_zwei(simple2.begin(), simple2.end());

		yggr_test_assert(cont_eins.size() == 10);
		yggr_test_assert(cont_zwei.size() == 5);
		cont_eins.merge(boost::move(cont_zwei));
		yggr_test_assert(cont_eins.size() == 15);
		yggr_test_assert(cont_zwei.size() == 0);
	}

	{
		l_cont_type cont_eins(simple1.begin(), simple1.end());
		r_cont_type cont_zwei(simple2.begin(), simple2.end());

		yggr_test_assert(cont_eins.size() == 10);
		yggr_test_assert(cont_zwei.size() == 5);
		cont_eins.merge(cont_zwei);
		yggr_test_assert(cont_eins.size() == 15);
		yggr_test_assert(cont_zwei.size() == 0);
	}
}

template<typename LCont, typename RCont>
void test_member_merge_mm(void)
{
	typedef LCont l_cont_type;
	typedef RCont r_cont_type;

	typedef typename l_cont_type::key_type val_type;

	val_type arr1[10] = {1, 1, 2, 2, 3, 3, 4, 4, 5, 5};
	val_type arr2[10] = {5, 5, 6, 6, 7, 7, 8, 8, 9, 9};

	std::multimap<val_type, val_type> simple1;
	std::multimap<val_type, val_type> simple2;

	for(int i = 0, isize = 10; i != isize; ++i)
	{
		simple1.insert(std::make_pair(arr1[i], arr1[i]));
		simple2.insert(std::make_pair(arr2[i], arr2[i]));
	}

	{
		l_cont_type cont_eins(simple1.begin(), simple1.end());
		r_cont_type cont_zwei(simple2.begin(), simple2.end());

		yggr_test_assert(cont_eins.size() == 10);
		yggr_test_assert(cont_zwei.size() == 10);
		cont_eins.merge(boost::move(cont_zwei));
		yggr_test_assert(cont_eins.size() == 20);
		yggr_test_assert(cont_zwei.size() == 0);
	}

	{
		l_cont_type cont_eins(simple1.begin(), simple1.end());
		r_cont_type cont_zwei(simple2.begin(), simple2.end());

		yggr_test_assert(cont_eins.size() == 10);
		yggr_test_assert(cont_zwei.size() == 10);
		cont_eins.merge(cont_zwei);
		yggr_test_assert(cont_eins.size() == 20);
		yggr_test_assert(cont_zwei.size() == 0);
	}
}

#endif //!(BOOST_VERSION < 106200)

template<typename LCont, typename RCont>
void test_merge_ss(void)
{
	typedef LCont l_cont_type;
	typedef RCont r_cont_type;

	typedef typename l_cont_type::key_type val_type;

	val_type arr1[10] = {1, 1, 2, 2, 3, 3, 4, 4, 5, 5};
	val_type arr2[10] = {5, 5, 6, 6, 7, 7, 8, 8, 9, 9};

	std::map<val_type, val_type> simple1;
	std::map<val_type, val_type> simple2;

	for(int i = 0, isize = 10; i != isize; ++i)
	{
		simple1.insert(std::make_pair(arr1[i], arr1[i]));
		simple2.insert(std::make_pair(arr2[i], arr2[i]));
	}

	{
		l_cont_type cont_eins(simple1.begin(), simple1.end());
		r_cont_type cont_zwei(simple2.begin(), simple2.end());

		yggr_test_assert(cont_eins.size() == 5);
		yggr_test_assert(cont_zwei.size() == 5);
		yggr::container::merge(cont_eins, boost::move(cont_zwei));
		yggr_test_assert(cont_eins.size() == 9);
		yggr_test_assert(cont_zwei.size() == 1);
	}

	{
		l_cont_type cont_eins(simple1.begin(), simple1.end());
		r_cont_type cont_zwei(simple2.begin(), simple2.end());

		yggr_test_assert(cont_eins.size() == 5);
		yggr_test_assert(cont_zwei.size() == 5);
		yggr::container::merge(cont_eins, cont_zwei);
		yggr_test_assert(cont_eins.size() == 9);
		yggr_test_assert(cont_zwei.size() == 1);
	}
}

template<typename LCont, typename RCont>
void test_merge_sm(void)
{
	typedef LCont l_cont_type;
	typedef RCont r_cont_type;

	typedef typename l_cont_type::key_type val_type;

	val_type arr1[10] = {1, 1, 2, 2, 3, 3, 4, 4, 5, 5};
	val_type arr2[10] = {5, 5, 6, 6, 7, 7, 8, 8, 9, 9};

	std::map<val_type, val_type> simple1;
	std::multimap<val_type, val_type> simple2;

	for(int i = 0, isize = 10; i != isize; ++i)
	{
		simple1.insert(std::make_pair(arr1[i], arr1[i]));
		simple2.insert(std::make_pair(arr2[i], arr2[i]));
	}

	{
		l_cont_type cont_eins(simple1.begin(), simple1.end());
		r_cont_type cont_zwei(simple2.begin(), simple2.end());

		yggr_test_assert(cont_eins.size() == 5);
		yggr_test_assert(cont_zwei.size() == 10);
		yggr::container::merge(cont_eins, boost::move(cont_zwei));
		yggr_test_assert(cont_eins.size() == 9);
		yggr_test_assert(cont_zwei.size() == 6);
	}

	{
		l_cont_type cont_eins(simple1.begin(), simple1.end());
		r_cont_type cont_zwei(simple2.begin(), simple2.end());

		yggr_test_assert(cont_eins.size() == 5);
		yggr_test_assert(cont_zwei.size() == 10);
		yggr::container::merge(cont_eins, cont_zwei);
		yggr_test_assert(cont_eins.size() == 9);
		yggr_test_assert(cont_zwei.size() == 6);
	}
}

template<typename LCont, typename RCont>
void test_merge_ms(void)
{
	typedef LCont l_cont_type;
	typedef RCont r_cont_type;

	typedef typename l_cont_type::key_type val_type;

	val_type arr1[10] = {1, 1, 2, 2, 3, 3, 4, 4, 5, 5};
	val_type arr2[10] = {5, 5, 6, 6, 7, 7, 8, 8, 9, 9};

	std::multimap<val_type, val_type> simple1;
	std::map<val_type, val_type> simple2;

	for(int i = 0, isize = 10; i != isize; ++i)
	{
		simple1.insert(std::make_pair(arr1[i], arr1[i]));
		simple2.insert(std::make_pair(arr2[i], arr2[i]));
	}

	{
		l_cont_type cont_eins(simple1.begin(), simple1.end());
		r_cont_type cont_zwei(simple2.begin(), simple2.end());

		yggr_test_assert(cont_eins.size() == 10);
		yggr_test_assert(cont_zwei.size() == 5);
		yggr::container::merge(cont_eins, boost::move(cont_zwei));
		yggr_test_assert(cont_eins.size() == 15);
		yggr_test_assert(cont_zwei.size() == 0);
	}

	{
		l_cont_type cont_eins(simple1.begin(), simple1.end());
		r_cont_type cont_zwei(simple2.begin(), simple2.end());

		yggr_test_assert(cont_eins.size() == 10);
		yggr_test_assert(cont_zwei.size() == 5);
		yggr::container::merge(cont_eins, cont_zwei);
		yggr_test_assert(cont_eins.size() == 15);
		yggr_test_assert(cont_zwei.size() == 0);
	}
}

template<typename LCont, typename RCont>
void test_merge_mm(void)
{
	typedef LCont l_cont_type;
	typedef RCont r_cont_type;

	typedef typename l_cont_type::key_type val_type;

	val_type arr1[10] = {1, 1, 2, 2, 3, 3, 4, 4, 5, 5};
	val_type arr2[10] = {5, 5, 6, 6, 7, 7, 8, 8, 9, 9};

	std::multimap<val_type, val_type> simple1;
	std::multimap<val_type, val_type> simple2;

	for(int i = 0, isize = 10; i != isize; ++i)
	{
		simple1.insert(std::make_pair(arr1[i], arr1[i]));
		simple2.insert(std::make_pair(arr2[i], arr2[i]));
	}

	{
		l_cont_type cont_eins(simple1.begin(), simple1.end());
		r_cont_type cont_zwei(simple2.begin(), simple2.end());

		yggr_test_assert(cont_eins.size() == 10);
		yggr_test_assert(cont_zwei.size() == 10);
		yggr::container::merge(cont_eins, boost::move(cont_zwei));
		yggr_test_assert(cont_eins.size() == 20);
		yggr_test_assert(cont_zwei.size() == 0);
	}

	{
		l_cont_type cont_eins(simple1.begin(), simple1.end());
		r_cont_type cont_zwei(simple2.begin(), simple2.end());

		yggr_test_assert(cont_eins.size() == 10);
		yggr_test_assert(cont_zwei.size() == 10);
		yggr::container::merge(cont_eins, cont_zwei);
		yggr_test_assert(cont_eins.size() == 20);
		yggr_test_assert(cont_zwei.size() == 0);
	}
}

void test_merge(void)
{
	typedef int val_type;
	typedef std::map<val_type, val_type> stl_map_type;
	typedef std::multimap<val_type, val_type> stl_mmap_type;

	typedef boost::container::map<val_type, val_type> boost_map_type;
	typedef boost::container::multimap<val_type, val_type> boost_mmap_type;

	typedef yggr::map<val_type, val_type> yggr_map_type;
	typedef yggr::multimap<val_type, val_type> yggr_mmap_type;

	// ss
	test_merge_ss<stl_map_type, stl_map_type>();
	test_merge_ss<stl_map_type, boost_map_type>();
	test_merge_ss<stl_map_type, yggr_map_type>();

	test_merge_ss<boost_map_type, stl_map_type>();
	test_merge_ss<boost_map_type, boost_map_type>();
	test_merge_ss<boost_map_type, yggr_map_type>();

	test_merge_ss<yggr_map_type, stl_map_type>();
	test_merge_ss<yggr_map_type, boost_map_type>();
	test_merge_ss<yggr_map_type, yggr_map_type>();

	// sm
	test_merge_sm<stl_map_type, stl_mmap_type>();
	test_merge_sm<stl_map_type, boost_mmap_type>();
	test_merge_sm<stl_map_type, yggr_mmap_type>();

	test_merge_sm<boost_map_type, stl_mmap_type>();
	test_merge_sm<boost_map_type, boost_mmap_type>();
	test_merge_sm<boost_map_type, yggr_mmap_type>();

	test_merge_sm<yggr_map_type, stl_mmap_type>();
	test_merge_sm<yggr_map_type, boost_mmap_type>();
	test_merge_sm<yggr_map_type, yggr_mmap_type>();

	// ms
	test_merge_ms<stl_mmap_type, stl_map_type>();
	test_merge_ms<stl_mmap_type, boost_map_type>();
	test_merge_ms<stl_mmap_type, yggr_map_type>();

	test_merge_ms<boost_mmap_type, stl_map_type>();
	test_merge_ms<boost_mmap_type, boost_map_type>();
	test_merge_ms<boost_mmap_type, yggr_map_type>();

	test_merge_ms<yggr_mmap_type, stl_map_type>();
	test_merge_ms<yggr_mmap_type, boost_map_type>();
	test_merge_ms<yggr_mmap_type, yggr_map_type>();

	// mm
	test_merge_mm<stl_mmap_type, stl_mmap_type>();
	test_merge_mm<stl_mmap_type, boost_mmap_type>();
	test_merge_mm<stl_mmap_type, yggr_mmap_type>();

	test_merge_mm<boost_mmap_type, stl_mmap_type>();
	test_merge_mm<boost_mmap_type, boost_mmap_type>();
	test_merge_mm<boost_mmap_type, yggr_mmap_type>();

	test_merge_mm<yggr_mmap_type, stl_mmap_type>();
	test_merge_mm<yggr_mmap_type, boost_mmap_type>();
	test_merge_mm<yggr_mmap_type, yggr_mmap_type>();

#if !(BOOST_VERSION < 106200)
	// ss
	test_member_merge_ss<yggr_map_type, stl_map_type>();
	test_member_merge_ss<yggr_map_type, boost_map_type>();
	test_member_merge_ss<yggr_map_type, yggr_map_type>();

	// sm
	test_member_merge_sm<yggr_map_type, stl_mmap_type>();
	test_member_merge_sm<yggr_map_type, boost_mmap_type>();
	test_member_merge_sm<yggr_map_type, yggr_mmap_type>();

	// ms
	test_member_merge_ms<yggr_mmap_type, stl_map_type>();
	test_member_merge_ms<yggr_mmap_type, boost_map_type>();
	test_member_merge_ms<yggr_mmap_type, yggr_map_type>();

	// mm
	test_member_merge_mm<yggr_mmap_type, stl_mmap_type>();
	test_member_merge_mm<yggr_mmap_type, boost_mmap_type>();
	test_member_merge_mm<yggr_mmap_type, yggr_mmap_type>();
#endif // !(BOOST_VERSION < 106200)

	std::cout << "------------test_merge end---------------" << std::endl;
}

template<std::size_t V>
struct test_pred_t
{
	inline bool operator()(const std::pair<const int, int>& n) const
	{
		return n.first == V;
	}
};

void test_erase_if(void)
{
	int arr[5] = {1, 1, 2, 2, 3};

	std::multimap<int, int> multimap_sample;
	for(int i = 0, isize = 5; i != isize; ++i)
	{
		multimap_sample.insert(std::make_pair(arr[i], arr[i]));
	}

	{
		typedef std::multimap<int, int> multimap_type;

		multimap_type multimap(multimap_sample.begin(), multimap_sample.end());

		std::size_t s = 0;

		s = yggr::container::erase_if(multimap, test_pred_t<1>());
		yggr_test_assert(s == 2);

		s = yggr::container::erase_if(multimap, test_pred_t<6>());
		yggr_test_assert(s == 0);
	}

	{
		typedef boost::container::multimap<int, int> multimap_type;

		multimap_type multimap(multimap_sample.begin(), multimap_sample.end());

		std::size_t s = 0;

		s = yggr::container::erase_if(multimap, test_pred_t<1>());
		yggr_test_assert(s == 2);

		s = yggr::container::erase_if(multimap, test_pred_t<6>());
		yggr_test_assert(s == 0);
	}

	{
		typedef yggr::multimap<int, int> multimap_type;

		multimap_type multimap(multimap_sample.begin(), multimap_sample.end());

		std::size_t s = 0;

		s = yggr::container::erase_if(multimap, test_pred_t<1>());
		yggr_test_assert(s == 2);

		s = yggr::container::erase_if(multimap, test_pred_t<6>());
		yggr_test_assert(s == 0);
	}

	std::cout << "------------test_erase_if end---------------" << std::endl;
}

int main(int argc, char* argv[])
{
	test_construct();
	test_compare();
	test_merge();

	test_erase_if();

	wait_any_key(argc, argv);
	return 0;
}