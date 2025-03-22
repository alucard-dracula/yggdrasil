// utf8_char_cast_conflict_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>

#include <yggr/charset/utf8_char.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(base)


int main(int argc, char* argv[])
{
	yggr::utf8_char utf8_ch('a');

	yggr::utf8_char::char_reference_type utf8_char_ref = utf8_ch;
	yggr::utf8_char::char_const_reference_type utf8_char_cref1 = utf8_ch;
	yggr::utf8_char::char_const_reference_type utf8_char_cref2 = utf8_char_ref;
	yggr::utf8_char::char_view_type utf8_ch_view1 = utf8_ch;
	yggr::utf8_char::char_view_type utf8_ch_view2 = utf8_char_ref;
	yggr::utf8_char::char_view_type utf8_ch_view3 = utf8_char_cref1;

	yggr::utf8_char utf8_ch1 = utf8_ch;
	yggr::utf8_char utf8_ch2 = utf8_char_ref;
	yggr::utf8_char utf8_ch3 = utf8_char_cref1;
	yggr::utf8_char utf8_ch4 = utf8_ch_view3;

	std::cout << utf8_ch1 << std::endl;
	std::cout << utf8_ch2 << std::endl;
	std::cout << utf8_ch3 << std::endl;
	std::cout << utf8_ch4 << std::endl;


	std::cout << utf8_char_ref << std::endl;
	std::cout << utf8_char_cref1 << std::endl;
	std::cout << utf8_char_cref2 << std::endl;
	std::cout << utf8_ch_view1 << std::endl;
	std::cout << utf8_ch_view2 << std::endl;
	std::cout << utf8_ch_view3 << std::endl;


	std::cout << static_cast<yggr::utf8_char::char_reference_type>(utf8_ch) << std::endl;

	std::cout << static_cast<yggr::utf8_char::char_const_reference_type>(utf8_ch) << std::endl;
	std::cout << static_cast<yggr::utf8_char::char_const_reference_type>(utf8_char_ref) << std::endl;

	std::cout << static_cast<yggr::utf8_char::char_view_type>(utf8_ch) << std::endl;
	std::cout << static_cast<yggr::utf8_char::char_view_type>(utf8_char_ref) << std::endl;
	std::cout << static_cast<yggr::utf8_char::char_view_type>(utf8_char_cref1) << std::endl;

	yggr_test_assert(static_cast<yggr::s32>(yggr::utf8_char::char_reference_type(utf8_char_ref) + utf8_ch_view3) == (97 + 97));
	try
	{
		std::cout << (yggr::utf8_char::char_reference_type(utf8_char_ref) + utf8_ch_view3) << std::endl;
	}
	catch(const yggr::error& e)
	{
		std::cerr << e.what() << std::endl;
	}

	yggr_test_assert(static_cast<yggr::s32>(utf8_char_ref.view() + utf8_ch) == (97 + 97));
	try
	{
		std::cout << (utf8_char_ref.view() + utf8_ch) << std::endl;
	}
	catch(const yggr::error& e)
	{
		std::cerr << e.what() << std::endl;
	}

	yggr_test_assert(static_cast<yggr::s32>(utf8_char_ref.view() + utf8_char_ref) == (97 + 97));
	try
	{
		std::cout << (utf8_char_ref.view() + utf8_char_ref) << std::endl;
	}
	catch(const yggr::error& e)
	{
		std::cerr << e.what() << std::endl;
	}

	yggr_test_assert(static_cast<yggr::s32>(utf8_char_ref.view() + utf8_char_cref1) == (97 + 97));
	try
	{
		std::cout << (utf8_char_ref.view() + utf8_char_cref1) << std::endl;
	}
	catch(const yggr::error& e)
	{
		std::cerr << e.what() << std::endl;
	}

	yggr_test_assert(static_cast<yggr::s32>(utf8_ch.view() + utf8_char_ref) == (97 + 97));
	try
	{
		std::cout << (utf8_ch.view() + utf8_char_ref) << std::endl;
	}
	catch(const yggr::error& e)
	{
		std::cerr << e.what() << std::endl;
	}

	wait_any_key(argc, argv);
	return 0;
}
