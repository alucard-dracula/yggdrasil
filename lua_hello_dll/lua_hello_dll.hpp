//lua_hello_dll.hpp

#ifndef __LUA_HELLO_DLL_HPP__
#define __LUA_HELLO_DLL_HPP__

#include <yggr/base/yggrdef.h>
#include <iostream>
#include <list>

class A
{
public:
	typedef yggr::u32 use_val_type;
	typedef yggr::u64 val_type;
public:
	A(void)
		: n(0)
	{
	}

	A(use_val_type nn)
		: n(nn)
	{
	}

	/*A(val_type nn)
		: n(nn)
	{
	}*/

	A(const A& right)
		: n(right.n)
	{
	}

	void print(void) const
	{
		std::cout << n << std::endl;
	}

	const A operator+(use_val_type tn) const
	{
		return A(n + tn);
	}

	void test_u64(yggr::s64 n)
	{
		std::cout << n << std::endl;
	}

	val_type n;
};

std::ostream& operator<<(std::ostream& os, const A& oa);

typedef std::list<int> int_list_type;

//class Ex_Test
//{
//public:
//	Ex_Test(void)
//	{
//	}
//
//	~Ex_Test(void)
//	{
//	}
//};

#endif //__LUA_HELLO_DLL_HPP__
