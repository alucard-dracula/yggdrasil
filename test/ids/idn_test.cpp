//idn_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <cassert>

#include <yggr/ids/id_n.hpp>
#include <yggr/ids/id_generator.hpp>
#include <yggr/container/list.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(ids)
#include YGGR_PP_LINK_LIB(base)

#define LEN 8
typedef yggr::ids::id_n<LEN> id_type;
typedef yggr::ids::id_generator<id_type> id_gen_type;

int main(int argc, char* argv[])
{
	id_gen_type gen;
	//yggr::u8 tmp_arr[LEN] = {0x0f, 0x00, 0x35, 0x00, 0x63, 0x1c, 0x7f, 0x7f};
	//id_type id1(tmp_arr, tmp_arr + 8);
	id_type id1 = gen();
	std::cout << "id1 = " << id1 << std::endl;
	yggr_test_assert(id1.size() == LEN);

	id_type id2(id1);
	std::cout << "id2 = " << id2 << std::endl;

	yggr_test_assert(id1 == id2);
	yggr_test_assert(!(id1 != id2));
	yggr_test_assert(!(id1 < id2));
	yggr_test_assert(!(id1 > id2));
	yggr_test_assert((id1 <= id2));
	yggr_test_assert((id1 >= id2));

	id_type::string_type str_id3 = id2;

	for(std::size_t i = str_id3.size(), isize = 0; i != isize; --i)
	{
		if(static_cast<yggr::u8>(str_id3[i - 1]) != 0xff)
		{
			str_id3[str_id3.size() - 1] += 1;
			break;
		}
	}

	id_type id3(str_id3);
	std::cout << "id3 = " << id3 << std::endl;

	yggr_test_assert(!(id1 == id3));
	yggr_test_assert((id1 != id3));

	yggr_test_assert((id1 < id3));
	yggr_test_assert((id1 <= id3));

	yggr_test_assert(!(id1 > id3));
	yggr_test_assert(!(id1 >= id3));

	yggr_test_assert(!(id1 == str_id3));
	yggr_test_assert((id1 != str_id3));

	//int nnn = id1.compare(str_id3);

	yggr_test_assert((id1 < str_id3));
	yggr_test_assert((id1 <= str_id3));

	yggr_test_assert(!(id1 > str_id3));
	yggr_test_assert(!(id1 >= str_id3));

	std::list<yggr::u8> list_id3(str_id3.begin(), str_id3.end());

	yggr_test_assert(!(id1 == list_id3));
	yggr_test_assert((id1 != list_id3));

	yggr_test_assert((id1 < list_id3));
	yggr_test_assert((id1 <= list_id3));

	yggr_test_assert(!(id1 > list_id3));
	yggr_test_assert(!(id1 >= list_id3));

	wait_any_key(argc, argv);
	return 0;
}
