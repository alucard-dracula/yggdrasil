//Runner.cpp

#include <yggr/file_system/local_file_operator.hpp>
#include <yggr/regular_parse/regex_parse.hpp>

typedef yggr::file_system::local_file_operator_type::path_list_type path_list;
typedef path_list::iterator path_list_iter;


void change_low(const std::string& path)
{
	std::string fname = path;
	std::string buf;
	yggr::file_system::local_file_operator_type::file_size_type size = 0;

	try
	{
		yggr::file_system::local_file_operator_type::read_file_of_binary(fname, buf, size);
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

	std::string str_re("(A)|(B)|(C)|(D)|(E)|(F)|(G)|(H)|(I)|(J)|(K)|(L)|(M)|(N)|(O)|(P)|(Q)|(R)|(S)|(T)|(U)|(V)|(W)|(X)|(Y)|(Z)");
	std::string str_fmt("(?1a)(?2b)(?3c)(?4d)(?5e)(?6f)(?7g)(?8h)(?9i)(?10j)(?11k)(?12l)(?13m)(?14n)(?15o)(?16p)(?17q)(?18r)"
						"(?19s)(?20t)(?21u)(?22v)(?23w)(?24x)(?25y)(?26z)");
	std::string new_buf(yggr::regular_parse::regex_parse::replace(buf, str_re, str_fmt));

	try
	{
		yggr::file_system::local_file_operator_type::write_file_of_binary(fname, new_buf.c_str(), size);
	}
	catch(const boost::filesystem::filesystem_error& e)
	{
		std::cerr << e.what() << std::endl;
	}
	catch(const compatibility::stl_exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
};


bool is_hit_file(const std::string& fname, const std::string& ftype)
{
	return (fname.substr(fname.length() - 4, 4) == ftype);
}

void search_file(const std::string& path, const std::string& ftype)
{
	path_list pl;
	try
	{
		yggr::file_system::local_file_operator_type::recursion_search_of_files(path, pl);
	}
	catch(const compatibility::stl_exception& e)
	{
		std::cerr << e.what() << std::endl;
		pl.clear();
	}

	for(path_list_iter p = pl.begin(); p != pl.end(); ++p)
	{
		if(!is_hit_file(*p, ftype))
		{
			continue;
		}
		
		change_low(*p);
	}
};

int main(int argc, char* argv[])
{
	std::string tftype;
	std::cin >> tftype;
	std::string ftype = std::string(".") + tftype;

	search_file("./", ftype);

	std::cout << "compleate" << std::endl;
	char cc = 0;
	std::cin >> cc;
	return 0;
}