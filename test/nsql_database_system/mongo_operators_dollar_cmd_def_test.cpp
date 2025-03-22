// mongo_operators_dollar_cmd_def_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <yggr/nsql_database_system/mongo_operators_dollar_cmd_def.hpp>
//#include <yggr/ppex/pp_debug.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(base)

int main(int argc, char* argv[])
{

	//std::cout << YGGR_PP_DEBUG(YGGR_TMP_PP_MAKE_FUNCTION(dollar_eq, "$eq")) << std::endl;
	typedef yggr::nsql_database_system::mongo_operators_dollar_cmd_def cmd_def_type;

	{
		const yggr::utf8_string& str = cmd_def_type::dollar_eq();
		yggr_test_assert((str == "$eq"));
	}

	{
		const boost::container::string& str = cmd_def_type::dollar_eq<boost::container::string>();
		yggr_test_assert((str == "$eq"));
	}

	{
		const std::string& str = cmd_def_type::dollar_eq<std::string>();
		yggr_test_assert((str == "$eq"));
	}

	{
		const boost::container::wstring& str = cmd_def_type::dollar_eq<boost::container::wstring>();
		yggr_test_assert((str == L"$eq"));
	}

	{
		const std::wstring& str = cmd_def_type::dollar_eq<std::wstring>();
		yggr_test_assert((str == L"$eq"));
	}

	wait_any_key(argc, argv);
	return 0;
}