// empty_string_string_view_check.cpp

#include <test/wait_any_key/wait_any_key.hpp>

#include <yggr/charset/utf8_string.hpp>
#include <yggr/charset/utf8_string_view.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(base)

template<typename String, typename StringView>
void test_detail(void)
{
	String str;
	StringView str_view;
	
	{
		String str(str_view);
		yggr_test_assert(str.empty());
	}

	yggr_test_assert(str.empty());
	yggr_test_assert(str_view.empty());

	yggr_test_assert(str == str_view);
	yggr_test_assert(str_view == str);

	yggr_test_assert(!(str < str_view));
	yggr_test_assert(!(str_view < str));

	yggr_test_assert(str <= str_view);
	yggr_test_assert(str_view <= str);

	yggr_test_assert(!(str > str_view));
	yggr_test_assert(!(str_view > str));

	yggr_test_assert(str >= str_view);
	yggr_test_assert(str_view >= str);
}

template<typename String, typename StringView>
void test_detail(const String& str, const StringView& str_view)
{
	yggr_test_assert(str.empty());
	yggr_test_assert(str_view.empty());

	yggr_test_assert(str == str_view);
	yggr_test_assert(str_view == str);

	yggr_test_assert(!(str < str_view));
	yggr_test_assert(!(str_view < str));

	yggr_test_assert(str <= str_view);
	yggr_test_assert(str_view <= str);

	yggr_test_assert(!(str > str_view));
	yggr_test_assert(!(str_view > str));

	yggr_test_assert(str >= str_view);
	yggr_test_assert(str_view >= str);
}

void test(void)
{
#if !(BOOST_VERSION < 106400)
	test_detail<yggr::string, yggr::string_view>();
	test_detail<yggr::wstring, yggr::wstring_view>();
#endif // #if !(BOOST_VERSION < 106400)

	test_detail<yggr::utf8_string, yggr::utf8_string_view>();
	test_detail<yggr::utf8_string, yggr::string_view>();
	test_detail<yggr::utf8_string, yggr::wstring_view>();

	test_detail<yggr::utf8_string_view, yggr::utf8_string_view>();
	test_detail<yggr::string_view, yggr::string_view>();
	test_detail<yggr::wstring_view, yggr::wstring_view>();

	test_detail(yggr::string(), yggr::string_view(""));
	test_detail(yggr::wstring(), yggr::wstring_view(L""));

	test_detail(yggr::utf8_string(), yggr::utf8_string_view(""));
	test_detail(yggr::utf8_string(), yggr::string_view(""));
	test_detail(yggr::utf8_string(), yggr::wstring_view(L""));

	test_detail(yggr::utf8_string_view(), yggr::utf8_string_view(""));
	test_detail(yggr::utf8_string_view(), yggr::string_view(""));
	test_detail(yggr::utf8_string_view(), yggr::wstring_view(L""));

	std::cout << "test end" << std::endl;
}

int main(int argc, char* argv[])
{
	test();

	wait_any_key(argc, argv);
	return 0;
}