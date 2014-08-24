//fuzzy_string_test.cpp

#ifdef __MINGW32__
#   error "mingw not support this file!!"
#endif // __MINGW32__

#define BOOST_HAS_ICU

#include <iostream>
#include <vector>
#include <utility>
#include <sstream>
#include <cassert>

#include <boost/bind.hpp>

#include <yggr/charset/string.hpp>
#include <yggr/charset/utf8_string.hpp>
#include <yggr/regular_parse/u32regex_parse.hpp>


#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

void test_utf8_match(void)
{
	yggr::utf8_string text("你好,世界");

#if 1
	yggr::utf8_string str_re("^你.*");
	try
	{
		assert(yggr::regular_parse::u32regex_parse::match(text, str_re) == 1);
	}
	catch(yggr::regular_parse::u32regex_parse::error_type& e)
	{
		std::cout << e.what() << std::endl;
	}
#else
	yggr::utf8_string str_re("^k你.*");
	try
	{
		assert(yggr::regular_parse::u32regex_parse::match(text, str_re) == 0);
	}
	catch(yggr::regular_parse::u32regex_parse::error_type& e)
	{
		std::cout << e.what() << std::endl;
	}
#endif // 1

	try
	{
		std::cout << yggr::regular_parse::u32regex_parse::match(text, str_re) << std::endl;
	}
	catch(yggr::regular_parse::u32regex_parse::error_type& e)
	{
		std::cout << e.what() << std::endl;
	}
}

bool handler_utf8_match(bool bret, const boost::match_results<yggr::utf8_string::base_type::const_iterator>& m)
{
	if(!bret)
	{
		return bret;
	}

	int isize = m.size();
	for(int i = 0; i < isize; ++i)
	{
		yggr::utf8_string str(m[i].str(), UTF8_CHARSET_NAME);

		std::cout << "i = " << i << " : " << str << std::endl;
	}

	return bret;
}

void test_utf8_match2(void)
{
	yggr::utf8_string text("你好,世界");
	
#if 1
	yggr::utf8_string str_re("你.*");

	try
	{
		assert(yggr::regular_parse::u32regex_parse::match(text, str_re,
															yggr::regular_parse::u32regex_parse::regex_base_type::normal,
															boost::match_default,
															boost::bind(handler_utf8_match, _1, _2)));
	}
	catch(yggr::regular_parse::u32regex_parse::error_type& e)
	{
		std::cout << e.what() << std::endl;
	}
#else
	yggr::utf8_string str_re("^k你.*");
	try
	{
		assert(yggr::regular_parse::u32regex_parse::match(text, str_re,
															yggr::regular_parse::u32regex_parse::regex_base_type::normal,
															boost::match_default,
															boost::bind(handler_utf8_match, _1, _2)));
	}
	catch(yggr::regular_parse::u32regex_parse::error_type& e)
	{
		std::cout << e.what() << std::endl;
	}
#endif // 1
	try
	{
		std::cout << yggr::regular_parse::u32regex_parse::match(text, str_re,
																	yggr::regular_parse::u32regex_parse::regex_base_type::normal,
																	boost::match_default,
																	boost::bind(handler_utf8_match, _1, _2)) << std::endl;
	}
	catch(yggr::regular_parse::u32regex_parse::error_type& e)
	{
		std::cout << e.what() << std::endl;
	}
}

void test_utf8_match3(void)
{
	yggr::utf8_string text("你好,世界");

#if 1
	std::vector<yggr::utf8_string::value_type> vt(text.byte_size() + 1, 0); // + 1 must use c-style string vector
#else
	boost::container::vector<yggr::utf8_string::value_type> vt(text.byte_size() + 1, 0);
#endif // 1

	//memcpy(&vt[0], &text[0], sizeof(yggr::utf8_string::value_type) * text.byte_size()); // this use is worng operator[]() return utf8_char;
	memcpy(&vt[0], text.c_str(), sizeof(yggr::utf8_string::value_type) * text.byte_size());

#if 1
	yggr::utf8_string str_re("你.*");
	try
	{
		assert(yggr::regular_parse::u32regex_parse::match(text, str_re) == 1);
	}
	catch(yggr::regular_parse::u32regex_parse::error_type& e)
	{
		std::cout << e.what() << std::endl;
	}
#else
	yggr::utf8_string str_re("^k你.*");
	try
	{
		assert(yggr::regular_parse::u32regex_parse::match(vt, str_re) == 0);
	}
	catch(yggr::regular_parse::u32regex_parse::error_type& e)
	{
		std::cout << e.what() << std::endl;
	}
#endif //1

	try
	{
		std::cout << yggr::regular_parse::u32regex_parse::match(vt, str_re) << std::endl;
	}
	catch(yggr::regular_parse::u32regex_parse::error_type& e)
	{
		std::cout << e.what() << std::endl;
	}
}


template<typename Iter>
bool handler_match2(bool bright, const boost::match_results<Iter>& m)
{
	if(!bright)
	{
		return false;
	}

	int isize = m.size();
	for(int i = 0; i < isize; ++i)
	{
		yggr::utf8_string str(m[i].str(), UTF8_CHARSET_NAME);
		std::cout << "i = " << i<< " : " << str << std::endl;
	}

	return true;
}


void test_utf8_match4(void)
{
	yggr::utf8_string text("你好,世界");

#if 1
	typedef std::vector<yggr::utf8_string::value_type> vt_type;
	vt_type vt(text.byte_size() + 1, 0);
#else
	typedef boost::container::vector<yggr::utf8_string::value_type> vt_type;
	vt_type vt(text.byte_size() + 1, 0);
#endif // 1

	memcpy(&vt[0], text.c_str(), sizeof(yggr::utf8_string::value_type) * text.byte_size());
#if 1
	yggr::utf8_string str_re("你.*");
	try
	{
		assert(yggr::regular_parse::u32regex_parse::match(vt, str_re,
															yggr::regular_parse::u32regex_parse::regex_base_type::normal,
															boost::match_default,
															boost::bind(&handler_match2<vt_type::const_iterator>,
															_1, _2)) == 1);
	}
	catch(yggr::regular_parse::u32regex_parse::error_type& e)
	{
		std::cout << e.what() << std::endl;
	}
#else
	yggr::utf8_string str_re("^k你.*");
	try
	{
		assert(yggr::regular_parse::u32regex_parse::match(vt, str_re,
															yggr::regular_parse::u32regex_parse::regex_base_type::normal,
															boost::match_default,
															boost::bind(&handler_match2<vt_type::const_iterator>,
															_1, _2)) == 0);
	}
	catch(yggr::regular_parse::u32regex_parse::error_type& e)
	{
		std::cout << e.what() << std::endl;
	}
#endif //1

	try
	{
		std::cout << yggr::regular_parse::u32regex_parse::match(vt, str_re,
																	yggr::regular_parse::u32regex_parse::regex_base_type::normal,
																	boost::match_default,
																	boost::bind(&handler_match2<vt_type::const_iterator>,
																	_1, _2)) << std::endl;
	}
	catch(yggr::regular_parse::u32regex_parse::error_type& e)
	{
		std::cout << e.what() << std::endl;
	}
	
}

void test_utf8_replace(void)
{
	yggr::utf8_string str_re("(a)|(b)");
	yggr::utf8_string str_fmt("(?1_)(?2*)");

	yggr::utf8_string text("aaacccbbb");
	
	try
	{
		yggr::utf8_string rst(yggr::regular_parse::u32regex_parse::replace(text, str_re, str_fmt));
		assert(rst == "___ccc***");
		std::cout << rst << std::endl;
	}
	catch(yggr::regular_parse::u32regex_parse::error_type& e)
	{
		std::cout << e.what() << std::endl;
	}
		
}

yggr::utf8_string handler_utf8_replace(const yggr::utf8_string& str)
{
	return str + "_fixed";
}

void test_utf8_replace2(void)
{
	yggr::utf8_string str_re("(你)|(好)");
	yggr::utf8_string str_fmt("(?1一)(?2二)");
	yggr::utf8_string text("你你你啊啊啊好好好");

	try
	{
		yggr::utf8_string rst(yggr::regular_parse::u32regex_parse::replace(text, str_re, str_fmt,
																	yggr::regular_parse::u32regex_parse::regex_base_type::normal,
																	boost::match_default
																		| boost::format_all,
																	boost::bind(handler_utf8_replace, _1)));
		assert(rst == "一一一啊啊啊二二二_fixed");
		std::cout << rst << std::endl;
	}
	catch(yggr::regular_parse::u32regex_parse::error_type& e)
	{
		std::cout << e.what() << std::endl;
	}
}

struct ss_helper
{
	typedef std::ostream_iterator<char> iterator;
	typedef std::stringstream ss_type;

	ss_helper(std::stringstream& ss)
		: ref_ss(ss)
	{
	}

	ss_helper(const ss_helper& right)
		: ref_ss(right.ref_ss)
	{
	}

	~ss_helper(void)
	{
	}

	operator std::stringstream&(void)
	{
		return ref_ss;
	}

	operator const std::stringstream&(void) const
	{
		return ref_ss;
	}

	std::stringstream& ref_ss;
};

void test_utf8_replace3(void)
{
	yggr::utf8_string str_re("(你)|(好)");
	yggr::utf8_string str_fmt("(?1一)(?2二)");
	yggr::utf8_string text("你你你啊啊啊好好好");

	std::stringstream ss;
	ss_helper helper(ss);

	try
	{
		yggr::regular_parse::u32regex_parse::replace(helper, text, str_re, str_fmt);
	}
	catch(yggr::regular_parse::u32regex_parse::error_type& e)
	{
		std::cout << e.what() << std::endl;
	}
	yggr::utf8_string rst(ss.str(), UTF8_CHARSET_NAME);

	assert(rst == "一一一啊啊啊二二二");
	std::cout << rst << std::endl;
}

ss_helper& handler_utf8_replace2(ss_helper& helper)
{
	yggr::utf8_string str("_被处理了");

	helper.ref_ss << str.c_str();
	
	return helper;
}

void test_utf8_replace4(void)
{
	yggr::utf8_string str_re("(你)|(好)");
	yggr::utf8_string str_fmt("(?1一)(?2二)");
	yggr::utf8_string text("你你你啊啊啊好好好");

	std::stringstream ss;
	ss_helper helper(ss);

	try
	{
		yggr::regular_parse::u32regex_parse::replace(helper, text, str_re, str_fmt,
														yggr::regular_parse::u32regex_parse::regex_base_type::normal,
														boost::match_default
														| boost::format_all,
														boost::bind(handler_utf8_replace2, _1));
	}
	catch(yggr::regular_parse::u32regex_parse::error_type& e)
	{
		std::cout << e.what() << std::endl;
	}

	yggr::utf8_string rst(ss.str(), UTF8_CHARSET_NAME);

	assert(rst == "一一一啊啊啊二二二_被处理了");
	std::cout << rst << std::endl;
}

void test_utf8_search(void)
{
	yggr::utf8_string text("你好,世界");

#if 1
	yggr::utf8_string str_re("^你.*");
	try
	{
		assert(yggr::regular_parse::u32regex_parse::search(text, str_re) == true);
	}
	catch(yggr::regular_parse::u32regex_parse::error_type& e)
	{
		std::cout << e.what() << std::endl;
	}
#else
	yggr::utf8_string str_re("^k你.*");
	try
	{
		assert(yggr::regular_parse::u32regex_parse::search(text, str_re) == false);
	}
	catch(yggr::regular_parse::u32regex_parse::error_type& e)
	{
		std::cout << e.what() << std::endl;
	}
#endif // 1

	try
	{
		std::cout << yggr::regular_parse::u32regex_parse::search(text, str_re) << std::endl;
	}
	catch(yggr::regular_parse::u32regex_parse::error_type& e)
	{
		std::cout << e.what() << std::endl;
	}
}

bool handler_utf8_search(bool bret, const boost::match_results<yggr::utf8_string::base_type::const_iterator>& m)
{
	if(!bret)
	{
		return bret;
	}

	int isize = m.size();
	for(int i = 0; i != isize; ++i)
	{
		yggr::utf8_string str(m[i].str(), UTF8_CHARSET_NAME);
		
		std::cout <<"i = " << i << " str = " << str << std::endl;
		
	}

	return bret;
}

void test_utf8_search2(void)
{
	yggr::utf8_string text("你好,世界");

#if 1
	yggr::utf8_string str_re("^你.*");
	try
	{
		assert(yggr::regular_parse::u32regex_parse::search(text, str_re,
															yggr::regular_parse::u32regex_parse::regex_base_type::normal,
															boost::match_default,
															boost::bind(&handler_utf8_search, _1, _2)) == true);
	}
	catch(yggr::regular_parse::u32regex_parse::error_type& e)
	{
		std::cout << e.what() << std::endl;
	}
#else
	yggr::utf8_string str_re("^k你.*");

	try
	{
		assert(yggr::regular_parse::u32regex_parse::search(text, str_re,
															yggr::regular_parse::u32regex_parse::regex_base_type::normal,
															boost::match_default,
															boost::bind(&handler_utf8_search, _1, _2)) == false);
	}
	catch(yggr::regular_parse::u32regex_parse::error_type& e)
	{
		std::cout << e.what() << std::endl;
	}
#endif // 1

	try
	{
		std::cout << yggr::regular_parse::u32regex_parse::search(text, str_re,
																	yggr::regular_parse::u32regex_parse::regex_base_type::normal,
																	boost::match_default,
																	boost::bind(&handler_utf8_search, _1, _2)) << std::endl;
	}
	catch(yggr::regular_parse::u32regex_parse::error_type& e)
	{
		std::cout << e.what() << std::endl;
	}
}

void test_utf8_search3(void)
{
	yggr::utf8_string text("你好,世界");

#if 1
	std::vector<yggr::utf8_string::value_type> vt(text.byte_size() + 1, 0);
#else
	boost::container::vector<yggr::utf8_string::value_type> vt(text.byte_size() + 1, 0);
#endif // 1

	memcpy(&vt[0], text.c_str(), sizeof(yggr::utf8_string::value_type) * text.byte_size());

#if 1
	yggr::utf8_string str_re("你.*");
	try
	{
		assert(yggr::regular_parse::u32regex_parse::search(vt, str_re) == 1);
	}
	catch(yggr::regular_parse::u32regex_parse::error_type& e)
	{
		std::cout << e.what() << std::endl;
	}
#else
	yggr::utf8_string str_re("^k你.*");
	try
	{
		assert(yggr::regular_parse::u32regex_parse::search(vt, str_re) == 0);
	}
	catch(yggr::regular_parse::u32regex_parse::error_type& e)
	{
		std::cout << e.what() << std::endl;
	}
#endif // 1

	try
	{
		std::cout << yggr::regular_parse::u32regex_parse::search(vt, str_re) << std::endl;
	}
	catch(yggr::regular_parse::u32regex_parse::error_type& e)
	{
		std::cout << e.what() << std::endl;
	}
}

template<typename Iter>
bool handler_search2(bool bright, const boost::match_results<Iter>& m)
{
	if(!bright)
	{
		return false;
	}

	int isize = m.size();
	for(int i = 0; i < isize; ++i)
	{
		yggr::utf8_string str(m[i].c_str(), UTF8_CHARSET_NAME);
		std::cout << "i = " << i<< " : " << str << std::endl;
	}

	return true;
}


void test_utf8_serarch4(void)
{
	yggr::utf8_string text("世界 你好,另一个世界");

#if 1
	typedef std::vector<yggr::utf8_string::value_type> vt_type;
	vt_type vt(text.byte_size() + 1, 0);
#else
	typedef boost::container::vector<yggr::utf8_string::value_type> vt_type;
	vt_type vt(text.byte_size() + 1, 0);
#endif // 1

	memcpy(&vt[0], text.c_str(), sizeof(yggr::utf8_string::value_type) * text.byte_size());

#if 1
	yggr::utf8_string str_re("你.*");
	try
	{
		assert(yggr::regular_parse::u32regex_parse::search(vt, str_re,
																yggr::regular_parse::u32regex_parse::regex_base_type::normal,
																boost::match_default,
																boost::bind(&handler_match2<vt_type::const_iterator>,
																			_1, _2)) == 1);
	}
	catch(yggr::regular_parse::u32regex_parse::error_type& e)
	{
		std::cout << e.what() << std::endl;
	}
#else
	yggr::utf8_string str_re("^k你.*");
	try
	{
		assert(yggr::regular_parse::u32regex_parse::search(vt, str_re,
																yggr::regular_parse::u32regex_parse::regex_base_type::normal,
																boost::match_default,
																boost::bind(&handler_match2<vt_type::const_iterator>,
																_1, _2)) == 0);
	}
	catch(yggr::regular_parse::u32regex_parse::error_type& e)
	{
		std::cout << e.what() << std::endl;
	}
#endif // 1

	try
	{
		std::cout << yggr::regular_parse::u32regex_parse::search(vt, str_re,
																	yggr::regular_parse::u32regex_parse::regex_base_type::normal,
																	boost::match_default,
																	boost::bind(&handler_match2<vt_type::const_iterator>,
																					_1, _2)) << std::endl;
	}
	catch(yggr::regular_parse::u32regex_parse::error_type& e)
	{
		std::cout << e.what() << std::endl;
	}
}

int main(int argc, char* argv[])
{
	boost::u32regex r = boost::make_u32regex("(?:\\A|.*\\\\)([^\\\\]+)");

	test_utf8_match();
	test_utf8_match2();
	test_utf8_match3();
	test_utf8_match4();

	test_utf8_replace();
	test_utf8_replace2();
	test_utf8_replace3();
	test_utf8_replace4();

	test_utf8_search();
	test_utf8_search2();
	test_utf8_search3();
	test_utf8_serarch4();

	char cc = 0;
	std::cin >> cc;
	return 0;
}
