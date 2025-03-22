// iterator_traits_ex_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>

#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_same.hpp>

#include <yggr/iterator_ex/iterator_traits.hpp>

#include <yggr/container/vector.hpp>
#include <yggr/container/list.hpp>
#include <yggr/container/set.hpp>

#ifdef _MSC_VER
#   include <vld.h>
#endif //_MSC_VER

template<typename Container> inline
void test_iterator_value_ex_detail(void)
{
	typedef Container cont_type;
	typedef typename cont_type::value_type val_type;
	typedef typename cont_type::iterator iter_type;
	typedef typename cont_type::const_iterator citer_type;
	typedef typename cont_type::reverse_iterator riter_type;
	typedef typename cont_type::const_reverse_iterator criter_type;

	BOOST_MPL_ASSERT((boost::is_same<typename yggr::iterator_value_ex<val_type*>::type, val_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename yggr::iterator_value_ex<const val_type*>::type, val_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename yggr::iterator_value_ex<val_type const*>::type, val_type>));

	BOOST_MPL_ASSERT((boost::is_same<typename yggr::iterator_value_ex<iter_type>::type, val_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename yggr::iterator_value_ex<citer_type>::type, val_type>));

	BOOST_MPL_ASSERT((boost::is_same<typename yggr::iterator_value_ex<riter_type>::type, val_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename yggr::iterator_value_ex<criter_type>::type, val_type>));

	BOOST_MPL_ASSERT((boost::is_same<typename yggr::iterator_value_ex<cont_type>::type, yggr::mplex::null_type>));
}
	
void test_iterator_value_ex(void)
{
	// vector
	{
		typedef boost::container::vector<int> cont_type;
		test_iterator_value_ex_detail<cont_type>();
	}

	{
		typedef std::vector<int> cont_type;
		test_iterator_value_ex_detail<cont_type>();
	}

	//list
	{
		typedef boost::container::list<int> cont_type;
		test_iterator_value_ex_detail<cont_type>();
	}

	{
		typedef std::list<int> cont_type;
		test_iterator_value_ex_detail<cont_type>();
	}

	// set
	{
		typedef boost::container::set<int> cont_type;
		test_iterator_value_ex_detail<cont_type>();
	}

	{
		typedef std::set<int> cont_type;
		test_iterator_value_ex_detail<cont_type>();
	}
}

template<typename Container> inline
void test_iterator_refernce_ex_detail(void)
{
	typedef Container cont_type;
	typedef typename cont_type::reference ref_type;
	typedef typename cont_type::const_reference cref_type;
	typedef typename cont_type::value_type val_type;
	typedef typename cont_type::iterator iter_type;
	typedef typename cont_type::const_iterator citer_type;
	typedef typename cont_type::reverse_iterator riter_type;
	typedef typename cont_type::const_reverse_iterator criter_type;

	BOOST_MPL_ASSERT((boost::is_same<typename yggr::iterator_reference_ex<val_type*>::type, ref_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename yggr::iterator_reference_ex<const val_type*>::type, cref_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename yggr::iterator_reference_ex<val_type const*>::type, cref_type>));

	BOOST_MPL_ASSERT((boost::is_same<typename yggr::iterator_reference_ex<iter_type>::type, ref_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename yggr::iterator_reference_ex<citer_type>::type, cref_type>));

	BOOST_MPL_ASSERT((boost::is_same<typename yggr::iterator_reference_ex<riter_type>::type, ref_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename yggr::iterator_reference_ex<criter_type>::type, cref_type>));

	BOOST_MPL_ASSERT((boost::is_same<typename yggr::iterator_reference_ex<cont_type>::type, yggr::mplex::null_type>));
}

template<typename Container> inline
void test_iterator_refernce_ex_detail_std_set(void)
{
	typedef Container cont_type;
	typedef typename cont_type::reference ref_type;
	typedef typename cont_type::const_reference cref_type;
	typedef typename cont_type::value_type val_type;
	typedef typename cont_type::iterator iter_type;
	typedef typename cont_type::const_iterator citer_type;
	typedef typename cont_type::reverse_iterator riter_type;
	typedef typename cont_type::const_reverse_iterator criter_type;

	BOOST_MPL_ASSERT((boost::is_same<typename yggr::iterator_reference_ex<iter_type>::type, cref_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename yggr::iterator_reference_ex<citer_type>::type, cref_type>));

	BOOST_MPL_ASSERT((boost::is_same<typename yggr::iterator_reference_ex<riter_type>::type, cref_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename yggr::iterator_reference_ex<criter_type>::type, cref_type>));

	BOOST_MPL_ASSERT((boost::is_same<typename yggr::iterator_reference_ex<cont_type>::type, yggr::mplex::null_type>));
}
	
void test_iterator_refernce_ex(void)
{
	// vector
	{
		typedef boost::container::vector<int> cont_type;
		test_iterator_refernce_ex_detail<cont_type>();
	}

	{
		typedef std::vector<int> cont_type;
		test_iterator_refernce_ex_detail<cont_type>();
	}

	//list
	{
		typedef boost::container::list<int> cont_type;
		test_iterator_refernce_ex_detail<cont_type>();
	}

	{
		typedef std::list<int> cont_type;
		test_iterator_refernce_ex_detail<cont_type>();
	}

	// set
	{
		typedef boost::container::set<int> cont_type;
		test_iterator_refernce_ex_detail<cont_type>();
	}

	{
		typedef std::set<int> cont_type;
		test_iterator_refernce_ex_detail_std_set<cont_type>();
	}
}

template<typename Container> inline
void test_iterator_pointer_ex_detail(void)
{
	typedef Container cont_type;
	typedef typename cont_type::pointer ptr_type;
	typedef typename cont_type::const_pointer cptr_type;
	typedef typename cont_type::value_type val_type;
	typedef typename cont_type::iterator iter_type;
	typedef typename cont_type::const_iterator citer_type;
	typedef typename cont_type::reverse_iterator riter_type;
	typedef typename cont_type::const_reverse_iterator criter_type;

	BOOST_MPL_ASSERT((boost::is_same<typename yggr::iterator_pointer_ex<val_type*>::type, ptr_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename yggr::iterator_pointer_ex<const val_type*>::type, cptr_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename yggr::iterator_pointer_ex<val_type const*>::type, cptr_type>));

	BOOST_MPL_ASSERT((boost::is_same<typename yggr::iterator_pointer_ex<iter_type>::type, ptr_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename yggr::iterator_pointer_ex<citer_type>::type, cptr_type>));

	BOOST_MPL_ASSERT((boost::is_same<typename yggr::iterator_pointer_ex<riter_type>::type, ptr_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename yggr::iterator_pointer_ex<criter_type>::type, cptr_type>));

	BOOST_MPL_ASSERT((boost::is_same<typename yggr::iterator_pointer_ex<cont_type>::type, yggr::mplex::null_type>));
}

template<typename Container> inline
void test_iterator_pointer_ex_detail_std_set(void)
{
	typedef Container cont_type;
	typedef typename cont_type::pointer ptr_type;
	typedef typename cont_type::const_pointer cptr_type;
	typedef typename cont_type::value_type val_type;
	typedef typename cont_type::iterator iter_type;
	typedef typename cont_type::const_iterator citer_type;
	typedef typename cont_type::reverse_iterator riter_type;
	typedef typename cont_type::const_reverse_iterator criter_type;

	BOOST_MPL_ASSERT((boost::is_same<typename yggr::iterator_pointer_ex<iter_type>::type, cptr_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename yggr::iterator_pointer_ex<citer_type>::type, cptr_type>));

	BOOST_MPL_ASSERT((boost::is_same<typename yggr::iterator_pointer_ex<riter_type>::type, cptr_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename yggr::iterator_pointer_ex<criter_type>::type, cptr_type>));

	BOOST_MPL_ASSERT((boost::is_same<typename yggr::iterator_pointer_ex<cont_type>::type, yggr::mplex::null_type>));
}
	
void test_iterator_pointer_ex(void)
{
	// vector
	{
		typedef boost::container::vector<int> cont_type;
		test_iterator_pointer_ex_detail<cont_type>();
	}

	{
		typedef std::vector<int> cont_type;
		test_iterator_pointer_ex_detail<cont_type>();
	}

	//list
	{
		typedef boost::container::list<int> cont_type;
		test_iterator_pointer_ex_detail<cont_type>();
	}

	{
		typedef std::list<int> cont_type;
		test_iterator_pointer_ex_detail<cont_type>();
	}

	// set
	{
		typedef boost::container::set<int> cont_type;
		test_iterator_pointer_ex_detail<cont_type>();
	}

	{
		typedef std::set<int> cont_type;
		test_iterator_pointer_ex_detail_std_set<cont_type>();
	}
}

template<typename Container> inline
void test_iterator_difference_ex_detail(void)
{
	typedef Container cont_type;
	typedef typename cont_type::difference_type diff_type;
	typedef typename cont_type::value_type val_type;
	typedef typename cont_type::iterator iter_type;
	typedef typename cont_type::const_iterator citer_type;
	typedef typename cont_type::reverse_iterator riter_type;
	typedef typename cont_type::const_reverse_iterator criter_type;

	BOOST_MPL_ASSERT((boost::is_same<typename yggr::iterator_difference_ex<val_type*>::type, diff_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename yggr::iterator_difference_ex<const val_type*>::type, diff_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename yggr::iterator_difference_ex<val_type const*>::type, diff_type>));

	BOOST_MPL_ASSERT((boost::is_same<typename yggr::iterator_difference_ex<iter_type>::type, diff_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename yggr::iterator_difference_ex<citer_type>::type, diff_type>));

	BOOST_MPL_ASSERT((boost::is_same<typename yggr::iterator_difference_ex<riter_type>::type, diff_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename yggr::iterator_difference_ex<criter_type>::type, diff_type>));

	BOOST_MPL_ASSERT((boost::is_same<typename yggr::iterator_difference_ex<cont_type>::type, yggr::mplex::null_type>));
}
	
void test_iterator_difference_ex(void)
{
	// vector
	{
		typedef boost::container::vector<int> cont_type;
		test_iterator_difference_ex_detail<cont_type>();
	}

	{
		typedef std::vector<int> cont_type;
		test_iterator_difference_ex_detail<cont_type>();
	}

	//list
	{
		typedef boost::container::list<int> cont_type;
		test_iterator_difference_ex_detail<cont_type>();
	}

	{
		typedef std::list<int> cont_type;
		test_iterator_difference_ex_detail<cont_type>();
	}

	// set
	{
		typedef boost::container::set<int> cont_type;
		test_iterator_difference_ex_detail<cont_type>();
	}

	{
		typedef std::set<int> cont_type;
		test_iterator_difference_ex_detail<cont_type>();
	}
}

template<typename Container> inline
void test_iterator_category_ex_detail(void)
{
	typedef Container cont_type;
	typedef typename cont_type::value_type val_type;
	typedef typename cont_type::iterator iter_type;
	typedef typename cont_type::const_iterator citer_type;
	typedef typename cont_type::reverse_iterator riter_type;
	typedef typename cont_type::const_reverse_iterator criter_type;

	BOOST_MPL_ASSERT((boost::is_same<typename yggr::iterator_category_ex<val_type*>::type, 
										typename boost::iterator_category<val_type*>::type>));

	BOOST_MPL_ASSERT((boost::is_same<typename yggr::iterator_category_ex<const val_type*>::type, 
										typename boost::iterator_category<const val_type*>::type>));

	BOOST_MPL_ASSERT((boost::is_same<typename yggr::iterator_category_ex<val_type const*>::type, 
										typename boost::iterator_category<val_type const*>::type>));

	BOOST_MPL_ASSERT((boost::is_same<typename yggr::iterator_category_ex<iter_type>::type, 
										typename boost::iterator_category<iter_type>::type>));

	BOOST_MPL_ASSERT((boost::is_same<typename yggr::iterator_category_ex<citer_type>::type, 
										typename boost::iterator_category<citer_type>::type>));

	BOOST_MPL_ASSERT((boost::is_same<typename yggr::iterator_category_ex<riter_type>::type, 
										typename boost::iterator_category<riter_type>::type>));

	BOOST_MPL_ASSERT((boost::is_same<typename yggr::iterator_category_ex<criter_type>::type, 
										typename boost::iterator_category<criter_type>::type>));

	BOOST_MPL_ASSERT((boost::is_same<typename yggr::iterator_category_ex<cont_type>::type, yggr::mplex::null_type>));
}
	
void test_iterator_category_ex(void)
{
	// vector
	{
		typedef boost::container::vector<int> cont_type;
		test_iterator_category_ex_detail<cont_type>();
	}

	{
		typedef std::vector<int> cont_type;
		test_iterator_category_ex_detail<cont_type>();
	}

	//list
	{
		typedef boost::container::list<int> cont_type;
		test_iterator_category_ex_detail<cont_type>();
	}

	{
		typedef std::list<int> cont_type;
		test_iterator_category_ex_detail<cont_type>();
	}

	// set
	{
		typedef boost::container::set<int> cont_type;
		test_iterator_category_ex_detail<cont_type>();
	}

	{
		typedef std::set<int> cont_type;
		test_iterator_category_ex_detail<cont_type>();
	}
}


int main(int argc, char* argv[])
{

	test_iterator_value_ex();
	test_iterator_refernce_ex();
	test_iterator_pointer_ex();
	test_iterator_difference_ex();
	test_iterator_category_ex();

	wait_any_key(argc, argv);
	return 0;
}