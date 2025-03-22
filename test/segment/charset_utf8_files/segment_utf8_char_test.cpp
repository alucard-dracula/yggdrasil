// segment_utf8_char_test.cpp

#if defined(_MSC_VER)
#	error "this file not supports MSVC only, if you are using other compilers, please use the same name file in the ../"
#endif // _MSC_VER

//#define YGGR_NO_CWCHAR

//#define YGGR_USE_STL_STRING
//#define _YGGR_DEBUG_UTF8_STRING_TEMPLATE_METHOD_ONLY

#include <test/wait_any_key/wait_any_key.hpp>
#include <cassert>

#include <yggr/segment/segment_info.hpp>
#include <yggr/segment/segment_container.hpp>
#include <yggr/segment/segment_op_proxy.hpp>
#include <yggr/segment/segment_op.hpp>
#include <yggr/segment/segment_mode.hpp>

#include <yggr/charset/utf8_char.hpp>
#include <yggr/charset/utf8_char_view.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(segment)
#include YGGR_PP_LINK_LIB(system_controller)
#include YGGR_PP_LINK_LIB(exception)
#include YGGR_PP_LINK_LIB(time)
#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(ids)
#include YGGR_PP_LINK_LIB(base)

template<typename Seg>
void delete_segment(const yggr::string& name)
{
	typedef Seg seg_type;
	typedef yggr::segment::segment_destroyer<seg_type> destroyer_type;

	if(name.empty())
	{
		return;
	}

	{ destroyer_type del(name.c_str()); }
}

template<typename Utf8Char, typename SegCont>
void test_constructor(SegCont& seg_cont)
{
	typedef Utf8Char utf8_char_type;
	typedef SegCont seg_cont_type;
	typedef typename utf8_char_type::allocator_type alloc_type;

	alloc_type alloc(seg_cont.segment().get_segment_manager());

	{
		utf8_char_type c(alloc);
		yggr_test_assert(c == "");
	}

	utf8_char_type c1('a', alloc);
	utf8_char_type c1_1('a', alloc);
	std::cout << c1 << std::endl;
	yggr_test_assert(c1 == c1_1);
	yggr_test_assert(c1 == 'a');
	yggr_test_assert(c1 == "a");

	utf8_char_type c2("b", alloc);

#if 0
	{
		utf8_char_type c2("abc", alloc); // assert fail
	}
#endif // 0, 1

	utf8_char_type c2_2("b", 1, alloc);
	std::cout << c2 << std::endl;

	yggr_test_assert(c2 == c2_2);

	utf8_char_type c3("你", alloc, YGGR_STR_UTF8_STRING_CHARSET_NAME());
	utf8_char_type c3_3(std::string("你").c_str(), std::string("你").size(), alloc, YGGR_STR_UTF8_STRING_CHARSET_NAME());
	std::cout << c3 << std::endl;
	yggr_test_assert(c3 == c3_3);
	yggr_test_assert(c3 == yggr::utf8_char("你", YGGR_STR_UTF8_STRING_CHARSET_NAME()));

	utf8_char_type c4(std::string("好"), alloc, YGGR_STR_UTF8_STRING_CHARSET_NAME());

#if 0
	{
		utf8_char_type c4(yggr::string("好a"), alloc, YGGR_STR_UTF8_STRING_CHARSET_NAME()); // assert fail
	}
#endif // 0, 1

	std::cout << c4 << std::endl;

#ifndef YGGR_NO_CWCHAR
	utf8_char_type c5(L"宽", alloc);
#else
    utf8_char_type c5("宽", alloc, YGGR_STR_UTF8_STRING_CHARSET_NAME());
#endif //YGGR_NO_CWCHAR
	std::cout << c5 << std::endl;

#ifndef YGGR_NO_CWCHAR
	utf8_char_type c6(yggr::wstring(L"字", 1), alloc);
#else
    utf8_char_type c6(yggr::string("字").c_str(), yggr::string("字").size(), alloc, YGGR_STR_UTF8_STRING_CHARSET_NAME());
#endif // YGGR_NO_CWCHAR

#if 0
	{
		utf8_char_type c4(yggr::string("好a"), alloc, YGGR_STR_UTF8_STRING_CHARSET_NAME()); // assert fail
	}
#endif // 0, 1
	std::cout << c6 << std::endl;

	utf8_char_type c7(c5.begin(), c5.end(), alloc, YGGR_STR_UTF8_STRING_CHARSET_NAME());

#if 0
	utf8_char_type c4(yggr::string("好a"), alloc, YGGR_STR_UTF8_STRING_CHARSET_NAME()); // assert fail
#endif // 0, 1

	std::cout << c7 << std::endl;

#ifndef YGGR_NO_CWCHAR
	std::wstring c6_2(L"字");
	utf8_char_type c8(c6_2.begin(), c6_2.end(), alloc);
#else
    std::string c6_2("字");
    utf8_char_type c8(c6_2.begin(), c6_2.end(), alloc, YGGR_STR_UTF8_STRING_CHARSET_NAME());
#endif //YGGR_NO_CWCHAR
	std::cout << c8 << std::endl;

	utf8_char_type c9 = c8;
	std::cout << c9 << std::endl;

	utf8_char_type c10(boost::move(c8));
	std::cout << c10 <<std::endl;

	// char
	{
		utf8_char_type ch('a', alloc);
		yggr_test_assert(ch == "a");
	}

	{
		utf8_char_type ch('a', alloc, YGGR_STR_UTF8_STRING_CHARSET_NAME());
		yggr_test_assert(ch == "a");
	}

	{
		utf8_char_type ch(yggr::charset::make_string_charset_helper('a'), alloc);
		yggr_test_assert(ch == "a");
	}

	// const char*
	{
		utf8_char_type ch("a", alloc);
		yggr_test_assert(ch == "a");
	}

	{
		utf8_char_type ch("a", alloc, YGGR_STR_UTF8_STRING_CHARSET_NAME());
		yggr_test_assert(ch == "a");
	}

	{
		utf8_char_type ch(yggr::charset::make_string_charset_helper("a"), alloc);
		yggr_test_assert(ch == "a");
	}

	{
		utf8_char_type ch1("a", alloc);
		utf8_char_type ch(ch1);
		yggr_test_assert(ch == "a");
	}

	{
		utf8_char_type ch1("a", alloc);
		utf8_char_type ch(yggr::charset::make_string_charset_helper(ch1));
		yggr_test_assert(ch == "a");
	}

	{
		typename utf8_char_type::base_type ch1("a", alloc);
		utf8_char_type ch(ch1);
		yggr_test_assert(ch == "a");
	}

	{
		typename utf8_char_type::base_type ch1("a", alloc);
		utf8_char_type ch(yggr::charset::make_string_charset_helper(ch1));
		yggr_test_assert(ch == "a");
	}

	{
		utf8_char_type ch1("a", alloc);
		utf8_char_type ch(boost::move(ch1), alloc);
		yggr_test_assert(ch == "a");
	}

	{
		utf8_char_type ch1("a", alloc);
		utf8_char_type ch(ch1, alloc);
		yggr_test_assert(ch == "a");
	}

	{
		utf8_char_type ch1("a", alloc);
		utf8_char_type ch(boost::move(ch1), YGGR_STR_UTF8_STRING_CHARSET_NAME());
		yggr_test_assert(ch == "a");
	}

	{
		utf8_char_type ch1("a", alloc);
		utf8_char_type ch(ch1, YGGR_STR_UTF8_STRING_CHARSET_NAME());
		yggr_test_assert(ch == "a");
	}

	{
		utf8_char_type ch1("a", alloc);
		utf8_char_type ch(boost::move(ch1), alloc, YGGR_STR_UTF8_STRING_CHARSET_NAME());
		yggr_test_assert(ch == "a");
	}

	{
		utf8_char_type ch1("a", alloc);
		utf8_char_type ch(ch1, alloc, YGGR_STR_UTF8_STRING_CHARSET_NAME());
		yggr_test_assert(ch == "a");
	}

	{
		yggr::charset::utf8_string_impl
		<
			typename utf8_char_type::value_type,
			typename utf8_char_type::traits_type,
			typename utf8_char_type::allocator_type
		> str("a", alloc);
		utf8_char_type ch(str[0]);
		yggr_test_assert(ch == "a");
	}

	{
		yggr::utf8_string str("a");
		utf8_char_type ch(str[0], alloc);
		yggr_test_assert(ch == "a");
	}

	{
		yggr::charset::utf8_string_impl
		<
			typename utf8_char_type::value_type,
			typename utf8_char_type::traits_type,
			typename utf8_char_type::allocator_type
		> str("a", alloc);
		utf8_char_type ch(str[0], YGGR_STR_UTF8_STRING_CHARSET_NAME());
		yggr_test_assert(ch == "a");
	}

	{
		yggr::utf8_string str("a");
		utf8_char_type ch(str[0], alloc, YGGR_STR_UTF8_STRING_CHARSET_NAME());
		yggr_test_assert(ch == "a");
	}

	{
		typedef
			yggr::charset::utf8_string_impl
			<
				typename utf8_char_type::value_type,
				typename utf8_char_type::traits_type,
				typename utf8_char_type::allocator_type
			> now_utf8_string_type;

		now_utf8_string_type str("a", alloc);
		const now_utf8_string_type& str_cref = str;

		utf8_char_type ch(str_cref[0]);
		yggr_test_assert(ch == "a");
	}

	{
		yggr::utf8_string str("a");
		const yggr::utf8_string& str_cref = str;
		utf8_char_type ch(str_cref[0], alloc);
		yggr_test_assert(ch == "a");
	}

	{
		typedef
			yggr::charset::utf8_string_impl
			<
				typename utf8_char_type::value_type,
				typename utf8_char_type::traits_type,
				typename utf8_char_type::allocator_type
			> now_utf8_string_type;

		now_utf8_string_type str("a", alloc);
		const now_utf8_string_type& str_cref = str;

		utf8_char_type ch(str_cref[0], YGGR_STR_UTF8_STRING_CHARSET_NAME());
		yggr_test_assert(ch == "a");
	}

	{
		yggr::utf8_string str("a");
		const yggr::utf8_string& str_cref = str;
		utf8_char_type ch(str_cref[0], alloc, YGGR_STR_UTF8_STRING_CHARSET_NAME());
		yggr_test_assert(ch == "a");
	}

	{
		yggr::utf8_char_view chv("a");
		utf8_char_type ch(chv, alloc);
		yggr_test_assert(ch == "a");
	}

	{
		yggr::utf8_char_view chv("a");
		utf8_char_type ch(chv, alloc, YGGR_STR_UTF8_STRING_CHARSET_NAME());
		yggr_test_assert(ch == "a");
	}

	std::cout << "----------test_constructor end-----------" << std::endl;
}

template<typename Utf8Char, typename SegCont>
void test_operator_equal(SegCont& seg_cont)
{
	typedef Utf8Char utf8_char_type;
	typedef SegCont seg_cont_type;
	typedef typename utf8_char_type::allocator_type alloc_type;

	alloc_type alloc(seg_cont.segment().get_segment_manager());

	utf8_char_type c1("你", alloc, YGGR_STR_UTF8_STRING_CHARSET_NAME());
	std::cout << c1 << std::endl;

	yggr_test_assert(c1 == yggr::utf8_char("你", YGGR_STR_UTF8_STRING_CHARSET_NAME()));

	c1 = 'a';
	std::cout << c1 << std::endl;

	yggr_test_assert(c1 == "a");

#if 0
	c1 = "abc"; // assert fail
#endif // 0, 1

#ifndef YGGR_NO_CWCHAR
	c1 = L"好";
#else
    c1 = yggr::charset::make_string_charset_helper("好", YGGR_STR_UTF8_STRING_CHARSET_NAME());
#endif //YGGR_NO_CWCHAR
	std::cout << c1 << std::endl;

	utf8_char_type c2('b', alloc);

	std::cout << c2 << std::endl;

	c2 = c1;
	std::cout << c2 << std::endl;

#ifndef YGGR_NO_CWCHAR
	std::wcout << c2 << std::endl;
#endif // YGGR_NO_CWCHAR

	std::cout << "----------test_operator_equal end-----------" << std::endl;
}

template<typename Utf8Char, typename SegCont>
void test_operator_compare(SegCont& seg_cont)
{
	typedef Utf8Char utf8_char_type;
	typedef SegCont seg_cont_type;
	typedef typename utf8_char_type::allocator_type alloc_type;

	alloc_type alloc(seg_cont.segment().get_segment_manager());

	{
		utf8_char_type uchar("好", alloc, YGGR_STR_UTF8_STRING_CHARSET_NAME());
		std::string str = uchar.template str<std::string>();
		yggr_test_assert(uchar == str);
		yggr_test_assert(uchar == str.c_str());
		yggr_test_assert(!(uchar != str));
		yggr_test_assert(!(uchar != str.c_str()));
	}

	{
		utf8_char_type uchar("好", alloc, YGGR_STR_UTF8_STRING_CHARSET_NAME());
#ifndef YGGR_NO_CWCHAR
		std::wstring str = L"好";
#else
        std::string str = uchar.template str<std::string>();
#endif // YGGR_NO_CWCHAR
		yggr_test_assert(uchar == str);
		yggr_test_assert(uchar == str.c_str());
		yggr_test_assert(!(uchar != str));
		yggr_test_assert(!(uchar != str.c_str()));
	}

	{
		utf8_char_type uchar("好", alloc, YGGR_STR_UTF8_STRING_CHARSET_NAME());
		boost::container::string str = yggr::utf8_char("不", YGGR_STR_UTF8_STRING_CHARSET_NAME()).str<boost::container::string>();
		yggr_test_assert(uchar != str);
		yggr_test_assert(uchar != str.c_str());
		yggr_test_assert(!(uchar == str));
		yggr_test_assert(!(uchar == str.c_str()));
	}

	{
		utf8_char_type uchar("好", alloc, YGGR_STR_UTF8_STRING_CHARSET_NAME());
#ifndef YGGR_NO_CWCHAR
		boost::container::wstring str = L"不";
#else
        boost::container::string str = yggr::utf8_char("不", YGGR_STR_UTF8_STRING_CHARSET_NAME()).str<boost::container::string>();
#endif // YGGR_NO_CWCHAR
		yggr_test_assert(uchar != str);
		yggr_test_assert(uchar != str.c_str());
		yggr_test_assert(!(uchar == str));
		yggr_test_assert(!(uchar == str.c_str()));
	}

	{
		utf8_char_type uchar("好", alloc, YGGR_STR_UTF8_STRING_CHARSET_NAME());
#ifndef YGGR_NO_CWCHAR
		utf8_char_type str(L"不", alloc);
#else
        utf8_char_type str("不", alloc, YGGR_STR_UTF8_STRING_CHARSET_NAME());
#endif // YGGR_NO_CWCHAR
		yggr_test_assert(uchar != str);
		yggr_test_assert(!(uchar == str));
#if 0
		yggr_test_assert(!(uchar == str.c_str())); //conv fail don't like to use
#else
		//ok
		yggr_test_assert(!(uchar == yggr::charset::make_string_charset_helper(
							str.c_str(), yggr::charset::charset_name_t<utf8_char_type>())));
#endif // 0, 1
	}

	std::cout << "----------test_operator_compare end-----------" << std::endl;
}

template<typename Utf8Char, typename SegCont>
void swap_test(SegCont& seg_cont)
{
	typedef Utf8Char utf8_char_type;
	typedef SegCont seg_cont_type;
	typedef typename utf8_char_type::allocator_type alloc_type;

	alloc_type alloc(seg_cont.segment().get_segment_manager());

	utf8_char_type c1("你", alloc, YGGR_STR_UTF8_STRING_CHARSET_NAME());
	utf8_char_type c2("好", alloc, YGGR_STR_UTF8_STRING_CHARSET_NAME());

	utf8_char_type c3 = c1;
	utf8_char_type c4 = c2;

	c3.swap(c4);
	yggr_test_assert(c1 == c4);
	yggr_test_assert(c2 == c3);

	boost::swap(c1, c2);
	yggr_test_assert(c1 == c3);
	yggr_test_assert(c2 == c4);

	std::swap(c1, c2);
	yggr_test_assert(c1 == c4);
	yggr_test_assert(c2 == c3);

	utf8_char_type c(alloc);
	yggr_test_assert(!c);
	c = 'a';
	yggr_test_assert(c);

	std::cout << "----------swap_test end-----------" << std::endl;
}

#include <yggr/charset/utf8_string.hpp>

template<typename Utf8Char, typename SegCont>
void utf8_string_to_char_test(SegCont& seg_cont)
{
	typedef Utf8Char utf8_char_type;
	typedef SegCont seg_cont_type;
	typedef typename utf8_char_type::allocator_type alloc_type;

	alloc_type alloc(seg_cont.segment().get_segment_manager());

	yggr::utf8_string str("你好bcdef吗", YGGR_STR_UTF8_STRING_CHARSET_NAME());
	utf8_char_type uc1(str.begin(), str.begin() + 1, alloc);
	std::cout << uc1 << std::endl;

	utf8_char_type uc2(str.rbegin(), str.rbegin() + 1, alloc);
	std::cout << uc2 << std::endl;

	std::cout << "----------utf8_string_to_char_test end-----------" << std::endl;
}

template<typename Utf8Char, typename SegCont>
void debug_test(SegCont& seg_cont)
{
	typedef Utf8Char utf8_char_type;

	test_constructor<utf8_char_type>(seg_cont);
	test_operator_equal<utf8_char_type>(seg_cont);
	test_operator_compare<utf8_char_type>(seg_cont);
	swap_test<utf8_char_type>(seg_cont);
	utf8_string_to_char_test<utf8_char_type>(seg_cont);

    std::cout << "debug_test end" << std::endl;
}

int main(int argc, char* argv[])
{
	
#ifndef YGGR_NO_CWCHAR
	TEST_PP_WIO_IMBUE("chs");
#endif // YGGR_NO_CWCHAR
	
	// managed_shared_memory
	{
		typedef boost::interprocess::managed_shared_memory seg_type;
		typedef yggr::segment::segment_container<seg_type> seg_cont_type;
		typedef boost::interprocess::allocator<char, seg_type::segment_manager> alloc_type;
		typedef
			yggr::charset::utf8_char_impl
			<
				char,
				yggr::charset::basic_string<char>::traits_type,
				alloc_type
			> utf8_char_type;


		seg_cont_type seg_cont("test_managed_shared_memory_utf8_char",
								yggr::segment::segment_op::create_only(),
								65536,
								boost::bind(
									&delete_segment<seg_type>,
									"test_managed_shared_memory_utf8_char") );

		debug_test<utf8_char_type>(seg_cont);

		seg_cont.free();
		yggr_test_assert(seg_cont.is_named_enable_object());
		yggr_test_assert(seg_cont.empty());
	}

	// test_mamaged_mapped_file
	{

		typedef boost::interprocess::managed_mapped_file seg_type;
		typedef yggr::segment::segment_container<seg_type> seg_cont_type;
		typedef boost::interprocess::allocator<char, seg_type::segment_manager> alloc_type;
		typedef
			yggr::charset::utf8_char_impl
			<
				char,
				yggr::charset::basic_string<char>::traits_type,
				alloc_type
			> utf8_char_type;

		seg_cont_type seg_cont("../test_out/segment/test_mamaged_mapped_file_utf8_char",
								yggr::segment::segment_op::create_only(),
								65536,
								boost::bind(
									&delete_segment<seg_type>,
									"../test_out/segment/test_mamaged_mapped_file_utf8_char"));

		debug_test<utf8_char_type>(seg_cont);

		seg_cont.free();
		yggr_test_assert(seg_cont.is_named_enable_object());
		yggr_test_assert(seg_cont.empty());
	}

	// test_managed_heap_memory
	{
		typedef boost::interprocess::managed_heap_memory seg_type;
		typedef yggr::segment::segment_container<seg_type> seg_cont_type;
		typedef boost::interprocess::allocator<char, seg_type::segment_manager> alloc_type;
		typedef
			yggr::charset::utf8_char_impl
			<
				char,
				yggr::charset::basic_string<char>::traits_type,
				alloc_type
			> utf8_char_type;

		seg_cont_type seg_cont(65536);

		debug_test<utf8_char_type>(seg_cont);

		seg_cont.free();
		yggr_test_assert(!seg_cont.is_named_enable_object());
		yggr_test_assert(seg_cont.empty());
	}

	wait_any_key(argc, argv);
	return 0;
}

