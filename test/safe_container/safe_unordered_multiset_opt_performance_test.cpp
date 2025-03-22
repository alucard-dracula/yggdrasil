//safe_unordered_multiset_opt_performance_test.cpp

//#define YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION

#include <test/wait_any_key/wait_any_key.hpp>
#include <cassert>
#include <functional>
#include <vector>
#include <boost/bind.hpp>

#include <yggr/thread/boost_thread.hpp>
#include <yggr/time/time.hpp>
#include <yggr/time/runtime_performance_tester.hpp>

#include <yggr/utility/swap.hpp>
#include <yggr/safe_container/safe_unordered_set.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

#include YGGR_PP_LINK_LIB(base)
#include YGGR_PP_LINK_LIB(time)
#include YGGR_PP_LINK_LIB(charset)

//-----------------------test safe---------------------------------------------

typedef std::string value_type;
typedef yggr::safe_container::safe_unordered_multiset<value_type> set_type;
typedef std::vector<value_type> vt_type;
typedef std::vector<value_type> key_vt_type;


typedef boost::unordered_multiset<int> tmp_int_set_type;
typedef boost::unordered_multiset<std::string> tmp_string_set_type;

//#ifndef YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION
//
//typedef yggr::safe_container::detail::cast_to_ucontainer_opt<tmp_int_set_type>::type tmp_int_set_opt_type;
//typedef yggr::safe_container::detail::cast_to_ucontainer_opt<tmp_string_set_type>::type tmp_string_set_opt_type;
//
//#endif // YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION

typedef yggr::time::runtime_performance_tester<yggr::time::time> test_type;

namespace now_test
{

const int hash_time_test_size = 100000;
const int string_key_size = 500;

} // namespace now_test

//#ifndef YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION
//
//void test_hash_time_of_int(void)
//{
//
//	typedef tmp_int_set_type::hasher hasher_type;
//
//	std::cout << "test_hash_time_of_int_start ..." << std::endl;
//
//	tmp_int_set_type set;
//	hasher_type hasher = set.hash_function();
//	int val = rand();
//
//	std::size_t result[now_test::hash_time_test_size] = {0};
//	int test_size = now_test::hash_time_test_size;
//	{
//		//test_type test(test_size);
//		test_type test(1);
//		for(int i = 0; i != test_size; ++i)
//		{
//			result[i] = tmp_int_set_opt_type::s_hash(hasher, val);
//		}
//	}
//
//	std::cout << "test_hash_time_of_int_end ..." << std::endl;
//}
//
//void test_hash_time_of_string(void)
//{
//	typedef tmp_string_set_opt_type::hasher hasher_type;
//
//	std::cout << "test_hash_time_of_string_start ..." << std::endl;
//
//	tmp_string_set_type set;
//	hasher_type hasher = set.hash_function();
//	std::string val(now_test::string_key_size, 'a');
//
//	std::size_t result[now_test::hash_time_test_size] = {0};
//	int test_size = now_test::hash_time_test_size;
//	{
//		//test_type test(test_size);
//		test_type test(1);
//		for(int i = 0; i != test_size; ++i)
//		{
//			result[i] = tmp_string_set_opt_type::s_hash(hasher, val);
//		}
//	}
//
//	std::cout << "test_hash_time_of_string_end ..." << std::endl;
//}
//
//void test_hash_time_of_string2(void)
//{
//	typedef tmp_string_set_opt_type::hasher hasher_type;
//
//	std::cout << "test_hash_time_of_string2_start ..." << std::endl;
//
//	tmp_string_set_type set;
//	hasher_type hasher = set.hash_function();
//	std::string val(now_test::string_key_size / 2, 'a');
//
//	std::size_t result[now_test::hash_time_test_size] = {0};
//	int test_size = now_test::hash_time_test_size;
//	{
//		//test_type test(test_size);
//		test_type test(1);
//		for(int i = 0; i != test_size; ++i)
//		{
//			result[i] = tmp_string_set_opt_type::s_hash(hasher, val);
//		}
//	}
//
//	std::cout << "test_hash_time_of_string2_end ..." << std::endl;
//}
//
//#endif // YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION

void test_safe_hash_set_insert(void)
{ 
	set_type set;

	std::string key(now_test::string_key_size, 'a');

	// count_size == 26 * 500 ==13000
	{
		test_type test(1);
		for(int i = 0, isize = now_test::string_key_size; i != isize; ++i)
		{
			for(int j = 0, jsize = 'z' - 'a' + 1; j != jsize; ++j)
			{
				key[i] = 'a' + j;
				set.insert(key);
			}
		}
	}
}


template<typename Iter>
void insert_of_multi_thread(set_type& set, Iter s, Iter e)
{
	for(int i = 0; s != e; ++i, ++s)
	{
		set.insert(*s);
	}
}

template<typename Iter>
void find_of_multi_thread(set_type& set, Iter s, Iter e)
{
	//std::vector<bool> vt(std::distance(s, e), false);
	bool bright = false;
	for(int i = 0; s != e; ++i, ++s)
	{
		bright = set.is_exists(*s);
	}
}

void test_safe_hash_set_insert_mthread(void)
{ 
	int test_size = 10;
	int trd_size = 4;
	
	int count_size = 26 * now_test::string_key_size * test_size;

	key_vt_type key_vt(count_size, std::string());
	set_type set;

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
												boost::ref(set), 
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
												boost::ref(set), 
												key_iter + i * sp_size, 
												key_iter + (i + 1) * sp_size));
		}
		trds2.join_all();
	}
}


int main(int argc, char* argv[])
{
	std::cout << "---------------" << std::endl;
	test_safe_hash_set_insert_mthread();

	wait_any_key(argc, argv);
	return 0;
}
