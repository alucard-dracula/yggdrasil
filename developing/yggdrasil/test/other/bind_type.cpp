//bind_type.cpp

#include<iostream>
#include<boost/bind.hpp>
#include<boost/any.hpp>

#include <yggr/archive/network_archive_partner.hpp>
#include <yggr/packet/packet.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

void out(void)
{
}

class A
{
public:
	bool out(int n)
	{
		return true;
	}
};

void test1(void)
{
	A obj;
	boost::any a(boost::bind(&A::out, &obj, 12));

	std::cout << a.type().name() << std::endl;
	
	std::cout << boost::bind(&A::out, &obj, 12)() << std::endl;
}

void test2(void)
{
	yggr::packet::packet< yggr::archive::archive_partner::network_oarchive_partner> pak;

	std::cout <<typeid(boost::bind(&yggr::packet::packet< yggr::archive::archive_partner::network_oarchive_partner>::save<int>,
		&pak, _1)).name() << std::endl;
}

int main(int argc, char* argv[])
{
	test1();
	test2();
	
	char cc = 0;
	std::cin >> cc;
	return 0;
}
