//local_file_operator_test.cpp

#ifdef _MSC_VER
#	include <vld.h>
#endif //_DEBUG

#include <yggr/file_system/local_file_operator.hpp>

void parsing_test(void)
{
	yggr::file_system::local_file_operator_type::path_list_type path;
	typedef yggr::file_system::local_file_operator_type::path_list_type::iterator iter_type;
	std::string name;

	std::string full_name("./yggr_config/server/svr.cfg");

	yggr::file_system::local_file_operator_type::parsing_file_path_and_name(full_name, path, name);
	iter_type isize = path.end();
	for(iter_type i = path.begin(); i != isize; ++i)
	{
		std::cout << *i << std::endl;
	}

	std::cout << name << std::endl;
	path.clear();
}

void parsing_text_test(void)
{
	std::string path;
	std::string name;

	std::string full_name("./yggr_config/server/svr.cfg");

	yggr::file_system::local_file_operator_type::parsing_file_path_and_name(full_name, path, name);
	std::cout << path << std::endl;
	std::cout << name << std::endl;
}

void parsing_path_test(void)
{
	yggr::file_system::local_file_operator_type::path_list_type path;
	typedef yggr::file_system::local_file_operator_type::path_list_type::iterator iter_type;
	std::string name;

	std::string full_name("./yggr_config/server/svr.cfg/");

	yggr::file_system::local_file_operator_type::parsing_file_path_and_name(full_name, path, name);
	iter_type isize = path.end();
	for(iter_type i = path.begin(); i != isize; ++i)
	{
		std::cout << *i << std::endl;
	}

	std::cout << name << std::endl;
	path.clear();
}

void parsing_path_only_test(void)
{
	yggr::file_system::local_file_operator_type::path_list_type path;
	typedef yggr::file_system::local_file_operator_type::path_list_type::iterator iter_type;
	std::string name;

	std::string full_name("./yggr_config/server/svr.cfg");

	yggr::file_system::local_file_operator_type::parsing_path(full_name, path);
	iter_type isize = path.end();
	for(iter_type i = path.begin(); i != isize; ++i)
	{
		std::cout << *i << std::endl;
	}
	path.clear();
}



void write_test(void)
{
	int n = 100;
	yggr::file_system::local_file_operator_type::write_file_of_binary("./a/b/c/a.bin", reinterpret_cast<const char*>(&n), sizeof(int));

}

void write_test_text(void)
{
	int n = 100;
	yggr::file_system::local_file_operator_type::write_file_of_text("a.txt", n);
	yggr::file_system::local_file_operator_type::append_file_of_text("a.txt", n);
}

void read_test(void)
{
	int n = 0;
	yggr::size_type size = 0;
	std::string buf;
	yggr::file_system::local_file_operator_type::read_file_of_binary("./a/b/c/a.bin", buf, size);

	if(!size)
	{
		return;
	}

	std::copy(buf.begin(), buf.end(), reinterpret_cast<char*>(&n));
	std::cout << "n = " << n << std::endl;
}


void read_test_text(void)
{
	int n = 0;


	if(!yggr::file_system::local_file_operator_type::read_file_of_text("a.txt", n))
	{
		return;
	}

	std::cout << "n = " << n << std::endl;
}

void create_dir_test(void)
{
	if(!yggr::file_system::local_file_operator_type::create_directory("", "abc"))
	{
		std::cout << "create fail" << std::endl;
	}
}

void create_path_test(void)
{
	std::string full_path = "./a/b/c/d/e/f/g/h";
	if(!yggr::file_system::local_file_operator_type::create_path(full_path))
	{
		std::cout << "create fail" << std::endl;
	}
}

void rename_dir_test(void)
{
	if(!yggr::file_system::local_file_operator_type::rename_directory("./a/b", "./a/c"))
	{
		std::cout << "rename fail" << std::endl;
	}
}

void remove_dir_test(void)
{
	yggr::file_system::local_file_operator_type::remove_directory("./a", true);
}

void copy_file_test(void)
{
	yggr::file_system::local_file_operator_type::copy_file("a.txt", "b.txt", true);
}

void remove_file_test(void)
{
	if(!yggr::file_system::local_file_operator_type::remove_file("a.txt"))
	{
		std::cout << "remove_file_failed" << std::endl;
	}
}

void cut_file_test(void)
{
	if(!yggr::file_system::local_file_operator_type::cut_file("b.txt", "../c.txt"))
	{
		std::cout << "cut_failed" << std::endl;
	}
}

void rename_file_test(void)
{
	yggr::file_system::local_file_operator_type::rename_file("c.txt", "b.txt");
}

void search_file_test(void)
{
	yggr::file_system::local_file_operator_type::path_list_type list;
	yggr::file_system::local_file_operator_type::search_files(".\\", list);

	typedef yggr::file_system::local_file_operator_type::path_list_type::iterator iter_type;

	iter_type isize = list.end();

	for(iter_type i = list.begin(); i!= isize; ++i)
	{
		std::cout << *i << std::endl;
	}


	list.clear();
}

void search_dir_test(void)
{
	yggr::file_system::local_file_operator_type::path_list_type list;
	yggr::file_system::local_file_operator_type::search_directorys(".\\", list);

	typedef yggr::file_system::local_file_operator_type::path_list_type::iterator iter_type;

	iter_type isize = list.end();

	for(iter_type i = list.begin(); i!= isize; ++i)
	{
		std::cout << *i << std::endl;
	}

	list.clear();
}

void search_all_type_test(void)
{
	yggr::file_system::local_file_operator_type::path_list_type list;
	yggr::file_system::local_file_operator_type::search_all_type(".\\", list);

	typedef yggr::file_system::local_file_operator_type::path_list_type::iterator iter_type;

	iter_type isize = list.end();

	for(iter_type i = list.begin(); i!= isize; ++i)
	{
		std::cout << *i << std::endl;
	}

	list.clear();
}

void recursion_search_of_files_test(void)
{
	yggr::file_system::local_file_operator_type::path_list_type list;
	yggr::file_system::local_file_operator_type::recursion_search_of_files("", list);

	typedef yggr::file_system::local_file_operator_type::path_list_type::iterator iter_type;

	iter_type isize = list.end();

	for(iter_type i = list.begin(); i!= isize; ++i)
	{
		std::cout << *i << std::endl;
	}

	list.clear();
}

void recursion_search_of_dirs_test(void)
{
	yggr::file_system::local_file_operator_type::path_list_type list;
	yggr::file_system::local_file_operator_type::recursion_search_of_directorys("", list);

	typedef yggr::file_system::local_file_operator_type::path_list_type::iterator iter_type;

	iter_type isize = list.end();

	for(iter_type i = list.begin(); i!= isize; ++i)
	{
		std::cout << *i << std::endl;
	}

	list.clear();
}

void recursion_search_of_all_type_test(void)
{
	yggr::file_system::local_file_operator_type::path_list_type list;
	yggr::file_system::local_file_operator_type::recursion_search_of_all_type("", list);

	typedef yggr::file_system::local_file_operator_type::path_list_type::iterator iter_type;

	iter_type isize = list.end();

	for(iter_type i = list.begin(); i!= isize; ++i)
	{
		std::cout << *i << std::endl;
	}

	list.clear();
}


void create_file_of_binary_test(void)
{
	yggr::file_system::local_file_operator_type::create_file_of_binary("test.bin", 65536);
}

int main(int argc, char* argv[])
{
	parsing_test();

	write_test();

	write_test_text();

	read_test();

	read_test_text();

	create_dir_test();

	parsing_path_test();

	parsing_path_only_test();

	create_path_test();

	parsing_text_test();

	rename_dir_test();

	remove_dir_test();

	copy_file_test();

	remove_file_test();

	cut_file_test();

	rename_file_test();

	search_file_test();

	search_dir_test();

	search_all_type_test();

	recursion_search_of_files_test();

	recursion_search_of_dirs_test();

	recursion_search_of_all_type_test();

	create_file_of_binary_test();

	char cc = 0;
	std::cin >> cc;
	return 0;
}
