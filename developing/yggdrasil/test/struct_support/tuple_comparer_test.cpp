//tuple_comparer_test.cpp

//#include <functional>
#include <iostream>
#include <cassert>

#include <yggr/struct_support/tuple.hpp> // must befor unordered before

#include <boost/unordered_set.hpp>


#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

void test_ene(void)
{
	assert(boost::make_tuple(1) == boost::make_tuple(1));
	assert(boost::make_tuple(2) != boost::make_tuple(1));

	assert(boost::make_tuple(1, 1) == boost::make_tuple(1, 1));
	assert(boost::make_tuple(1, 2) != boost::make_tuple(1, 1));

	assert(boost::make_tuple(1, 1) == boost::make_tuple(1.0, 1.0));
	assert(boost::make_tuple(1, 2) != boost::make_tuple(1.0, 1.0));

	assert(boost::make_tuple(1, 1) != boost::make_tuple(1.0));

	assert(boost::make_tuple(1, 1, 1, 1, 2) == boost::make_tuple(1.0, 1.0, 1.0, 1.0, 2.0));

	std::equal_to< boost::tuple<int, int> > eq;
	assert( (eq(boost::make_tuple(1,2), boost::make_tuple(1, 2))) );

	std::cout << boost::make_tuple(1, 2, 3, 4, 5) << std::endl;

#ifdef _MSC_VER
	std::wcout.imbue(std::locale("chs"));
	std::wcout << boost::make_tuple(1, 2, 3, 4, 5) << std::endl;
#endif // _MSC_VER
}

typedef boost::tuple<int, int> tt_type;
void tmp_swap(tt_type& l, BOOST_RV_REF(tt_type) r)
//void tmp_swap(tt_type& l, const tt_type& r)
{
}

void test_swap(void)
{
	boost::tuple<int, int> t1(boost::make_tuple(1, 1));
	boost::tuple<int, int> t2(boost::make_tuple(2, 2));

	boost::tuple<int, int> t3(t1);
	boost::tuple<int, int> t4(t2);

	assert(t1 == t3);
	assert(t2 == t4);

	std::swap(t1, t4);
	boost::swap(t2, t3);

	assert(boost::get<0>(t1) == 2);
	assert(boost::get<0>(t4) == 1);
	assert(t1 == t3);
	assert(t2 == t4);


#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
	std::swap(t1, boost::make_tuple(3, 3));
	std::swap(boost::move(boost::make_tuple(3, 3)), t3);

	std::swap(t2, boost::make_tuple(4, 4));
	std::swap(boost::make_tuple(4, 4), t4);

	assert(t1 == t3);
	assert(t2 == t4);
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES
}

void test_unordered_container(void)
{
	typedef boost::tuple<int, int> tuple_type;
	typedef boost::unordered_set<tuple_type> tuple_set_type;

	tuple_set_type set;
	set.insert(boost::make_tuple(10, 10));
}

int main(int argc, char* argv[])
{
	test_ene();
	test_swap();
	test_unordered_container();

	char cc = 0;
	std::cin >> cc;
	return 0;
}
