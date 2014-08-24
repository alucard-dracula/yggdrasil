//files_merge.cpp

#include <yggr/base/yggrdef.h>
#include <iostream>
#include <yggr/file_system/ofile_mgr.hpp>
#include <yggr/file_system/ifile_mgr.hpp>
#include <yggr/file_system/local_file_operator.hpp>

int main(int argc, char* argv[])
{
	if(argc < 2)
	{
		return 0;
	}

	std::string dir_name = argv[1];

	//std::string dir_name;
	//std::cin >> dir_name;

	{
		typedef yggr::file_system::local_file_operator_type::path_list_type path_list_type;

		path_list_type list;
		yggr::file_system::local_file_operator_type::recursion_search_of_files(dir_name, list);

		yggr::file_system::ofile_mgr ofm("test_all_file.fsys");

		for(;!list.empty();)
		{
			std::string buf;
			yggr::file_system::local_file_operator_type::file_size_type size = 0;

			yggr::file_system::local_file_operator_type::read_file_of_binary(list.front(), buf, size);
			ofm.add_file(list.front(), yggr::file_system::ofile_mgr::E_File_Binary, buf);
			list.pop_front();
		}

		ofm.save();
		ofm.close();
	}

    //if(1)
	if(argc ==3 && !strcmp(argv[2], "-list"))
	{
		yggr::file_system::ifile_mgr::file_list_type file_list;
		yggr::file_system::ifile_mgr ifm("test_all_file.fsys");
		ifm.get_file_list(file_list);

		yggr::u64 size = file_list.size();

		for(yggr::u64 i = 0; i< size; ++i)
		{
			std::cout << file_list[i] << std::endl;
		}

		ifm.close();
	}

	/*char cc = 0;
	std::cin >> cc;*/
	return 0;
}
