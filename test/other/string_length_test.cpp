//text_archive_test.cpp

#include <iostream>
#include <string>
int main(int argc, char* argv[])
{
	std::string str;
	str.resize(100, 0);

	std::cout << (int*)&str[0] << std::endl;

	str.resize(50, 0);
	std::cout << (int*)&str[0] << std::endl;
	str.resize(101, 0);
	std::cout << (int*)&str[0] << std::endl;

	str.resize(120, 0);
	std::cout << (int*)&str[0] << std::endl;

	std::cout << str.size() << std::endl;
	std::cout << str.capacity() << std::endl;
	char cc = 0;
	std::cin >> cc;
	return 0;
}
