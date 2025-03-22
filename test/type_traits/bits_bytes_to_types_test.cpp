// bits_bytes_to_type_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_same.hpp>
#include <yggr/type_traits/bytes_to_types.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

void test_bits(void)
{
	BOOST_MPL_ASSERT((boost::is_same<yggr::bits_to_signed<0>::type, yggr::mplex::null_type>));
	BOOST_MPL_ASSERT((boost::is_same<yggr::bits_to_signed<(1 * 8)>::type, yggr::s8>));
	BOOST_MPL_ASSERT((boost::is_same<yggr::bits_to_signed<(2 * 8)>::type, yggr::s16>));
	BOOST_MPL_ASSERT((boost::is_same<yggr::bits_to_signed<(4 * 8)>::type, yggr::s32>));
	BOOST_MPL_ASSERT((boost::is_same<yggr::bits_to_signed<(8 * 8)>::type, yggr::s64>));
	BOOST_MPL_ASSERT((boost::is_same<yggr::bits_to_signed<100>::type, yggr::mplex::null_type>));

	BOOST_MPL_ASSERT((boost::is_same<yggr::bits_to_unsigned<0>::type, yggr::mplex::null_type>));
	BOOST_MPL_ASSERT((boost::is_same<yggr::bits_to_unsigned<(1 * 8)>::type, yggr::u8>));
	BOOST_MPL_ASSERT((boost::is_same<yggr::bits_to_unsigned<(2 * 8)>::type, yggr::u16>));
	BOOST_MPL_ASSERT((boost::is_same<yggr::bits_to_unsigned<(4 * 8)>::type, yggr::u32>));
	BOOST_MPL_ASSERT((boost::is_same<yggr::bits_to_unsigned<(8 * 8)>::type, yggr::u64>));
	BOOST_MPL_ASSERT((boost::is_same<yggr::bits_to_unsigned<100>::type, yggr::mplex::null_type>));

	BOOST_MPL_ASSERT((boost::is_same<yggr::bits_to_float<0>::type, yggr::mplex::null_type>));
	BOOST_MPL_ASSERT((boost::is_same<yggr::bits_to_float<(4 * 8)>::type, yggr::f32>));
	BOOST_MPL_ASSERT((boost::is_same<yggr::bits_to_float<(8 * 8)>::type, yggr::f64>));
	BOOST_MPL_ASSERT((boost::is_same<yggr::bits_to_float<100>::type, yggr::mplex::null_type>));
}

void test_bytes(void)
{
	BOOST_MPL_ASSERT((boost::is_same<yggr::bytes_to_signed<0>::type, yggr::mplex::null_type>));
	BOOST_MPL_ASSERT((boost::is_same<yggr::bytes_to_signed<sizeof(yggr::u8)>::type, yggr::s8>));
	BOOST_MPL_ASSERT((boost::is_same<yggr::bytes_to_signed<sizeof(yggr::u16)>::type, yggr::s16>));
	BOOST_MPL_ASSERT((boost::is_same<yggr::bytes_to_signed<sizeof(yggr::u32)>::type, yggr::s32>));
	BOOST_MPL_ASSERT((boost::is_same<yggr::bytes_to_signed<sizeof(yggr::u64)>::type, yggr::s64>));
	BOOST_MPL_ASSERT((boost::is_same<yggr::bytes_to_signed<100>::type, yggr::mplex::null_type>));

	BOOST_MPL_ASSERT((boost::is_same<yggr::bytes_to_unsigned<0>::type, yggr::mplex::null_type>));
	BOOST_MPL_ASSERT((boost::is_same<yggr::bytes_to_unsigned<(1)>::type, yggr::u8>));
	BOOST_MPL_ASSERT((boost::is_same<yggr::bytes_to_unsigned<(2)>::type, yggr::u16>));
	BOOST_MPL_ASSERT((boost::is_same<yggr::bytes_to_unsigned<(4)>::type, yggr::u32>));
	BOOST_MPL_ASSERT((boost::is_same<yggr::bytes_to_unsigned<(8)>::type, yggr::u64>));
	BOOST_MPL_ASSERT((boost::is_same<yggr::bytes_to_unsigned<100>::type, yggr::mplex::null_type>));

	BOOST_MPL_ASSERT((boost::is_same<yggr::bytes_to_float<0>::type, yggr::mplex::null_type>));
	BOOST_MPL_ASSERT((boost::is_same<yggr::bytes_to_float<(4)>::type, yggr::f32>));
	BOOST_MPL_ASSERT((boost::is_same<yggr::bytes_to_float<(8)>::type, yggr::f64>));
	BOOST_MPL_ASSERT((boost::is_same<yggr::bytes_to_float<100>::type, yggr::mplex::null_type>));
}

int main(int argc, char* argv[])
{
	test_bits();
	test_bytes();

	wait_any_key(argc, argv);
	return 0;
}