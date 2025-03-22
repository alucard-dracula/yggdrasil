//id_generator.cpp

#include <test/wait_any_key/wait_any_key.hpp>

#include <yggr/base/random.h>

#include <yggr/ids/base_ids_def.hpp>
#include <yggr/ids/inner_process_id.hpp>
#include <yggr/ids/uuid.hpp>

#include <boost/bind.hpp>
#include <boost/progress.hpp>

#include <set>
#include <sstream>
#include <algorithm>
#include <functional>
#include <typeinfo>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(ids)
#include YGGR_PP_LINK_LIB(time)
#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(base)

#if 0
static yggr::u64 g_test_size = 10000000;
#else
static yggr::u64 g_test_size = 100;
#endif // 0, 1

void test_process_inner_id(void)
{
	typedef yggr::ids::inner_process_id val_type;
	typedef yggr::ids::id_generator<val_type> gen_type;
	typedef std::set<val_type> set_type;

	const yggr::size_type size = g_test_size;

	gen_type gen;
	set_type set;

	std::cout << gen() << std::endl;

	boost::progress_display pd(size);

	{
		for(yggr::size_type i = 0; i != size; ++i, ++pd)
		{
			val_type val = gen();
			if(!set.insert(val).second)
			{
				set_type::iterator iter= set.find(val);

				val_type val2 = *iter;

				std::cout << (val == val2) << std::endl;

				std::size_t s1 = val.hash();
				std::size_t s2 = val2.hash();
				std::cout << "same!!! " << s1 << ", " << s2 << std::endl;
				break;
			}
		}
	}

	set.clear();
	std::cout << "ok" << std::endl;
}

void test_normal_id(void)
{
	std::cout <<((yggr::ids::create_id_generator<yggr::u32>(boost::bind(&yggr::random::gen_random<yggr::u32>)))())
				<< std::endl;

}

void test_uuid1(void)
{
	typedef yggr::ids::uuid val_type;
#if 0
	typedef yggr::ids::id_generator<val_type> gen_type;
#else
	typedef yggr::ids::id_generator<val_type, yggr::ids::parse_uuid_genner::lagged_fibonacci607_type> gen_type;
#endif // 0, 1
	typedef std::set<val_type> set_type;

	const yggr::size_type size = g_test_size;

	gen_type gen;
	set_type set;

	std::cout << typeid(gen()).name() << std::endl;
	boost::progress_display pd(size);

	{
		for(yggr::size_type i = 0; i != size; ++i, ++pd)
		{
			val_type val = gen();
			if(!set.insert(val).second)
			{
				set_type::iterator iter= set.find(val);

				val_type val2 = *iter;

				std::cout << (val == val2) << std::endl;

				std::size_t s1 = val.hash();
				std::size_t s2 = val2.hash();
				std::cout << "same!!! " << s1 << ", " << s2 << std::endl;
				break;
			}
		}
	}

	set.clear();
	std::cout << "ok" << std::endl;
}

void test_uuid2(void)
{
	typedef yggr::ids::uuid val_type;
	typedef yggr::ids::id_generator<val_type, yggr::ids::parse_uuid_genner::lagged_fibonacci607_type> gen_type;

	gen_type gen;

	std::cout << gen() << std::endl;

#ifndef YGGR_NO_CWCHAR
	std::wcout << gen() << std::endl;
#endif // YGGR_NO_CWCHAR
}

void test_uuid3(void)
{
	typedef yggr::ids::uuid val_type;
	typedef yggr::ids::id_generator<val_type, yggr::ids::parse_uuid_genner::lagged_fibonacci607_type> gen_type;

	gen_type gen;

	val_type val1 = gen();

	std::string str_uuid;
	std::stringstream ss;
	ss << val1;
	ss >> str_uuid;

	std::cout << str_uuid << std::endl;

	val_type val2;
	bool b = val_type::from_code(val2, str_uuid);
	yggr_test_assert(b);
	yggr_test_assert(val1 == val2);

}

void test_process_inner_id2(void)
{
	typedef yggr::ids::inner_process_id val_type;
	typedef yggr::ids::id_generator<val_type> gen_type;

	gen_type gen;
	std::cout << gen() << std::endl;

#ifndef YGGR_NO_CWCHAR
	std::wcout << gen() << std::endl;
#endif // YGGR_NO_CWCHAR
}

#include <yggr/smart_ptr_ex/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

int main(int argc, char* argv[])
{

#ifndef YGGR_NO_CWCHAR
	TEST_PP_WIO_IMBUE("chs");
#endif // YGGR_NO_CWCHAR

	yggr::random::init();

	test_process_inner_id();
	test_normal_id();
	test_uuid1();
	test_uuid2();
	test_uuid3();

	test_process_inner_id2();

	wait_any_key(argc, argv);
	return 0;
}
