//template_repaceor_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>

#include <yggr/container/vector.hpp>
#include <yggr/mplex/typename_replaceor.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

void test1(void)
{
	{
		typedef std::vector<char>::allocator_type char_alloc_type;
		typedef std::vector<int>::allocator_type int_alloc_type;

		{
			typedef yggr::mplex::typename_replaceor<char_alloc_type, int>::type now_alloc_type;
			BOOST_MPL_ASSERT((boost::is_same<int_alloc_type, now_alloc_type>));
		}

		{
			typedef yggr::mplex::typename_replaceor<char_alloc_type>::type now_alloc_type;
			BOOST_MPL_ASSERT((boost::is_same<char_alloc_type, now_alloc_type>));
		}

		
		{
			typedef yggr::mplex::typename_replaceor<int_alloc_type, char>::type now_alloc_type;
			BOOST_MPL_ASSERT((boost::is_same<char_alloc_type, now_alloc_type>));
		}
	}

	{
		typedef yggr::vector<char>::allocator_type char_alloc_type;
		typedef yggr::vector<int>::allocator_type int_alloc_type;

		{
			typedef yggr::mplex::typename_replaceor<char_alloc_type>::type now_alloc_type;
			BOOST_MPL_ASSERT((boost::is_same<char_alloc_type, now_alloc_type>));
		}

		
		{
			typedef yggr::mplex::typename_replaceor<int_alloc_type, char>::type now_alloc_type;
			BOOST_MPL_ASSERT((boost::is_same<char_alloc_type, now_alloc_type>));
		}
	}

	std::cout << "------------test1 succeed--------------" << std::endl;
}

template<typename T1, typename T2, typename T3>
struct A
{
};

void test2(void)
{
	typedef A<char, char, char> eins_type;
	typedef A<int, char, char> zwei_type;
	typedef A<int, int, char> drei_type;
	typedef A<int, int, int> vier_type;
	typedef A<char, char, int> funf_type;

	typedef yggr::mplex::typename_replaceor<eins_type>::type ret_eins_type;
	typedef yggr::mplex::typename_replaceor<eins_type, int>::type ret_zwei_type;
	typedef yggr::mplex::typename_replaceor<eins_type, int, int>::type ret_drei_type;
	typedef yggr::mplex::typename_replaceor<eins_type, int, int, int>::type ret_vier_type;
	typedef yggr::mplex::typename_replaceor<eins_type, yggr::mplex::_keep, yggr::mplex::_keep, int>::type ret_funf_type;

	BOOST_MPL_ASSERT((boost::is_same<eins_type, ret_eins_type>));
	BOOST_MPL_ASSERT((boost::is_same<zwei_type, ret_zwei_type>));
	BOOST_MPL_ASSERT((boost::is_same<drei_type, ret_drei_type>));
	BOOST_MPL_ASSERT((boost::is_same<vier_type, ret_vier_type>));
	BOOST_MPL_ASSERT((boost::is_same<funf_type, ret_funf_type>));

	std::cout << "------------test2 succeed--------------" << std::endl;
}

void test3(void)
{
	
	typedef std::vector<int> int_vt_type;
	typedef std::vector<char> char_vt_type;

	typedef yggr::mplex::typename_replaceor<int_vt_type, char, std::allocator<char> >::type ret_char_vt_type;
	BOOST_MPL_ASSERT((boost::is_same<char_vt_type, ret_char_vt_type>));
	
	std::cout << "------------test3 succeed--------------" << std::endl;
}

int main(int argc, char* argv[])
{
	test1();
	test2();
	test3();

	wait_any_key(argc, argv);
	return 0;
}