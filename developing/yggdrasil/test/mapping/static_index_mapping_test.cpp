//static_index_mapping_test.cpp

#include <cassert>
#include <iostream>
#include <string>

#include <yggr/mapping/static_index_mapping.hpp>
#include <yggr/ppex/cast.hpp>
#include <yggr/ppex/cast_to_string.hpp>

struct tag_1 {};

YGGR_PP_STATIC_MAPPING_INDEXES_BEGIN(tag_1)
	YGGR_PP_STATIC_MAPPING_INDEX(test1)
	YGGR_PP_STATIC_MAPPING_INDEX(test2)
YGGR_PP_STATIC_MAPPING_INDEXES_END()

YGGR_PP_STATIC_MAPPING_VALUES_BEGIN(tag_1, const char*)
	YGGR_PP_STATIC_MAPPING_VALUE(YGGR_PP_CAST(test1, YGGR_PP_CAST_TO_STRING))
	YGGR_PP_STATIC_MAPPING_VALUE(YGGR_PP_CAST(test2, YGGR_PP_CAST_TO_STRING))
YGGR_PP_STATIC_MAPPING_VALUES_END()

YGGR_PP_STATIC_MAPPING_MAKE(YGGR_PP_GET_STATIC_MAPPING_INDEXES(tag_1), YGGR_PP_GET_STATIC_MAPPING_VALUES(tag_1))


int main(int argc, char* argv[])
{
	
	assert(YGGR_PP_GET_STATIC_MAPPING_INDEX(tag_1, test1) == 0);
	assert(YGGR_PP_GET_STATIC_MAPPING_INDEX(tag_1, test2) == 1);

	assert(std::string("test1") == YGGR_PP_GET_STATIC_MAPPING_VALUE(tag_1, test1));
	assert(std::string("test2") == YGGR_PP_GET_STATIC_MAPPING_VALUE(tag_1, test2));

	assert(YGGR_PP_GET_STATIC_MAPPING_SIZE(tag_1) == 2);


	for(YGGR_PP_GET_STATIC_MAPPING(tag_1)::const_iterator i = YGGR_PP_GET_STATIC_MAPPING(tag_1)::begin(),
			isize = YGGR_PP_GET_STATIC_MAPPING(tag_1)::end(); i != isize; ++i)
	{
		std::cout << *i << std::endl;
	}

	for(YGGR_PP_GET_STATIC_MAPPING(tag_1)::const_reverse_iterator i = YGGR_PP_GET_STATIC_MAPPING(tag_1)::rbegin(),
			isize = YGGR_PP_GET_STATIC_MAPPING(tag_1)::rend(); i != isize; ++i)
	{
		std::cout << *i << std::endl;
	}

	assert(std::string("test1") == YGGR_PP_GET_STATIC_MAPPING(tag_1)::front());
	assert(std::string("test2") == YGGR_PP_GET_STATIC_MAPPING(tag_1)::back());

	assert(!YGGR_PP_GET_STATIC_MAPPING(tag_1)::empty());
	assert(YGGR_PP_GET_STATIC_MAPPING(tag_1)::size() == 2);

	assert(std::string("test1") == YGGR_PP_GET_STATIC_MAPPING(tag_1)::at(0));
	assert(std::string("test2") == YGGR_PP_GET_STATIC_MAPPING(tag_1)::at(1));

	std::cout << "test end" << std::endl;
	char cc = 0;
	std::cin >> cc;
	return 0;
}