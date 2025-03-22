//cast_to_container_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>

#include <boost/mpl/assert.hpp>

#include <yggr/mplex/get_reference_if_in.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER


void test(void)
{
	// not in boost::mpl::vector<yggr::u32, yggr::f32>
	{
		typedef
			yggr::mplex::get_reference_if_in
			<
				yggr::u8,
				yggr::u32,
				boost::mpl::vector<yggr::u32, yggr::f32>
			> if_in_type;


		BOOST_MPL_ASSERT((boost::is_same<if_in_type::nctype, yggr::u32>));
		BOOST_MPL_ASSERT((boost::is_same<if_in_type::ctype, yggr::u32>));
	}

	// not in boost::mpl::vector<yggr::u32, yggr::f32>
	{
		typedef
			yggr::mplex::get_reference_if_in
			<
				yggr::s32,
				yggr::u32,
				boost::mpl::vector<yggr::u32, yggr::f32>
			> if_in_type;


		BOOST_MPL_ASSERT((boost::is_same<if_in_type::nctype, yggr::u32>));
		BOOST_MPL_ASSERT((boost::is_same<if_in_type::ctype, yggr::u32>));
	}

	// in boost::mpl::vector<yggr::u32, yggr::f32>
	{
		typedef
			yggr::mplex::get_reference_if_in
			<
				yggr::u32,
				yggr::u32,
				boost::mpl::vector<yggr::u32, yggr::f32>
			> if_in_type;


		BOOST_MPL_ASSERT((boost::is_same<if_in_type::nctype, yggr::u32&>));
		BOOST_MPL_ASSERT((boost::is_same<if_in_type::ctype, const yggr::u32&>));
	}

	{
		typedef
			yggr::mplex::get_reference_if_in
			<
				yggr::f32,
				yggr::u32,
				boost::mpl::vector<yggr::u32, yggr::f32>
			> if_in_type;


		BOOST_MPL_ASSERT((boost::is_same<if_in_type::nctype, yggr::f32&>));
		BOOST_MPL_ASSERT((boost::is_same<if_in_type::ctype, const yggr::f32&>));
	}
}

int main(int argc, char* argv[])
{
	test();

	wait_any_key(argc, argv);
	return 0;
}
