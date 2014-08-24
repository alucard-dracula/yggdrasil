//min_file_system_test.cpp
#include <iostream>
#include <string>
#include <fstream>
#include <yggr/file_system/ofile_mgr.hpp>
#include <yggr/file_system/ifile_mgr.hpp>

#include <yggr/packet/packet.hpp>
#include <yggr/archive/binary_archive_partner.hpp>
#include <sstream>

#ifdef _WIN32
#   include <vld.h>
#endif //_WIN32

#pragma comment(lib, "vld.lib")

typedef yggr::packet::packet<yggr::archive::archive_partner::binary_oarchive_partner> opak_type;
typedef yggr::packet::packet<yggr::archive::archive_partner::binary_iarchive_partner> ipak_type;
//
//typedef yggr::file_system::file_mgr<std::ofstream> ofile_mgr_type;
//typedef yggr::file_system::file_mgr<std::ifstream> ifile_mgr_type;
typedef yggr::file_system::ofile_mgr ofile_mgr_type;
typedef yggr::file_system::ifile_mgr ifile_mgr_type;

struct A
{
public:
	A(void)
		: n(0), f(0.0f)
	{
	}
	A(int a)
		: n(100), f(20.12345), str("abcdefg")
	{
	}

	~A(void)
	{
	}

private:
	friend class boost::serialization::access;

	template<typename Archive>
	void serialize(Archive& ar, yggr::u32 version)
	{
		ar & n;
		ar & str;
		ar & f;
	}

public:
	int n;
	std::string str;
	float f;
};

int file_size = 0;

void write(void)
{
	ofile_mgr_type ofmgr("abc.fsys");

	A a(1);
	opak_type opak;
	opak.save(a);

	for(int i = 0; i < file_size; ++i)
	{
//		if(i == 9)
//		{
//			int nnn = 0;
//		}
		std::stringstream ss;
		ss << i << ".bin";
		ofmgr.add_file(ss.str(), ofile_mgr_type::E_File_Binary, opak.org_buf());
	}

	ofmgr.save();

	ofile_mgr_type::file_list_type file_list;
	ofmgr.get_file_list(file_list);


	for(int i = 0; i < file_list.size(); ++i)
	{
		std::cout << file_list[i] << std::endl;
	}

	ofmgr.close();
}


void read(void)
{

	ifile_mgr_type ifmgr("abc.fsys", 100);

	for(int i = 0; i < file_size; ++i)
	{
		std::stringstream ss;
		ss << i << ".bin";

		ifile_mgr_type::buf_type buf;
		ifmgr.get_file(ss.str(), buf);
		ipak_type ipak(buf);
		A a;
		ipak.load(a);

		std::cout << a.n << "," << a.f << "," << a.str << std::endl;

	}

	ifile_mgr_type::file_list_type file_list;
	ifmgr.get_file_list(file_list);
	for(int i = 0; i < file_list.size(); ++i)
	{
		std::cout << file_list[i] << std::endl;
	}

	ifmgr.close();
}

int main(int argc, char* argv[])
{
	std::cin >> file_size;
	write();
	std::cout << "write end" << std::endl;
	char c = 0;
	std::cin >> c;
	read();
	std::cout << "read end" << std::endl;
	char cc = 0;
	std::cin >> cc;
	return 0;
}
