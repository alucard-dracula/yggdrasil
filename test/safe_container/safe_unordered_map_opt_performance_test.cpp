//safe_unordered_map_opt_performance_test.cpp

//#define YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION
////#define YGGR_SAFE_CONTAINER_NOT_USING_ALLOCATOR_OPT

#include <test/wait_any_key/wait_any_key.hpp>
#include <cassert>
#include <functional>

#include <boost/bind.hpp>

#include <yggr/thread/boost_thread.hpp>
#include <yggr/time/time.hpp>
#include <yggr/time/runtime_performance_tester.hpp>

#include <yggr/utility/swap.hpp>
#include <yggr/charset/string.hpp>
#include <yggr/container/vector.hpp>
#include <yggr/safe_container/safe_unordered_map.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

#include YGGR_PP_LINK_LIB(base)
#include YGGR_PP_LINK_LIB(time)
#include YGGR_PP_LINK_LIB(charset)


//-----------------------test safe---------------------------------------------

typedef std::pair<std::string, int> value_type;

typedef std::vector<value_type> vt_type;
typedef std::vector<std::string> key_vt_type;

typedef yggr::time::runtime_performance_tester<yggr::time::time> test_type;

namespace now_test
{

const int string_key_size = 500;

} // namespace now_test

template<typename SafeCont, typename Iter>
void insert_of_multi_thread(SafeCont& cont, Iter s, Iter e)
{
	for(int i = 0; s != e; ++i, ++s)
	{
		cont.insert(*s, i);
	}
}

template<typename SafeCont, typename Iter>
void find_of_multi_thread(SafeCont& cont, Iter s, Iter e)
{
	bool bright = false;
	for(int i = 0; s != e; ++i, ++s)
	{
		bright = cont.is_exists(*s);
	}
}

template<typename SafeCont>
void test_safe_hash_map_insert_mthread(void)
{
	typedef SafeCont safe_cont_type;

	int test_size = 10;
	int trd_size = 4;

	int count_size = 26 * now_test::string_key_size * test_size;

	key_vt_type key_vt(count_size, std::string());
	safe_cont_type map;

	std::string key(now_test::string_key_size, 'a');

	// count_size == 26 * 500 * 10 ==130000

	//int max = 0;
	for(int i = 0, isize = test_size; i != isize; ++i)
	{
		for(int j = 0, jsize = now_test::string_key_size; j != jsize; ++j)
		{
			for(int k = 0, ksize = 'z' - 'a' + 1; k != ksize; ++k)
			{
				key[j] = 'a' + k;
				key_vt[(i * jsize + j) * ksize + k] = key;
			}
		}
	}

	yggr_test_assert(key_vt.size() == count_size);
	yggr_test_assert(!(key_vt.back().empty()));

	boost::thread_group trds;
	key_vt_type::const_iterator key_iter = key_vt.begin();

	int sp_size = count_size / trd_size;
	yggr_test_assert(sp_size * trd_size == count_size);

	{
		test_type test(1);

		for(int i = 0, isize = trd_size; i != isize; ++i)
		{
			trds.create_thread(boost::bind(&insert_of_multi_thread<safe_cont_type, key_vt_type::const_iterator>,
												boost::ref(map),
												key_iter + i * sp_size,
												key_iter + (i + 1) * sp_size));
		}
		trds.join_all();
	}

	boost::thread_group trds2;

	{
		test_type test(1);

		for(int i = 0, isize = trd_size; i != isize; ++i)
		{
			trds2.create_thread(boost::bind(&find_of_multi_thread<safe_cont_type, key_vt_type::const_iterator>,
												boost::ref(map),
												key_iter + i * sp_size,
												key_iter + (i + 1) * sp_size));
		}
		trds2.join_all();
	}
}


int main(int argc, char* argv[])
{
	std::cout << "---------------" << std::endl;

	{
		typedef yggr::safe_container::safe_unordered_map<std::string, int> map_type;
		test_safe_hash_map_insert_mthread<map_type>();
	}

#if !defined(YGGR_NO_CXX11_HDR_UNORDERED_MAP)
	{
		typedef
			yggr::safe_container::safe_unordered_map
			<
				std::string, int,
				boost::mutex,
#   if defined(__GNUC__)
#       if (YGGR_CPP_VER_17 <= YGGR_CPP_VERSION)
				// in GCC CPP17 and later stl, hashes are split into fast and non-fast,
                //  fast hash and non-fast hash do not support merge operations
                std::hash<std::string>,
#       else
                boost::hash<std::string>,
#       endif // (YGGR_CPP_VER_17 <= YGGR_CPP_VERSION)
#   else
                std::hash<std::string>,
#   endif // __GNUC__
				std::equal_to<std::string>,
				std::allocator<std::pair<const std::string, int> >,
				std::unordered_map
			> map_type;
		test_safe_hash_map_insert_mthread<map_type>();
	}
#endif // YGGR_NO_CXX11_HDR_UNORDERED_MAP

	wait_any_key(argc, argv);
	return 0;
}
