//string_byte_size_testc.cpp

#include <cassert>
#include <test/wait_any_key/wait_any_key.hpp>
#include <yggr/charset/string_byte_size.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(charset)

void test_string_byte_size_t(void)
{
	{
		std::string str("abc");
		yggr::charset::detail::string_byte_size_t<std::string> sbs;
		yggr_test_assert(sbs(str) == str.size());
		yggr_test_assert(yggr::charset::string_byte_size(str) == sbs(str));
		sbs(str);
	}

	{
		std::wstring str(L"abc");
		yggr::charset::detail::string_byte_size_t<std::wstring> sbs;
		yggr_test_assert(sbs(str) == str.size() * sizeof(wchar_t));
		yggr_test_assert(yggr::charset::string_byte_size(str) == sbs(str));
		sbs(str);
	}

	{
		boost::container::string str("abc");
		yggr::charset::detail::string_byte_size_t<boost::container::string> sbs;
		yggr_test_assert(sbs(str) == str.size());
		yggr_test_assert(yggr::charset::string_byte_size(str) == sbs(str));
		sbs(str);
	}

	{
		boost::container::wstring str(L"abc");
		yggr::charset::detail::string_byte_size_t<boost::container::wstring> sbs;
		yggr_test_assert(sbs(str) == str.size() * sizeof(wchar_t));
		yggr_test_assert(yggr::charset::string_byte_size(str) == sbs(str));
		sbs(str);
	}

	{
		char str[] = "abc";
		yggr::charset::detail::string_byte_size_t<char[4]> sbs;
		yggr_test_assert(sbs(str) == sizeof(str) - 1);
		yggr_test_assert(yggr::charset::string_byte_size(str) == sbs(str));
		sbs(str);
	}

	{
		wchar_t str[] = L"abc";
		yggr::charset::detail::string_byte_size_t<wchar_t[4]> sbs;
		yggr_test_assert(sbs(str) == sizeof(str) - sizeof(wchar_t));
		yggr_test_assert(yggr::charset::string_byte_size(str) == sbs(str));
		sbs(str);
	}

	{
		const char* str = "abc";
		yggr::charset::detail::string_byte_size_t<const char*> sbs;
		yggr_test_assert(sbs(str) == std::strlen(str));
		yggr_test_assert(yggr::charset::string_byte_size(str) == sbs(str));
		sbs(str);
	}

	{
		const wchar_t* str = L"abc";
		yggr::charset::detail::string_byte_size_t<const wchar_t*> sbs;
		yggr_test_assert(sbs(str) == std::wcslen(str) * sizeof(wchar_t));
		sbs(str);
	}

	{
		char* str = const_cast<char*>("abc");
		yggr::charset::detail::string_byte_size_t<char*> sbs;
		yggr_test_assert(sbs(str) == std::strlen(str));
		yggr_test_assert(yggr::charset::string_byte_size(str) == sbs(str));
		sbs(str);
	}

	{
		wchar_t* str = const_cast<wchar_t*>(L"abc");
		yggr::charset::detail::string_byte_size_t<wchar_t*> sbs;
		yggr_test_assert(sbs(str) == std::wcslen(str) * sizeof(wchar_t));
		yggr_test_assert(yggr::charset::string_byte_size(str) == sbs(str));
		sbs(str);
	}

	std::cout << "test_string_byte_size_t end" << std::endl;
}

void test_char_buffer_byte_size_t(void)
{
	{
		std::string str("abc");
		yggr::charset::detail::char_buffer_byte_size_t<std::string> cbs;
		yggr_test_assert(cbs(str) == str.size() + 1);
		yggr_test_assert(yggr::charset::char_buffer_byte_size(str) == cbs(str));
		cbs(str);
	}

	{
		std::wstring str(L"abc");
		yggr::charset::detail::char_buffer_byte_size_t<std::wstring> cbs;
		yggr_test_assert(cbs(str) == (str.size() + 1) * sizeof(wchar_t));
		yggr_test_assert(yggr::charset::char_buffer_byte_size(str) == cbs(str));
		cbs(str);
	}

	{
		boost::container::string str("abc");
		yggr::charset::detail::char_buffer_byte_size_t<boost::container::string> cbs;
		yggr_test_assert(cbs(str) == str.size() + 1);
		yggr_test_assert(yggr::charset::char_buffer_byte_size(str) == cbs(str));
		cbs(str);
	}

	{
		boost::container::wstring str(L"abc");
		yggr::charset::detail::char_buffer_byte_size_t<boost::container::wstring> cbs;
		yggr_test_assert(cbs(str) == (str.size() + 1) * sizeof(wchar_t));
		yggr_test_assert(yggr::charset::char_buffer_byte_size(str) == cbs(str));
		cbs(str);
	}

	{
		char str[] = "abc";
		yggr::charset::detail::char_buffer_byte_size_t<char[4]> cbs;
		yggr_test_assert(cbs(str) == sizeof(str));
		yggr_test_assert(yggr::charset::char_buffer_byte_size(str) == cbs(str));
		cbs(str);
	}

	{
		wchar_t str[] = L"abc";
		yggr::charset::detail::char_buffer_byte_size_t<wchar_t[4]> cbs;
		yggr_test_assert(cbs(str) == sizeof(str));
		yggr_test_assert(yggr::charset::char_buffer_byte_size(str) == cbs(str));
		cbs(str);
	}

	{
		const char* str = "abc";
		yggr::charset::detail::char_buffer_byte_size_t<const char*> cbs;
		yggr_test_assert(cbs(str) == std::strlen(str) + 1);
		yggr_test_assert(yggr::charset::char_buffer_byte_size(str) == cbs(str));
		cbs(str);
	}

	{
		const wchar_t* str = L"abc";
		yggr::charset::detail::char_buffer_byte_size_t<const wchar_t*> cbs;
		yggr_test_assert(cbs(str) == (std::wcslen(str) + 1) * sizeof(wchar_t));
		yggr_test_assert(yggr::charset::char_buffer_byte_size(str) == cbs(str));
		cbs(str);
	}

	{
		char* str = const_cast<char*>("abc");
		yggr::charset::detail::char_buffer_byte_size_t<char*> cbs;
		yggr_test_assert(cbs(str) == std::strlen(str) + 1);
		yggr_test_assert(yggr::charset::char_buffer_byte_size(str) == cbs(str));
		cbs(str);
	}

	{
		wchar_t* str = const_cast<wchar_t*>(L"abc");
		yggr::charset::detail::char_buffer_byte_size_t<wchar_t*> cbs;
		yggr_test_assert(cbs(str) == (std::wcslen(str) + 1) * sizeof(wchar_t));
		yggr_test_assert(yggr::charset::char_buffer_byte_size(str) == cbs(str));
		cbs(str);
	}

	std::cout << "test_char_buffer_byte_size_t end" << std::endl;
}

int main(int argc, char* argv[])
{
	test_string_byte_size_t();
	test_char_buffer_byte_size_t();

	wait_any_key(argc, argv);
	return 0;
}
