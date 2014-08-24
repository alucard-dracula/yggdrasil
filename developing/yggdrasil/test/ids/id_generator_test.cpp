//id_generator.cpp

#include <iostream>
#include <yggr/ids/base_ids_def.hpp>
#include <set>
#include <algorithm>
#include <functional>
#include <boost/bind.hpp>
#include <boost/progress.hpp>

#include <yggr/ids/inner_process_id.hpp>

#include <yggr/base/random.h>
#include <yggr/ids/uuid.hpp>

#ifdef _MSC_VER
#   include <vld.h>
#endif //_MSC_VER

void test_process_inner_id(void)
{
	//typedef yggr::u64 val_type;
	typedef yggr::ids::inner_process_id val_type;
	typedef yggr::ids::id_generator<val_type> gen_type;
	typedef std::set<val_type> set_type;

	//const yggr::size_type size = 10000000;
	//const yggr::size_type size = 100000;
	const yggr::size_type size = 100;

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
				std::cout << "same!!!" << std::endl;
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
	//typedef yggr::ids::id_generator<val_type> gen_type;
	typedef yggr::ids::id_generator<val_type, yggr::ids::parse_uuid_genner::lagged_fibonacci607_type> gen_type;
	typedef std::set<val_type> set_type;

	//const yggr::size_type size = 10000000;
	const yggr::size_type size = 100;

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
				std::cout << "same!!!" << std::endl;
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

#ifndef __MINGW32__
	std::wcout.imbue(std::locale("chs"));
	std::wcout << gen() << std::endl;
#endif // __MINGW32__

}

void test_process_inner_id2(void)
{
	typedef yggr::ids::inner_process_id val_type;
	typedef yggr::ids::id_generator<val_type> gen_type;

	gen_type gen;
	std::cout << gen() << std::endl;
#ifndef __MINGW32__
	std::wcout.imbue(std::locale("chs"));
	std::wcout << gen() << std::endl;
#endif // __MINGW32__
}

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

int main(int argc, char* argv[])
{
	yggr::random::init();

	test_process_inner_id();
	test_normal_id();
	test_uuid1();
	test_uuid2();

	test_process_inner_id2();

	char cc = 0;
	std::cin >> cc;
	return 0;
}
