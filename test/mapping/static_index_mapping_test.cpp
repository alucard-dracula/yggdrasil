//static_index_mapping_test.cpp

#include <cassert>
#include <test/wait_any_key/wait_any_key.hpp>
#include <string>

#include <yggr/mapping/static_index_mapping.hpp>
#include <yggr/ppex/cast.hpp>
#include <yggr/ppex/cast_to_string.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

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
	
	yggr_test_assert(YGGR_PP_GET_STATIC_MAPPING_INDEX(tag_1, test1) == 0);
	yggr_test_assert(YGGR_PP_GET_STATIC_MAPPING_INDEX(tag_1, test2) == 1);

	yggr_test_assert(std::string("test1") == YGGR_PP_GET_STATIC_MAPPING_VALUE(tag_1, test1));
	yggr_test_assert(std::string("test2") == YGGR_PP_GET_STATIC_MAPPING_VALUE(tag_1, test2));

	yggr_test_assert(YGGR_PP_GET_STATIC_MAPPING_SIZE(tag_1) == 2);


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

	yggr_test_assert(std::string("test1") == YGGR_PP_GET_STATIC_MAPPING(tag_1)::front());
	yggr_test_assert(std::string("test2") == YGGR_PP_GET_STATIC_MAPPING(tag_1)::back());

	yggr_test_assert(!YGGR_PP_GET_STATIC_MAPPING(tag_1)::empty());
	yggr_test_assert(YGGR_PP_GET_STATIC_MAPPING(tag_1)::size() == 2);

	yggr_test_assert(std::string("test1") == YGGR_PP_GET_STATIC_MAPPING(tag_1)::at(0));
	yggr_test_assert(std::string("test2") == YGGR_PP_GET_STATIC_MAPPING(tag_1)::at(1));

	wait_any_key(argc, argv);
	return 0;
}