// tuple_byte_size_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>

#include <yggr/base/yggrdef.h>
#include <yggr/charset/string.hpp>
#include <yggr/container/list.hpp>
#include <yggr/bytes/byte_size.hpp>
#include <yggr/tuple/tuple.hpp>
#include <yggr/tuple/byte_size.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

void test(void)
{
#if !defined(YGGR_NO_CXX11_HDR_TUPLE)
	// std
	{
		typedef std::tuple<> tuple_type;
		tuple_type t;
		yggr_test_assert(0 == yggr::byte_size(t));
	}

	{
		typedef std::tuple<int> tuple_type;
		tuple_type t = std::make_tuple(1);
		yggr_test_assert(4 == yggr::byte_size(t));
	}

	{
		typedef std::tuple<int, int> tuple_type;
		tuple_type t = std::make_tuple(1, 2);
		yggr_test_assert(8 == yggr::byte_size(t));
	}

	{
		typedef std::tuple<int, int, int> tuple_type;
		tuple_type t = std::make_tuple(1, 2, 3);
		yggr_test_assert(12 == yggr::byte_size(t));
	}

	{
		typedef std::tuple<yggr::string, int, int> tuple_type;
		tuple_type t = std::make_tuple("1", 2, 3);
		yggr_test_assert(9 == yggr::byte_size(t));
	}

	{
		typedef std::tuple<int, int, int> tuple_type;
		typedef yggr::list<tuple_type> tuple_vt_type;

		tuple_vt_type vt;
		vt.push_back(std::make_tuple(1, 2, 3));
		vt.push_back(std::make_tuple(1, 2, 3));
		yggr_test_assert(24 == yggr::byte_size(vt));
	}

	{
		typedef std::tuple<yggr::string, int, int> tuple_type;
		typedef yggr::list<tuple_type> tuple_vt_type;

		tuple_vt_type vt;
		vt.push_back(std::make_tuple("1", 2, 3));
		vt.push_back(std::make_tuple("1", 2, 3));
		yggr_test_assert(18 == yggr::byte_size(vt));
	}
#endif // #if !defined(YGGR_NO_CXX11_HDR_TUPLE)

	// boost
	{
		typedef boost::tuple<> tuple_type;
		tuple_type t;
		yggr_test_assert(0 == yggr::byte_size(t));
	}

	{
		typedef boost::tuple<int> tuple_type;
		tuple_type t = boost::make_tuple(1);
		yggr_test_assert(4 == yggr::byte_size(t));
	}

	{
		typedef boost::tuple<int, int> tuple_type;
		tuple_type t = boost::make_tuple(1, 2);
		yggr_test_assert(8 == yggr::byte_size(t));
	}

	{
		typedef boost::tuple<int, int, int> tuple_type;

		tuple_type t = boost::make_tuple(1, 2, 3);
		yggr_test_assert(12 == yggr::byte_size(t));
	}

	{
		typedef boost::tuple<yggr::string, int, int> tuple_type;

		tuple_type t = boost::make_tuple("1", 2, 3);
		yggr_test_assert(9 == yggr::byte_size(t));
	}

	{
		typedef boost::tuple<int, int, int> tuple_type;
		typedef yggr::list<tuple_type> tuple_vt_type;

		tuple_vt_type vt;
		vt.push_back(boost::make_tuple(1, 2, 3));
		vt.push_back(boost::make_tuple(1, 2, 3));
		yggr_test_assert(24 == yggr::byte_size(vt));
	}

	{
		typedef boost::tuple<yggr::string, int, int> tuple_type;
		typedef yggr::list<tuple_type> tuple_vt_type;

		tuple_vt_type vt;
		vt.push_back(boost::make_tuple("1", 2, 3));
		vt.push_back(boost::make_tuple("1", 2, 3));
		yggr_test_assert(18 == yggr::byte_size(vt));
	}

	// yggr
	{
		typedef yggr::tuple<> tuple_type;
		tuple_type t;
		yggr_test_assert(0 == yggr::byte_size(t));
	}

	{
		typedef yggr::tuple<int> tuple_type;
		tuple_type t = yggr::make_tuple(1);
		yggr_test_assert(4 == yggr::byte_size(t));
	}

	{
		typedef yggr::tuple<int, int> tuple_type;
		tuple_type t = yggr::make_tuple(1, 2);
		yggr_test_assert(8 == yggr::byte_size(t));
	}

	{
		typedef yggr::tuple<int, int, int> tuple_type;
		tuple_type t = yggr::make_tuple(1, 2, 3);
		yggr_test_assert(12 == yggr::byte_size(t));
	}

	{
		typedef yggr::tuple<yggr::string, int, int> tuple_type;
		tuple_type t = yggr::make_tuple("1", 2, 3);
		yggr_test_assert(9 == yggr::byte_size(t));
	}

	{
		typedef yggr::tuple<int, int, int> tuple_type;
		typedef yggr::list<tuple_type> tuple_vt_type;

		tuple_vt_type vt;
		vt.push_back(yggr::make_tuple(1, 2, 3));
		vt.push_back(yggr::make_tuple(1, 2, 3));
		yggr_test_assert(24 == yggr::byte_size(vt));
	}

	{
		typedef yggr::tuple<yggr::string, int, int> tuple_type;
		typedef yggr::list<tuple_type> tuple_vt_type;

		tuple_vt_type vt;
		vt.push_back(yggr::make_tuple("1", 2, 3));
		vt.push_back(yggr::make_tuple("1", 2, 3));
		yggr_test_assert(18 == yggr::byte_size(vt));
	}
}

int main(int argc, char* argv[])
{
	test();

	wait_any_key(argc, argv);
	return 0;
}
