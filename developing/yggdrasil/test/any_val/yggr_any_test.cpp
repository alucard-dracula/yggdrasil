//yggr_any_test.cpp

#include <iostream>
#include <cassert>
#include <yggr/any_val/any.hpp>

#ifdef _MSC_VER
#include <vld.h>
#endif // _MSC_VER

int main(int argc, char* argv[])
{
	{
		boost::any any(10);
		assert(10 == yggr::any_cast<int>(any));
	}
	{
		yggr::any any(10);

		yggr::any any2(any);
		assert(10 == yggr::any_cast<int>(any));
		assert(10 == yggr::any_cast<int>(any2));

		assert(!yggr::any_cast<float>(&any));
		assert(yggr::any_cast<int>(&any));

		const yggr::any& cref_any = any; 
		assert(yggr::any_cast<int>(cref_any) == 10);
		assert(!yggr::any_cast<float>(&cref_any));
		assert(yggr::any_cast<int>(&cref_any));
	}

	std::cout << "complete" << std::endl;
	char cc = 0;
	std::cin >> cc;
	return 0;
}