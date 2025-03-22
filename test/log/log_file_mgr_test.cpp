//log_file_mgr_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>

#include <fstream>
#include <string>
#include <sstream>

#include <yggr/container/vector.hpp>
#include <yggr/container/map.hpp>

#include <yggr/log/log_file.hpp>
#include <yggr/log/log_thread_file.hpp>
#include <yggr/log/log_file_mgr.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(log)
#include YGGR_PP_LINK_LIB(time)
#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(base)

struct A
{
	A(void)
		:a(100), f(123.5f), str("Hello")
	{
	}

	inline const std::string to_string(void) const
	{
		std::stringstream ss;
		ss << a << "," << f << "," << str;
		return ss.str();
	}

	int a;
	float f;
	std::string str;

};

template<typename Char, typename Traits>
std::basic_ostream<Char, Traits>& 
	operator<<(std::basic_ostream<Char, Traits>& os, const A& a)
{
	os << a.to_string();
	return os;
}

void test_of_log_file1(void)
{
	typedef std::ofstream ostream_type;

	typedef yggr::log::log_op::log_file<ostream_type> o_log_file_type;
	typedef yggr::log::log_op::log_file_mgr<o_log_file_type> log_file_mgr_type;
	typedef log_file_mgr_type::init_type init_type;

	init_type init;
	init.push_back("../test_out/log/lfm_test1.txt");
	init.push_back("../test_out/log/lfm_test2.txt");
	init.push_back("../test_out/log/lfm_test3.txt");
	init.push_back("../test_out/log/lfm_test4.txt");
	A a;

	log_file_mgr_type lfm(init);

	for(init_type::const_iterator i = init.begin(), isize = init.end(); i != isize; ++i)
	{
		lfm.append(*i, a);
		lfm.append(*i, a);
		lfm.append(*i, a);
		lfm.append(*i, a);
		lfm.clear((*i));
	}

	lfm.append_all(a);
	//lfm.clear_all();
}

void test_of_log_file2(void)
{
	typedef std::ofstream ostream_type;

	typedef yggr::log::log_op::log_file<ostream_type> o_log_file_type;
	typedef yggr::log::log_op::log_file_mgr<o_log_file_type> log_file_mgr_type;

	std::vector<std::string> init;
	init.push_back("../test_out/log/lfm_test1.txt");
	init.push_back("../test_out/log/lfm_test2.txt");
	init.push_back("../test_out/log/lfm_test3.txt");
	init.push_back("../test_out/log/lfm_test4.txt");
	A a;

	log_file_mgr_type lfm(init);

	for(std::vector<std::string>::const_iterator i = init.begin(), isize = init.end(); i != isize; ++i)
	{
		lfm.append(*i, a);
		lfm.append(*i, a);
		lfm.append(*i, a);
		lfm.append(*i, a);
		lfm.clear(*i);
	}

	lfm.append_all(a);
	lfm.clear_all();
}

void test_of_log_file3(void)
{
	typedef std::ofstream ostream_type;

	typedef yggr::log::log_op::log_file<ostream_type> o_log_file_type;
	typedef yggr::log::log_op::log_file_mgr<o_log_file_type> log_file_mgr_type;

	std::map<int, std::string> init;

	init[1] = ("../test_out/log/lfm_test1.txt");
	init[2] = ("../test_out/log/lfm_test2.txt");
	init[3] = ("../test_out/log/lfm_test3.txt");
	init[4] = ("../test_out/log/lfm_test4.txt");
	
	A a;

	log_file_mgr_type lfm(init);

	for(std::map<int, std::string>::const_iterator i = init.begin(), isize = init.end(); i != isize; ++i)
	{
		lfm.append(i->second, a);
		lfm.append(i->second, a);
		lfm.append(i->second, a, true);
		lfm.append(i->second, a);
		//lfm.clear(i->second);
	}

	lfm.append_all(a);
	//lfm.clear_all();
}

// log_thread_file
template<yggr::u64 tid>
struct TA
	: public A
{
	TA(void)
	{
	}

	yggr::u64 thread_id(void) const
	{
		return tid;
	}
};

//this foo may not be needed
template<typename Char, typename Traits, yggr::u64 tid>
std::basic_ostream<Char, Traits>& 
	operator<<(std::basic_ostream<Char, Traits>& os, const TA<tid>& a)
{
	os << a.to_string();
	return os;
}

void test_of_log_thread_file(void)
{
	typedef std::ofstream ostream_type;

	typedef yggr::log::log_op::log_thread_file<ostream_type, yggr::u64> o_log_file_type;
	typedef yggr::log::log_op::log_file_mgr<o_log_file_type> log_file_mgr_type;

	TA<10> ta10;
	TA<20> ta20;

	std::map<int, std::string> init;

	init[1] = ("../test_out/log/ltfm_test1.txt");
	init[2] = ("../test_out/log/ltfm_test2.txt");
	init[3] = ("../test_out/log/ltfm_test3.txt");
	init[4] = ("../test_out/log/ltfm_test4.txt");

	log_file_mgr_type lfm(init);

	for(std::map<int, std::string>::const_iterator i = init.begin(), isize = init.end(); i != isize; ++i)
	{
		lfm.append(i->second, ta10);
		lfm.append(i->second, ta10);
		lfm.append(i->second, ta20);
		lfm.append(i->second, ta20, true);
		lfm.clear(i->second, yggr::u64(10));
	}

	lfm.append_all(ta10);
	lfm.append_all(ta20);
	
	lfm.clear_all(yggr::u64(10));
}

int main(int argc, char* argv[])
{
	test_of_log_file1();
	test_of_log_file2();
	test_of_log_file3();

	test_of_log_thread_file();

	std::cout << "result in test_out dir" << std::endl;
	
	wait_any_key(argc, argv);
	return 0;
}
