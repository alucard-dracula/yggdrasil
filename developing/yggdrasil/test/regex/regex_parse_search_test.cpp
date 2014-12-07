//regex_parse_search_test.cpp



#include <iostream>

#include <list>

#include <boost/bind.hpp>
#include <vector>
#include <sstream>

#include <yggr/regular_parse/regex_parse.hpp>

#ifdef _MSC_VER
#   include <vld.h> //max min at this include defined
#endif //_MSC_VER

#include <yggr/support/max_min_undef.ipp>

void test_match(void)
{
	//std::string str_re("aBB");
	//std::string text("Abb");

	std::string str_re("(a)(?(1)BB|ccc)");
	std::string text("abb");

	try
	{
		std::cout << yggr::regular_parse::regex_parse::match(text, str_re,
																yggr::regular_parse::regex_parse::regex_base_type::icase
																| yggr::regular_parse::regex_parse::regex_base_type::normal) << std::endl;
	}
	catch(const yggr::regular_parse::regex_parse::error_type& e)
	{
		std::cout << e.what() << std::endl;
	}
}

bool handler_match(bool bright, const boost::smatch& m)
{
	if(!bright)
	{
		return false;
	}

	int isize = m.size();
	for(int i = 0; i < isize; ++i)
	{
		std::cout << "i = " << i << " : " << m[i] << std::endl;
	}

	return true;
}

void test_match2(void)
{
	//std::string str_re("(a)(?(1)bb|ccc)");
	//std::string text("abb");

	//std::string str_re("a*b*");
	std::string str_re("(a*)(b*)");
	//std::string str_re("(a)*(b)*");
	std::string text("aaaabbbb");

	try
	{
		std::cout << yggr::regular_parse::regex_parse::match(text, str_re,
																yggr::regular_parse::regex_parse::regex_base_type::normal,
																boost::match_default,
																boost::bind(handler_match, _1, _2)) << std::endl;
	}
	catch(yggr::regular_parse::regex_parse::error_type& e)
	{
		std::cout << e.what() << std::endl;
	}
}

void test_match3(void)
{
	std::string str_re("[[:alpha:]]*");
	std::vector<std::string::value_type> vt;
	vt.push_back('a');
	vt.push_back('b');
	vt.push_back('c');
	//vt.push_back('0'); // result false

	try
	{
		std::cout << yggr::regular_parse::regex_parse::match(vt, str_re) << std::endl;
	}
	catch(yggr::regular_parse::regex_parse::error_type& e)
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
		std::cout << "i = " << i<< " : " << m[i] << std::endl;
	}

	return true;
}


void test_match4(void)
{
	std::string str_re("[[:alpha:]]*");
	std::vector<std::string::value_type> vt;
	vt.push_back('a');
	vt.push_back('b');
	vt.push_back('c');
	//vt.push_back('0'); // result false

	try
	{
		std::cout << yggr::regular_parse::regex_parse::match(vt, str_re, yggr::regular_parse::regex_parse::regex_base_type::normal,
																boost::match_default,
																boost::bind(&handler_match2<std::vector<std::string::value_type>::const_iterator>,
																_1, _2)) << std::endl;
	}
	catch(yggr::regular_parse::regex_parse::error_type& e)
	{
		std::cout << e.what() << std::endl;
	}
}


void test_replace1(void)
{
	std::string str_re("(a)|(b)");
	std::string str_fmt("(?1_)(?2_)");
	std::string text("aaacccbbb");

	try
	{
		std::cout << yggr::regular_parse::regex_parse::replace(text, str_re, str_fmt) << std::endl;
	}
	catch(yggr::regular_parse::regex_parse::error_type& e)
	{
		std::cout << e.what() << std::endl;
	}
}

std::string handler_replace(const std::string& rst)
{
	return std::string("fixed")+rst;
}

void test_replace2(void)
{
	std::string str_re("(a)|(b)");
	std::string str_fmt("(?1_)(?2_)");
	std::string text("aaacccbbb");

	try
	{
		std::cout << yggr::regular_parse::regex_parse::replace(text, str_re, str_fmt,
																yggr::regular_parse::regex_parse::regex_base_type::normal,
																boost::match_default
																| boost::format_all,
																boost::bind(handler_replace, _1)) << std::endl;
	}
	catch(yggr::regular_parse::regex_parse::error_type& e)
	{
		std::cout << e.what() << std::endl;
	}
}

struct ss_helper
{
	typedef std::ostream_iterator<char> iterator;

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

void test_replace3(void)
{
	std::string str_re("(a)|(b)");
	std::string str_fmt("(?1_)(?2_)");
	std::string text("aaacccbbb");

	std::stringstream ss;

	ss_helper helper(ss);

	try
	{
		yggr::regular_parse::regex_parse::replace(helper, text, str_re, str_fmt);
	}
	catch(yggr::regular_parse::regex_parse::error_type& e)
	{
		std::cout << e.what() << std::endl;
	}

	std::cout << ss.str() << std::endl;
}

ss_helper& handler_replace2(ss_helper& helper)
{
	helper.ref_ss << "_fided";
	return helper;
}

void test_replace4(void)
{
	std::string str_re("(a)|(b)");
	std::string str_fmt("(?1_)(?2_)");
	std::string text("aaacccbbb");

	std::stringstream ss;

	ss_helper helper(ss);

	try
	{
		yggr::regular_parse::regex_parse::replace(helper, text, str_re, str_fmt,
													yggr::regular_parse::regex_parse::regex_base_type::normal,
													boost::match_default
													| boost::format_all,
													boost::bind(handler_replace2, _1));
	}
	catch(yggr::regular_parse::regex_parse::error_type& e)
	{
		std::cout << e.what() << std::endl;
	}

	std::cout << ss.str() << std::endl;
}

//<a href="http://www.google.com">google</a>


void test_split1_1(void)
{
	std::list<std::string> list;
	std::string text("aaa bbb");

	std::cout << yggr::regular_parse::regex_parse::split(list, text) << std::endl;

	std::cout <<"text = " << text << std::endl;

	for(;!(list.empty());)
	{
		std::cout << list.front() << std::endl;
		list.pop_front();
	}

}

void test_split1_2(void)
{
	std::list<std::string> list;
	//boost::container::string text("aaa bbb");
	std::string text("aaa bbb");

	try
	{
		std::cout << yggr::regular_parse::regex_parse::split(list, boost::ref(text)) << std::endl;
	}
	catch(yggr::regular_parse::regex_parse::error_type& e)
	{
		std::cout << e.what() << std::endl;
	}

	std::cout << "text = " << text << std::endl;

	for(;!(list.empty());)
	{
		std::cout << list.front() << std::endl;
		list.pop_front();
	}

}

template<typename Container>
yggr::size_type handler_split(Container& cnt, yggr::size_type size)
{
	typedef Container container_type;
	typedef typename container_type::iterator iter_type;

	iter_type isize = cnt.end();

	for(iter_type i = cnt.begin() ; i != isize; ++i)
	{
		(*i) += "_fix";
	}

	return size;
}

void test_split2_1(void)
{
	std::list<std::string> list;
	std::string text("aaa bbb");

	try
	{
		std::cout << yggr::regular_parse::regex_parse::split(list,
																text,
																boost::bind(handler_split< std::list<std::string> >, _1, _2))
					<< std::endl;
	}
	catch(yggr::regular_parse::regex_parse::error_type& e)
	{
		std::cout << e.what() << std::endl;
	}

	std::cout <<"text = " << text << std::endl;

	for(;!(list.empty());)
	{
		std::cout << list.front() << std::endl;
		list.pop_front();
	}
}

void test_split2_2(void)
{
	std::list<std::string> list;
	std::string text("aaa bbb");

	try
	{
		std::cout << yggr::regular_parse::regex_parse::split(list, boost::ref(text),
																boost::bind(handler_split<std::list<std::string> >, _1, _2))
																<< std::endl;
	}
	catch(yggr::regular_parse::regex_parse::error_type& e)
	{
		std::cout << e.what() << std::endl;
	}

	std::cout <<"text = " << text << std::endl;

	for(;!(list.empty());)
	{
		std::cout << list.front() << std::endl;
		list.pop_front();
	}
}

void test_split3_1(void)
{
	std::list<std::string> list;
	std::string text("<a href=\"http://www.google.com\">google</a>");
	std::string str_re("<\\s*A\\s+[^>]*href\\s*=\\s*\"([^\"]*)\"");

	try
	{
		std::cout << yggr::regular_parse::regex_parse::split(list, text, str_re)<< std::endl;
	}
	catch(yggr::regular_parse::regex_parse::error_type& e)
	{
		std::cout << e.what() << std::endl;
	}

	std::cout <<"text = " << text << std::endl;

	for(;!(list.empty());)
	{
		std::cout << list.front() << std::endl;
		list.pop_front();
	}
}

void test_split3_1_2(void)
{
	std::list<std::string> list;
	std::string text("  aaa bbb,,ccc#ddd  ");
	std::string str_re("\\s+|,|#");

	try
	{
		std::cout << yggr::regular_parse::regex_parse::split(list, text, str_re)<< std::endl;
	}
	catch(yggr::regular_parse::regex_parse::error_type& e)
	{
		std::cout << e.what() << std::endl;
	}

	std::cout <<"text = " << text << std::endl;

	for(;!(list.empty());)
	{
		std::cout << list.front() << std::endl;
		list.pop_front();
	}
}

void test_split3_2(void)
{
	std::list<std::string> list;
	std::string text("<a href=\"http://www.google.com\">google</a>");
	std::string str_re("<\\s*A\\s+[^>]*href\\s*=\\s*\"([^\"]*)\"");

	try
	{
		std::cout << yggr::regular_parse::regex_parse::split(list, boost::ref(text), str_re)<< std::endl;
	}
	catch(yggr::regular_parse::regex_parse::error_type& e)
	{
		std::cout << e.what() << std::endl;
	}

	std::cout <<"text = " << text << std::endl;

	for(;!(list.empty());)
	{
		std::cout << list.front() << std::endl;
		list.pop_front();
	}
}

void test_split4_1(void)
{
	std::list<std::string> list;
	std::string text("<a href=\"http://www.google.com\">google</a>");
	std::string str_re("<\\s*A\\s+[^>]*href\\s*=\\s*\"([^\"]*)\"");

	try
	{
		std::cout << (yggr::regular_parse::regex_parse::split(list, text, str_re,
																boost::regex::normal | boost::regex::icase,
																boost::match_default,
																std::numeric_limits<yggr::size_type>::max(),
																boost::bind(&handler_split< std::list<std::string> >, _1, _2)))<< std::endl;
	}
	catch(yggr::regular_parse::regex_parse::error_type& e)
	{
		std::cout << e.what() << std::endl;
	}


	std::cout <<"text = " << text << std::endl;

	for(;!(list.empty());)
	{
		std::cout << list.front() << std::endl;
		list.pop_front();
	}
}

void test_split4_2(void)
{
	std::list<std::string> list;
	std::string text("<a href=\"http://www.google.com\">google</a>");
	std::string str_re("<\\s*A\\s+[^>]*href\\s*=\\s*\"([^\"]*)\"");
	//std::string str_re("<\\s*A\\s+[^>]*href\\s*=\\s*\"[^\"]*\"");

	try
	{
		std::cout << (yggr::regular_parse::regex_parse::split(list, boost::ref(text), str_re,
																boost::regex::normal | boost::regex::icase,
																boost::match_default,
																std::numeric_limits<yggr::size_type>::max(),
																boost::bind(handler_split< std::list<std::string> >, _1, _2)))<< std::endl;
	}
	catch(yggr::regular_parse::regex_parse::error_type& e)
	{
		std::cout << e.what() << std::endl;
	}

	std::cout <<"text = " << text << std::endl;

	for(;!(list.empty());)
	{
		std::cout << list.front() << std::endl;
		list.pop_front();
	}
}

#include <map>

typedef std::map<std::string, std::string::difference_type, std::less<std::string> > map_type;


bool handler_grep1(map_type& m, std::string::const_iterator base,
						const boost::match_results<std::string::const_iterator>& what)
{
	m[std::string(what[5].first, what[5].second) + std::string(what[6].first, what[6].second)] = what[5].first - base;
	return true;
}

bool handler_grep2(map_type& m, std::vector<char>::const_iterator base, const

boost::match_results<std::vector<char>::const_iterator>& what)
{
	m[std::string(what[5].first, what[5].second) + std::string(what[6].first, what[6].second)] = what[5].first - base;
	return true;
}

void test_grep_1(void)
{
	map_type map;
	std::string str_re("^[[:space:]]*"
						"(template[[:space:]]*<[^;:{]+>[[:space:]]*)?"
						"(class|struct)[[:space:]]*"
						"("
						"\\<\\w+\\>"
						"("
						"[[:blank:]]*\\([^)]*\\)"
						")?"
						"[[:space:]]*"
						")*"
						"(\\<\\w*\\>)[[:space:]]*"
						"(<[^;:{]+>)?[[:space:]]*"
						"(\\{|:[^;\\{()]*\\{)"
						);

	std::string text("template <typename T> \nclass A \n{\n}; template <typename T> \nclass B \n{\n};");

	try
	{
	std::cout << yggr::regular_parse::regex_parse::grep(
															boost::bind(&handler_grep1,
																			boost::ref(map),
																			text.begin(), _1),
															text, str_re
														) <<std::endl;
	}
	catch(yggr::regular_parse::regex_parse::error_type& e)
	{
		std::cout << e.what() << std::endl;
	}

	map_type::iterator isize = map.end();

	for(map_type::iterator i = map.begin(); i != isize; ++i)
	{
		std::cout << i->first << " : " << i->second << std::endl;
	}

}

void test_grep_2(void)
{
	map_type map;
	std::string str_re("^[[:space:]]*"
						"(template[[:space:]]*<[^;:{]+>[[:space:]]*)?"
						"(class|struct)[[:space:]]*"
						"("
						"\\<\\w+\\>"
						"("
						"[[:blank:]]*\\([^)]*\\)"
						")?"
						"[[:space:]]*"
						")*"
						"(\\<\\w*\\>)[[:space:]]*"
						"(<[^;:{]+>)?[[:space:]]*"
						"(\\{|:[^;\\{()]*\\{)"
						);

	std::string text("template <typename T> \nclass A \n{\n}; template <typename T> \nclass B \n{\n};");

	std::vector<char> cvt(text.size(), 0);
	memcpy(&cvt[0], &text[0], text.size());

	try
	{
		std::cout << yggr::regular_parse::regex_parse::grep(
													boost::bind(&handler_grep2,
																	boost::ref(map),
																	cvt.begin(), _1),
													cvt, str_re) <<std::endl;
	}
	catch(yggr::regular_parse::regex_parse::error_type& e)
	{
		std::cout << e.what() << std::endl;
	}

	map_type::iterator isize = map.end();

	for(map_type::iterator i = map.begin(); i != isize; ++i)
	{
		std::cout << i->first << " : " << i->second << std::endl;
	}

}

void test_replac_real(void)
{
	std::string str_re("(?<a_mark>a)|(?<b_mark>b)");
	std::string str_fmt("(?{a_mark}$+{b_mark})(?{b_mark}$+{a_mark})");
	std::string text("aaacccbbb");

	try
	{
		std::cout << yggr::regular_parse::regex_parse::replace(text, str_re, str_fmt) << std::endl;
	}
	catch(yggr::regular_parse::regex_parse::error_type& e)
	{
		std::cout << e.what() << std::endl;
	}
}


int main(int argc, char* argv[])
{
	test_match();
	test_match2();
	test_match3();
	test_match4();

	test_replace1();
	test_replace2();
	test_replace3();
	test_replace4();

	test_split1_1();
	test_split1_2();

	test_split2_1();
	test_split2_2();

	test_split3_1();
	test_split3_1_2();
	test_split3_2();

	test_split4_1();
	test_split4_2();

	test_grep_1();
	test_grep_2();
	test_replac_real();

	char cc = 0;
	std::cin >> cc;
	return 0;
}
