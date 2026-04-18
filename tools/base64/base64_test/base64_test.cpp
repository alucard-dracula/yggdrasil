//base64_test.cpp

#include <iostream>

#include <cstring> // memcmp
#include <string>
#include <cassert>

#include <base64.h>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

#ifdef _MSC_VER
#	if defined(_DEBUG)
#		if defined(_DLL)
#			if _MSC_VER == 1600
#				pragma comment(lib, "libbase64-vc100-d.lib")
#			elif _MSC_VER == 1700
#				pragma comment(lib, "libbase64-vc110-d.lib")
#			elif _MSC_VER == 1800
#				pragma comment(lib, "libbase64-vc120-d.lib")
#			elif _MSC_VER == 1900
#				pragma comment(lib, "libbase64-vc140-d.lib")
#			elif _MSC_VER == 1910
#				pragma comment(lib, "libbase64-vc141-d.lib")
#			elif 1920 <= _MSC_VER && _MSC_VER <= 1929
#				pragma comment(lib, "libbase64-vc142-d.lib")
#			elif 1930 <= _MSC_VER && _MSC_VER < 2000
#				pragma comment(lib, "libbase64-vc143-d.lib")
#			else
#				error "set libs"
#			endif // ( _MSC_VER == 1600)
#		else
#			if _MSC_VER == 1600
#				pragma comment(lib, "libbase64-vc100-s-d.lib")
#			elif _MSC_VER == 1700
#				pragma comment(lib, "libbase64-vc110-s-d.lib")
#			elif _MSC_VER == 1800
#				pragma comment(lib, "libbase64-vc120-s-d.lib")
#			elif _MSC_VER == 1900
#				pragma comment(lib, "libbase64-vc140-s-d.lib")
#			elif _MSC_VER == 1910
#				pragma comment(lib, "libbase64-vc141-s-d.lib")
#			elif 1920 <= _MSC_VER && _MSC_VER <= 1929
#				pragma comment(lib, "libbase64-vc142-s-d.lib")
#			elif 1930 <= _MSC_VER && _MSC_VER < 2000
#				pragma comment(lib, "libbase64-vc143-s-d.lib")
#			else
#				error "set libs"
#			endif // ( _MSC_VER == 1600)
#		endif // _DLL
#	else // _DEBUG
#		if defined(_DLL)
#			if _MSC_VER == 1600
#				pragma comment(lib, "libbase64-vc100.lib")
#			elif _MSC_VER == 1700
#				pragma comment(lib, "libbase64-vc110.lib")
#			elif _MSC_VER == 1800
#				pragma comment(lib, "libbase64-vc120.lib")
#			elif _MSC_VER == 1900
#				pragma comment(lib, "libbase64-vc140.lib")
#			elif _MSC_VER == 1910
#				pragma comment(lib, "libbase64-vc141.lib")
#			elif 1920 <= _MSC_VER && _MSC_VER <= 1929
#				pragma comment(lib, "libbase64-vc142.lib")
#			elif 1930 <= _MSC_VER && _MSC_VER < 2000
#				pragma comment(lib, "libbase64-vc143.lib")
#			else
#				error "set libs"
#			endif // ( _MSC_VER == 1600)
#		else
#			if _MSC_VER == 1600
#				pragma comment(lib, "libbase64-vc100-s.lib")
#			elif _MSC_VER == 1700
#				pragma comment(lib, "libbase64-vc110-s.lib")
#			elif _MSC_VER == 1800
#				pragma comment(lib, "libbase64-vc120-s.lib")
#			elif _MSC_VER == 1900
#				pragma comment(lib, "libbase64-vc140-s.lib")
#			elif _MSC_VER == 1910
#				pragma comment(lib, "libbase64-vc141-s.lib")
#			elif 1920 <= _MSC_VER && _MSC_VER <= 1929
#				pragma comment(lib, "libbase64-vc142-s.lib")
#			elif 1930 <= _MSC_VER && _MSC_VER < 2000
#				pragma comment(lib, "libbase64-vc143-s.lib")
#			else
#				error "set libs"
#			endif // ( _MSC_VER == 1600)
#		endif // _DLL
#	endif // _DEBUG
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
