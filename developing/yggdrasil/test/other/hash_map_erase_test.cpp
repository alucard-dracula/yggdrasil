//hash_map_erase_test.cpp


#include <iostream>
#include <boost/unordered_map.hpp>

#ifdef _MSC_VER
#include <vld.h>
#endif //_MSC_VER

typedef boost::unordered_map<int, int> map_type;
typedef map_type::iterator map_iter_type;
typedef map_type::const_iterator map_citer_type;

int main(int argc, char* argv[])
{

	map_type map;
	map[0] = 2;
	map[1] = 1;
	map[2] = 4;
	map[3] = 1;
	map[5] = 3;

	for(map_iter_type i = map.begin(), isize = map.end(), rm; i!= isize;)
	{
		if(i->second == 1)
		{	rm = i;
			++i;
			map.erase(rm);
		}
		else
		{
			++i;
		}
	}


	for(map_iter_type j = map.begin(), jsize = map.end(); j != jsize; ++j)
	{
		std::cout << j->second << std::endl;
	}

	char cc = 0;
	std::cin >> cc;
	return 0;
}
