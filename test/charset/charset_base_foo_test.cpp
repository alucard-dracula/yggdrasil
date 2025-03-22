// charset_base_foo_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>

#include <cassert>
#include <yggr/charset/charset_base_foo.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(charset)

int main(int argc, char* argv[])
{
	char str_arr[] = { 0, 0, 'a', 'b', 'c', 0 };
	std::string str(str_arr, sizeof(str_arr));

	yggr_test_assert(3 == yggr::charset::charset_base_foo::strlen("abc"));
	//yggr_test_assert(5 == yggr::charset::charset_base_foo::strlen("abc", 5));
	yggr_test_assert(yggr::charset::charset_base_foo::E_space_expansion_rate * (5 + 1)
			== yggr::charset::charset_base_foo::xchglen(5));

	yggr_test_assert(yggr::charset::charset_base_foo::E_space_expansion_rate * (3 + 1)
			== yggr::charset::charset_base_foo::xchglen("abc"));

	yggr_test_assert(yggr::charset::charset_base_foo::E_space_expansion_rate * (sizeof(str_arr) + 1)
			== yggr::charset::charset_base_foo::xchglen(str));

	{
		char str_arr[] = { 0, 0, 'a', 'b', 'c', 0 };
		std::string str(str_arr, sizeof(str_arr));
		yggr_test_assert(str.size() == sizeof(str_arr));
		yggr_test_assert(yggr::charset::charset_base_foo::normal_length(str).size() == 0);
	}

	{
		char str_arr[] = { 'a', 0, 'a', 'b', 'c', 0 };
		std::string str(str_arr, sizeof(str_arr));
		yggr_test_assert(str.size() == sizeof(str_arr));
		yggr_test_assert(yggr::charset::charset_base_foo::normal_length(str).size() == 1);
	}

	{
		char str_arr[] = { 'a', 'b', 'a', 'b', 'c', 0 };
		std::string str(str_arr, sizeof(str_arr));
		yggr_test_assert(str.size() == sizeof(str_arr));
		yggr_test_assert(yggr::charset::charset_base_foo::normal_length(str).size() == 5);
	}

	{
		char str_arr[] = { 'a', 'b', 'a', 'b', 'c', 0, 0, 0, 0 };
		std::string str(str_arr, sizeof(str_arr));
		yggr_test_assert(str.size() == sizeof(str_arr));
		yggr_test_assert(yggr::charset::charset_base_foo::normal_length(str).size() == 5);
	}

#ifndef YGGR_NO_CWCHAR

	wchar_t wstr_arr[] = { 0, 0, L'a', L'b', L'c', 0 };
	std::wstring wstr(wstr_arr, sizeof(wstr_arr) / sizeof(wchar_t));

	yggr_test_assert(3 == yggr::charset::charset_base_foo::strlen(L"abc"));
	//yggr_test_assert(5 == yggr::charset::charset_base_foo::strlen(L"abc", 5));

	yggr_test_assert(yggr::charset::charset_base_foo::E_space_expansion_rate * (3 + 1)
			== yggr::charset::charset_base_foo::xchglen(L"abc"));

	yggr_test_assert(yggr::charset::charset_base_foo::E_space_expansion_rate * (sizeof(wstr_arr) / sizeof(wchar_t) + 1)
			== yggr::charset::charset_base_foo::xchglen(wstr));

	{
		wchar_t wstr_arr[] = { 0, 0, L'a', L'b', L'c', 0 };
		std::wstring wstr(wstr_arr, sizeof(wstr_arr) / sizeof(wchar_t));
		yggr_test_assert(wstr.size() == sizeof(wstr_arr) / sizeof(wchar_t));
		yggr_test_assert(yggr::charset::charset_base_foo::normal_length(wstr).size() == 0);
	}

	{
		wchar_t wstr_arr[] = { L'a', 0, L'a', L'b', L'c', 0 };
		std::wstring wstr(wstr_arr, sizeof(wstr_arr) / sizeof(wchar_t));
		yggr_test_assert(wstr.size() == sizeof(wstr_arr) / sizeof(wchar_t));
		yggr_test_assert(yggr::charset::charset_base_foo::normal_length(wstr).size() == 1);
	}

	{
		wchar_t wstr_arr[] = { L'a', L'b', L'a', L'b', L'c', 0 };
		std::wstring wstr(wstr_arr, sizeof(wstr_arr) / sizeof(wchar_t));
		yggr_test_assert(wstr.size() == sizeof(wstr_arr) / sizeof(wchar_t));
		yggr_test_assert(yggr::charset::charset_base_foo::normal_length(wstr).size() == 5);
	}

	{
		wchar_t wstr_arr[] = { L'a', L'b', L'a', L'b', L'c', 0, 0, 0, 0, 0 };
		std::wstring wstr(wstr_arr, sizeof(wstr_arr) / sizeof(wchar_t));
		yggr_test_assert(wstr.size() == sizeof(wstr_arr) / sizeof(wchar_t));
		yggr_test_assert(yggr::charset::charset_base_foo::normal_length(wstr).size() == 5);
	}

#endif // YGGR_NO_CWCHAR

	{
		int arr[3] = {'a', 'b', 0};
		int arr2[3] = {'A', 'B', 0};
		int arr3[3] = {'C', 'D', 0};

		yggr_test_assert(yggr::charset::charset_base_foo::strlen(arr) == 2);
		
		yggr_test_assert(yggr::charset::charset_base_foo::xchglen(arr) == yggr::charset::charset_base_foo::xchglen(2));

		yggr_test_assert(yggr::charset::charset_base_foo::strcmp_case(arr, arr2) == 0);
		yggr_test_assert(yggr::charset::charset_base_foo::strcmp_case(arr, arr3) != 0);

		yggr_test_assert(yggr::charset::charset_base_foo::strncmp_case(arr, arr2, 2) == 0);
		yggr_test_assert(yggr::charset::charset_base_foo::strncmp_case(arr, arr3, 2) != 0);
	}

	wait_any_key(argc, argv);
	return 0;
}