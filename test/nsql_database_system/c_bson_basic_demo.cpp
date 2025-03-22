//c_bson_basic_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <yggr/nsql_database_system/c_bson.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(nsql_database_system)
#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(base)

void deom(void)
{
	typedef yggr::nsql_database_system::c_bson bson_type;
	typedef bson_type::iterator bson_iter_type;

	yggr::nsql_database_system::bson_mark_splice mark;
	bson_type bs;
	bs.save("name", "uid");
	bs.save_document_start("seq", mark);
		bs.save("$gt", 1);
	bs.save_document_end(mark);
	bs.save_document_start("expries", mark);
		bs.save("$gt", 2);
	bs.save_document_end(mark);

	std::cout << bs << std::endl;
}

void deom2(void)
{
	typedef yggr::nsql_database_system::c_bson bson_type;
	typedef bson_type::iterator bson_iter_type;

	bson_type bs;
	bs.save("name", "uid");
	bs.save("seq", bson_type("{\"$gt\" : 1 }"));
	bs.save("expries", bson_type("{\"$gt\" : 2 }"));

	std::cout << bs << std::endl;
}

int main(int argc, char* argv[])
{
	deom();
	deom2();

	wait_any_key(argc, argv);
	return 0;
}