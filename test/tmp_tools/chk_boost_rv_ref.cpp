//chk_boost_rv_ref.cpp

#include <iostream>
#include <fstream>

#include <string>
#include <list>
#include <boost/unordered_set.hpp>

int main(int argc, char* argv[])
{
	typedef boost::unordered_set<std::string> str_set_type;
	str_set_type def_set;
	str_set_type use_set;

	{
		char str[1024] = { 0 };
		std::ifstream ifs;
		ifs.open("./tmp_tools/def_text.txt");

		for(;!ifs.eof();)
		{
			ifs.getline(str, 1023);
			def_set.insert(std::string(str));
		}
		
		ifs.close();

		assert(def_set.size());

	}

	{
		char str[1024] = { 0 };
		std::ifstream ifs;
		ifs.open("./tmp_tools/use_text.txt");

		for(;!ifs.eof();)
		{
			ifs.getline(str, 1023);
			use_set.insert(std::string(str));
		}

		ifs.close();

		assert(use_set.size());
	}


	std::string rst;
	for(str_set_type::const_iterator i = use_set.begin(), isize  = use_set.end();
			i != isize; ++i)
	{
		if(def_set.find(*i) != def_set.end())
		{
			continue;
		}

		rst += (*i) + "\r\n";
	}

	std::ofstream ofs;
	ofs.open("./tmp_tools/chk_rv_ref.txt");

	ofs.write(&rst[0], rst.size());

	ofs.close();
	
	std::cout << "end" << std::endl;

	char cc = 0;
	std::cin >> cc;
	return 0;
}