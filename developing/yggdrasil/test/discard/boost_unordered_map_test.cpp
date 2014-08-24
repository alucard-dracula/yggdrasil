//unicode_utf8.cpp

#include <iostream>
#include <string>

#include <algorithm>
#include <boost/unordered_map.hpp>

#include <vector>

#ifdef WIN32
#   ifdef _DEBUG
#	    include <vld.h>
#   endif // _DEBUG
#endif //WIN32

typedef boost::unordered_map<int, int> map_type;
typedef map_type::value_type map_val_type;
typedef map_type::iterator map_iter_type;
typedef std::vector<map_iter_type> map_iter_vt_type;

int main(int argc, char* argv[])
{
	map_type map;
	map_iter_vt_type vt;

	{
	for(int i = 0; i < 10000; ++i)
	{
		std::pair<map_iter_type, bool> ins_rst = map.insert(map_val_type(i, i));

		if(ins_rst.second)
		{
			vt.push_back(ins_rst.first);
		}
		else
		{
			std::cout << "error" << std::endl;
		}

	}
	}
	map.erase(vt.back());

	{
	for(int i = 0; i < 10000 - 1; ++i)
	{
		std::cout << vt[i]->first <<","<< vt[i]->second << std::endl;
	}
	}

	std::cout << map[100000] << std::endl;



	char cc = 0;
	std::cin >> cc;
	return 0;
}
