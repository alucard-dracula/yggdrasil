//base_any_type_operator_test.cpp


#error "!!!! now test must modify base_any_type_operator class member function to public!!!"

#include <iostream>

#ifdef WIN32
#   ifdef _DEBUG
#     include <vld.h>
#   endif //_DEBUG
#endif //WIN32

#include <yggr/any_val/base_any_type_operator.hpp>
#include <yggr/charset/utf8_string.hpp>
#include <boost/bind.hpp>

void build_test(void)
{
	int c = 10;
	boost::any a(10);

	boost::any b = yggr::any_val::base_any_type_operator::negate(a); //-10
	boost::any d = yggr::any_val::base_any_type_operator::after_plus_plus(b);

	int e = boost::any_cast<int>(b);
	int f = boost::any_cast<int>(d);

	std::cout << e << "," << f << std::endl;
}

void test_befor_plus_plus(void)
{
	boost::any a(ANY_SET(yggr::u32, 10));
	boost::any b = yggr::any_val::base_any_type_operator::befor_plus_plus(a);

	if(!b.empty())
	{
		std::cout << boost::any_cast<yggr::u32>(b) << std::endl;
	}
	else
	{
		std::cout << "any empty" << std::endl;
	}

	if(!a.empty())
	{
		std::cout << boost::any_cast<yggr::u32>(a) << std::endl;
	}
	else
	{
		std::cout << "any empty" << std::endl;
	}
}

void test_after_plus_plus(void)
{
	boost::any a(ANY_SET(yggr::u32, 10));
	boost::any b = yggr::any_val::base_any_type_operator::after_plus_plus(a);

	if(!b.empty())
	{
		std::cout << boost::any_cast<yggr::u32>(b) << std::endl;
	}
	else
	{
		std::cout << "any empty" << std::endl;
	}

	if(!a.empty())
	{
		std::cout << boost::any_cast<yggr::u32>(a) << std::endl;
	}
	else
	{
		std::cout << "any empty" << std::endl;
	}
}

void test_befor_minus_minus(void)
{
	boost::any a(ANY_SET(yggr::u32, 10));
	boost::any b = yggr::any_val::base_any_type_operator::befor_minus_minus(a);

	if(!b.empty())
	{
		std::cout << boost::any_cast<yggr::u32>(b) << std::endl;
	}
	else
	{
		std::cout << "any empty" << std::endl;
	}

	if(!a.empty())
	{
		std::cout << boost::any_cast<yggr::u32>(a) << std::endl;
	}
	else
	{
		std::cout << "any empty" << std::endl;
	}
}

void test_after_minus_minus(void)
{
	boost::any a(ANY_SET(yggr::u32, 10));
	boost::any b = yggr::any_val::base_any_type_operator::after_minus_minus(a);

	if(!b.empty())
	{
		std::cout << boost::any_cast<yggr::u32>(b) << std::endl;
	}
	else
	{
		std::cout << "any empty" << std::endl;
	}

	if(!a.empty())
	{
		std::cout << boost::any_cast<yggr::u32>(a) << std::endl;
	}
	else
	{
		std::cout << "any empty" << std::endl;
	}
}

void test_plus(void)
{
	{
		boost::any a(10);
		boost::any b(20.5f);

		boost::any c = yggr::any_val::base_any_type_operator::plus(a, b);

		if(!c.empty())
		{
			std::cout << boost::any_cast<int>(c) << std::endl;
		}
		else
		{
			std::cout << "any empty" << std::endl;
		}
	}

	{
		std::string str1("abc");
		std::string str2("def");
		boost::any astr(str1);
		boost::any bstr(str2);

		boost::any cstr = yggr::any_val::base_any_type_operator::plus(astr, bstr);

		std::cout << boost::any_cast<std::string>(cstr) << std::endl;
	}

	{
		yggr::charset::utf8_string str1("abc");
		yggr::charset::utf8_string str2("def");
		boost::any astr(str1);
		boost::any bstr(str2);

		boost::any cstr = yggr::any_val::base_any_type_operator::plus(astr, bstr);


		std::cout << boost::any_cast<yggr::charset::utf8_string>(cstr) << std::endl;
	}

	{
		yggr::charset::utf8_string str1("abc");
		std::string str2("def");
		boost::any astr(str1);
		boost::any bstr(str2);

		boost::any cstr = yggr::any_val::base_any_type_operator::plus(astr, bstr);

		std::cout << boost::any_cast<yggr::charset::utf8_string>(cstr) << std::endl;
	}

	{
		std::string str1("abc");
		yggr::charset::utf8_string str2("def");
		boost::any astr(str1);
		boost::any bstr(str2);

		boost::any cstr = yggr::any_val::base_any_type_operator::plus(astr, bstr);

		if(!cstr.empty())
		{
			std::cout << boost::any_cast<std::string>(cstr) << std::endl;
		}
		else
		{
			std::cout << "any empty" << std::endl;
		}
	}
}

void test_plus_set(void)
{
	{
		boost::any a(10);
		boost::any b(20.5f);

		yggr::any_val::base_any_type_operator::plus_set(a, b);

		std::cout << boost::any_cast<int>(a) << std::endl;
	}

	{
		std::string str1("abc");
		std::string str2("def");
		boost::any astr(str1);
		boost::any bstr(str2);

		yggr::any_val::base_any_type_operator::plus_set(astr, bstr);

		std::cout << boost::any_cast<std::string>(astr) << std::endl;
	}

	{
		yggr::charset::utf8_string str1("abc");
		yggr::charset::utf8_string str2("def");
		boost::any astr(str1);
		boost::any bstr(str2);

		yggr::any_val::base_any_type_operator::plus_set(astr, bstr);


		std::cout << boost::any_cast<yggr::charset::utf8_string>(astr) << std::endl;
	}

	{
		yggr::charset::utf8_string str1("abc");
		std::string str2("def");
		boost::any astr(str1);
		boost::any bstr(str2);

		yggr::any_val::base_any_type_operator::plus_set(astr, bstr);

		std::cout << boost::any_cast<yggr::charset::utf8_string>(astr) << std::endl;
	}

	{
		std::string str1("abc");
		yggr::charset::utf8_string str2("def");
		boost::any astr(str1);
		boost::any bstr(str2);

		yggr::any_val::base_any_type_operator::plus_set(astr, bstr);

		if(!astr.empty())
		{
			std::cout << boost::any_cast<std::string>(astr) << std::endl;
		}
		else
		{
			std::cout << "any empty" << std::endl;
		}
	}
}

void test_minus(void)
{
	{
		boost::any a(10);
		boost::any b(20.5f);

		boost::any c = yggr::any_val::base_any_type_operator::minus(a, b);

		if(!c.empty())
		{
			std::cout << boost::any_cast<int>(c) << std::endl;
		}
		else
		{
			std::cout << "any empty" << std::endl;
		}
	}

	{
		std::string str1("abc");
		std::string str2("def");
		boost::any astr(str1);
		boost::any bstr(str2);

		boost::any cstr = yggr::any_val::base_any_type_operator::minus(astr, bstr);

		if(!cstr.empty())
		{
			std::cout << boost::any_cast<std::string>(cstr) << std::endl;
		}
		else
		{
			std::cout << "any empty" << std::endl;
		}
	}

	{
		yggr::charset::utf8_string str1("abc");
		yggr::charset::utf8_string str2("def");
		boost::any astr(str1);
		boost::any bstr(str2);

		boost::any cstr = yggr::any_val::base_any_type_operator::minus(astr, bstr);

		if(!cstr.empty())
		{
			std::cout << boost::any_cast<yggr::charset::utf8_string>(cstr) << std::endl;
		}
		else
		{
			std::cout << "any empty" << std::endl;
		}
	}

	{
		yggr::charset::utf8_string str1("abc");
		std::string str2("def");
		boost::any astr(str1);
		boost::any bstr(str2);

		boost::any cstr = yggr::any_val::base_any_type_operator::minus(astr, bstr);

		if(!cstr.empty())
		{
			std::cout << boost::any_cast<yggr::charset::utf8_string>(cstr) << std::endl;
		}
		else
		{
			std::cout << "any empty" << std::endl;
		}
	}

	{
		std::string str1("abc");
		yggr::charset::utf8_string str2("def");
		boost::any astr(str1);
		boost::any bstr(str2);

		boost::any cstr = yggr::any_val::base_any_type_operator::minus(astr, bstr);

		if(!cstr.empty())
		{
			std::cout << boost::any_cast<std::string>(cstr) << std::endl;
		}
		else
		{
			std::cout << "any empty" << std::endl;
		}
	}
}

void test_minus_set(void)
{
	{
		boost::any a(10);
		boost::any b(20.5f);

		yggr::any_val::base_any_type_operator::minus_set(a, b);

		std::cout << boost::any_cast<int>(a) << std::endl;
	}

	{
		std::string str1("abc");
		std::string str2("def");
		boost::any astr(str1);
		boost::any bstr(str2);

		yggr::any_val::base_any_type_operator::minus_set(astr, bstr);

		std::cout << boost::any_cast<std::string>(astr) << std::endl;
	}

	{
		yggr::charset::utf8_string str1("abc");
		yggr::charset::utf8_string str2("def");
		boost::any astr(str1);
		boost::any bstr(str2);

		yggr::any_val::base_any_type_operator::minus_set(astr, bstr);


		std::cout << boost::any_cast<yggr::charset::utf8_string>(astr) << std::endl;
	}

	{
		yggr::charset::utf8_string str1("abc");
		std::string str2("def");
		boost::any astr(str1);
		boost::any bstr(str2);

		yggr::any_val::base_any_type_operator::minus_set(astr, bstr);

		std::cout << boost::any_cast<yggr::charset::utf8_string>(astr) << std::endl;
	}

	{
		std::string str1("abc");
		yggr::charset::utf8_string str2("def");
		boost::any astr(str1);
		boost::any bstr(str2);

		yggr::any_val::base_any_type_operator::minus_set(astr, bstr);

		if(!astr.empty())
		{
			std::cout << boost::any_cast<std::string>(astr) << std::endl;
		}
		else
		{
			std::cout << "any empty" << std::endl;
		}
	}
}

void test_multiplies(void)
{
	{
		boost::any a(10);
		boost::any b(20.5f);

		boost::any c = yggr::any_val::base_any_type_operator::multiplies(a, b);

		if(!c.empty())
		{
			std::cout << boost::any_cast<int>(c) << std::endl;
		}
		else
		{
			std::cout << "any empty" << std::endl;
		}
	}

	{
		std::string str1("abc");
		std::string str2("def");
		boost::any astr(str1);
		boost::any bstr(str2);

		boost::any cstr = yggr::any_val::base_any_type_operator::multiplies(astr, bstr);

		if(!cstr.empty())
		{
			std::cout << boost::any_cast<std::string>(cstr) << std::endl;
		}
		else
		{
			std::cout << "any empty" << std::endl;
		}
	}

	{
		yggr::charset::utf8_string str1("abc");
		yggr::charset::utf8_string str2("def");
		boost::any astr(str1);
		boost::any bstr(str2);

		boost::any cstr = yggr::any_val::base_any_type_operator::multiplies(astr, bstr);

		if(!cstr.empty())
		{
			std::cout << boost::any_cast<yggr::charset::utf8_string>(cstr) << std::endl;
		}
		else
		{
			std::cout << "any empty" << std::endl;
		}
	}

	{
		yggr::charset::utf8_string str1("abc");
		std::string str2("def");
		boost::any astr(str1);
		boost::any bstr(str2);

		boost::any cstr = yggr::any_val::base_any_type_operator::multiplies(astr, bstr);

		if(!cstr.empty())
		{
			std::cout << boost::any_cast<yggr::charset::utf8_string>(cstr) << std::endl;
		}
		else
		{
			std::cout << "any empty" << std::endl;
		}
	}

	{
		std::string str1("abc");
		yggr::charset::utf8_string str2("def");
		boost::any astr(str1);
		boost::any bstr(str2);

		boost::any cstr = yggr::any_val::base_any_type_operator::multiplies(astr, bstr);

		if(!cstr.empty())
		{
			std::cout << boost::any_cast<std::string>(cstr) << std::endl;
		}
		else
		{
			std::cout << "any empty" << std::endl;
		}
	}
}

void test_multiplies_set(void)
{
	{
		boost::any a(10);
		boost::any b(20.5f);

		yggr::any_val::base_any_type_operator::multiplies_set(a, b);

		std::cout << boost::any_cast<int>(a) << std::endl;
	}

	{
		std::string str1("abc");
		std::string str2("def");
		boost::any astr(str1);
		boost::any bstr(str2);

		yggr::any_val::base_any_type_operator::multiplies_set(astr, bstr);

		std::cout << boost::any_cast<std::string>(astr) << std::endl;
	}

	{
		yggr::charset::utf8_string str1("abc");
		yggr::charset::utf8_string str2("def");
		boost::any astr(str1);
		boost::any bstr(str2);

		yggr::any_val::base_any_type_operator::multiplies_set(astr, bstr);


		std::cout << boost::any_cast<yggr::charset::utf8_string>(astr) << std::endl;
	}

	{
		yggr::charset::utf8_string str1("abc");
		std::string str2("def");
		boost::any astr(str1);
		boost::any bstr(str2);

		yggr::any_val::base_any_type_operator::multiplies_set(astr, bstr);

		std::cout << boost::any_cast<yggr::charset::utf8_string>(astr) << std::endl;
	}

	{
		std::string str1("abc");
		yggr::charset::utf8_string str2("def");
		boost::any astr(str1);
		boost::any bstr(str2);

		yggr::any_val::base_any_type_operator::multiplies_set(astr, bstr);

		if(!astr.empty())
		{
			std::cout << boost::any_cast<std::string>(astr) << std::endl;
		}
		else
		{
			std::cout << "any empty" << std::endl;
		}
	}
}


void test_divides(void)
{
	{
		boost::any a(10);
		boost::any b(20.5f);

		boost::any c = yggr::any_val::base_any_type_operator::divides(a, b);

		if(!c.empty())
		{
			std::cout << boost::any_cast<int>(c) << std::endl;
		}
		else
		{
			std::cout << "any empty" << std::endl;
		}
	}

	{
		std::string str1("abc");
		std::string str2("def");
		boost::any astr(str1);
		boost::any bstr(str2);

		boost::any cstr = yggr::any_val::base_any_type_operator::divides(astr, bstr);

		if(!cstr.empty())
		{
			std::cout << boost::any_cast<std::string>(cstr) << std::endl;
		}
		else
		{
			std::cout << "any empty" << std::endl;
		}
	}

}

void test_divides_set(void)
{
	{
		boost::any a(10);
		boost::any b(20.5);

		yggr::any_val::base_any_type_operator::divides_set(a, b);

		std::cout << boost::any_cast<int>(a) << std::endl;
	}
}


void test_modulus(void)
{
	{
		boost::any a(10);
		boost::any b(20);

		boost::any c = yggr::any_val::base_any_type_operator::modulus(a, b);

		if(!c.empty())
		{
			std::cout << boost::any_cast<int>(c) << std::endl;
		}
		else
		{
			std::cout << "any empty" << std::endl;
		}
	}

	{
		std::string str1("abc");
		std::string str2("def");
		boost::any astr(str1);
		boost::any bstr(str2);

		boost::any cstr = yggr::any_val::base_any_type_operator::modulus(astr, bstr);

		if(!cstr.empty())
		{
			std::cout << boost::any_cast<std::string>(cstr) << std::endl;
		}
		else
		{
			std::cout << "any empty" << std::endl;
		}
	}
}

void test_modulus_set(void)
{
	{
		boost::any a(10);
		boost::any b(20);

		yggr::any_val::base_any_type_operator::modulus_set(a, b);

		std::cout << boost::any_cast<int>(a) << std::endl;
	}
}

void test_logic_and(void)
{
	boost::any a('a');
	boost::any b(20.05f);
	boost::any c(0.0f);

	std::cout << yggr::any_val::base_any_type_operator::logic_and(a, b) << std::endl;
	std::cout << yggr::any_val::base_any_type_operator::logic_and(a, c) << std::endl;
}

void test_logic_or(void)
{
	boost::any a('a');
	boost::any b(20.05f);
	boost::any c(0.0f);

	std::cout << yggr::any_val::base_any_type_operator::logic_or(a, b) << std::endl;
	std::cout << yggr::any_val::base_any_type_operator::logic_or(a, c) << std::endl;
}

void test_logic_not(void)
{
	boost::any a('a');
	boost::any c(0.0f);

	std::cout << yggr::any_val::base_any_type_operator::logic_not(a) << std::endl;
	std::cout << yggr::any_val::base_any_type_operator::logic_not(c) << std::endl;
}

void test_bit_and(void)
{
	yggr::s32 na = 1;
	yggr::u32 nb = 3;
	boost::any a(na);
	boost::any b(nb);

	boost::any c = yggr::any_val::base_any_type_operator::bit_and(a, b);

	if(!c.empty())
	{
		std::cout << boost::any_cast<yggr::s32>(c) << std::endl;
	}
	else
	{
		std::cout << "any empty" << std::endl;
	}
}

void test_bit_and_set(void)
{
	yggr::s32 na = 1;
	yggr::u32 nb = 3;
	boost::any a(na);
	boost::any b(nb);

	yggr::any_val::base_any_type_operator::bit_and_set(a, b);

	if(!a.empty())
	{
		std::cout << boost::any_cast<yggr::s32>(a) << std::endl;
	}
	else
	{
		std::cout << "any empty" << std::endl;
	}
}

void test_bit_or(void)
{
	yggr::s32 na = 1;
	yggr::u32 nb = 2;
	boost::any a(na);
	boost::any b(nb);

	boost::any c = yggr::any_val::base_any_type_operator::bit_or(a, b);

	if(!c.empty())
	{
		std::cout << boost::any_cast<yggr::s32>(c) << std::endl;
	}
	else
	{
		std::cout << "any empty" << std::endl;
	}
}

void test_bit_or_set(void)
{
	yggr::s32 na = 1;
	yggr::u32 nb = 2;
	boost::any a(na);
	boost::any b(nb);

	yggr::any_val::base_any_type_operator::bit_or_set(a, b);

	if(!a.empty())
	{
		std::cout << boost::any_cast<yggr::s32>(a) << std::endl;
	}
	else
	{
		std::cout << "any empty" << std::endl;
	}
}


void test_bit_xor(void)
{
	yggr::s32 na = 1;
	yggr::u32 nb = 2;
	boost::any a(na);
	boost::any b(nb);

	boost::any c = yggr::any_val::base_any_type_operator::bit_or(a, b);

	if(!c.empty())
	{
		std::cout << boost::any_cast<yggr::s32>(c) << std::endl;
	}
	else
	{
		std::cout << "any empty" << std::endl;
	}
}

void test_bit_xor_set(void)
{
	yggr::s32 na = 1;
	yggr::u32 nb = 2;
	boost::any a(na);
	boost::any b(nb);

	yggr::any_val::base_any_type_operator::bit_or_set(a, b);

	if(!a.empty())
	{
		std::cout << boost::any_cast<yggr::s32>(a) << std::endl;
	}
	else
	{
		std::cout << "any empty" << std::endl;
	}
}

void test_bit_left_trans(void)
{
	yggr::s8 na = 1;
	yggr::u32 nb = 2;
	boost::any a(na);
	boost::any b(nb);

	boost::any c = yggr::any_val::base_any_type_operator::bit_left_trans(a, b);

	if(!c.empty())
	{
		std::cout << (int)boost::any_cast<yggr::s8>(c) << std::endl;
	}
	else
	{
		std::cout << "any empty" << std::endl;
	}
}

void test_bit_left_trans_set(void)
{
	yggr::s8 na = 1;
	yggr::u32 nb = 2;
	boost::any a(na);
	boost::any b(nb);

	yggr::any_val::base_any_type_operator::bit_left_trans_set(a, b);

	if(!a.empty())
	{
		std::cout << (int)boost::any_cast<yggr::s8>(a) << std::endl;
	}
	else
	{
		std::cout << "any empty" << std::endl;
	}
}

void test_bit_right_trans(void)
{
	yggr::s8 na = 2;
	yggr::u32 nb = 1;
	boost::any a(na);
	boost::any b(nb);

	boost::any c = yggr::any_val::base_any_type_operator::bit_right_trans(a, b);

	if(!c.empty())
	{
		std::cout << (int)boost::any_cast<yggr::s8>(c) << std::endl;
	}
	else
	{
		std::cout << "any empty" << std::endl;
	}
}

void test_bit_right_trans_set(void)
{
	yggr::s8 na = 2;
	yggr::u32 nb = 1;
	boost::any a(na);
	boost::any b(nb);

	yggr::any_val::base_any_type_operator::bit_right_trans_set(a, b);

	if(!a.empty())
	{
		std::cout << (int)boost::any_cast<yggr::s8>(a) << std::endl;
	}
	else
	{
		std::cout << "any empty" << std::endl;
	}
}

void test_equal_to(void)
{
	boost::any a(1.0f);
	boost::any b(1);
	boost::any c(2.0);

	std::cout << yggr::any_val::base_any_type_operator::equal_to(a, b) << std::endl;
	std::cout << yggr::any_val::base_any_type_operator::equal_to(a,c) << std::endl;
}

void test_not_equal_to(void)
{
	boost::any a(1.0f);
	boost::any b(1);
	boost::any c(2.0);

	std::cout << yggr::any_val::base_any_type_operator::not_equal_to(a, b) << std::endl;
	std::cout << yggr::any_val::base_any_type_operator::not_equal_to(a, c) << std::endl;
}


void test_less(void)
{
	boost::any a(1.0f);
	boost::any b(2);
	boost::any c(0.0);
	boost::any d('\1');

	std::cout << yggr::any_val::base_any_type_operator::less(a, b) << std::endl;
	std::cout << yggr::any_val::base_any_type_operator::less(a, c) << std::endl;
	std::cout << yggr::any_val::base_any_type_operator::less(a, d) << std::endl;
}

void test_less_equal(void)
{
	boost::any a(1.0f);
	boost::any b(2);
	boost::any c(0.0);
	boost::any d('\1');

	std::cout << yggr::any_val::base_any_type_operator::less_equal(a, b) << std::endl;
	std::cout << yggr::any_val::base_any_type_operator::less_equal(a, c) << std::endl;
	std::cout << yggr::any_val::base_any_type_operator::less_equal(a, d) << std::endl;
}

void test_greater(void)
{
	boost::any a(1.0f);
	boost::any b(2);
	boost::any c(0.0);
	boost::any d('\1');

	std::cout << yggr::any_val::base_any_type_operator::greater(a, b) << std::endl;
	std::cout << yggr::any_val::base_any_type_operator::greater(a, c) << std::endl;
	std::cout << yggr::any_val::base_any_type_operator::greater(a, d) << std::endl;
}

void test_greater_equal(void)
{
	boost::any a(1.0f);
	boost::any b(2);
	boost::any c(0.0);
	boost::any d('\1');

	std::cout << yggr::any_val::base_any_type_operator::greater_equal(a, b) << std::endl;
	std::cout << yggr::any_val::base_any_type_operator::greater_equal(a, c) << std::endl;
	std::cout << yggr::any_val::base_any_type_operator::greater_equal(a, d) << std::endl;
}


void test_set(void)
{
	boost::any a(10);
	boost::any b(20.5f);

	yggr::any_val::base_any_type_operator::set(a, b);

	if(!a.empty())
	{
		std::cout << boost::any_cast<int>(a) << std::endl;
	}
	else
	{
		std::cout << "any empty" << std::endl;
	}
}

bool less_any(const boost::any& l, const boost::any& r)
{
	assert(!(l.empty() && r.empty()));
	assert(l.type() == typeid(yggr::u32) && r.type() == typeid(yggr::u8));
	const yggr::u32* p1 = boost::any_cast<yggr::u32>(&l);
	const yggr::u8* p2 = boost::any_cast<yggr::u8>(&r);

	return (p1 && p2)? *p1 < *p2 : false;
}

bool not_any(const boost::any& l)
{
	assert(!(l.empty()));
	assert(l.type() == typeid(yggr::u32));
	const yggr::u32* p = boost::any_cast<yggr::u32>(&l);

	return p? !(*p) : false;
}

void test_logic_op_1(void)
{
	boost::any a(ANY_SET(yggr::u32, 10));

	std::cout << yggr::any_val::base_any_type_operator::logic_op(a, boost::bind(not_any, _1)) << std::endl;
}

void test_logic_op_2(void)
{
	boost::any a(ANY_SET(yggr::u32, 10));
	boost::any b(ANY_SET(yggr::u8, 20));

	std::cout << yggr::any_val::base_any_type_operator::logic_op(a, b, boost::bind(less_any, _1, _2)) << std::endl;

}

boost::any negate_any(const boost::any& l)
{
	assert(!(l.empty()));
	assert(l.type() == typeid(yggr::s32));

	const yggr::s32 *p = boost::any_cast<yggr::s32>(&l);

	return p? boost::any(-(*p)) : boost::any();
}

boost::any add_any(const boost::any& l, const boost::any& r)
{
	assert(!(l.empty() && r.empty()));
	assert(l.type() == typeid(yggr::u32) && r.type() == typeid(yggr::u8));
	const yggr::u32* p1 = boost::any_cast<yggr::u32>(&l);
	const yggr::u8* p2 = boost::any_cast<yggr::u8>(&r);

	return (p1 && p2)? *p1 + *p2 : boost::any();
}

void test_math_op_1_const(void)
{
	boost::any a(ANY_SET(yggr::s32, 10));

	boost::any b = yggr::any_val::base_any_type_operator::math_op(a, boost::bind(negate_any, _1));

	if(!b.empty())
	{
		std::cout << boost::any_cast<yggr::s32>(b) << std::endl;
	}
	else
	{
		std::cout << "any empty" << std::endl;
	}
}

void test_math_op_2(void)
{

	boost::any a(ANY_SET(yggr::u32, 10));
	boost::any b(ANY_SET(yggr::u8, 20));

	boost::any c = yggr::any_val::base_any_type_operator::math_op(a, b, boost::bind(add_any, _1, _2));

	if(!c.empty())
	{
		std::cout << boost::any_cast<yggr::u32>(c) << std::endl;
	}
	else
	{
		std::cout << "any empty" << std::endl;
	}
}

void set_any(boost::any& l, const boost::any& r)
{
	assert(!(l.empty() && r.empty()));
	assert(l.type() == typeid(yggr::u32) && r.type() == typeid(yggr::u8));

	yggr::u32 *pl = boost::any_cast<yggr::u32>(&l);
	const yggr::u8 *pr = boost::any_cast<yggr::u8>(&r);

	if(pl && pr)
	{
		*pl = *pr;
	}
}

void test_set_op(void)
{
	boost::any a(ANY_SET(yggr::u32, 10));
	boost::any b(ANY_SET(yggr::u8, 20));

	yggr::any_val::base_any_type_operator::set_op(a, b, boost::bind(set_any, _1, _2));

	if(!(a.empty()))
	{
		std::cout << boost::any_cast<yggr::u32>(a) << std::endl;
	}
	else
	{
		std::cout << "any empty" << std::endl;
	}
}

int main(int argc, char* argv[])
{
	build_test();

	test_plus();
	test_plus_set();

	test_minus();
	test_minus_set();

	test_multiplies();
	test_multiplies_set();

	test_divides();
	test_divides_set();

	test_modulus();
	test_modulus_set();

	test_logic_and();
	test_logic_or();
	test_logic_not();

	test_bit_and();
	test_bit_and_set();

	test_bit_or();
	test_bit_or_set();

	test_bit_xor();
	test_bit_xor_set();

	test_bit_left_trans();
	test_bit_left_trans_set();

	test_bit_right_trans();
	test_bit_right_trans_set();

	test_equal_to();
	test_not_equal_to();

	test_less();
	test_less_equal();

	test_greater();
	test_greater_equal();

	test_set();

	test_logic_op_2();

	test_math_op_2();

	test_math_op_1_const();

	test_set_op();

	test_befor_plus_plus();

	test_after_plus_plus();

	test_befor_minus_minus();
	test_after_minus_minus();

	char cc = 0;
	std::cin >> cc;
	return 0;
}
