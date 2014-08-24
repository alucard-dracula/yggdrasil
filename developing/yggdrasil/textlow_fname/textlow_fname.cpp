//Runner.cpp

#include <yggr/file_system/local_file_operator.hpp>
#include <yggr/regular_parse/regex_parse.hpp>

typedef yggr::file_system::local_file_operator_type::path_list_type path_list;
typedef path_list::iterator path_list_iter;


void change_low(const std::string& path)
{
	std::string full_name = path;

	std::string dname;
	std::string fname;
	yggr::file_system::local_file_operator_type::parsing_file_path_and_name(full_name, dname, fname);

	std::string str_re("(A)|(B)|(C)|(D)|(E)|(F)|(G)|(H)|(I)|(J)|(K)|(L)|(M)|(N)|(O)|(P)|(Q)|(R)|(S)|(T)|(U)|(V)|(W)|(X)|(Y)|(Z)");
	std::string str_fmt("(?1a)(?2b)(?3c)(?4d)(?5e)(?6f)(?7g)(?8h)(?9i)(?10j)(?11k)(?12l)(?13m)(?14n)(?15o)(?16p)(?17q)(?18r)"
							"(?19s)(?20t)(?21u)(?22v)(?23w)(?24x)(?25y)(?26z)");

	//std::string str_re("(vcxproj)");
	//std::string str_fmt("(?1vcproj)");

	std::string new_fname(yggr::regular_parse::regex_parse::replace(fname, str_re, str_fmt));  

	if(new_fname == fname)
	{
		return;
	}

	std::string new_full_name = dname + new_fname;

	yggr::file_system::local_file_operator_type::rename_file(full_name, new_full_name);
};

void search_file(const std::string& path)
{
	path_list pl;
	yggr::file_system::local_file_operator_type::recursion_search_of_files(path, pl);

	for(path_list_iter p = pl.begin(); p != pl.end(); ++p)
	{		
		change_low(*p);
	}
};

int main(int argc, char* argv[])
{
	search_file("./");

	std::cout << "compleate" << std::endl;
	char cc = 0;
	std::cin >> cc;
	return 0;
}