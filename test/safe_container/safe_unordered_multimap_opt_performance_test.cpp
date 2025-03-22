//safe_unordered_multimap_opt_performance_test.cpp

// #define YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION

#include <test/wait_any_key/wait_any_key.hpp>
#include <cassert>
#include <functional>
#include <vector>
#include <boost/bind.hpp>

#include <yggr/thread/boost_thread.hpp>
#include <yggr/time/time.hpp>
#include <yggr/time/runtime_performance_tester.hpp>

#include <yggr/utility/swap.hpp>
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
typedef yggr::safe_container::safe_unordered_multimap<std::string, int> map_type;
typedef std::vector<value_type> vt_type;
typedef std::vector<std::string> key_vt_type;


typedef boost::unordered_map<int, int> tmp_int_map_type;
typedef boost::unordered_map<std::string, int> tmp_string_map_type;

typedef yggr::time::runtime_performance_tester<yggr::time::time> test_type;

namespace now_test
{

const int hash_time_test_size = 100000;
const int string_key_size = 500;

} // namespace now_test

void test_safe_hash_mmap_insert(void)
{
	map_type map;

	std::string key(now_test::string_key_size, 'a');

	// count_size == 26 * 500 ==13000
	{
		test_type test(1);
		for(int i = 0, isize = now_test::string_key_size; i != isize; ++i)
		{
			for(int j = 0, jsize = 'z' - 'a' + 1; j != jsize; ++j)
			{
				key[i] = 'a' + j;
				map.insert(key, (i + 1) * (j + 1));
			}
		}
	}
}

template<typename Iter>
void insert_of_multi_thread(map_type& map, Iter s, Iter e)
{
	for(int i = 0; s != e; ++i, ++s)
	{
		map.insert(*s, i);
	}
}

template<typename Iter>
void find_of_multi_thread(map_type& map, Iter s, Iter e)
{
	bool bright = false;
	for(int i = 0; s != e; ++i, ++s)
	{
		bright = map.is_exists(*s);
	}
}

void test_safe_hash_mmap_insert_mthread(void)
{
	int test_size = 10;
	int trd_size = 4;

	int count_size = 26 * now_test::string_key_size * test_size;

	key_vt_type key_vt(count_size, std::string());
	map_type map;

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
			trds.create_thread(boost::bind(&insert_of_multi_thread<key_vt_type::const_iterator>,
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
			trds2.create_thread(boost::bind(&find_of_multi_thread<key_vt_type::const_iterator>,
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
	test_safe_hash_mmap_insert_mthread();

	wait_any_key(argc, argv);
	return 0;
}
