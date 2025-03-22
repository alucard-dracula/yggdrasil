//regex_parse_search_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <vector>

#include <yggr/regular_parse/regex_parse.hpp>
#include <yggr/file_system/local_fsys_operators.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(charset)

template<typename Container, typename Iter>
bool handler_split(Container& cont, const boost::sub_match<Iter>& m)
{
	typedef Container cont_type;
	typedef typename boost::range_value<cont_type>::type cont_value_type;

	if(m.matched)
	{
		std::inserter(cont, boost::end(cont)) = cont_value_type(m.first, m.second);
	}
	return true;
}

int main(int argc, char* argv[])
{
	std::string buf;
	yggr::size_type size;

	try
	{
		size = yggr::file_system::local_fsys_operators::read_file_of_binary("./regex/string_list.txt", buf); // load file
	}
	catch(const ::yggr::stl_exception& e)
	{
		std::cerr << e.what() << std::endl;
		return -1;
	}

	yggr_test_assert(size);

	std::vector<std::string> strvt;
	std::string sp_str("\\r\\n");
	yggr::u32 sp_count = 0;

	try
	{
		sp_count = 
			yggr::regular_parse::regex_parse::split(
				boost::bind(
					&handler_split<std::vector<std::string>, std::string::const_iterator>, 
					boost::ref(strvt), _1),
				buf,
				yggr::regular_parse::regex_parse::make_regex(sp_str));
	}
	catch(const yggr::regular_parse::regex_parse::error_type& e)
	{
		std::cout << e.what() << std::endl;
		return -1;
	}

	yggr_test_assert(sp_count == strvt.size());
	for(yggr::size_type i = 0; i != strvt.size(); ++i)
	{
		std::cout << strvt[i] << std::endl;
	}

	wait_any_key(argc, argv);
	return 0;
}
