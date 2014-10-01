//unique_mapping.cpp

#include <iostream>
#include <boost/thread/thread.hpp>
#include <boost/array.hpp>
#include <yggr/mapping/unique_mapping.hpp>

void test_mapping(void)
{
	typedef yggr::mapping::unique_mapping<int, float, long> mapping_type;
	mapping_type mapping;
	mapping.append(10, 20.0f, 30);
	mapping.append(50, 60.0f, 70);
	mapping.append(80, 90.0f, 100);
	bool bright = mapping.append(10, 300.0f, 30);
	assert(!bright);

	bright = mapping.must_append(100, 3000.0f, 300);
	assert(bright);

	bright = mapping.append(100, 3000.0f, 300);
	assert(!bright);

	bright = mapping.must_append(100, 5000.0f, 300);
	assert(bright);

	bright = mapping.must_append(100, 5000.0f, 300);
	assert(bright);

	bright = mapping.must_append(10, 5000.0f, 300);
	assert(!bright);

	{
		std::pair<float, bool> ret
			= mapping.mapping_value< mapping_type::value_type::arg<0>, mapping_type::value_type::arg<1> >(10);
		assert((ret.second == true && ret.first == 20.0f));
	}

	{
		std::pair<float, bool> ret
			= mapping.mapping_value< mapping_type::value_type::arg<0>, mapping_type::value_type::arg<1> >(100);
		assert((ret.second == true && ret.first == 5000.0f));
	}

	{
		std::pair<float, bool> ret
			= mapping.mapping_value<0, 1>(80);
		assert((ret.second == true && ret.first == 90.0f));
	}

	{
		std::pair<long, bool> ret
			= mapping.mapping_value<1, 2>(60.0f);
		assert((ret.second == true && ret.first == 70));
	}

	{
		bool b = mapping.modify< mapping_type::value_type::arg<0>, mapping_type::value_type::arg<1> >(80, 200.0f);
		assert(b);
		std::pair<float, bool> ret
			= mapping.mapping_value<0, 1>(80);
		assert((ret.second == true && ret.first == 200.0f));
	}

	{
		bool b = mapping.modify< 0, 1 >(10, 200.0f);
		assert(!b);
		std::pair<float, bool> ret
			= mapping.mapping_value<0, 1>(10);
		assert((ret.second == true && ret.first == 20.0f));
	}

	{
		bool b = mapping.remove< mapping_type::value_type::arg<0> >(10);
		assert(b);
		bool b2 = mapping.remove<1>(100.0f);
		assert(!b2);
		std::pair<float, bool> ret
			= mapping.mapping_value<mapping_type::value_type::arg<0>, mapping_type::value_type::arg<1> >(10);
		assert((ret.second == false));
	}

	{
		mapping.clear();
		assert(0 == mapping.size());
	}

	std::cout << "test_mapping end" << std::endl;
}

void test_must_insert(void)
{
	typedef yggr::mapping::unique_mapping<int, float, long> mapping_type;
	mapping_type mapping;

	bool bright = false;
	int arr0[] = {1, 2, 3, 1, 2, 3};
	float arr1[] = {1.0f, 2.0f, 3.0f, 1.0f, 2.0f, 3.0f};
	long arr2[] = {4, 5, 6, 7, 8, 9};
	for(int i = 0; i != 10000; ++i)
	{
		int idx = i % 7;
		std::pair<long, bool> rst;
		do
		{
			rst = mapping.mapping_value<1, 2>(arr1[idx]);
			if(rst.second)
			{
				bright = mapping.modify<1, 0, 2>(arr1[idx], arr0[idx], arr2[idx]);
			}
			else
			{
				bright = mapping.append(arr0[idx], arr1[idx], arr2[idx]);
			}

			if(!bright)
			{
				int size = mapping.size();
				int nnn = 0;
			}
		} while(!bright) ;
	}

	std::cout << "end1" << std::endl;
}

void test_1(yggr::mapping::unique_mapping<int, float, long>& mapping)
{
	bool bright = false;
	int arr0[] = {1, 2, 3, 1, 2, 3};
	float arr1[] = {1.0f, 2.0f, 3.0f, 1.0f, 2.0f, 3.0f};
	long arr2[] = {4, 5, 6, 7, 8, 9};
	for(int i = 0; i != 10000; ++i)
	{
		int idx = i % 7;
		std::pair<long, bool> rst;
		do
		{
			rst = mapping.mapping_value<1, 2>(arr1[idx]);
			if(rst.second)
			{
				bright = mapping.modify<1, 0, 2>(arr1[idx], arr0[idx], arr2[idx]);
			}
			else
			{
				bright = mapping.append(arr0[idx], arr1[idx], arr2[idx]);
			}

			if(!bright) //check error, debug break pointer point
			{
				int size = mapping.size();
				int nnn = 0;
			}
		} while(!bright) ;
	}
}

void test_2(yggr::mapping::unique_mapping<int, float, long>& mapping)
{
	bool bright = false;
	int arr0[] = {1, 2, 3, 1, 2, 3};
	float arr1[] = {1.0f, 2.0f, 3.0f, 1.0f, 2.0f, 3.0f};
	long arr2[] = {10, 11, 12, 13, 14, 15};
	for(int i = 0; i != 10000; ++i)
	{
		int idx = i % 7;
		std::pair<long, bool> rst;
		do
		{
			rst = mapping.mapping_value<1, 2>(arr1[idx]);
			if(rst.second)
			{
				bright = mapping.modify<1, 0, 2>(arr1[idx], arr0[idx], arr2[idx]);
			}
			else
			{
				bright = mapping.append(arr0[idx], arr1[idx], arr2[idx]);
			}

			if(!bright)  //check error, debuf break pointer point
			{
				int size = mapping.size();
				int nnn = 0;
			}
		} while(!bright) ;
	}
}

void test_must_insert_thread(void)
{
	typedef yggr::mapping::unique_mapping<int, float, long> mapping_type;
	mapping_type mapping;

	boost::thread trd1(boost::bind(&test_1, boost::ref(mapping)));
	boost::thread trd2(boost::bind(&test_2, boost::ref(mapping)));

	trd1.join();
	trd2.join();

	std::cout << "end2" << std::endl;
}

int main(int argc, char* argv[])
{
	test_mapping();
	test_must_insert();
	test_must_insert_thread();
	char cc = 0;
	std::cin >> cc;
	return 0;
}
