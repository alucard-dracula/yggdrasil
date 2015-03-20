//regex_parse_search_test.cpp

#include <iostream>
#include <vector>

#include <yggr/regular_parse/regex_parse.hpp>
#include <yggr/file_system/local_file_operator.hpp>

int main(int argc, char* argv[])
{
	std::string buf;
	yggr::size_type size;

	try
	{
		yggr::file_system::local_file_operator_type::read_file_of_binary("./regex/string_list.txt", buf, size); // load file
	}
	catch(const compatibility::stl_exception& e)
	{
		std::cerr << e.what() << std::endl;
		return -1;
	}

	std::vector<std::string> strvt;
	std::string sp_str("\\r\\n");

	try
	{
		yggr::regular_parse::regex_parse::split(strvt, buf, sp_str); //parse string to string vt
	}
	catch(const yggr::regular_parse::regex_parse::error_type& e)
	{
		std::cout << e.what() << std::endl;
		return -1;
	}

	for(yggr::size_type i = 0; i != strvt.size(); ++i)
	{
		std::cout << strvt[i] << std::endl;
	}

	char cc = 0;
	std::cin >> cc;
	return 0;
}
