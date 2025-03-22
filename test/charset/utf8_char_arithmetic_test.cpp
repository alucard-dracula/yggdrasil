//utf8_char_arithmetic_test.cpp

#if !defined(_MSC_VER)
#	error "this file supports MSVC only, if you are using other compilers, please use the same name file in the charset_utf8_files"	
#endif // _MSC_VER

/*
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!!! if you want to test utf8, don't forgot add YGGR_USE_UTF8_EX_MODE to user_config.hpp !!!
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
*/

//#define YGGR_NO_CWCHAR

#include <test/wait_any_key/wait_any_key.hpp>

#include <yggr/charset/utf8_char.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(base)

/*
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!!! utf8_char arithmetic rule:																						!!!
!!!	if result is utf8_char the result must in utf8_char's range														!!!
!!!		in utf8_default_mode the range is 0x00000000 - 0x0010ffff (the value defined in utf8_foo::E_char_max_num)	!!!
!!!		in utf8_ex_mode the range is 0x00000000 - 0x7fffffff														!!!
!!!																													!!!
!!!	if result is number, the result is number + utf8_char.to_number(),												!!!
!!!		range is std::numeric_limits<result_type>::min() - std::numeric_limits<result_type>::max()					!!!
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
*/

void test_plus_plus_of_utf8_char(void)
{
	{
		char ch = 'a';
		yggr::utf8_char utf8_ch = ch;

		for(int i = 0, isize = 3; i != isize; ++i)
		{
			yggr_test_assert(++ch == ++utf8_ch);
			std::cout << utf8_ch << std::endl;
		}
	}

	{
		char ch = 'a';
		yggr::utf8_char utf8_ch = ch;

		for(int i = 0, isize = 3; i != isize; ++i)
		{
			yggr_test_assert(ch++ == utf8_ch++);
			std::cout << utf8_ch << std::endl;
		}
	}

#ifndef YGGR_NO_CWCHAR

	{
		wchar_t ch = 0x007f;
		yggr::utf8_char utf8_ch = ch;

		for(int i = 0, isize = 3; i != isize; ++i)
		{
			yggr_test_assert(++ch == ++utf8_ch);
			std::cout << static_cast<yggr::s32>(utf8_ch) << std::endl;
		}
	}

	{
		wchar_t ch = 0x07ff;
		yggr::utf8_char utf8_ch = ch;

		for(int i = 0, isize = 3; i != isize; ++i)
		{
			yggr_test_assert(ch++ == utf8_ch++);
			std::cout << static_cast<yggr::s32>(utf8_ch) << std::endl;
		}
	}

	{
		wchar_t ch = L'Äã';
		yggr::utf8_char utf8_ch = ch;

		for(int i = 0, isize = 3; i != isize; ++i)
		{
			yggr_test_assert(++ch == ++utf8_ch);
			std::cout << utf8_ch << std::endl;
		}
	}

	{
		wchar_t ch = L'Äã';
		yggr::utf8_char utf8_ch = ch;

		for(int i = 0, isize = 3; i != isize; ++i)
		{
			yggr_test_assert(ch++ == utf8_ch++);
			std::cout << utf8_ch << std::endl;
		}
	}

#endif // YGGR_NO_CWCHAR

	std::cout << "test_plus_plus_of_utf8_char succeed" << std::endl;
}

void test_plus_plus_of_utf8_char_reference(void)
{
	typedef yggr::utf8_char::char_reference_type ref_type;

	{
		char ch = 'a';
		yggr::utf8_char utf8_ch = ch;
		ref_type utf8_ch_ref = utf8_ch;

		for(int i = 0, isize = 3; i != isize; ++i)
		{
			yggr_test_assert(++ch == ++utf8_ch_ref);
			std::cout << utf8_ch << std::endl;
		}
	}

	{
		char ch = 'a';
		yggr::utf8_char utf8_ch = ch;
		ref_type utf8_ch_ref = utf8_ch;

		for(int i = 0, isize = 3; i != isize; ++i)
		{
			yggr_test_assert(ch++ == utf8_ch_ref++);
			std::cout << utf8_ch << std::endl;
		}
	}

#ifndef YGGR_NO_CWCHAR

	{
		wchar_t ch = 0x007f;
		yggr::utf8_char utf8_ch = ch;
		ref_type utf8_ch_ref = utf8_ch;

		for(int i = 0, isize = 3; i != isize; ++i)
		{
			yggr_test_assert(++ch == ++utf8_ch_ref);
			std::cout << static_cast<yggr::s32>(utf8_ch) << std::endl;
		}
	}

	{
		wchar_t ch = 0x07ff;
		yggr::utf8_char utf8_ch = ch;
		ref_type utf8_ch_ref = utf8_ch;

		for(int i = 0, isize = 3; i != isize; ++i)
		{
			yggr_test_assert(ch++ == utf8_ch_ref++);
			std::cout << static_cast<yggr::s32>(utf8_ch) << std::endl;
		}
	}

	{
		wchar_t ch = L'Äã';
		yggr::utf8_char utf8_ch = ch;
		ref_type utf8_ch_ref = utf8_ch;

		for(int i = 0, isize = 3; i != isize; ++i)
		{
			yggr_test_assert(++ch == ++utf8_ch_ref);
			std::cout << utf8_ch << std::endl;
		}
	}

	{
		wchar_t ch = L'Äã';
		yggr::utf8_char utf8_ch = ch;
		ref_type utf8_ch_ref = utf8_ch;

		for(int i = 0, isize = 3; i != isize; ++i)
		{
			yggr_test_assert(ch++ == utf8_ch_ref++);
			std::cout << utf8_ch << std::endl;
		}
	}

#endif // YGGR_NO_CWCHAR

	std::cout << "test_plus_plus_of_utf8_char_reference succeed" << std::endl;
}

void test_minus_minus_of_utf8_char(void)
{

	{
		char ch = 'd';
		yggr::utf8_char utf8_ch = ch;

		for(int i = 0, isize = 3; i != isize; ++i)
		{
			yggr_test_assert(--ch == --utf8_ch);
			std::cout << utf8_ch << std::endl;
		}
	}

	{
		char ch = 'd';
		yggr::utf8_char utf8_ch = ch;

		for(int i = 0, isize = 3; i != isize; ++i)
		{
			yggr_test_assert(ch-- == utf8_ch--);
			std::cout << utf8_ch << std::endl;
		}
	}

#ifndef YGGR_NO_CWCHAR

	{
		wchar_t ch = 0;
		yggr::utf8_char utf8_ch = ch;

		for(int i = 0, isize = 3; i != isize; ++i)
		{
			std::cout << static_cast<yggr::s32>(--utf8_ch) << std::endl;
			yggr_test_assert(utf8_ch.is_validate());
		}
	}

	{
		wchar_t ch = 0;
		yggr::utf8_char utf8_ch = ch;

		for(int i = 0, isize = 3; i != isize; ++i)
		{
			std::cout << static_cast<yggr::s32>(utf8_ch--) << std::endl;
			yggr_test_assert(utf8_ch.is_validate());
		}
	}

	{
		wchar_t ch = L'Ó¶';
		yggr::utf8_char utf8_ch = ch;

		for(int i = 0, isize = 3; i != isize; ++i)
		{
			yggr_test_assert(--ch == --utf8_ch);
			std::cout << utf8_ch << std::endl;
		}
	}

	{
		wchar_t ch = L'Ó¶';
		yggr::utf8_char utf8_ch = ch;

		for(int i = 0, isize = 3; i != isize; ++i)
		{
			yggr_test_assert(ch-- == utf8_ch--);
			std::cout << utf8_ch << std::endl;
		}
	}

#endif // YGGR_NO_CWCHAR

	std::cout << "test_minus_minus_of_utf8_char succeed" << std::endl;
}

void test_minus_minus_of_utf8_char_reference(void)
{
	typedef yggr::utf8_char::char_reference_type ref_type;

	{
		char ch = 'd';
		yggr::utf8_char utf8_ch = ch;
		ref_type utf8_ch_ref = utf8_ch;

		for(int i = 0, isize = 3; i != isize; ++i)
		{
			yggr_test_assert(--ch == --utf8_ch_ref);
			std::cout << utf8_ch << std::endl;
		}
	}

	{
		char ch = 'd';
		yggr::utf8_char utf8_ch = ch;
		ref_type utf8_ch_ref = utf8_ch;

		for(int i = 0, isize = 3; i != isize; ++i)
		{
			yggr_test_assert(ch == yggr::utf8_char(utf8_ch));
			yggr_test_assert(ch-- == utf8_ch_ref--);
			std::cout << utf8_ch << std::endl;
		}
	}

#ifndef YGGR_NO_CWCHAR

	{
		wchar_t ch = 0;
		yggr::utf8_char utf8_ch = ch;
		ref_type utf8_ch_ref = utf8_ch;

		for(int i = 0, isize = 3; i != isize; ++i)
		{
			std::cout << static_cast<yggr::s32>(--utf8_ch_ref) << std::endl;
			yggr_test_assert(utf8_ch.is_validate());
		}
	}

	{
		wchar_t ch = 0;
		yggr::utf8_char utf8_ch = ch;
		ref_type utf8_ch_ref = utf8_ch;

		for(int i = 0, isize = 3; i != isize; ++i)
		{
			std::cout << static_cast<yggr::s32>(utf8_ch_ref--) << std::endl;
			yggr_test_assert(utf8_ch.is_validate());
		}
	}

	{
		wchar_t ch = L'Ó¶';
		yggr::utf8_char utf8_ch = ch;
		ref_type utf8_ch_ref = utf8_ch;

		for(int i = 0, isize = 3; i != isize; ++i)
		{
			yggr_test_assert(--ch == --utf8_ch_ref);
			std::cout << utf8_ch << std::endl;
		}
	}

	{
		wchar_t ch = L'Ó¶';
		yggr::utf8_char utf8_ch = ch;
		ref_type utf8_ch_ref = utf8_ch;

		for(int i = 0, isize = 3; i != isize; ++i)
		{
			yggr_test_assert(ch-- == utf8_ch_ref--);
			std::cout << utf8_ch << std::endl;
		}
	}

#endif // YGGR_NO_CWCHAR

	std::cout << "test_minus_minus_of_utf8_char_reference succeed" << std::endl;
}

void test_positive_negative_bitnot_of_utf8_char(void)
{
	{
		yggr::utf8_char utf8_ch('b');

		yggr_test_assert(+utf8_ch == 'b');
		yggr_test_assert(-utf8_ch == yggr::charset::utf8_foo::E_char_max_range - 'b');
		yggr_test_assert(~utf8_ch == yggr::charset::utf8_foo::E_char_max_range - 'b' - 1);

		yggr_test_assert(-(-utf8_ch) == utf8_ch);
	}

#ifndef YGGR_NO_CWCHAR

	{
		yggr::utf8_char utf8_ch(L'Äã');

		yggr_test_assert(+utf8_ch == L'Äã');
		yggr_test_assert(-utf8_ch == yggr::charset::utf8_foo::E_char_max_range - L'Äã');
		yggr_test_assert(~utf8_ch == yggr::charset::utf8_foo::E_char_max_range - L'Äã' - 1);
		yggr_test_assert(-(-utf8_ch) == utf8_ch);
	}

#endif // YGGR_NO_CWCHAR

	std::cout << "test_positive_negative_bitnot_of_utf8_char succeed" << std::endl;
}

void test_positive_negative_bitnot_of_utf8_char_reference(void)
{
	typedef yggr::utf8_char::char_reference_type ref_type;

	{
		yggr::utf8_char utf8_ch('b');
		ref_type utf8_ch_ref = utf8_ch;
		yggr::utf8_char utf8_ch_minus = -utf8_ch;

		yggr_test_assert(+utf8_ch_ref == 'b');
		yggr_test_assert(-utf8_ch_ref == yggr::charset::utf8_foo::E_char_max_range - 'b');
		yggr_test_assert(~utf8_ch_ref == yggr::charset::utf8_foo::E_char_max_range - 'b' - 1);

		yggr_test_assert(-static_cast<yggr::utf8_char::char_reference_type>(utf8_ch_minus) == utf8_ch);
	}

#ifndef YGGR_NO_CWCHAR

	{
		yggr::utf8_char utf8_ch(L'Äã');
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert(+utf8_ch_ref == L'Äã');
		yggr_test_assert(-utf8_ch_ref == yggr::charset::utf8_foo::E_char_max_range - L'Äã');
		yggr_test_assert(~utf8_ch_ref == yggr::charset::utf8_foo::E_char_max_range - L'Äã' - 1);

		yggr_test_assert(-static_cast<ref_type>(-utf8_ch_ref) == utf8_ch);
	}

#endif // YGGR_NO_CWCHAR

	std::cout << "test_positive_negative_bitnot_of_utf8_char_reference succeed" << std::endl;
}

void test_plus_set_of_utf8_char(void)
{
	{
		char ch = 'a';
		yggr::utf8_char utf8_ch = ch;

		yggr_test_assert((ch += char(3)) == (utf8_ch += 3));
		std::cout << utf8_ch << std::endl;
	}

	{
		char ch = 'a';
		yggr::utf8_char utf8_ch = ch;

		yggr_test_assert((ch += char(3)) == (utf8_ch += yggr::utf8_char(3)));
		std::cout << utf8_ch << std::endl;
	}

#ifndef YGGR_NO_CWCHAR

	{
		wchar_t ch = 0x007f;
		yggr::utf8_char utf8_ch = ch;

		yggr_test_assert((ch += wchar_t(3)) == (utf8_ch += 3));
		std::cout << static_cast<yggr::s32>(utf8_ch) << std::endl;
	}

	{
		wchar_t ch = L'Äã';
		yggr::utf8_char utf8_ch = ch;

		yggr_test_assert((ch += wchar_t(3)) == (utf8_ch += char(3)));
		std::cout << utf8_ch << std::endl;
	}

	{
		wchar_t ch = L'Äã';
		yggr::utf8_char utf8_ch = ch;

		yggr_test_assert((ch += wchar_t(3)) == (utf8_ch += yggr::utf8_char(3)));
		std::cout << utf8_ch << std::endl;
	}

	{
		wchar_t ch = L'Äã';
		yggr::utf8_char utf8_ch = ch;
		yggr::utf8_char step = 3;


		yggr_test_assert((ch += 3) == (utf8_ch += static_cast<yggr::utf8_char::char_reference_type>(step)));
		std::cout << utf8_ch << std::endl;
	}

	{
		wchar_t ch = L'Äã';
		yggr::utf8_char utf8_ch = ch;
		yggr::utf8_char step = 3;


		yggr_test_assert((ch += 3) == (utf8_ch += static_cast<yggr::utf8_char::char_const_reference_type>(step)));
		std::cout << utf8_ch << std::endl;
	}

	{
		wchar_t ch = L'Äã';
		yggr::utf8_char utf8_ch = ch;
		yggr::utf8_char step = 3;


		yggr_test_assert((ch += 3) == (utf8_ch += step.view()));
		std::cout << utf8_ch << std::endl;
	}

#endif // YGGR_NO_CWCHAR

	std::cout << "test_plus_set_of_utf8_char succeed" << std::endl;
}

void test_plus_set_of_utf8_char_reference(void)
{
	typedef yggr::utf8_char::char_reference_type ref_type;

	{
		char ch = 'a';
		yggr::utf8_char utf8_ch = ch;
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((ch += char(3)) == (utf8_ch_ref += 3));
		std::cout << utf8_ch << std::endl;
	}

	{
		char ch = 'a';
		yggr::utf8_char utf8_ch = ch;
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((ch += char(3)) == (utf8_ch_ref += yggr::utf8_char(3)));
		std::cout << utf8_ch << std::endl;
	}

#ifndef YGGR_NO_CWCHAR

	{
		wchar_t ch = 0x007f;
		yggr::utf8_char utf8_ch = ch;
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((ch += wchar_t(3)) == (utf8_ch_ref += 3));
		std::cout << static_cast<yggr::s32>(utf8_ch) << std::endl;
	}

	{
		wchar_t ch = L'Äã';
		yggr::utf8_char utf8_ch = ch;
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((ch += wchar_t(3)) == (utf8_ch_ref += char(3)));
		std::cout << utf8_ch << std::endl;
	}

	{
		wchar_t ch = L'Äã';
		yggr::utf8_char utf8_ch = ch;
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((ch += wchar_t(3)) == (utf8_ch_ref += yggr::utf8_char(3)));
		std::cout << utf8_ch << std::endl;
	}

	{
		wchar_t ch = L'Äã';
		yggr::utf8_char utf8_ch = ch;
		yggr::utf8_char step = 3;
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((ch += 3) == (utf8_ch_ref += static_cast<ref_type>(step)));
		std::cout << utf8_ch << std::endl;
	}

	{
		wchar_t ch = L'Äã';
		yggr::utf8_char utf8_ch = ch;
		yggr::utf8_char step = 3;
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((ch += 3) == (utf8_ch_ref += static_cast<ref_type>(step)));
		std::cout << utf8_ch << std::endl;
	}

	{
		wchar_t ch = L'Äã';
		yggr::utf8_char utf8_ch = ch;
		yggr::utf8_char step = 3;
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((ch += 3) == (utf8_ch_ref += step.view()));
		std::cout << utf8_ch << std::endl;
	}

#endif // YGGR_NO_CWCHAR

	std::cout << "test_plus_set_of_utf8_char_reference succeed" << std::endl;
}

void test_minus_set_of_utf8_char(void)
{
	{
		char ch = 'd';
		yggr::utf8_char utf8_ch = ch;

		yggr_test_assert((ch -= char(3)) == (utf8_ch -= 3));
		std::cout << utf8_ch << std::endl;
	}

	{
		char ch = 'd';
		yggr::utf8_char utf8_ch = ch;

		yggr_test_assert((ch -= char(3)) == (utf8_ch += -static_cast<yggr::s64>(yggr::utf8_char(3))));
		std::cout << utf8_ch << std::endl;

		yggr_test_assert((ch -= char(3)) == (utf8_ch += -static_cast<yggr::s8>(yggr::utf8_char(3))));
	}

#ifndef YGGR_NO_CWCHAR

	{
		wchar_t ch = 0;
		yggr::utf8_char utf8_ch = ch;

		utf8_ch -= 3;
		yggr_test_assert(utf8_ch.is_validate());
		std::cout << static_cast<yggr::s32>(utf8_ch) << std::endl;
	}

	{
		wchar_t ch = L'Ó¶';
		yggr::utf8_char utf8_ch = ch;

		yggr_test_assert((ch -= wchar_t(3)) == (utf8_ch -= char(3)));
		std::cout << utf8_ch << std::endl;
	}

	{
		wchar_t ch = L'Ó¶';
		yggr::utf8_char utf8_ch = ch;

		yggr_test_assert((ch -= wchar_t(3)) == (utf8_ch -= yggr::utf8_char(3)));
		std::cout << utf8_ch << std::endl;
	}

	{
		wchar_t ch = L'Ó¶';
		yggr::utf8_char utf8_ch = ch;
		yggr::utf8_char step = 3;


		yggr_test_assert((ch -= 3) == (utf8_ch -= static_cast<yggr::utf8_char::char_reference_type>(step)));
		std::cout << utf8_ch << std::endl;
	}

	{
		wchar_t ch = L'Ó¶';
		yggr::utf8_char utf8_ch = ch;
		yggr::utf8_char step = 3;


		yggr_test_assert((ch -= 3) == (utf8_ch -= static_cast<yggr::utf8_char::char_const_reference_type>(step)));
		std::cout << utf8_ch << std::endl;
	}

	{
		wchar_t ch = L'Ó¶';
		yggr::utf8_char utf8_ch = ch;
		yggr::utf8_char step = 3;


		yggr_test_assert((ch -= 3) == (utf8_ch -= step.view()));
		std::cout << utf8_ch << std::endl;
	}

#endif // YGGR_NO_CWCHAR

	std::cout << "test_minus_set_of_utf8_char succeed" << std::endl;
}

void test_minus_set_of_utf8_char_reference(void)
{
	typedef yggr::utf8_char::char_reference_type ref_type;

	{
		char ch = 'd';
		yggr::utf8_char utf8_ch = ch;
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((ch -= char(3)) == (utf8_ch_ref -= 3));
		std::cout << utf8_ch << std::endl;
	}

	{
		char ch = 'd';
		yggr::utf8_char utf8_ch = ch;
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((ch -= char(3)) == (utf8_ch_ref += -static_cast<yggr::s64>(yggr::utf8_char(3))));
		std::cout << utf8_ch << std::endl;

		yggr_test_assert((ch -= char(3)) == (utf8_ch_ref += -static_cast<yggr::s8>(yggr::utf8_char(3))));
	}

#ifndef YGGR_NO_CWCHAR

	{
		wchar_t ch = 0;
		yggr::utf8_char utf8_ch = ch;
		ref_type utf8_ch_ref = utf8_ch;

		utf8_ch_ref -= 3;
		yggr_test_assert(utf8_ch.is_validate());
		std::cout << static_cast<yggr::s32>(utf8_ch) << std::endl;
	}

	{
		wchar_t ch = L'Ó¶';
		yggr::utf8_char utf8_ch = ch;
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((ch -= wchar_t(3)) == (utf8_ch_ref -= char(3)));
		std::cout << utf8_ch << std::endl;
	}

	{
		wchar_t ch = L'Ó¶';
		yggr::utf8_char utf8_ch = ch;
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((ch -= wchar_t(3)) == (utf8_ch_ref -= yggr::utf8_char(3)));
		std::cout << utf8_ch << std::endl;
	}

	{
		wchar_t ch = L'Ó¶';
		yggr::utf8_char utf8_ch = ch;
		yggr::utf8_char step = 3;
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((ch -= 3) == (utf8_ch_ref -= static_cast<yggr::utf8_char::char_reference_type>(step)));
		std::cout << utf8_ch << std::endl;
	}

	{
		wchar_t ch = L'Ó¶';
		yggr::utf8_char utf8_ch = ch;
		yggr::utf8_char step = 3;
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((ch -= 3) == (utf8_ch_ref -= static_cast<yggr::utf8_char::char_const_reference_type>(step)));
		std::cout << utf8_ch << std::endl;
	}

	{
		wchar_t ch = L'Ó¶';
		yggr::utf8_char utf8_ch = ch;
		yggr::utf8_char step = 3;
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((ch -= 3) == (utf8_ch_ref -= step.view()));
		std::cout << utf8_ch << std::endl;
	}

#endif // YGGR_NO_CWCHAR

	std::cout << "test_minus_set_of_utf8_char_reference succeed" << std::endl;
}

void test_mul_set_of_utf8_char(void)
{

	{
		yggr::utf8_char utf8_ch(2);
		yggr_test_assert((utf8_ch *= 48.5) == 'a');
		yggr_test_assert((utf8_ch *= 0) == 0);
		yggr_test_assert((utf8_ch *= yggr::utf8_char(49)) == 0);
	}

	{
		yggr::utf8_char utf8_ch(yggr::charset::utf8_foo::E_char_max_num);
		utf8_ch *= 2;
		yggr_test_assert(utf8_ch.is_validate());
		yggr_test_assert(utf8_ch == yggr::charset::utf8_foo::E_char_max_num
							+ yggr::charset::utf8_foo::E_char_max_num
							- yggr::charset::utf8_foo::E_char_max_range);
	}

	{
		yggr::utf8_char utf8_ch(2);
		yggr_test_assert((utf8_ch *= yggr::utf8_char(49)) == 'b');
	}

	{
		yggr::utf8_char utf8_ch(2);
		yggr_test_assert((utf8_ch *= yggr::utf8_char(49).view()) == 'b');
	}

	{
		yggr::utf8_char utf8_ch(2);
		yggr::utf8_char utf8_ch2(49);
		yggr_test_assert((utf8_ch *= static_cast<yggr::utf8_char::char_reference_type>(utf8_ch2)) == 'b');
	}

	{
		yggr::utf8_char utf8_ch(2);
		yggr_test_assert((utf8_ch *= static_cast<yggr::utf8_char::char_const_reference_type>(yggr::utf8_char(49))) == 'b');
	}

#ifndef YGGR_NO_CWCHAR
	{
		yggr::utf8_char utf8_ch(2);
		yggr_test_assert((utf8_ch *= (L'Äã' / 2)) == L'Äã');
		yggr_test_assert((utf8_ch *= 0) == 0);
		yggr_test_assert((utf8_ch *= yggr::utf8_char(49)) == 0);
	}

	{
		wchar_t t = L'Äã';
		yggr_test_assert(t % 2 == 0);
		yggr::utf8_char utf8_ch(2);
		yggr_test_assert((utf8_ch *= yggr::utf8_char((L'Äã' / 2))) == L'Äã');
	}

	{
		wchar_t t = L'Äã';
		yggr_test_assert(t % 2 == 0);
		yggr::utf8_char utf8_ch(2);
		yggr_test_assert((utf8_ch *= yggr::utf8_char((L'Äã' / 2)).view()) == L'Äã');
	}

	{
		wchar_t t = L'Äã';
		yggr_test_assert(t % 2 == 0);
		yggr::utf8_char utf8_ch(2);
		yggr::utf8_char utf8_ch2(L'Äã' / 2);
		yggr_test_assert((utf8_ch *= static_cast<yggr::utf8_char::char_reference_type>(utf8_ch2)) == L'Äã');
	}

	{
		wchar_t t = L'Äã';
		yggr_test_assert(t % 2 == 0);
		yggr::utf8_char utf8_ch(2);
		yggr_test_assert((utf8_ch *= static_cast<yggr::utf8_char::char_const_reference_type>(yggr::utf8_char((L'Äã' / 2)))) == L'Äã');
	}
#endif // YGGR_NO_CWCHAR

	std::cout << "test_mul_set_of_utf8_char succeed" << std::endl;
}

void test_mul_set_of_utf8_char_reference(void)
{
	typedef yggr::utf8_char::char_reference_type ref_type;

	{
		yggr::utf8_char utf8_ch(2);
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((utf8_ch_ref *= 48.5) == 'a');
		yggr_test_assert((utf8_ch_ref *= 0) == 0);
		yggr_test_assert((utf8_ch_ref *= yggr::utf8_char(49)) == 0);
	}

	{
		yggr::utf8_char utf8_ch(yggr::charset::utf8_foo::E_char_max_num);
		ref_type utf8_ch_ref = utf8_ch;

		utf8_ch_ref *= 2;
		yggr_test_assert(utf8_ch.is_validate());
		yggr_test_assert(utf8_ch == yggr::charset::utf8_foo::E_char_max_num
							+ yggr::charset::utf8_foo::E_char_max_num
							- yggr::charset::utf8_foo::E_char_max_range);
	}

	{
		yggr::utf8_char utf8_ch(2);
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((utf8_ch_ref *= yggr::utf8_char(49)) == 'b');
	}

	{
		yggr::utf8_char utf8_ch(2);
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((utf8_ch_ref *= yggr::utf8_char(49).view()) == 'b');
	}

	{
		yggr::utf8_char utf8_ch(2);
		ref_type utf8_ch_ref = utf8_ch;
		yggr::utf8_char utf8_ch2(49);

		yggr_test_assert((utf8_ch_ref *= static_cast<yggr::utf8_char::char_reference_type>(utf8_ch2)) == 'b');
	}

	{
		yggr::utf8_char utf8_ch(2);
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((utf8_ch_ref *= static_cast<yggr::utf8_char::char_const_reference_type>(yggr::utf8_char(49))) == 'b');
	}

#ifndef YGGR_NO_CWCHAR
	{
		yggr::utf8_char utf8_ch(2);
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((utf8_ch_ref *= (L'Äã' / 2)) == L'Äã');
		yggr_test_assert((utf8_ch_ref *= 0) == 0);
		yggr_test_assert((utf8_ch_ref *= yggr::utf8_char(49)) == 0);
	}

	{
		wchar_t t = L'Äã';
		yggr_test_assert(t % 2 == 0);
		yggr::utf8_char utf8_ch(2);
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((utf8_ch_ref *= yggr::utf8_char((L'Äã' / 2))) == L'Äã');
	}

	{
		wchar_t t = L'Äã';
		yggr_test_assert(t % 2 == 0);
		yggr::utf8_char utf8_ch(2);
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((utf8_ch_ref *= yggr::utf8_char((L'Äã' / 2)).view()) == L'Äã');
	}

	{
		wchar_t t = L'Äã';
		yggr_test_assert(t % 2 == 0);
		yggr::utf8_char utf8_ch(2);
		ref_type utf8_ch_ref = utf8_ch;
		yggr::utf8_char utf8_ch2(L'Äã' / 2);

		yggr_test_assert((utf8_ch_ref *= static_cast<yggr::utf8_char::char_reference_type>(yggr::utf8_char(utf8_ch2))) == L'Äã');
	}

	{
		wchar_t t = L'Äã';
		yggr_test_assert(t % 2 == 0);
		yggr::utf8_char utf8_ch(2);
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((utf8_ch_ref *= static_cast<yggr::utf8_char::char_const_reference_type>(yggr::utf8_char((L'Äã' / 2)))) == L'Äã');
	}
#endif // YGGR_NO_CWCHAR

	std::cout << "test_mul_set_of_utf8_char_reference succeed" << std::endl;
}

void test_div_set_of_utf8_char(void)
{
	{
		yggr::utf8_char utf8_ch('a' * 2);
		yggr_test_assert((utf8_ch /= 2) == 'a');
		yggr_test_assert((utf8_ch /= yggr::charset::utf8_foo::E_char_max_range) == 0);
#if 0
		yggr_test_assert((utf8_ch /= 0) == 0); // div 0 exception
#endif // 0, 1
	}

	{
		yggr::utf8_char utf8_ch(0);
		yggr_test_assert((utf8_ch /= yggr::utf8_char(2)) == 0);
	}

	{
		yggr::utf8_char utf8_ch('a' * 2);
		yggr_test_assert((utf8_ch /= yggr::utf8_char(2)) == 'a');
	}

	{
		yggr::utf8_char utf8_ch('a' * 2);
		yggr_test_assert((utf8_ch /= yggr::utf8_char(2).view()) == 'a');
	}

	{
		yggr::utf8_char utf8_ch('a' * 2);
		yggr::utf8_char utf8_ch2(2);
		yggr_test_assert((utf8_ch /= static_cast<yggr::utf8_char::char_reference_type>(utf8_ch2)) == 'a');
	}

	{
		yggr::utf8_char utf8_ch('a' * 2);
		yggr_test_assert((utf8_ch /= static_cast<yggr::utf8_char::char_const_reference_type>(yggr::utf8_char(2))) == 'a');
	}

#ifndef YGGR_NO_CWCHAR

	{
		yggr::utf8_char utf8_ch(L'Äã');
		yggr_test_assert((utf8_ch /= yggr::charset::utf8_foo::E_char_max_range) == 0);
	}

	{
		yggr::utf8_char utf8_ch(L'Äã' * 2);
		yggr_test_assert((utf8_ch /= 2) == L'Äã');
	}

	{
		yggr::utf8_char utf8_ch(L'Äã' * 2);
		yggr_test_assert((utf8_ch /= yggr::utf8_char(2)) == L'Äã');
	}

	{
		yggr::utf8_char utf8_ch(L'Äã' * 2);
		yggr_test_assert((utf8_ch /= yggr::utf8_char(2).view()) == L'Äã');
	}

	{
		yggr::utf8_char utf8_ch(L'Äã' * 2);
		yggr::utf8_char utf8_ch2(2);
		yggr_test_assert((utf8_ch /= static_cast<yggr::utf8_char::char_reference_type>(utf8_ch2)) == L'Äã');
	}

	{
		yggr::utf8_char utf8_ch(L'a' * 2);
		yggr_test_assert((utf8_ch /= static_cast<yggr::utf8_char::char_const_reference_type>(yggr::utf8_char(2))) == L'a');
	}
#endif // YGGR_NO_CWCHAR

	std::cout << "test_div_set_of_utf8_char succeed" << std::endl;
}

void test_div_set_of_utf8_char_reference(void)
{
	typedef yggr::utf8_char::char_reference_type ref_type;

	{
		yggr::utf8_char utf8_ch('a' * 2);
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((utf8_ch_ref /= 2) == 'a');
		yggr_test_assert((utf8_ch_ref /= yggr::charset::utf8_foo::E_char_max_range) == 0);
#if 0
		yggr_test_assert((utf8_ch /= 0) == 0); // div 0 exception
#endif // 0, 1
	}

	{
		yggr::utf8_char utf8_ch(0);
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((utf8_ch_ref /= yggr::utf8_char(2)) == 0);
	}

	{
		yggr::utf8_char utf8_ch('a' * 2);
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((utf8_ch_ref /= yggr::utf8_char(2)) == 'a');
	}

	{
		yggr::utf8_char utf8_ch('a' * 2);
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((utf8_ch_ref /= yggr::utf8_char(2).view()) == 'a');
	}

	{
		yggr::utf8_char utf8_ch('a' * 2);
		ref_type utf8_ch_ref = utf8_ch;
		yggr::utf8_char utf8_ch2(2);

		yggr_test_assert((utf8_ch_ref /= static_cast<yggr::utf8_char::char_reference_type>(utf8_ch2)) == 'a');
	}

	{
		yggr::utf8_char utf8_ch('a' * 2);
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((utf8_ch_ref /= static_cast<yggr::utf8_char::char_const_reference_type>(yggr::utf8_char(2))) == 'a');
	}

#ifndef YGGR_NO_CWCHAR

	{
		yggr::utf8_char utf8_ch(L'Äã');
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((utf8_ch_ref /= yggr::charset::utf8_foo::E_char_max_range) == 0);
	}

	{
		yggr::utf8_char utf8_ch(L'Äã' * 2);
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((utf8_ch_ref /= 2) == L'Äã');
	}

	{
		yggr::utf8_char utf8_ch(L'Äã' * 2);
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((utf8_ch_ref /= yggr::utf8_char(2)) == L'Äã');
	}

	{
		yggr::utf8_char utf8_ch(L'Äã' * 2);
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((utf8_ch_ref /= yggr::utf8_char(2).view()) == L'Äã');
	}

	{
		yggr::utf8_char utf8_ch(L'Äã' * 2);
		ref_type utf8_ch_ref = utf8_ch;
		yggr::utf8_char utf8_ch2(2);

		yggr_test_assert((utf8_ch_ref /= static_cast<yggr::utf8_char::char_reference_type>(utf8_ch2)) == L'Äã');
	}

	{
		yggr::utf8_char utf8_ch(L'a' * 2);
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((utf8_ch_ref /= static_cast<yggr::utf8_char::char_const_reference_type>(yggr::utf8_char(2))) == L'a');
	}
#endif // YGGR_NO_CWCHAR

	std::cout << "test_div_set_of_utf8_char_reference succeed" << std::endl;
}

void test_mod_set_of_utf8_char(void)
{
	{
		yggr::utf8_char utf8_ch('a');
		yggr_test_assert((utf8_ch %= 'b') == 'a');
		yggr_test_assert((utf8_ch %= 2 ) == 1);
#if 0
		yggr_test_assert((utf8_ch %= 0) == 0); // div 0 exception
#endif // 0, 1
	}

	{
		yggr::utf8_char utf8_ch(0);
		yggr_test_assert((utf8_ch %= yggr::utf8_char(2)) == 0);
	}

	{
		yggr::utf8_char utf8_ch('a');
		yggr_test_assert((utf8_ch %= yggr::utf8_char(2)) == 1);
	}

	{
		yggr::utf8_char utf8_ch('a');
		yggr_test_assert((utf8_ch %= yggr::utf8_char(2).view()) == 1);
	}

	{
		yggr::utf8_char utf8_ch('a');
		yggr::utf8_char utf8_ch2(2);
		yggr_test_assert((utf8_ch %= static_cast<yggr::utf8_char::char_reference_type>(yggr::utf8_char(2))) == 1);
	}

	{
		yggr::utf8_char utf8_ch('a');
		yggr_test_assert((utf8_ch %= static_cast<yggr::utf8_char::char_const_reference_type>(yggr::utf8_char(2))) == 1);
	}

#ifndef YGGR_NO_CWCHAR
	{
		yggr::utf8_char utf8_ch(L'Äã');
		yggr_test_assert((utf8_ch %= L'Äã' + 1) == L'Äã');
		yggr_test_assert((utf8_ch %= 2 ) == 0);
	}

	{
		yggr::utf8_char utf8_ch(L'Äã');
		yggr_test_assert((utf8_ch %= yggr::utf8_char(2)) == 0);
	}

	{
		yggr::utf8_char utf8_ch(L'Äã');
		yggr_test_assert((utf8_ch %= yggr::utf8_char(2).view()) == 0);
	}

	{
		yggr::utf8_char utf8_ch(L'Äã');
		yggr::utf8_char utf8_ch2(2);
		yggr_test_assert((utf8_ch %= static_cast<yggr::utf8_char::char_reference_type>(utf8_ch2)) == 0);
	}

	{
		yggr::utf8_char utf8_ch(L'Äã');
		yggr_test_assert((utf8_ch %= static_cast<yggr::utf8_char::char_const_reference_type>(yggr::utf8_char(2))) == 0);
	}
#endif // YGGR_NO_CWCHAR

	std::cout << "test_mod_set_of_utf8_char succeed" << std::endl;
}

void test_mod_set_of_utf8_char_reference(void)
{
	typedef yggr::utf8_char::char_reference_type ref_type;

	{
		yggr::utf8_char utf8_ch('a');
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((utf8_ch_ref %= 'b') == 'a');
		yggr_test_assert((utf8_ch_ref %= 2 ) == 1);
#if 0
		yggr_test_assert((utf8_ch_ref %= 0) == 0); // div 0 exception
#endif // 0, 1
	}

	{
		yggr::utf8_char utf8_ch(0);
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((utf8_ch_ref %= yggr::utf8_char(2)) == 0);
	}

	{
		yggr::utf8_char utf8_ch('a');
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((utf8_ch_ref %= yggr::utf8_char(2)) == 1);
	}

	{
		yggr::utf8_char utf8_ch('a');
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((utf8_ch_ref %= yggr::utf8_char(2).view()) == 1);
	}

	{
		yggr::utf8_char utf8_ch('a');
		ref_type utf8_ch_ref = utf8_ch;
		yggr::utf8_char utf8_ch2(2);

		yggr_test_assert((utf8_ch_ref %= static_cast<yggr::utf8_char::char_reference_type>(utf8_ch2)) == 1);
	}

	{
		yggr::utf8_char utf8_ch('a');
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((utf8_ch_ref %= static_cast<yggr::utf8_char::char_const_reference_type>(yggr::utf8_char(2))) == 1);
	}

#ifndef YGGR_NO_CWCHAR
	{
		yggr::utf8_char utf8_ch(L'Äã');
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((utf8_ch_ref %= L'Äã' + 1) == L'Äã');
		yggr_test_assert((utf8_ch_ref %= 2 ) == 0);
	}

	{
		yggr::utf8_char utf8_ch(L'Äã');
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((utf8_ch_ref %= yggr::utf8_char(2)) == 0);
	}

	{
		yggr::utf8_char utf8_ch(L'Äã');
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((utf8_ch_ref %= yggr::utf8_char(2).view()) == 0);
	}

	{
		yggr::utf8_char utf8_ch(L'Äã');
		ref_type utf8_ch_ref = utf8_ch;
		yggr::utf8_char utf8_ch2(2);

		yggr_test_assert((utf8_ch_ref %= static_cast<yggr::utf8_char::char_reference_type>(utf8_ch2)) == 0);
	}

	{
		yggr::utf8_char utf8_ch(L'Äã');
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((utf8_ch_ref %= static_cast<yggr::utf8_char::char_const_reference_type>(yggr::utf8_char(2))) == 0);
	}
#endif // YGGR_NO_CWCHAR

	std::cout << "test_mod_set_of_utf8_char_reference succeed" << std::endl;
}

void test_bitwise_left_shift_set_of_utf8_char(void)
{
	{
		yggr::utf8_char utf8_ch(49);
		yggr_test_assert((utf8_ch <<= 1) == 'b');
	}

	{
		yggr::utf8_char utf8_ch(yggr::charset::utf8_foo::E_char_max_num);
		utf8_ch <<= 1;
		yggr_test_assert(utf8_ch.is_validate());
		yggr_test_assert(utf8_ch == yggr::charset::utf8_foo::E_char_max_num
							+ yggr::charset::utf8_foo::E_char_max_num
							- yggr::charset::utf8_foo::E_char_max_range);
	}

	{
		yggr::utf8_char utf8_ch(49);
		yggr_test_assert((utf8_ch <<= yggr::utf8_char(1)) == 'b');
	}

	{
		yggr::utf8_char utf8_ch(49);
		yggr_test_assert((utf8_ch <<= yggr::utf8_char(1).view()) == 'b');
	}

	{
		yggr::utf8_char utf8_ch(49);
		yggr::utf8_char utf8_ch2(1);
		yggr_test_assert((utf8_ch <<= static_cast<yggr::utf8_char::char_reference_type>(utf8_ch2)) == 'b');
	}

	{
		yggr::utf8_char utf8_ch(49);
		yggr_test_assert((utf8_ch <<= static_cast<yggr::utf8_char::char_const_reference_type>(yggr::utf8_char(1))) == 'b');
	}

#ifndef YGGR_NO_CWCHAR
	{
		yggr::utf8_char utf8_ch(L'Äã');
		yggr_test_assert((utf8_ch <<= 1) == L'Äã' * 2);
	}

	{
		yggr::utf8_char utf8_ch(L'Äã');
		yggr_test_assert((utf8_ch <<= yggr::utf8_char(1)) == L'Äã' * 2);
	}

	{
		yggr::utf8_char utf8_ch(L'Äã');
		yggr_test_assert((utf8_ch <<= yggr::utf8_char(1).view()) == L'Äã' * 2);
	}

	{
		yggr::utf8_char utf8_ch(L'Äã');
		yggr::utf8_char utf8_ch2(1);
		yggr_test_assert((utf8_ch <<= static_cast<yggr::utf8_char::char_reference_type>(yggr::utf8_char(1))) == L'Äã' * 2);
	}

	{
		yggr::utf8_char utf8_ch(L'Äã');
		yggr_test_assert((utf8_ch <<= static_cast<yggr::utf8_char::char_const_reference_type>(yggr::utf8_char(1))) == L'Äã' * 2);
	}
#endif // YGGR_NO_CWCHAR

	std::cout << "test_bitwise_left_shift_set_of_utf8_char succeed" << std::endl;
}

void test_bitwise_left_shift_set_of_utf8_char_reference(void)
{
	typedef yggr::utf8_char::char_reference_type ref_type;

	{
		yggr::utf8_char utf8_ch(49);
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((utf8_ch_ref <<= 1) == 'b');
	}

	{
		yggr::utf8_char utf8_ch(yggr::charset::utf8_foo::E_char_max_num);
		ref_type utf8_ch_ref = utf8_ch;

		utf8_ch_ref <<= 1;
		yggr_test_assert(utf8_ch.is_validate());
		yggr_test_assert(utf8_ch == yggr::charset::utf8_foo::E_char_max_num
							+ yggr::charset::utf8_foo::E_char_max_num
							- yggr::charset::utf8_foo::E_char_max_range);
	}

	{
		yggr::utf8_char utf8_ch(49);
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((utf8_ch_ref <<= yggr::utf8_char(1)) == 'b');
	}

	{
		yggr::utf8_char utf8_ch(49);
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((utf8_ch_ref <<= yggr::utf8_char(1).view()) == 'b');
	}

	{
		yggr::utf8_char utf8_ch(49);
		ref_type utf8_ch_ref = utf8_ch;
		yggr::utf8_char utf8_ch2(1);

		yggr_test_assert((utf8_ch_ref <<= static_cast<yggr::utf8_char::char_reference_type>(utf8_ch2)) == 'b');
	}

	{
		yggr::utf8_char utf8_ch(49);
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((utf8_ch_ref <<= static_cast<yggr::utf8_char::char_const_reference_type>(yggr::utf8_char(1))) == 'b');
	}

#ifndef YGGR_NO_CWCHAR
	{
		yggr::utf8_char utf8_ch(L'Äã');
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((utf8_ch_ref <<= 1) == L'Äã' * 2);
	}

	{
		yggr::utf8_char utf8_ch(L'Äã');
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((utf8_ch_ref <<= yggr::utf8_char(1)) == L'Äã' * 2);
	}

	{
		yggr::utf8_char utf8_ch(L'Äã');
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((utf8_ch_ref <<= yggr::utf8_char(1).view()) == L'Äã' * 2);
	}

	{
		yggr::utf8_char utf8_ch(L'Äã');
		ref_type utf8_ch_ref = utf8_ch;
		yggr::utf8_char utf8_ch2(1);

		yggr_test_assert((utf8_ch_ref <<= static_cast<yggr::utf8_char::char_reference_type>(yggr::utf8_char(1))) == L'Äã' * 2);
	}

	{
		yggr::utf8_char utf8_ch(L'Äã');
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((utf8_ch_ref <<= static_cast<yggr::utf8_char::char_const_reference_type>(yggr::utf8_char(1))) == L'Äã' * 2);
	}
#endif // YGGR_NO_CWCHAR

	std::cout << "test_bitwise_left_shift_set_of_utf8_char_reference succeed" << std::endl;
}

void test_bitwise_right_shift_set_of_utf8_char(void)
{
	{
		yggr::utf8_char utf8_ch('a' * 2);
		yggr_test_assert((utf8_ch >>= 1) == 'a');
	}

	{
		yggr::utf8_char utf8_ch(1);
		utf8_ch >>= 1;
		yggr_test_assert(utf8_ch.is_validate());
		yggr_test_assert(utf8_ch == 0);
	}

	{
		yggr::utf8_char utf8_ch('a' * 2);
		yggr_test_assert((utf8_ch >>= yggr::utf8_char(1)) == 'a');
	}

	{
		yggr::utf8_char utf8_ch('a' * 2);
		yggr_test_assert((utf8_ch >>= yggr::utf8_char(1).view()) == 'a');
	}

	{
		yggr::utf8_char utf8_ch('a' * 2);
		yggr::utf8_char utf8_ch2(1);
		yggr_test_assert((utf8_ch >>= static_cast<yggr::utf8_char::char_reference_type>(utf8_ch2)) == 'a');
	}

	{
		yggr::utf8_char utf8_ch('a' * 2);
		yggr_test_assert((utf8_ch >>= static_cast<yggr::utf8_char::char_const_reference_type>(yggr::utf8_char(1))) == 'a');
	}

#ifndef YGGR_NO_CWCHAR
	{
		yggr::utf8_char utf8_ch(L'Äã' * 2);
		yggr_test_assert((utf8_ch >>= 1) == L'Äã');
	}

	{
		yggr::utf8_char utf8_ch(L'Äã' * 2);
		yggr_test_assert((utf8_ch >>= yggr::utf8_char(1)) == L'Äã');
	}

	{
		yggr::utf8_char utf8_ch(L'Äã' * 2);
		yggr_test_assert((utf8_ch >>= yggr::utf8_char(1).view()) == L'Äã');
	}

	{
		yggr::utf8_char utf8_ch(L'Äã' * 2);
		yggr::utf8_char utf8_ch2(1);
		yggr_test_assert((utf8_ch >>= static_cast<yggr::utf8_char::char_reference_type>(utf8_ch2)) == L'Äã');
	}

	{
		yggr::utf8_char utf8_ch(L'Äã' * 2);
		yggr_test_assert((utf8_ch >>= static_cast<yggr::utf8_char::char_const_reference_type>(yggr::utf8_char(1))) == L'Äã');
	}
#endif // YGGR_NO_CWCHAR

	std::cout << "test_bitwise_right_shift_set_of_utf8_char succeed" << std::endl;
}

void test_bitwise_right_shift_set_of_utf8_char_reference(void)
{
	typedef yggr::utf8_char::char_reference_type ref_type;

	{
		yggr::utf8_char utf8_ch('a' * 2);
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((utf8_ch_ref >>= 1) == 'a');
	}

	{
		yggr::utf8_char utf8_ch(1);
		ref_type utf8_ch_ref = utf8_ch;

		utf8_ch_ref >>= 1;
		yggr_test_assert(utf8_ch.is_validate());
		yggr_test_assert(utf8_ch == 0);
	}

	{
		yggr::utf8_char utf8_ch('a' * 2);
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((utf8_ch_ref >>= yggr::utf8_char(1)) == 'a');
	}

	{
		yggr::utf8_char utf8_ch('a' * 2);
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((utf8_ch_ref >>= yggr::utf8_char(1).view()) == 'a');
	}

	{
		yggr::utf8_char utf8_ch('a' * 2);
		ref_type utf8_ch_ref = utf8_ch;
		yggr::utf8_char utf8_ch2(1);

		yggr_test_assert((utf8_ch_ref >>= static_cast<yggr::utf8_char::char_reference_type>(utf8_ch2)) == 'a');
	}

	{
		yggr::utf8_char utf8_ch('a' * 2);
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((utf8_ch_ref >>= static_cast<yggr::utf8_char::char_const_reference_type>(yggr::utf8_char(1))) == 'a');
	}

#ifndef YGGR_NO_CWCHAR
	{
		yggr::utf8_char utf8_ch(L'Äã' * 2);
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((utf8_ch_ref >>= 1) == L'Äã');
	}

	{
		yggr::utf8_char utf8_ch(L'Äã' * 2);
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((utf8_ch_ref >>= yggr::utf8_char(1)) == L'Äã');
	}

	{
		yggr::utf8_char utf8_ch(L'Äã' * 2);
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((utf8_ch_ref >>= yggr::utf8_char(1).view()) == L'Äã');
	}

	{
		yggr::utf8_char utf8_ch(L'Äã' * 2);
		ref_type utf8_ch_ref = utf8_ch;
		yggr::utf8_char utf8_ch2(1);

		yggr_test_assert((utf8_ch_ref >>= static_cast<yggr::utf8_char::char_reference_type>(yggr::utf8_char(1))) == L'Äã');
	}

	{
		yggr::utf8_char utf8_ch(L'Äã' * 2);
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((utf8_ch_ref >>= static_cast<yggr::utf8_char::char_const_reference_type>(yggr::utf8_char(1))) == L'Äã');
	}
#endif // YGGR_NO_CWCHAR

	std::cout << "test_bitwise_right_shift_set_of_utf8_char_reference succeed" << std::endl;
}

void test_bitwise_and_set_of_utf8_char(void)
{
	{
		char ch = 'a';
		yggr::utf8_char utf8_ch('a');

		yggr_test_assert((ch &= utf8_ch) == utf8_ch);

		yggr_test_assert((ch &= yggr::utf8_char('b')) == ('a' & 'b'));

#if 0
		utf8_ch &= ch; // not support
#endif // 0, 1
	}

#ifndef YGGR_NO_CWCHAR
	{
		wchar_t ch = L'Äã';
		yggr::utf8_char utf8_ch(L'Äã');

		yggr_test_assert((ch &= utf8_ch) == utf8_ch);
		yggr_test_assert((ch &= yggr::utf8_char(L'ºÃ')) == (L'Äã' & L'ºÃ'));
	}
#endif // YGGR_NO_CWCHAR

	std::cout << "test_bitwise_and_set_of_utf8_char succeed" << std::endl;
}

void test_bitwise_and_set_of_utf8_char_reference(void)
{
	typedef yggr::utf8_char::char_reference_type ref_type;

	{
		char ch = 'a';
		yggr::utf8_char utf8_ch('a');
		yggr::utf8_char utf8_ch2('b');
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((ch &= utf8_ch_ref) == utf8_ch_ref);

		yggr_test_assert((ch &= ref_type(utf8_ch2)) == ('a' & 'b'));

#if 0
		utf8_ch_ref &= ch; // not support
#endif // 0, 1
	}

#ifndef YGGR_NO_CWCHAR
	{
		wchar_t ch = L'Äã';
		yggr::utf8_char utf8_ch(L'Äã');
		yggr::utf8_char utf8_ch2(L'ºÃ');
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((ch &= utf8_ch_ref) == utf8_ch);
		yggr_test_assert((ch &= ref_type(utf8_ch2)) == (L'Äã' & L'ºÃ'));
	}
#endif // YGGR_NO_CWCHAR

	std::cout << "test_bitwise_and_set_of_utf8_char_reference succeed" << std::endl;
}

void test_bitwise_and_set_of_utf8_char_const_reference(void)
{
	typedef yggr::utf8_char::char_const_reference_type ref_type;

	{
		char ch = 'a';
		yggr::utf8_char utf8_ch('a');
		yggr::utf8_char utf8_ch2('b');
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((ch &= utf8_ch_ref) == utf8_ch_ref);

		yggr_test_assert((ch &= ref_type(utf8_ch2)) == ('a' & 'b'));

#if 0
		utf8_ch_ref &= ch; // not support
#endif // 0, 1
	}

#ifndef YGGR_NO_CWCHAR
	{
		wchar_t ch = L'Äã';
		yggr::utf8_char utf8_ch(L'Äã');
		yggr::utf8_char utf8_ch2(L'ºÃ');
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((ch &= utf8_ch_ref) == utf8_ch);
		yggr_test_assert((ch &= ref_type(utf8_ch2)) == (L'Äã' & L'ºÃ'));
	}
#endif // YGGR_NO_CWCHAR

	std::cout << "test_bitwise_and_set_of_utf8_char_const_reference succeed" << std::endl;
}

void test_bitwise_and_set_of_utf8_char_view(void)
{
	typedef yggr::utf8_char::char_view_type ref_type;

	{
		char ch = 'a';
		yggr::utf8_char utf8_ch('a');
		yggr::utf8_char utf8_ch2('b');
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((ch &= utf8_ch_ref) == utf8_ch_ref);

		yggr_test_assert((ch &= ref_type(utf8_ch2)) == ('a' & 'b'));

#if 0
		utf8_ch_ref &= ch; // not support
#endif // 0, 1
	}

#ifndef YGGR_NO_CWCHAR
	{
		wchar_t ch = L'Äã';
		yggr::utf8_char utf8_ch(L'Äã');
		yggr::utf8_char utf8_ch2(L'ºÃ');
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((ch &= utf8_ch_ref) == utf8_ch);
		yggr_test_assert((ch &= ref_type(utf8_ch2)) == (L'Äã' & L'ºÃ'));
	}
#endif // YGGR_NO_CWCHAR

	std::cout << "test_bitwise_and_set_of_utf8_char_view succeed" << std::endl;
}

void test_bitwise_or_set_of_utf8_char(void)
{
	{
		char ch = 'a';
		yggr::utf8_char utf8_ch('a');

		yggr_test_assert((ch |= utf8_ch) == utf8_ch);

		yggr_test_assert((ch |= yggr::utf8_char('b')) == ('a' | 'b'));

#if 0
		utf8_ch |= ch; // not support
#endif // 0, 1
	}

#ifndef YGGR_NO_CWCHAR
	{
		wchar_t ch = L'Äã';
		yggr::utf8_char utf8_ch(L'Äã');

		yggr_test_assert((ch |= utf8_ch) == utf8_ch);
		yggr_test_assert((ch |= yggr::utf8_char(L'ºÃ')) == (L'Äã' | L'ºÃ'));
	}
#endif // YGGR_NO_CWCHAR

	std::cout << "test_bitwise_or_set_of_utf8_char succeed" << std::endl;
}

void test_bitwise_or_set_of_utf8_char_reference(void)
{
	typedef yggr::utf8_char::char_reference_type ref_type;

	{
		char ch = 'a';
		yggr::utf8_char utf8_ch('a');
		yggr::utf8_char utf8_ch2('b');
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((ch |= utf8_ch_ref) == utf8_ch);

		yggr_test_assert((ch |= ref_type(utf8_ch2)) == ('a' | 'b'));

#if 0
		utf8_ch_ref |= ch; // not support
#endif // 0, 1
	}

#ifndef YGGR_NO_CWCHAR
	{
		wchar_t ch = L'Äã';
		yggr::utf8_char utf8_ch(L'Äã');
		yggr::utf8_char utf8_ch2(L'ºÃ');
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((ch |= utf8_ch_ref) == utf8_ch);
		yggr_test_assert((ch |= ref_type(utf8_ch2)) == (L'Äã' | L'ºÃ'));
	}
#endif // YGGR_NO_CWCHAR

	std::cout << "test_bitwise_or_set_of_utf8_char_reference succeed" << std::endl;
}

void test_bitwise_or_set_of_utf8_char_const_reference(void)
{
	typedef yggr::utf8_char::char_const_reference_type ref_type;

	{
		char ch = 'a';
		yggr::utf8_char utf8_ch('a');
		yggr::utf8_char utf8_ch2('b');
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((ch |= utf8_ch_ref) == utf8_ch);

		yggr_test_assert((ch |= ref_type(utf8_ch2)) == ('a' | 'b'));

#if 0
		utf8_ch_ref |= ch; // not support
#endif // 0, 1
	}

#ifndef YGGR_NO_CWCHAR
	{
		wchar_t ch = L'Äã';
		yggr::utf8_char utf8_ch(L'Äã');
		yggr::utf8_char utf8_ch2(L'ºÃ');
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((ch |= utf8_ch_ref) == utf8_ch);
		yggr_test_assert((ch |= ref_type(utf8_ch2)) == (L'Äã' | L'ºÃ'));
	}
#endif // YGGR_NO_CWCHAR

	std::cout << "test_bitwise_or_set_of_utf8_char_const_reference succeed" << std::endl;
}

void test_bitwise_or_set_of_utf8_char_view(void)
{
	typedef yggr::utf8_char::char_view_type ref_type;

	{
		char ch = 'a';
		yggr::utf8_char utf8_ch('a');
		yggr::utf8_char utf8_ch2('b');
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((ch |= utf8_ch_ref) == utf8_ch);

		yggr_test_assert((ch |= ref_type(utf8_ch2)) == ('a' | 'b'));

#if 0
		utf8_ch_ref |= ch; // not support
#endif // 0, 1
	}

#ifndef YGGR_NO_CWCHAR
	{
		wchar_t ch = L'Äã';
		yggr::utf8_char utf8_ch(L'Äã');
		yggr::utf8_char utf8_ch2(L'ºÃ');
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((ch |= utf8_ch_ref) == utf8_ch);
		yggr_test_assert((ch |= ref_type(utf8_ch2)) == (L'Äã' | L'ºÃ'));
	}
#endif // YGGR_NO_CWCHAR

	std::cout << "test_bitwise_or_set_of_utf8_char_view succeed" << std::endl;
}

void test_bitwise_xor_set_of_utf8_char(void)
{
	{
		char ch = 'a';
		yggr::utf8_char utf8_ch('a');

		yggr_test_assert((ch ^= utf8_ch) == 0);

		yggr_test_assert((ch ^= yggr::utf8_char('b')) == (0 ^ 'b'));

#if 0
		utf8_ch ^= ch; // not support
#endif // 0, 1
	}

#ifndef YGGR_NO_CWCHAR
	{
		wchar_t ch = L'Äã';
		yggr::utf8_char utf8_ch(L'Äã');

		yggr_test_assert((ch ^= utf8_ch) == 0);
		yggr_test_assert((ch ^= yggr::utf8_char(L'ºÃ')) == (0 ^ L'ºÃ'));
	}
#endif // YGGR_NO_CWCHAR

	std::cout << "test_bitwise_xor_set_of_utf8_char succeed" << std::endl;
}

void test_bitwise_xor_set_of_utf8_char_reference(void)
{
	typedef yggr::utf8_char::char_reference_type ref_type;

	{
		char ch = 'a';
		yggr::utf8_char utf8_ch('a');
		yggr::utf8_char utf8_ch2('b');
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((ch ^= utf8_ch_ref) == 0);
		yggr_test_assert((ch ^= ref_type(utf8_ch2)) == (0 ^ 'b'));

#if 0
		utf8_ch_ref ^= ch; // not support
#endif // 0, 1
	}

#ifndef YGGR_NO_CWCHAR
	{
		wchar_t ch = L'Äã';
		yggr::utf8_char utf8_ch(L'Äã');
		yggr::utf8_char utf8_ch2(L'ºÃ');
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((ch ^= utf8_ch_ref) == 0);
		yggr_test_assert((ch ^= ref_type(utf8_ch2)) == (0 ^ L'ºÃ'));
	}
#endif // YGGR_NO_CWCHAR

	std::cout << "test_bitwise_xor_set_of_utf8_char_reference succeed" << std::endl;
}

void test_bitwise_xor_set_of_utf8_char_const_reference(void)
{
	typedef yggr::utf8_char::char_const_reference_type ref_type;

	{
		char ch = 'a';
		yggr::utf8_char utf8_ch('a');
		yggr::utf8_char utf8_ch2('b');
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((ch ^= utf8_ch_ref) == 0);
		yggr_test_assert((ch ^= ref_type(utf8_ch2)) == (0 ^ 'b'));

#if 0
		utf8_ch &= ch; // not support
#endif // 0, 1
	}

#ifndef YGGR_NO_CWCHAR
	{
		wchar_t ch = L'Äã';
		yggr::utf8_char utf8_ch(L'Äã');
		yggr::utf8_char utf8_ch2(L'ºÃ');
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((ch ^= utf8_ch_ref) == 0);
		yggr_test_assert((ch ^= ref_type(utf8_ch2)) == (0 ^ L'ºÃ'));
	}
#endif // YGGR_NO_CWCHAR

	std::cout << "test_bitwise_xor_set_of_utf8_char_const_reference succeed" << std::endl;
}

void test_bitwise_xor_set_of_utf8_char_view(void)
{
	typedef yggr::utf8_char::char_view_type ref_type;

	{
		char ch = 'a';
		yggr::utf8_char utf8_ch('a');
		yggr::utf8_char utf8_ch2('b');
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((ch ^= utf8_ch_ref) == 0);
		yggr_test_assert((ch ^= ref_type(utf8_ch2)) == (0 ^ 'b'));

#if 0
		utf8_ch_ref ^= ch; // not support
#endif // 0, 1
	}

#ifndef YGGR_NO_CWCHAR
	{
		wchar_t ch = L'Äã';
		yggr::utf8_char utf8_ch(L'Äã');
		yggr::utf8_char utf8_ch2(L'ºÃ');
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((ch ^= utf8_ch_ref) == 0);
		yggr_test_assert((ch ^= ref_type(utf8_ch2)) == (0 ^ L'ºÃ'));
	}
#endif // YGGR_NO_CWCHAR

	std::cout << "test_bitwise_xor_set_of_utf8_char_view succeed" << std::endl;
}


void test_plus_of_utf8_char(void)
{
	// utf8_char + n
	{
		yggr::utf8_char utf8_ch = 'a';
		yggr_test_assert(utf8_ch + yggr::charset::utf8_foo::E_char_max_range == 'a');
		yggr_test_assert(utf8_ch + yggr::charset::utf8_foo::E_char_max_range * 2 == 'a');
		yggr_test_assert(utf8_ch + 2 == 'c');
		yggr_test_assert(utf8_ch + 2.9 == 'c');
	}

	{
		yggr_test_assert(yggr::utf8_char('a') + yggr::charset::utf8_foo::E_char_max_range == 'a');
		yggr_test_assert(yggr::utf8_char('a') + 2 == 'c');
	}

	// n + utf8_char
	{
		yggr::utf8_char utf8_ch = 'a';
		yggr_test_assert(yggr::charset::utf8_foo::E_char_max_range + utf8_ch
				== yggr::charset::utf8_foo::E_char_max_range + 'a');

		yggr_test_assert(2 + utf8_ch == 'c');
		yggr_test_assert(2.5 + utf8_ch == 99.5);
	}

	{
		yggr_test_assert(yggr::charset::utf8_foo::E_char_max_range + yggr::utf8_char('a')
				== yggr::charset::utf8_foo::E_char_max_range + 'a');
	}

	// utf8_char + utf8_char
	{
		yggr::utf8_char utf8_ch = 'a';
		yggr_test_assert(yggr::utf8_char(2) + utf8_ch == 'c');
		yggr_test_assert(yggr::utf8_char(2) + yggr::utf8_char('a') == 'c');
	}

	// utf8_char + utf8_char_view
	{
		yggr::utf8_char utf8_ch = 'a';
		yggr_test_assert(yggr::utf8_char(2).view() + utf8_ch == 'c');
		yggr_test_assert(yggr::utf8_char(2) + yggr::utf8_char('a').view() == 'c');
	}

	// utf8_char + utf8_char_reference
	{
		yggr::utf8_char utf8_ch = 'a';
		yggr_test_assert(static_cast<yggr::utf8_char::char_reference_type>(utf8_ch) + yggr::utf8_char(2) == 'c');
		yggr_test_assert(yggr::utf8_char(2) + yggr::utf8_char::char_reference_type(utf8_ch) == 'c');
	}

	// utf8_char + utf8_char_const_reference
	{
		yggr::utf8_char utf8_ch = 'a';
		yggr_test_assert(static_cast<yggr::utf8_char::char_const_reference_type>(utf8_ch) + yggr::utf8_char(2) == 'c');
		yggr_test_assert(yggr::utf8_char(2) + yggr::utf8_char::char_const_reference_type(utf8_ch) == 'c');
	}

#ifndef YGGR_NO_CWCHAR
	// utf8_char + n
	{
		yggr::utf8_char utf8_ch = L'Äã';
		yggr_test_assert(utf8_ch + yggr::charset::utf8_foo::E_char_max_range == L'Äã');
	}

	{
		yggr_test_assert(yggr::utf8_char(L'Äã') + yggr::charset::utf8_foo::E_char_max_range == L'Äã');
	}

	// n + utf8_char
	{
		yggr::utf8_char utf8_ch = L'Äã';
		yggr_test_assert(yggr::charset::utf8_foo::E_char_max_range + utf8_ch
				== yggr::charset::utf8_foo::E_char_max_range + L'Äã');
	}

	{
		yggr_test_assert(yggr::charset::utf8_foo::E_char_max_range + yggr::utf8_char(L'Äã')
				== yggr::charset::utf8_foo::E_char_max_range + L'Äã');
	}

	// utf8_char + utf8_char
	{
		yggr::utf8_char utf8_ch = L'Äã';
		yggr_test_assert(yggr::utf8_char(2) + utf8_ch == L'Äã' + 2);
		yggr_test_assert(yggr::utf8_char(2) + yggr::utf8_char(L'Äã') == static_cast<wchar_t>(L'Äã' + 2));
	}

	// utf8_char + utf8_char_view
	{
		yggr::utf8_char utf8_ch = L'Äã';
		yggr_test_assert(yggr::utf8_char(2).view() + utf8_ch == L'Äã' + 2);
		yggr_test_assert(yggr::utf8_char(2) + yggr::utf8_char(L'Äã').view() == static_cast<wchar_t>(L'Äã' + 2));
	}

	// utf8_char + utf8_char_reference
	{
		yggr::utf8_char utf8_ch = L'Äã';
		yggr_test_assert(static_cast<yggr::utf8_char::char_reference_type>(utf8_ch) + yggr::utf8_char(2) == L'Äã' + 2);
		yggr_test_assert(yggr::utf8_char(2) + yggr::utf8_char::char_reference_type(utf8_ch) == L'Äã' + 2);
	}

	// utf8_char + utf8_char_const_reference
	{
		yggr::utf8_char utf8_ch = L'Äã';
		yggr_test_assert(static_cast<yggr::utf8_char::char_const_reference_type>(utf8_ch) + yggr::utf8_char(2) == L'Äã' + 2);
		yggr_test_assert(yggr::utf8_char(2) + yggr::utf8_char::char_const_reference_type(utf8_ch) == L'Äã' + 2);
	}

#endif // YGGR_NO_CWCHAR

	std::cout << "test_plus_of_utf8_char succeed" << std::endl;
}

void test_plus_of_utf8_char_reference(void)
{
	typedef yggr::utf8_char::char_reference_type ref_type;

	// utf8_char_ref + n
	{
		yggr::utf8_char utf8_ch = 'a';
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert(utf8_ch_ref + yggr::charset::utf8_foo::E_char_max_range == 'a');
		yggr_test_assert(utf8_ch_ref + yggr::charset::utf8_foo::E_char_max_range * 2 == 'a');
		yggr_test_assert(utf8_ch_ref + 2 == 'c');
		yggr_test_assert(utf8_ch_ref + 2.9 == 'c');
	}

	{
		yggr::utf8_char utf8_ch = 'a';
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert(ref_type(utf8_ch_ref) + yggr::charset::utf8_foo::E_char_max_range == 'a');
		yggr_test_assert(ref_type(utf8_ch_ref) + 2 == 'c');
	}

	// n + utf8_char_ref
	{
		yggr::utf8_char utf8_ch = 'a';
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert(yggr::charset::utf8_foo::E_char_max_range + ref_type(utf8_ch_ref)
				== yggr::charset::utf8_foo::E_char_max_range + 'a');

		yggr_test_assert(2 + utf8_ch_ref == 'c');
		yggr_test_assert(2.5 + utf8_ch_ref == 99.5);
	}

	{
		yggr::utf8_char utf8_ch = 'a';

		yggr_test_assert(yggr::charset::utf8_foo::E_char_max_range + ref_type(utf8_ch)
				== yggr::charset::utf8_foo::E_char_max_range + 'a');
	}

	// utf8_char_ref + utf8_char_ref
	{
		yggr::utf8_char utf8_ch_l = 'a';
		yggr::utf8_char utf8_ch_r = 2;
		ref_type utf8_ch_ref_l = utf8_ch_l;
		ref_type utf8_ch_ref_r = utf8_ch_r;

		yggr_test_assert(utf8_ch_ref_l + utf8_ch_ref_r == 'c');
		yggr_test_assert(utf8_ch_ref_l + ref_type(utf8_ch_ref_r) == 'c');
		yggr_test_assert(ref_type(utf8_ch_ref_l) + utf8_ch_ref_r == 'c');
		yggr_test_assert(ref_type(utf8_ch_ref_l) + ref_type(utf8_ch_ref_r) == 'c');
	}

	// utf8_char_ref + utf8_char_view
	{
		yggr::utf8_char utf8_ch = 'a';
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert(yggr::utf8_char(2).view() + utf8_ch_ref == 'c');
		yggr_test_assert(ref_type(utf8_ch_ref) + yggr::utf8_char(2).view() == 'c');
	}

	// utf8_char_ref + utf8_char_const_reference
	{
		yggr::utf8_char utf8_ch_l = 'a';
		yggr::utf8_char utf8_ch_r = 2;
		ref_type utf8_ch_ref_l = utf8_ch_l;
		ref_type utf8_ch_ref_r = utf8_ch_r;

		yggr_test_assert(utf8_ch_ref_l.const_cast_() + utf8_ch_ref_r == 'c');
		yggr_test_assert(ref_type(utf8_ch_ref_r) + utf8_ch_ref_l.const_cast_<ref_type>() == 'c');
	}

#ifndef YGGR_NO_CWCHAR
	// utf8_char_ref + n
	{
		yggr::utf8_char utf8_ch = L'Äã';
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert(utf8_ch_ref + yggr::charset::utf8_foo::E_char_max_range == L'Äã');
	}

	{
		yggr::utf8_char utf8_ch = L'Äã';
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert(ref_type(utf8_ch_ref) + yggr::charset::utf8_foo::E_char_max_range == L'Äã');
	}

	// n + utf8_char_ref
	{
		yggr::utf8_char utf8_ch = L'Äã';
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert(yggr::charset::utf8_foo::E_char_max_range + utf8_ch_ref
				== yggr::charset::utf8_foo::E_char_max_range + L'Äã');
	}

	{
		yggr::utf8_char utf8_ch = L'Äã';
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert(yggr::charset::utf8_foo::E_char_max_range + ref_type(utf8_ch_ref)
				== yggr::charset::utf8_foo::E_char_max_range + L'Äã');
	}

	// utf8_char_ref + utf8_char_ref
	{
		yggr::utf8_char utf8_ch_l = L'Äã';
		yggr::utf8_char utf8_ch_r = 2;

		ref_type utf8_ch_ref_l = utf8_ch_l;
		ref_type utf8_ch_ref_r = utf8_ch_r;

		yggr_test_assert(utf8_ch_ref_l + utf8_ch_ref_r == L'Äã' + 2);
		yggr_test_assert(ref_type(utf8_ch_ref_r) + utf8_ch_ref_l == L'Äã' + 2);
		yggr_test_assert(utf8_ch_ref_l + ref_type(utf8_ch_ref_r) == L'Äã' + 2);
		yggr_test_assert(ref_type(utf8_ch_ref_r) + ref_type(utf8_ch_ref_l) == (L'Äã' + 2));
	}

	// utf8_char_ref + utf8_char_view
	{
		yggr::utf8_char utf8_ch_l = L'Äã';
		yggr::utf8_char utf8_ch_r = 2;

		ref_type utf8_ch_ref_l = utf8_ch_l;
		ref_type utf8_ch_ref_r = utf8_ch_r;

		yggr_test_assert(yggr::utf8_char(2).view() + utf8_ch_ref_l == L'Äã' + 2);
		yggr_test_assert(ref_type(utf8_ch_ref_r) + yggr::utf8_char(L'Äã').view() == (L'Äã' + 2));
	}

	// utf8_char_ref + utf8_char_const_reference
	{
		yggr::utf8_char utf8_ch_l = L'Äã';
		yggr::utf8_char utf8_ch_r = 2;
		ref_type utf8_ch_ref_l = utf8_ch_l;
		ref_type utf8_ch_ref_r = utf8_ch_r;

		yggr_test_assert(utf8_ch_ref_l.const_cast_() + utf8_ch_ref_r == L'Äã' + 2);
		yggr_test_assert(utf8_ch_ref_l + utf8_ch_ref_r.const_cast_() == L'Äã' + 2);
	}

#endif // YGGR_NO_CWCHAR

	std::cout << "test_plus_of_utf8_char_reference succeed" << std::endl;
}

void test_plus_of_utf8_char_const_reference(void)
{
	typedef yggr::utf8_char::char_const_reference_type ref_type;

	// utf8_char_ref + n
	{
		yggr::utf8_char utf8_ch = 'a';
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert(utf8_ch_ref + yggr::charset::utf8_foo::E_char_max_range == 'a');
		yggr_test_assert(utf8_ch_ref + yggr::charset::utf8_foo::E_char_max_range * 2 == 'a');
		yggr_test_assert(utf8_ch_ref + 2 == 'c');
		yggr_test_assert(utf8_ch_ref + 2.9 == 'c');
	}

	{
		yggr::utf8_char utf8_ch = 'a';
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert(ref_type(utf8_ch_ref) + yggr::charset::utf8_foo::E_char_max_range == 'a');
		yggr_test_assert(ref_type(utf8_ch_ref) + 2 == 'c');
	}

	// n + utf8_char_ref
	{
		yggr::utf8_char utf8_ch = 'a';
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert(yggr::charset::utf8_foo::E_char_max_range + ref_type(utf8_ch_ref)
				== yggr::charset::utf8_foo::E_char_max_range + 'a');

		yggr_test_assert(2 + utf8_ch_ref == 'c');
		yggr_test_assert(2.5 + utf8_ch_ref == 99.5);
	}

	{
		yggr::utf8_char utf8_ch = 'a';

		yggr_test_assert(yggr::charset::utf8_foo::E_char_max_range + ref_type(utf8_ch)
				== yggr::charset::utf8_foo::E_char_max_range + 'a');
	}

	// utf8_char_ref + utf8_char_ref
	{
		yggr::utf8_char utf8_ch_l = 'a';
		yggr::utf8_char utf8_ch_r = 2;
		ref_type utf8_ch_ref_l = utf8_ch_l;
		ref_type utf8_ch_ref_r = utf8_ch_r;

		yggr_test_assert(utf8_ch_ref_l + utf8_ch_ref_r == 'c');
		yggr_test_assert(utf8_ch_ref_l + ref_type(utf8_ch_ref_r) == 'c');
		yggr_test_assert(ref_type(utf8_ch_ref_l) + utf8_ch_ref_r == 'c');
		yggr_test_assert(ref_type(utf8_ch_ref_l) + ref_type(utf8_ch_ref_r) == 'c');
	}

	// utf8_char_ref + utf8_char_view
	{
		yggr::utf8_char utf8_ch = 'a';
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert(yggr::utf8_char(2).view() + utf8_ch_ref == 'c');
		yggr_test_assert(ref_type(utf8_ch_ref) + yggr::utf8_char(2).view() == 'c');
	}

#ifndef YGGR_NO_CWCHAR
	// utf8_char_ref + n
	{
		yggr::utf8_char utf8_ch = L'Äã';
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert(utf8_ch_ref + yggr::charset::utf8_foo::E_char_max_range == L'Äã');
	}

	{
		yggr::utf8_char utf8_ch = L'Äã';
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert(ref_type(utf8_ch_ref) + yggr::charset::utf8_foo::E_char_max_range == L'Äã');
	}

	// n + utf8_char_ref
	{
		yggr::utf8_char utf8_ch = L'Äã';
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert(yggr::charset::utf8_foo::E_char_max_range + utf8_ch_ref
				== yggr::charset::utf8_foo::E_char_max_range + L'Äã');
	}

	{
		yggr::utf8_char utf8_ch = L'Äã';
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert(yggr::charset::utf8_foo::E_char_max_range + ref_type(utf8_ch_ref)
				== yggr::charset::utf8_foo::E_char_max_range + L'Äã');
	}

	// utf8_char_ref + utf8_char_ref
	{
		yggr::utf8_char utf8_ch_l = L'Äã';
		yggr::utf8_char utf8_ch_r = 2;

		ref_type utf8_ch_ref_l = utf8_ch_l;
		ref_type utf8_ch_ref_r = utf8_ch_r;

		yggr_test_assert(utf8_ch_ref_l + utf8_ch_ref_r == L'Äã' + 2);
		yggr_test_assert(ref_type(utf8_ch_ref_r) + utf8_ch_ref_l == L'Äã' + 2);
		yggr_test_assert(utf8_ch_ref_l + ref_type(utf8_ch_ref_r) == L'Äã' + 2);
		yggr_test_assert(ref_type(utf8_ch_ref_r) + ref_type(utf8_ch_ref_l) == (L'Äã' + 2));
	}

	// utf8_char_ref + utf8_char_view
	{
		yggr::utf8_char utf8_ch_l = L'Äã';
		yggr::utf8_char utf8_ch_r = 2;

		ref_type utf8_ch_ref_l = utf8_ch_l;
		ref_type utf8_ch_ref_r = utf8_ch_r;

		yggr_test_assert(yggr::utf8_char(2).view() + utf8_ch_ref_l == L'Äã' + 2);
		yggr_test_assert(ref_type(utf8_ch_ref_r) + yggr::utf8_char(L'Äã').view() == (L'Äã' + 2));
	}

#endif // YGGR_NO_CWCHAR

	std::cout << "test_plus_of_utf8_char_const_reference succeed" << std::endl;
}

void test_plus_of_utf8_char_view(void)
{
	typedef yggr::utf8_char::char_view_type ref_type;

	// utf8_char_ref + n
	{
		yggr::utf8_char utf8_ch = 'a';
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert(utf8_ch_ref + yggr::charset::utf8_foo::E_char_max_range == 'a');
		yggr_test_assert(utf8_ch_ref + yggr::charset::utf8_foo::E_char_max_range * 2 == 'a');
		yggr_test_assert(utf8_ch_ref + 2 == 'c');
		yggr_test_assert(utf8_ch_ref + 2.9 == 'c');
	}

	{
		yggr::utf8_char utf8_ch = 'a';
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert(ref_type(utf8_ch_ref) + yggr::charset::utf8_foo::E_char_max_range == 'a');
		yggr_test_assert(ref_type(utf8_ch_ref) + 2 == 'c');
	}

	// n + utf8_char_ref
	{
		yggr::utf8_char utf8_ch = 'a';
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert(yggr::charset::utf8_foo::E_char_max_range + ref_type(utf8_ch_ref)
				== yggr::charset::utf8_foo::E_char_max_range + 'a');

		yggr_test_assert(2 + utf8_ch_ref == 'c');
		yggr_test_assert(2.5 + utf8_ch_ref == 99.5);
	}

	{
		yggr::utf8_char utf8_ch = 'a';

		yggr_test_assert(yggr::charset::utf8_foo::E_char_max_range + ref_type(utf8_ch)
				== yggr::charset::utf8_foo::E_char_max_range + 'a');
	}

	// utf8_char_ref + utf8_char_ref
	{
		yggr::utf8_char utf8_ch_l = 'a';
		yggr::utf8_char utf8_ch_r = 2;
		ref_type utf8_ch_ref_l = utf8_ch_l;
		ref_type utf8_ch_ref_r = utf8_ch_r;

		yggr_test_assert(utf8_ch_ref_l + utf8_ch_ref_r == 'c');
		yggr_test_assert(utf8_ch_ref_l + ref_type(utf8_ch_ref_r) == 'c');
		yggr_test_assert(ref_type(utf8_ch_ref_l) + utf8_ch_ref_r == 'c');
		yggr_test_assert(ref_type(utf8_ch_ref_l) + ref_type(utf8_ch_ref_r) == 'c');
	}

#ifndef YGGR_NO_CWCHAR
	// utf8_char_ref + n
	{
		yggr::utf8_char utf8_ch = L'Äã';
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert(utf8_ch_ref + yggr::charset::utf8_foo::E_char_max_range == L'Äã');
	}

	{
		yggr::utf8_char utf8_ch = L'Äã';
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert(ref_type(utf8_ch_ref) + yggr::charset::utf8_foo::E_char_max_range == L'Äã');
	}

	// n + utf8_char_ref
	{
		yggr::utf8_char utf8_ch = L'Äã';
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert(yggr::charset::utf8_foo::E_char_max_range + utf8_ch_ref
				== yggr::charset::utf8_foo::E_char_max_range + L'Äã');
	}

	{
		yggr::utf8_char utf8_ch = L'Äã';
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert(yggr::charset::utf8_foo::E_char_max_range + ref_type(utf8_ch_ref)
				== yggr::charset::utf8_foo::E_char_max_range + L'Äã');
	}

	// utf8_char_ref + utf8_char_ref
	{
		yggr::utf8_char utf8_ch_l = L'Äã';
		yggr::utf8_char utf8_ch_r = 2;

		ref_type utf8_ch_ref_l = utf8_ch_l;
		ref_type utf8_ch_ref_r = utf8_ch_r;

		yggr_test_assert(utf8_ch_ref_l + utf8_ch_ref_r == L'Äã' + 2);
		yggr_test_assert(ref_type(utf8_ch_ref_r) + utf8_ch_ref_l == L'Äã' + 2);
		yggr_test_assert(utf8_ch_ref_l + ref_type(utf8_ch_ref_r) == L'Äã' + 2);
		yggr_test_assert(ref_type(utf8_ch_ref_r) + ref_type(utf8_ch_ref_l) == (L'Äã' + 2));
	}

#endif // YGGR_NO_CWCHAR

	std::cout << "test_plus_of_utf8_char_view succeed" << std::endl;
}

void test_minus_of_utf8_char(void)
{
	// utf8_char - n
	{
		yggr::utf8_char utf8_ch = 'e';
		yggr_test_assert(utf8_ch - yggr::charset::utf8_foo::E_char_max_range == 'e');
		yggr_test_assert(utf8_ch - yggr::charset::utf8_foo::E_char_max_range * 2 == 'e');
		yggr_test_assert(utf8_ch - 2 == 'c');
		yggr_test_assert(utf8_ch - 2.9 == 'b');
	}

	{
		yggr_test_assert(yggr::utf8_char('e') - yggr::charset::utf8_foo::E_char_max_range == 'e');
		yggr_test_assert(yggr::utf8_char('e') - 2 == 'c');
	}

	// n - utf8_char
	{
		yggr::utf8_char utf8_ch = 'e';
		yggr_test_assert(yggr::charset::utf8_foo::E_char_max_range - utf8_ch
				== yggr::charset::utf8_foo::E_char_max_range - 'e');

		yggr_test_assert(2 - utf8_ch == 2 - 'e');
		yggr_test_assert(2.5 - utf8_ch == 2.5 - 'e');
	}

	{
		yggr_test_assert(yggr::charset::utf8_foo::E_char_max_range - yggr::utf8_char('e')
				== yggr::charset::utf8_foo::E_char_max_range - 'e');
	}

	// utf8_char - utf8_char
	{
		yggr::utf8_char utf8_ch = 'e';
		yggr_test_assert(utf8_ch - yggr::utf8_char(2)  == 'c');
		yggr_test_assert(yggr::utf8_char('e') - yggr::utf8_char(2) == 'c');
	}

	// utf8_char - utf8_char_view
	{
		yggr::utf8_char utf8_ch = 'e';
		yggr_test_assert(utf8_ch - yggr::utf8_char(2).view() == 'c');
		yggr_test_assert(yggr::utf8_char('e').view() - yggr::utf8_char(2) == 'c');
	}

	// utf8_char - utf8_char_reference
	{
		yggr::utf8_char utf8_ch = 'e';
		yggr::utf8_char utf8_ch2 = 2;
		yggr_test_assert(static_cast<yggr::utf8_char::char_reference_type>(utf8_ch) - yggr::utf8_char(2) == 'c');
		yggr_test_assert(utf8_ch - static_cast<yggr::utf8_char::char_reference_type>(utf8_ch2) == 'c');
	}

	// utf8_char - utf8_char_const_reference
	{
		yggr::utf8_char utf8_ch = 'e';
		yggr_test_assert(static_cast<yggr::utf8_char::char_const_reference_type>(utf8_ch) - yggr::utf8_char(2) == 'c');
		yggr_test_assert(utf8_ch - yggr::utf8_char::char_const_reference_type(yggr::utf8_char(2)) == 'c');
	}

#ifndef YGGR_NO_CWCHAR
	// utf8_char - n
	{
		yggr::utf8_char utf8_ch = L'Äã';
		yggr_test_assert(utf8_ch - yggr::charset::utf8_foo::E_char_max_range == L'Äã');
		yggr_test_assert(utf8_ch - yggr::charset::utf8_foo::E_char_max_range * 2 == L'Äã');
		yggr_test_assert(utf8_ch - 2 == L'Äã' - 2);
		yggr_test_assert(utf8_ch - 2.9 == L'Äã' - 2.1);
		yggr_test_assert(utf8_ch - 2.9 == L'Äã' - 2.9);
		yggr_test_assert(utf8_ch - 2.9 == L'Äã' - 3);
	}

	{
		yggr_test_assert(yggr::utf8_char(L'Äã') - yggr::charset::utf8_foo::E_char_max_range == L'Äã');
		yggr_test_assert(yggr::utf8_char(L'Äã') - 2 == L'Äã' - 2);
	}

	// n - utf8_char
	{
		yggr::utf8_char utf8_ch = L'Äã';
		yggr_test_assert(yggr::charset::utf8_foo::E_char_max_range - utf8_ch
				== yggr::charset::utf8_foo::E_char_max_range - L'Äã');

		yggr_test_assert(2 - utf8_ch == 2 - L'Äã');
		yggr_test_assert(2.5 - utf8_ch == 2.5 - L'Äã');
	}

	{
		yggr_test_assert(yggr::charset::utf8_foo::E_char_max_range - yggr::utf8_char(L'Äã')
				== yggr::charset::utf8_foo::E_char_max_range - L'Äã');
	}

	// utf8_char - utf8_char
	{
		yggr::utf8_char utf8_ch = L'Äã';
		yggr_test_assert(utf8_ch - yggr::utf8_char(2)  == L'Äã' - 2);
		yggr_test_assert(yggr::utf8_char(L'Äã') - yggr::utf8_char(2) == L'Äã' - 2);
	}

	// utf8_char - utf8_char_view
	{
		yggr::utf8_char utf8_ch = L'Äã';
		yggr_test_assert(utf8_ch - yggr::utf8_char(2).view() == L'Äã' - 2);
		yggr_test_assert(yggr::utf8_char(L'Äã').view() - yggr::utf8_char(2) == L'Äã' - 2);
	}

	// utf8_char - utf8_char_reference
	{
		yggr::utf8_char utf8_ch = L'Äã';
		yggr::utf8_char utf8_ch2 = 2;
		yggr_test_assert(static_cast<yggr::utf8_char::char_reference_type>(utf8_ch) - yggr::utf8_char(2) == L'Äã' - 2);
		yggr_test_assert(utf8_ch - static_cast<yggr::utf8_char::char_reference_type>(utf8_ch2) == L'Äã' - 2);
	}

	// utf8_char - utf8_char_const_reference
	{
		yggr::utf8_char utf8_ch = L'Äã';
		yggr_test_assert(static_cast<yggr::utf8_char::char_const_reference_type>(utf8_ch) - yggr::utf8_char(2) == L'Äã' - 2);
		yggr_test_assert(utf8_ch - yggr::utf8_char::char_const_reference_type(yggr::utf8_char(2)) == L'Äã' - 2);
	}

#endif // YGGR_NO_CWCHAR

	std::cout << "test_minus_of_utf8_char succeed" << std::endl;
}

void test_minus_of_utf8_char_reference(void)
{
	typedef yggr::utf8_char::char_reference_type ref_type;

	// utf8_char_ref - n
	{
		yggr::utf8_char utf8_ch = 'e';
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert(utf8_ch_ref - yggr::charset::utf8_foo::E_char_max_range == 'e');
		yggr_test_assert(utf8_ch_ref - yggr::charset::utf8_foo::E_char_max_range * 2 == 'e');
		yggr_test_assert(utf8_ch_ref - 2 == 'c');
		yggr_test_assert(utf8_ch_ref - 2.9 == 'b');
	}

	{
		yggr::utf8_char utf8_ch = 'e';
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert(ref_type(utf8_ch_ref) - yggr::charset::utf8_foo::E_char_max_range == 'e');
		yggr_test_assert(ref_type(utf8_ch_ref) - 2 == 'c');
	}

	// n - utf8_char_ref
	{
		yggr::utf8_char utf8_ch = 'e';
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert(yggr::charset::utf8_foo::E_char_max_range - utf8_ch_ref
				== yggr::charset::utf8_foo::E_char_max_range - 'e');

		yggr_test_assert(2 - utf8_ch_ref == 2 - 'e');
		yggr_test_assert(2.5 - utf8_ch_ref == 2.5 - 'e');
	}

	{
		yggr::utf8_char utf8_ch = 'e';
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert(yggr::charset::utf8_foo::E_char_max_range - ref_type(utf8_ch_ref)
				== yggr::charset::utf8_foo::E_char_max_range - 'e');
	}

	// utf8_char_ref - utf8_char_ref
	{
		yggr::utf8_char utf8_ch_l = 'e';
		yggr::utf8_char utf8_ch_r = 2;
		ref_type utf8_ch_ref_l = utf8_ch_l;
		ref_type utf8_ch_ref_r = utf8_ch_r;

		yggr_test_assert(utf8_ch_ref_l - ref_type(utf8_ch_ref_r) == 'c');
		yggr_test_assert(ref_type(utf8_ch_ref_l) - utf8_ch_ref_r == 'c');
		yggr_test_assert(ref_type(utf8_ch_ref_l) - ref_type(utf8_ch_ref_r) == 'c');
		yggr_test_assert(utf8_ch_ref_l - utf8_ch_ref_r == 'c');
	}

	// utf8_char_ref- utf8_char_view
	{
		yggr::utf8_char utf8_ch_l = 'e';
		yggr::utf8_char utf8_ch_r = 2;
		ref_type utf8_ch_ref_l = utf8_ch_l;
		ref_type utf8_ch_ref_r = utf8_ch_r;

		yggr_test_assert(utf8_ch_ref_l - yggr::utf8_char(2).view() == 'c');
		yggr_test_assert(yggr::utf8_char('e').view() - utf8_ch_ref_r == 'c');
	}

	// utf8_char_ref - utf8_char_const_reference
	{
		yggr::utf8_char utf8_ch_l = 'e';
		yggr::utf8_char utf8_ch_r = 2;
		ref_type utf8_ch_ref_l = utf8_ch_l;
		ref_type utf8_ch_ref_r = utf8_ch_r;

		yggr_test_assert(utf8_ch_ref_l.const_cast_() - ref_type(utf8_ch_ref_r) == 'c');
		yggr_test_assert(utf8_ch_ref_l - utf8_ch_ref_r.const_cast_() == 'c');
	}

#ifndef YGGR_NO_CWCHAR
	// utf8_char_ref - n
	{
		yggr::utf8_char utf8_ch = L'Äã';
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert(utf8_ch_ref - yggr::charset::utf8_foo::E_char_max_range == L'Äã');
		yggr_test_assert(utf8_ch_ref - yggr::charset::utf8_foo::E_char_max_range * 2 == L'Äã');
		yggr_test_assert(utf8_ch_ref - 2 == L'Äã' - 2);
		yggr_test_assert(utf8_ch_ref - 2.9 == L'Äã' - 2.1);
		yggr_test_assert(utf8_ch_ref - 2.9 == L'Äã' - 2.9);
		yggr_test_assert(utf8_ch_ref - 2.9 == L'Äã' - 3);
	}

	{
		yggr::utf8_char utf8_ch = L'Äã';
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert(ref_type(utf8_ch_ref) - yggr::charset::utf8_foo::E_char_max_range == L'Äã');
		yggr_test_assert(ref_type(utf8_ch_ref) - 2 == L'Äã' - 2);
	}

	// n - utf8_char_ref
	{
		yggr::utf8_char utf8_ch = L'Äã';
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert(yggr::charset::utf8_foo::E_char_max_range - utf8_ch_ref
				== yggr::charset::utf8_foo::E_char_max_range - L'Äã');

		yggr_test_assert(2 - utf8_ch_ref == 2 - L'Äã');
		yggr_test_assert(2.5 - utf8_ch_ref == 2.5 - L'Äã');
	}

	{
		yggr::utf8_char utf8_ch = L'Äã';
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert(yggr::charset::utf8_foo::E_char_max_range - ref_type(utf8_ch_ref)
				== yggr::charset::utf8_foo::E_char_max_range - L'Äã');
	}

	// utf8_char_ref - utf8_char_ref
	{
		yggr::utf8_char utf8_ch_l = L'Äã';
		yggr::utf8_char utf8_ch_r = 2;
		ref_type utf8_ch_ref_l = utf8_ch_l;
		ref_type utf8_ch_ref_r = utf8_ch_r;

		yggr_test_assert(utf8_ch_ref_l - utf8_ch_ref_r == L'Äã' - 2);
		yggr_test_assert(ref_type(utf8_ch_ref_l) - ref_type(utf8_ch_ref_r) == L'Äã' - 2);
		yggr_test_assert(ref_type(utf8_ch_ref_l) - utf8_ch_ref_r == L'Äã' - 2);
		yggr_test_assert(utf8_ch_ref_l - ref_type(utf8_ch_ref_r) == L'Äã' - 2);
	}

	// utf8_char_ref - utf8_char_view
	{
		yggr::utf8_char utf8_ch_l = L'Äã';
		yggr::utf8_char utf8_ch_r = 2;
		ref_type utf8_ch_ref_l = utf8_ch_l;
		ref_type utf8_ch_ref_r = utf8_ch_r;

		yggr_test_assert(utf8_ch_ref_l - yggr::utf8_char(2).view() == L'Äã' - 2);
		yggr_test_assert(yggr::utf8_char(L'Äã').view() - utf8_ch_ref_r == L'Äã' - 2);
	}

	// utf8_char_ref - utf8_char_const_reference
	{
		yggr::utf8_char utf8_ch_l = L'Äã';
		yggr::utf8_char utf8_ch_r = 2;
		ref_type utf8_ch_ref_l = utf8_ch_l;
		ref_type utf8_ch_ref_r = utf8_ch_r;

		yggr_test_assert(utf8_ch_ref_l.const_cast_() - utf8_ch_ref_r == L'Äã' - 2);
		yggr_test_assert(utf8_ch_ref_l - utf8_ch_ref_r.const_cast_() == L'Äã' - 2);
	}

#endif // YGGR_NO_CWCHAR

	std::cout << "test_minus_of_utf8_char_reference succeed" << std::endl;
}

void test_minus_of_utf8_char_const_reference(void)
{
	typedef yggr::utf8_char::char_const_reference_type ref_type;

	// utf8_char_ref - n
	{
		yggr::utf8_char utf8_ch = 'e';
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert(utf8_ch_ref - yggr::charset::utf8_foo::E_char_max_range == 'e');
		yggr_test_assert(utf8_ch_ref - yggr::charset::utf8_foo::E_char_max_range * 2 == 'e');
		yggr_test_assert(utf8_ch_ref - 2 == 'c');
		yggr_test_assert(utf8_ch_ref - 2.9 == 'b');
	}

	{
		yggr::utf8_char utf8_ch = 'e';
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert(ref_type(utf8_ch_ref) - yggr::charset::utf8_foo::E_char_max_range == 'e');
		yggr_test_assert(ref_type(utf8_ch_ref) - 2 == 'c');
	}

	// n - utf8_char_ref
	{
		yggr::utf8_char utf8_ch = 'e';
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert(yggr::charset::utf8_foo::E_char_max_range - utf8_ch_ref
				== yggr::charset::utf8_foo::E_char_max_range - 'e');

		yggr_test_assert(2 - utf8_ch_ref == 2 - 'e');
		yggr_test_assert(2.5 - utf8_ch_ref == 2.5 - 'e');
	}

	{
		yggr::utf8_char utf8_ch = 'e';
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert(yggr::charset::utf8_foo::E_char_max_range - ref_type(utf8_ch_ref)
				== yggr::charset::utf8_foo::E_char_max_range - 'e');
	}

	// utf8_char_ref - utf8_char_ref
	{
		yggr::utf8_char utf8_ch_l = 'e';
		yggr::utf8_char utf8_ch_r = 2;
		ref_type utf8_ch_ref_l = utf8_ch_l;
		ref_type utf8_ch_ref_r = utf8_ch_r;

		yggr_test_assert(utf8_ch_ref_l - ref_type(utf8_ch_ref_r) == 'c');
		yggr_test_assert(ref_type(utf8_ch_ref_l) - utf8_ch_ref_r == 'c');
		yggr_test_assert(ref_type(utf8_ch_ref_l) - ref_type(utf8_ch_ref_r) == 'c');
		yggr_test_assert(utf8_ch_ref_l - utf8_ch_ref_r == 'c');
	}

	// utf8_char_ref- utf8_char_view
	{
		yggr::utf8_char utf8_ch_l = 'e';
		yggr::utf8_char utf8_ch_r = 2;
		ref_type utf8_ch_ref_l = utf8_ch_l;
		ref_type utf8_ch_ref_r = utf8_ch_r;

		yggr_test_assert(utf8_ch_ref_l - yggr::utf8_char(2).view() == 'c');
		yggr_test_assert(yggr::utf8_char('e').view() - utf8_ch_ref_r == 'c');
	}

#ifndef YGGR_NO_CWCHAR
	// utf8_char_ref - n
	{
		yggr::utf8_char utf8_ch = L'Äã';
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert(utf8_ch_ref - yggr::charset::utf8_foo::E_char_max_range == L'Äã');
		yggr_test_assert(utf8_ch_ref - yggr::charset::utf8_foo::E_char_max_range * 2 == L'Äã');
		yggr_test_assert(utf8_ch_ref - 2 == L'Äã' - 2);
		yggr_test_assert(utf8_ch_ref - 2.9 == L'Äã' - 2.1);
		yggr_test_assert(utf8_ch_ref - 2.9 == L'Äã' - 2.9);
		yggr_test_assert(utf8_ch_ref - 2.9 == L'Äã' - 3);
	}

	{
		yggr::utf8_char utf8_ch = L'Äã';
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert(ref_type(utf8_ch_ref) - yggr::charset::utf8_foo::E_char_max_range == L'Äã');
		yggr_test_assert(ref_type(utf8_ch_ref) - 2 == L'Äã' - 2);
	}

	// n - utf8_char_ref
	{
		yggr::utf8_char utf8_ch = L'Äã';
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert(yggr::charset::utf8_foo::E_char_max_range - utf8_ch_ref
				== yggr::charset::utf8_foo::E_char_max_range - L'Äã');

		yggr_test_assert(2 - utf8_ch_ref == 2 - L'Äã');
		yggr_test_assert(2.5 - utf8_ch_ref == 2.5 - L'Äã');
	}

	{
		yggr::utf8_char utf8_ch = L'Äã';
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert(yggr::charset::utf8_foo::E_char_max_range - ref_type(utf8_ch_ref)
				== yggr::charset::utf8_foo::E_char_max_range - L'Äã');
	}

	// utf8_char_ref - utf8_char_ref
	{
		yggr::utf8_char utf8_ch_l = L'Äã';
		yggr::utf8_char utf8_ch_r = 2;
		ref_type utf8_ch_ref_l = utf8_ch_l;
		ref_type utf8_ch_ref_r = utf8_ch_r;

		yggr_test_assert(utf8_ch_ref_l - utf8_ch_ref_r == L'Äã' - 2);
		yggr_test_assert(ref_type(utf8_ch_ref_l) - ref_type(utf8_ch_ref_r) == L'Äã' - 2);
		yggr_test_assert(ref_type(utf8_ch_ref_l) - utf8_ch_ref_r == L'Äã' - 2);
		yggr_test_assert(utf8_ch_ref_l - ref_type(utf8_ch_ref_r) == L'Äã' - 2);
	}

	// utf8_char_ref - utf8_char_view
	{
		yggr::utf8_char utf8_ch_l = L'Äã';
		yggr::utf8_char utf8_ch_r = 2;
		ref_type utf8_ch_ref_l = utf8_ch_l;
		ref_type utf8_ch_ref_r = utf8_ch_r;

		yggr_test_assert(utf8_ch_ref_l - yggr::utf8_char(2).view() == L'Äã' - 2);
		yggr_test_assert(yggr::utf8_char(L'Äã').view() - utf8_ch_ref_r == L'Äã' - 2);
	}

#endif // YGGR_NO_CWCHAR

	std::cout << "test_minus_of_utf8_char_const_reference succeed" << std::endl;
}

void test_minus_of_utf8_char_view(void)
{
	typedef yggr::utf8_char::char_view_type ref_type;

	// utf8_char_ref - n
	{
		yggr::utf8_char utf8_ch = 'e';
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert(utf8_ch_ref - yggr::charset::utf8_foo::E_char_max_range == 'e');
		yggr_test_assert(utf8_ch_ref - yggr::charset::utf8_foo::E_char_max_range * 2 == 'e');
		yggr_test_assert(utf8_ch_ref - 2 == 'c');
		yggr_test_assert(utf8_ch_ref - 2.9 == 'b');
	}

	{
		yggr::utf8_char utf8_ch = 'e';
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert(ref_type(utf8_ch_ref) - yggr::charset::utf8_foo::E_char_max_range == 'e');
		yggr_test_assert(ref_type(utf8_ch_ref) - 2 == 'c');
	}

	// n - utf8_char_ref
	{
		yggr::utf8_char utf8_ch = 'e';
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert(yggr::charset::utf8_foo::E_char_max_range - utf8_ch_ref
				== yggr::charset::utf8_foo::E_char_max_range - 'e');

		yggr_test_assert(2 - utf8_ch_ref == 2 - 'e');
		yggr_test_assert(2.5 - utf8_ch_ref == 2.5 - 'e');
	}

	{
		yggr::utf8_char utf8_ch = 'e';
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert(yggr::charset::utf8_foo::E_char_max_range - ref_type(utf8_ch_ref)
				== yggr::charset::utf8_foo::E_char_max_range - 'e');
	}

	// utf8_char_ref - utf8_char_ref
	{
		yggr::utf8_char utf8_ch_l = 'e';
		yggr::utf8_char utf8_ch_r = 2;
		ref_type utf8_ch_ref_l = utf8_ch_l;
		ref_type utf8_ch_ref_r = utf8_ch_r;

		yggr_test_assert(utf8_ch_ref_l - ref_type(utf8_ch_ref_r) == 'c');
		yggr_test_assert(ref_type(utf8_ch_ref_l) - utf8_ch_ref_r == 'c');
		yggr_test_assert(ref_type(utf8_ch_ref_l) - ref_type(utf8_ch_ref_r) == 'c');
		yggr_test_assert(utf8_ch_ref_l - utf8_ch_ref_r == 'c');
	}

#ifndef YGGR_NO_CWCHAR
	// utf8_char_ref - n
	{
		yggr::utf8_char utf8_ch = L'Äã';
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert(utf8_ch_ref - yggr::charset::utf8_foo::E_char_max_range == L'Äã');
		yggr_test_assert(utf8_ch_ref - yggr::charset::utf8_foo::E_char_max_range * 2 == L'Äã');
		yggr_test_assert(utf8_ch_ref - 2 == L'Äã' - 2);
		yggr_test_assert(utf8_ch_ref - 2.9 == L'Äã' - 2.1);
		yggr_test_assert(utf8_ch_ref - 2.9 == L'Äã' - 2.9);
		yggr_test_assert(utf8_ch_ref - 2.9 == L'Äã' - 3);
	}

	{
		yggr::utf8_char utf8_ch = L'Äã';
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert(ref_type(utf8_ch_ref) - yggr::charset::utf8_foo::E_char_max_range == L'Äã');
		yggr_test_assert(ref_type(utf8_ch_ref) - 2 == L'Äã' - 2);
	}

	// n - utf8_char_ref
	{
		yggr::utf8_char utf8_ch = L'Äã';
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert(yggr::charset::utf8_foo::E_char_max_range - utf8_ch_ref
				== yggr::charset::utf8_foo::E_char_max_range - L'Äã');

		yggr_test_assert(2 - utf8_ch_ref == 2 - L'Äã');
		yggr_test_assert(2.5 - utf8_ch_ref == 2.5 - L'Äã');
	}

	{
		yggr::utf8_char utf8_ch = L'Äã';
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert(yggr::charset::utf8_foo::E_char_max_range - ref_type(utf8_ch_ref)
				== yggr::charset::utf8_foo::E_char_max_range - L'Äã');
	}

	// utf8_char_ref - utf8_char_ref
	{
		yggr::utf8_char utf8_ch_l = L'Äã';
		yggr::utf8_char utf8_ch_r = 2;
		ref_type utf8_ch_ref_l = utf8_ch_l;
		ref_type utf8_ch_ref_r = utf8_ch_r;

		yggr_test_assert(utf8_ch_ref_l - utf8_ch_ref_r == L'Äã' - 2);
		yggr_test_assert(ref_type(utf8_ch_ref_l) - ref_type(utf8_ch_ref_r) == L'Äã' - 2);
		yggr_test_assert(ref_type(utf8_ch_ref_l) - utf8_ch_ref_r == L'Äã' - 2);
		yggr_test_assert(utf8_ch_ref_l - ref_type(utf8_ch_ref_r) == L'Äã' - 2);
	}

#endif // YGGR_NO_CWCHAR

	std::cout << "test_minus_of_utf8_char_view succeed" << std::endl;
}

void test_mul_of_utf8_char(void)
{

#ifdef _MSC_VER
#	pragma warning( push )
#	pragma warning (disable : 4307)
#endif //_MSC_VER

	// utf8_char * n
	{
		yggr::utf8_char utf8_ch = 49;
		yggr_test_assert((utf8_ch * yggr::charset::utf8_foo::E_char_max_range).is_validate());
		yggr_test_assert(utf8_ch * 2 == 'b');
		yggr_test_assert(utf8_ch * 2.9 == 142);

		yggr_test_assert((utf8_ch * -2).is_validate());
		yggr_test_assert((-utf8_ch * 2).is_validate());
		yggr_test_assert(utf8_ch * -2 == -utf8_ch * 2);

		yggr_test_assert((utf8_ch * 2).is_validate());
		yggr_test_assert((-utf8_ch * -2).is_validate());
		yggr_test_assert(utf8_ch * 2 == -utf8_ch * -2);
		yggr_test_assert((-utf8_ch * -2) == 'b');

		yggr_test_assert((utf8_ch * 0.1).is_validate());
		yggr_test_assert((-utf8_ch * -0.1).is_validate());
		yggr_test_assert(utf8_ch * 0.1 == -utf8_ch * -0.1);
	}

	{
		yggr_test_assert((yggr::utf8_char(49) * yggr::charset::utf8_foo::E_char_max_range).is_validate());
		yggr_test_assert(yggr::utf8_char(49) * 2 == 'b');
	}

	// n * utf8_char
	{
		yggr::utf8_char utf8_ch = 49;
		yggr_test_assert(yggr::charset::utf8_foo::E_char_max_range * utf8_ch
				== yggr::charset::utf8_foo::E_char_max_range * 49);

		yggr_test_assert(2 * utf8_ch == 'b');
		yggr_test_assert(2.5 * utf8_ch == 122.5);
	}

	{
		yggr_test_assert(yggr::charset::utf8_foo::E_char_max_range * yggr::utf8_char('b')
				== yggr::charset::utf8_foo::E_char_max_range * 'b');
	}

	// utf8_char * utf8_char
	{
		yggr::utf8_char utf8_ch = 49;
		yggr_test_assert(yggr::utf8_char(2) * utf8_ch == 'b');
		yggr_test_assert(yggr::utf8_char(2) * yggr::utf8_char(49) == 'b');
	}

	// utf8_char * utf8_char_view
	{
		yggr::utf8_char utf8_ch = 49;
		yggr_test_assert(yggr::utf8_char(2).view() * utf8_ch == 'b');
		yggr_test_assert(yggr::utf8_char(2) * yggr::utf8_char(49).view() == 'b');
	}

	// utf8_char * utf8_char_reference
	{
		yggr::utf8_char utf8_ch = 49;
		yggr_test_assert(static_cast<yggr::utf8_char::char_reference_type>(utf8_ch) * yggr::utf8_char(2) == 'b');
		yggr_test_assert(yggr::utf8_char(2) * yggr::utf8_char::char_reference_type(utf8_ch) == 'b');
	}

	// utf8_char * utf8_char_const_reference
	{
		yggr::utf8_char utf8_ch = 49;
		yggr_test_assert(static_cast<yggr::utf8_char::char_const_reference_type>(utf8_ch) * yggr::utf8_char(2) == 'b');
		yggr_test_assert(yggr::utf8_char(2) * yggr::utf8_char::char_const_reference_type(utf8_ch) == 'b');
	}

#ifndef YGGR_NO_CWCHAR
	// utf8_char * n
	{
		yggr::utf8_char utf8_ch = (L'Äã' / 2);
		yggr_test_assert((utf8_ch * yggr::charset::utf8_foo::E_char_max_range).is_validate());
		yggr_test_assert(utf8_ch * 2 == L'Äã');
		yggr_test_assert(utf8_ch * 2.9 == (L'Äã' / 2) * 2.9);

		yggr_test_assert((utf8_ch * -2).is_validate());
		yggr_test_assert((-utf8_ch * 2).is_validate());
		yggr_test_assert(utf8_ch * -2 == -utf8_ch * 2);
	}

	{
		yggr_test_assert((yggr::utf8_char( L'Äã' / 2) * yggr::charset::utf8_foo::E_char_max_range).is_validate());
		yggr_test_assert(yggr::utf8_char( L'Äã' / 2) * 2 == L'Äã');
	}

	// n * utf8_char
	{
		yggr::utf8_char utf8_ch = (L'Äã' / 2);
		yggr_test_assert(static_cast<yggr::s64>(yggr::charset::utf8_foo::E_char_max_range) * utf8_ch
				== static_cast<yggr::s64>(yggr::charset::utf8_foo::E_char_max_range) * (L'Äã' / 2));

		yggr_test_assert(2 * utf8_ch == L'Äã');
		yggr_test_assert(2.5 * utf8_ch == 2.5 * (L'Äã' / 2));
	}

	{
		yggr_test_assert(static_cast<yggr::s64>(yggr::charset::utf8_foo::E_char_max_range) * yggr::utf8_char(L'Äã')
				== static_cast<yggr::s64>(yggr::charset::utf8_foo::E_char_max_range) * L'Äã');
	}

	// utf8_char * utf8_char
	{
		yggr::utf8_char utf8_ch = (L'Äã' / 2);
		yggr_test_assert(yggr::utf8_char(2) * utf8_ch == L'Äã');
		yggr_test_assert(yggr::utf8_char(2) * yggr::utf8_char((L'Äã' / 2)) == L'Äã');
	}

	// utf8_char * utf8_char_view
	{
		yggr::utf8_char utf8_ch = (L'Äã' / 2);
		yggr_test_assert(yggr::utf8_char(2).view() * utf8_ch == L'Äã');
		yggr_test_assert(yggr::utf8_char(2) * yggr::utf8_char((L'Äã' / 2)).view() == L'Äã');
	}

	// utf8_char * utf8_char_reference
	{
		yggr::utf8_char utf8_ch = (L'Äã' / 2);
		yggr_test_assert(static_cast<yggr::utf8_char::char_reference_type>(utf8_ch) * yggr::utf8_char(2) == L'Äã');
		yggr_test_assert(yggr::utf8_char(2) * yggr::utf8_char::char_reference_type(utf8_ch) == L'Äã');
	}

	// utf8_char * utf8_char_const_reference
	{
		yggr::utf8_char utf8_ch = (L'Äã' / 2);
		yggr_test_assert(static_cast<yggr::utf8_char::char_const_reference_type>(utf8_ch) * yggr::utf8_char(2) == L'Äã');
		yggr_test_assert(yggr::utf8_char(2) * yggr::utf8_char::char_const_reference_type(utf8_ch) == L'Äã');
	}

#endif // YGGR_NO_CWCHAR

#ifdef _MSC_VER
#	pragma warning( pop )
#endif //_MSC_VER

	std::cout << "test_mul_of_utf8_char succeed" << std::endl;
}

void test_mul_of_utf8_char_reference(void)
{
	typedef yggr::utf8_char::char_reference_type ref_type;

#ifdef _MSC_VER
#	pragma warning( push )
#	pragma warning (disable : 4307)
#endif //_MSC_VER

	// utf8_char_ref * n
	{
		yggr::utf8_char utf8_ch = 49;
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((utf8_ch_ref * yggr::charset::utf8_foo::E_char_max_range).is_validate());
		yggr_test_assert(utf8_ch_ref * 2 == 'b');
		yggr_test_assert(utf8_ch_ref * 2.9 == 142);

		yggr_test_assert((utf8_ch_ref * -2).is_validate());
		yggr_test_assert((-utf8_ch_ref * 2).is_validate());
		yggr_test_assert(utf8_ch_ref * -2 == -utf8_ch_ref * 2);

		yggr_test_assert((utf8_ch_ref * 2).is_validate());
		yggr_test_assert((-utf8_ch_ref * -2).is_validate());
		yggr_test_assert(utf8_ch_ref * 2 == -utf8_ch_ref * -2);
		yggr_test_assert((-utf8_ch_ref * -2) == 'b');

		yggr_test_assert((utf8_ch_ref * 0.1).is_validate());
		yggr_test_assert((-utf8_ch_ref * -0.1).is_validate());
		yggr_test_assert(utf8_ch_ref * 0.1 == -utf8_ch * -0.1);
	}

	{
		yggr::utf8_char utf8_ch = 49;
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((ref_type(utf8_ch_ref) * yggr::charset::utf8_foo::E_char_max_range).is_validate());
		yggr_test_assert(ref_type(utf8_ch_ref) * 2 == 'b');
	}

	// n * utf8_char_ref
	{
		yggr::utf8_char utf8_ch = 49;
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert(yggr::charset::utf8_foo::E_char_max_range * utf8_ch
				== yggr::charset::utf8_foo::E_char_max_range * 49);

		yggr_test_assert(2 * utf8_ch == 'b');
		yggr_test_assert(2.5 * utf8_ch == 122.5);
	}

	{
		yggr_test_assert(yggr::charset::utf8_foo::E_char_max_range * yggr::utf8_char('b')
				== yggr::charset::utf8_foo::E_char_max_range * 'b');
	}

	// utf8_char_ref * utf8_char_ref
	{
		yggr::utf8_char utf8_ch_l = 49;
		yggr::utf8_char utf8_ch_r = 2;
		ref_type utf8_ch_ref_l = utf8_ch_l;
		ref_type utf8_ch_ref_r = utf8_ch_r;

		yggr_test_assert(utf8_ch_ref_l * utf8_ch_ref_r == 'b');
		yggr_test_assert(utf8_ch_ref_l * ref_type(utf8_ch_ref_r) == 'b');
		yggr_test_assert(ref_type(utf8_ch_ref_l) * utf8_ch_ref_r == 'b');
		yggr_test_assert(ref_type(utf8_ch_ref_l) * ref_type(utf8_ch_ref_r) == 'b');
	}

	// utf8_char_ref * utf8_char_view
	{
		yggr::utf8_char utf8_ch_l = 49;
		yggr::utf8_char utf8_ch_r = 2;
		ref_type utf8_ch_ref_l = utf8_ch_l;
		ref_type utf8_ch_ref_r = utf8_ch_r;

		yggr_test_assert(yggr::utf8_char(2).view() * utf8_ch_ref_l == 'b');
		yggr_test_assert(utf8_ch_ref_r * yggr::utf8_char(49).view() == 'b');
	}

	// utf8_char_ref * utf8_char_const_reference
	{
		yggr::utf8_char utf8_ch_l = 49;
		yggr::utf8_char utf8_ch_r = 2;
		ref_type utf8_ch_ref_l = utf8_ch_l;
		ref_type utf8_ch_ref_r = utf8_ch_r;

		yggr_test_assert(utf8_ch_ref_l.const_cast_() * utf8_ch_ref_r == 'b');
		yggr_test_assert(ref_type(utf8_ch_ref_r) * utf8_ch_ref_l.const_cast_() == 'b');
	}

#ifndef YGGR_NO_CWCHAR
	// utf8_char_ref * n
	{
		yggr::utf8_char utf8_ch = (L'Äã' / 2);
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((utf8_ch_ref * yggr::charset::utf8_foo::E_char_max_range).is_validate());
		yggr_test_assert(utf8_ch_ref * 2 == L'Äã');
		yggr_test_assert(utf8_ch_ref * 2.9 == (L'Äã' / 2) * 2.9);

		yggr_test_assert((utf8_ch_ref * -2).is_validate());
		yggr_test_assert((-utf8_ch_ref * 2).is_validate());
		yggr_test_assert(utf8_ch_ref * -2 == -utf8_ch_ref * 2);
	}

	{
		yggr::utf8_char utf8_ch = (L'Äã' / 2);
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((utf8_ch_ref * yggr::charset::utf8_foo::E_char_max_range).is_validate());
		yggr_test_assert(ref_type(utf8_ch_ref) * 2 == L'Äã');
	}

	// n * utf8_char_ref
	{
		yggr::utf8_char utf8_ch = (L'Äã' / 2);
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert(static_cast<yggr::s64>(yggr::charset::utf8_foo::E_char_max_range) * utf8_ch_ref
				== static_cast<yggr::s64>(yggr::charset::utf8_foo::E_char_max_range) * (L'Äã' / 2));

		yggr_test_assert(2 * utf8_ch_ref == L'Äã');
		yggr_test_assert(2.5 * utf8_ch_ref == 2.5 * (L'Äã' / 2));
	}

	{
		yggr::utf8_char utf8_ch = L'Äã';
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert(static_cast<yggr::s64>(yggr::charset::utf8_foo::E_char_max_range) * ref_type(utf8_ch_ref)
				== static_cast<yggr::s64>(yggr::charset::utf8_foo::E_char_max_range) * L'Äã');
	}

	// utf8_char_ref * utf8_char_ref
	{
		yggr::utf8_char utf8_ch_l = (L'Äã' / 2);
		yggr::utf8_char utf8_ch_r = 2;
		ref_type utf8_ch_ref_l = utf8_ch_l;
		ref_type utf8_ch_ref_r = utf8_ch_r;

		yggr_test_assert(ref_type(utf8_ch_ref_l) * ref_type(utf8_ch_ref_r) == L'Äã');
		yggr_test_assert(ref_type(utf8_ch_ref_l) * utf8_ch_ref_r == L'Äã');
		yggr_test_assert(utf8_ch_ref_l * ref_type(utf8_ch_ref_r) == L'Äã');
		yggr_test_assert(utf8_ch_ref_l * utf8_ch_ref_r == L'Äã');
	}

	// utf8_char_ref * utf8_char_view
	{
		yggr::utf8_char utf8_ch_l = (L'Äã' / 2);
		yggr::utf8_char utf8_ch_r = 2;
		ref_type utf8_ch_ref_l = utf8_ch_l;
		ref_type utf8_ch_ref_r = utf8_ch_r;

		yggr_test_assert(yggr::utf8_char(2).view() * utf8_ch_ref_l == L'Äã');
		yggr_test_assert(utf8_ch_ref_r * yggr::utf8_char((L'Äã' / 2)).view() == L'Äã');
	}

	// utf8_char_ref * utf8_char_const_reference
	{
		yggr::utf8_char utf8_ch_l = (L'Äã' / 2);
		yggr::utf8_char utf8_ch_r = 2;
		ref_type utf8_ch_ref_l = utf8_ch_l;
		ref_type utf8_ch_ref_r = utf8_ch_r;

		yggr_test_assert(utf8_ch_ref_l.const_cast_() * utf8_ch_ref_r == L'Äã');
		yggr_test_assert(utf8_ch_ref_r * utf8_ch_ref_l.const_cast_() == L'Äã');
	}

#endif // YGGR_NO_CWCHAR

#ifdef _MSC_VER
#	pragma warning( pop )
#endif //_MSC_VER

	std::cout << "test_mul_of_utf8_char_reference succeed" << std::endl;
}

void test_mul_of_utf8_char_const_reference(void)
{
	typedef yggr::utf8_char::char_const_reference_type ref_type;

#ifdef _MSC_VER
#	pragma warning( push )
#	pragma warning (disable : 4307)
#endif //_MSC_VER

	// utf8_char_ref * n
	{
		yggr::utf8_char utf8_ch = 49;
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((utf8_ch_ref * yggr::charset::utf8_foo::E_char_max_range).is_validate());
		yggr_test_assert(utf8_ch_ref * 2 == 'b');
		yggr_test_assert(utf8_ch_ref * 2.9 == 142);

		yggr_test_assert((utf8_ch_ref * -2).is_validate());
		yggr_test_assert((-utf8_ch_ref * 2).is_validate());
		yggr_test_assert(utf8_ch_ref * -2 == -utf8_ch_ref * 2);

		yggr_test_assert((utf8_ch_ref * 2).is_validate());
		yggr_test_assert((-utf8_ch_ref * -2).is_validate());
		yggr_test_assert(utf8_ch_ref * 2 == -utf8_ch_ref * -2);
		yggr_test_assert((-utf8_ch_ref * -2) == 'b');

		yggr_test_assert((utf8_ch_ref * 0.1).is_validate());
		yggr_test_assert((-utf8_ch_ref * -0.1).is_validate());
		yggr_test_assert(utf8_ch_ref * 0.1 == -utf8_ch * -0.1);
	}

	{
		yggr::utf8_char utf8_ch = 49;
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((ref_type(utf8_ch_ref) * yggr::charset::utf8_foo::E_char_max_range).is_validate());
		yggr_test_assert(ref_type(utf8_ch_ref) * 2 == 'b');
	}

	// n * utf8_char_ref
	{
		yggr::utf8_char utf8_ch = 49;
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert(yggr::charset::utf8_foo::E_char_max_range * utf8_ch
				== yggr::charset::utf8_foo::E_char_max_range * 49);

		yggr_test_assert(2 * utf8_ch == 'b');
		yggr_test_assert(2.5 * utf8_ch == 122.5);
	}

	{
		yggr_test_assert(yggr::charset::utf8_foo::E_char_max_range * yggr::utf8_char('b')
				== yggr::charset::utf8_foo::E_char_max_range * 'b');
	}

	// utf8_char_ref * utf8_char_ref
	{
		yggr::utf8_char utf8_ch_l = 49;
		yggr::utf8_char utf8_ch_r = 2;
		ref_type utf8_ch_ref_l = utf8_ch_l;
		ref_type utf8_ch_ref_r = utf8_ch_r;

		yggr_test_assert(utf8_ch_ref_l * utf8_ch_ref_r == 'b');
		yggr_test_assert(utf8_ch_ref_l * ref_type(utf8_ch_ref_r) == 'b');
		yggr_test_assert(ref_type(utf8_ch_ref_l) * utf8_ch_ref_r == 'b');
		yggr_test_assert(ref_type(utf8_ch_ref_l) * ref_type(utf8_ch_ref_r) == 'b');
	}

	// utf8_char_ref * utf8_char_view
	{
		yggr::utf8_char utf8_ch_l = 49;
		yggr::utf8_char utf8_ch_r = 2;
		ref_type utf8_ch_ref_l = utf8_ch_l;
		ref_type utf8_ch_ref_r = utf8_ch_r;

		yggr_test_assert(yggr::utf8_char(2).view() * utf8_ch_ref_l == 'b');
		yggr_test_assert(utf8_ch_ref_r * yggr::utf8_char(49).view() == 'b');
	}

#ifndef YGGR_NO_CWCHAR
	// utf8_char_ref * n
	{
		yggr::utf8_char utf8_ch = (L'Äã' / 2);
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((utf8_ch_ref * yggr::charset::utf8_foo::E_char_max_range).is_validate());
		yggr_test_assert(utf8_ch_ref * 2 == L'Äã');
		yggr_test_assert(utf8_ch_ref * 2.9 == (L'Äã' / 2) * 2.9);

		yggr_test_assert((utf8_ch_ref * -2).is_validate());
		yggr_test_assert((-utf8_ch_ref * 2).is_validate());
		yggr_test_assert(utf8_ch_ref * -2 == -utf8_ch_ref * 2);
	}

	{
		yggr::utf8_char utf8_ch = (L'Äã' / 2);
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((utf8_ch_ref * yggr::charset::utf8_foo::E_char_max_range).is_validate());
		yggr_test_assert(ref_type(utf8_ch_ref) * 2 == L'Äã');
	}

	// n * utf8_char_ref
	{
		yggr::utf8_char utf8_ch = (L'Äã' / 2);
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert(static_cast<yggr::s64>(yggr::charset::utf8_foo::E_char_max_range) * utf8_ch_ref
				== static_cast<yggr::s64>(yggr::charset::utf8_foo::E_char_max_range) * (L'Äã' / 2));

		yggr_test_assert(2 * utf8_ch_ref == L'Äã');
		yggr_test_assert(2.5 * utf8_ch_ref == 2.5 * (L'Äã' / 2));
	}

	{
		yggr::utf8_char utf8_ch = L'Äã';
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert(static_cast<yggr::s64>(yggr::charset::utf8_foo::E_char_max_range) * ref_type(utf8_ch_ref)
				== static_cast<yggr::s64>(yggr::charset::utf8_foo::E_char_max_range) * L'Äã');
	}

	// utf8_char_ref * utf8_char_ref
	{
		yggr::utf8_char utf8_ch_l = (L'Äã' / 2);
		yggr::utf8_char utf8_ch_r = 2;
		ref_type utf8_ch_ref_l = utf8_ch_l;
		ref_type utf8_ch_ref_r = utf8_ch_r;

		yggr_test_assert(ref_type(utf8_ch_ref_l) * ref_type(utf8_ch_ref_r) == L'Äã');
		yggr_test_assert(ref_type(utf8_ch_ref_l) * utf8_ch_ref_r == L'Äã');
		yggr_test_assert(utf8_ch_ref_l * ref_type(utf8_ch_ref_r) == L'Äã');
		yggr_test_assert(utf8_ch_ref_l * utf8_ch_ref_r == L'Äã');
	}

	// utf8_char_ref * utf8_char_view
	{
		yggr::utf8_char utf8_ch_l = (L'Äã' / 2);
		yggr::utf8_char utf8_ch_r = 2;
		ref_type utf8_ch_ref_l = utf8_ch_l;
		ref_type utf8_ch_ref_r = utf8_ch_r;

		yggr_test_assert(yggr::utf8_char(2).view() * utf8_ch_ref_l == L'Äã');
		yggr_test_assert(utf8_ch_ref_r * yggr::utf8_char((L'Äã' / 2)).view() == L'Äã');
	}

#endif // YGGR_NO_CWCHAR

#ifdef _MSC_VER
#	pragma warning( pop )
#endif //_MSC_VER

	std::cout << "test_mul_of_utf8_char_const_reference succeed" << std::endl;
}

void test_mul_of_utf8_char_view(void)
{
	typedef yggr::utf8_char::char_view_type ref_type;

#ifdef _MSC_VER
#	pragma warning( push )
#	pragma warning (disable : 4307)
#endif //_MSC_VER

	// utf8_char_ref * n
	{
		yggr::utf8_char utf8_ch = 49;
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((utf8_ch_ref * yggr::charset::utf8_foo::E_char_max_range).is_validate());
		yggr_test_assert(utf8_ch_ref * 2 == 'b');
		yggr_test_assert(utf8_ch_ref * 2.9 == 142);

		yggr_test_assert((utf8_ch_ref * -2).is_validate());
		yggr_test_assert((-utf8_ch_ref * 2).is_validate());
		yggr_test_assert(utf8_ch_ref * -2 == -utf8_ch_ref * 2);

		yggr_test_assert((utf8_ch_ref * 2).is_validate());
		yggr_test_assert((-utf8_ch_ref * -2).is_validate());
		yggr_test_assert(utf8_ch_ref * 2 == -utf8_ch_ref * -2);
		yggr_test_assert((-utf8_ch_ref * -2) == 'b');

		yggr_test_assert((utf8_ch_ref * 0.1).is_validate());
		yggr_test_assert((-utf8_ch_ref * -0.1).is_validate());
		yggr_test_assert(utf8_ch_ref * 0.1 == -utf8_ch * -0.1);
	}

	{
		yggr::utf8_char utf8_ch = 49;
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((ref_type(utf8_ch_ref) * yggr::charset::utf8_foo::E_char_max_range).is_validate());
		yggr_test_assert(ref_type(utf8_ch_ref) * 2 == 'b');
	}

	// n * utf8_char_ref
	{
		yggr::utf8_char utf8_ch = 49;
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert(yggr::charset::utf8_foo::E_char_max_range * utf8_ch
				== yggr::charset::utf8_foo::E_char_max_range * 49);

		yggr_test_assert(2 * utf8_ch == 'b');
		yggr_test_assert(2.5 * utf8_ch == 122.5);
	}

	{
		yggr_test_assert(yggr::charset::utf8_foo::E_char_max_range * yggr::utf8_char('b')
				== yggr::charset::utf8_foo::E_char_max_range * 'b');
	}

	// utf8_char_ref * utf8_char_ref
	{
		yggr::utf8_char utf8_ch_l = 49;
		yggr::utf8_char utf8_ch_r = 2;
		ref_type utf8_ch_ref_l = utf8_ch_l;
		ref_type utf8_ch_ref_r = utf8_ch_r;

		yggr_test_assert(utf8_ch_ref_l * utf8_ch_ref_r == 'b');
		yggr_test_assert(utf8_ch_ref_l * ref_type(utf8_ch_ref_r) == 'b');
		yggr_test_assert(ref_type(utf8_ch_ref_l) * utf8_ch_ref_r == 'b');
		yggr_test_assert(ref_type(utf8_ch_ref_l) * ref_type(utf8_ch_ref_r) == 'b');
	}

#ifndef YGGR_NO_CWCHAR
	// utf8_char_ref * n
	{
		yggr::utf8_char utf8_ch = (L'Äã' / 2);
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((utf8_ch_ref * yggr::charset::utf8_foo::E_char_max_range).is_validate());
		yggr_test_assert(utf8_ch_ref * 2 == L'Äã');
		yggr_test_assert(utf8_ch_ref * 2.9 == (L'Äã' / 2) * 2.9);

		yggr_test_assert((utf8_ch_ref * -2).is_validate());
		yggr_test_assert((-utf8_ch_ref * 2).is_validate());
		yggr_test_assert(utf8_ch_ref * -2 == -utf8_ch_ref * 2);
	}

	{
		yggr::utf8_char utf8_ch = (L'Äã' / 2);
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((utf8_ch_ref * yggr::charset::utf8_foo::E_char_max_range).is_validate());
		yggr_test_assert(ref_type(utf8_ch_ref) * 2 == L'Äã');
	}

	// n * utf8_char_ref
	{
		yggr::utf8_char utf8_ch = (L'Äã' / 2);
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert(static_cast<yggr::s64>(yggr::charset::utf8_foo::E_char_max_range) * utf8_ch_ref
				== static_cast<yggr::s64>(yggr::charset::utf8_foo::E_char_max_range) * (L'Äã' / 2));

		yggr_test_assert(2 * utf8_ch_ref == L'Äã');
		yggr_test_assert(2.5 * utf8_ch_ref == 2.5 * (L'Äã' / 2));
	}

	{
		yggr::utf8_char utf8_ch = L'Äã';
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert(static_cast<yggr::s64>(yggr::charset::utf8_foo::E_char_max_range) * ref_type(utf8_ch_ref)
				== static_cast<yggr::s64>(yggr::charset::utf8_foo::E_char_max_range) * L'Äã');
	}

	// utf8_char_ref * utf8_char_ref
	{
		yggr::utf8_char utf8_ch_l = (L'Äã' / 2);
		yggr::utf8_char utf8_ch_r = 2;
		ref_type utf8_ch_ref_l = utf8_ch_l;
		ref_type utf8_ch_ref_r = utf8_ch_r;

		yggr_test_assert(ref_type(utf8_ch_ref_l) * ref_type(utf8_ch_ref_r) == L'Äã');
		yggr_test_assert(ref_type(utf8_ch_ref_l) * utf8_ch_ref_r == L'Äã');
		yggr_test_assert(utf8_ch_ref_l * ref_type(utf8_ch_ref_r) == L'Äã');
		yggr_test_assert(utf8_ch_ref_l * utf8_ch_ref_r == L'Äã');
	}

#endif // YGGR_NO_CWCHAR

#ifdef _MSC_VER
#	pragma warning( pop )
#endif //_MSC_VER

	std::cout << "test_mul_of_utf8_char_view succeed" << std::endl;
}

void test_div_of_utf8_char(void)
{

#ifdef _MSC_VER
#	pragma warning( push )
#	pragma warning (disable : 4804)
#endif //_MSC_VER

	// utf8_char / n
	{
		yggr::utf8_char utf8_ch = 'a' * 2;
		yggr_test_assert((utf8_ch / yggr::charset::utf8_foo::E_char_max_range).is_validate());
		yggr_test_assert(utf8_ch / 2 == 'a');
		yggr_test_assert(utf8_ch / 2.9 == 66);

		yggr_test_assert((utf8_ch / 0.1).is_validate());
		yggr_test_assert((-utf8_ch / -0.1).is_validate());

		yggr_test_assert(utf8_ch / 0.1 == -utf8_ch / -0.1);

		yggr_test_assert((utf8_ch / -2).is_validate());
		yggr_test_assert((-utf8_ch / 2).is_validate());

		yggr_test_assert(utf8_ch / -2 == -utf8_ch / 2);
	}

	{
		yggr_test_assert((yggr::utf8_char('a' * 2) / yggr::charset::utf8_foo::E_char_max_range).is_validate());
		yggr_test_assert(0 == (yggr::utf8_char('a' * 2) / yggr::charset::utf8_foo::E_char_max_range));
		yggr_test_assert(yggr::utf8_char('a' * 2) / 2 == 'a');
	}

	// n / utf8_char
	{
		yggr::utf8_char utf8_ch = 2;
		yggr_test_assert(yggr::charset::utf8_foo::E_char_max_range / utf8_ch
				== yggr::charset::utf8_foo::E_char_max_range / 2);

		yggr_test_assert('a' * 2 / utf8_ch == 'a');
		yggr_test_assert('a' * 2.5 / utf8_ch == 121.25);
	}

	{
		yggr_test_assert(yggr::charset::utf8_foo::E_char_max_range / yggr::utf8_char('a')
				== yggr::charset::utf8_foo::E_char_max_range / 'a');
	}

	// utf8_char / utf8_char
	{
		yggr::utf8_char utf8_ch = 2;
		yggr_test_assert(yggr::utf8_char('a' * 2) / utf8_ch == 'a');
		yggr_test_assert(yggr::utf8_char('a' * 2) / yggr::utf8_char(2) == 'a');
	}

	// utf8_char / utf8_char_view
	{
		yggr::utf8_char utf8_ch = 2;
		yggr_test_assert(yggr::utf8_char('a' * 2).view() / utf8_ch == 'a');
		yggr_test_assert(yggr::utf8_char('a' * 2) / yggr::utf8_char(2).view() == 'a');
	}

	// utf8_char / utf8_char_reference
	{
		yggr::utf8_char utf8_ch = 'a' * 2;
		yggr_test_assert(static_cast<yggr::utf8_char::char_reference_type>(utf8_ch) / yggr::utf8_char(2) == 'a');
		yggr_test_assert(utf8_ch / yggr::utf8_char::char_reference_type(yggr::utf8_char(2)) == 'a');
	}

	// utf8_char / utf8_char_const_reference
	{
		yggr::utf8_char utf8_ch = 'a' * 2;
		yggr_test_assert(static_cast<yggr::utf8_char::char_const_reference_type>(utf8_ch) / yggr::utf8_char(2) == 'a');
		yggr_test_assert(utf8_ch / yggr::utf8_char::char_const_reference_type(yggr::utf8_char(2)) == 'a');
	}

#ifndef YGGR_NO_CWCHAR
	// utf8_char / n
	{
		yggr::utf8_char utf8_ch = L'Äã' * 2;
		yggr_test_assert((utf8_ch / yggr::charset::utf8_foo::E_char_max_range).is_validate());
		yggr_test_assert(utf8_ch / 2 == L'Äã');
		yggr_test_assert(utf8_ch / 2.9 == L'Äã' * 2 / 2.9);

		yggr_test_assert((utf8_ch / 0.1).is_validate());
		yggr_test_assert((-utf8_ch / -0.1).is_validate());

		yggr_test_assert(utf8_ch / 0.1 == -utf8_ch / -0.1);

		yggr_test_assert((utf8_ch / -2).is_validate());
		yggr_test_assert((-utf8_ch / 2).is_validate());

		yggr_test_assert(utf8_ch / -2 == -utf8_ch / 2);
	}

	{
		yggr_test_assert((yggr::utf8_char(L'Äã' * 2) / yggr::charset::utf8_foo::E_char_max_range).is_validate());
		yggr_test_assert(0 == (yggr::utf8_char(L'Äã' * 2) / yggr::charset::utf8_foo::E_char_max_range));
		yggr_test_assert(yggr::utf8_char(L'Äã' * 2) / 2 == L'Äã');
	}

	// n / utf8_char
	{
		yggr::utf8_char utf8_ch = 2;
		yggr_test_assert(yggr::charset::utf8_foo::E_char_max_range / utf8_ch
				== yggr::charset::utf8_foo::E_char_max_range / 2);

		yggr_test_assert(L'Äã' * 2 / utf8_ch == L'Äã');
		yggr_test_assert(L'Äã' * 2.5 / utf8_ch == L'Äã' * 2.5 / 2);
	}

	{
		yggr_test_assert(yggr::charset::utf8_foo::E_char_max_range / yggr::utf8_char(L'Äã')
				== yggr::charset::utf8_foo::E_char_max_range / L'Äã');
	}

	// utf8_char / utf8_char
	{
		yggr::utf8_char utf8_ch = 2;
		yggr_test_assert(yggr::utf8_char(L'Äã' * 2) / utf8_ch == L'Äã');
		yggr_test_assert(yggr::utf8_char(L'Äã' * 2) / yggr::utf8_char(2) == L'Äã');
	}

	// utf8_char / utf8_char_view
	{
		yggr::utf8_char utf8_ch = 2;
		yggr_test_assert(yggr::utf8_char(L'Äã' * 2).view() / utf8_ch == L'Äã');
		yggr_test_assert(yggr::utf8_char(L'Äã' * 2) / yggr::utf8_char(2).view() == L'Äã');
	}

	// utf8_char / utf8_char_reference
	{
		yggr::utf8_char utf8_ch = L'Äã' * 2;
		yggr_test_assert(static_cast<yggr::utf8_char::char_reference_type>(utf8_ch) / yggr::utf8_char(2) == L'Äã');
		yggr_test_assert(utf8_ch / yggr::utf8_char::char_reference_type(yggr::utf8_char(2)) == L'Äã');
	}

	// utf8_char / utf8_char_const_reference
	{
		yggr::utf8_char utf8_ch = L'Äã' * 2;
		yggr_test_assert(static_cast<yggr::utf8_char::char_const_reference_type>(utf8_ch) / yggr::utf8_char(2) == L'Äã');
		yggr_test_assert(utf8_ch / yggr::utf8_char::char_const_reference_type(yggr::utf8_char(2)) == L'Äã');
	}

#endif // YGGR_NO_CWCHAR

#ifdef _MSC_VER
#	pragma warning( pop )
#endif //_MSC_VER


	std::cout << "test_div_of_utf8_char succeed" << std::endl;
}

void test_div_of_utf8_char_reference(void)
{
	typedef yggr::utf8_char::char_reference_type ref_type;

#ifdef _MSC_VER
#	pragma warning( push )
#	pragma warning (disable : 4804)
#endif //_MSC_VER

	// utf8_char_ref / n
	{
		yggr::utf8_char utf8_ch = 'a' * 2;
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((utf8_ch_ref / yggr::charset::utf8_foo::E_char_max_range).is_validate());
		yggr_test_assert(utf8_ch_ref / 2 == 'a');
		yggr_test_assert(utf8_ch_ref / 2.9 == 66);

		yggr_test_assert((utf8_ch_ref / 0.1).is_validate());
		yggr_test_assert((-utf8_ch_ref / -0.1).is_validate());

		yggr_test_assert(utf8_ch_ref / 0.1 == -utf8_ch_ref / -0.1);

		yggr_test_assert((utf8_ch_ref / -2).is_validate());
		yggr_test_assert((-utf8_ch_ref / 2).is_validate());

		yggr_test_assert(utf8_ch_ref / -2 == -utf8_ch / 2);
	}

	{
		yggr::utf8_char utf8_ch = 'a' * 2;
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((utf8_ch_ref / yggr::charset::utf8_foo::E_char_max_range).is_validate());
		yggr_test_assert(0 == (utf8_ch_ref / yggr::charset::utf8_foo::E_char_max_range));
		yggr_test_assert(ref_type(utf8_ch_ref) / 2 == 'a');
	}

	// n / utf8_char_ref
	{
		yggr::utf8_char utf8_ch = 2;
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert(yggr::charset::utf8_foo::E_char_max_range / utf8_ch_ref
				== yggr::charset::utf8_foo::E_char_max_range / 2);

		yggr_test_assert('a' * 2 / utf8_ch_ref == 'a');
		yggr_test_assert('a' * 2.5 / utf8_ch_ref == 121.25);
	}

	{
		yggr::utf8_char utf8_ch = 'a';
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert(yggr::charset::utf8_foo::E_char_max_range / utf8_ch_ref
				== yggr::charset::utf8_foo::E_char_max_range / 'a');
	}

	// utf8_char_ref / utf8_char_ref
	{
		yggr::utf8_char utf8_ch_l = 'a' * 2;
		yggr::utf8_char utf8_ch_r = 2;
		ref_type utf8_ch_ref_l = utf8_ch_l;
		ref_type utf8_ch_ref_r = utf8_ch_r;

		yggr_test_assert(ref_type(utf8_ch_ref_l) / ref_type(utf8_ch_ref_r) == 'a');
		yggr_test_assert(ref_type(utf8_ch_ref_l) / utf8_ch_ref_r == 'a');
		yggr_test_assert(utf8_ch_ref_l / ref_type(utf8_ch_ref_r) == 'a');
		yggr_test_assert(utf8_ch_ref_l / utf8_ch_ref_r == 'a');
	}

	// utf8_char_ref / utf8_char_view
	{
		yggr::utf8_char utf8_ch_l = 'a' * 2;
		yggr::utf8_char utf8_ch_r = 2;
		ref_type utf8_ch_ref_l = utf8_ch_l;
		ref_type utf8_ch_ref_r = utf8_ch_r;

		yggr_test_assert(yggr::utf8_char('a' * 2).view() / utf8_ch_ref_r == 'a');
		yggr_test_assert(utf8_ch_ref_l / yggr::utf8_char(2).view() == 'a');
	}

	// utf8_char_ref / utf8_char_const_reference
	{
		yggr::utf8_char utf8_ch_l = 'a' * 2;
		yggr::utf8_char utf8_ch_r = 2;
		ref_type utf8_ch_ref_l = utf8_ch_l;
		ref_type utf8_ch_ref_r = utf8_ch_r;

		yggr_test_assert(utf8_ch_ref_l.const_cast_() / utf8_ch_ref_r == 'a');
		yggr_test_assert(utf8_ch_ref_l / utf8_ch_ref_r.const_cast_() == 'a');
	}

#ifndef YGGR_NO_CWCHAR
	// utf8_char_ref / n
	{
		yggr::utf8_char utf8_ch = L'Äã' * 2;
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((utf8_ch_ref / yggr::charset::utf8_foo::E_char_max_range).is_validate());
		yggr_test_assert(utf8_ch_ref / 2 == L'Äã');
		yggr_test_assert(utf8_ch_ref / 2.9 == L'Äã' * 2 / 2.9);

		yggr_test_assert((utf8_ch_ref / 0.1).is_validate());
		yggr_test_assert((-utf8_ch_ref / -0.1).is_validate());

		yggr_test_assert(utf8_ch_ref / 0.1 == -utf8_ch / -0.1);

		yggr_test_assert((utf8_ch_ref / -2).is_validate());
		yggr_test_assert((-utf8_ch_ref / 2).is_validate());

		yggr_test_assert(utf8_ch_ref / -2 == -utf8_ch_ref / 2);
	}

	{
		yggr::utf8_char utf8_ch = L'Äã' * 2;
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((utf8_ch_ref / yggr::charset::utf8_foo::E_char_max_range).is_validate());
		yggr_test_assert(0 == (ref_type(utf8_ch_ref) / yggr::charset::utf8_foo::E_char_max_range));
		yggr_test_assert(utf8_ch_ref / 2 == L'Äã');
	}

	// n / utf8_char_ref
	{
		yggr::utf8_char utf8_ch = 2;
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert(yggr::charset::utf8_foo::E_char_max_range / utf8_ch_ref
				== yggr::charset::utf8_foo::E_char_max_range / 2);

		yggr_test_assert(L'Äã' * 2 / utf8_ch_ref == L'Äã');
		yggr_test_assert(L'Äã' * 2.5 / utf8_ch_ref == L'Äã' * 2.5 / 2);
	}

	{
		yggr::utf8_char utf8_ch = L'Äã';
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert(yggr::charset::utf8_foo::E_char_max_range / utf8_ch_ref
				== yggr::charset::utf8_foo::E_char_max_range / L'Äã');
	}

	// utf8_char_ref / utf8_char_ref
	{
		yggr::utf8_char utf8_ch_l = L'Äã' * 2;
		yggr::utf8_char utf8_ch_r = 2;
		ref_type utf8_ch_ref_l = utf8_ch_l;
		ref_type utf8_ch_ref_r = utf8_ch_r;

		yggr_test_assert(ref_type(utf8_ch_ref_l) / ref_type(utf8_ch_ref_r) == L'Äã');
		yggr_test_assert(ref_type(utf8_ch_ref_l) / utf8_ch_ref_r == L'Äã');
		yggr_test_assert(utf8_ch_ref_l / ref_type(utf8_ch_ref_r) == L'Äã');
		yggr_test_assert(utf8_ch_ref_l / utf8_ch_ref_r == L'Äã');
	}

	// utf8_char_ref / utf8_char_view
	{
		yggr::utf8_char utf8_ch_l = L'Äã' * 2;
		yggr::utf8_char utf8_ch_r = 2;
		ref_type utf8_ch_ref_l = utf8_ch_l;
		ref_type utf8_ch_ref_r = utf8_ch_r;

		yggr_test_assert(yggr::utf8_char(L'Äã' * 2).view() / utf8_ch_ref_r == L'Äã');
		yggr_test_assert(utf8_ch_ref_l / yggr::utf8_char(2).view() == L'Äã');
	}

	// utf8_char_ref / utf8_char_const_reference
	{
		yggr::utf8_char utf8_ch_l = L'Äã' * 2;
		yggr::utf8_char utf8_ch_r = 2;
		ref_type utf8_ch_ref_l = utf8_ch_l;
		ref_type utf8_ch_ref_r = utf8_ch_r;

		yggr_test_assert(utf8_ch_ref_l.const_cast_() / utf8_ch_ref_r == L'Äã');
		yggr_test_assert(utf8_ch_ref_l / utf8_ch_ref_r.const_cast_() == L'Äã');
	}

#endif // YGGR_NO_CWCHAR

#ifdef _MSC_VER
#	pragma warning( pop )
#endif //_MSC_VER


	std::cout << "test_div_of_utf8_char_reference succeed" << std::endl;
}

void test_div_of_utf8_char_const_reference(void)
{
	typedef yggr::utf8_char::char_const_reference_type ref_type;

#ifdef _MSC_VER
#	pragma warning( push )
#	pragma warning (disable : 4804)
#endif //_MSC_VER

	// utf8_char_ref / n
	{
		yggr::utf8_char utf8_ch = 'a' * 2;
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((utf8_ch_ref / yggr::charset::utf8_foo::E_char_max_range).is_validate());
		yggr_test_assert(utf8_ch_ref / 2 == 'a');
		yggr_test_assert(utf8_ch_ref / 2.9 == 66);

		yggr_test_assert((utf8_ch_ref / 0.1).is_validate());
		yggr_test_assert((-utf8_ch_ref / -0.1).is_validate());

		yggr_test_assert(utf8_ch_ref / 0.1 == -utf8_ch_ref / -0.1);

		yggr_test_assert((utf8_ch_ref / -2).is_validate());
		yggr_test_assert((-utf8_ch_ref / 2).is_validate());

		yggr_test_assert(utf8_ch_ref / -2 == -utf8_ch / 2);
	}

	{
		yggr::utf8_char utf8_ch = 'a' * 2;
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((utf8_ch_ref / yggr::charset::utf8_foo::E_char_max_range).is_validate());
		yggr_test_assert(0 == (utf8_ch_ref / yggr::charset::utf8_foo::E_char_max_range));
		yggr_test_assert(ref_type(utf8_ch_ref) / 2 == 'a');
	}

	// n / utf8_char_ref
	{
		yggr::utf8_char utf8_ch = 2;
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert(yggr::charset::utf8_foo::E_char_max_range / utf8_ch_ref
				== yggr::charset::utf8_foo::E_char_max_range / 2);

		yggr_test_assert('a' * 2 / utf8_ch_ref == 'a');
		yggr_test_assert('a' * 2.5 / utf8_ch_ref == 121.25);
	}

	{
		yggr::utf8_char utf8_ch = 'a';
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert(yggr::charset::utf8_foo::E_char_max_range / utf8_ch_ref
				== yggr::charset::utf8_foo::E_char_max_range / 'a');
	}

	// utf8_char_ref / utf8_char_ref
	{
		yggr::utf8_char utf8_ch_l = 'a' * 2;
		yggr::utf8_char utf8_ch_r = 2;
		ref_type utf8_ch_ref_l = utf8_ch_l;
		ref_type utf8_ch_ref_r = utf8_ch_r;

		yggr_test_assert(ref_type(utf8_ch_ref_l) / ref_type(utf8_ch_ref_r) == 'a');
		yggr_test_assert(ref_type(utf8_ch_ref_l) / utf8_ch_ref_r == 'a');
		yggr_test_assert(utf8_ch_ref_l / ref_type(utf8_ch_ref_r) == 'a');
		yggr_test_assert(utf8_ch_ref_l / utf8_ch_ref_r == 'a');
	}

	// utf8_char_ref / utf8_char_view
	{
		yggr::utf8_char utf8_ch_l = 'a' * 2;
		yggr::utf8_char utf8_ch_r = 2;
		ref_type utf8_ch_ref_l = utf8_ch_l;
		ref_type utf8_ch_ref_r = utf8_ch_r;

		yggr_test_assert(yggr::utf8_char('a' * 2).view() / utf8_ch_ref_r == 'a');
		yggr_test_assert(utf8_ch_ref_l / yggr::utf8_char(2).view() == 'a');
	}

#ifndef YGGR_NO_CWCHAR
	// utf8_char_ref / n
	{
		yggr::utf8_char utf8_ch = L'Äã' * 2;
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((utf8_ch_ref / yggr::charset::utf8_foo::E_char_max_range).is_validate());
		yggr_test_assert(utf8_ch_ref / 2 == L'Äã');
		yggr_test_assert(utf8_ch_ref / 2.9 == L'Äã' * 2 / 2.9);

		yggr_test_assert((utf8_ch_ref / 0.1).is_validate());
		yggr_test_assert((-utf8_ch_ref / -0.1).is_validate());

		yggr_test_assert(utf8_ch_ref / 0.1 == -utf8_ch / -0.1);

		yggr_test_assert((utf8_ch_ref / -2).is_validate());
		yggr_test_assert((-utf8_ch_ref / 2).is_validate());

		yggr_test_assert(utf8_ch_ref / -2 == -utf8_ch_ref / 2);
	}

	{
		yggr::utf8_char utf8_ch = L'Äã' * 2;
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((utf8_ch_ref / yggr::charset::utf8_foo::E_char_max_range).is_validate());
		yggr_test_assert(0 == (ref_type(utf8_ch_ref) / yggr::charset::utf8_foo::E_char_max_range));
		yggr_test_assert(utf8_ch_ref / 2 == L'Äã');
	}

	// n / utf8_char_ref
	{
		yggr::utf8_char utf8_ch = 2;
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert(yggr::charset::utf8_foo::E_char_max_range / utf8_ch_ref
				== yggr::charset::utf8_foo::E_char_max_range / 2);

		yggr_test_assert(L'Äã' * 2 / utf8_ch_ref == L'Äã');
		yggr_test_assert(L'Äã' * 2.5 / utf8_ch_ref == L'Äã' * 2.5 / 2);
	}

	{
		yggr::utf8_char utf8_ch = L'Äã';
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert(yggr::charset::utf8_foo::E_char_max_range / utf8_ch_ref
				== yggr::charset::utf8_foo::E_char_max_range / L'Äã');
	}

	// utf8_char_ref / utf8_char_ref
	{
		yggr::utf8_char utf8_ch_l = L'Äã' * 2;
		yggr::utf8_char utf8_ch_r = 2;
		ref_type utf8_ch_ref_l = utf8_ch_l;
		ref_type utf8_ch_ref_r = utf8_ch_r;

		yggr_test_assert(ref_type(utf8_ch_ref_l) / ref_type(utf8_ch_ref_r) == L'Äã');
		yggr_test_assert(ref_type(utf8_ch_ref_l) / utf8_ch_ref_r == L'Äã');
		yggr_test_assert(utf8_ch_ref_l / ref_type(utf8_ch_ref_r) == L'Äã');
		yggr_test_assert(utf8_ch_ref_l / utf8_ch_ref_r == L'Äã');
	}

	// utf8_char_ref / utf8_char_view
	{
		yggr::utf8_char utf8_ch_l = L'Äã' * 2;
		yggr::utf8_char utf8_ch_r = 2;
		ref_type utf8_ch_ref_l = utf8_ch_l;
		ref_type utf8_ch_ref_r = utf8_ch_r;

		yggr_test_assert(yggr::utf8_char(L'Äã' * 2).view() / utf8_ch_ref_r == L'Äã');
		yggr_test_assert(utf8_ch_ref_l / yggr::utf8_char(2).view() == L'Äã');
	}

#endif // YGGR_NO_CWCHAR

#ifdef _MSC_VER
#	pragma warning( pop )
#endif //_MSC_VER

	std::cout << "test_div_of_utf8_char_const_reference succeed" << std::endl;
}

void test_div_of_utf8_char_view(void)
{
	typedef yggr::utf8_char::char_view_type ref_type;

#ifdef _MSC_VER
#	pragma warning( push )
#	pragma warning (disable : 4804)
#endif //_MSC_VER

	// utf8_char_ref / n
	{
		yggr::utf8_char utf8_ch = 'a' * 2;
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((utf8_ch_ref / yggr::charset::utf8_foo::E_char_max_range).is_validate());
		yggr_test_assert(utf8_ch_ref / 2 == 'a');
		yggr_test_assert(utf8_ch_ref / 2.9 == 66);

		yggr_test_assert((utf8_ch_ref / 0.1).is_validate());
		yggr_test_assert((-utf8_ch_ref / -0.1).is_validate());

		yggr_test_assert(utf8_ch_ref / 0.1 == -utf8_ch_ref / -0.1);

		yggr_test_assert((utf8_ch_ref / -2).is_validate());
		yggr_test_assert((-utf8_ch_ref / 2).is_validate());

		yggr_test_assert(utf8_ch_ref / -2 == -utf8_ch / 2);
	}

	{
		yggr::utf8_char utf8_ch = 'a' * 2;
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((utf8_ch_ref / yggr::charset::utf8_foo::E_char_max_range).is_validate());
		yggr_test_assert(0 == (utf8_ch_ref / yggr::charset::utf8_foo::E_char_max_range));
		yggr_test_assert(ref_type(utf8_ch_ref) / 2 == 'a');
	}

	// n / utf8_char_ref
	{
		yggr::utf8_char utf8_ch = 2;
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert(yggr::charset::utf8_foo::E_char_max_range / utf8_ch_ref
				== yggr::charset::utf8_foo::E_char_max_range / 2);

		yggr_test_assert('a' * 2 / utf8_ch_ref == 'a');
		yggr_test_assert('a' * 2.5 / utf8_ch_ref == 121.25);
	}

	{
		yggr::utf8_char utf8_ch = 'a';
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert(yggr::charset::utf8_foo::E_char_max_range / utf8_ch_ref
				== yggr::charset::utf8_foo::E_char_max_range / 'a');
	}

	// utf8_char_ref / utf8_char_ref
	{
		yggr::utf8_char utf8_ch_l = 'a' * 2;
		yggr::utf8_char utf8_ch_r = 2;
		ref_type utf8_ch_ref_l = utf8_ch_l;
		ref_type utf8_ch_ref_r = utf8_ch_r;

		yggr_test_assert(ref_type(utf8_ch_ref_l) / ref_type(utf8_ch_ref_r) == 'a');
		yggr_test_assert(ref_type(utf8_ch_ref_l) / utf8_ch_ref_r == 'a');
		yggr_test_assert(utf8_ch_ref_l / ref_type(utf8_ch_ref_r) == 'a');
		yggr_test_assert(utf8_ch_ref_l / utf8_ch_ref_r == 'a');
	}

#ifndef YGGR_NO_CWCHAR
	// utf8_char_ref / n
	{
		yggr::utf8_char utf8_ch = L'Äã' * 2;
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((utf8_ch_ref / yggr::charset::utf8_foo::E_char_max_range).is_validate());
		yggr_test_assert(utf8_ch_ref / 2 == L'Äã');
		yggr_test_assert(utf8_ch_ref / 2.9 == L'Äã' * 2 / 2.9);

		yggr_test_assert((utf8_ch_ref / 0.1).is_validate());
		yggr_test_assert((-utf8_ch_ref / -0.1).is_validate());

		yggr_test_assert(utf8_ch_ref / 0.1 == -utf8_ch / -0.1);

		yggr_test_assert((utf8_ch_ref / -2).is_validate());
		yggr_test_assert((-utf8_ch_ref / 2).is_validate());

		yggr_test_assert(utf8_ch_ref / -2 == -utf8_ch_ref / 2);
	}

	{
		yggr::utf8_char utf8_ch = L'Äã' * 2;
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((utf8_ch_ref / yggr::charset::utf8_foo::E_char_max_range).is_validate());
		yggr_test_assert(0 == (ref_type(utf8_ch_ref) / yggr::charset::utf8_foo::E_char_max_range));
		yggr_test_assert(utf8_ch_ref / 2 == L'Äã');
	}

	// n / utf8_char_ref
	{
		yggr::utf8_char utf8_ch = 2;
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert(yggr::charset::utf8_foo::E_char_max_range / utf8_ch_ref
				== yggr::charset::utf8_foo::E_char_max_range / 2);

		yggr_test_assert(L'Äã' * 2 / utf8_ch_ref == L'Äã');
		yggr_test_assert(L'Äã' * 2.5 / utf8_ch_ref == L'Äã' * 2.5 / 2);
	}

	{
		yggr::utf8_char utf8_ch = L'Äã';
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert(yggr::charset::utf8_foo::E_char_max_range / utf8_ch_ref
				== yggr::charset::utf8_foo::E_char_max_range / L'Äã');
	}

	// utf8_char_ref / utf8_char_ref
	{
		yggr::utf8_char utf8_ch_l = L'Äã' * 2;
		yggr::utf8_char utf8_ch_r = 2;
		ref_type utf8_ch_ref_l = utf8_ch_l;
		ref_type utf8_ch_ref_r = utf8_ch_r;

		yggr_test_assert(ref_type(utf8_ch_ref_l) / ref_type(utf8_ch_ref_r) == L'Äã');
		yggr_test_assert(ref_type(utf8_ch_ref_l) / utf8_ch_ref_r == L'Äã');
		yggr_test_assert(utf8_ch_ref_l / ref_type(utf8_ch_ref_r) == L'Äã');
		yggr_test_assert(utf8_ch_ref_l / utf8_ch_ref_r == L'Äã');
	}

#endif // YGGR_NO_CWCHAR

#ifdef _MSC_VER
#	pragma warning( pop )
#endif //_MSC_VER

	std::cout << "test_div_of_utf8_char_view succeed" << std::endl;
}

void test_mod_of_utf8_char(void)
{
	// utf8_char % n
	{
		yggr::utf8_char utf8_ch = 'a';
		yggr_test_assert(utf8_ch % yggr::charset::utf8_foo::E_char_max_range == 'a');
		yggr_test_assert(utf8_ch % (yggr::charset::utf8_foo::E_char_max_range * 2) == 'a');
		yggr_test_assert(utf8_ch % 2 == 1);

#if 0
		yggr_test_assert(utf8_ch % 2.9 == 0); // error % float
#endif // 0, 1
	}

	{
		yggr_test_assert(yggr::utf8_char('a') % yggr::charset::utf8_foo::E_char_max_range == 'a');
		yggr_test_assert(yggr::utf8_char('a') % 2 == 1);
	}

	// n % utf8_char
	{
		yggr::utf8_char utf8_ch = 'a';
		yggr_test_assert(yggr::charset::utf8_foo::E_char_max_range % utf8_ch
				== yggr::charset::utf8_foo::E_char_max_range % 'a');

		yggr_test_assert((2 % utf8_ch) == 2);
#if 0
		yggr_test_assert(2.5 % utf8_ch == 0); // error % float
#endif // 0, 1
	}

	{
		yggr_test_assert(yggr::charset::utf8_foo::E_char_max_range % yggr::utf8_char('a')
				== yggr::charset::utf8_foo::E_char_max_range % 'a');
	}

	// utf8_char % utf8_char
	{
		yggr::utf8_char utf8_ch = 'a';
		yggr_test_assert(yggr::utf8_char(2) % utf8_ch == 2);
		yggr_test_assert(yggr::utf8_char(2) % yggr::utf8_char('a') == 2);
	}

	// utf8_char % utf8_char_view
	{
		yggr::utf8_char utf8_ch = 'a';
		yggr_test_assert(yggr::utf8_char(2).view() % utf8_ch == 2);
		yggr_test_assert(yggr::utf8_char(2) % yggr::utf8_char('a').view() == 2);
	}

	// utf8_char % utf8_char_reference
	{
		yggr::utf8_char utf8_ch = 'a';
		yggr_test_assert(static_cast<yggr::utf8_char::char_reference_type>(utf8_ch) % yggr::utf8_char(2) == 1);
		yggr_test_assert(yggr::utf8_char(2) % yggr::utf8_char::char_reference_type(utf8_ch) == 2);
	}

	// utf8_char % utf8_char_const_reference
	{
		yggr::utf8_char utf8_ch = 'a';
		yggr_test_assert(static_cast<yggr::utf8_char::char_const_reference_type>(utf8_ch) % yggr::utf8_char(2) == 1);
		yggr_test_assert(yggr::utf8_char(2) % yggr::utf8_char::char_const_reference_type(utf8_ch) == 2);
	}

#ifndef YGGR_NO_CWCHAR
	// utf8_char % n
	{
		yggr::utf8_char utf8_ch = L'Äã';
		yggr_test_assert(utf8_ch % yggr::charset::utf8_foo::E_char_max_range == L'Äã');
	}

	{
		yggr_test_assert(yggr::utf8_char(L'Äã') % yggr::charset::utf8_foo::E_char_max_range == L'Äã');
	}

	// n % utf8_char
	{
		yggr::utf8_char utf8_ch = L'Äã';
		yggr_test_assert(yggr::charset::utf8_foo::E_char_max_range % utf8_ch
				== yggr::charset::utf8_foo::E_char_max_range % L'Äã');
	}

	{
		yggr_test_assert(yggr::charset::utf8_foo::E_char_max_range % yggr::utf8_char(L'Äã')
				== yggr::charset::utf8_foo::E_char_max_range % L'Äã');
	}

	// utf8_char + utf8_char
	{
		yggr::utf8_char utf8_ch = L'Äã';
		yggr_test_assert(yggr::utf8_char(2) % utf8_ch == 2);
		yggr_test_assert(yggr::utf8_char(2) % yggr::utf8_char(L'Äã') == 2);
	}

	// utf8_char + utf8_char_view
	{
		yggr::utf8_char utf8_ch = L'Äã';
		yggr_test_assert(yggr::utf8_char(2).view() % utf8_ch == 2);
		yggr_test_assert(yggr::utf8_char(2) % yggr::utf8_char(L'Äã').view() == 2);
	}

	// utf8_char + utf8_char_reference
	{
		yggr::utf8_char utf8_ch = L'Äã';
		yggr_test_assert(static_cast<yggr::utf8_char::char_reference_type>(utf8_ch) % yggr::utf8_char(2) == 0);
		yggr_test_assert(yggr::utf8_char(2) % yggr::utf8_char::char_reference_type(utf8_ch) == 2);
	}

	// utf8_char + utf8_char_const_reference
	{
		yggr::utf8_char utf8_ch = L'Äã';
		yggr_test_assert(static_cast<yggr::utf8_char::char_const_reference_type>(utf8_ch) % yggr::utf8_char(2) == 0);
		yggr_test_assert(yggr::utf8_char(2) % yggr::utf8_char::char_const_reference_type(utf8_ch) == 2);
	}

#endif // YGGR_NO_CWCHAR

	std::cout << "test_mod_of_utf8_char succeed" << std::endl;
}

void test_mod_of_utf8_char_reference(void)
{
	typedef yggr::utf8_char::char_reference_type ref_type;

	// utf8_char_ref % n
	{
		yggr::utf8_char utf8_ch = 'a';
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert(utf8_ch_ref % yggr::charset::utf8_foo::E_char_max_range == 'a');
		yggr_test_assert(utf8_ch_ref % (yggr::charset::utf8_foo::E_char_max_range * 2) == 'a');
		yggr_test_assert(utf8_ch_ref % 2 == 1);

#if 0
		yggr_test_assert(utf8_ch_ref % 2.9 == 0); // error % float
#endif // 0, 1
	}

	{
		yggr::utf8_char utf8_ch = 'a';
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert(utf8_ch_ref % yggr::charset::utf8_foo::E_char_max_range == 'a');
		yggr_test_assert(utf8_ch_ref % 2 == 1);
	}

	// n % utf8_char_ref
	{
		yggr::utf8_char utf8_ch = 'a';
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert(yggr::charset::utf8_foo::E_char_max_range % utf8_ch_ref
				== yggr::charset::utf8_foo::E_char_max_range % 'a');

		yggr_test_assert(2 % utf8_ch == 2);
#if 0
		yggr_test_assert(2.5 % utf8_ch == 0); // error % float
#endif // 0, 1
	}

	{
		yggr::utf8_char utf8_ch = 'a';
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert(yggr::charset::utf8_foo::E_char_max_range % utf8_ch_ref.const_cast_<ref_type>()
				== yggr::charset::utf8_foo::E_char_max_range % 'a');
	}

	// utf8_char_ref % utf8_char_ref
	{
		yggr::utf8_char utf8_ch_l = 'a';
		yggr::utf8_char utf8_ch_r = 2;
		ref_type utf8_ch_ref_l = utf8_ch_l;
		ref_type utf8_ch_ref_r = utf8_ch_r;

		yggr_test_assert(utf8_ch_ref_l % utf8_ch_ref_r == 1);
		yggr_test_assert(ref_type(utf8_ch_ref_l) % ref_type(utf8_ch_ref_r) == 1);
		yggr_test_assert(utf8_ch_ref_l % ref_type(utf8_ch_ref_r) == 1);
		yggr_test_assert(ref_type(utf8_ch_ref_r) % ref_type(utf8_ch_ref_l) == 2);
	}

	// utf8_char_ref % utf8_char_view
	{
		yggr::utf8_char utf8_ch_l = 'a';
		yggr::utf8_char utf8_ch_r = 2;
		ref_type utf8_ch_ref_l = utf8_ch_l;
		ref_type utf8_ch_ref_r = utf8_ch_r;

		yggr_test_assert(yggr::utf8_char(2).view() % utf8_ch_ref_l == 2);
		yggr_test_assert(utf8_ch_ref_r % yggr::utf8_char('a').view() == 2);
	}

	// utf8_char_ref % utf8_char_const_reference
	{
		yggr::utf8_char utf8_ch_l = 'a';
		yggr::utf8_char utf8_ch_r = 2;
		ref_type utf8_ch_ref_l = utf8_ch_l;
		ref_type utf8_ch_ref_r = utf8_ch_r;

		yggr_test_assert(utf8_ch_ref_l.const_cast_() % utf8_ch_ref_r == 1);
		yggr_test_assert(utf8_ch_ref_r % utf8_ch_ref_l.const_cast_() == 2);
	}

#ifndef YGGR_NO_CWCHAR
	// utf8_char_ref % n
	{
		yggr::utf8_char utf8_ch = L'Äã';
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert(utf8_ch_ref % yggr::charset::utf8_foo::E_char_max_range == L'Äã');
	}

	{
		yggr::utf8_char utf8_ch = L'Äã';
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert(ref_type(utf8_ch_ref) % yggr::charset::utf8_foo::E_char_max_range == L'Äã');
	}

	// n % utf8_char_ref
	{
		yggr::utf8_char utf8_ch = L'Äã';
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert(yggr::charset::utf8_foo::E_char_max_range % utf8_ch_ref
				== yggr::charset::utf8_foo::E_char_max_range % L'Äã');
	}

	{
		yggr::utf8_char utf8_ch = L'Äã';
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert(yggr::charset::utf8_foo::E_char_max_range % ref_type(utf8_ch_ref)
				== yggr::charset::utf8_foo::E_char_max_range % L'Äã');
	}

	// utf8_char_ref + utf8_char_ref
	{
		yggr::utf8_char utf8_ch_l = L'Äã';
		yggr::utf8_char utf8_ch_r = 2;
		ref_type utf8_ch_ref_l = utf8_ch_l;
		ref_type utf8_ch_ref_r = utf8_ch_r;

		yggr_test_assert(utf8_ch_ref_l % utf8_ch_ref_r == 0);
		yggr_test_assert(ref_type(utf8_ch_ref_l) % utf8_ch_ref_r == 0);
		yggr_test_assert(utf8_ch_ref_l % ref_type(utf8_ch_ref_r) == 0);
		yggr_test_assert(ref_type(utf8_ch_ref_r) % ref_type(utf8_ch_ref_l) == 2);
	}

	// utf8_char_ref + utf8_char_view
	{
		yggr::utf8_char utf8_ch_l = L'Äã';
		yggr::utf8_char utf8_ch_r = 2;
		ref_type utf8_ch_ref_l = utf8_ch_l;
		ref_type utf8_ch_ref_r = utf8_ch_r;

		yggr_test_assert(yggr::utf8_char(2).view() % utf8_ch_ref_l == 2);
		yggr_test_assert(utf8_ch_ref_r % yggr::utf8_char(L'Äã').view() == 2);
	}

	// utf8_char_ref + utf8_char_const_reference
	{
		yggr::utf8_char utf8_ch_l = L'Äã';
		yggr::utf8_char utf8_ch_r = 2;
		ref_type utf8_ch_ref_l = utf8_ch_l;
		ref_type utf8_ch_ref_r = utf8_ch_r;

		yggr_test_assert(utf8_ch_ref_l.const_cast_() % utf8_ch_ref_r == 0);
		yggr_test_assert(utf8_ch_ref_r % utf8_ch_ref_l.const_cast_() == 2);
	}

#endif // YGGR_NO_CWCHAR

	std::cout << "test_mod_of_utf8_char_reference succeed" << std::endl;
}

void test_mod_of_utf8_char_const_reference(void)
{
	typedef yggr::utf8_char::char_const_reference_type ref_type;

	// utf8_char_ref % n
	{
		yggr::utf8_char utf8_ch = 'a';
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert(utf8_ch_ref % yggr::charset::utf8_foo::E_char_max_range == 'a');
		yggr_test_assert(utf8_ch_ref % (yggr::charset::utf8_foo::E_char_max_range * 2) == 'a');
		yggr_test_assert(utf8_ch_ref % 2 == 1);

#if 0
		yggr_test_assert(utf8_ch_ref % 2.9 == 0); // error % float
#endif // 0, 1
	}

	{
		yggr::utf8_char utf8_ch = 'a';
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert(utf8_ch_ref % yggr::charset::utf8_foo::E_char_max_range == 'a');
		yggr_test_assert(utf8_ch_ref % 2 == 1);
	}

	// n % utf8_char_ref
	{
		yggr::utf8_char utf8_ch = 'a';
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert(yggr::charset::utf8_foo::E_char_max_range % utf8_ch_ref
				== yggr::charset::utf8_foo::E_char_max_range % 'a');

		yggr_test_assert(2 % utf8_ch == 2);
#if 0
		yggr_test_assert(2.5 % utf8_ch == 0); // error % float
#endif // 0, 1
	}

	{
		yggr::utf8_char utf8_ch = 'a';
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert(yggr::charset::utf8_foo::E_char_max_range % utf8_ch_ref.const_cast_<ref_type>()
				== yggr::charset::utf8_foo::E_char_max_range % 'a');
	}

	// utf8_char_ref % utf8_char_ref
	{
		yggr::utf8_char utf8_ch_l = 'a';
		yggr::utf8_char utf8_ch_r = 2;
		ref_type utf8_ch_ref_l = utf8_ch_l;
		ref_type utf8_ch_ref_r = utf8_ch_r;

		yggr_test_assert(utf8_ch_ref_l % utf8_ch_ref_r == 1);
		yggr_test_assert(ref_type(utf8_ch_ref_l) % ref_type(utf8_ch_ref_r) == 1);
		yggr_test_assert(utf8_ch_ref_l % ref_type(utf8_ch_ref_r) == 1);
		yggr_test_assert(ref_type(utf8_ch_ref_r) % ref_type(utf8_ch_ref_l) == 2);
	}

	// utf8_char_ref % utf8_char_view
	{
		yggr::utf8_char utf8_ch_l = 'a';
		yggr::utf8_char utf8_ch_r = 2;
		ref_type utf8_ch_ref_l = utf8_ch_l;
		ref_type utf8_ch_ref_r = utf8_ch_r;

		yggr_test_assert(yggr::utf8_char(2).view() % utf8_ch_ref_l == 2);
		yggr_test_assert(utf8_ch_ref_r % yggr::utf8_char('a').view() == 2);
	}

#ifndef YGGR_NO_CWCHAR
	// utf8_char_ref % n
	{
		yggr::utf8_char utf8_ch = L'Äã';
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert(utf8_ch_ref % yggr::charset::utf8_foo::E_char_max_range == L'Äã');
	}

	{
		yggr::utf8_char utf8_ch = L'Äã';
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert(ref_type(utf8_ch_ref) % yggr::charset::utf8_foo::E_char_max_range == L'Äã');
	}

	// n % utf8_char_ref
	{
		yggr::utf8_char utf8_ch = L'Äã';
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert(yggr::charset::utf8_foo::E_char_max_range % utf8_ch_ref
				== yggr::charset::utf8_foo::E_char_max_range % L'Äã');
	}

	{
		yggr::utf8_char utf8_ch = L'Äã';
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert(yggr::charset::utf8_foo::E_char_max_range % ref_type(utf8_ch_ref)
				== yggr::charset::utf8_foo::E_char_max_range % L'Äã');
	}

	// utf8_char_ref + utf8_char_ref
	{
		yggr::utf8_char utf8_ch_l = L'Äã';
		yggr::utf8_char utf8_ch_r = 2;
		ref_type utf8_ch_ref_l = utf8_ch_l;
		ref_type utf8_ch_ref_r = utf8_ch_r;

		yggr_test_assert(utf8_ch_ref_l % utf8_ch_ref_r == 0);
		yggr_test_assert(ref_type(utf8_ch_ref_l) % utf8_ch_ref_r == 0);
		yggr_test_assert(utf8_ch_ref_l % ref_type(utf8_ch_ref_r) == 0);
		yggr_test_assert(ref_type(utf8_ch_ref_r) % ref_type(utf8_ch_ref_l) == 2);
	}

	// utf8_char_ref + utf8_char_view
	{
		yggr::utf8_char utf8_ch_l = L'Äã';
		yggr::utf8_char utf8_ch_r = 2;
		ref_type utf8_ch_ref_l = utf8_ch_l;
		ref_type utf8_ch_ref_r = utf8_ch_r;

		yggr_test_assert(yggr::utf8_char(2).view() % utf8_ch_ref_l == 2);
		yggr_test_assert(utf8_ch_ref_r % yggr::utf8_char(L'Äã').view() == 2);
	}

#endif // YGGR_NO_CWCHAR

	std::cout << "test_mod_of_utf8_char_const_reference succeed" << std::endl;
}

void test_mod_of_utf8_char_view(void)
{
	typedef yggr::utf8_char::char_view_type ref_type;

	// utf8_char_ref % n
	{
		yggr::utf8_char utf8_ch = 'a';
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert(utf8_ch_ref % yggr::charset::utf8_foo::E_char_max_range == 'a');
		yggr_test_assert(utf8_ch_ref % (yggr::charset::utf8_foo::E_char_max_range * 2) == 'a');
		yggr_test_assert(utf8_ch_ref % 2 == 1);

#if 0
		yggr_test_assert(utf8_ch_ref % 2.9 == 0); // error % float
#endif // 0, 1
	}

	{
		yggr::utf8_char utf8_ch = 'a';
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert(utf8_ch_ref % yggr::charset::utf8_foo::E_char_max_range == 'a');
		yggr_test_assert(utf8_ch_ref % 2 == 1);
	}

	// n % utf8_char_ref
	{
		yggr::utf8_char utf8_ch = 'a';
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert(yggr::charset::utf8_foo::E_char_max_range % utf8_ch_ref
				== yggr::charset::utf8_foo::E_char_max_range % 'a');

		yggr_test_assert(2 % utf8_ch == 2);
#if 0
		yggr_test_assert(2.5 % utf8_ch == 0); // error % float
#endif // 0, 1
	}

	{
		yggr::utf8_char utf8_ch = 'a';
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert(yggr::charset::utf8_foo::E_char_max_range % ref_type(utf8_ch_ref)
				== yggr::charset::utf8_foo::E_char_max_range % 'a');
	}

	// utf8_char_ref % utf8_char_ref
	{
		yggr::utf8_char utf8_ch_l = 'a';
		yggr::utf8_char utf8_ch_r = 2;
		ref_type utf8_ch_ref_l = utf8_ch_l;
		ref_type utf8_ch_ref_r = utf8_ch_r;

		yggr_test_assert(utf8_ch_ref_l % utf8_ch_ref_r == 1);
		yggr_test_assert(ref_type(utf8_ch_ref_l) % ref_type(utf8_ch_ref_r) == 1);
		yggr_test_assert(utf8_ch_ref_l % ref_type(utf8_ch_ref_r) == 1);
		yggr_test_assert(ref_type(utf8_ch_ref_r) % ref_type(utf8_ch_ref_l) == 2);
	}

#ifndef YGGR_NO_CWCHAR
	// utf8_char_ref % n
	{
		yggr::utf8_char utf8_ch = L'Äã';
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert(utf8_ch_ref % yggr::charset::utf8_foo::E_char_max_range == L'Äã');
	}

	{
		yggr::utf8_char utf8_ch = L'Äã';
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert(ref_type(utf8_ch_ref) % yggr::charset::utf8_foo::E_char_max_range == L'Äã');
	}

	// n % utf8_char_ref
	{
		yggr::utf8_char utf8_ch = L'Äã';
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert(yggr::charset::utf8_foo::E_char_max_range % utf8_ch_ref
				== yggr::charset::utf8_foo::E_char_max_range % L'Äã');
	}

	{
		yggr::utf8_char utf8_ch = L'Äã';
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert(yggr::charset::utf8_foo::E_char_max_range % ref_type(utf8_ch_ref)
				== yggr::charset::utf8_foo::E_char_max_range % L'Äã');
	}

	// utf8_char_ref + utf8_char_ref
	{
		yggr::utf8_char utf8_ch_l = L'Äã';
		yggr::utf8_char utf8_ch_r = 2;
		ref_type utf8_ch_ref_l = utf8_ch_l;
		ref_type utf8_ch_ref_r = utf8_ch_r;

		yggr_test_assert(utf8_ch_ref_l % utf8_ch_ref_r == 0);
		yggr_test_assert(ref_type(utf8_ch_ref_l) % utf8_ch_ref_r == 0);
		yggr_test_assert(utf8_ch_ref_l % ref_type(utf8_ch_ref_r) == 0);
		yggr_test_assert(ref_type(utf8_ch_ref_r) % ref_type(utf8_ch_ref_l) == 2);
	}

#endif // YGGR_NO_CWCHAR

	std::cout << "test_mod_of_utf8_char_view succeed" << std::endl;
}

void test_bitwise_left_shfit_of_utf8_char(void)
{
	// utf8_char << n
	{
		yggr::utf8_char utf8_ch = 49;
		yggr_test_assert(utf8_ch << 1 == 'b');

		yggr_test_assert((-utf8_ch << 1).is_validate());
		yggr_test_assert((utf8_ch << 1).is_validate());
		yggr_test_assert((utf8_ch * 2) == (utf8_ch << 1));
		yggr_test_assert((-utf8_ch * 2) == -utf8_ch << 1);
	}

	{
		yggr_test_assert(yggr::utf8_char(49) << 1 == 'b');
	}

	// n << utf8_char
	{
		yggr::utf8_char utf8_ch = 2;
		yggr_test_assert(2 << utf8_ch == 8);
	}

	// utf8_char << utf8_char
	{
		yggr::utf8_char utf8_ch = 1;
		yggr_test_assert(yggr::utf8_char(49) << utf8_ch == 'b');
		yggr_test_assert(yggr::utf8_char(49) << yggr::utf8_char(1) == 'b');
	}

	// utf8_char << utf8_char_view
	{
		yggr::utf8_char utf8_ch = 1;
		yggr_test_assert(yggr::utf8_char(49).view() << utf8_ch == 'b');
		yggr_test_assert(yggr::utf8_char(49) << yggr::utf8_char(1).view() == 'b');
	}

	// utf8_char << utf8_char_reference
	{
		yggr::utf8_char utf8_ch = 49;
		yggr_test_assert(static_cast<yggr::utf8_char::char_reference_type>(utf8_ch) << yggr::utf8_char(1) == 'b');
		yggr_test_assert(utf8_ch << yggr::utf8_char::char_reference_type(yggr::utf8_char(1)) == 'b');
	}

	// utf8_char << utf8_char_const_reference
	{
		yggr::utf8_char utf8_ch = 49;
		yggr_test_assert(static_cast<yggr::utf8_char::char_const_reference_type>(utf8_ch) << yggr::utf8_char(1) == 'b');
		yggr_test_assert(utf8_ch << yggr::utf8_char::char_const_reference_type(yggr::utf8_char(1)) == 'b');
	}

#ifndef YGGR_NO_CWCHAR

	// utf8_char << n
	{
		yggr::utf8_char utf8_ch = L'Äã' / 2;
		yggr_test_assert(utf8_ch << 1 == L'Äã');

		yggr_test_assert((-utf8_ch << 1).is_validate());
		yggr_test_assert((utf8_ch << 1).is_validate());
		yggr_test_assert((utf8_ch * 2) == (utf8_ch << 1));
		yggr_test_assert((-utf8_ch * 2) == -utf8_ch << 1);
	}

	{
		yggr_test_assert(yggr::utf8_char(L'Äã' / 2) << 1 == L'Äã');
	}

	// n << utf8_char
	{
		yggr::utf8_char utf8_ch = 2;
		yggr_test_assert(2 << utf8_ch == 8);
	}

	// utf8_char << utf8_char
	{
		yggr::utf8_char utf8_ch = 1;
		yggr_test_assert(yggr::utf8_char(L'Äã' / 2) << utf8_ch == L'Äã');
		yggr_test_assert(yggr::utf8_char(L'Äã' / 2) << yggr::utf8_char(1) == L'Äã');
	}

	// utf8_char << utf8_char_view
	{
		yggr::utf8_char utf8_ch = 1;
		yggr_test_assert(yggr::utf8_char(L'Äã' / 2).view() << utf8_ch == L'Äã');
		yggr_test_assert(yggr::utf8_char(L'Äã' / 2) << yggr::utf8_char(1).view() == L'Äã');
	}

	// utf8_char << utf8_char_reference
	{
		yggr::utf8_char utf8_ch = L'Äã' / 2;
		yggr_test_assert(static_cast<yggr::utf8_char::char_reference_type>(utf8_ch) << yggr::utf8_char(1) == L'Äã');
		yggr_test_assert(utf8_ch << yggr::utf8_char::char_reference_type(yggr::utf8_char(1)) == L'Äã');
	}

	// utf8_char << utf8_char_const_reference
	{
		yggr::utf8_char utf8_ch = L'Äã' / 2;
		yggr_test_assert(static_cast<yggr::utf8_char::char_const_reference_type>(utf8_ch) << yggr::utf8_char(1) == L'Äã');
		yggr_test_assert(utf8_ch << yggr::utf8_char::char_const_reference_type(yggr::utf8_char(1)) == L'Äã');
	}

#endif // YGGR_NO_CWCHAR

	std::cout << "test_bitwise_left_shfit_of_utf8_char succeed" << std::endl;
}

void test_bitwise_left_shfit_of_utf8_char_reference(void)
{
	typedef yggr::utf8_char::char_reference_type ref_type;

	// utf8_char_ref << n
	{
		yggr::utf8_char utf8_ch = 49;
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert(utf8_ch_ref << 1 == 'b');

		yggr_test_assert((-utf8_ch_ref << 1).is_validate());
		yggr_test_assert((utf8_ch_ref << 1).is_validate());
		yggr_test_assert((utf8_ch_ref * 2) == (utf8_ch_ref << 1));
		yggr_test_assert((-utf8_ch_ref * 2) == -utf8_ch_ref << 1);
	}

	{
		yggr::utf8_char utf8_ch = 49;
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert(ref_type(utf8_ch_ref) << 1 == 'b');
	}

	// n << utf8_char_ref
	{
		yggr::utf8_char utf8_ch = 2;
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert(2 << utf8_ch_ref == 8);
	}

	// utf8_char_ref << utf8_char_ref
	{
		yggr::utf8_char utf8_ch_l = 49;
		yggr::utf8_char utf8_ch_r = 1;
		ref_type utf8_ch_ref_l = utf8_ch_l;
		ref_type utf8_ch_ref_r = utf8_ch_r;

		yggr_test_assert(ref_type(utf8_ch_ref_l) << ref_type(utf8_ch_ref_r) == 'b');
		yggr_test_assert(ref_type(utf8_ch_ref_l) << utf8_ch_ref_r == 'b');
		yggr_test_assert(utf8_ch_ref_l << ref_type(utf8_ch_ref_r) == 'b');
		yggr_test_assert(utf8_ch_ref_l << utf8_ch_ref_r == 'b');
	}

	// utf8_char_ref << utf8_char_view
	{
		yggr::utf8_char utf8_ch_l = 49;
		yggr::utf8_char utf8_ch_r = 1;
		ref_type utf8_ch_ref_l = utf8_ch_l;
		ref_type utf8_ch_ref_r = utf8_ch_r;

		yggr_test_assert(yggr::utf8_char(49).view() << utf8_ch_ref_r == 'b');
		yggr_test_assert(utf8_ch_ref_l << yggr::utf8_char(1).view() == 'b');
	}

	// utf8_char_ref << utf8_char_const_reference
	{
		yggr::utf8_char utf8_ch_l = 49;
		yggr::utf8_char utf8_ch_r = 1;
		ref_type utf8_ch_ref_l = utf8_ch_l;
		ref_type utf8_ch_ref_r = utf8_ch_r;

		yggr_test_assert(utf8_ch_ref_l.const_cast_() << utf8_ch_ref_r == 'b');
		yggr_test_assert(utf8_ch_ref_l << utf8_ch_ref_r.const_cast_() == 'b');
	}

#ifndef YGGR_NO_CWCHAR

	// utf8_char_ref << n
	{
		yggr::utf8_char utf8_ch = L'Äã' / 2;
		yggr_test_assert(utf8_ch << 1 == L'Äã');

		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((-utf8_ch_ref << 1).is_validate());
		yggr_test_assert((utf8_ch_ref << 1).is_validate());
		yggr_test_assert((utf8_ch_ref * 2) == (utf8_ch_ref << 1));
		yggr_test_assert((-utf8_ch * 2) == -utf8_ch_ref << 1);
	}

	{
		yggr::utf8_char utf8_ch = L'Äã' / 2;
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert(ref_type(utf8_ch_ref) << 1 == L'Äã');
	}

	// n << utf8_char_ref
	{
		yggr::utf8_char utf8_ch = 2;
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert(2 << utf8_ch_ref == 8);
	}

	// utf8_char_ref << utf8_char_ref
	{
		yggr::utf8_char utf8_ch_l = L'Äã' / 2;
		yggr::utf8_char utf8_ch_r = 1;
		ref_type utf8_ch_ref_l = utf8_ch_l;
		ref_type utf8_ch_ref_r = utf8_ch_r;

		yggr_test_assert(ref_type(utf8_ch_ref_l) << ref_type(utf8_ch_ref_r) == L'Äã');
		yggr_test_assert(ref_type(utf8_ch_ref_l) << utf8_ch_ref_r == L'Äã');
		yggr_test_assert(utf8_ch_ref_l << ref_type(utf8_ch_ref_r) == L'Äã');
		yggr_test_assert(utf8_ch_ref_l << utf8_ch_ref_r == L'Äã');
	}

	// utf8_char_ref << utf8_char_view
	{
		yggr::utf8_char utf8_ch_l = L'Äã' / 2;
		yggr::utf8_char utf8_ch_r = 1;
		ref_type utf8_ch_ref_l = utf8_ch_l;
		ref_type utf8_ch_ref_r = utf8_ch_r;

		yggr_test_assert(yggr::utf8_char(L'Äã' / 2).view() << utf8_ch_ref_r == L'Äã');
		yggr_test_assert(utf8_ch_ref_l << yggr::utf8_char(1).view() == L'Äã');
	}

	// utf8_char_ref << utf8_char_const_reference
	{
		yggr::utf8_char utf8_ch_l = L'Äã' / 2;
		yggr::utf8_char utf8_ch_r = 1;
		ref_type utf8_ch_ref_l = utf8_ch_l;
		ref_type utf8_ch_ref_r = utf8_ch_r;

		yggr_test_assert(utf8_ch_ref_l.const_cast_() << utf8_ch_ref_r == L'Äã');
		yggr_test_assert(utf8_ch_ref_l << utf8_ch_ref_r.const_cast_() == L'Äã');
	}

#endif // YGGR_NO_CWCHAR

	std::cout << "test_bitwise_left_shfit_of_utf8_char_reference succeed" << std::endl;
}

void test_bitwise_left_shfit_of_utf8_char_const_reference(void)
{
	typedef yggr::utf8_char::char_const_reference_type ref_type;

	// utf8_char_ref << n
	{
		yggr::utf8_char utf8_ch = 49;
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert(utf8_ch_ref << 1 == 'b');

		yggr_test_assert((-utf8_ch_ref << 1).is_validate());
		yggr_test_assert((utf8_ch_ref << 1).is_validate());
		yggr_test_assert((utf8_ch_ref * 2) == (utf8_ch_ref << 1));
		yggr_test_assert((-utf8_ch_ref * 2) == -utf8_ch_ref << 1);
	}

	{
		yggr::utf8_char utf8_ch = 49;
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert(ref_type(utf8_ch_ref) << 1 == 'b');
	}

	// n << utf8_char_ref
	{
		yggr::utf8_char utf8_ch = 2;
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert(2 << utf8_ch_ref == 8);
	}

	// utf8_char_ref << utf8_char_ref
	{
		yggr::utf8_char utf8_ch_l = 49;
		yggr::utf8_char utf8_ch_r = 1;
		ref_type utf8_ch_ref_l = utf8_ch_l;
		ref_type utf8_ch_ref_r = utf8_ch_r;

		yggr_test_assert(ref_type(utf8_ch_ref_l) << ref_type(utf8_ch_ref_r) == 'b');
		yggr_test_assert(ref_type(utf8_ch_ref_l) << utf8_ch_ref_r == 'b');
		yggr_test_assert(utf8_ch_ref_l << ref_type(utf8_ch_ref_r) == 'b');
		yggr_test_assert(utf8_ch_ref_l << utf8_ch_ref_r == 'b');
	}

	// utf8_char_ref << utf8_char_view
	{
		yggr::utf8_char utf8_ch_l = 49;
		yggr::utf8_char utf8_ch_r = 1;
		ref_type utf8_ch_ref_l = utf8_ch_l;
		ref_type utf8_ch_ref_r = utf8_ch_r;

		yggr_test_assert(yggr::utf8_char(49).view() << utf8_ch_ref_r == 'b');
		yggr_test_assert(utf8_ch_ref_l << yggr::utf8_char(1).view() == 'b');
	}

#ifndef YGGR_NO_CWCHAR

	// utf8_char_ref << n
	{
		yggr::utf8_char utf8_ch = L'Äã' / 2;
		yggr_test_assert(utf8_ch << 1 == L'Äã');

		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((-utf8_ch_ref << 1).is_validate());
		yggr_test_assert((utf8_ch_ref << 1).is_validate());
		yggr_test_assert((utf8_ch_ref * 2) == (utf8_ch_ref << 1));
		yggr_test_assert((-utf8_ch * 2) == -utf8_ch_ref << 1);
	}

	{
		yggr::utf8_char utf8_ch = L'Äã' / 2;
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert(ref_type(utf8_ch_ref) << 1 == L'Äã');
	}

	// n << utf8_char_ref
	{
		yggr::utf8_char utf8_ch = 2;
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert(2 << utf8_ch_ref == 8);
	}

	// utf8_char_ref << utf8_char_ref
	{
		yggr::utf8_char utf8_ch_l = L'Äã' / 2;
		yggr::utf8_char utf8_ch_r = 1;
		ref_type utf8_ch_ref_l = utf8_ch_l;
		ref_type utf8_ch_ref_r = utf8_ch_r;

		yggr_test_assert(ref_type(utf8_ch_ref_l) << ref_type(utf8_ch_ref_r) == L'Äã');
		yggr_test_assert(ref_type(utf8_ch_ref_l) << utf8_ch_ref_r == L'Äã');
		yggr_test_assert(utf8_ch_ref_l << ref_type(utf8_ch_ref_r) == L'Äã');
		yggr_test_assert(utf8_ch_ref_l << utf8_ch_ref_r == L'Äã');
	}

	// utf8_char_ref << utf8_char_view
	{
		yggr::utf8_char utf8_ch_l = L'Äã' / 2;
		yggr::utf8_char utf8_ch_r = 1;
		ref_type utf8_ch_ref_l = utf8_ch_l;
		ref_type utf8_ch_ref_r = utf8_ch_r;

		yggr_test_assert(yggr::utf8_char(L'Äã' / 2).view() << utf8_ch_ref_r == L'Äã');
		yggr_test_assert(utf8_ch_ref_l << yggr::utf8_char(1).view() == L'Äã');
	}

#endif // YGGR_NO_CWCHAR

	std::cout << "test_bitwise_left_shfit_of_utf8_char_const_reference succeed" << std::endl;
}

void test_bitwise_left_shfit_of_utf8_char_view(void)
{
	typedef yggr::utf8_char::char_const_reference_type ref_type;

	// utf8_char_ref << n
	{
		yggr::utf8_char utf8_ch = 49;
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert(utf8_ch_ref << 1 == 'b');

		yggr_test_assert((-utf8_ch_ref << 1).is_validate());
		yggr_test_assert((utf8_ch_ref << 1).is_validate());
		yggr_test_assert((utf8_ch_ref * 2) == (utf8_ch_ref << 1));
		yggr_test_assert((-utf8_ch_ref * 2) == -utf8_ch_ref << 1);
	}

	{
		yggr::utf8_char utf8_ch = 49;
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert(ref_type(utf8_ch_ref) << 1 == 'b');
	}

	// n << utf8_char_ref
	{
		yggr::utf8_char utf8_ch = 2;
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert(2 << utf8_ch_ref == 8);
	}

	// utf8_char_ref << utf8_char_ref
	{
		yggr::utf8_char utf8_ch_l = 49;
		yggr::utf8_char utf8_ch_r = 1;
		ref_type utf8_ch_ref_l = utf8_ch_l;
		ref_type utf8_ch_ref_r = utf8_ch_r;

		yggr_test_assert(ref_type(utf8_ch_ref_l) << ref_type(utf8_ch_ref_r) == 'b');
		yggr_test_assert(ref_type(utf8_ch_ref_l) << utf8_ch_ref_r == 'b');
		yggr_test_assert(utf8_ch_ref_l << ref_type(utf8_ch_ref_r) == 'b');
		yggr_test_assert(utf8_ch_ref_l << utf8_ch_ref_r == 'b');
	}

#ifndef YGGR_NO_CWCHAR

	// utf8_char_ref << n
	{
		yggr::utf8_char utf8_ch = L'Äã' / 2;
		yggr_test_assert(utf8_ch << 1 == L'Äã');

		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((-utf8_ch_ref << 1).is_validate());
		yggr_test_assert((utf8_ch_ref << 1).is_validate());
		yggr_test_assert((utf8_ch_ref * 2) == (utf8_ch_ref << 1));
		yggr_test_assert((-utf8_ch * 2) == -utf8_ch_ref << 1);
	}

	{
		yggr::utf8_char utf8_ch = L'Äã' / 2;
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert(ref_type(utf8_ch_ref) << 1 == L'Äã');
	}

	// n << utf8_char_ref
	{
		yggr::utf8_char utf8_ch = 2;
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert(2 << utf8_ch_ref == 8);
	}

	// utf8_char_ref << utf8_char_ref
	{
		yggr::utf8_char utf8_ch_l = L'Äã' / 2;
		yggr::utf8_char utf8_ch_r = 1;
		ref_type utf8_ch_ref_l = utf8_ch_l;
		ref_type utf8_ch_ref_r = utf8_ch_r;

		yggr_test_assert(ref_type(utf8_ch_ref_l) << ref_type(utf8_ch_ref_r) == L'Äã');
		yggr_test_assert(ref_type(utf8_ch_ref_l) << utf8_ch_ref_r == L'Äã');
		yggr_test_assert(utf8_ch_ref_l << ref_type(utf8_ch_ref_r) == L'Äã');
		yggr_test_assert(utf8_ch_ref_l << utf8_ch_ref_r == L'Äã');
	}

#endif // YGGR_NO_CWCHAR

	std::cout << "test_bitwise_left_shfit_of_utf8_char_view succeed" << std::endl;
}

void test_bitwise_right_shift_of_utf8_char(void)
{
	// utf8_char >> n
	{
		yggr::utf8_char utf8_ch = 'a' * 2;
		yggr_test_assert(utf8_ch >> 1 == 'a');
		yggr_test_assert((-utf8_ch >> 1).is_validate());

		yggr_test_assert((-utf8_ch >> 1) == (-utf8_ch / 2));
	}

	// n >> utf8_char
	{
		yggr::utf8_char utf8_ch = 1;
		yggr_test_assert(yggr::charset::utf8_foo::E_char_max_range >> utf8_ch
				== yggr::charset::utf8_foo::E_char_max_range / 2);

		yggr_test_assert('a' * 2 >> utf8_ch == 'a');
	}

	// utf8_char >> utf8_char
	{
		yggr::utf8_char utf8_ch = 1;
		yggr_test_assert(yggr::utf8_char('a' * 2) >> utf8_ch == 'a');
		yggr_test_assert(yggr::utf8_char('a' * 2) >> yggr::utf8_char(1) == 'a');
	}

	// utf8_char >> utf8_char_view
	{
		yggr::utf8_char utf8_ch = 1;
		yggr_test_assert(yggr::utf8_char('a' * 2).view() >> utf8_ch == 'a');
		yggr_test_assert(yggr::utf8_char('a' * 2) >> yggr::utf8_char(1).view() == 'a');
	}

	// utf8_char >> utf8_char_reference
	{
		yggr::utf8_char utf8_ch = 'a' * 2;
		yggr_test_assert(static_cast<yggr::utf8_char::char_reference_type>(utf8_ch) >> yggr::utf8_char(1) == 'a');
		yggr_test_assert(utf8_ch >> yggr::utf8_char::char_reference_type(yggr::utf8_char(1)) == 'a');
	}

	// utf8_char >> utf8_char_const_reference
	{
		yggr::utf8_char utf8_ch = 'a' * 2;
		yggr_test_assert(static_cast<yggr::utf8_char::char_const_reference_type>(utf8_ch) >> yggr::utf8_char(1) == 'a');
		yggr_test_assert(utf8_ch >> yggr::utf8_char::char_const_reference_type(yggr::utf8_char(1)) == 'a');
	}

#ifndef YGGR_NO_CWCHAR
	// utf8_char >> n
	{
		yggr::utf8_char utf8_ch = L'Äã' * 2;
		yggr_test_assert(utf8_ch >> 1 == L'Äã');
		yggr_test_assert((-utf8_ch >> 1).is_validate());

		yggr_test_assert((-utf8_ch >> 1) == (-utf8_ch / 2));
	}

	// n >> utf8_char
	{
		yggr::utf8_char utf8_ch = 1;
		yggr_test_assert(yggr::charset::utf8_foo::E_char_max_range >> utf8_ch
				== yggr::charset::utf8_foo::E_char_max_range / 2);

		yggr_test_assert(L'Äã' * 2 >> utf8_ch == L'Äã');
	}

	// utf8_char >> utf8_char
	{
		yggr::utf8_char utf8_ch = 1;
		yggr_test_assert(yggr::utf8_char(L'Äã' * 2) >> utf8_ch == L'Äã');
		yggr_test_assert(yggr::utf8_char(L'Äã' * 2) >> yggr::utf8_char(1) == L'Äã');
	}

	// utf8_char >> utf8_char_view
	{
		yggr::utf8_char utf8_ch = 1;
		yggr_test_assert(yggr::utf8_char(L'Äã' * 2).view() >> utf8_ch == L'Äã');
		yggr_test_assert(yggr::utf8_char(L'Äã' * 2) >> yggr::utf8_char(1).view() == L'Äã');
	}

	// utf8_char >> utf8_char_reference
	{
		yggr::utf8_char utf8_ch = L'Äã' * 2;
		yggr_test_assert(static_cast<yggr::utf8_char::char_reference_type>(utf8_ch) >> yggr::utf8_char(1) == L'Äã');
		yggr_test_assert(utf8_ch >> yggr::utf8_char::char_reference_type(yggr::utf8_char(1)) == L'Äã');
	}

	// utf8_char >> utf8_char_const_reference
	{
		yggr::utf8_char utf8_ch = L'Äã' * 2;
		yggr_test_assert(static_cast<yggr::utf8_char::char_const_reference_type>(utf8_ch) >> yggr::utf8_char(1) == L'Äã');
		yggr_test_assert(utf8_ch >> yggr::utf8_char::char_const_reference_type(yggr::utf8_char(1)) == L'Äã');
	}
#endif // YGGR_NO_CWCHAR

	std::cout << "test_bitwise_right_shift_of_utf8_char succeed" << std::endl;
}

void test_bitwise_right_shift_of_utf8_char_reference(void)
{
	typedef yggr::utf8_char::char_reference_type ref_type;

	// utf8_char_ref >> n
	{
		yggr::utf8_char utf8_ch = 'a' * 2;
		yggr_test_assert(utf8_ch >> 1 == 'a');

		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((-utf8_ch_ref >> 1).is_validate());
		yggr_test_assert((-utf8_ch_ref >> 1) == (-utf8_ch_ref / 2));
	}

	// n >> utf8_char_ref
	{
		yggr::utf8_char utf8_ch = 1;
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert(yggr::charset::utf8_foo::E_char_max_range >> utf8_ch_ref
				== yggr::charset::utf8_foo::E_char_max_range / 2);

		yggr_test_assert('a' * 2 >> utf8_ch_ref == 'a');
	}

	// utf8_char_ref >> utf8_char_ref
	{
		yggr::utf8_char utf8_ch_l = 'a' * 2;
		yggr::utf8_char utf8_ch_r = 1;
		ref_type utf8_ch_ref_l = utf8_ch_l;
		ref_type utf8_ch_ref_r = utf8_ch_r;

		yggr_test_assert(ref_type(utf8_ch_ref_l) >> ref_type(utf8_ch_ref_r) == 'a');
		yggr_test_assert(ref_type(utf8_ch_ref_l) >> utf8_ch_ref_r == 'a');
		yggr_test_assert(utf8_ch_ref_l >> ref_type(utf8_ch_ref_r) == 'a');
		yggr_test_assert(utf8_ch_ref_l >> utf8_ch_ref_r == 'a');
	}

	// utf8_char_ref >> utf8_char_view
	{
		yggr::utf8_char utf8_ch_l = 'a' * 2;
		yggr::utf8_char utf8_ch_r = 1;
		ref_type utf8_ch_ref_l = utf8_ch_l;
		ref_type utf8_ch_ref_r = utf8_ch_r;

		yggr_test_assert(yggr::utf8_char('a' * 2).view() >> utf8_ch_ref_r == 'a');
		yggr_test_assert(utf8_ch_ref_l >> yggr::utf8_char(1).view() == 'a');
	}


	// utf8_char_ref >> utf8_char_const_reference
	{
		yggr::utf8_char utf8_ch_l = 'a' * 2;
		yggr::utf8_char utf8_ch_r = 1;
		ref_type utf8_ch_ref_l = utf8_ch_l;
		ref_type utf8_ch_ref_r = utf8_ch_r;

		yggr_test_assert(utf8_ch_ref_l.const_cast_() >> utf8_ch_ref_r == 'a');
		yggr_test_assert(utf8_ch_ref_l >> utf8_ch_ref_r.const_cast_() == 'a');
	}

#ifndef YGGR_NO_CWCHAR
	// utf8_char_ref >> n
	{
		yggr::utf8_char utf8_ch = L'Äã' * 2;
		yggr_test_assert(utf8_ch >> 1 == L'Äã');
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((-utf8_ch_ref >> 1).is_validate());
		yggr_test_assert((-utf8_ch_ref >> 1) == (-utf8_ch_ref / 2));
	}

	// n >> utf8_char_ref
	{
		yggr::utf8_char utf8_ch = 1;
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert(yggr::charset::utf8_foo::E_char_max_range >> utf8_ch_ref
				== yggr::charset::utf8_foo::E_char_max_range / 2);

		yggr_test_assert(L'Äã' * 2 >> utf8_ch_ref == L'Äã');
	}

	// utf8_char_ref >> utf8_char_ref
	{
		yggr::utf8_char utf8_ch_l = L'Äã' * 2;
		yggr::utf8_char utf8_ch_r = 1;
		ref_type utf8_ch_ref_l = utf8_ch_l;
		ref_type utf8_ch_ref_r = utf8_ch_r;

		yggr_test_assert(ref_type(utf8_ch_ref_l) >> ref_type(utf8_ch_ref_r) == L'Äã');
		yggr_test_assert(ref_type(utf8_ch_ref_l) >> utf8_ch_ref_r == L'Äã');
		yggr_test_assert(utf8_ch_ref_l >> ref_type(utf8_ch_ref_r) == L'Äã');
		yggr_test_assert(utf8_ch_ref_l >> utf8_ch_ref_r == L'Äã');
	}

	// utf8_char_ref >> utf8_char_view
	{
		yggr::utf8_char utf8_ch_l = L'Äã' * 2;
		yggr::utf8_char utf8_ch_r = 1;
		ref_type utf8_ch_ref_l = utf8_ch_l;
		ref_type utf8_ch_ref_r = utf8_ch_r;

		yggr_test_assert(yggr::utf8_char(L'Äã' * 2).view() >> utf8_ch_ref_r == L'Äã');
		yggr_test_assert(utf8_ch_ref_l >> yggr::utf8_char(1).view() == L'Äã');
	}

	// utf8_char_ref >> utf8_char_const_reference
	{
		yggr::utf8_char utf8_ch_l = L'Äã' * 2;
		yggr::utf8_char utf8_ch_r = 1;
		ref_type utf8_ch_ref_l = utf8_ch_l;
		ref_type utf8_ch_ref_r = utf8_ch_r;

		yggr_test_assert(utf8_ch_ref_l.const_cast_() >> utf8_ch_ref_r == L'Äã');
		yggr_test_assert(utf8_ch_ref_l >> utf8_ch_ref_r.const_cast_() == L'Äã');
	}
#endif // YGGR_NO_CWCHAR

	std::cout << "test_bitwise_right_shift_of_utf8_char_reference succeed" << std::endl;
}

void test_bitwise_right_shift_of_utf8_char_const_reference(void)
{
	typedef yggr::utf8_char::char_const_reference_type ref_type;

	// utf8_char_ref >> n
	{
		yggr::utf8_char utf8_ch = 'a' * 2;
		yggr_test_assert(utf8_ch >> 1 == 'a');

		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((-utf8_ch_ref >> 1).is_validate());
		yggr_test_assert((-utf8_ch_ref >> 1) == (-utf8_ch_ref / 2));
	}

	// n >> utf8_char_ref
	{
		yggr::utf8_char utf8_ch = 1;
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert(yggr::charset::utf8_foo::E_char_max_range >> utf8_ch_ref
				== yggr::charset::utf8_foo::E_char_max_range / 2);

		yggr_test_assert('a' * 2 >> utf8_ch_ref == 'a');
	}

	// utf8_char_ref >> utf8_char_ref
	{
		yggr::utf8_char utf8_ch_l = 'a' * 2;
		yggr::utf8_char utf8_ch_r = 1;
		ref_type utf8_ch_ref_l = utf8_ch_l;
		ref_type utf8_ch_ref_r = utf8_ch_r;

		yggr_test_assert(ref_type(utf8_ch_ref_l) >> ref_type(utf8_ch_ref_r) == 'a');
		yggr_test_assert(ref_type(utf8_ch_ref_l) >> utf8_ch_ref_r == 'a');
		yggr_test_assert(utf8_ch_ref_l >> ref_type(utf8_ch_ref_r) == 'a');
		yggr_test_assert(utf8_ch_ref_l >> utf8_ch_ref_r == 'a');
	}

	// utf8_char_ref >> utf8_char_view
	{
		yggr::utf8_char utf8_ch_l = 'a' * 2;
		yggr::utf8_char utf8_ch_r = 1;
		ref_type utf8_ch_ref_l = utf8_ch_l;
		ref_type utf8_ch_ref_r = utf8_ch_r;

		yggr_test_assert(yggr::utf8_char('a' * 2).view() >> utf8_ch_ref_r == 'a');
		yggr_test_assert(utf8_ch_ref_l >> yggr::utf8_char(1).view() == 'a');
	}

#ifndef YGGR_NO_CWCHAR
	// utf8_char_ref >> n
	{
		yggr::utf8_char utf8_ch = L'Äã' * 2;
		yggr_test_assert(utf8_ch >> 1 == L'Äã');
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((-utf8_ch_ref >> 1).is_validate());
		yggr_test_assert((-utf8_ch_ref >> 1) == (-utf8_ch_ref / 2));
	}

	// n >> utf8_char_ref
	{
		yggr::utf8_char utf8_ch = 1;
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert(yggr::charset::utf8_foo::E_char_max_range >> utf8_ch_ref
				== yggr::charset::utf8_foo::E_char_max_range / 2);

		yggr_test_assert(L'Äã' * 2 >> utf8_ch_ref == L'Äã');
	}

	// utf8_char_ref >> utf8_char_ref
	{
		yggr::utf8_char utf8_ch_l = L'Äã' * 2;
		yggr::utf8_char utf8_ch_r = 1;
		ref_type utf8_ch_ref_l = utf8_ch_l;
		ref_type utf8_ch_ref_r = utf8_ch_r;

		yggr_test_assert(ref_type(utf8_ch_ref_l) >> ref_type(utf8_ch_ref_r) == L'Äã');
		yggr_test_assert(ref_type(utf8_ch_ref_l) >> utf8_ch_ref_r == L'Äã');
		yggr_test_assert(utf8_ch_ref_l >> ref_type(utf8_ch_ref_r) == L'Äã');
		yggr_test_assert(utf8_ch_ref_l >> utf8_ch_ref_r == L'Äã');
	}

	// utf8_char_ref >> utf8_char_view
	{
		yggr::utf8_char utf8_ch_l = L'Äã' * 2;
		yggr::utf8_char utf8_ch_r = 1;
		ref_type utf8_ch_ref_l = utf8_ch_l;
		ref_type utf8_ch_ref_r = utf8_ch_r;

		yggr_test_assert(yggr::utf8_char(L'Äã' * 2).view() >> utf8_ch_ref_r == L'Äã');
		yggr_test_assert(utf8_ch_ref_l >> yggr::utf8_char(1).view() == L'Äã');
	}

#endif // YGGR_NO_CWCHAR

	std::cout << "test_bitwise_right_shift_of_utf8_char_const_reference succeed" << std::endl;
}

void test_bitwise_right_shift_of_utf8_char_view(void)
{
	typedef yggr::utf8_char::char_const_reference_type ref_type;

	// utf8_char_ref >> n
	{
		yggr::utf8_char utf8_ch = 'a' * 2;
		yggr_test_assert(utf8_ch >> 1 == 'a');

		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((-utf8_ch_ref >> 1).is_validate());
		yggr_test_assert((-utf8_ch_ref >> 1) == (-utf8_ch_ref / 2));
	}

	// n >> utf8_char_ref
	{
		yggr::utf8_char utf8_ch = 1;
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert(yggr::charset::utf8_foo::E_char_max_range >> utf8_ch_ref
				== yggr::charset::utf8_foo::E_char_max_range / 2);

		yggr_test_assert('a' * 2 >> utf8_ch_ref == 'a');
	}

	// utf8_char_ref >> utf8_char_ref
	{
		yggr::utf8_char utf8_ch_l = 'a' * 2;
		yggr::utf8_char utf8_ch_r = 1;
		ref_type utf8_ch_ref_l = utf8_ch_l;
		ref_type utf8_ch_ref_r = utf8_ch_r;

		yggr_test_assert(ref_type(utf8_ch_ref_l) >> ref_type(utf8_ch_ref_r) == 'a');
		yggr_test_assert(ref_type(utf8_ch_ref_l) >> utf8_ch_ref_r == 'a');
		yggr_test_assert(utf8_ch_ref_l >> ref_type(utf8_ch_ref_r) == 'a');
		yggr_test_assert(utf8_ch_ref_l >> utf8_ch_ref_r == 'a');
	}

#ifndef YGGR_NO_CWCHAR
	// utf8_char_ref >> n
	{
		yggr::utf8_char utf8_ch = L'Äã' * 2;
		yggr_test_assert(utf8_ch >> 1 == L'Äã');
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((-utf8_ch_ref >> 1).is_validate());
		yggr_test_assert((-utf8_ch_ref >> 1) == (-utf8_ch_ref / 2));
	}

	// n >> utf8_char_ref
	{
		yggr::utf8_char utf8_ch = 1;
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert(yggr::charset::utf8_foo::E_char_max_range >> utf8_ch_ref
				== yggr::charset::utf8_foo::E_char_max_range / 2);

		yggr_test_assert(L'Äã' * 2 >> utf8_ch_ref == L'Äã');
	}

	// utf8_char_ref >> utf8_char_ref
	{
		yggr::utf8_char utf8_ch_l = L'Äã' * 2;
		yggr::utf8_char utf8_ch_r = 1;
		ref_type utf8_ch_ref_l = utf8_ch_l;
		ref_type utf8_ch_ref_r = utf8_ch_r;

		yggr_test_assert(ref_type(utf8_ch_ref_l) >> ref_type(utf8_ch_ref_r) == L'Äã');
		yggr_test_assert(ref_type(utf8_ch_ref_l) >> utf8_ch_ref_r == L'Äã');
		yggr_test_assert(utf8_ch_ref_l >> ref_type(utf8_ch_ref_r) == L'Äã');
		yggr_test_assert(utf8_ch_ref_l >> utf8_ch_ref_r == L'Äã');
	}

#endif // YGGR_NO_CWCHAR

	std::cout << "test_bitwise_right_shift_of_utf8_char_view succeed" << std::endl;
}

void test_bitwise_and_of_utf8_char(void)
{
	// utf8_char & n
	{
		yggr::utf8_char utf8_ch = 'a';
		yggr_test_assert((utf8_ch & 'a') == 'a');
		yggr_test_assert((utf8_ch & 'b') == ('a' & 'b'));
		yggr_test_assert((yggr::utf8_char('a') & 'a') == 'a');
		yggr_test_assert((yggr::utf8_char('a') & 'b') == ('a' & 'b'));
	}

	// n & utf8_char
	{
		yggr::utf8_char utf8_ch = 'a';
		yggr_test_assert(('a' & utf8_ch ) == 'a');
		yggr_test_assert(('b' & utf8_ch) == ('b' & 'a'));
		yggr_test_assert(('a' & yggr::utf8_char('a')) == 'a');
		yggr_test_assert(('b' & yggr::utf8_char('a')) == ('b' & 'a'));
	}

	// utf8_char & utf8_char
	{
		yggr::utf8_char utf8_ch_l = 'a';
		yggr::utf8_char utf8_ch_r = 'b';

		yggr_test_assert((utf8_ch_l & utf8_ch_l) == utf8_ch_l);
		yggr_test_assert((utf8_ch_l & utf8_ch_r) == ('a' & 'b'));

		yggr_test_assert((yggr::utf8_char('a') & utf8_ch_l) == 'a');
		yggr_test_assert((utf8_ch_l & yggr::utf8_char('b')) == ('a' & 'b'));

		yggr_test_assert((yggr::utf8_char('a') & yggr::utf8_char('b')) == ('a' & 'b'));
	}

	// utf8_char & utf8_char_view
	{
		yggr::utf8_char utf8_ch_l = 'a';
		yggr::utf8_char utf8_ch_r = 'b';

		yggr_test_assert((utf8_ch_l.view() & utf8_ch_l) == utf8_ch_l);
		yggr_test_assert((utf8_ch_l & utf8_ch_r.view()) == ('a' & 'b'));

		yggr_test_assert((yggr::utf8_char('a') & utf8_ch_l.view()) == 'a');
		yggr_test_assert((utf8_ch_l.view() & yggr::utf8_char('b')) == ('a' & 'b'));
	}

	// utf8_char & utf8_char_reference
	{
		yggr::utf8_char utf8_ch_l = 'a';
		yggr::utf8_char utf8_ch_r = 'b';

		yggr_test_assert((static_cast<yggr::utf8_char::char_reference_type>(utf8_ch_l) & utf8_ch_l) == utf8_ch_l);
		yggr_test_assert((utf8_ch_l & static_cast<yggr::utf8_char::char_reference_type>(utf8_ch_r)) == ('a' & 'b'));

		yggr_test_assert((yggr::utf8_char('a') & static_cast<yggr::utf8_char::char_reference_type>(utf8_ch_l)) == 'a');
		yggr_test_assert((static_cast<yggr::utf8_char::char_reference_type>(utf8_ch_l) & yggr::utf8_char('b')) == ('a' & 'b'));
	}

	// utf8_char & utf8_char_const_reference
	{
		yggr::utf8_char utf8_ch_l = 'a';
		yggr::utf8_char utf8_ch_r = 'b';

		yggr_test_assert((static_cast<yggr::utf8_char::char_const_reference_type>(utf8_ch_l) & utf8_ch_l) == utf8_ch_l);
		yggr_test_assert((utf8_ch_l & static_cast<yggr::utf8_char::char_const_reference_type>(utf8_ch_r)) == ('a' & 'b'));

		yggr_test_assert((yggr::utf8_char('a') & static_cast<yggr::utf8_char::char_const_reference_type>(utf8_ch_l)) == 'a');
		yggr_test_assert((static_cast<yggr::utf8_char::char_const_reference_type>(utf8_ch_l) & yggr::utf8_char('b')) == ('a' & 'b'));
	}

#ifndef YGGR_NO_CWCHAR

	// utf8_char & n
	{
		yggr::utf8_char utf8_ch = L'Äã';
		yggr_test_assert((utf8_ch & L'Äã') == L'Äã');
		yggr_test_assert((utf8_ch & L'ºÃ') == (L'Äã' & L'ºÃ'));
		yggr_test_assert((yggr::utf8_char(L'Äã') & L'Äã') == L'Äã');
		yggr_test_assert((yggr::utf8_char(L'Äã') & L'ºÃ') == (L'Äã' & L'ºÃ'));
	}

	// n & utf8_char
	{
		yggr::utf8_char utf8_ch = L'Äã';
		yggr_test_assert((L'Äã' & utf8_ch ) == L'Äã');
		yggr_test_assert((L'ºÃ' & utf8_ch) == (L'ºÃ' & L'Äã'));
		yggr_test_assert((L'Äã' & yggr::utf8_char(L'Äã')) == L'Äã');
		yggr_test_assert((L'ºÃ' & yggr::utf8_char(L'Äã')) == (L'ºÃ' & L'Äã'));
	}

	// utf8_char & utf8_char
	{
		yggr::utf8_char utf8_ch_l = L'Äã';
		yggr::utf8_char utf8_ch_r = L'ºÃ';

		yggr_test_assert((utf8_ch_l & utf8_ch_l) == utf8_ch_l);
		yggr_test_assert((utf8_ch_l & utf8_ch_r) == (L'Äã' & L'ºÃ'));

		yggr_test_assert((yggr::utf8_char(L'Äã') & utf8_ch_l) == L'Äã');
		yggr_test_assert((utf8_ch_l & yggr::utf8_char(L'ºÃ')) == (L'Äã' & L'ºÃ'));

		yggr_test_assert((yggr::utf8_char(L'Äã') & yggr::utf8_char(L'ºÃ')) == (L'Äã' & L'ºÃ'));
	}

	// utf8_char & utf8_char_view
	{
		yggr::utf8_char utf8_ch_l = L'Äã';
		yggr::utf8_char utf8_ch_r = L'ºÃ';

		yggr_test_assert((utf8_ch_l.view() & utf8_ch_l) == utf8_ch_l);
		yggr_test_assert((utf8_ch_l & utf8_ch_r.view()) == (L'Äã' & L'ºÃ'));

		yggr_test_assert((yggr::utf8_char(L'Äã') & utf8_ch_l.view()) == L'Äã');
		yggr_test_assert((utf8_ch_l.view() & yggr::utf8_char(L'ºÃ')) == (L'Äã' & L'ºÃ'));
	}

	// utf8_char & utf8_char_reference
	{
		yggr::utf8_char utf8_ch_l = L'Äã';
		yggr::utf8_char utf8_ch_r = L'ºÃ';

		yggr_test_assert((static_cast<yggr::utf8_char::char_reference_type>(utf8_ch_l) & utf8_ch_l) == utf8_ch_l);
		yggr_test_assert((utf8_ch_l & static_cast<yggr::utf8_char::char_reference_type>(utf8_ch_r)) == (L'Äã' & L'ºÃ'));

		yggr_test_assert((yggr::utf8_char(L'Äã') & static_cast<yggr::utf8_char::char_reference_type>(utf8_ch_l)) == L'Äã');
		yggr_test_assert((static_cast<yggr::utf8_char::char_reference_type>(utf8_ch_l) & yggr::utf8_char(L'ºÃ')) == (L'Äã' & L'ºÃ'));
	}

	// utf8_char & utf8_char_const_reference
	{
		yggr::utf8_char utf8_ch_l = L'Äã';
		yggr::utf8_char utf8_ch_r = L'ºÃ';

		yggr_test_assert((static_cast<yggr::utf8_char::char_const_reference_type>(utf8_ch_l) & utf8_ch_l) == utf8_ch_l);
		yggr_test_assert((utf8_ch_l & static_cast<yggr::utf8_char::char_const_reference_type>(utf8_ch_r)) == (L'Äã' & L'ºÃ'));

		yggr_test_assert((yggr::utf8_char(L'Äã') & static_cast<yggr::utf8_char::char_const_reference_type>(utf8_ch_l)) == L'Äã');
		yggr_test_assert((static_cast<yggr::utf8_char::char_const_reference_type>(utf8_ch_l) & yggr::utf8_char(L'ºÃ')) == (L'Äã' & L'ºÃ'));
	}

#endif // YGGR_NO_CWCHAR

	std::cout << "test_bitwise_and_of_utf8_char succeed" << std::endl;
}

void test_bitwise_and_of_utf8_char_reference(void)
{
	typedef yggr::utf8_char::char_reference_type ref_type;

	// utf8_char_ref & n
	{
		yggr::utf8_char utf8_ch = 'a';
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((utf8_ch_ref & 'a') == 'a');
		yggr_test_assert((utf8_ch_ref & 'b') == ('a' & 'b'));
		yggr_test_assert((ref_type(utf8_ch_ref) & 'a') == 'a');
		yggr_test_assert((ref_type(utf8_ch_ref) & 'b') == ('a' & 'b'));
	}

	// n & utf8_char_ref
	{
		yggr::utf8_char utf8_ch = 'a';
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert(('a' & utf8_ch_ref ) == 'a');
		yggr_test_assert(('b' & utf8_ch_ref) == ('b' & 'a'));
		yggr_test_assert(('a' & ref_type(utf8_ch_ref)) == 'a');
		yggr_test_assert(('b' & ref_type(utf8_ch_ref)) == ('b' & 'a'));
	}

	// utf8_char_ref & utf8_char_ref
	{
		yggr::utf8_char utf8_ch_l = 'a';
		yggr::utf8_char utf8_ch_r = 'b';
		ref_type utf8_ch_ref_l = utf8_ch_l;
		ref_type utf8_ch_ref_r = utf8_ch_r;

		yggr_test_assert((ref_type(utf8_ch_ref_l) & ref_type(utf8_ch_ref_l)) == utf8_ch_l);
		yggr_test_assert((ref_type(utf8_ch_ref_l) & utf8_ch_ref_l) == utf8_ch_l);
		yggr_test_assert((utf8_ch_ref_l & ref_type(utf8_ch_ref_l)) == utf8_ch_l);
		yggr_test_assert((utf8_ch_ref_l & utf8_ch_ref_l) == utf8_ch_l);

		yggr_test_assert((ref_type(utf8_ch_ref_l) & ref_type(utf8_ch_ref_r)) == ('a' & 'b'));
		yggr_test_assert((ref_type(utf8_ch_ref_l) & utf8_ch_ref_r) == ('a' & 'b'));
		yggr_test_assert((utf8_ch_ref_l & ref_type(utf8_ch_ref_r)) == ('a' & 'b'));
		yggr_test_assert((utf8_ch_ref_l & utf8_ch_ref_r) == ('a' & 'b'));
	}

	// utf8_char_ref & utf8_char_view
	{
		yggr::utf8_char utf8_ch_l = 'a';
		yggr::utf8_char utf8_ch_r = 'b';
		ref_type utf8_ch_ref_l = utf8_ch_l;
		ref_type utf8_ch_ref_r = utf8_ch_r;

		yggr_test_assert((utf8_ch_ref_l.view() & utf8_ch_ref_l) == utf8_ch_l);
		yggr_test_assert((utf8_ch_ref_l & utf8_ch_ref_r.view()) == ('a' & 'b'));

		yggr_test_assert((ref_type(utf8_ch_ref_l) & utf8_ch_ref_l.view()) == 'a');
		yggr_test_assert((utf8_ch_ref_l.view() & ref_type(utf8_ch_ref_r)) == ('a' & 'b'));
	}

	// utf8_char_ref & utf8_char_const_reference
	{
		yggr::utf8_char utf8_ch_l = 'a';
		yggr::utf8_char utf8_ch_r = 'b';
		ref_type utf8_ch_ref_l = utf8_ch_l;
		ref_type utf8_ch_ref_r = utf8_ch_r;

		yggr_test_assert((utf8_ch_ref_l.const_cast_() & utf8_ch_ref_l) == utf8_ch_l);
		yggr_test_assert((utf8_ch_ref_l & utf8_ch_ref_r.const_cast_()) == ('a' & 'b'));

		yggr_test_assert((ref_type(utf8_ch_ref_l) & utf8_ch_ref_l.const_cast_()) == 'a');
		yggr_test_assert((utf8_ch_ref_l.const_cast_() & utf8_ch_ref_r) == ('a' & 'b'));
	}

#ifndef YGGR_NO_CWCHAR

	// utf8_char_ref & n
	{
		yggr::utf8_char utf8_ch = L'Äã';
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((utf8_ch & L'Äã') == L'Äã');
		yggr_test_assert((utf8_ch & L'ºÃ') == (L'Äã' & L'ºÃ'));
		yggr_test_assert((yggr::utf8_char(L'Äã') & L'Äã') == L'Äã');
		yggr_test_assert((yggr::utf8_char(L'Äã') & L'ºÃ') == (L'Äã' & L'ºÃ'));
	}

	// n & utf8_char_ref
	{
		yggr::utf8_char utf8_ch = L'Äã';
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((L'Äã' & utf8_ch_ref ) == L'Äã');
		yggr_test_assert((L'ºÃ' & utf8_ch_ref) == (L'ºÃ' & L'Äã'));
		yggr_test_assert((L'Äã' & ref_type(utf8_ch_ref)) == L'Äã');
		yggr_test_assert((L'ºÃ' & ref_type(utf8_ch_ref)) == (L'ºÃ' & L'Äã'));
	}

	// utf8_char_ref & utf8_char_ref
	{
		yggr::utf8_char utf8_ch_l = L'Äã';
		yggr::utf8_char utf8_ch_r = L'ºÃ';
		ref_type utf8_ch_ref_l = utf8_ch_l;
		ref_type utf8_ch_ref_r = utf8_ch_r;

		yggr_test_assert((ref_type(utf8_ch_ref_l) & ref_type(utf8_ch_ref_l)) == utf8_ch_l);
		yggr_test_assert((ref_type(utf8_ch_ref_l) & utf8_ch_ref_l) == utf8_ch_l);
		yggr_test_assert((utf8_ch_ref_l & ref_type(utf8_ch_ref_l)) == utf8_ch_l);
		yggr_test_assert((utf8_ch_ref_l & utf8_ch_ref_l) == utf8_ch_l);

		yggr_test_assert((ref_type(utf8_ch_ref_l) & ref_type(utf8_ch_ref_r)) == (L'Äã' & L'ºÃ'));
		yggr_test_assert((ref_type(utf8_ch_ref_l) & utf8_ch_ref_r) == (L'Äã' & L'ºÃ'));
		yggr_test_assert((utf8_ch_ref_l & ref_type(utf8_ch_ref_r)) == (L'Äã' & L'ºÃ'));
		yggr_test_assert((utf8_ch_ref_l & utf8_ch_ref_r) == (L'Äã' & L'ºÃ'));
	}

	// utf8_char_ref & utf8_char_view
	{
		yggr::utf8_char utf8_ch_l = L'Äã';
		yggr::utf8_char utf8_ch_r = L'ºÃ';
		ref_type utf8_ch_ref_l = utf8_ch_l;
		ref_type utf8_ch_ref_r = utf8_ch_r;

		yggr_test_assert((utf8_ch_ref_l.view() & utf8_ch_ref_l) == utf8_ch_l);
		yggr_test_assert((utf8_ch_ref_l & utf8_ch_ref_r.view()) == (L'Äã' & L'ºÃ'));

		yggr_test_assert((ref_type(utf8_ch_ref_l) & utf8_ch_ref_l.view()) == L'Äã');
		yggr_test_assert((utf8_ch_ref_l.view() & ref_type(utf8_ch_ref_r)) == (L'Äã' & L'ºÃ'));
	}

	// utf8_char_ref & utf8_char_const_reference
	{
		yggr::utf8_char utf8_ch_l = L'Äã';
		yggr::utf8_char utf8_ch_r = L'ºÃ';
		ref_type utf8_ch_ref_l = utf8_ch_l;
		ref_type utf8_ch_ref_r = utf8_ch_r;

		yggr_test_assert((utf8_ch_ref_l.const_cast_() & utf8_ch_ref_l) == utf8_ch_l);
		yggr_test_assert((utf8_ch_ref_l & utf8_ch_ref_r.const_cast_()) == (L'Äã' & L'ºÃ'));

		yggr_test_assert((ref_type(utf8_ch_ref_l) & utf8_ch_ref_l.const_cast_()) == L'Äã');
		yggr_test_assert((utf8_ch_ref_l.const_cast_() & ref_type(utf8_ch_ref_r)) == (L'Äã' & L'ºÃ'));
	}

#endif // YGGR_NO_CWCHAR

	std::cout << "test_bitwise_and_of_utf8_char_reference succeed" << std::endl;
}

void test_bitwise_and_of_utf8_char_const_reference(void)
{
	typedef yggr::utf8_char::char_const_reference_type ref_type;

	// utf8_char_ref & n
	{
		yggr::utf8_char utf8_ch = 'a';
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((utf8_ch_ref & 'a') == 'a');
		yggr_test_assert((utf8_ch_ref & 'b') == ('a' & 'b'));
		yggr_test_assert((ref_type(utf8_ch_ref) & 'a') == 'a');
		yggr_test_assert((ref_type(utf8_ch_ref) & 'b') == ('a' & 'b'));
	}

	// n & utf8_char_ref
	{
		yggr::utf8_char utf8_ch = 'a';
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert(('a' & utf8_ch_ref ) == 'a');
		yggr_test_assert(('b' & utf8_ch_ref) == ('b' & 'a'));
		yggr_test_assert(('a' & ref_type(utf8_ch_ref)) == 'a');
		yggr_test_assert(('b' & ref_type(utf8_ch_ref)) == ('b' & 'a'));
	}

	// utf8_char_ref & utf8_char_ref
	{
		yggr::utf8_char utf8_ch_l = 'a';
		yggr::utf8_char utf8_ch_r = 'b';
		ref_type utf8_ch_ref_l = utf8_ch_l;
		ref_type utf8_ch_ref_r = utf8_ch_r;

		yggr_test_assert((ref_type(utf8_ch_ref_l) & ref_type(utf8_ch_ref_l)) == utf8_ch_l);
		yggr_test_assert((ref_type(utf8_ch_ref_l) & utf8_ch_ref_l) == utf8_ch_l);
		yggr_test_assert((utf8_ch_ref_l & ref_type(utf8_ch_ref_l)) == utf8_ch_l);
		yggr_test_assert((utf8_ch_ref_l & utf8_ch_ref_l) == utf8_ch_l);

		yggr_test_assert((ref_type(utf8_ch_ref_l) & ref_type(utf8_ch_ref_r)) == ('a' & 'b'));
		yggr_test_assert((ref_type(utf8_ch_ref_l) & utf8_ch_ref_r) == ('a' & 'b'));
		yggr_test_assert((utf8_ch_ref_l & ref_type(utf8_ch_ref_r)) == ('a' & 'b'));
		yggr_test_assert((utf8_ch_ref_l & utf8_ch_ref_r) == ('a' & 'b'));
	}

	// utf8_char_ref & utf8_char_view
	{
		yggr::utf8_char utf8_ch_l = 'a';
		yggr::utf8_char utf8_ch_r = 'b';
		ref_type utf8_ch_ref_l = utf8_ch_l;
		ref_type utf8_ch_ref_r = utf8_ch_r;

		yggr_test_assert((utf8_ch_ref_l.view() & utf8_ch_ref_l) == utf8_ch_l);
		yggr_test_assert((utf8_ch_ref_l & utf8_ch_ref_r.view()) == ('a' & 'b'));

		yggr_test_assert((ref_type(utf8_ch_ref_l) & utf8_ch_ref_l.view()) == 'a');
		yggr_test_assert((utf8_ch_ref_l.view() & ref_type(utf8_ch_ref_r)) == ('a' & 'b'));
	}

#ifndef YGGR_NO_CWCHAR

	// utf8_char_ref & n
	{
		yggr::utf8_char utf8_ch = L'Äã';
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((utf8_ch & L'Äã') == L'Äã');
		yggr_test_assert((utf8_ch & L'ºÃ') == (L'Äã' & L'ºÃ'));
		yggr_test_assert((yggr::utf8_char(L'Äã') & L'Äã') == L'Äã');
		yggr_test_assert((yggr::utf8_char(L'Äã') & L'ºÃ') == (L'Äã' & L'ºÃ'));
	}

	// n & utf8_char_ref
	{
		yggr::utf8_char utf8_ch = L'Äã';
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((L'Äã' & utf8_ch_ref ) == L'Äã');
		yggr_test_assert((L'ºÃ' & utf8_ch_ref) == (L'ºÃ' & L'Äã'));
		yggr_test_assert((L'Äã' & ref_type(utf8_ch_ref)) == L'Äã');
		yggr_test_assert((L'ºÃ' & ref_type(utf8_ch_ref)) == (L'ºÃ' & L'Äã'));
	}

	// utf8_char_ref & utf8_char_ref
	{
		yggr::utf8_char utf8_ch_l = L'Äã';
		yggr::utf8_char utf8_ch_r = L'ºÃ';
		ref_type utf8_ch_ref_l = utf8_ch_l;
		ref_type utf8_ch_ref_r = utf8_ch_r;

		yggr_test_assert((ref_type(utf8_ch_ref_l) & ref_type(utf8_ch_ref_l)) == utf8_ch_l);
		yggr_test_assert((ref_type(utf8_ch_ref_l) & utf8_ch_ref_l) == utf8_ch_l);
		yggr_test_assert((utf8_ch_ref_l & ref_type(utf8_ch_ref_l)) == utf8_ch_l);
		yggr_test_assert((utf8_ch_ref_l & utf8_ch_ref_l) == utf8_ch_l);

		yggr_test_assert((ref_type(utf8_ch_ref_l) & ref_type(utf8_ch_ref_r)) == (L'Äã' & L'ºÃ'));
		yggr_test_assert((ref_type(utf8_ch_ref_l) & utf8_ch_ref_r) == (L'Äã' & L'ºÃ'));
		yggr_test_assert((utf8_ch_ref_l & ref_type(utf8_ch_ref_r)) == (L'Äã' & L'ºÃ'));
		yggr_test_assert((utf8_ch_ref_l & utf8_ch_ref_r) == (L'Äã' & L'ºÃ'));
	}

	// utf8_char_ref & utf8_char_view
	{
		yggr::utf8_char utf8_ch_l = L'Äã';
		yggr::utf8_char utf8_ch_r = L'ºÃ';
		ref_type utf8_ch_ref_l = utf8_ch_l;
		ref_type utf8_ch_ref_r = utf8_ch_r;

		yggr_test_assert((utf8_ch_ref_l.view() & utf8_ch_ref_l) == utf8_ch_l);
		yggr_test_assert((utf8_ch_ref_l & utf8_ch_ref_r.view()) == (L'Äã' & L'ºÃ'));

		yggr_test_assert((ref_type(utf8_ch_ref_l) & utf8_ch_ref_l.view()) == L'Äã');
		yggr_test_assert((utf8_ch_ref_l.view() & ref_type(utf8_ch_ref_r)) == (L'Äã' & L'ºÃ'));
	}

#endif // YGGR_NO_CWCHAR

	std::cout << "test_bitwise_and_of_utf8_char_const_reference succeed" << std::endl;
}

void test_bitwise_and_of_utf8_char_view(void)
{
	typedef yggr::utf8_char::char_view_type ref_type;

	// utf8_char_ref & n
	{
		yggr::utf8_char utf8_ch = 'a';
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((utf8_ch_ref & 'a') == 'a');
		yggr_test_assert((utf8_ch_ref & 'b') == ('a' & 'b'));
		yggr_test_assert((ref_type(utf8_ch_ref) & 'a') == 'a');
		yggr_test_assert((ref_type(utf8_ch_ref) & 'b') == ('a' & 'b'));
	}

	// n & utf8_char_ref
	{
		yggr::utf8_char utf8_ch = 'a';
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert(('a' & utf8_ch_ref ) == 'a');
		yggr_test_assert(('b' & utf8_ch_ref) == ('b' & 'a'));
		yggr_test_assert(('a' & ref_type(utf8_ch_ref)) == 'a');
		yggr_test_assert(('b' & ref_type(utf8_ch_ref)) == ('b' & 'a'));
	}

	// utf8_char_ref & utf8_char_ref
	{
		yggr::utf8_char utf8_ch_l = 'a';
		yggr::utf8_char utf8_ch_r = 'b';
		ref_type utf8_ch_ref_l = utf8_ch_l;
		ref_type utf8_ch_ref_r = utf8_ch_r;

		yggr_test_assert((ref_type(utf8_ch_ref_l) & ref_type(utf8_ch_ref_l)) == utf8_ch_l);
		yggr_test_assert((ref_type(utf8_ch_ref_l) & utf8_ch_ref_l) == utf8_ch_l);
		yggr_test_assert((utf8_ch_ref_l & ref_type(utf8_ch_ref_l)) == utf8_ch_l);
		yggr_test_assert((utf8_ch_ref_l & utf8_ch_ref_l) == utf8_ch_l);

		yggr_test_assert((ref_type(utf8_ch_ref_l) & ref_type(utf8_ch_ref_r)) == ('a' & 'b'));
		yggr_test_assert((ref_type(utf8_ch_ref_l) & utf8_ch_ref_r) == ('a' & 'b'));
		yggr_test_assert((utf8_ch_ref_l & ref_type(utf8_ch_ref_r)) == ('a' & 'b'));
		yggr_test_assert((utf8_ch_ref_l & utf8_ch_ref_r) == ('a' & 'b'));
	}

#ifndef YGGR_NO_CWCHAR

	// utf8_char_ref & n
	{
		yggr::utf8_char utf8_ch = L'Äã';
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((utf8_ch & L'Äã') == L'Äã');
		yggr_test_assert((utf8_ch & L'ºÃ') == (L'Äã' & L'ºÃ'));
		yggr_test_assert((yggr::utf8_char(L'Äã') & L'Äã') == L'Äã');
		yggr_test_assert((yggr::utf8_char(L'Äã') & L'ºÃ') == (L'Äã' & L'ºÃ'));
	}

	// n & utf8_char_ref
	{
		yggr::utf8_char utf8_ch = L'Äã';
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((L'Äã' & utf8_ch_ref ) == L'Äã');
		yggr_test_assert((L'ºÃ' & utf8_ch_ref) == (L'ºÃ' & L'Äã'));
		yggr_test_assert((L'Äã' & ref_type(utf8_ch_ref)) == L'Äã');
		yggr_test_assert((L'ºÃ' & ref_type(utf8_ch_ref)) == (L'ºÃ' & L'Äã'));
	}

	// utf8_char_ref & utf8_char_ref
	{
		yggr::utf8_char utf8_ch_l = L'Äã';
		yggr::utf8_char utf8_ch_r = L'ºÃ';
		ref_type utf8_ch_ref_l = utf8_ch_l;
		ref_type utf8_ch_ref_r = utf8_ch_r;

		yggr_test_assert((ref_type(utf8_ch_ref_l) & ref_type(utf8_ch_ref_l)) == utf8_ch_l);
		yggr_test_assert((ref_type(utf8_ch_ref_l) & utf8_ch_ref_l) == utf8_ch_l);
		yggr_test_assert((utf8_ch_ref_l & ref_type(utf8_ch_ref_l)) == utf8_ch_l);
		yggr_test_assert((utf8_ch_ref_l & utf8_ch_ref_l) == utf8_ch_l);

		yggr_test_assert((ref_type(utf8_ch_ref_l) & ref_type(utf8_ch_ref_r)) == (L'Äã' & L'ºÃ'));
		yggr_test_assert((ref_type(utf8_ch_ref_l) & utf8_ch_ref_r) == (L'Äã' & L'ºÃ'));
		yggr_test_assert((utf8_ch_ref_l & ref_type(utf8_ch_ref_r)) == (L'Äã' & L'ºÃ'));
		yggr_test_assert((utf8_ch_ref_l & utf8_ch_ref_r) == (L'Äã' & L'ºÃ'));
	}

#endif // YGGR_NO_CWCHAR

	std::cout << "test_bitwise_and_of_utf8_char_view succeed" << std::endl;
}

void test_bitwise_or_utf8_char(void)
{
	// utf8_char | n
	{
		yggr::utf8_char utf8_ch = 'a';
		yggr_test_assert((utf8_ch | 'a') == 'a');
		yggr_test_assert((utf8_ch | 'b') == ('a' | 'b'));
		yggr_test_assert((yggr::utf8_char('a') | 'a') == 'a');
		yggr_test_assert((yggr::utf8_char('a') | 'b') == ('a' | 'b'));
	}

	// n | utf8_char
	{
		yggr::utf8_char utf8_ch = 'a';
		yggr_test_assert(('a' | utf8_ch ) == 'a');
		yggr_test_assert(('b' | utf8_ch) == ('b' | 'a'));
		yggr_test_assert(('a' | yggr::utf8_char('a')) == 'a');
		yggr_test_assert(('b' | yggr::utf8_char('a')) == ('b' | 'a'));
	}

	// utf8_char | utf8_char
	{
		yggr::utf8_char utf8_ch_l = 'a';
		yggr::utf8_char utf8_ch_r = 'b';

		yggr_test_assert((utf8_ch_l | utf8_ch_l) == utf8_ch_l);
		yggr_test_assert((utf8_ch_l | utf8_ch_r) == ('a' | 'b'));

		yggr_test_assert((yggr::utf8_char('a') | utf8_ch_l) == 'a');
		yggr_test_assert((utf8_ch_l | yggr::utf8_char('b')) == ('a' | 'b'));

		yggr_test_assert((yggr::utf8_char('a') | yggr::utf8_char('b')) == ('a' | 'b'));
	}

	// utf8_char | utf8_char_view
	{
		yggr::utf8_char utf8_ch_l = 'a';
		yggr::utf8_char utf8_ch_r = 'b';

		yggr_test_assert((utf8_ch_l.view() | utf8_ch_l) == utf8_ch_l);
		yggr_test_assert((utf8_ch_l | utf8_ch_r.view()) == ('a' | 'b'));

		yggr_test_assert((yggr::utf8_char('a') | utf8_ch_l.view()) == 'a');
		yggr_test_assert((utf8_ch_l.view() | yggr::utf8_char('b')) == ('a' | 'b'));
	}

	// utf8_char | utf8_char_reference
	{
		yggr::utf8_char utf8_ch_l = 'a';
		yggr::utf8_char utf8_ch_r = 'b';

		yggr_test_assert((static_cast<yggr::utf8_char::char_reference_type>(utf8_ch_l) | utf8_ch_l) == utf8_ch_l);
		yggr_test_assert((utf8_ch_l | static_cast<yggr::utf8_char::char_reference_type>(utf8_ch_r)) == ('a' | 'b'));

		yggr_test_assert((yggr::utf8_char('a') | static_cast<yggr::utf8_char::char_reference_type>(utf8_ch_l)) == 'a');
		yggr_test_assert((static_cast<yggr::utf8_char::char_reference_type>(utf8_ch_l) | yggr::utf8_char('b')) == ('a' | 'b'));
	}

	// utf8_char | utf8_char_const_reference
	{
		yggr::utf8_char utf8_ch_l = 'a';
		yggr::utf8_char utf8_ch_r = 'b';

		yggr_test_assert((static_cast<yggr::utf8_char::char_const_reference_type>(utf8_ch_l) | utf8_ch_l) == utf8_ch_l);
		yggr_test_assert((utf8_ch_l | static_cast<yggr::utf8_char::char_const_reference_type>(utf8_ch_r)) == ('a' | 'b'));

		yggr_test_assert((yggr::utf8_char('a') | static_cast<yggr::utf8_char::char_const_reference_type>(utf8_ch_l)) == 'a');
		yggr_test_assert((static_cast<yggr::utf8_char::char_const_reference_type>(utf8_ch_l) | yggr::utf8_char('b')) == ('a' | 'b'));
	}

#ifndef YGGR_NO_CWCHAR

	// utf8_char | n
	{
		yggr::utf8_char utf8_ch = L'Äã';
		yggr_test_assert((utf8_ch | L'Äã') == L'Äã');
		yggr_test_assert((utf8_ch | L'ºÃ') == (L'Äã' | L'ºÃ'));
		yggr_test_assert((yggr::utf8_char(L'Äã') | L'Äã') == L'Äã');
		yggr_test_assert((yggr::utf8_char(L'Äã') | L'ºÃ') == (L'Äã' | L'ºÃ'));
	}

	// n | utf8_char
	{
		yggr::utf8_char utf8_ch = L'Äã';
		yggr_test_assert((L'Äã' | utf8_ch ) == L'Äã');
		yggr_test_assert((L'ºÃ' | utf8_ch) == (L'ºÃ' | L'Äã'));
		yggr_test_assert((L'Äã' | yggr::utf8_char(L'Äã')) == L'Äã');
		yggr_test_assert((L'ºÃ' | yggr::utf8_char(L'Äã')) == (L'ºÃ' | L'Äã'));
	}

	// utf8_char | utf8_char
	{
		yggr::utf8_char utf8_ch_l = L'Äã';
		yggr::utf8_char utf8_ch_r = L'ºÃ';

		yggr_test_assert((utf8_ch_l | utf8_ch_l) == utf8_ch_l);
		yggr_test_assert((utf8_ch_l | utf8_ch_r) == (L'Äã' | L'ºÃ'));

		yggr_test_assert((yggr::utf8_char(L'Äã') | utf8_ch_l) == L'Äã');
		yggr_test_assert((utf8_ch_l | yggr::utf8_char(L'ºÃ')) == (L'Äã' | L'ºÃ'));

		yggr_test_assert((yggr::utf8_char(L'Äã') | yggr::utf8_char(L'ºÃ')) == (L'Äã' | L'ºÃ'));
	}

	// utf8_char | utf8_char_view
	{
		yggr::utf8_char utf8_ch_l = L'Äã';
		yggr::utf8_char utf8_ch_r = L'ºÃ';

		yggr_test_assert((utf8_ch_l.view() | utf8_ch_l) == utf8_ch_l);
		yggr_test_assert((utf8_ch_l | utf8_ch_r.view()) == (L'Äã' | L'ºÃ'));

		yggr_test_assert((yggr::utf8_char(L'Äã') | utf8_ch_l.view()) == L'Äã');
		yggr_test_assert((utf8_ch_l.view() | yggr::utf8_char(L'ºÃ')) == (L'Äã' | L'ºÃ'));
	}

	// utf8_char | utf8_char_reference
	{
		yggr::utf8_char utf8_ch_l = L'Äã';
		yggr::utf8_char utf8_ch_r = L'ºÃ';

		yggr_test_assert((static_cast<yggr::utf8_char::char_reference_type>(utf8_ch_l) | utf8_ch_l) == utf8_ch_l);
		yggr_test_assert((utf8_ch_l | static_cast<yggr::utf8_char::char_reference_type>(utf8_ch_r)) == (L'Äã' | L'ºÃ'));

		yggr_test_assert((yggr::utf8_char(L'Äã') | static_cast<yggr::utf8_char::char_reference_type>(utf8_ch_l)) == L'Äã');
		yggr_test_assert((static_cast<yggr::utf8_char::char_reference_type>(utf8_ch_l) | yggr::utf8_char(L'ºÃ')) == (L'Äã' | L'ºÃ'));
	}

	// utf8_char | utf8_char_const_reference
	{
		yggr::utf8_char utf8_ch_l = L'Äã';
		yggr::utf8_char utf8_ch_r = L'ºÃ';

		yggr_test_assert((static_cast<yggr::utf8_char::char_const_reference_type>(utf8_ch_l) | utf8_ch_l) == utf8_ch_l);
		yggr_test_assert((utf8_ch_l | static_cast<yggr::utf8_char::char_const_reference_type>(utf8_ch_r)) == (L'Äã' | L'ºÃ'));

		yggr_test_assert((yggr::utf8_char(L'Äã') | static_cast<yggr::utf8_char::char_const_reference_type>(utf8_ch_l)) == L'Äã');
		yggr_test_assert((static_cast<yggr::utf8_char::char_const_reference_type>(utf8_ch_l) | yggr::utf8_char(L'ºÃ')) == (L'Äã' | L'ºÃ'));
	}

#endif // YGGR_NO_CWCHAR

	std::cout << "test_bitwise_or_utf8_char succeed" << std::endl;
}

void test_bitwise_or_utf8_char_reference(void)
{
	typedef yggr::utf8_char::char_reference_type ref_type;

	// utf8_char_ref | n
	{
		yggr::utf8_char utf8_ch = 'a';
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((utf8_ch_ref | 'a') == 'a');
		yggr_test_assert((utf8_ch_ref | 'b') == ('a' | 'b'));
		yggr_test_assert((ref_type(utf8_ch_ref) | 'a') == 'a');
		yggr_test_assert((ref_type(utf8_ch_ref) | 'b') == ('a' | 'b'));
	}

	// n | utf8_char_ref
	{
		yggr::utf8_char utf8_ch = 'a';
		ref_type utf8_ch_erf = utf8_ch;

		yggr_test_assert(('a' | utf8_ch ) == 'a');
		yggr_test_assert(('b' | utf8_ch) == ('b' | 'a'));
		yggr_test_assert(('a' | yggr::utf8_char('a')) == 'a');
		yggr_test_assert(('b' | yggr::utf8_char('a')) == ('b' | 'a'));
	}

	// utf8_char_ref | utf8_char_ref
	{
		yggr::utf8_char utf8_ch_l = 'a';
		yggr::utf8_char utf8_ch_r = 'b';
		ref_type utf8_ch_ref_l = utf8_ch_l;
		ref_type utf8_ch_ref_r = utf8_ch_r;

		yggr_test_assert((ref_type(utf8_ch_ref_l) | ref_type(utf8_ch_ref_l)) == utf8_ch_l);
		yggr_test_assert((ref_type(utf8_ch_ref_l) | utf8_ch_ref_l) == utf8_ch_l);
		yggr_test_assert((utf8_ch_ref_l | ref_type(utf8_ch_ref_l)) == utf8_ch_l);
		yggr_test_assert((utf8_ch_ref_l | utf8_ch_ref_l) == utf8_ch_l);

		yggr_test_assert((ref_type(utf8_ch_ref_l) | ref_type(utf8_ch_ref_r)) == ('a' | 'b'));
		yggr_test_assert((ref_type(utf8_ch_ref_l) | utf8_ch_ref_r) == ('a' | 'b'));
		yggr_test_assert((utf8_ch_ref_l | ref_type(utf8_ch_ref_r)) == ('a' | 'b'));
		yggr_test_assert((utf8_ch_ref_l | utf8_ch_ref_r) == ('a' | 'b'));
	}

	// utf8_char_ref | utf8_char_view
	{
		yggr::utf8_char utf8_ch_l = 'a';
		yggr::utf8_char utf8_ch_r = 'b';
		ref_type utf8_ch_ref_l = utf8_ch_l;
		ref_type utf8_ch_ref_r = utf8_ch_r;

		yggr_test_assert((utf8_ch_ref_l.view() | utf8_ch_ref_l) == utf8_ch_l);
		yggr_test_assert((utf8_ch_ref_l | utf8_ch_ref_r.view()) == ('a' | 'b'));

		yggr_test_assert((ref_type(utf8_ch_ref_l) | utf8_ch_ref_l.view()) == 'a');
		yggr_test_assert((utf8_ch_ref_l.view() | ref_type(utf8_ch_ref_r)) == ('a' | 'b'));
	}

	// utf8_char_ref | utf8_char_const_reference
	{
		yggr::utf8_char utf8_ch_l = 'a';
		yggr::utf8_char utf8_ch_r = 'b';
		ref_type utf8_ch_ref_l = utf8_ch_l;
		ref_type utf8_ch_ref_r = utf8_ch_r;

		yggr_test_assert((utf8_ch_ref_l.const_cast_() | utf8_ch_ref_l) == utf8_ch_l);
		yggr_test_assert((utf8_ch_ref_l | utf8_ch_ref_r.const_cast_()) == ('a' | 'b'));

		yggr_test_assert((ref_type(utf8_ch_ref_l) | utf8_ch_ref_l.const_cast_()) == 'a');
		yggr_test_assert((utf8_ch_ref_l.const_cast_() | ref_type(utf8_ch_ref_r)) == ('a' | 'b'));
	}

#ifndef YGGR_NO_CWCHAR

	// utf8_char_ref | n
	{
		yggr::utf8_char utf8_ch = L'Äã';
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((utf8_ch_ref | L'Äã') == L'Äã');
		yggr_test_assert((utf8_ch_ref | L'ºÃ') == (L'Äã' | L'ºÃ'));
		yggr_test_assert((ref_type(utf8_ch_ref) | L'Äã') == L'Äã');
		yggr_test_assert((ref_type(utf8_ch_ref) | L'ºÃ') == (L'Äã' | L'ºÃ'));
	}

	// n | utf8_char_ref
	{
		yggr::utf8_char utf8_ch = L'Äã';
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((L'Äã' | utf8_ch_ref ) == L'Äã');
		yggr_test_assert((L'ºÃ' | utf8_ch_ref) == (L'ºÃ' | L'Äã'));
		yggr_test_assert((L'Äã' | ref_type(utf8_ch_ref)) == L'Äã');
		yggr_test_assert((L'ºÃ' | ref_type(utf8_ch_ref)) == (L'ºÃ' | L'Äã'));
	}

	// utf8_char_ref | utf8_char_ref
	{
		yggr::utf8_char utf8_ch_l = L'Äã';
		yggr::utf8_char utf8_ch_r = L'ºÃ';
		ref_type utf8_ch_ref_l = utf8_ch_l;
		ref_type utf8_ch_ref_r = utf8_ch_r;

		yggr_test_assert((ref_type(utf8_ch_ref_l) | ref_type(utf8_ch_ref_l)) == utf8_ch_l);
		yggr_test_assert((ref_type(utf8_ch_ref_l) | utf8_ch_ref_l) == utf8_ch_l);
		yggr_test_assert((utf8_ch_ref_l | ref_type(utf8_ch_ref_l)) == utf8_ch_l);
		yggr_test_assert((utf8_ch_ref_l | utf8_ch_ref_l) == utf8_ch_l);

		yggr_test_assert((ref_type(utf8_ch_ref_l) | ref_type(utf8_ch_ref_r)) == (L'Äã' | L'ºÃ'));
		yggr_test_assert((ref_type(utf8_ch_ref_l) | utf8_ch_ref_r) == (L'Äã' | L'ºÃ'));
		yggr_test_assert((utf8_ch_ref_l | ref_type(utf8_ch_ref_r)) == (L'Äã' | L'ºÃ'));
		yggr_test_assert((utf8_ch_ref_l | utf8_ch_ref_r) == (L'Äã' | L'ºÃ'));
	}

	// utf8_char_ref | utf8_char_view
	{
		yggr::utf8_char utf8_ch_l = L'Äã';
		yggr::utf8_char utf8_ch_r = L'ºÃ';
		ref_type utf8_ch_ref_l = utf8_ch_l;
		ref_type utf8_ch_ref_r = utf8_ch_r;

		yggr_test_assert((utf8_ch_ref_l.view() | utf8_ch_ref_l) == utf8_ch_l);
		yggr_test_assert((utf8_ch_ref_l | utf8_ch_ref_r.view()) == (L'Äã' | L'ºÃ'));

		yggr_test_assert((ref_type(utf8_ch_ref_l) | utf8_ch_ref_l.view()) == L'Äã');
		yggr_test_assert((utf8_ch_ref_l.view() | ref_type(utf8_ch_ref_r)) == (L'Äã' | L'ºÃ'));
	}

	// utf8_char_ref | utf8_char_const_reference
	{
		yggr::utf8_char utf8_ch_l = L'Äã';
		yggr::utf8_char utf8_ch_r = L'ºÃ';
		ref_type utf8_ch_ref_l = utf8_ch_l;
		ref_type utf8_ch_ref_r = utf8_ch_r;

		yggr_test_assert((utf8_ch_ref_l.const_cast_() | utf8_ch_ref_l) == utf8_ch_l);
		yggr_test_assert((utf8_ch_ref_l | utf8_ch_ref_r.const_cast_()) == (L'Äã' | L'ºÃ'));

		yggr_test_assert((ref_type(utf8_ch_ref_l) | utf8_ch_ref_l) == L'Äã');
		yggr_test_assert((utf8_ch_ref_l.const_cast_() | ref_type(utf8_ch_ref_r)) == (L'Äã' | L'ºÃ'));
	}

#endif // YGGR_NO_CWCHAR

	std::cout << "test_bitwise_or_utf8_char_reference succeed" << std::endl;
}

void test_bitwise_or_utf8_char_const_reference(void)
{
	typedef yggr::utf8_char::char_const_reference_type ref_type;

	// utf8_char_ref | n
	{
		yggr::utf8_char utf8_ch = 'a';
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((utf8_ch_ref | 'a') == 'a');
		yggr_test_assert((utf8_ch_ref | 'b') == ('a' | 'b'));
		yggr_test_assert((ref_type(utf8_ch_ref) | 'a') == 'a');
		yggr_test_assert((ref_type(utf8_ch_ref) | 'b') == ('a' | 'b'));
	}

	// n | utf8_char_ref
	{
		yggr::utf8_char utf8_ch = 'a';
		ref_type utf8_ch_erf = utf8_ch;

		yggr_test_assert(('a' | utf8_ch ) == 'a');
		yggr_test_assert(('b' | utf8_ch) == ('b' | 'a'));
		yggr_test_assert(('a' | yggr::utf8_char('a')) == 'a');
		yggr_test_assert(('b' | yggr::utf8_char('a')) == ('b' | 'a'));
	}

	// utf8_char_ref | utf8_char_ref
	{
		yggr::utf8_char utf8_ch_l = 'a';
		yggr::utf8_char utf8_ch_r = 'b';
		ref_type utf8_ch_ref_l = utf8_ch_l;
		ref_type utf8_ch_ref_r = utf8_ch_r;

		yggr_test_assert((ref_type(utf8_ch_ref_l) | ref_type(utf8_ch_ref_l)) == utf8_ch_l);
		yggr_test_assert((ref_type(utf8_ch_ref_l) | utf8_ch_ref_l) == utf8_ch_l);
		yggr_test_assert((utf8_ch_ref_l | ref_type(utf8_ch_ref_l)) == utf8_ch_l);
		yggr_test_assert((utf8_ch_ref_l | utf8_ch_ref_l) == utf8_ch_l);

		yggr_test_assert((ref_type(utf8_ch_ref_l) | ref_type(utf8_ch_ref_r)) == ('a' | 'b'));
		yggr_test_assert((ref_type(utf8_ch_ref_l) | utf8_ch_ref_r) == ('a' | 'b'));
		yggr_test_assert((utf8_ch_ref_l | ref_type(utf8_ch_ref_r)) == ('a' | 'b'));
		yggr_test_assert((utf8_ch_ref_l | utf8_ch_ref_r) == ('a' | 'b'));
	}

	// utf8_char_ref | utf8_char_view
	{
		yggr::utf8_char utf8_ch_l = 'a';
		yggr::utf8_char utf8_ch_r = 'b';
		ref_type utf8_ch_ref_l = utf8_ch_l;
		ref_type utf8_ch_ref_r = utf8_ch_r;

		yggr_test_assert((utf8_ch_ref_l.view() | utf8_ch_ref_l) == utf8_ch_l);
		yggr_test_assert((utf8_ch_ref_l | utf8_ch_ref_r.view()) == ('a' | 'b'));

		yggr_test_assert((ref_type(utf8_ch_ref_l) | utf8_ch_ref_l.view()) == 'a');
		yggr_test_assert((utf8_ch_ref_l.view() | ref_type(utf8_ch_ref_r)) == ('a' | 'b'));
	}

#ifndef YGGR_NO_CWCHAR

	// utf8_char_ref | n
	{
		yggr::utf8_char utf8_ch = L'Äã';
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((utf8_ch_ref | L'Äã') == L'Äã');
		yggr_test_assert((utf8_ch_ref | L'ºÃ') == (L'Äã' | L'ºÃ'));
		yggr_test_assert((ref_type(utf8_ch_ref) | L'Äã') == L'Äã');
		yggr_test_assert((ref_type(utf8_ch_ref) | L'ºÃ') == (L'Äã' | L'ºÃ'));
	}

	// n | utf8_char_ref
	{
		yggr::utf8_char utf8_ch = L'Äã';
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((L'Äã' | utf8_ch_ref ) == L'Äã');
		yggr_test_assert((L'ºÃ' | utf8_ch_ref) == (L'ºÃ' | L'Äã'));
		yggr_test_assert((L'Äã' | ref_type(utf8_ch_ref)) == L'Äã');
		yggr_test_assert((L'ºÃ' | ref_type(utf8_ch_ref)) == (L'ºÃ' | L'Äã'));
	}

	// utf8_char_ref | utf8_char_ref
	{
		yggr::utf8_char utf8_ch_l = L'Äã';
		yggr::utf8_char utf8_ch_r = L'ºÃ';
		ref_type utf8_ch_ref_l = utf8_ch_l;
		ref_type utf8_ch_ref_r = utf8_ch_r;

		yggr_test_assert((ref_type(utf8_ch_ref_l) | ref_type(utf8_ch_ref_l)) == utf8_ch_l);
		yggr_test_assert((ref_type(utf8_ch_ref_l) | utf8_ch_ref_l) == utf8_ch_l);
		yggr_test_assert((utf8_ch_ref_l | ref_type(utf8_ch_ref_l)) == utf8_ch_l);
		yggr_test_assert((utf8_ch_ref_l | utf8_ch_ref_l) == utf8_ch_l);

		yggr_test_assert((ref_type(utf8_ch_ref_l) | ref_type(utf8_ch_ref_r)) == (L'Äã' | L'ºÃ'));
		yggr_test_assert((ref_type(utf8_ch_ref_l) | utf8_ch_ref_r) == (L'Äã' | L'ºÃ'));
		yggr_test_assert((utf8_ch_ref_l | ref_type(utf8_ch_ref_r)) == (L'Äã' | L'ºÃ'));
		yggr_test_assert((utf8_ch_ref_l | utf8_ch_ref_r) == (L'Äã' | L'ºÃ'));
	}

	// utf8_char_ref | utf8_char_view
	{
		yggr::utf8_char utf8_ch_l = L'Äã';
		yggr::utf8_char utf8_ch_r = L'ºÃ';
		ref_type utf8_ch_ref_l = utf8_ch_l;
		ref_type utf8_ch_ref_r = utf8_ch_r;

		yggr_test_assert((utf8_ch_ref_l.view() | utf8_ch_ref_l) == utf8_ch_l);
		yggr_test_assert((utf8_ch_ref_l | utf8_ch_ref_r.view()) == (L'Äã' | L'ºÃ'));

		yggr_test_assert((ref_type(utf8_ch_ref_l) | utf8_ch_ref_l.view()) == L'Äã');
		yggr_test_assert((utf8_ch_ref_l.view() | ref_type(utf8_ch_ref_r)) == (L'Äã' | L'ºÃ'));
	}

#endif // YGGR_NO_CWCHAR

	std::cout << "test_bitwise_or_utf8_char_const_reference succeed" << std::endl;
}

void test_bitwise_or_utf8_char_view(void)
{
	typedef yggr::utf8_char::char_view_type ref_type;

	// utf8_char_ref | n
	{
		yggr::utf8_char utf8_ch = 'a';
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((utf8_ch_ref | 'a') == 'a');
		yggr_test_assert((utf8_ch_ref | 'b') == ('a' | 'b'));
		yggr_test_assert((ref_type(utf8_ch_ref) | 'a') == 'a');
		yggr_test_assert((ref_type(utf8_ch_ref) | 'b') == ('a' | 'b'));
	}

	// n | utf8_char_ref
	{
		yggr::utf8_char utf8_ch = 'a';
		ref_type utf8_ch_erf = utf8_ch;

		yggr_test_assert(('a' | utf8_ch ) == 'a');
		yggr_test_assert(('b' | utf8_ch) == ('b' | 'a'));
		yggr_test_assert(('a' | yggr::utf8_char('a')) == 'a');
		yggr_test_assert(('b' | yggr::utf8_char('a')) == ('b' | 'a'));
	}

	// utf8_char_ref | utf8_char_ref
	{
		yggr::utf8_char utf8_ch_l = 'a';
		yggr::utf8_char utf8_ch_r = 'b';
		ref_type utf8_ch_ref_l = utf8_ch_l;
		ref_type utf8_ch_ref_r = utf8_ch_r;

		yggr_test_assert((ref_type(utf8_ch_ref_l) | ref_type(utf8_ch_ref_l)) == utf8_ch_l);
		yggr_test_assert((ref_type(utf8_ch_ref_l) | utf8_ch_ref_l) == utf8_ch_l);
		yggr_test_assert((utf8_ch_ref_l | ref_type(utf8_ch_ref_l)) == utf8_ch_l);
		yggr_test_assert((utf8_ch_ref_l | utf8_ch_ref_l) == utf8_ch_l);

		yggr_test_assert((ref_type(utf8_ch_ref_l) | ref_type(utf8_ch_ref_r)) == ('a' | 'b'));
		yggr_test_assert((ref_type(utf8_ch_ref_l) | utf8_ch_ref_r) == ('a' | 'b'));
		yggr_test_assert((utf8_ch_ref_l | ref_type(utf8_ch_ref_r)) == ('a' | 'b'));
		yggr_test_assert((utf8_ch_ref_l | utf8_ch_ref_r) == ('a' | 'b'));
	}

#ifndef YGGR_NO_CWCHAR

	// utf8_char_ref | n
	{
		yggr::utf8_char utf8_ch = L'Äã';
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((utf8_ch_ref | L'Äã') == L'Äã');
		yggr_test_assert((utf8_ch_ref | L'ºÃ') == (L'Äã' | L'ºÃ'));
		yggr_test_assert((ref_type(utf8_ch_ref) | L'Äã') == L'Äã');
		yggr_test_assert((ref_type(utf8_ch_ref) | L'ºÃ') == (L'Äã' | L'ºÃ'));
	}

	// n | utf8_char_ref
	{
		yggr::utf8_char utf8_ch = L'Äã';
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((L'Äã' | utf8_ch_ref ) == L'Äã');
		yggr_test_assert((L'ºÃ' | utf8_ch_ref) == (L'ºÃ' | L'Äã'));
		yggr_test_assert((L'Äã' | ref_type(utf8_ch_ref)) == L'Äã');
		yggr_test_assert((L'ºÃ' | ref_type(utf8_ch_ref)) == (L'ºÃ' | L'Äã'));
	}

	// utf8_char_ref | utf8_char_ref
	{
		yggr::utf8_char utf8_ch_l = L'Äã';
		yggr::utf8_char utf8_ch_r = L'ºÃ';
		ref_type utf8_ch_ref_l = utf8_ch_l;
		ref_type utf8_ch_ref_r = utf8_ch_r;

		yggr_test_assert((ref_type(utf8_ch_ref_l) | ref_type(utf8_ch_ref_l)) == utf8_ch_l);
		yggr_test_assert((ref_type(utf8_ch_ref_l) | utf8_ch_ref_l) == utf8_ch_l);
		yggr_test_assert((utf8_ch_ref_l | ref_type(utf8_ch_ref_l)) == utf8_ch_l);
		yggr_test_assert((utf8_ch_ref_l | utf8_ch_ref_l) == utf8_ch_l);

		yggr_test_assert((ref_type(utf8_ch_ref_l) | ref_type(utf8_ch_ref_r)) == (L'Äã' | L'ºÃ'));
		yggr_test_assert((ref_type(utf8_ch_ref_l) | utf8_ch_ref_r) == (L'Äã' | L'ºÃ'));
		yggr_test_assert((utf8_ch_ref_l | ref_type(utf8_ch_ref_r)) == (L'Äã' | L'ºÃ'));
		yggr_test_assert((utf8_ch_ref_l | utf8_ch_ref_r) == (L'Äã' | L'ºÃ'));
	}

#endif // YGGR_NO_CWCHAR

	std::cout << "test_bitwise_or_utf8_char_view succeed" << std::endl;
}

void test_bitwise_xor_of_utf8_char(void)
{
	// utf8_char ^ n
	{
		yggr::utf8_char utf8_ch = 'a';
		yggr_test_assert((utf8_ch ^ 'a') == 0);
		yggr_test_assert((utf8_ch ^ 'b') == ('a' ^ 'b'));
		yggr_test_assert((yggr::utf8_char('a') ^ 'a') == 0);
		yggr_test_assert((yggr::utf8_char('a') ^ 'b') == ('a' ^ 'b'));
	}

	// n ^ utf8_char
	{
		yggr::utf8_char utf8_ch = 'a';
		yggr_test_assert(('a' ^ utf8_ch ) == 0);
		yggr_test_assert(('b' ^ utf8_ch) == ('b' ^ 'a'));
		yggr_test_assert(('a' ^ yggr::utf8_char('a')) == 0);
		yggr_test_assert(('b' ^ yggr::utf8_char('a')) == ('b' ^ 'a'));
	}

	// utf8_char ^ utf8_char
	{
		yggr::utf8_char utf8_ch_l = 'a';
		yggr::utf8_char utf8_ch_r = 'b';

		yggr_test_assert((utf8_ch_l ^ utf8_ch_l) == 0);
		yggr_test_assert((utf8_ch_l ^ utf8_ch_r) == ('a' ^ 'b'));

		yggr_test_assert((yggr::utf8_char('a') ^ utf8_ch_l) == 0);
		yggr_test_assert((utf8_ch_l ^ yggr::utf8_char('b')) == ('a' ^ 'b'));

		yggr_test_assert((yggr::utf8_char('a') ^ yggr::utf8_char('b')) == ('a' ^ 'b'));
	}

	// utf8_char ^ utf8_char_view
	{
		yggr::utf8_char utf8_ch_l = 'a';
		yggr::utf8_char utf8_ch_r = 'b';

		yggr_test_assert((utf8_ch_l.view() ^ utf8_ch_l) == 0);
		yggr_test_assert((utf8_ch_l ^ utf8_ch_r.view()) == ('a' ^ 'b'));

		yggr_test_assert((yggr::utf8_char('a') ^ utf8_ch_l.view()) == 0);
		yggr_test_assert((utf8_ch_l.view() ^ yggr::utf8_char('b')) == ('a' ^ 'b'));
	}

	// utf8_char ^ utf8_char_reference
	{
		yggr::utf8_char utf8_ch_l = 'a';
		yggr::utf8_char utf8_ch_r = 'b';

		yggr_test_assert((static_cast<yggr::utf8_char::char_reference_type>(utf8_ch_l) ^ utf8_ch_l) == 0);
		yggr_test_assert((utf8_ch_l ^ static_cast<yggr::utf8_char::char_reference_type>(utf8_ch_r)) == ('a' ^ 'b'));

		yggr_test_assert((yggr::utf8_char('a') ^ static_cast<yggr::utf8_char::char_reference_type>(utf8_ch_l)) == 0);
		yggr_test_assert((static_cast<yggr::utf8_char::char_reference_type>(utf8_ch_l) ^ yggr::utf8_char('b')) == ('a' ^ 'b'));
	}

	// utf8_char ^ utf8_char_const_reference
	{
		yggr::utf8_char utf8_ch_l = 'a';
		yggr::utf8_char utf8_ch_r = 'b';

		yggr_test_assert((static_cast<yggr::utf8_char::char_const_reference_type>(utf8_ch_l) ^ utf8_ch_l) == 0);
		yggr_test_assert((utf8_ch_l ^ static_cast<yggr::utf8_char::char_const_reference_type>(utf8_ch_r)) == ('a' ^ 'b'));

		yggr_test_assert((yggr::utf8_char('a') ^ static_cast<yggr::utf8_char::char_const_reference_type>(utf8_ch_l)) == 0);
		yggr_test_assert((static_cast<yggr::utf8_char::char_const_reference_type>(utf8_ch_l) ^ yggr::utf8_char('b')) == ('a' ^ 'b'));
	}

#ifndef YGGR_NO_CWCHAR

	// utf8_char ^ n
	{
		yggr::utf8_char utf8_ch = L'Äã';
		yggr_test_assert((utf8_ch ^ L'Äã') == 0);
		yggr_test_assert((utf8_ch ^ L'ºÃ') == (L'Äã' ^ L'ºÃ'));
		yggr_test_assert((yggr::utf8_char(L'Äã') ^ L'Äã') == 0);
		yggr_test_assert((yggr::utf8_char(L'Äã') ^ L'ºÃ') == (L'Äã' ^ L'ºÃ'));
	}

	// n ^ utf8_char
	{
		yggr::utf8_char utf8_ch = L'Äã';
		yggr_test_assert((L'Äã' ^ utf8_ch ) == 0);
		yggr_test_assert((L'ºÃ' ^ utf8_ch) == (L'ºÃ' ^ L'Äã'));
		yggr_test_assert((L'Äã' ^ yggr::utf8_char(L'Äã')) == 0);
		yggr_test_assert((L'ºÃ' ^ yggr::utf8_char(L'Äã')) == (L'ºÃ' ^ L'Äã'));
	}

	// utf8_char ^ utf8_char
	{
		yggr::utf8_char utf8_ch_l = L'Äã';
		yggr::utf8_char utf8_ch_r = L'ºÃ';

		yggr_test_assert((utf8_ch_l ^ utf8_ch_l) == 0);
		yggr_test_assert((utf8_ch_l ^ utf8_ch_r) == (L'Äã' ^ L'ºÃ'));

		yggr_test_assert((yggr::utf8_char(L'Äã') ^ utf8_ch_l) == 0);
		yggr_test_assert((utf8_ch_l ^ yggr::utf8_char(L'ºÃ')) == (L'Äã' ^ L'ºÃ'));

		yggr_test_assert((yggr::utf8_char(L'Äã') ^ yggr::utf8_char(L'ºÃ')) == (L'Äã' ^ L'ºÃ'));
	}

	// utf8_char ^ utf8_char_view
	{
		yggr::utf8_char utf8_ch_l = L'Äã';
		yggr::utf8_char utf8_ch_r = L'ºÃ';

		yggr_test_assert((utf8_ch_l.view() ^ utf8_ch_l) == 0);
		yggr_test_assert((utf8_ch_l ^ utf8_ch_r.view()) == (L'Äã' ^ L'ºÃ'));

		yggr_test_assert((yggr::utf8_char(L'Äã') ^ utf8_ch_l.view()) == 0);
		yggr_test_assert((utf8_ch_l.view() ^ yggr::utf8_char(L'ºÃ')) == (L'Äã' ^ L'ºÃ'));
	}

	// utf8_char ^ utf8_char_reference
	{
		yggr::utf8_char utf8_ch_l = L'Äã';
		yggr::utf8_char utf8_ch_r = L'ºÃ';

		yggr_test_assert((static_cast<yggr::utf8_char::char_reference_type>(utf8_ch_l) ^ utf8_ch_l) == 0);
		yggr_test_assert((utf8_ch_l ^ static_cast<yggr::utf8_char::char_reference_type>(utf8_ch_r)) == (L'Äã' ^ L'ºÃ'));

		yggr_test_assert((yggr::utf8_char(L'Äã') ^ static_cast<yggr::utf8_char::char_reference_type>(utf8_ch_l)) == 0);
		yggr_test_assert((static_cast<yggr::utf8_char::char_reference_type>(utf8_ch_l) ^ yggr::utf8_char(L'ºÃ')) == (L'Äã' ^ L'ºÃ'));
	}

	// utf8_char ^ utf8_char_const_reference
	{
		yggr::utf8_char utf8_ch_l = L'Äã';
		yggr::utf8_char utf8_ch_r = L'ºÃ';

		yggr_test_assert((static_cast<yggr::utf8_char::char_const_reference_type>(utf8_ch_l) ^ utf8_ch_l) == 0);
		yggr_test_assert((utf8_ch_l ^ static_cast<yggr::utf8_char::char_const_reference_type>(utf8_ch_r)) == (L'Äã' ^ L'ºÃ'));

		yggr_test_assert((yggr::utf8_char(L'Äã') ^ static_cast<yggr::utf8_char::char_const_reference_type>(utf8_ch_l)) == 0);
		yggr_test_assert((static_cast<yggr::utf8_char::char_const_reference_type>(utf8_ch_l) ^ yggr::utf8_char(L'ºÃ')) == (L'Äã' ^ L'ºÃ'));
	}

#endif // YGGR_NO_CWCHAR

	std::cout << "test_bitwise_xor_of_utf8_char succeed" << std::endl;
}

void test_bitwise_xor_of_utf8_char_reference(void)
{
	typedef yggr::utf8_char::char_reference_type ref_type;

	// utf8_char_ref ^ n
	{
		yggr::utf8_char utf8_ch = 'a';
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((utf8_ch_ref ^ 'a') == 0);
		yggr_test_assert((utf8_ch_ref ^ 'b') == ('a' ^ 'b'));
		yggr_test_assert((ref_type(utf8_ch_ref) ^ 'a') == 0);
		yggr_test_assert((ref_type(utf8_ch_ref) ^ 'b') == ('a' ^ 'b'));
	}

	// n ^ utf8_char_ref
	{
		yggr::utf8_char utf8_ch = 'a';
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert(('a' ^ utf8_ch_ref ) == 0);
		yggr_test_assert(('b' ^ utf8_ch_ref) == ('b' ^ 'a'));
		yggr_test_assert(('a' ^ ref_type(utf8_ch_ref)) == 0);
		yggr_test_assert(('b' ^ ref_type(utf8_ch_ref)) == ('b' ^ 'a'));
	}

	// utf8_char_ref ^ utf8_char_ref
	{
		yggr::utf8_char utf8_ch_l = 'a';
		yggr::utf8_char utf8_ch_r = 'b';
		ref_type utf8_ch_ref_l = utf8_ch_l;
		ref_type utf8_ch_ref_r = utf8_ch_r;

		yggr_test_assert((ref_type(utf8_ch_ref_l) ^ ref_type(utf8_ch_ref_l)) == 0);
		yggr_test_assert((ref_type(utf8_ch_ref_l) ^ utf8_ch_ref_l) == 0);
		yggr_test_assert((utf8_ch_ref_l ^ ref_type(utf8_ch_ref_l)) == 0);
		yggr_test_assert((utf8_ch_ref_l ^ utf8_ch_ref_l) == 0);

		yggr_test_assert((ref_type(utf8_ch_ref_l) ^ ref_type(utf8_ch_ref_r)) == ('a' ^ 'b'));
		yggr_test_assert((ref_type(utf8_ch_ref_l) ^ utf8_ch_ref_r) == ('a' ^ 'b'));
		yggr_test_assert((utf8_ch_ref_l ^ ref_type(utf8_ch_ref_r)) == ('a' ^ 'b'));
		yggr_test_assert((utf8_ch_ref_l ^ utf8_ch_ref_r) == ('a' ^ 'b'));
	}

	// utf8_char_ref ^ utf8_char_view
	{
		yggr::utf8_char utf8_ch_l = 'a';
		yggr::utf8_char utf8_ch_r = 'b';
		ref_type utf8_ch_ref_l = utf8_ch_l;
		ref_type utf8_ch_ref_r = utf8_ch_r;

		yggr_test_assert((utf8_ch_ref_l.view() ^ utf8_ch_ref_l) == 0);
		yggr_test_assert((utf8_ch_ref_l ^ utf8_ch_ref_r.view()) == ('a' ^ 'b'));

		yggr_test_assert((ref_type(utf8_ch_ref_l) ^ utf8_ch_ref_l.view()) == 0);
		yggr_test_assert((utf8_ch_ref_l.view() ^ ref_type(utf8_ch_ref_r)) == ('a' ^ 'b'));
	}

	// utf8_char_ref ^ utf8_char_const_reference
	{
		yggr::utf8_char utf8_ch_l = 'a';
		yggr::utf8_char utf8_ch_r = 'b';
		ref_type utf8_ch_ref_l = utf8_ch_l;
		ref_type utf8_ch_ref_r = utf8_ch_r;

		yggr_test_assert((utf8_ch_ref_l.const_cast_() ^ utf8_ch_ref_l) == 0);
		yggr_test_assert((utf8_ch_ref_l ^ utf8_ch_ref_r.const_cast_()) == ('a' ^ 'b'));

		yggr_test_assert((ref_type(utf8_ch_ref_l) ^ utf8_ch_ref_l.const_cast_()) == 0);
		yggr_test_assert((utf8_ch_ref_l.const_cast_() ^ ref_type(utf8_ch_ref_r)) == ('a' ^ 'b'));
	}

#ifndef YGGR_NO_CWCHAR

	// utf8_char_ref ^ n
	{
		yggr::utf8_char utf8_ch = L'Äã';
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((utf8_ch_ref ^ L'Äã') == 0);
		yggr_test_assert((utf8_ch_ref ^ L'ºÃ') == (L'Äã' ^ L'ºÃ'));
		yggr_test_assert((ref_type(utf8_ch_ref) ^ L'Äã') == 0);
		yggr_test_assert((ref_type(utf8_ch_ref) ^ L'ºÃ') == (L'Äã' ^ L'ºÃ'));
	}

	// n ^ utf8_char_ref
	{
		yggr::utf8_char utf8_ch = L'Äã';
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((L'Äã' ^ utf8_ch_ref ) == 0);
		yggr_test_assert((L'ºÃ' ^ utf8_ch_ref) == (L'ºÃ' ^ L'Äã'));
		yggr_test_assert((L'Äã' ^ ref_type(utf8_ch_ref)) == 0);
		yggr_test_assert((L'ºÃ' ^ ref_type(utf8_ch_ref)) == (L'ºÃ' ^ L'Äã'));
	}

	// utf8_char_ref ^ utf8_char_ref
	{
		yggr::utf8_char utf8_ch_l = L'Äã';
		yggr::utf8_char utf8_ch_r = L'ºÃ';
		ref_type utf8_ch_ref_l = utf8_ch_l;
		ref_type utf8_ch_ref_r = utf8_ch_r;

		yggr_test_assert((ref_type(utf8_ch_ref_l) ^ ref_type(utf8_ch_ref_l)) == 0);
		yggr_test_assert((ref_type(utf8_ch_ref_l) ^ utf8_ch_ref_l) == 0);
		yggr_test_assert((utf8_ch_ref_l ^ ref_type(utf8_ch_ref_l)) == 0);
		yggr_test_assert((utf8_ch_ref_l ^ utf8_ch_ref_l) == 0);

		yggr_test_assert((ref_type(utf8_ch_ref_l) ^ ref_type(utf8_ch_ref_r)) == (L'Äã' ^ L'ºÃ'));
		yggr_test_assert((ref_type(utf8_ch_ref_l) ^ utf8_ch_ref_r) == (L'Äã' ^ L'ºÃ'));
		yggr_test_assert((utf8_ch_ref_l ^ ref_type(utf8_ch_ref_r)) == (L'Äã' ^ L'ºÃ'));
		yggr_test_assert((utf8_ch_ref_l ^ utf8_ch_ref_r) == (L'Äã' ^ L'ºÃ'));
	}

	// utf8_char_ref ^ utf8_char_view
	{
		yggr::utf8_char utf8_ch_l = L'Äã';
		yggr::utf8_char utf8_ch_r = L'ºÃ';
		ref_type utf8_ch_ref_l = utf8_ch_l;
		ref_type utf8_ch_ref_r = utf8_ch_r;

		yggr_test_assert((utf8_ch_ref_l.view() ^ utf8_ch_ref_l) == 0);
		yggr_test_assert((utf8_ch_ref_l ^ utf8_ch_ref_r.view()) == (L'Äã' ^ L'ºÃ'));

		yggr_test_assert((ref_type(utf8_ch_ref_l) ^ utf8_ch_ref_l.view()) == 0);
		yggr_test_assert((utf8_ch_ref_l.view() ^ ref_type(utf8_ch_ref_r)) == (L'Äã' ^ L'ºÃ'));
	}

	// utf8_char_ref ^ utf8_char_const_reference
	{
		yggr::utf8_char utf8_ch_l = L'Äã';
		yggr::utf8_char utf8_ch_r = L'ºÃ';
		ref_type utf8_ch_ref_l = utf8_ch_l;
		ref_type utf8_ch_ref_r = utf8_ch_r;

		yggr_test_assert((utf8_ch_ref_l.const_cast_() ^ utf8_ch_ref_l) == 0);
		yggr_test_assert((utf8_ch_ref_l ^ utf8_ch_ref_r.const_cast_()) == (L'Äã' ^ L'ºÃ'));

		yggr_test_assert((ref_type(utf8_ch_ref_l) ^ utf8_ch_ref_l.const_cast_()) == 0);
		yggr_test_assert((utf8_ch_ref_l.const_cast_() ^ ref_type(utf8_ch_ref_r)) == (L'Äã' ^ L'ºÃ'));
	}

#endif // YGGR_NO_CWCHAR

	std::cout << "test_bitwise_xor_of_utf8_char_reference succeed" << std::endl;
}

void test_bitwise_xor_of_utf8_char_const_reference(void)
{
	typedef yggr::utf8_char::char_const_reference_type ref_type;

	// utf8_char_ref ^ n
	{
		yggr::utf8_char utf8_ch = 'a';
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((utf8_ch_ref ^ 'a') == 0);
		yggr_test_assert((utf8_ch_ref ^ 'b') == ('a' ^ 'b'));
		yggr_test_assert((ref_type(utf8_ch_ref) ^ 'a') == 0);
		yggr_test_assert((ref_type(utf8_ch_ref) ^ 'b') == ('a' ^ 'b'));
	}

	// n ^ utf8_char_ref
	{
		yggr::utf8_char utf8_ch = 'a';
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert(('a' ^ utf8_ch_ref ) == 0);
		yggr_test_assert(('b' ^ utf8_ch_ref) == ('b' ^ 'a'));
		yggr_test_assert(('a' ^ ref_type(utf8_ch_ref)) == 0);
		yggr_test_assert(('b' ^ ref_type(utf8_ch_ref)) == ('b' ^ 'a'));
	}

	// utf8_char_ref ^ utf8_char_ref
	{
		yggr::utf8_char utf8_ch_l = 'a';
		yggr::utf8_char utf8_ch_r = 'b';
		ref_type utf8_ch_ref_l = utf8_ch_l;
		ref_type utf8_ch_ref_r = utf8_ch_r;

		yggr_test_assert((ref_type(utf8_ch_ref_l) ^ ref_type(utf8_ch_ref_l)) == 0);
		yggr_test_assert((ref_type(utf8_ch_ref_l) ^ utf8_ch_ref_l) == 0);
		yggr_test_assert((utf8_ch_ref_l ^ ref_type(utf8_ch_ref_l)) == 0);
		yggr_test_assert((utf8_ch_ref_l ^ utf8_ch_ref_l) == 0);

		yggr_test_assert((ref_type(utf8_ch_ref_l) ^ ref_type(utf8_ch_ref_r)) == ('a' ^ 'b'));
		yggr_test_assert((ref_type(utf8_ch_ref_l) ^ utf8_ch_ref_r) == ('a' ^ 'b'));
		yggr_test_assert((utf8_ch_ref_l ^ ref_type(utf8_ch_ref_r)) == ('a' ^ 'b'));
		yggr_test_assert((utf8_ch_ref_l ^ utf8_ch_ref_r) == ('a' ^ 'b'));
	}

	// utf8_char_ref ^ utf8_char_view
	{
		yggr::utf8_char utf8_ch_l = 'a';
		yggr::utf8_char utf8_ch_r = 'b';
		ref_type utf8_ch_ref_l = utf8_ch_l;
		ref_type utf8_ch_ref_r = utf8_ch_r;

		yggr_test_assert((utf8_ch_ref_l.view() ^ utf8_ch_ref_l) == 0);
		yggr_test_assert((utf8_ch_ref_l ^ utf8_ch_ref_r.view()) == ('a' ^ 'b'));

		yggr_test_assert((ref_type(utf8_ch_ref_l) ^ utf8_ch_ref_l.view()) == 0);
		yggr_test_assert((utf8_ch_ref_l.view() ^ ref_type(utf8_ch_ref_r)) == ('a' ^ 'b'));
	}

#ifndef YGGR_NO_CWCHAR

	// utf8_char_ref ^ n
	{
		yggr::utf8_char utf8_ch = L'Äã';
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((utf8_ch_ref ^ L'Äã') == 0);
		yggr_test_assert((utf8_ch_ref ^ L'ºÃ') == (L'Äã' ^ L'ºÃ'));
		yggr_test_assert((ref_type(utf8_ch_ref) ^ L'Äã') == 0);
		yggr_test_assert((ref_type(utf8_ch_ref) ^ L'ºÃ') == (L'Äã' ^ L'ºÃ'));
	}

	// n ^ utf8_char_ref
	{
		yggr::utf8_char utf8_ch = L'Äã';
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((L'Äã' ^ utf8_ch_ref ) == 0);
		yggr_test_assert((L'ºÃ' ^ utf8_ch_ref) == (L'ºÃ' ^ L'Äã'));
		yggr_test_assert((L'Äã' ^ ref_type(utf8_ch_ref)) == 0);
		yggr_test_assert((L'ºÃ' ^ ref_type(utf8_ch_ref)) == (L'ºÃ' ^ L'Äã'));
	}

	// utf8_char_ref ^ utf8_char_ref
	{
		yggr::utf8_char utf8_ch_l = L'Äã';
		yggr::utf8_char utf8_ch_r = L'ºÃ';
		ref_type utf8_ch_ref_l = utf8_ch_l;
		ref_type utf8_ch_ref_r = utf8_ch_r;

		yggr_test_assert((ref_type(utf8_ch_ref_l) ^ ref_type(utf8_ch_ref_l)) == 0);
		yggr_test_assert((ref_type(utf8_ch_ref_l) ^ utf8_ch_ref_l) == 0);
		yggr_test_assert((utf8_ch_ref_l ^ ref_type(utf8_ch_ref_l)) == 0);
		yggr_test_assert((utf8_ch_ref_l ^ utf8_ch_ref_l) == 0);

		yggr_test_assert((ref_type(utf8_ch_ref_l) ^ ref_type(utf8_ch_ref_r)) == (L'Äã' ^ L'ºÃ'));
		yggr_test_assert((ref_type(utf8_ch_ref_l) ^ utf8_ch_ref_r) == (L'Äã' ^ L'ºÃ'));
		yggr_test_assert((utf8_ch_ref_l ^ ref_type(utf8_ch_ref_r)) == (L'Äã' ^ L'ºÃ'));
		yggr_test_assert((utf8_ch_ref_l ^ utf8_ch_ref_r) == (L'Äã' ^ L'ºÃ'));
	}

	// utf8_char_ref ^ utf8_char_view
	{
		yggr::utf8_char utf8_ch_l = L'Äã';
		yggr::utf8_char utf8_ch_r = L'ºÃ';
		ref_type utf8_ch_ref_l = utf8_ch_l;
		ref_type utf8_ch_ref_r = utf8_ch_r;

		yggr_test_assert((utf8_ch_ref_l.view() ^ utf8_ch_ref_l) == 0);
		yggr_test_assert((utf8_ch_ref_l ^ utf8_ch_ref_r.view()) == (L'Äã' ^ L'ºÃ'));

		yggr_test_assert((ref_type(utf8_ch_ref_l) ^ utf8_ch_ref_l.view()) == 0);
		yggr_test_assert((utf8_ch_ref_l.view() ^ ref_type(utf8_ch_ref_r)) == (L'Äã' ^ L'ºÃ'));
	}

#endif // YGGR_NO_CWCHAR

	std::cout << "test_bitwise_xor_of_utf8_char_const_reference succeed" << std::endl;
}

void test_bitwise_xor_of_utf8_char_view(void)
{
	typedef yggr::utf8_char::char_view_type ref_type;

	// utf8_char_ref ^ n
	{
		yggr::utf8_char utf8_ch = 'a';
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((utf8_ch_ref ^ 'a') == 0);
		yggr_test_assert((utf8_ch_ref ^ 'b') == ('a' ^ 'b'));
		yggr_test_assert((ref_type(utf8_ch_ref) ^ 'a') == 0);
		yggr_test_assert((ref_type(utf8_ch_ref) ^ 'b') == ('a' ^ 'b'));
	}

	// n ^ utf8_char_ref
	{
		yggr::utf8_char utf8_ch = 'a';
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert(('a' ^ utf8_ch_ref ) == 0);
		yggr_test_assert(('b' ^ utf8_ch_ref) == ('b' ^ 'a'));
		yggr_test_assert(('a' ^ ref_type(utf8_ch_ref)) == 0);
		yggr_test_assert(('b' ^ ref_type(utf8_ch_ref)) == ('b' ^ 'a'));
	}

	// utf8_char_ref ^ utf8_char_ref
	{
		yggr::utf8_char utf8_ch_l = 'a';
		yggr::utf8_char utf8_ch_r = 'b';
		ref_type utf8_ch_ref_l = utf8_ch_l;
		ref_type utf8_ch_ref_r = utf8_ch_r;

		yggr_test_assert((ref_type(utf8_ch_ref_l) ^ ref_type(utf8_ch_ref_l)) == 0);
		yggr_test_assert((ref_type(utf8_ch_ref_l) ^ utf8_ch_ref_l) == 0);
		yggr_test_assert((utf8_ch_ref_l ^ ref_type(utf8_ch_ref_l)) == 0);
		yggr_test_assert((utf8_ch_ref_l ^ utf8_ch_ref_l) == 0);

		yggr_test_assert((ref_type(utf8_ch_ref_l) ^ ref_type(utf8_ch_ref_r)) == ('a' ^ 'b'));
		yggr_test_assert((ref_type(utf8_ch_ref_l) ^ utf8_ch_ref_r) == ('a' ^ 'b'));
		yggr_test_assert((utf8_ch_ref_l ^ ref_type(utf8_ch_ref_r)) == ('a' ^ 'b'));
		yggr_test_assert((utf8_ch_ref_l ^ utf8_ch_ref_r) == ('a' ^ 'b'));
	}

#ifndef YGGR_NO_CWCHAR

	// utf8_char_ref ^ n
	{
		yggr::utf8_char utf8_ch = L'Äã';
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((utf8_ch_ref ^ L'Äã') == 0);
		yggr_test_assert((utf8_ch_ref ^ L'ºÃ') == (L'Äã' ^ L'ºÃ'));
		yggr_test_assert((ref_type(utf8_ch_ref) ^ L'Äã') == 0);
		yggr_test_assert((ref_type(utf8_ch_ref) ^ L'ºÃ') == (L'Äã' ^ L'ºÃ'));
	}

	// n ^ utf8_char_ref
	{
		yggr::utf8_char utf8_ch = L'Äã';
		ref_type utf8_ch_ref = utf8_ch;

		yggr_test_assert((L'Äã' ^ utf8_ch_ref ) == 0);
		yggr_test_assert((L'ºÃ' ^ utf8_ch_ref) == (L'ºÃ' ^ L'Äã'));
		yggr_test_assert((L'Äã' ^ ref_type(utf8_ch_ref)) == 0);
		yggr_test_assert((L'ºÃ' ^ ref_type(utf8_ch_ref)) == (L'ºÃ' ^ L'Äã'));
	}

	// utf8_char_ref ^ utf8_char_ref
	{
		yggr::utf8_char utf8_ch_l = L'Äã';
		yggr::utf8_char utf8_ch_r = L'ºÃ';
		ref_type utf8_ch_ref_l = utf8_ch_l;
		ref_type utf8_ch_ref_r = utf8_ch_r;

		yggr_test_assert((ref_type(utf8_ch_ref_l) ^ ref_type(utf8_ch_ref_l)) == 0);
		yggr_test_assert((ref_type(utf8_ch_ref_l) ^ utf8_ch_ref_l) == 0);
		yggr_test_assert((utf8_ch_ref_l ^ ref_type(utf8_ch_ref_l)) == 0);
		yggr_test_assert((utf8_ch_ref_l ^ utf8_ch_ref_l) == 0);

		yggr_test_assert((ref_type(utf8_ch_ref_l) ^ ref_type(utf8_ch_ref_r)) == (L'Äã' ^ L'ºÃ'));
		yggr_test_assert((ref_type(utf8_ch_ref_l) ^ utf8_ch_ref_r) == (L'Äã' ^ L'ºÃ'));
		yggr_test_assert((utf8_ch_ref_l ^ ref_type(utf8_ch_ref_r)) == (L'Äã' ^ L'ºÃ'));
		yggr_test_assert((utf8_ch_ref_l ^ utf8_ch_ref_r) == (L'Äã' ^ L'ºÃ'));
	}

#endif // YGGR_NO_CWCHAR

	std::cout << "test_bitwise_xor_of_utf8_char_view succeed" << std::endl;
}

int main(int argc, char* argv[])
{
	BOOST_MPL_ASSERT((boost::mpl::bool_<(yggr::charset::utf8_foo::E_char_max_range > 0)>));

	test_plus_plus_of_utf8_char();
	test_plus_plus_of_utf8_char_reference();
	std::cout << "----------------" << std::endl;

	test_minus_minus_of_utf8_char();
	test_minus_minus_of_utf8_char_reference();
	std::cout << "----------------" << std::endl;

	test_positive_negative_bitnot_of_utf8_char();
	test_positive_negative_bitnot_of_utf8_char_reference();
	std::cout << "----------------" << std::endl;

	test_plus_set_of_utf8_char();
	test_plus_set_of_utf8_char_reference();
	std::cout << "----------------" << std::endl;

	test_minus_set_of_utf8_char();
	test_minus_set_of_utf8_char_reference();
	std::cout << "----------------" << std::endl;

	test_mul_set_of_utf8_char();
	test_mul_set_of_utf8_char_reference();
	std::cout << "----------------" << std::endl;

	test_div_set_of_utf8_char();
	test_div_set_of_utf8_char_reference();
	std::cout << "----------------" << std::endl;

	test_mod_set_of_utf8_char();
	test_mod_set_of_utf8_char_reference();
	std::cout << "----------------" << std::endl;

	test_bitwise_left_shift_set_of_utf8_char();
	test_bitwise_left_shift_set_of_utf8_char_reference();
	std::cout << "----------------" << std::endl;

	test_bitwise_right_shift_set_of_utf8_char();
	test_bitwise_right_shift_set_of_utf8_char_reference();
	std::cout << "----------------" << std::endl;

	test_bitwise_and_set_of_utf8_char();
	test_bitwise_and_set_of_utf8_char_reference();
	test_bitwise_and_set_of_utf8_char_const_reference();
	test_bitwise_and_set_of_utf8_char_view();
	std::cout << "----------------" << std::endl;

	test_bitwise_or_set_of_utf8_char();
	test_bitwise_or_set_of_utf8_char_reference();
	test_bitwise_or_set_of_utf8_char_const_reference();
	test_bitwise_or_set_of_utf8_char_view();
	std::cout << "----------------" << std::endl;

	test_bitwise_xor_set_of_utf8_char();
	test_bitwise_xor_set_of_utf8_char_reference();
	test_bitwise_xor_set_of_utf8_char_const_reference();
	test_bitwise_xor_set_of_utf8_char_view();
	std::cout << "----------------" << std::endl;

	test_plus_of_utf8_char();
	test_plus_of_utf8_char_reference();
	test_plus_of_utf8_char_const_reference();
	test_plus_of_utf8_char_view();
	std::cout << "----------------" << std::endl;

	test_minus_of_utf8_char();
	test_minus_of_utf8_char_reference();
	test_minus_of_utf8_char_const_reference();
	test_minus_of_utf8_char_view();
	std::cout << "----------------" << std::endl;

	test_mul_of_utf8_char();
	test_mul_of_utf8_char_reference();
	test_mul_of_utf8_char_const_reference();
	test_mul_of_utf8_char_view();
	std::cout << "----------------" << std::endl;

	test_div_of_utf8_char();
	test_div_of_utf8_char_reference();
	test_div_of_utf8_char_const_reference();
	test_div_of_utf8_char_view();
	std::cout << "----------------" << std::endl;

	test_mod_of_utf8_char();
	test_mod_of_utf8_char_reference();
	test_mod_of_utf8_char_const_reference();
	test_mod_of_utf8_char_view();
	std::cout << "----------------" << std::endl;

	test_bitwise_left_shfit_of_utf8_char();
	test_bitwise_left_shfit_of_utf8_char_reference();
	test_bitwise_left_shfit_of_utf8_char_const_reference();
	test_bitwise_left_shfit_of_utf8_char_view();
	std::cout << "----------------" << std::endl;

	test_bitwise_right_shift_of_utf8_char();
	test_bitwise_right_shift_of_utf8_char_reference();
	test_bitwise_right_shift_of_utf8_char_const_reference();
	test_bitwise_right_shift_of_utf8_char_view();
	std::cout << "----------------" << std::endl;

	test_bitwise_and_of_utf8_char();
	test_bitwise_and_of_utf8_char_reference();
	test_bitwise_and_of_utf8_char_const_reference();
	test_bitwise_and_of_utf8_char_view();
	std::cout << "----------------" << std::endl;

	test_bitwise_or_utf8_char();
	test_bitwise_or_utf8_char_reference();
	test_bitwise_or_utf8_char_const_reference();
	test_bitwise_or_utf8_char_view();
	std::cout << "----------------" << std::endl;

	test_bitwise_xor_of_utf8_char();
	test_bitwise_xor_of_utf8_char_reference();
	test_bitwise_xor_of_utf8_char_const_reference();
	test_bitwise_xor_of_utf8_char_view();
	std::cout << "----------------" << std::endl;

	wait_any_key(argc, argv);
	return 0;
}
