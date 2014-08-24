//unique_mapping.cpp

#include <iostream>
#include <yggr/mapping/unique_mapping.hpp>

void test_mapping(void)
{
	typedef yggr::mapping::unique_mapping<int, float, long> mapping_type;
	mapping_type mapping;
	mapping.append(10, 20.0f, 30);
	mapping.append(50, 60.0f, 70);
	mapping.append(80, 90.0f, 100);

	{
		std::pair<float, bool> ret
			= mapping.mapping_value< mapping_type::value_type::arg<0>, mapping_type::value_type::arg<1> >(10);
		assert((ret.second == true && ret.first == 20.0f));
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

int main(int argc, char* argv[])
{
	test_mapping();
	char cc = 0;
	std::cin >> cc;
	return 0;
}
