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
	try
	{
		yggr::file_system::local_file_operator_type::write_file_of_binary("./a/b/c/a.bin", reinterpret_cast<const char*>(&n), sizeof(int));
	}
	catch(const boost::filesystem::filesystem_error& e)
	{
		std::cerr << e.what() << std::endl;
	}
	catch(const compatibility::stl_exception& e)
	{
		std::cerr << e.what() << std::endl;
	}

}

void write_test_text(void)
{
	int n = 100;
	bool bright = true;
	try
	{
		bright = yggr::file_system::local_file_operator_type::write_file_of_text("a.txt", n) && bright;
		bright = yggr::file_system::local_file_operator_type::append_file_of_text("a.txt", n) && bright;
	}
	catch(const compatibility::stl_exception& e)
	{
		std::cerr << e.what() << std::endl;
		bright = false;
	}
}

void read_test(void)
{
	int n = 0;
	yggr::size_type size = 0;
	std::string buf;

	try
	{
		yggr::file_system::local_file_operator_type::read_file_of_binary("./a/b/c/a.bin", buf, size);
	}
	catch(const compatibility::stl_exception& e)
	{
		std::cerr << e.what() << std::endl;
		return;
	}

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
	bool bright = false;

	try
	{
		bright = yggr::file_system::local_file_operator_type::read_file_of_text("a.txt", n);
	}
	catch(const compatibility::stl_exception& e)
	{
		std::cerr << e.what() << std::endl;
		return;
	}

	if(!bright)
	{
		return;
	}

	std::cout << "n = " << n << std::endl;
}

void create_dir_test(void)
{
	bool bright = false;
	try
	{
		bright = yggr::file_system::local_file_operator_type::create_directory("", "abc");
	}
	catch(const boost::filesystem::filesystem_error& e)
	{
		std::cerr << e.what() << std::endl;
	}
	catch(const compatibility::stl_exception& e)
	{
		std::cerr << e.what() << std::endl;
	}

	if(!bright)
	{
		std::cout << "create fail" << std::endl;
	}
}

void create_path_test(void)
{
	std::string full_path = "./a/b/c/d/e/f/g/h";
	bool bright = false;
	try
	{
		bright = yggr::file_system::local_file_operator_type::create_path(full_path);
	}
	catch(const boost::filesystem::filesystem_error& e)
	{
		std::cerr << e.what() << std::endl;
	}
	catch(const compatibility::stl_exception& e)
	{
		std::cerr << e.what() << std::endl;
	}

	if(!bright)
	{
		std::cout << "create fail" << std::endl;
	}
}

void rename_dir_test(void)
{
	bool bright = false;
	try
	{
		bright = yggr::file_system::local_file_operator_type::rename_directory("./a/b", "./a/c");
	}
	catch(const boost::filesystem::filesystem_error& e)
	{
		std::cerr << e.what() << std::endl;
	}
	catch(const compatibility::stl_exception& e)
	{
		std::cerr << e.what() << std::endl;
		return;
	}

	if(!bright)
	{
		std::cout << "rename fail" << std::endl;
	}
}

void remove_dir_test(void)
{
	bool bright = false;
	try
	{
		bright = yggr::file_system::local_file_operator_type::remove_directory("./a", true);
	}
	catch(const boost::filesystem::filesystem_error& e)
	{
		std::cerr << e.what() << std::endl;
	}
	catch(const compatibility::stl_exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
}

void copy_file_test(void)
{
	bool bright = false;
	try
	{
		bright = yggr::file_system::local_file_operator_type::copy_file("a.txt", "b.txt", true);
	}
	catch(const boost::filesystem::filesystem_error& e)
	{
		std::cerr << e.what() << std::endl;
	}
	catch(const compatibility::stl_exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
}

void remove_file_test(void)
{
	bool bright = false;
	try
	{
		bright = yggr::file_system::local_file_operator_type::remove_file("a.txt");
	}
	catch(const boost::filesystem::filesystem_error& e)
	{
		std::cerr << e.what() << std::endl;
		return;
	}
	catch(const compatibility::stl_exception& e)
	{
		std::cerr << e.what() << std::endl;
		return;
	}

	if(!bright)
	{
		std::cout << "remove_file_failed" << std::endl;
	}
}

void cut_file_test(void)
{
	bool bright = false;
	try
	{
		bright = yggr::file_system::local_file_operator_type::cut_file("b.txt", "../c.txt");
	}
	catch(const boost::filesystem::filesystem_error& e)
	{
		std::cerr << e.what() << std::endl;
		return;
	}
	catch(const compatibility::stl_exception& e)
	{
		std::cerr << e.what() << std::endl;
		return;
	}

	if(!bright)
	{
		std::cout << "cut_failed" << std::endl;
	}
}

void rename_file_test(void)
{
	bool bright = false;
	try
	{
		bright = yggr::file_system::local_file_operator_type::rename_file("c.txt", "b.txt");
	}
	catch(const boost::filesystem::filesystem_error& e)
	{
		std::cerr << e.what() << std::endl;
	}
	catch(const compatibility::stl_exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
}


void search_file_test(void)
{
	typedef yggr::file_system::local_file_operator_type::path_list_type::iterator iter_type;
	
	yggr::file_system::local_file_operator_type::path_list_type list;
	try
	{
		yggr::file_system::local_file_operator_type::search_files(".\\", list);
	}
	catch(const compatibility::stl_exception& e)
	{
		std::cerr << e.what() << std::endl;
		return;
	}

	for(iter_type i = list.begin(), isize = list.end(); i != isize; ++i)
	{
		std::cout << *i << std::endl;
	}


	list.clear();
}

void search_dir_test(void)
{
	typedef yggr::file_system::local_file_operator_type::path_list_type::iterator iter_type;
	
	yggr::file_system::local_file_operator_type::path_list_type list;
	
	try
	{
		yggr::file_system::local_file_operator_type::search_directorys(".\\", list);
	}
	catch(const compatibility::stl_exception& e)
	{
		std::cerr << e.what() << std::endl;
		return;
	}

	for(iter_type i = list.begin(), isize = list.end(); i != isize; ++i)
	{
		std::cout << *i << std::endl;
	}

	list.clear();
}

void search_all_type_test(void)
{
	typedef yggr::file_system::local_file_operator_type::path_list_type::iterator iter_type;
	
	yggr::file_system::local_file_operator_type::path_list_type list;

	try
	{
		yggr::file_system::local_file_operator_type::search_all_type(".\\", list);
	}
	catch(const compatibility::stl_exception& e)
	{
		std::cerr << e.what() << std::endl;
		return;
	}

	for(iter_type i = list.begin(), isize = list.end(); i != isize; ++i)
	{
		std::cout << *i << std::endl;
	}

	list.clear();
}

void recursion_search_of_files_test(void)
{
	typedef yggr::file_system::local_file_operator_type::path_list_type::iterator iter_type;
	
	yggr::file_system::local_file_operator_type::path_list_type list;

	try
	{
		yggr::file_system::local_file_operator_type::recursion_search_of_files("", list);
	}
	catch(const compatibility::stl_exception& e)
	{
		std::cerr << e.what() << std::endl;
		return;
	}

	iter_type isize = list.end();

	for(iter_type i = list.begin(); i != isize; ++i)
	{
		std::cout << *i << std::endl;
	}

	list.clear();
}

void recursion_search_of_dirs_test(void)
{
	typedef yggr::file_system::local_file_operator_type::path_list_type::iterator iter_type;
	
	yggr::file_system::local_file_operator_type::path_list_type list;
	
	try
	{
		yggr::file_system::local_file_operator_type::recursion_search_of_directorys("", list);
	}
	catch(const compatibility::stl_exception& e)
	{
		std::cerr << e.what() << std::endl;
		list.clear();
	}

	for(iter_type i = list.begin(), isize = list.end(); i != isize; ++i)
	{
		std::cout << *i << std::endl;
	}

	list.clear();
}

void recursion_search_of_all_type_test(void)
{
	typedef yggr::file_system::local_file_operator_type::path_list_type::iterator iter_type;
	
	yggr::file_system::local_file_operator_type::path_list_type list;
	
	try
	{
		yggr::file_system::local_file_operator_type::recursion_search_of_all_type("", list);
	}
	catch(const compatibility::stl_exception& e)
	{
		std::cerr << e.what() << std::endl;
		list.clear();
	}

	for(iter_type i = list.begin(), isize = list.end(); i != isize; ++i)
	{
		std::cout << *i << std::endl;
	}

	list.clear();
}


void create_file_of_binary_test(void)
{
	bool bright = false;
	try
	{
		bright = yggr::file_system::local_file_operator_type::create_file_of_binary("test.bin", 65536);
	}
	catch(const boost::filesystem::filesystem_error& e) 
	{
		std::cerr << e.what() << std::endl;
	}
	catch(const compatibility::stl_exception& e)
	{
		std::cerr << e.what() << std::endl;
	}

	if(!bright)
	{
		std::cerr << "create_file_of_binary_test failed" << std::endl; 
	}
}

void current_path_test(void)
{
	std::string str;
	try
	{
		str =  yggr::file_system::local_file_operator_type::current_path<std::string>();
	}
	catch(const boost::filesystem::filesystem_error& e)
	{
		std::cerr << e.what() << std::endl;
	}
	catch(const compatibility::stl_exception& e)
	{
		std::cerr << e.what() << std::endl;
	}

	std::cout << str << std::endl;
}

void create_hard_link_test(void)
{
	std::string src = "a.data";
	std::string dst = "a.hlnk";

	bool bright = false;
	yggr::u64 fsize = 0;
	try
	{
		if(!yggr::file_system::local_file_operator_type::is_exists_path(src))
		{
			fsize = yggr::file_system::local_file_operator_type::create_file_of_binary(src, 1024);
			assert(fsize);
		}

		if(yggr::file_system::local_file_operator_type::is_exists_path(dst))
		{
			 yggr::file_system::local_file_operator_type::remove_file(dst);
		}

		bright = yggr::file_system::local_file_operator_type::create_hard_link(src, dst);
	}
	catch(const boost::filesystem::filesystem_error& e)
	{
		std::cerr << e.what() << std::endl;
	}
	catch(const compatibility::stl_exception& e)
	{
		std::cerr << e.what() << std::endl;
	}

	assert(bright);
}

void hard_link_count_test(void)
{
	std::string lname = "a.hlnk";
	yggr::u64 size = 0;
	try
	{
		size = yggr::file_system::local_file_operator_type::hard_link_count(lname);
	}
	catch(const boost::filesystem::filesystem_error& e)
	{
		std::cerr << e.what() << std::endl;
	}
	catch(const compatibility::stl_exception& e)
	{
		std::cerr << e.what() << std::endl;
	}

	assert(size);
	std::cout << "hard_link_count = " << size << std::endl;
}

void create_symlink_test(void) // need admin
{
	std::string src = "a.data";
	std::string dst = "a.slnk";

	bool bright = false;
	yggr::u64 fsize = 0;
	try
	{
		if(!yggr::file_system::local_file_operator_type::is_exists_path(src))
		{
			fsize = yggr::file_system::local_file_operator_type::create_file_of_binary(src, 1024);
			assert(fsize);
		}
		bright = yggr::file_system::local_file_operator_type::create_symlink(src, dst);
	}
	catch(const boost::filesystem::filesystem_error& e)
	{
		std::cerr << e.what() << std::endl;
	}
	catch(const compatibility::stl_exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
}

void copy_symlink_test(void) // need admin
{
	std::string src = "a.slnk";
	std::string dst = "b.slnk";

	bool bright = false;
	try
	{
		bright = yggr::file_system::local_file_operator_type::copy_symlink(src, dst);
	}
	catch(const boost::filesystem::filesystem_error& e)
	{
		std::cerr << e.what() << std::endl;
	}
	catch(const compatibility::stl_exception& e)
	{
		std::cerr << e.what() << std::endl;
	}

	assert(bright);
}

void is_directory_test(void)
{
	std::string fname = "a.data";
	std::string dir_name = "time";
	assert(!yggr::file_system::local_file_operator_type::is_directory(fname));
	assert(yggr::file_system::local_file_operator_type::is_directory(dir_name));
}

void clear(void)
{
	try
	{
		yggr::file_system::local_file_operator_type::remove_directory("./a/b/c/a.bin", true);
		yggr::file_system::local_file_operator_type::remove_file("a.txt");
		yggr::file_system::local_file_operator_type::remove_directory("abc", true);
		yggr::file_system::local_file_operator_type::remove_file("b.txt");
		yggr::file_system::local_file_operator_type::remove_file("../c.txt");
		yggr::file_system::local_file_operator_type::remove_file("test.bin");
		yggr::file_system::local_file_operator_type::remove_file("a.data");
		yggr::file_system::local_file_operator_type::remove_file("a.hlnk");
		//yggr::file_system::local_file_operator_type::remove_file("a.slnk");
		//yggr::file_system::local_file_operator_type::remove_file("b.slnk");
	}
	catch(const boost::filesystem::filesystem_error& e)
	{
		std::cerr << e.what() << std::endl;
	}
	catch(const compatibility::stl_exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
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

	current_path_test();

	create_hard_link_test();

	hard_link_count_test();

	//create_symlink_test(); // need admin

	//copy_symlink_test(); // need admin

	is_directory_test();

	clear();

	char cc = 0;
	std::cin >> cc;
	return 0;
}
