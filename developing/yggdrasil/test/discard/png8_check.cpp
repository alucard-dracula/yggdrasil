//png8_check.cpp

#include <yggr/base/yggrdef.h>
#include <iostream>
#include <yggr/file_system/local_file_operator.hpp>
#include <string>

typedef yggr::file_system::local_file_operator_type::path_list_type path_list;
typedef path_list::iterator path_list_iter;

bool is_png8(const std::string& fname)
{
	yggr::u32 ver = 0;
	std::ifstream ifs;
	ifs.open(fname.c_str(), std::ios::in | std::ios::binary);

	ifs.seekg(0x19, std::ios_base::beg);

	ifs.read((char*)&ver, sizeof(yggr::u32));

	ifs.close();

	return ver == 3;
}

bool is_png(const std::string& str)
{

	return str.size() > 4
			&& (*(str.end() - 1)) == 'g' 
			&& (*(str.end() - 2)) == 'n'
			&& (*(str.end() - 3)) == 'p'
			&& (*(str.end() - 4)) == '.';
}
void search_file(const std::string& path)
{
	std::ofstream ofs;
	ofs.open("fail.txt");

	path_list pl;

	yggr::file_system::local_file_operator_type::recursion_search_of_files(path, pl);

	for(path_list_iter p = pl.begin(); p != pl.end(); ++p)
	{
		if(!is_png(*p))
		{
			continue;
		}

		if(!is_png8(*p))
		{
			std::cout << *p << std::endl;
			ofs << *p << "\r\n";
		}
	}


	ofs.close();
};

int main(int argc, char* argv[])
{
	search_file("./");

	std::cout << "compleate" << std::endl;
	char cc = 0;
	std::cin >> cc;
	return 0;
}