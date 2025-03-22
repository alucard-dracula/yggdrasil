// array_ex_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>

#include <yggr/base/yggrdef.h>
#include <yggr/move/move.hpp>
#include <yggr/container_ex/array.hpp>

#include <boost/config.hpp>

#include <boost/mpl/size_t.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_same.hpp>


#if defined(_MSC_VER)
#	include <vld.h>
#endif // _MSC_VER

#define TEST_COMPILE_ERROR 0


void test_get(void)
{
	{
		typedef int(array_type)[3];
		array_type arr = {1, 2, 3};
		bool bchk = false;

		bchk = (yggr::get<0>(static_cast<array_type&>(arr)) == arr[0]);
		yggr_test_assert(bchk);
#if !defined(BOOST_NO_CXX11_RVALUE_REFERENCES)
#	if !defined(_MSC_VER) || !(_MSC_VER < 1700)
		bchk = (yggr::get<1>(boost::move(arr)) == arr[1]);
		yggr_test_assert(bchk);
#	endif // !defined(_MSC_VER) || !(_MSC_VER < 1700)
#endif // BOOST_NO_CXX11_RVALUE_REFERENCES
		bchk = (yggr::get<2>(static_cast<const array_type&>(arr)) == arr[2]);
		yggr_test_assert(bchk);

		bchk = (yggr::get<boost::mpl::size_t<0> >(static_cast<array_type&>(arr)) == arr[0]);
		yggr_test_assert(bchk);
#if !defined(BOOST_NO_CXX11_RVALUE_REFERENCES)
#	if !defined(_MSC_VER) || !(_MSC_VER < 1700)
		bchk = (yggr::get<boost::mpl::size_t<1> >(boost::move(arr)) == arr[1]);
		yggr_test_assert(bchk);
#	endif // !defined(_MSC_VER) || !(_MSC_VER < 1700)
#endif // BOOST_NO_CXX11_RVALUE_REFERENCES
		bchk = (yggr::get<boost::mpl::size_t<2> >(static_cast<const array_type&>(arr)) == arr[2]);
		yggr_test_assert(bchk);

#if TEST_COMPILE_ERROR
		bchk = (yggr::get<3>(arr) == arr[3]);
		yggr_test_assert(bchk);

		bchk = (yggr::get<boost::mpl::size_t<3> >(arr) == arr[3]);
		yggr_test_assert(bchk);
#endif // TEST_COMPILE_ERROR
	}

#ifndef YGGR_NO_CXX11_HDR_ARRAY
	{
		typedef std::array<int, 3> array_type;
		array_type arr = {1, 2, 3};
		bool bchk = false;

		bchk = (yggr::get<0>(static_cast<array_type&>(arr)) == arr[0]);
		yggr_test_assert(bchk);
		bchk = (yggr::get<1>(boost::move(arr)) == arr[1]);
		yggr_test_assert(bchk);
		bchk = (yggr::get<2>(static_cast<const array_type&>(arr)) == arr[2]);
		yggr_test_assert(bchk);

		bchk = (yggr::get<boost::mpl::size_t<0> >(static_cast<array_type&>(arr)) == arr[0]);
		yggr_test_assert(bchk);
		bchk = (yggr::get<boost::mpl::size_t<1> >(boost::move(arr)) == arr[1]);
		yggr_test_assert(bchk);
		bchk = (yggr::get<boost::mpl::size_t<2> >(static_cast<const array_type&>(arr)) == arr[2]);
		yggr_test_assert(bchk);


#if TEST_COMPILE_ERROR
		bchk = (yggr::get<3>(arr) == arr[3]);
		yggr_test_assert(bchk);

		bchk = (yggr::get<boost::mpl::size_t<3> >(arr) == arr[3]);
		yggr_test_assert(bchk);
#endif // TEST_COMPILE_ERROR
	}
#endif // YGGR_NO_CXX11_HDR_ARRAY

	{
		typedef boost::array<int, 3> array_type;
		array_type arr = {1, 2, 3};
		bool bchk = false;

		bchk = (yggr::get<0>(static_cast<array_type&>(arr)) == arr[0]);
		yggr_test_assert(bchk);
		bchk = (yggr::get<1>(boost::move(arr)) == arr[1]);
		yggr_test_assert(bchk);
		bchk = (yggr::get<2>(static_cast<const array_type&>(arr)) == arr[2]);
		yggr_test_assert(bchk);

bchk = (yggr::get<boost::mpl::size_t<0> >(static_cast<array_type&>(arr)) == arr[0]);
		yggr_test_assert(bchk);
		bchk = (yggr::get<boost::mpl::size_t<1> >(boost::move(arr)) == arr[1]);
		yggr_test_assert(bchk);
		bchk = (yggr::get<boost::mpl::size_t<2> >(static_cast<const array_type&>(arr)) == arr[2]);
		yggr_test_assert(bchk);


#if TEST_COMPILE_ERROR
		bchk = (yggr::get<3>(arr) == arr[3]);
		yggr_test_assert(bchk);

		bchk = (yggr::get<boost::mpl::size_t<3> >(arr) == arr[3]);
		yggr_test_assert(bchk);
#endif // TEST_COMPILE_ERROR
	}

	{
		typedef yggr::array<int, 3> array_type;
		array_type arr = {1, 2, 3};
		bool bchk = false;

		bchk = (yggr::get<0>(static_cast<array_type&>(arr)) == arr[0]);
		yggr_test_assert(bchk);
		bchk = (yggr::get<1>(boost::move(arr)) == arr[1]);
		yggr_test_assert(bchk);
		bchk = (yggr::get<2>(static_cast<const array_type&>(arr)) == arr[2]);
		yggr_test_assert(bchk);

		bchk = (yggr::get<boost::mpl::size_t<0> >(static_cast<array_type&>(arr)) == arr[0]);
		yggr_test_assert(bchk);
		bchk = (yggr::get<boost::mpl::size_t<1> >(boost::move(arr)) == arr[1]);
		yggr_test_assert(bchk);
		bchk = (yggr::get<boost::mpl::size_t<2> >(static_cast<const array_type&>(arr)) == arr[2]);
		yggr_test_assert(bchk);


#if TEST_COMPILE_ERROR
		bchk = (yggr::get<3>(arr) == arr[3]);
		yggr_test_assert(bchk);

		bchk = (yggr::get<boost::mpl::size_t<3> >(arr) == arr[3]);
		yggr_test_assert(bchk);
#endif // TEST_COMPILE_ERROR
	}
}

void test_to_array(void)
{
	typedef int(native_arr_type)[3];

	native_arr_type arr = {1, 2, 3};

	{
		typedef yggr::array<int, 3> arr_type;

		arr_type obj_arr = yggr::to_array(static_cast<native_arr_type&>(arr));
		yggr_test_assert(obj_arr[0] == arr[0]);
		yggr_test_assert(obj_arr[1] == arr[1]);
		yggr_test_assert(obj_arr[2] == arr[2]);
	}

#if !(defined(BOOST_NO_CXX11_RVALUE_REFERENCES) || (defined(_MSC_VER) && (_MSC_VER < 1700) ))
	{
		typedef yggr::array<int, 3> arr_type;

		arr_type obj_arr = yggr::to_array(boost::move(arr));
		yggr_test_assert(obj_arr[0] == arr[0]);
		yggr_test_assert(obj_arr[1] == arr[1]);
		yggr_test_assert(obj_arr[2] == arr[2]);
	}
#endif // BOOST_NO_CXX11_RVALUE_REFERENCES

	{
		typedef yggr::array<int, 3> arr_type;

		arr_type obj_arr = yggr::to_array(static_cast<const native_arr_type&>(arr));
		yggr_test_assert(obj_arr[0] == arr[0]);
		yggr_test_assert(obj_arr[1] == arr[1]);
		yggr_test_assert(obj_arr[2] == arr[2]);
	}

#ifndef YGGR_NO_CXX11_HDR_ARRAY
	{
		typedef std::array<int, 3> arr_type;

		arr_type obj_arr = yggr::to_array<std::array>(static_cast<native_arr_type&>(arr));
		yggr_test_assert(obj_arr[0] == arr[0]);
		yggr_test_assert(obj_arr[1] == arr[1]);
		yggr_test_assert(obj_arr[2] == arr[2]);
	}

	{
		typedef boost::array<int, 3> arr_type;

		arr_type obj_arr = yggr::to_array<boost::array>(arr);
		yggr_test_assert(obj_arr[0] == arr[0]);
		yggr_test_assert(obj_arr[1] == arr[1]);
		yggr_test_assert(obj_arr[2] == arr[2]);
	}

	{
		typedef yggr::array<int, 3> arr_type;

		arr_type obj_arr = yggr::to_array<yggr::array>(static_cast<const native_arr_type&>(arr));
		yggr_test_assert(obj_arr[0] == arr[0]);
		yggr_test_assert(obj_arr[1] == arr[1]);
		yggr_test_assert(obj_arr[2] == arr[2]);
	}

#endif // YGGR_NO_CXX11_HDR_ARRAY

}

void test_array_fill(void)
{
	{
		typedef int(arr_type)[3];

		arr_type arr = {0};

		yggr::fill(arr, 1);
		yggr_test_assert(arr[0] == 1);
		yggr_test_assert(arr[1] == 1);
		yggr_test_assert(arr[2] == 1);
	}

#ifndef YGGR_NO_CXX11_HDR_ARRAY
	{
		typedef std::array<int, 3> arr_type;

		arr_type arr = {0};

		yggr::fill(arr, 1);
		yggr_test_assert(arr[0] == 1);
		yggr_test_assert(arr[1] == 1);
		yggr_test_assert(arr[2] == 1);
	}
#endif // BOOST_NO_CXX11_RVALUE_REFERENCES

	{
		typedef boost::array<int, 3> arr_type;

		arr_type arr = {0};

		yggr::fill(arr, 1);
		yggr_test_assert(arr[0] == 1);
		yggr_test_assert(arr[1] == 1);
		yggr_test_assert(arr[2] == 1);
	}

	{
		typedef yggr::array<int, 3> arr_type;

		arr_type arr = {0};

		yggr::fill(arr, 1);
		yggr_test_assert(arr[0] == 1);
		yggr_test_assert(arr[1] == 1);
		yggr_test_assert(arr[2] == 1);
	}

}

void test_tuple_utility(void)
{
	{
		typedef int(arr_type)[3];
		BOOST_MPL_ASSERT((boost::mpl::bool_<(yggr::tuple_size<arr_type>::value == 3)>));
		BOOST_MPL_ASSERT((boost::is_same<yggr::tuple_element<0, arr_type>::type, int>));
		BOOST_MPL_ASSERT((boost::is_same<yggr::tuple_element<1, arr_type>::type, int>));
		BOOST_MPL_ASSERT((boost::is_same<yggr::tuple_element<2, arr_type>::type, int>));

#if TEST_COMPILE_ERROR
		BOOST_MPL_ASSERT((boost::is_same<yggr::tuple_element<3, arr_type>::type, int>));
#endif // TEST_COMPILE_ERROR
	}

#ifndef YGGR_NO_CXX11_HDR_ARRAY
	{
		typedef std::array<int, 3> arr_type;
		BOOST_MPL_ASSERT((boost::mpl::bool_<(yggr::tuple_size<arr_type>::value == 3)>));
		BOOST_MPL_ASSERT((boost::is_same<yggr::tuple_element<0, arr_type>::type, int>));
		BOOST_MPL_ASSERT((boost::is_same<yggr::tuple_element<1, arr_type>::type, int>));
		BOOST_MPL_ASSERT((boost::is_same<yggr::tuple_element<2, arr_type>::type, int>));

#if TEST_COMPILE_ERROR
		BOOST_MPL_ASSERT((boost::is_same<yggr::tuple_element<3, arr_type>::type, int>));
#endif // TEST_COMPILE_ERROR
	}
#endif // YGGR_NO_CXX11_HDR_ARRAY

	{
		typedef boost::array<int, 3> arr_type;
		BOOST_MPL_ASSERT((boost::mpl::bool_<(yggr::tuple_size<arr_type>::value == 3)>));
		BOOST_MPL_ASSERT((boost::is_same<yggr::tuple_element<0, arr_type>::type, int>));
		BOOST_MPL_ASSERT((boost::is_same<yggr::tuple_element<1, arr_type>::type, int>));
		BOOST_MPL_ASSERT((boost::is_same<yggr::tuple_element<2, arr_type>::type, int>));

#if TEST_COMPILE_ERROR
		BOOST_MPL_ASSERT((boost::is_same<yggr::tuple_element<3, arr_type>::type, int>));
#endif // TEST_COMPILE_ERROR
	}

	{
		typedef yggr::array<int, 3> arr_type;
		BOOST_MPL_ASSERT((boost::mpl::bool_<(yggr::tuple_size<arr_type>::value == 3)>));
		BOOST_MPL_ASSERT((boost::is_same<yggr::tuple_element<0, arr_type>::type, int>));
		BOOST_MPL_ASSERT((boost::is_same<yggr::tuple_element<1, arr_type>::type, int>));
		BOOST_MPL_ASSERT((boost::is_same<yggr::tuple_element<2, arr_type>::type, int>));

#if TEST_COMPILE_ERROR
		BOOST_MPL_ASSERT((boost::is_same<yggr::tuple_element<3, arr_type>::type, int>));
#endif // TEST_COMPILE_ERROR
	}

}


int main(int argc, char* argv[])
{
	test_get();
	test_to_array();
	test_array_fill();
	test_tuple_utility();

	wait_any_key(argc, argv);
	return 0;
}
