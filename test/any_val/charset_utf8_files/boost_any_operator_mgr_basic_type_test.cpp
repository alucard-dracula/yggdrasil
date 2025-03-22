// boost_any_operator_mgr_basic_type_test.cpp

#if defined(_MSC_VER)
#	error "this file not supports MSVC only, if you are using other compilers, please use the same name file in the ../"
#endif // _MSC_VER

#include <test/wait_any_key/wait_any_key.hpp>

#include <yggr/charset/utf8_string.hpp>
#include <yggr/charset/utf8_string_view.hpp>

#include <yggr/any_val/any_operator_mgr_boost_basic_t_reg.hpp>
#include <yggr/any_val/any_set.hpp>
#include <yggr/math/flaw_comparer.hpp>

#include <cassert>

#include <yggr/compile_link/linker.hpp>


#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

#include YGGR_PP_LINK_LIB(any_val)
#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(base)

// test_negate

void test_negate_1(void)
{
	std::cout << "----------test_negate_1 S---------------" << std::endl;
	boost::any a1(ANY_SET(yggr::s32, 10));
	boost::any a2 = -a1;

	std::cout << yggr::any_cast<yggr::s32>(a2) <<std::endl;

	yggr_test_assert(yggr::any_cast<yggr::s32>(a2) == -10);

	std::cout << "----------test_negate_1 E---------------" << std::endl;
}

void test_negate_2(void)
{
	std::cout << "----------test_negate_2 S---------------" << std::endl;
	boost::any a1(ANY_SET(yggr::f32, 10));
	boost::any a2 = -a1;

	std::cout << yggr::any_cast<yggr::f32>(a2) <<std::endl;

	yggr_test_assert(yggr::any_cast<yggr::f32>(a2) == -10.0f);

	std::cout << "----------test_negate_2 E---------------" << std::endl;
}

// test_before_plus_plus
void test_before_plus_plus_1(void)
{
	std::cout << "----------test_before_plus_plus_1 S---------------" << std::endl;
	boost::any a1(ANY_SET(yggr::s32, 10));
	boost::any a2 = ++a1;

	std::cout << yggr::any_cast<yggr::s32>(a1) <<std::endl;

	yggr_test_assert(yggr::any_cast<yggr::s32>(a2) == yggr::any_cast<yggr::s32>(a1));

	std::cout << "----------test_before_plus_plus_1 E---------------" << std::endl;
}

void test_before_plus_plus_2(void)
{
	std::cout << "----------test_before_plus_plus_2 S---------------" << std::endl;
	boost::any a1(ANY_SET(yggr::f32, 10));
	boost::any a2 = ++a1;

	std::cout << yggr::any_cast<yggr::f32>(a1) <<std::endl;

	yggr_test_assert(yggr::any_cast<yggr::f32>(a2) == yggr::any_cast<yggr::f32>(a1));

	std::cout << "----------test_before_plus_plus_2 E---------------" << std::endl;
}

// test_after_plus_plus
void test_after_plus_plus_1(void)
{
	std::cout << "----------test_after_plus_plus_1 S---------------" << std::endl;
	boost::any a1(ANY_SET(yggr::s32, 10));
	boost::any a2 = a1++;

	std::cout << yggr::any_cast<yggr::s32>(a1) <<std::endl;

	yggr_test_assert(yggr::any_cast<yggr::s32>(a2) + 1 == yggr::any_cast<yggr::s32>(a1));

	std::cout << "----------test_after_plus_plus_1 E---------------" << std::endl;
}

void test_after_plus_plus_2(void)
{
	std::cout << "----------test_after_plus_plus_2 S---------------" << std::endl;
	boost::any a1(ANY_SET(yggr::f32, 10));
	boost::any a2 = a1++;

	std::cout << yggr::any_cast<yggr::f32>(a1) <<std::endl;

	yggr_test_assert(yggr::any_cast<yggr::f32>(a2) + 1.0f == yggr::any_cast<yggr::f32>(a1));

	std::cout << "----------test_after_plus_plus_2 E---------------" << std::endl;
}

// test_before_minus_minus
void test_before_minus_minus_1(void)
{
	std::cout << "----------test_before_minus_minus_1 S---------------" << std::endl;
	boost::any a1(ANY_SET(yggr::s32, 10));
	boost::any a2 = --a1;

	std::cout << yggr::any_cast<yggr::s32>(a1) <<std::endl;

	yggr_test_assert(yggr::any_cast<yggr::s32>(a2) == yggr::any_cast<yggr::s32>(a1));

	std::cout << "----------test_before_minus_minus_1 E---------------" << std::endl;
}

void test_before_minus_minus_2(void)
{
	std::cout << "----------test_before_minus_minus_2 S---------------" << std::endl;
	boost::any a1(ANY_SET(yggr::f32, 10));
	boost::any a2 = --a1;

	std::cout << yggr::any_cast<yggr::f32>(a1) <<std::endl;

	yggr_test_assert(yggr::any_cast<yggr::f32>(a2) == yggr::any_cast<yggr::f32>(a1));

	std::cout << "----------test_before_minus_minus_2 E---------------" << std::endl;
}

// test_after_minus_minus
void test_after_minus_minus_1(void)
{
	std::cout << "----------test_after_minus_minus_1 S---------------" << std::endl;
	boost::any a1(ANY_SET(yggr::s32, 10));
	boost::any a2 = a1--;

	std::cout << yggr::any_cast<yggr::s32>(a1) <<std::endl;

	yggr_test_assert(yggr::any_cast<yggr::s32>(a2) - 1 == yggr::any_cast<yggr::s32>(a1));

	std::cout << "----------test_after_minus_minus_1 E---------------" << std::endl;
}

void test_after_minus_minus_2(void)
{
	std::cout << "----------test_after_minus_minus_1 S---------------" << std::endl;
	boost::any a1(ANY_SET(yggr::f32, 10));
	boost::any a2 = a1--;

	std::cout << yggr::any_cast<yggr::f32>(a1) <<std::endl;

	yggr_test_assert(yggr::any_cast<yggr::f32>(a2) - 1.0f == yggr::any_cast<yggr::f32>(a1));

	std::cout << "----------test_after_minus_minus_1 E---------------" << std::endl;
}

//----set----------

void test_set_1(void)
{
	std::cout << "----------test_set_1 S---------------" << std::endl;
	boost::any a1(ANY_SET(yggr::s32, 10));
	boost::any a2(ANY_SET(yggr::s32, 20));

	boost::any a3 = operator_set(a1, a2);
	std::cout << yggr::any_cast<yggr::s32>(a3) << std::endl;

	yggr_test_assert(yggr::any_cast<yggr::s32>(a2) == yggr::any_cast<yggr::s32>(a1));
	yggr_test_assert(yggr::any_cast<yggr::s32>(a3) == yggr::any_cast<yggr::s32>(a1));
	std::cout << "----------test_set_1 E---------------" << std::endl;
}

void test_set_2(void)
{
	std::cout << "----------test_set_2 S---------------" << std::endl;
	boost::any a1(ANY_SET(std::string, "Hello "));
	boost::any a2(ANY_SET(std::string, "World"));

	boost::any a3 = operator_set(a1, a2);
	std::cout << yggr::any_cast<std::string>(a3) << std::endl;
	yggr_test_assert(yggr::any_cast<std::string>(a3) == yggr::any_cast<std::string>(a1));
	yggr_test_assert(yggr::any_cast<std::string>(a2) == yggr::any_cast<std::string>(a1));
	std::cout << "----------test_set_2 E---------------" << std::endl;
}

void test_set_3(void)
{
	std::cout << "----------test_set_3 S---------------" << std::endl;
	boost::any a1(yggr::charset::utf8_string(yggr::utf8_string_view("你好 ")));
	boost::any a2(yggr::charset::utf8_string(yggr::utf8_string_view("世界 utf8")));

	boost::any a3 = operator_set(a1, a2);
	std::cout << yggr::any_cast<yggr::charset::utf8_string>(a3) << std::endl;

	yggr_test_assert(yggr::any_cast<yggr::charset::utf8_string>(a3) == yggr::any_cast<yggr::charset::utf8_string>(a1));
	yggr_test_assert(yggr::any_cast<yggr::charset::utf8_string>(a2) == yggr::any_cast<yggr::charset::utf8_string>(a1));
	std::cout << "----------test_set_3 E---------------" << std::endl;
}

void test_set_4(void)
{
	std::cout << "----------test_set_4 S---------------" << std::endl;
	boost::any a1(ANY_SET(yggr::s32, 10));
	boost::any a2(ANY_SET(yggr::f32, 20.5));

	boost::any a3 = operator_set(a1, a2);
	std::cout << yggr::any_cast<yggr::s32>(a3) << std::endl;

	yggr_test_assert(yggr::any_cast<yggr::f32>(a2) - 0.5f == yggr::any_cast<yggr::s32>(a1));
	yggr_test_assert(yggr::any_cast<yggr::s32>(a3) == yggr::any_cast<yggr::s32>(a1));

	boost::any a4(ANY_SET(std::string, "100"));
	boost::any a5 = operator_set(a3, a4);

	std::cout << a5.type().name() << std::endl;
	std::cout << a3.type().name() << std::endl;

	std::cout << "----------test_set_4 E---------------" << std::endl;
}

void test_set_5(void)
{
	std::cout << "----------test_set_5 S---------------" << std::endl;
	boost::any a1(ANY_SET(yggr::charset::utf8_string, yggr::utf8_string_view("你好")));
	boost::any a2(ANY_SET(std::string, yggr::utf8_string_view("stl世界").str<std::string>()));
	boost::any a3(ANY_SET(boost::container::string, yggr::utf8_string_view("boost世界").str<boost::container::string>()));

	boost::any a4 = operator_set(a1, a2);
	boost::any a5 = operator_set(a1, a3);

	std::cout << yggr::any_cast<yggr::charset::utf8_string>(a4) << std::endl;
	std::cout << yggr::any_cast<yggr::charset::utf8_string>(a5) << std::endl;

	std::cout << a4.type().name() << std::endl;
	std::cout << a5.type().name() << std::endl;

	std::cout << "----------test_set_5 E---------------" << std::endl;
}


//---------plus-------
void test_plus_1(void)
{
	std::cout << "----------test_plus_1 S---------------" << std::endl;
	boost::any a1(ANY_SET(yggr::s32, 10));
	boost::any a2(ANY_SET(yggr::s32, 20));

	boost::any a3 = a1 + a2;
	std::cout << yggr::any_cast<yggr::s32>(a3) << std::endl;

	yggr_test_assert(yggr::any_cast<yggr::s32>(a3) == 10 + 20);
	std::cout << "----------test_plus_1 E---------------" << std::endl;
}

void test_plus_2(void)
{
	std::cout << "----------test_plus_2 S---------------" << std::endl;
	boost::any a1(ANY_SET(std::string, "Hello "));
	boost::any a2(ANY_SET(std::string, "World"));

	boost::any a3 = a1 + a2;
	std::cout << yggr::any_cast<std::string>(a3) << std::endl;
	yggr_test_assert(yggr::any_cast<std::string>(a3) == std::string("Hello ") + std::string("World"));
	std::cout << "----------test_plus_2 E---------------" << std::endl;
}

void test_plus_3(void)
{
	std::cout << "----------test_plus_3 S---------------" << std::endl;
	boost::any a1(yggr::charset::utf8_string(yggr::utf8_string_view("你好 ")));
	boost::any a2(yggr::charset::utf8_string(yggr::utf8_string_view("世界 utf8")));

	boost::any a3 = a1 + a2;
	std::cout << yggr::any_cast<yggr::charset::utf8_string>(a3) << std::endl;

	yggr_test_assert(yggr::any_cast<yggr::charset::utf8_string>(a3)
			== yggr::charset::utf8_string(yggr::utf8_string_view("你好 "))
                   + yggr::charset::utf8_string(yggr::utf8_string_view("世界 utf8")));
	std::cout << "----------test_plus_3 E---------------" << std::endl;
}

void test_plus_4(void)
{
	std::cout << "----------test_plus_4 S---------------" << std::endl;
	boost::any a1(ANY_SET(yggr::s32, 10));
	boost::any a2(ANY_SET(yggr::f32, 20.0));

	boost::any a3 = a1 + a2;
	std::cout << yggr::any_cast<BOOST_TYPEOF(yggr::s32() + yggr::f32())>(a3) << std::endl;

	yggr_test_assert(yggr::any_cast<yggr::f32>(a3) == 10 + 20.0f);

	boost::any a4(ANY_SET(std::string, "100"));
	boost::any a5 = a3 + a4;

	std::cout << a5.type().name() << std::endl;

	std::cout << "----------test_plus_4 E---------------" << std::endl;
}

//---------plus set-------
void test_plus_set_1(void)
{
	std::cout << "----------test_plus_set_1 S---------------" << std::endl;
	boost::any a1(ANY_SET(yggr::s32, 10));
	boost::any a2(ANY_SET(yggr::s32, 20));

	a1 += a2;
	std::cout << yggr::any_cast<yggr::s32>(a1) << std::endl;

	yggr_test_assert(yggr::any_cast<yggr::s32>(a1) == 10 + 20);
	std::cout << "----------test_plus_set_1 E---------------" << std::endl;
}

void test_plus_set_2(void)
{
	std::cout << "----------test_plus_set_2 S---------------" << std::endl;
	boost::any a1(ANY_SET(std::string, "Hello "));
	boost::any a2(ANY_SET(std::string, "World"));

	a1 += a2;
	std::cout << yggr::any_cast<std::string>(a1) << std::endl;
	yggr_test_assert(yggr::any_cast<std::string>(a1) == std::string("Hello ") + std::string("World"));
	std::cout << "----------test_plus_set_2 E---------------" << std::endl;
}

void test_plus_set_3(void)
{
	std::cout << "----------test_plus_set_3 S---------------" << std::endl;
	boost::any a1(yggr::charset::utf8_string(yggr::utf8_string_view("你好 ")));
	boost::any a2(yggr::charset::utf8_string(yggr::utf8_string_view("世界 utf8")));

	a1 += a2;
	std::cout << yggr::any_cast<yggr::charset::utf8_string>(a1) << std::endl;

	yggr_test_assert(yggr::any_cast<yggr::charset::utf8_string>(a1)
			== yggr::charset::utf8_string(yggr::utf8_string_view("你好 ")) + yggr::charset::utf8_string(yggr::utf8_string_view("世界 utf8")));
	std::cout << "----------test_plus_set_3 E---------------" << std::endl;
}

void test_plus_set_4(void)
{
	std::cout << "----------test_plus_set_4 S---------------" << std::endl;
	boost::any a1(ANY_SET(yggr::s32, 10));
	boost::any a2(ANY_SET(yggr::f32, 20.5));

	a1 += a2;
	std::cout << yggr::any_cast<yggr::s32>(a1) << std::endl;

	yggr_test_assert(yggr::any_cast<yggr::s32>(a1) == static_cast<yggr::s32>(10 + 20.5f));

	boost::any a4(ANY_SET(std::string, "100"));
	a4 += a1;

	std::cout << a4.type().name() << std::endl;

	std::cout << "----------test_plus_set_4 E---------------" << std::endl;
}

//---------minus-------
void test_minus_1(void)
{
	std::cout << "----------test_minus_1 S---------------" << std::endl;
	boost::any a1(ANY_SET(yggr::s32, 10));
	boost::any a2(ANY_SET(yggr::s32, 20));

	boost::any a3 = a1 - a2;
	std::cout << yggr::any_cast<yggr::s32>(a3) << std::endl;

	yggr_test_assert(yggr::any_cast<yggr::s32>(a3) == 10 - 20);
	std::cout << "----------test_minus_1 E---------------" << std::endl;
}

void test_minus_2(void)
{
	std::cout << "----------test_minus_2 S---------------" << std::endl;
	boost::any a1(ANY_SET(std::string, "Hello "));
	boost::any a2(ANY_SET(std::string, "World"));

	boost::any a3 = a1 - a2;
	std::cout << a3.type().name() << std::endl;
	yggr_test_assert(a3.empty());
	std::cout << "----------test_minus_2 E---------------" << std::endl;
}

void test_minus_3(void)
{
	std::cout << "----------test_minus_3 S---------------" << std::endl;
	boost::any a1(yggr::charset::utf8_string(yggr::utf8_string_view("你好 ")));
	boost::any a2(yggr::charset::utf8_string(yggr::utf8_string_view("世界 utf8")));

	boost::any a3 = a1 - a2;
	std::cout << a3.type().name() << std::endl;
	yggr_test_assert(a3.empty());

	std::cout << "----------test_minus_3 E---------------" << std::endl;
}

void test_minus_4(void)
{
	std::cout << "----------test_minus_4 S---------------" << std::endl;
	boost::any a1(ANY_SET(yggr::s32, 10));
	boost::any a2(ANY_SET(yggr::f32, 20.5));

	boost::any a3 = a1 - a2;
	std::cout << yggr::any_cast<BOOST_TYPEOF(yggr::s32() - yggr::f32())>(a3) << std::endl;

	yggr_test_assert(yggr::any_cast<yggr::f32>(a3) == 10 - 20.5f);

	boost::any a4(ANY_SET(std::string, "100"));
	boost::any a5 = a3 - a4;

	std::cout << a5.type().name() << std::endl;

	std::cout << "----------test_minus_4 E---------------" << std::endl;
}

//---------minus set-------
void test_minus_set_1(void)
{
	std::cout << "----------test_minus_set_1 S---------------" << std::endl;
	boost::any a1(ANY_SET(yggr::s32, 10));
	boost::any a2(ANY_SET(yggr::s32, 20));

	a1 -= a2;
	std::cout << yggr::any_cast<yggr::s32>(a1) << std::endl;

	yggr_test_assert(yggr::any_cast<yggr::s32>(a1) == 10 - 20);
	std::cout << "----------test_minus_set_1 E---------------" << std::endl;
}

void test_minus_set_2(void)
{
	std::cout << "----------test_minus_set_2 S---------------" << std::endl;
	boost::any a1(ANY_SET(std::string, "Hello "));
	boost::any a2(ANY_SET(std::string, "World"));

	a1 -= a2;
	std::cout << a1.type().name() << std::endl;
	yggr_test_assert(a1.empty());
	std::cout << "----------test_minus_set_2 E---------------" << std::endl;
}

void test_minus_set_3(void)
{
	std::cout << "----------test_minus_set_3 S---------------" << std::endl;
	boost::any a1(yggr::charset::utf8_string(yggr::utf8_string_view("你好 ")));
	boost::any a2(yggr::charset::utf8_string(yggr::utf8_string_view("世界 utf8")));

	a1 -= a2;
	std::cout << a1.type().name() << std::endl;
	yggr_test_assert(a1.empty());
	std::cout << "----------test_minus_set_3 E---------------" << std::endl;
}

void test_minus_set_4(void)
{
	std::cout << "----------test_plus_set_4 S---------------" << std::endl;
	boost::any a1(ANY_SET(yggr::s32, 10));
	boost::any a2(ANY_SET(yggr::f32, 20.5));

	a1 -= a2;
	std::cout << yggr::any_cast<yggr::s32>(a1) << std::endl;

	yggr_test_assert(yggr::any_cast<yggr::s32>(a1) == static_cast<yggr::s32>(10 - 20.5f));

	boost::any a4(ANY_SET(std::string, "100"));
	a4 -= a1;

	std::cout << a4.type().name() << std::endl;

	std::cout << "----------test_plus_set_4 E---------------" << std::endl;
}

// ----------mul---------
void test_mul_1(void)
{
	std::cout << "----------test_mul_1 S---------------" << std::endl;
	boost::any a1(ANY_SET(yggr::s32, 10));
	boost::any a2(ANY_SET(yggr::s32, 20));

	boost::any a3 = a1 * a2;
	std::cout << yggr::any_cast<yggr::s32>(a3) << std::endl;

	yggr_test_assert(yggr::any_cast<yggr::s32>(a3) == 10 * 20);
	std::cout << "----------test_mul_1 E---------------" << std::endl;
}

void test_mul_2(void)
{
	std::cout << "----------test_mul_2 S---------------" << std::endl;
	boost::any a1(ANY_SET(yggr::s32, 10));
	boost::any a2(ANY_SET(yggr::f32, 20.5));

	boost::any a3 = a1 * a2;
	std::cout << yggr::any_cast<BOOST_TYPEOF(yggr::s32() * yggr::f32())>(a3) << std::endl;

	yggr_test_assert(yggr::any_cast<BOOST_TYPEOF(yggr::s32() * yggr::f32())>(a3) == 10 * 20.5f);

	boost::any a4(ANY_SET(std::string, "100"));
	boost::any a5 = a3 * a4;

	std::cout << a5.type().name() << std::endl;
	yggr_test_assert(a5.empty());

	std::cout << "----------test_mul_2 E---------------" << std::endl;
}

//-----mul_set--------

void test_mul_set_1(void)
{
	std::cout << "----------test_mul_set_1 S---------------" << std::endl;
	boost::any a1(ANY_SET(yggr::s32, 10));
	boost::any a2(ANY_SET(yggr::s32, 20));

	a1 *= a2;
	std::cout << yggr::any_cast<yggr::s32>(a1) << std::endl;

	yggr_test_assert(yggr::any_cast<yggr::s32>(a1) == 10 * 20);
	std::cout << "----------test_mul_set_1 E---------------" << std::endl;
}

void test_mul_set_2(void)
{
	std::cout << "----------test_mul_set_2 S---------------" << std::endl;
	boost::any a1(ANY_SET(yggr::s32, 10));
	boost::any a2(ANY_SET(yggr::f32, 20.5));

	a1 *= a2;
	std::cout << yggr::any_cast<yggr::s32>(a1) << std::endl;

	yggr_test_assert(yggr::any_cast<yggr::s32>(a1) == static_cast<yggr::s32>(10 * 20.5f));

	boost::any a4(ANY_SET(std::string, "100"));
	a4 *= a1;

	std::cout << a4.type().name() << std::endl;

	std::cout << "----------test_mul_set_2 E---------------" << std::endl;
}

// ----------div---------
void test_div_1(void)
{
	std::cout << "----------test_div_1 S---------------" << std::endl;
	boost::any a1(ANY_SET(yggr::s32, 10));
	boost::any a2(ANY_SET(yggr::s32, 20));

	boost::any a3 = a1 / a2;
	std::cout << yggr::any_cast<yggr::s32>(a3) << std::endl;

	yggr_test_assert(yggr::any_cast<yggr::s32>(a3) == 10 / 20);
	std::cout << "----------test_div_1 E---------------" << std::endl;
}

void test_div_2(void)
{
    typedef BOOST_TYPEOF(yggr::s32() / yggr::f32()) cmp_val_type;
    typedef yggr::math::flaw_comparer<cmp_val_type> cmper_type;

	std::cout << "----------test_div_2 S---------------" << std::endl;
	boost::any a1(ANY_SET(yggr::s32, 10));
	boost::any a2(ANY_SET(yggr::f32, 20.5));

	boost::any a3 = a1 / a2;
	std::cout << yggr::any_cast<cmp_val_type>(a3) << std::endl;

	yggr_test_assert(yggr::any_cast<cmp_val_type>(a3) == cmper_type(10 / 20.5f));

	boost::any a4(ANY_SET(std::string, "100"));
	boost::any a5 = a3 / a4;

	std::cout << a5.type().name() << std::endl;
	yggr_test_assert(a5.empty());

	std::cout << "----------test_div_2 E---------------" << std::endl;
}

//-----div_set--------

void test_div_set_1(void)
{
	std::cout << "----------test_div_set_1 S---------------" << std::endl;
	boost::any a1(ANY_SET(yggr::s32, 10));
	boost::any a2(ANY_SET(yggr::s32, 20));

	a1 /= a2;
	std::cout << yggr::any_cast<yggr::s32>(a1) << std::endl;

	yggr_test_assert(yggr::any_cast<yggr::s32>(a1) == 10 / 20);
	std::cout << "----------test_div_set_1 E---------------" << std::endl;
}

void test_div_set_2(void)
{
	std::cout << "----------test_div_set_2 S---------------" << std::endl;
	boost::any a1(ANY_SET(yggr::s32, 10));
	boost::any a2(ANY_SET(yggr::f32, 20.5));

	a1 /= a2;
	std::cout << yggr::any_cast<yggr::s32>(a1) << std::endl;

	yggr_test_assert(yggr::any_cast<yggr::s32>(a1) == static_cast<yggr::s32>(10 / 20.5f));

	boost::any a4(ANY_SET(std::string, "100"));
	a4 /= a1;

	std::cout << a4.type().name() << std::endl;

	std::cout << "----------test_div_set_2 E---------------" << std::endl;
}


// ----------mod---------
void test_mod_1(void)
{
	std::cout << "----------test_div_1 S---------------" << std::endl;
	boost::any a1(ANY_SET(yggr::s32, 10));
	boost::any a2(ANY_SET(yggr::s32, 20));

	boost::any a3 = a1 % a2;
	std::cout << yggr::any_cast<yggr::s32>(a3) << std::endl;

	yggr_test_assert(yggr::any_cast<yggr::s32>(a3) == 10 % 20);
	std::cout << "----------test_div_1 E---------------" << std::endl;
}

void test_mod_2(void)
{
	std::cout << "----------test_div_2 S---------------" << std::endl;
	boost::any a1(ANY_SET(yggr::s32, 10));
	boost::any a2(ANY_SET(yggr::f32, 20.5));

	boost::any a3 = a1 % a2;
	std::cout << yggr::any_cast<BOOST_TYPEOF(yggr::s32() / yggr::f32())>(a3) << std::endl;

	yggr_test_assert(yggr::any_cast<BOOST_TYPEOF(yggr::s32() / yggr::f32())>(a3) == std::fmod(10, 20.5f));

	boost::any a4(ANY_SET(std::string, "100"));
	boost::any a5 = a3 % a4;

	std::cout << a5.type().name() << std::endl;
	yggr_test_assert(a5.empty());

	std::cout << "----------test_div_2 E---------------" << std::endl;
}

// ---------mod_set---------

void test_mod_set_1(void)
{
	std::cout << "----------test_mod_set_1 S---------------" << std::endl;
	boost::any a1(ANY_SET(yggr::s32, 10));
	boost::any a2(ANY_SET(yggr::s32, 20));

	a1 %= a2;
	std::cout << yggr::any_cast<yggr::s32>(a1) << std::endl;

	yggr_test_assert(yggr::any_cast<yggr::s32>(a1) == 10 % 20);
	std::cout << "----------test_mod_set_1 E---------------" << std::endl;
}

void test_mod_set_2(void)
{
	std::cout << "----------test_mod_set_2 S---------------" << std::endl;
	boost::any a1(ANY_SET(yggr::s32, 10));
	boost::any a2(ANY_SET(yggr::f32, 20.5));

	a1 %= a2;
	std::cout << yggr::any_cast<yggr::s32>(a1) << std::endl;

	yggr_test_assert(yggr::any_cast<yggr::s32>(a1) == static_cast<yggr::s32>(std::fmod(10, 20.5f)));

	boost::any a4(ANY_SET(std::string, "100"));
	a4 %= a1;

	std::cout << a4.type().name() << std::endl;

	std::cout << "----------test_mod_set_2 E---------------" << std::endl;
}

// test_logic_and

void test_logic_and(void)
{
	std::cout << "----------test_logic_and S---------------" << std::endl;
	boost::any a1(ANY_SET(bool, true));
	boost::any a2(ANY_SET(bool, false));
	boost::any a3(ANY_SET(bool, true));

	yggr_test_assert(!yggr::any_cast<bool>(a1 && a2));
	yggr_test_assert(yggr::any_cast<bool>(a1 && a3));
	yggr_test_assert(yggr::any_cast<bool>(a1 && a1));

	boost::any a4(ANY_SET(yggr::s32, 10));
	boost::any a5(ANY_SET(yggr::f32, 0.0f));

	yggr_test_assert(!yggr::any_cast<bool>(a4 && a5));
	yggr_test_assert(!yggr::any_cast<bool>(a1 && a5));

	yggr_test_assert(yggr::any_cast<bool>(a4 && a4));
	yggr_test_assert(yggr::any_cast<bool>(a4 && a1));

	boost::any a6(ANY_SET(std::string, "abc"));

	yggr_test_assert((a4 && a6).empty());

	std::cout << "all success" << std::endl;

	std::cout << "----------test_logic_and E---------------" << std::endl;
}

// test_logic_or

void test_logic_or(void)
{
	std::cout << "----------test_logic_or S---------------" << std::endl;
	boost::any a1(ANY_SET(bool, true));
	boost::any a2(ANY_SET(bool, false));
	boost::any a3(ANY_SET(bool, true));

	yggr_test_assert(yggr::any_cast<bool>(a1 || a2));
	yggr_test_assert(yggr::any_cast<bool>(a1 || a3));
	yggr_test_assert(!yggr::any_cast<bool>(a2 || a2));

	boost::any a4(ANY_SET(yggr::s32, 10));
	boost::any a5(ANY_SET(yggr::f32, 0.0f));

	yggr_test_assert(yggr::any_cast<bool>(a4 || a5));
	yggr_test_assert(yggr::any_cast<bool>(a1 || a5));

	yggr_test_assert(yggr::any_cast<bool>(a4 || a4));
	yggr_test_assert(!yggr::any_cast<bool>(a5 || a5));

	boost::any a6(ANY_SET(std::string, "abc"));

	yggr_test_assert((a4 || a6).empty());

	std::cout << "all success" << std::endl;

	std::cout << "----------test_logic_or E---------------" << std::endl;
}

// test_logic_not
void test_logic_not(void)
{
	//std::cout << "----------test_logic_not S---------------" << std::endl;
	boost::any a1(ANY_SET(bool, true));
	boost::any a2(ANY_SET(bool, false));
	boost::any a4(ANY_SET(yggr::s32, 10));
	boost::any a5(ANY_SET(yggr::f32, 0.0f));

	yggr_test_assert(!yggr::any_cast<bool>(!a1));
	yggr_test_assert((yggr::any_cast<bool>(!a2)));
	yggr_test_assert((!yggr::any_cast<bool>(!a4)));
	yggr_test_assert((yggr::any_cast<bool>(!a5)));

	boost::any a6(ANY_SET(std::string, "abc"));

	yggr_test_assert((!a6).empty());

	std::cout << "all success" << std::endl;

	std::cout << "----------test_logic_not E---------------" << std::endl;
}

//test_bitwise_and
void test_bitwise_and(void)
{
	std::cout << "----------test_bitwise_and S---------------" << std::endl;
	boost::any a1(ANY_SET(yggr::s32, 0x10000001));
	boost::any a2(ANY_SET(yggr::s32, 0x00000001));

	boost::any a3 = a1 & a2;
	std::cout << yggr::any_cast<yggr::s32>(a3) << std::endl;

	yggr_test_assert(yggr::any_cast<yggr::s32>(a3) == (0x10000001 & 0x00000001));

	boost::any a4(ANY_SET(std::string, "Hello"));

	boost::any a5 = a1 & a4;

	yggr_test_assert(a5.empty());
	std::cout << a5.type().name() << std::endl;

	std::cout << "----------test_bitwise_and E---------------" << std::endl;
}

// test_bitwise_and_set
void test_bitwise_and_set(void)
{
	std::cout << "----------test_bitwise_and_set S---------------" << std::endl;
	boost::any a1(ANY_SET(yggr::s32, 0x10000001));
	boost::any a2(ANY_SET(yggr::s32, 0x00000001));

	a1 &= a2;
	std::cout << yggr::any_cast<yggr::s32>(a1) << std::endl;

	yggr_test_assert(yggr::any_cast<yggr::s32>(a1) == (0x10000001 & 0x00000001));

	boost::any a4(ANY_SET(std::string, "Hello"));

	a1 &= a4;

	yggr_test_assert(a1.empty());
	std::cout << a1.type().name() << std::endl;

	std::cout << "----------test_bitwise_and_set E---------------" << std::endl;
}

//test_bitwise_and
void test_bitwise_or(void)
{
	std::cout << "----------test_bitwise_or S---------------" << std::endl;
	boost::any a1(ANY_SET(yggr::s32, 0x10000001));
	boost::any a2(ANY_SET(yggr::s32, 0x00000001));

	boost::any a3 = a1 | a2;
	std::cout << yggr::any_cast<yggr::s32>(a3) << std::endl;

	yggr_test_assert(yggr::any_cast<yggr::s32>(a3) == (0x10000001 | 0x00000001));

	boost::any a4(ANY_SET(std::string, "Hello"));

	boost::any a5 = a1 | a4;

	yggr_test_assert(a5.empty());
	std::cout << a5.type().name() << std::endl;

	std::cout << "----------test_bitwise_or E---------------" << std::endl;
}

// test_bitwise_or_set
void test_bitwise_or_set(void)
{
	std::cout << "----------test_bitwise_or_set S---------------" << std::endl;
	boost::any a1(ANY_SET(yggr::s32, 0x10000001));
	boost::any a2(ANY_SET(yggr::s32, 0x00000001));

	a1 |= a2;
	std::cout << yggr::any_cast<yggr::s32>(a1) << std::endl;

	yggr_test_assert(yggr::any_cast<yggr::s32>(a1) == (0x10000001 | 0x00000001));

	boost::any a4(ANY_SET(std::string, "Hello"));

	a1 |= a4;

	yggr_test_assert(a1.empty());
	std::cout << a1.type().name() << std::endl;

	std::cout << "----------test_bitwise_or_set E---------------" << std::endl;
}

// test_bitwise_xor
void test_bitwise_xor(void)
{
	std::cout << "----------test_bitwise_xor S---------------" << std::endl;
	boost::any a1(ANY_SET(yggr::s32, 0x10000001));
	boost::any a2(ANY_SET(yggr::s32, 0x00000001));

	boost::any a3 = a1 ^ a2;
	std::cout << yggr::any_cast<yggr::s32>(a3) << std::endl;

	yggr_test_assert(yggr::any_cast<yggr::s32>(a3) == (0x10000001 ^ 0x00000001));

	boost::any a4(ANY_SET(std::string, "Hello"));

	boost::any a5 = a1 ^ a4;

	yggr_test_assert(a5.empty());
	std::cout << a5.type().name() << std::endl;

	std::cout << "----------test_bitwise_xor E---------------" << std::endl;
}

// test_bitwise_xor_set
void test_bitwise_xor_set(void)
{
	std::cout << "----------test_bitwise_xor_set S---------------" << std::endl;
	boost::any a1(ANY_SET(yggr::s32, 0x10000001));
	boost::any a2(ANY_SET(yggr::s32, 0x00000001));

	a1 ^= a2;
	std::cout << yggr::any_cast<yggr::s32>(a1) << std::endl;

	yggr_test_assert(yggr::any_cast<yggr::s32>(a1) == (0x10000001 ^ 0x00000001));

	boost::any a4(ANY_SET(std::string, "Hello"));

	a1 ^= a4;

	yggr_test_assert(a1.empty());
	std::cout << a1.type().name() << std::endl;

	std::cout << "----------test_bitwise_xor_set E---------------" << std::endl;
}

// test_bitwise_left_shift
void test_bitwise_left_shift(void)
{
	std::cout << "----------test_bitwise_left_shift S---------------" << std::endl;
	boost::any a1(ANY_SET(yggr::s32, 10));
	boost::any a2(ANY_SET(yggr::s32, 1));

	boost::any a3 = a1 << a2;
	std::cout << yggr::any_cast<yggr::s32>(a3) << std::endl;

	yggr_test_assert(yggr::any_cast<yggr::s32>(a3) == (10 << 1));

	boost::any a4(ANY_SET(std::string, "Hello"));

	boost::any a5 = a1 << a4;

	yggr_test_assert(a5.empty());
	std::cout << a5.type().name() << std::endl;

	std::cout << "----------test_bitwise_left_shift E---------------" << std::endl;
}

// test_bitwise_left_shift_set
void test_bitwise_left_shift_set(void)
{
	std::cout << "----------test_bitwise_left_shift_set S---------------" << std::endl;
	boost::any a1(ANY_SET(yggr::s32, 10));
	boost::any a2(ANY_SET(yggr::s32, 1));

	a1 <<= a2;
	std::cout << yggr::any_cast<yggr::s32>(a1) << std::endl;

	yggr_test_assert(yggr::any_cast<yggr::s32>(a1) == (10 << 1));

	boost::any a4(ANY_SET(std::string, "Hello"));

	a1 <<= a4;

	yggr_test_assert(a1.empty());
	std::cout << a1.type().name() << std::endl;

	std::cout << "----------test_bitwise_left_shift_set E---------------" << std::endl;
}

// test_bitwise_right_shift
void test_bitwise_right_shift(void)
{
	std::cout << "----------test_bitwise_left_shift S---------------" << std::endl;
	boost::any a1(ANY_SET(yggr::s32, 10));
	boost::any a2(ANY_SET(yggr::s32, 1));

	boost::any a3 = a1 >> a2;
	std::cout << yggr::any_cast<yggr::s32>(a3) << std::endl;

	yggr_test_assert(yggr::any_cast<yggr::s32>(a3) == (10 >> 1));

	boost::any a4(ANY_SET(std::string, "Hello"));

	boost::any a5 = a1 >> a4;

	yggr_test_assert(a5.empty());
	std::cout << a5.type().name() << std::endl;

	std::cout << "----------test_bitwise_left_shift E---------------" << std::endl;
}

// test_bitwise_right_shift_set
void test_bitwise_right_shift_set(void)
{
	std::cout << "----------test_bitwise_right_shift_set S---------------" << std::endl;
	boost::any a1(ANY_SET(yggr::s32, 10));
	boost::any a2(ANY_SET(yggr::s32, 1));

	a1 >>= a2;
	std::cout << yggr::any_cast<yggr::s32>(a1) << std::endl;

	yggr_test_assert(yggr::any_cast<yggr::s32>(a1) == (10 >> 1));

	boost::any a4(ANY_SET(std::string, "Hello"));

	a1 >>= a4;

	yggr_test_assert(a1.empty());
	std::cout << a1.type().name() << std::endl;

	std::cout << "----------test_bitwise_right_shift_set E---------------" << std::endl;
}

// test_bitwise_not
void test_bitwise_not(void)
{
	std::cout << "----------test_bitwise_not S---------------" << std::endl;

	boost::any a1(ANY_SET(yggr::s32, 10));
	boost::any a2 = ~a1;

	std::cout << yggr::any_cast<yggr::s32>(a2) << std::endl;

	yggr_test_assert(yggr::any_cast<yggr::s32>(a2) == (~10));

	boost::any a4(ANY_SET(std::string, "Hello"));

	boost::any a5 = ~a4;

	yggr_test_assert(a5.empty());
	std::cout << a5.type().name() << std::endl;

	std::cout << "----------test_bitwise_not E---------------" << std::endl;
}

// test_equal_to
void test_equal_to_1(void)
{
	std::cout << "----------test_equal_to_1 S---------------" << std::endl;
	boost::any a1(ANY_SET(yggr::s32, 10));
	boost::any a2(ANY_SET(yggr::s32, 20));
	boost::any a3(ANY_SET(yggr::s32, 20));

	std::cout << yggr::any_cast<bool>(a1 == a2) << std::endl;
	yggr_test_assert(!yggr::any_cast<bool>(a1 == a2));

	std::cout << yggr::any_cast<bool>(a2 == a3) << std::endl;
	yggr_test_assert(yggr::any_cast<bool>(a2 == a3));

	std::cout << "----------test_equal_to_1 E---------------" << std::endl;
}

void test_equal_to_2(void)
{
	std::cout << "----------test_equal_to_2 S---------------" << std::endl;
	boost::any a1(ANY_SET(std::string, "Hello "));
	boost::any a2(ANY_SET(std::string, "World"));
	boost::any a3(ANY_SET(std::string, "World"));

	std::cout << yggr::any_cast<bool>(a1 == a2) << std::endl;
	yggr_test_assert(!yggr::any_cast<bool>(a1 == a2));

	std::cout << yggr::any_cast<bool>(a2 == a3) << std::endl;
	yggr_test_assert(yggr::any_cast<bool>(a2 == a3));

	std::cout << "----------test_equal_to_2 E---------------" << std::endl;
}

void test_equal_to_3(void)
{
	std::cout << "----------test_equal_to_3 S---------------" << std::endl;
	boost::any a1(yggr::charset::utf8_string(yggr::utf8_string_view("你好 ")));
	boost::any a2(yggr::charset::utf8_string(yggr::utf8_string_view("世界 utf8")));
	boost::any a3(yggr::charset::utf8_string(yggr::utf8_string_view("世界 utf8")));

	std::cout << yggr::any_cast<bool>(a1 == a2) << std::endl;
	yggr_test_assert(!yggr::any_cast<bool>(a1 == a2));

	std::cout << yggr::any_cast<bool>(a2 == a3) << std::endl;
	yggr_test_assert(yggr::any_cast<bool>(a2 == a3));

	std::cout << "----------test_equal_to_3 E---------------" << std::endl;
}

void test_equal_to_4(void)
{
	std::cout << "----------test_equal_to_4 S---------------" << std::endl;
	boost::any a1(ANY_SET(yggr::s32, 10));
	boost::any a2(ANY_SET(yggr::f32, 20.5));
	boost::any a3(ANY_SET(yggr::f32, 20.5));

	std::cout << yggr::any_cast<bool>(a1 == a2) << std::endl;
	yggr_test_assert(!yggr::any_cast<bool>(a1 == a2));

	std::cout << yggr::any_cast<bool>(a2 == a3) << std::endl;
	yggr_test_assert(yggr::any_cast<bool>(a2 == a3));

	std::cout << "----------test_equal_to_4 E---------------" << std::endl;
}

void test_equal_to_5(void)
{
	std::cout << "----------test_equal_to_5 S---------------" << std::endl;
	boost::any a1(ANY_SET(boost::container::string, "hello"));
	boost::any a2(ANY_SET(yggr::utf8_string, "hello"));

	boost::any a3(a1 == a2);
	yggr_test_assert((yggr::any_cast<bool>(a3)));
	std::cout << "----------test_equal_to_5 E---------------" << std::endl;
}

void test_equal_to_6(void)
{
	std::cout << "----------test_equal_to_6 S---------------" << std::endl;
	boost::any a1(ANY_SET(yggr::s32, 10));
	boost::any a2(ANY_SET(yggr::utf8_string, "hello"));

	boost::any a3(a1 == a2);

	std::cout << a3.type().name() << std::endl;
	yggr_test_assert(a3.empty());

	std::cout << "----------test_equal_to_6 E---------------" << std::endl;
}

// test_not_equal_to
void test_not_equal_to_1(void)
{
	std::cout << "----------test_not_equal_to_1 S---------------" << std::endl;
	boost::any a1(ANY_SET(yggr::s32, 10));
	boost::any a2(ANY_SET(yggr::s32, 20));
	boost::any a3(ANY_SET(yggr::s32, 20));

	std::cout << yggr::any_cast<bool>(a1 != a2) << std::endl;
	yggr_test_assert(yggr::any_cast<bool>(a1 != a2));

	std::cout << yggr::any_cast<bool>(a2 != a3) << std::endl;
	yggr_test_assert(!yggr::any_cast<bool>(a2 != a3));

	std::cout << "----------test_not_equal_to_1 E---------------" << std::endl;
}

void test_not_equal_to_2(void)
{
	std::cout << "----------test_not_equal_to_2 S---------------" << std::endl;
	boost::any a1(ANY_SET(std::string, "Hello "));
	boost::any a2(ANY_SET(std::string, "World"));
	boost::any a3(ANY_SET(std::string, "World"));

	std::cout << yggr::any_cast<bool>(a1 != a2) << std::endl;
	yggr_test_assert(yggr::any_cast<bool>(a1 != a2));

	std::cout << yggr::any_cast<bool>(a2 != a3) << std::endl;
	yggr_test_assert(!yggr::any_cast<bool>(a2 != a3));

	std::cout << "----------test_not_equal_to_2 E---------------" << std::endl;
}

void test_not_equal_to_3(void)
{
	std::cout << "----------test_not_equal_to_3 S---------------" << std::endl;
	boost::any a1(yggr::charset::utf8_string(yggr::utf8_string_view("你好 ")));
	boost::any a2(yggr::charset::utf8_string(yggr::utf8_string_view("世界 utf8")));
	boost::any a3(yggr::charset::utf8_string(yggr::utf8_string_view("世界 utf8")));

	std::cout << yggr::any_cast<bool>(a1 != a2) << std::endl;
	yggr_test_assert(yggr::any_cast<bool>(a1 != a2));

	std::cout << yggr::any_cast<bool>(a2 != a3) << std::endl;
	yggr_test_assert(!yggr::any_cast<bool>(a2 != a3));

	std::cout << "----------test_not_equal_to_3 E---------------" << std::endl;
}

void test_not_equal_to_4(void)
{
	std::cout << "----------test_not_equal_to_4 S---------------" << std::endl;
	boost::any a1(ANY_SET(yggr::s32, 10));
	boost::any a2(ANY_SET(yggr::f32, 20.5));
	boost::any a3(ANY_SET(yggr::f32, 20.5));

	std::cout << yggr::any_cast<bool>(a1 != a2) << std::endl;
	yggr_test_assert(yggr::any_cast<bool>(a1 != a2));

	std::cout << yggr::any_cast<bool>(a2 != a3) << std::endl;
	yggr_test_assert(!yggr::any_cast<bool>(a2 != a3));

	std::cout << "----------test_not_equal_to_4 E---------------" << std::endl;
}

void test_not_equal_to_5(void)
{
	std::cout << "----------test_not_equal_to_5 S---------------" << std::endl;
	boost::any a1(ANY_SET(boost::container::string, "hello"));
	boost::any a2(ANY_SET(yggr::utf8_string, "world"));

	boost::any a3(a1 != a2);
	yggr_test_assert((yggr::any_cast<bool>(a3)));
	std::cout << "----------test_not_equal_to_5 E---------------" << std::endl;
}

void test_not_equal_to_6(void)
{
	std::cout << "----------test_not_equal_to_6 S---------------" << std::endl;
	boost::any a1(ANY_SET(yggr::s32, 10));
	boost::any a2(ANY_SET(yggr::utf8_string, "hello"));

	boost::any a3(a1 != a2);

	std::cout << a3.type().name() << std::endl;
	yggr_test_assert(a3.empty());

	std::cout << "----------test_not_equal_to_6 E---------------" << std::endl;
}

// test_less
void test_less_1(void)
{
	std::cout << "----------test_less_1 S---------------" << std::endl;
	boost::any a1(ANY_SET(yggr::s32, 10));
	boost::any a2(ANY_SET(yggr::s32, 20));
	boost::any a3(ANY_SET(yggr::s32, 30));

	std::cout << yggr::any_cast<bool>(a1 < a2) << std::endl;
	yggr_test_assert(yggr::any_cast<bool>(a1 < a2));

	std::cout << yggr::any_cast<bool>(a3 < a2) << std::endl;
	yggr_test_assert(!yggr::any_cast<bool>(a3 < a2));

	std::cout << "----------test_less_1 E---------------" << std::endl;
}

void test_less_2(void)
{
	std::cout << "----------test_less_2 S---------------" << std::endl;
	boost::any a1(ANY_SET(std::string, "Hello "));
	boost::any a2(ANY_SET(std::string, "World"));
	boost::any a3(ANY_SET(std::string, "Zete"));

	std::cout << yggr::any_cast<bool>(a1 < a2) << std::endl;
	yggr_test_assert(yggr::any_cast<bool>(a1 < a2));

	std::cout << yggr::any_cast<bool>(a3 < a2) << std::endl;
	yggr_test_assert(!yggr::any_cast<bool>(a3 < a2));

	std::cout << "----------test_less_2 E---------------" << std::endl;
}

// test_less_equal
void test_less_equal_1(void)
{
	std::cout << "----------test_less_equal_1 S---------------" << std::endl;
	boost::any a1(ANY_SET(yggr::s32, 10));
	boost::any a2(ANY_SET(yggr::s32, 20));
	boost::any a3(ANY_SET(yggr::s32, 30));
	boost::any a4(ANY_SET(yggr::s32, 30));

	std::cout << yggr::any_cast<bool>(a1 <= a2) << std::endl;
	yggr_test_assert(yggr::any_cast<bool>(a1 <= a2));

	std::cout << yggr::any_cast<bool>(a3 <= a2) << std::endl;
	yggr_test_assert(!yggr::any_cast<bool>(a3 <= a2));

	std::cout << yggr::any_cast<bool>(a3 <= a4) << std::endl;
	yggr_test_assert(yggr::any_cast<bool>(a3 <= a4));

	std::cout << "----------test_less_equal_1 E---------------" << std::endl;
}

void test_less_equal_2(void)
{
	std::cout << "----------test_less_equal_2 S---------------" << std::endl;
	boost::any a1(ANY_SET(std::string, "Hello "));
	boost::any a2(ANY_SET(std::string, "World"));
	boost::any a3(ANY_SET(std::string, "Zete"));
	boost::any a4(ANY_SET(std::string, "Zete"));

	std::cout << yggr::any_cast<bool>(a1 <= a2) << std::endl;
	yggr_test_assert(yggr::any_cast<bool>(a1 <= a2));

	std::cout << yggr::any_cast<bool>(a3 <= a2) << std::endl;
	yggr_test_assert(!yggr::any_cast<bool>(a3 <= a2));

	std::cout << yggr::any_cast<bool>(a3 <= a4) << std::endl;
	yggr_test_assert(yggr::any_cast<bool>(a3 <= a4));

	std::cout << "----------test_less_equal_2 E---------------" << std::endl;
}

// test_greater
void test_greater_1(void)
{
	std::cout << "----------test_greater_1 S---------------" << std::endl;
	boost::any a1(ANY_SET(yggr::s32, 10));
	boost::any a2(ANY_SET(yggr::s32, 20));
	boost::any a3(ANY_SET(yggr::s32, 30));

	std::cout << yggr::any_cast<bool>(a2 > a1) << std::endl;
	yggr_test_assert(yggr::any_cast<bool>(a2 > a1));

	std::cout << yggr::any_cast<bool>(a2 > a3) << std::endl;
	yggr_test_assert(!yggr::any_cast<bool>(a2 > a3));

	std::cout << "----------test_greater_1 E---------------" << std::endl;
}

void test_greater_2(void)
{
	std::cout << "----------test_greater_2 S---------------" << std::endl;
	boost::any a1(ANY_SET(std::string, "Hello "));
	boost::any a2(ANY_SET(std::string, "World"));
	boost::any a3(ANY_SET(std::string, "Zete"));

	std::cout << yggr::any_cast<bool>(a2 > a1) << std::endl;
	yggr_test_assert(yggr::any_cast<bool>(a2 > a1));

	std::cout << yggr::any_cast<bool>(a2 > a3) << std::endl;
	yggr_test_assert(!yggr::any_cast<bool>(a2 > a3));

	std::cout << "----------test_greater_2 E---------------" << std::endl;
}

// test_greater_equal
void test_greater_equal_1(void)
{
	std::cout << "----------test_greater_equal_1 S---------------" << std::endl;
	boost::any a1(ANY_SET(yggr::s32, 10));
	boost::any a2(ANY_SET(yggr::s32, 20));
	boost::any a3(ANY_SET(yggr::s32, 30));
	boost::any a4(ANY_SET(yggr::s32, 30));

	std::cout << yggr::any_cast<bool>(a2 >= a1) << std::endl;
	yggr_test_assert(yggr::any_cast<bool>(a2 >= a1));

	std::cout << yggr::any_cast<bool>(a2 >= a3) << std::endl;
	yggr_test_assert(!yggr::any_cast<bool>(a2 >= a3));

	std::cout << yggr::any_cast<bool>(a3 >= a4) << std::endl;
	yggr_test_assert(yggr::any_cast<bool>(a3 >= a4));

	std::cout << "----------test_greater_equal_1 E---------------" << std::endl;
}

void test_greater_equal_2(void)
{
	std::cout << "----------test_greater_equal_2 S---------------" << std::endl;
	boost::any a1(ANY_SET(std::string, "Hello "));
	boost::any a2(ANY_SET(std::string, "World"));
	boost::any a3(ANY_SET(std::string, "Zete"));
	boost::any a4(ANY_SET(std::string, "Zete"));

	std::cout << yggr::any_cast<bool>(a2 >= a1) << std::endl;
	yggr_test_assert(yggr::any_cast<bool>(a2 >= a1));

	std::cout << yggr::any_cast<bool>(a2 >= a3) << std::endl;
	yggr_test_assert(!yggr::any_cast<bool>(a2 >= a3));

	std::cout << yggr::any_cast<bool>(a3 >= a4) << std::endl;
	yggr_test_assert(yggr::any_cast<bool>(a3 >= a4));

	std::cout << "----------test_greater_equal_2 E---------------" << std::endl;
}


int main(int argc, char* argv[])
{
	yggr::any_val::any_operator_mgr_boost_basic_t_reg::s_init(true);

	// --------test_negate----------
	test_negate_1();
	test_negate_2();

	//--------test_before_plus_plus----------
	test_before_plus_plus_1();
	test_before_plus_plus_2();

	//--------test_after_plus_plus----------
	test_after_plus_plus_1();
	test_after_plus_plus_2();

	//--------test_before_minus_minus_1-------
	test_before_minus_minus_1();
	test_before_minus_minus_2();

	//--------test_before_minus_minus_1---------
	test_after_minus_minus_1();
	test_after_minus_minus_2();

	//---------test_set_1--------
	test_set_1();
	test_set_2();
	test_set_3();
	test_set_4();
	test_set_5();

	// --------test_plus------------
	test_plus_1();
	test_plus_2();
	test_plus_3();
	test_plus_4();

	// --------test_plus_set------------
	test_plus_set_1();
	test_plus_set_2();
	test_plus_set_3();
	test_plus_set_4();

	// ------test_minus-------
	test_minus_1();
	test_minus_2();
	test_minus_3();
	test_minus_4();

	// -------test_minus_set---------
	test_minus_set_1();
	test_minus_set_2();
	test_minus_set_3();
	test_minus_set_4();

	//----------test_mul---------
	test_mul_1();
	test_mul_2();

	//----------test_mul_set---------
	test_mul_set_1();
	test_mul_set_2();

	//----------test_div---------
	test_div_1();
	test_div_2();

	//----------test_mul_set---------
	test_div_set_1();
	test_div_set_2();

	//----------test_mod---------
	test_mod_1();
	test_mod_2();

	//----------test_mod_set---------
	test_mod_set_1();
	test_mod_set_2();

	//---------test_logic_and----------
	test_logic_and();

	//---------test_logic_or----------
	test_logic_or();

	//-----------test_logic_not------
	test_logic_not();

	//-----------test_bitwise_and------
	test_bitwise_and();

	//-----------test_bitwise_and_set-------
	test_bitwise_and_set();

	//-----------test_bitwise_or------
	test_bitwise_or();

	//-----------test_bitwise_or_set------
	test_bitwise_or_set();

	//---------test_bitwise_xor-----
	test_bitwise_xor();

	//----------test_bitwise_xor_set--------
	test_bitwise_xor_set();

	//----------test_bitwise_left_shift--------
	test_bitwise_left_shift();

	//----------test_bitwise_left_shift_set--------
	test_bitwise_left_shift_set();

	//----------test_bitwise_right_shift--------
	test_bitwise_right_shift();

	//----------test_bitwise_right_shift_set--------
	test_bitwise_right_shift_set();

	//------------test_bitwise_not----------
	test_bitwise_not();

	//------------test_equal_to----------
	test_equal_to_1();
	test_equal_to_2();
	test_equal_to_3();
	test_equal_to_4();
	test_equal_to_5();
	test_equal_to_6();

	//------------test_not_equal_to----------
	test_not_equal_to_1();
	test_not_equal_to_2();
	test_not_equal_to_3();
	test_not_equal_to_4();
	test_not_equal_to_5();
	test_not_equal_to_6();

	//------------test_less----------
	test_less_1();
	test_less_2();

	//------------test_less_equal----------
	test_less_equal_1();
	test_less_equal_2();

	//------------test_greater----------
	test_greater_1();
	test_greater_2();

	//------------test_greater_equal----------
	test_greater_equal_1();
	test_greater_equal_2();

	wait_any_key(argc, argv);
	return 0;
}
