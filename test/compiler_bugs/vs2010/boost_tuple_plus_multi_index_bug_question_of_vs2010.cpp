//boost_tuple_plus_multi_index_question_bug_of_vs2010.cpp

#include <cassert>
#include <iostream>
#include <boost/tuple/tuple.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/indexed_by.hpp>
#include <boost/multi_index/member.hpp> 
#include <boost/multi_index/mem_fun.hpp>  
#include <boost/multi_index_container.hpp>

typedef boost::tuple<int, float, int> tuple_type;

struct tag1 {};
struct tag2 {};
struct tag3 {};

typedef boost::multi_index::multi_index_container<
		tuple_type,
		boost::multi_index::indexed_by<
			boost::multi_index::hashed_non_unique<
				boost::multi_index::tag<tag1>,
				BOOST_MULTI_INDEX_CONST_MEM_FUN( tuple_type::inherited, const int&, get<0> )
			>,

			boost::multi_index::hashed_non_unique<
				boost::multi_index::tag<tag2>,
				BOOST_MULTI_INDEX_CONST_MEM_FUN( tuple_type::inherited, const float&, get<1> )
			>,

			boost::multi_index::hashed_non_unique<
				boost::multi_index::tag<tag3>,
				BOOST_MULTI_INDEX_CONST_MEM_FUN( tuple_type::inherited, const int&, get<2> )
			>
		>
	>  midx_type;


void test_error(void)
{
	midx_type midx;
	
	tuple_type tuple(boost::make_tuple(1, 2.0f, 3));
	const tuple_type& cref_tuple = tuple;
	assert(cref_tuple.get<0>() == 1);
	assert(cref_tuple.get<1>() == 2.0f);
	assert(cref_tuple.get<2>() == 3);

	midx.insert(tuple);

	assert((midx.get<0>().find(1) != midx.get<0>().end()));
	assert((midx.get<1>().find(2.0f) != midx.get<1>().end()));
	assert((midx.get<2>().find(3) != midx.get<2>().end())); // assert failed why?
}

int main(int argc, char* argv[])
{
	test_error();

	char cc = 0;
	std::cin >> cc;
	return 0;
}