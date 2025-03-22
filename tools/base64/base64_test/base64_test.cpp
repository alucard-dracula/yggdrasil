//base64_test.cpp

#include <iostream>

#include <cstring> // memcmp
#include <string>
#include <cassert>

#include <base64.h>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

int main(int argc, char* argv[])
{
	char arr[] = {1, 2, 3, 4, 5, 6, 7, 8};
	int str_len = b64_ntop_byte_size(sizeof(arr));

	std::string str(str_len, 0);

	b64_ntop(reinterpret_cast<uint8_t*>(arr), sizeof(arr), &str[0], str_len);

	std::cout << str << std::endl;

	int b64n_len = b64_pton_byte_size(str.size());
	std::string nbuf(b64n_len, 0);

	int real_size = b64_pton(str.c_str(), reinterpret_cast<uint8_t*>(&nbuf[0]), nbuf.size());

	bool bchk_size = (real_size == sizeof(arr));
	assert(bchk_size);

	nbuf.erase(nbuf.end() - (b64n_len - real_size));

	bool bchk_data = (0 == memcmp(&nbuf[0], arr, sizeof(arr)));
	assert(bchk_data);

	if(bchk_size && bchk_data)
    {
        std::cout << "test base64 success" << std::endl;
    }
    else
    {
        std::cout << "test base64 falied" << std::endl;
    }

	char cc = 0;
	std::cin >> cc;

	return 0;
}
