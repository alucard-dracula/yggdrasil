//utf8_string_get.cpp

#include <iostream>
#include <yggr/charset/utf8_string.hpp>

int main(int argc, char* argv[])
{
	typedef yggr::charset::utf8_string string_type;

	/*string_type str1 = "¹§Ï²";
	string_type str2 = "Éý¼¶µ½vip";

	{
		const string_type::base_type& str = str1;

		for(int i = 0, isize = str.size(); i != isize; ++i)
		{
			std::cout  << (int)str[i] << ",";
		}
	}

	std::cout << std::endl;

	{
		const string_type::base_type& str = str2;

		for(int i = 0, isize = str.size(); i != isize; ++i)
		{
			std::cout << (int)str[i] << ",";
		}
	}*/

	char tstr[] = {-26, -127, -83, -27, -106, -100, 0};
	char tstr1[] = {-27, -115, -121, -25, -70, -89, -27, -120, -80, 118, 105, 112, 0};
	string_type ustr(tstr, UTF8_CHARSET_NAME);
	string_type ustr1(tstr1, UTF8_CHARSET_NAME);

	std::cout << ustr << "," << ustr1 << std::endl;

	char cc = 0;
	std::cin >> cc;

	return 0;
}
