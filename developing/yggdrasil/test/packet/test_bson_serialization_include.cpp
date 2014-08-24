//test_bson_serialization_include.cpp

// test 0, 1, 2
#define INCLUDE_BEFORE 2

#if (INCLUDE_BEFORE == 1 || INCLUDE_BEFORE == 2)
#include <yggr/serialization/collection_size_type.hpp>
#include <yggr/serialization/array.hpp>
#include <yggr/serialization/deque.hpp>
#include <yggr/serialization/list.hpp>
#include <yggr/serialization/vector.hpp>
#include <yggr/serialization/utility.hpp>
#include <yggr/serialization/string.hpp>
#include <yggr/serialization/utf8_string.hpp>
#include <yggr/serialization/set.hpp>
#include <yggr/serialization/map.hpp>
#include <yggr/serialization/unordered_set.hpp>
#include <yggr/serialization/unordered_map.hpp>
#include <yggr/serialization/multi_index_container.hpp>
#include <yggr/serialization/shared_ptr.hpp>
#include <yggr/serialization/tuple.hpp>
#endif // (INCLUDE_BEFORE == 1 || INCLUDE_BEFORE == 2)

#if (INCLUDE_BEFORE == 0 || INCLUDE_BEFORE == 1)
#include <yggr/archive/bson_oarchive.hpp>
#include <yggr/archive/bson_iarchive.hpp>
#endif // (INCLUDE_BEFORE == 0 || INCLUDE_BEFORE == 1)

#if INCLUDE_BEFORE == 0
#include <yggr/serialization/collection_size_type.hpp>
#include <yggr/serialization/array.hpp>
#include <yggr/serialization/deque.hpp>
#include <yggr/serialization/list.hpp>
#include <yggr/serialization/vector.hpp>
#include <yggr/serialization/utility.hpp>
#include <yggr/serialization/string.hpp>
#include <yggr/serialization/utf8_string.hpp>
#include <yggr/serialization/set.hpp>
#include <yggr/serialization/map.hpp>
#include <yggr/serialization/unordered_set.hpp>
#include <yggr/serialization/unordered_map.hpp>
#include <yggr/serialization/multi_index_container.hpp>
#include <yggr/serialization/shared_ptr.hpp>
#include <yggr/serialization/tuple.hpp>
#endif // INCLUDE_BEFORE == 0

#include <iostream>

int main(int argc, char* argv[])
{
	char cc = 0;
	std::cin >> cc;

	return 0;
};
