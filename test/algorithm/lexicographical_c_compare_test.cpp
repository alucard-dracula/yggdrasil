//  lexicographical_c_compare_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <yggr/algorithm/lexicographical_c_compare.hpp>

#include <yggr/container/vector.hpp>
#include <yggr/container/list.hpp>
#include <yggr/container/begin.hpp>
#include <yggr/container/end.hpp>
#include <yggr/container/rbegin.hpp>
#include <yggr/container/rend.hpp>

void test_arr_arr(void)
{
	yggr::s32 sarr1[] = {1, 2, 3, 4, 5};
	yggr::s32 sarr2[] = {1, 2, 3, 4, 5};
	yggr::s32 sarr3[] = {1, 2, 3, 4, 6};
	yggr::s32 sarr4[] = {1, 2, 3, 4, 5, 6};
	yggr::s32 sarr5[] = {1, 2, 3, 4, -1};

	yggr::u32 uarr1[] = {1, 2, 3, 4, 5};
	yggr::u32 uarr2[] = {1, 2, 3, 4, 5};
	yggr::u32 uarr3[] = {1, 2, 3, 4, 6};
	yggr::u32 uarr4[] = {1, 2, 3, 4, 5, 6};
	yggr::u32 uarr5[] = {1, 2, 3, 4, static_cast<yggr::u32>(-1)};

	{
		yggr_test_assert(0 == yggr::lexicographical_c_compare(yggr::begin(sarr1), yggr::end(sarr1), yggr::begin(sarr1), yggr::end(sarr1)));
		yggr_test_assert(0 == yggr::lexicographical_c_compare(yggr::cbegin(sarr1), yggr::cend(sarr1), yggr::begin(sarr2), yggr::end(sarr2)));
		yggr_test_assert(-1 == yggr::lexicographical_c_compare(yggr::cbegin(sarr1), yggr::cend(sarr1), yggr::begin(sarr3), yggr::end(sarr3)));
		yggr_test_assert(-1 == yggr::lexicographical_c_compare(yggr::cbegin(sarr1), yggr::cend(sarr1), yggr::begin(sarr4), yggr::end(sarr4)));
		yggr_test_assert(1 == yggr::lexicographical_c_compare(yggr::cbegin(sarr3), yggr::cend(sarr3), yggr::begin(sarr1), yggr::end(sarr1)));
		yggr_test_assert(1 == yggr::lexicographical_c_compare(yggr::cbegin(sarr3), yggr::cend(sarr3), yggr::begin(sarr4), yggr::end(sarr4)));
		yggr_test_assert(1 == yggr::lexicographical_c_compare(yggr::cbegin(sarr1), yggr::cend(sarr1), yggr::begin(sarr5), yggr::end(sarr5)));

		yggr_test_assert(0 == yggr::lexicographical_c_compare(yggr::begin(sarr1), yggr::end(sarr1), yggr::begin(sarr1), yggr::end(sarr1), yggr::c_hex_cmp()));
		yggr_test_assert(0 == yggr::lexicographical_c_compare(yggr::cbegin(sarr1), yggr::cend(sarr1), yggr::begin(sarr2), yggr::end(sarr2), yggr::c_hex_cmp()));
		yggr_test_assert(-1 == yggr::lexicographical_c_compare(yggr::cbegin(sarr1), yggr::cend(sarr1), yggr::begin(sarr3), yggr::end(sarr3), yggr::c_hex_cmp()));
		yggr_test_assert(-1 == yggr::lexicographical_c_compare(yggr::cbegin(sarr1), yggr::cend(sarr1), yggr::begin(sarr4), yggr::end(sarr4), yggr::c_hex_cmp()));
		yggr_test_assert(1 == yggr::lexicographical_c_compare(yggr::cbegin(sarr3), yggr::cend(sarr3), yggr::begin(sarr1), yggr::end(sarr1), yggr::c_hex_cmp()));
		yggr_test_assert(1 == yggr::lexicographical_c_compare(yggr::cbegin(sarr3), yggr::cend(sarr3), yggr::begin(sarr4), yggr::end(sarr4), yggr::c_hex_cmp()));
		yggr_test_assert(-1 == yggr::lexicographical_c_compare(yggr::cbegin(sarr1), yggr::cend(sarr1), yggr::begin(sarr5), yggr::end(sarr5), yggr::c_hex_cmp()));

		yggr_test_assert(0 == yggr::lexicographical_c_compare(yggr::begin(sarr1), yggr::end(sarr1), yggr::begin(sarr1), yggr::end(sarr1), yggr::c_mem_cmp()));
		yggr_test_assert(0 == yggr::lexicographical_c_compare(yggr::cbegin(sarr1), yggr::cend(sarr1), yggr::begin(sarr2), yggr::end(sarr2), yggr::c_mem_cmp()));
		yggr_test_assert(-1 == yggr::lexicographical_c_compare(yggr::cbegin(sarr1), yggr::cend(sarr1), yggr::begin(sarr3), yggr::end(sarr3), yggr::c_mem_cmp()));
		yggr_test_assert(-1 == yggr::lexicographical_c_compare(yggr::cbegin(sarr1), yggr::cend(sarr1), yggr::begin(sarr4), yggr::end(sarr4), yggr::c_mem_cmp()));
		yggr_test_assert(1 == yggr::lexicographical_c_compare(yggr::cbegin(sarr3), yggr::cend(sarr3), yggr::begin(sarr1), yggr::end(sarr1), yggr::c_mem_cmp()));
		yggr_test_assert(1 == yggr::lexicographical_c_compare(yggr::cbegin(sarr3), yggr::cend(sarr3), yggr::begin(sarr4), yggr::end(sarr4), yggr::c_mem_cmp()));
		yggr_test_assert(-1 == yggr::lexicographical_c_compare(yggr::cbegin(sarr1), yggr::cend(sarr1), yggr::begin(sarr5), yggr::end(sarr5), yggr::c_mem_cmp()));
	}

	{
		yggr_test_assert(0 == yggr::lexicographical_c_compare(yggr::begin(uarr1), yggr::end(uarr1), yggr::begin(uarr1), yggr::end(uarr1)));
		yggr_test_assert(0 == yggr::lexicographical_c_compare(yggr::cbegin(uarr1), yggr::cend(uarr1), yggr::begin(uarr2), yggr::end(uarr2)));
		yggr_test_assert(-1 == yggr::lexicographical_c_compare(yggr::cbegin(uarr1), yggr::cend(uarr1), yggr::begin(uarr3), yggr::end(uarr3)));
		yggr_test_assert(-1 == yggr::lexicographical_c_compare(yggr::cbegin(uarr1), yggr::cend(uarr1), yggr::begin(uarr4), yggr::end(uarr4)));
		yggr_test_assert(1 == yggr::lexicographical_c_compare(yggr::cbegin(uarr3), yggr::cend(uarr3), yggr::begin(uarr1), yggr::end(uarr1)));
		yggr_test_assert(1 == yggr::lexicographical_c_compare(yggr::cbegin(uarr3), yggr::cend(uarr3), yggr::begin(uarr4), yggr::end(uarr4)));
		yggr_test_assert(-1 == yggr::lexicographical_c_compare(yggr::cbegin(uarr1), yggr::cend(uarr1), yggr::begin(uarr5), yggr::end(uarr5)));

		yggr_test_assert(0 == yggr::lexicographical_c_compare(yggr::begin(uarr1), yggr::end(uarr1), yggr::begin(uarr1), yggr::end(uarr1), yggr::c_hex_cmp()));
		yggr_test_assert(0 == yggr::lexicographical_c_compare(yggr::cbegin(uarr1), yggr::cend(uarr1), yggr::begin(uarr2), yggr::end(uarr2), yggr::c_hex_cmp()));
		yggr_test_assert(-1 == yggr::lexicographical_c_compare(yggr::cbegin(uarr1), yggr::cend(uarr1), yggr::begin(uarr3), yggr::end(uarr3), yggr::c_hex_cmp()));
		yggr_test_assert(-1 == yggr::lexicographical_c_compare(yggr::cbegin(uarr1), yggr::cend(uarr1), yggr::begin(uarr4), yggr::end(uarr4), yggr::c_hex_cmp()));
		yggr_test_assert(1 == yggr::lexicographical_c_compare(yggr::cbegin(uarr3), yggr::cend(uarr3), yggr::begin(uarr1), yggr::end(uarr1), yggr::c_hex_cmp()));
		yggr_test_assert(1 == yggr::lexicographical_c_compare(yggr::cbegin(uarr3), yggr::cend(uarr3), yggr::begin(uarr4), yggr::end(uarr4), yggr::c_hex_cmp()));
		yggr_test_assert(-1 == yggr::lexicographical_c_compare(yggr::cbegin(uarr1), yggr::cend(uarr1), yggr::begin(uarr5), yggr::end(uarr5), yggr::c_hex_cmp()));

		yggr_test_assert(0 == yggr::lexicographical_c_compare(yggr::begin(uarr1), yggr::end(uarr1), yggr::begin(uarr1), yggr::end(uarr1), yggr::c_mem_cmp()));
		yggr_test_assert(0 == yggr::lexicographical_c_compare(yggr::cbegin(uarr1), yggr::cend(uarr1), yggr::begin(uarr2), yggr::end(uarr2), yggr::c_mem_cmp()));
		yggr_test_assert(-1 == yggr::lexicographical_c_compare(yggr::cbegin(uarr1), yggr::cend(uarr1), yggr::begin(uarr3), yggr::end(uarr3), yggr::c_mem_cmp()));
		yggr_test_assert(-1 == yggr::lexicographical_c_compare(yggr::cbegin(uarr1), yggr::cend(uarr1), yggr::begin(uarr4), yggr::end(uarr4), yggr::c_mem_cmp()));
		yggr_test_assert(1 == yggr::lexicographical_c_compare(yggr::cbegin(uarr3), yggr::cend(uarr3), yggr::begin(uarr1), yggr::end(uarr1), yggr::c_mem_cmp()));
		yggr_test_assert(1 == yggr::lexicographical_c_compare(yggr::cbegin(uarr3), yggr::cend(uarr3), yggr::begin(uarr4), yggr::end(uarr4), yggr::c_mem_cmp()));
		yggr_test_assert(-1 == yggr::lexicographical_c_compare(yggr::cbegin(uarr1), yggr::cend(uarr1), yggr::begin(uarr5), yggr::end(uarr5), yggr::c_mem_cmp()));
	}

	std::cout << "---------test_arr_arr end-------------" << std::endl;
}

void test_vt_vt(void)
{
	typedef yggr::vector<yggr::s32> s32_vt_type;
	typedef yggr::vector<yggr::u32> u32_vt_type;

	yggr::s32 sarr1[] = {1, 2, 3, 4, 5};
	yggr::s32 sarr2[] = {1, 2, 3, 4, 5};
	yggr::s32 sarr3[] = {1, 2, 3, 4, 6};
	yggr::s32 sarr4[] = {1, 2, 3, 4, 5, 6};
	yggr::s32 sarr5[] = {1, 2, 3, 4, -1};

	s32_vt_type svt1(yggr::begin(sarr1), yggr::end(sarr1));
	s32_vt_type svt2(yggr::begin(sarr2), yggr::end(sarr2));
	s32_vt_type svt3(yggr::begin(sarr3), yggr::end(sarr3));
	s32_vt_type svt4(yggr::begin(sarr4), yggr::end(sarr4));
	s32_vt_type svt5(yggr::begin(sarr5), yggr::end(sarr5));

	u32_vt_type uvt1(yggr::begin(sarr1), yggr::end(sarr1));
	u32_vt_type uvt2(yggr::begin(sarr2), yggr::end(sarr2));
	u32_vt_type uvt3(yggr::begin(sarr3), yggr::end(sarr3));
	u32_vt_type uvt4(yggr::begin(sarr4), yggr::end(sarr4));
	u32_vt_type uvt5(yggr::begin(sarr5), yggr::end(sarr5));

	{
		yggr_test_assert(0 == yggr::lexicographical_c_compare(yggr::begin(svt1), yggr::end(svt1), yggr::begin(svt1), yggr::end(svt1)));
		yggr_test_assert(0 == yggr::lexicographical_c_compare(yggr::cbegin(svt1), yggr::cend(svt1), yggr::begin(svt2), yggr::end(svt2)));
		yggr_test_assert(-1 == yggr::lexicographical_c_compare(yggr::cbegin(svt1), yggr::cend(svt1), yggr::begin(svt3), yggr::end(svt3)));
		yggr_test_assert(-1 == yggr::lexicographical_c_compare(yggr::cbegin(svt1), yggr::cend(svt1), yggr::begin(svt4), yggr::end(svt4)));
		yggr_test_assert(1 == yggr::lexicographical_c_compare(yggr::cbegin(svt3), yggr::cend(svt3), yggr::begin(svt1), yggr::end(svt1)));
		yggr_test_assert(1 == yggr::lexicographical_c_compare(yggr::cbegin(svt3), yggr::cend(svt3), yggr::begin(svt4), yggr::end(svt4)));
		yggr_test_assert(1 == yggr::lexicographical_c_compare(yggr::cbegin(svt1), yggr::cend(svt1), yggr::begin(svt5), yggr::end(svt5)));

		yggr_test_assert(0 == yggr::lexicographical_c_compare(yggr::begin(svt1), yggr::end(svt1), yggr::begin(svt1), yggr::end(svt1), yggr::c_hex_cmp()));
		yggr_test_assert(0 == yggr::lexicographical_c_compare(yggr::cbegin(svt1), yggr::cend(svt1), yggr::begin(svt2), yggr::end(svt2), yggr::c_hex_cmp()));
		yggr_test_assert(-1 == yggr::lexicographical_c_compare(yggr::cbegin(svt1), yggr::cend(svt1), yggr::begin(svt3), yggr::end(svt3), yggr::c_hex_cmp()));
		yggr_test_assert(-1 == yggr::lexicographical_c_compare(yggr::cbegin(svt1), yggr::cend(svt1), yggr::begin(svt4), yggr::end(svt4), yggr::c_hex_cmp()));
		yggr_test_assert(1 == yggr::lexicographical_c_compare(yggr::cbegin(svt3), yggr::cend(svt3), yggr::begin(svt1), yggr::end(svt1), yggr::c_hex_cmp()));
		yggr_test_assert(1 == yggr::lexicographical_c_compare(yggr::cbegin(svt3), yggr::cend(svt3), yggr::begin(svt4), yggr::end(svt4), yggr::c_hex_cmp()));
		yggr_test_assert(-1 == yggr::lexicographical_c_compare(yggr::cbegin(svt1), yggr::cend(svt1), yggr::begin(svt5), yggr::end(svt5), yggr::c_hex_cmp()));

		yggr_test_assert(0 == yggr::lexicographical_c_compare(yggr::begin(svt1), yggr::end(svt1), yggr::begin(svt1), yggr::end(svt1), yggr::c_mem_cmp()));
		yggr_test_assert(0 == yggr::lexicographical_c_compare(yggr::cbegin(svt1), yggr::cend(svt1), yggr::begin(svt2), yggr::end(svt2), yggr::c_mem_cmp()));
		yggr_test_assert(-1 == yggr::lexicographical_c_compare(yggr::cbegin(svt1), yggr::cend(svt1), yggr::begin(svt3), yggr::end(svt3), yggr::c_mem_cmp()));
		yggr_test_assert(-1 == yggr::lexicographical_c_compare(yggr::cbegin(svt1), yggr::cend(svt1), yggr::begin(svt4), yggr::end(svt4), yggr::c_mem_cmp()));
		yggr_test_assert(1 == yggr::lexicographical_c_compare(yggr::cbegin(svt3), yggr::cend(svt3), yggr::begin(svt1), yggr::end(svt1), yggr::c_mem_cmp()));
		yggr_test_assert(1 == yggr::lexicographical_c_compare(yggr::cbegin(svt3), yggr::cend(svt3), yggr::begin(svt4), yggr::end(svt4), yggr::c_mem_cmp()));
		yggr_test_assert(-1 == yggr::lexicographical_c_compare(yggr::cbegin(svt1), yggr::cend(svt1), yggr::begin(svt5), yggr::end(svt5), yggr::c_mem_cmp()));
	}

	{
		yggr_test_assert(0 == yggr::lexicographical_c_compare(yggr::begin(uvt1), yggr::end(uvt1), yggr::begin(uvt1), yggr::end(uvt1)));
		yggr_test_assert(0 == yggr::lexicographical_c_compare(yggr::cbegin(uvt1), yggr::cend(uvt1), yggr::begin(uvt2), yggr::end(uvt2)));
		yggr_test_assert(-1 == yggr::lexicographical_c_compare(yggr::cbegin(uvt1), yggr::cend(uvt1), yggr::begin(uvt3), yggr::end(uvt3)));
		yggr_test_assert(-1 == yggr::lexicographical_c_compare(yggr::cbegin(uvt1), yggr::cend(uvt1), yggr::begin(uvt4), yggr::end(uvt4)));
		yggr_test_assert(1 == yggr::lexicographical_c_compare(yggr::cbegin(uvt3), yggr::cend(uvt3), yggr::begin(uvt1), yggr::end(uvt1)));
		yggr_test_assert(1 == yggr::lexicographical_c_compare(yggr::cbegin(uvt3), yggr::cend(uvt3), yggr::begin(uvt4), yggr::end(uvt4)));
		yggr_test_assert(-1 == yggr::lexicographical_c_compare(yggr::cbegin(uvt1), yggr::cend(uvt1), yggr::begin(uvt5), yggr::end(uvt5)));

		yggr_test_assert(0 == yggr::lexicographical_c_compare(yggr::begin(uvt1), yggr::end(uvt1), yggr::begin(uvt1), yggr::end(uvt1), yggr::c_hex_cmp()));
		yggr_test_assert(0 == yggr::lexicographical_c_compare(yggr::cbegin(uvt1), yggr::cend(uvt1), yggr::begin(uvt2), yggr::end(uvt2), yggr::c_hex_cmp()));
		yggr_test_assert(-1 == yggr::lexicographical_c_compare(yggr::cbegin(uvt1), yggr::cend(uvt1), yggr::begin(uvt3), yggr::end(uvt3), yggr::c_hex_cmp()));
		yggr_test_assert(-1 == yggr::lexicographical_c_compare(yggr::cbegin(uvt1), yggr::cend(uvt1), yggr::begin(uvt4), yggr::end(uvt4), yggr::c_hex_cmp()));
		yggr_test_assert(1 == yggr::lexicographical_c_compare(yggr::cbegin(uvt3), yggr::cend(uvt3), yggr::begin(uvt1), yggr::end(uvt1), yggr::c_hex_cmp()));
		yggr_test_assert(1 == yggr::lexicographical_c_compare(yggr::cbegin(uvt3), yggr::cend(uvt3), yggr::begin(uvt4), yggr::end(uvt4), yggr::c_hex_cmp()));
		yggr_test_assert(-1 == yggr::lexicographical_c_compare(yggr::cbegin(uvt1), yggr::cend(uvt1), yggr::begin(uvt5), yggr::end(uvt5), yggr::c_hex_cmp()));

		yggr_test_assert(0 == yggr::lexicographical_c_compare(yggr::begin(uvt1), yggr::end(uvt1), yggr::begin(uvt1), yggr::end(uvt1), yggr::c_mem_cmp()));
		yggr_test_assert(0 == yggr::lexicographical_c_compare(yggr::cbegin(uvt1), yggr::cend(uvt1), yggr::begin(uvt2), yggr::end(uvt2), yggr::c_mem_cmp()));
		yggr_test_assert(-1 == yggr::lexicographical_c_compare(yggr::cbegin(uvt1), yggr::cend(uvt1), yggr::begin(uvt3), yggr::end(uvt3), yggr::c_mem_cmp()));
		yggr_test_assert(-1 == yggr::lexicographical_c_compare(yggr::cbegin(uvt1), yggr::cend(uvt1), yggr::begin(uvt4), yggr::end(uvt4), yggr::c_mem_cmp()));
		yggr_test_assert(1 == yggr::lexicographical_c_compare(yggr::cbegin(uvt3), yggr::cend(uvt3), yggr::begin(uvt1), yggr::end(uvt1), yggr::c_mem_cmp()));
		yggr_test_assert(1 == yggr::lexicographical_c_compare(yggr::cbegin(uvt3), yggr::cend(uvt3), yggr::begin(uvt4), yggr::end(uvt4), yggr::c_mem_cmp()));
		yggr_test_assert(-1 == yggr::lexicographical_c_compare(yggr::cbegin(uvt1), yggr::cend(uvt1), yggr::begin(uvt5), yggr::end(uvt5), yggr::c_mem_cmp()));
	}

	std::cout << "---------test_vt_vt end-------------" << std::endl;
}

void test_arr_vt(void)
{
	typedef yggr::vector<yggr::s32> s32_vt_type;
	typedef yggr::vector<yggr::u32> u32_vt_type;

	yggr::s32 sarr1[] = {1, 2, 3, 4, 5};
	yggr::s32 sarr2[] = {1, 2, 3, 4, 5};
	yggr::s32 sarr3[] = {1, 2, 3, 4, 6};
	yggr::s32 sarr4[] = {1, 2, 3, 4, 5, 6};
	yggr::s32 sarr5[] = {1, 2, 3, 4, -1};

	s32_vt_type svt1(yggr::begin(sarr1), yggr::end(sarr1));
	s32_vt_type svt2(yggr::begin(sarr2), yggr::end(sarr2));
	s32_vt_type svt3(yggr::begin(sarr3), yggr::end(sarr3));
	s32_vt_type svt4(yggr::begin(sarr4), yggr::end(sarr4));
	s32_vt_type svt5(yggr::begin(sarr5), yggr::end(sarr5));

	{
		yggr_test_assert(0 == yggr::lexicographical_c_compare(yggr::begin(sarr1), yggr::end(sarr1), yggr::begin(svt1), yggr::end(svt1)));
		yggr_test_assert(0 == yggr::lexicographical_c_compare(yggr::cbegin(sarr1), yggr::cend(sarr1), yggr::begin(svt2), yggr::end(svt2)));
		yggr_test_assert(-1 == yggr::lexicographical_c_compare(yggr::cbegin(sarr1), yggr::cend(sarr1), yggr::begin(svt3), yggr::end(svt3)));
		yggr_test_assert(-1 == yggr::lexicographical_c_compare(yggr::cbegin(sarr1), yggr::cend(sarr1), yggr::begin(svt4), yggr::end(svt4)));
		yggr_test_assert(1 == yggr::lexicographical_c_compare(yggr::cbegin(svt3), yggr::cend(svt3), yggr::begin(sarr1), yggr::end(sarr1)));
		yggr_test_assert(1 == yggr::lexicographical_c_compare(yggr::cbegin(svt3), yggr::cend(svt3), yggr::begin(svt4), yggr::end(svt4)));
		yggr_test_assert(1 == yggr::lexicographical_c_compare(yggr::cbegin(sarr1), yggr::cend(sarr1), yggr::begin(svt5), yggr::end(svt5)));

		yggr_test_assert(0 == yggr::lexicographical_c_compare(yggr::begin(sarr1), yggr::end(sarr1), yggr::begin(svt1), yggr::end(svt1), yggr::c_hex_cmp()));
		yggr_test_assert(0 == yggr::lexicographical_c_compare(yggr::cbegin(sarr1), yggr::cend(sarr1), yggr::begin(svt2), yggr::end(svt2), yggr::c_hex_cmp()));
		yggr_test_assert(-1 == yggr::lexicographical_c_compare(yggr::cbegin(sarr1), yggr::cend(sarr1), yggr::begin(svt3), yggr::end(svt3), yggr::c_hex_cmp()));
		yggr_test_assert(-1 == yggr::lexicographical_c_compare(yggr::cbegin(sarr1), yggr::cend(sarr1), yggr::begin(svt4), yggr::end(svt4), yggr::c_hex_cmp()));
		yggr_test_assert(1 == yggr::lexicographical_c_compare(yggr::cbegin(svt3), yggr::cend(svt3), yggr::begin(sarr1), yggr::end(sarr1), yggr::c_hex_cmp()));
		yggr_test_assert(1 == yggr::lexicographical_c_compare(yggr::cbegin(svt3), yggr::cend(svt3), yggr::begin(svt4), yggr::end(svt4), yggr::c_hex_cmp()));
		yggr_test_assert(-1 == yggr::lexicographical_c_compare(yggr::cbegin(sarr1), yggr::cend(sarr1), yggr::begin(svt5), yggr::end(svt5), yggr::c_hex_cmp()));

		yggr_test_assert(0 == yggr::lexicographical_c_compare(yggr::begin(sarr1), yggr::end(sarr1), yggr::begin(svt1), yggr::end(svt1), yggr::c_mem_cmp()));
		yggr_test_assert(0 == yggr::lexicographical_c_compare(yggr::cbegin(sarr1), yggr::cend(sarr1), yggr::begin(svt2), yggr::end(svt2), yggr::c_mem_cmp()));
		yggr_test_assert(-1 == yggr::lexicographical_c_compare(yggr::cbegin(sarr1), yggr::cend(sarr1), yggr::begin(svt3), yggr::end(svt3), yggr::c_mem_cmp()));
		yggr_test_assert(-1 == yggr::lexicographical_c_compare(yggr::cbegin(sarr1), yggr::cend(sarr1), yggr::begin(svt4), yggr::end(svt4), yggr::c_mem_cmp()));
		yggr_test_assert(1 == yggr::lexicographical_c_compare(yggr::cbegin(svt3), yggr::cend(svt3), yggr::begin(sarr1), yggr::end(sarr1), yggr::c_mem_cmp()));
		yggr_test_assert(1 == yggr::lexicographical_c_compare(yggr::cbegin(svt3), yggr::cend(svt3), yggr::begin(svt4), yggr::end(svt4), yggr::c_mem_cmp()));
		yggr_test_assert(-1 == yggr::lexicographical_c_compare(yggr::cbegin(sarr1), yggr::cend(sarr1), yggr::begin(svt5), yggr::end(svt5), yggr::c_mem_cmp()));
	}

	std::cout << "---------test_arr_vt end-------------" << std::endl;
}

void test_lst_lst(void)
{
	typedef yggr::list<yggr::s32> s32_lst_type;
	typedef yggr::list<yggr::u32> u32_lst_type;

	yggr::s32 sarr1[] = {1, 2, 3, 4, 5};
	yggr::s32 sarr2[] = {1, 2, 3, 4, 5};
	yggr::s32 sarr3[] = {1, 2, 3, 4, 6};
	yggr::s32 sarr4[] = {1, 2, 3, 4, 5, 6};
	yggr::s32 sarr5[] = {1, 2, 3, 4, -1};

	s32_lst_type slst1(yggr::begin(sarr1), yggr::end(sarr1));
	s32_lst_type slst2(yggr::begin(sarr2), yggr::end(sarr2));
	s32_lst_type slst3(yggr::begin(sarr3), yggr::end(sarr3));
	s32_lst_type slst4(yggr::begin(sarr4), yggr::end(sarr4));
	s32_lst_type slst5(yggr::begin(sarr5), yggr::end(sarr5));

	u32_lst_type ulst1(yggr::begin(sarr1), yggr::end(sarr1));
	u32_lst_type ulst2(yggr::begin(sarr2), yggr::end(sarr2));
	u32_lst_type ulst3(yggr::begin(sarr3), yggr::end(sarr3));
	u32_lst_type ulst4(yggr::begin(sarr4), yggr::end(sarr4));
	u32_lst_type ulst5(yggr::begin(sarr5), yggr::end(sarr5));

	{
		yggr_test_assert(0 == yggr::lexicographical_c_compare(yggr::begin(slst1), yggr::end(slst1), yggr::begin(slst1), yggr::end(slst1)));
		yggr_test_assert(0 == yggr::lexicographical_c_compare(yggr::cbegin(slst1), yggr::cend(slst1), yggr::begin(slst2), yggr::end(slst2)));
		yggr_test_assert(-1 == yggr::lexicographical_c_compare(yggr::cbegin(slst1), yggr::cend(slst1), yggr::begin(slst3), yggr::end(slst3)));
		yggr_test_assert(-1 == yggr::lexicographical_c_compare(yggr::cbegin(slst1), yggr::cend(slst1), yggr::begin(slst4), yggr::end(slst4)));
		yggr_test_assert(1 == yggr::lexicographical_c_compare(yggr::cbegin(slst3), yggr::cend(slst3), yggr::begin(slst1), yggr::end(slst1)));
		yggr_test_assert(1 == yggr::lexicographical_c_compare(yggr::cbegin(slst3), yggr::cend(slst3), yggr::begin(slst4), yggr::end(slst4)));
		yggr_test_assert(1 == yggr::lexicographical_c_compare(yggr::cbegin(slst1), yggr::cend(slst1), yggr::begin(slst5), yggr::end(slst5)));

		yggr_test_assert(0 == yggr::lexicographical_c_compare(yggr::begin(slst1), yggr::end(slst1), yggr::begin(slst1), yggr::end(slst1), yggr::c_hex_cmp()));
		yggr_test_assert(0 == yggr::lexicographical_c_compare(yggr::cbegin(slst1), yggr::cend(slst1), yggr::begin(slst2), yggr::end(slst2), yggr::c_hex_cmp()));
		yggr_test_assert(-1 == yggr::lexicographical_c_compare(yggr::cbegin(slst1), yggr::cend(slst1), yggr::begin(slst3), yggr::end(slst3), yggr::c_hex_cmp()));
		yggr_test_assert(-1 == yggr::lexicographical_c_compare(yggr::cbegin(slst1), yggr::cend(slst1), yggr::begin(slst4), yggr::end(slst4), yggr::c_hex_cmp()));
		yggr_test_assert(1 == yggr::lexicographical_c_compare(yggr::cbegin(slst3), yggr::cend(slst3), yggr::begin(slst1), yggr::end(slst1), yggr::c_hex_cmp()));
		yggr_test_assert(1 == yggr::lexicographical_c_compare(yggr::cbegin(slst3), yggr::cend(slst3), yggr::begin(slst4), yggr::end(slst4), yggr::c_hex_cmp()));
		yggr_test_assert(-1 == yggr::lexicographical_c_compare(yggr::cbegin(slst1), yggr::cend(slst1), yggr::begin(slst5), yggr::end(slst5), yggr::c_hex_cmp()));

		// force using yggr::c_mem_cmp to list is undefined
		//yggr_test_assert(0 == yggr::lexicographical_c_compare(yggr::begin(slst1), yggr::end(slst1), yggr::begin(slst1), yggr::end(slst1), yggr::c_mem_cmp()));
		//yggr_test_assert(0 == yggr::lexicographical_c_compare(yggr::cbegin(slst1), yggr::cend(slst1), yggr::begin(slst2), yggr::end(slst2), yggr::c_mem_cmp()));
		//yggr_test_assert(-1 == yggr::lexicographical_c_compare(yggr::cbegin(slst1), yggr::cend(slst1), yggr::begin(slst3), yggr::end(slst3), yggr::c_mem_cmp()));
		//yggr_test_assert(-1 == yggr::lexicographical_c_compare(yggr::cbegin(slst1), yggr::cend(slst1), yggr::begin(slst4), yggr::end(slst4), yggr::c_mem_cmp()));
		//yggr_test_assert(1 == yggr::lexicographical_c_compare(yggr::cbegin(slst3), yggr::cend(slst3), yggr::begin(slst1), yggr::end(slst1), yggr::c_mem_cmp()));
		//yggr_test_assert(1 == yggr::lexicographical_c_compare(yggr::cbegin(slst3), yggr::cend(slst3), yggr::begin(slst4), yggr::end(slst4), yggr::c_mem_cmp()));
		//yggr_test_assert(-1 == yggr::lexicographical_c_compare(yggr::cbegin(slst1), yggr::cend(slst1), yggr::begin(slst5), yggr::end(slst5), yggr::c_mem_cmp()));
	}

	{
		yggr_test_assert(0 == yggr::lexicographical_c_compare(yggr::begin(ulst1), yggr::end(ulst1), yggr::begin(ulst1), yggr::end(ulst1)));
		yggr_test_assert(0 == yggr::lexicographical_c_compare(yggr::cbegin(ulst1), yggr::cend(ulst1), yggr::begin(ulst2), yggr::end(ulst2)));
		yggr_test_assert(-1 == yggr::lexicographical_c_compare(yggr::cbegin(ulst1), yggr::cend(ulst1), yggr::begin(ulst3), yggr::end(ulst3)));
		yggr_test_assert(-1 == yggr::lexicographical_c_compare(yggr::cbegin(ulst1), yggr::cend(ulst1), yggr::begin(ulst4), yggr::end(ulst4)));
		yggr_test_assert(1 == yggr::lexicographical_c_compare(yggr::cbegin(ulst3), yggr::cend(ulst3), yggr::begin(ulst1), yggr::end(ulst1)));
		yggr_test_assert(1 == yggr::lexicographical_c_compare(yggr::cbegin(ulst3), yggr::cend(ulst3), yggr::begin(ulst4), yggr::end(ulst4)));
		yggr_test_assert(-1 == yggr::lexicographical_c_compare(yggr::cbegin(ulst1), yggr::cend(ulst1), yggr::begin(ulst5), yggr::end(ulst5)));

		yggr_test_assert(0 == yggr::lexicographical_c_compare(yggr::begin(ulst1), yggr::end(ulst1), yggr::begin(ulst1), yggr::end(ulst1), yggr::c_hex_cmp()));
		yggr_test_assert(0 == yggr::lexicographical_c_compare(yggr::cbegin(ulst1), yggr::cend(ulst1), yggr::begin(ulst2), yggr::end(ulst2), yggr::c_hex_cmp()));
		yggr_test_assert(-1 == yggr::lexicographical_c_compare(yggr::cbegin(ulst1), yggr::cend(ulst1), yggr::begin(ulst3), yggr::end(ulst3), yggr::c_hex_cmp()));
		yggr_test_assert(-1 == yggr::lexicographical_c_compare(yggr::cbegin(ulst1), yggr::cend(ulst1), yggr::begin(ulst4), yggr::end(ulst4), yggr::c_hex_cmp()));
		yggr_test_assert(1 == yggr::lexicographical_c_compare(yggr::cbegin(ulst3), yggr::cend(ulst3), yggr::begin(ulst1), yggr::end(ulst1), yggr::c_hex_cmp()));
		yggr_test_assert(1 == yggr::lexicographical_c_compare(yggr::cbegin(ulst3), yggr::cend(ulst3), yggr::begin(ulst4), yggr::end(ulst4), yggr::c_hex_cmp()));
		yggr_test_assert(-1 == yggr::lexicographical_c_compare(yggr::cbegin(ulst1), yggr::cend(ulst1), yggr::begin(ulst5), yggr::end(ulst5), yggr::c_hex_cmp()));

		// force using yggr::c_mem_cmp to list is undefined
		//yggr_test_assert(0 == yggr::lexicographical_c_compare(yggr::begin(ulst1), yggr::end(ulst1), yggr::begin(ulst1), yggr::end(ulst1), yggr::c_mem_cmp()));
		//yggr_test_assert(0 == yggr::lexicographical_c_compare(yggr::cbegin(ulst1), yggr::cend(ulst1), yggr::begin(ulst2), yggr::end(ulst2), yggr::c_mem_cmp()));
		//yggr_test_assert(-1 == yggr::lexicographical_c_compare(yggr::cbegin(ulst1), yggr::cend(ulst1), yggr::begin(ulst3), yggr::end(ulst3), yggr::c_mem_cmp()));
		//yggr_test_assert(-1 == yggr::lexicographical_c_compare(yggr::cbegin(ulst1), yggr::cend(ulst1), yggr::begin(ulst4), yggr::end(ulst4), yggr::c_mem_cmp()));
		//yggr_test_assert(1 == yggr::lexicographical_c_compare(yggr::cbegin(ulst3), yggr::cend(ulst3), yggr::begin(ulst1), yggr::end(ulst1), yggr::c_mem_cmp()));
		//yggr_test_assert(1 == yggr::lexicographical_c_compare(yggr::cbegin(ulst3), yggr::cend(ulst3), yggr::begin(ulst4), yggr::end(ulst4), yggr::c_mem_cmp()));
		//yggr_test_assert(-1 == yggr::lexicographical_c_compare(yggr::cbegin(ulst1), yggr::cend(ulst1), yggr::begin(ulst5), yggr::end(ulst5), yggr::c_mem_cmp()));
	}

	std::cout << "---------test_lst_lst end-------------" << std::endl;
}

void test_lst_vt(void)
{
	typedef yggr::vector<yggr::s32> s32_vt_type;
	typedef yggr::list<yggr::s32> s32_list_type;

	yggr::s32 sarr1[] = {1, 2, 3, 4, 5};
	yggr::s32 sarr2[] = {1, 2, 3, 4, 5};
	yggr::s32 sarr3[] = {1, 2, 3, 4, 6};
	yggr::s32 sarr4[] = {1, 2, 3, 4, 5, 6};
	yggr::s32 sarr5[] = {1, 2, 3, 4, -1};

	s32_vt_type svt1(yggr::begin(sarr1), yggr::end(sarr1));
	s32_vt_type svt2(yggr::begin(sarr2), yggr::end(sarr2));
	s32_vt_type svt3(yggr::begin(sarr3), yggr::end(sarr3));
	s32_vt_type svt4(yggr::begin(sarr4), yggr::end(sarr4));
	s32_vt_type svt5(yggr::begin(sarr5), yggr::end(sarr5));

	s32_list_type slst1(yggr::begin(sarr1), yggr::end(sarr1));
	s32_list_type slst2(yggr::begin(sarr2), yggr::end(sarr2));
	s32_list_type slst3(yggr::begin(sarr3), yggr::end(sarr3));
	s32_list_type slst4(yggr::begin(sarr4), yggr::end(sarr4));
	s32_list_type slst5(yggr::begin(sarr5), yggr::end(sarr5));

	{
		yggr_test_assert(0 == yggr::lexicographical_c_compare(yggr::begin(svt1), yggr::end(svt1), yggr::begin(slst1), yggr::end(slst1)));
		yggr_test_assert(0 == yggr::lexicographical_c_compare(yggr::cbegin(svt1), yggr::cend(svt1), yggr::begin(slst2), yggr::end(slst2)));
		yggr_test_assert(-1 == yggr::lexicographical_c_compare(yggr::cbegin(svt1), yggr::cend(svt1), yggr::begin(slst3), yggr::end(slst3)));
		yggr_test_assert(-1 == yggr::lexicographical_c_compare(yggr::cbegin(svt1), yggr::cend(svt1), yggr::begin(slst4), yggr::end(slst4)));
		yggr_test_assert(1 == yggr::lexicographical_c_compare(yggr::cbegin(slst3), yggr::cend(slst3), yggr::begin(svt1), yggr::end(svt1)));
		yggr_test_assert(1 == yggr::lexicographical_c_compare(yggr::cbegin(slst3), yggr::cend(slst3), yggr::begin(slst4), yggr::end(slst4)));
		yggr_test_assert(1 == yggr::lexicographical_c_compare(yggr::cbegin(svt1), yggr::cend(svt1), yggr::begin(slst5), yggr::end(slst5)));

		yggr_test_assert(0 == yggr::lexicographical_c_compare(yggr::begin(svt1), yggr::end(svt1), yggr::begin(slst1), yggr::end(slst1), yggr::c_hex_cmp()));
		yggr_test_assert(0 == yggr::lexicographical_c_compare(yggr::cbegin(svt1), yggr::cend(svt1), yggr::begin(slst2), yggr::end(slst2), yggr::c_hex_cmp()));
		yggr_test_assert(-1 == yggr::lexicographical_c_compare(yggr::cbegin(svt1), yggr::cend(svt1), yggr::begin(slst3), yggr::end(slst3), yggr::c_hex_cmp()));
		yggr_test_assert(-1 == yggr::lexicographical_c_compare(yggr::cbegin(svt1), yggr::cend(svt1), yggr::begin(slst4), yggr::end(slst4), yggr::c_hex_cmp()));
		yggr_test_assert(1 == yggr::lexicographical_c_compare(yggr::cbegin(slst3), yggr::cend(slst3), yggr::begin(svt1), yggr::end(svt1), yggr::c_hex_cmp()));
		yggr_test_assert(1 == yggr::lexicographical_c_compare(yggr::cbegin(slst3), yggr::cend(slst3), yggr::begin(slst4), yggr::end(slst4), yggr::c_hex_cmp()));
		yggr_test_assert(-1 == yggr::lexicographical_c_compare(yggr::cbegin(svt1), yggr::cend(svt1), yggr::begin(slst5), yggr::end(slst5), yggr::c_hex_cmp()));
	}

	std::cout << "---------test_lst_vt end-------------" << std::endl;
}

int main(int argc, char* argv[])
{
	test_arr_arr();
	test_vt_vt();
	test_arr_vt();
	test_lst_lst();
	test_lst_vt();

	std::cout << "------------all test end-------------" << std::endl;

	wait_any_key(argc, argv);
	return 0;
}
