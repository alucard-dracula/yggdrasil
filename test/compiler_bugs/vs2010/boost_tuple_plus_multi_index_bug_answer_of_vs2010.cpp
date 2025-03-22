//test.cpp

#include <cassert>
#include <iostream>
#include <boost/tuple/tuple.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/indexed_by.hpp>
//#include <boost/multi_index/mem_fun.hpp> 
#include <boost/multi_index/global_fun.hpp> 
#include <boost/multi_index_container.hpp>

typedef boost::tuple<int, float, int> tuple_type;
typedef const int& (tuple_type::*tuple_get_int_foo_type)(void) const;

static tuple_get_int_foo_type pfoo_tget_int0 = &tuple_type::get<0>;
static tuple_get_int_foo_type pfoo_tget_int2 = &tuple_type::get<2>;


struct tag1 { enum {value = 0}; typedef const int& cref_value_type; };
struct tag2 { enum {value = 1}; typedef const float& cref_value_type; };
struct tag3 { enum {value = 2}; typedef const int& cref_value_type; };

template<typename Tag, typename Tuple>
typename Tag::cref_value_type tuple_get(const Tuple& tuple)
{
	return boost::get<Tag::value>(tuple);
}

//typedef boost::multi_index::multi_index_container<
//		tuple_type,
//		boost::multi_index::indexed_by<
//			boost::multi_index::hashed_non_unique<
//				boost::multi_index::tag<tag1>,
//				BOOST_MULTI_INDEX_CONST_MEM_FUN( tuple_type::inherited, const int&, get<0> )
//			>,
//
//			boost::multi_index::hashed_non_unique<
//				boost::multi_index::tag<tag2>,
//				BOOST_MULTI_INDEX_CONST_MEM_FUN( tuple_type::inherited, const float&, get<1> )
//			>,
//
//			boost::multi_index::hashed_non_unique<
//				boost::multi_index::tag<tag3>,
//				BOOST_MULTI_INDEX_CONST_MEM_FUN( tuple_type::inherited, const int&, get<2> )
//			>
//		>
//	>  midx_type;

typedef boost::multi_index::multi_index_container<
		tuple_type,
		boost::multi_index::indexed_by<
			boost::multi_index::hashed_non_unique<
				boost::multi_index::tag<tag1>,
				boost::multi_index::global_fun<const tuple_type&, const int&, &tuple_get<tag1, tuple_type> >
			>,

			boost::multi_index::hashed_non_unique<
				boost::multi_index::tag<tag2>,
				boost::multi_index::global_fun<const tuple_type&, const float&, &tuple_get<tag2, tuple_type> >
			>,

			boost::multi_index::hashed_non_unique<
				boost::multi_index::tag<tag3>,
				boost::multi_index::global_fun<const tuple_type&, const int&, &tuple_get<tag3, tuple_type> >
			>
		>
	>  midx_type;


//void error_check(void)
//{
//	BOOST_MULTI_INDEX_CONST_MEM_FUN( tuple_type::inherited, const int&, get<0> ) p1;
//	BOOST_MULTI_INDEX_CONST_MEM_FUN( tuple_type::inherited, const int&, get<2> ) p2;
//
//	p1(tuple); // debug here check this bug
//	p2(tuple);
//}

void error_answer(void)
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
	assert((midx.get<2>().find(3) != midx.get<2>().end()));
}

int main(int argc, char* argv[])
{
	error_answer();

	char cc = 0;
	std::cin >> cc;
	return 0;
}