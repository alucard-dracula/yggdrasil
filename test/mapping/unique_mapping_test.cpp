//unique_mapping.cpp

//#define YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION

#include <test/wait_any_key/wait_any_key.hpp>
#include <boost/thread/thread.hpp>
#include <boost/array.hpp>
#include <yggr/mapping/unique_mapping.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

void test_mapping(void)
{
	typedef yggr::mapping::unique_mapping<int, float, long> mapping_type;
	mapping_type mapping;
	mapping.append(10, 20.0f, 30);
	mapping.append(50, 60.0f, 70);
	mapping.append(80, 90.0f, 100);
	bool bright = mapping.append(10, 300.0f, 30);
	yggr_test_assert(!bright);

	bright = mapping.compulsory_append(100, 3000.0f, 300);
	yggr_test_assert(bright);

	bright = mapping.append(100, 3000.0f, 300);
	yggr_test_assert(!bright);

	bright = mapping.compulsory_append(100, 5000.0f, 300);
	yggr_test_assert(bright);

	bright = mapping.compulsory_append(100, 5000.0f, 300);
	yggr_test_assert(bright);

	bright = mapping.compulsory_append(10, 5000.0f, 300);
	yggr_test_assert(!bright);

	{
		int a = 100;
		float b = 6000;
		long c = 300;

		bright = mapping.compulsory_exchange(a, b, c);
		yggr_test_assert(bright);

		yggr_test_assert(a == 100 && b == 5000.0f && c == 300);
	}

	{
		int a = 10;
		float b = 6000;
		long c = 300;

		bright = mapping.compulsory_exchange(a, b, c);
		yggr_test_assert(!bright);

		yggr_test_assert(a == 10 && b == 6000.0f && c == 300);
	}

	{
		std::pair<float, bool> ret
			= mapping.mapping_value< mapping_type::value_type::arg<0>, mapping_type::value_type::arg<1> >(10);
		yggr_test_assert((ret.second == true && ret.first == 20.0f));
	}

	{
		std::pair<float, bool> ret
			= mapping.mapping_value< mapping_type::value_type::arg<0>, mapping_type::value_type::arg<1> >(100);
		yggr_test_assert((ret.second == true && ret.first == 6000.0f));
	}

	{
		std::pair<float, bool> ret
			= mapping.mapping_value<0, 1>(80);
		yggr_test_assert((ret.second == true && ret.first == 90.0f));
	}

	{
		std::pair<long, bool> ret
			= mapping.mapping_value<1, 2>(60.0f);
		yggr_test_assert((ret.second == true && ret.first == 70));
	}

	{
		std::vector<float> vt
			= mapping.mapping_other_values<0, 1, std::vector<float> >(10);

		yggr_test_assert(vt.size() == 3);

		for(std::vector<float>::const_iterator i = vt.begin(), isize = vt.end();
				i != isize; ++i)
		{
			yggr_test_assert(*i != 20.0f);
			yggr_test_assert((*i == 60.0f || *i == 90.0f || *i == 6000.0f));
		}

	}

	{
		bool b = mapping.modify< mapping_type::value_type::arg<0>, mapping_type::value_type::arg<1> >(80, 200.0f);
		yggr_test_assert(b);
		std::pair<float, bool> ret
			= mapping.mapping_value<0, 1>(80);
		yggr_test_assert((ret.second == true && ret.first == 200.0f));
	}

	{
		bool b = mapping.modify< 0, 1 >(10, 200.0f);
		yggr_test_assert(!b);
		std::pair<float, bool> ret
			= mapping.mapping_value<0, 1>(10);
		yggr_test_assert((ret.second == true && ret.first == 20.0f));
	}

	{
		float val = 400.0f;
		bool b = mapping.exchange< mapping_type::value_type::arg<0>, mapping_type::value_type::arg<1> >(80, val);
		yggr_test_assert(b);
		std::pair<float, bool> ret
			= mapping.mapping_value<0, 1>(80);
		yggr_test_assert((ret.second == true && ret.first == 400.0f && val == 200.0f));
	}

	{
		float val = 400.0f;
		bool b = mapping.exchange< 0, 1 >(10, val);
		yggr_test_assert(!b);
		std::pair<float, bool> ret
			= mapping.mapping_value<0, 1>(10);
		yggr_test_assert((ret.second == true && ret.first == 20.0f && val == 400.0f));
	}

	{
		bool b = mapping.remove< mapping_type::value_type::arg<0> >(10);
		yggr_test_assert(b);
		bool b2 = mapping.remove<1>(100.0f);
		yggr_test_assert(!b2);
		std::pair<float, bool> ret
			= mapping.mapping_value<mapping_type::value_type::arg<0>, mapping_type::value_type::arg<1> >(10);
		yggr_test_assert((ret.second == false));
	}

	{
		mapping_type tmp;
		yggr_test_assert(tmp.empty());
		yggr_test_assert(!mapping.empty());

		yggr::swap(tmp, mapping);

		yggr_test_assert(!tmp.empty());
		yggr_test_assert(mapping.empty());

		yggr::swap(tmp, mapping);

		yggr_test_assert(tmp.empty());
		yggr_test_assert(!mapping.empty());
	}

	{
		mapping.clear();
		yggr_test_assert(0 == mapping.size());
	}

	std::cout << "test_mapping end" << std::endl;
}

void test_compulsory_insert(void)
{
	typedef yggr::mapping::unique_mapping<int, float, long> mapping_type;
	mapping_type mapping;

	bool bright = false;
	int arr0[] = {1, 2, 3, 1, 2, 3};
	float arr1[] = {1.0f, 2.0f, 3.0f, 1.0f, 2.0f, 3.0f};
	long arr2[] = {4, 5, 6, 7, 8, 9};
	for(int i = 0; i != 10000; ++i)
	{
		int idx = i % 6;
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

				if(bright)
				{
					std::pair<long, bool> chk = mapping.mapping_value<1, 2>(arr1[idx]);
					yggr_test_assert(chk.first == arr2[idx]);
					yggr_test_assert(chk.second );
				}
			}

			//if(!bright)
			//{
			//	int size = mapping.size();
			//	int nnn = 0;
			//}
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
		int idx = i % 6;
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

			//if(!bright) //check error, debug break pointer point
			//{
			//	int size = mapping.size();
			//	int nnn = 0;
			//}
		} while(!bright) ;

		yggr_test_assert(bright);
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
		int idx = i % 6;
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

			//if(!bright)  //check error, debug break pointer point
			//{
			//	int size = mapping.size();
			//	int nnn = 0;
			//	yggr_test_assert(false);
			//}
		} while(!bright) ;
		yggr_test_assert(bright);
	}
}

void test_compulsory_insert_thread(void)
{
	typedef yggr::mapping::unique_mapping<int, float, long> mapping_type;
	mapping_type mapping;

	boost::thread trd1(boost::bind(&test_1, boost::ref(mapping)));
	boost::thread trd2(boost::bind(&test_2, boost::ref(mapping)));

	trd1.join();
	trd2.join();

	yggr_test_assert(mapping.size() == 3);

	std::cout << "end2" << std::endl;
}

int main(int argc, char* argv[])
{
	test_mapping();
	test_compulsory_insert();
	test_compulsory_insert_thread();
	
	wait_any_key(argc, argv);
	return 0;
}
