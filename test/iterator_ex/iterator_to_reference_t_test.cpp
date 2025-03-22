// test_iterator_to_reference_t.cpp

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
void test_iterator_to_refernce_t_detail(void)
{
	typedef Container cont_type;
	typedef typename cont_type::reference ref_type;
	typedef typename cont_type::const_reference cref_type;
	typedef typename cont_type::value_type val_type;
	typedef typename cont_type::iterator iter_type;
	typedef typename cont_type::const_iterator citer_type;
	typedef typename cont_type::reverse_iterator riter_type;
	typedef typename cont_type::const_reverse_iterator criter_type;

	BOOST_MPL_ASSERT((boost::is_same<typename yggr::iterator_ex::iterator_reference_ex<val_type*>::type, ref_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename yggr::iterator_ex::iterator_reference_ex<const val_type*>::type, cref_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename yggr::iterator_ex::iterator_reference_ex<val_type const*>::type, cref_type>));

	BOOST_MPL_ASSERT((boost::is_same<typename yggr::iterator_ex::iterator_reference_ex<iter_type>::type, ref_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename yggr::iterator_ex::iterator_reference_ex<citer_type>::type, cref_type>));

	BOOST_MPL_ASSERT((boost::is_same<typename yggr::iterator_ex::iterator_reference_ex<riter_type>::type, ref_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename yggr::iterator_ex::iterator_reference_ex<criter_type>::type, cref_type>));

	BOOST_MPL_ASSERT((boost::is_same<typename yggr::iterator_ex::iterator_reference_ex<cont_type>::type, yggr::mplex::null_type>));
}

template<typename Container> inline
void test_iterator_to_refernce_t_detail_std_set(void)
{
	typedef Container cont_type;
	typedef typename cont_type::reference ref_type;
	typedef typename cont_type::const_reference cref_type;
	typedef typename cont_type::value_type val_type;
	typedef typename cont_type::iterator iter_type;
	typedef typename cont_type::const_iterator citer_type;
	typedef typename cont_type::reverse_iterator riter_type;
	typedef typename cont_type::const_reverse_iterator criter_type;

	BOOST_MPL_ASSERT((boost::is_same<typename yggr::iterator_ex::iterator_reference_ex<iter_type>::type, cref_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename yggr::iterator_ex::iterator_reference_ex<citer_type>::type, cref_type>));

	BOOST_MPL_ASSERT((boost::is_same<typename yggr::iterator_ex::iterator_reference_ex<riter_type>::type, cref_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename yggr::iterator_ex::iterator_reference_ex<criter_type>::type, cref_type>));

	BOOST_MPL_ASSERT((boost::is_same<typename yggr::iterator_ex::iterator_reference_ex<cont_type>::type, yggr::mplex::null_type>));
}
	
void test_iterator_to_refernce_t(void)
{
	// vector
	{
		typedef boost::container::vector<int> cont_type;
		test_iterator_to_refernce_t_detail<cont_type>();
	}

	{
		typedef std::vector<int> cont_type;
		test_iterator_to_refernce_t_detail<cont_type>();
	}

	//list
	{
		typedef boost::container::list<int> cont_type;
		test_iterator_to_refernce_t_detail<cont_type>();
	}

	{
		typedef std::list<int> cont_type;
		test_iterator_to_refernce_t_detail<cont_type>();
	}

	// set
	{
		typedef boost::container::set<int> cont_type;
		test_iterator_to_refernce_t_detail<cont_type>();
	}

	{
		typedef std::set<int> cont_type;
		test_iterator_to_refernce_t_detail_std_set<cont_type>();
	}
}

int main(int argc, char* argv[])
{

	test_iterator_to_refernce_t();

	wait_any_key(argc, argv);
	return 0;
}