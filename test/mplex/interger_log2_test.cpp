//integer_log2_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <boost/type_traits/is_same.hpp>
#include <yggr/mplex/integer_log2.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

void test(void)
{
	{
		typedef yggr::mplex::integer_log2<sizeof(yggr::u8)>::type exp_type;
		BOOST_MPL_ASSERT((boost::is_same<exp_type, boost::mpl::size_t<0> >));
	}

	{
		typedef yggr::mplex::integer_log2<sizeof(yggr::u16)>::type exp_type;
		BOOST_MPL_ASSERT((boost::is_same<exp_type, boost::mpl::size_t<1> >));
	}

	{
		typedef yggr::mplex::integer_log2<sizeof(yggr::u32)>::type exp_type;
		BOOST_MPL_ASSERT((boost::is_same<exp_type, boost::mpl::size_t<2> >));
	}

	{
		typedef yggr::mplex::integer_log2<sizeof(yggr::u64)>::type exp_type;
		BOOST_MPL_ASSERT((boost::is_same<exp_type, boost::mpl::size_t<3> >));
	}

	{
		typedef yggr::mplex::integer_log2<16>::type exp_type;
		BOOST_MPL_ASSERT((boost::is_same<exp_type, boost::mpl::size_t<4> >));
	}

	{
		typedef yggr::mplex::integer_log2<32>::type exp_type;
		BOOST_MPL_ASSERT((boost::is_same<exp_type, boost::mpl::size_t<5> >));
	}

	{
		typedef yggr::mplex::integer_log2<64>::type exp_type;
		BOOST_MPL_ASSERT((boost::is_same<exp_type, boost::mpl::size_t<6> >));
	}

	{
		typedef yggr::mplex::integer_log2<65536>::type exp_type;
		BOOST_MPL_ASSERT((boost::is_same<exp_type, boost::mpl::size_t<16> >));
	}

	{
		typedef yggr::mplex::integer_log2<3>::type exp_type;
		BOOST_MPL_ASSERT((boost::is_same<exp_type, yggr::mplex::interger_log2_failed>));
	}
}

int main(int argc, char* argv[])
{
	test();

	wait_any_key(argc, argv);
	return 0;
}
