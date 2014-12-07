//base_any_type_operator_test.cpp


#error "!!!! now test must modify base_any_type_operator class member function to public!!!1"

#include <iostream>

#ifdef WIN32
#   ifdef _DEBUG
#       include <vld.h>
#   endif //_DEBUG
#endif //WIN32

#include <yggr/any_val/base_any_type_operator.hpp>
#include <yggr/any_val/any_val_operator.hpp>
#include <yggr/charset/utf8_string.hpp>
#include <boost/bind.hpp>

typedef yggr::any_val::any_val_operator<yggr::any_val::base_any_type_operator> any_val_op_type;

void build_test(void)
{
	int c = 10;
	boost::any a(10);

	boost::any b = any_val_op_type::negate(a); //-10
	boost::any d = any_val_op_type::after_plus_plus(b);

	int e = boost::any_cast<int>(b);
	int f = boost::any_cast<int>(d);

	std::cout << e << "," << f << std::endl;
}

void test_befor_plus_plus(void)
{
	boost::any a(ANY_SET(yggr::u32, 10));
	boost::any b = any_val_op_type::befor_plus_plus(a);

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
	boost::any b = any_val_op_type::after_plus_plus(a);

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
	boost::any b = any_val_op_type::befor_minus_minus(a);

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
	boost::any b = any_val_op_type::after_minus_minus(a);

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

		boost::any c = any_val_op_type::plus(a, b);

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

		boost::any cstr = any_val_op_type::plus(astr, bstr);

		std::cout << boost::any_cast<std::string>(cstr) << std::endl;
	}

	{
		yggr::charset::utf8_string str1("abc");
		yggr::charset::utf8_string str2("def");
		boost::any astr(str1);
		boost::any bstr(str2);

		boost::any cstr = any_val_op_type::plus(astr, bstr);


		std::cout << boost::any_cast<yggr::charset::utf8_string>(cstr) << std::endl;
	}

	{
		yggr::charset::utf8_string str1("abc");
		std::string str2("def");
		boost::any astr(str1);
		boost::any bstr(str2);

		boost::any cstr = any_val_op_type::plus(astr, bstr);

		std::cout << boost::any_cast<yggr::charset::utf8_string>(cstr) << std::endl;
	}

	{
		std::string str1("abc");
		yggr::charset::utf8_string str2("def");
		boost::any astr(str1);
		boost::any bstr(str2);

		boost::any cstr = any_val_op_type::plus(astr, bstr);

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

		any_val_op_type::plus_set(a, b);

		std::cout << boost::any_cast<int>(a) << std::endl;
	}

	{
		std::string str1("abc");
		std::string str2("def");
		boost::any astr(str1);
		boost::any bstr(str2);

		any_val_op_type::plus_set(astr, bstr);

		std::cout << boost::any_cast<std::string>(astr) << std::endl;
	}

	{
		yggr::charset::utf8_string str1("abc");
		yggr::charset::utf8_string str2("def");
		boost::any astr(str1);
		boost::any bstr(str2);

		any_val_op_type::plus_set(astr, bstr);


		std::cout << boost::any_cast<yggr::charset::utf8_string>(astr) << std::endl;
	}

	{
		yggr::charset::utf8_string str1("abc");
		std::string str2("def");
		boost::any astr(str1);
		boost::any bstr(str2);

		any_val_op_type::plus_set(astr, bstr);

		std::cout << boost::any_cast<yggr::charset::utf8_string>(astr) << std::endl;
	}

	{
		std::string str1("abc");
		yggr::charset::utf8_string str2("def");
		boost::any astr(str1);
		boost::any bstr(str2);

		any_val_op_type::plus_set(astr, bstr);

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

		boost::any c = any_val_op_type::minus(a, b);

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

		boost::any cstr = any_val_op_type::minus(astr, bstr);

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

		boost::any cstr = any_val_op_type::minus(astr, bstr);

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

		boost::any cstr = any_val_op_type::minus(astr, bstr);

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

		boost::any cstr = any_val_op_type::minus(astr, bstr);

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

		any_val_op_type::minus_set(a, b);

		std::cout << boost::any_cast<int>(a) << std::endl;
	}

	{
		std::string str1("abc");
		std::string str2("def");
		boost::any astr(str1);
		boost::any bstr(str2);

		any_val_op_type::minus_set(astr, bstr);

		std::cout << boost::any_cast<std::string>(astr) << std::endl;
	}

	{
		yggr::charset::utf8_string str1("abc");
		yggr::charset::utf8_string str2("def");
		boost::any astr(str1);
		boost::any bstr(str2);

		any_val_op_type::minus_set(astr, bstr);


		std::cout << boost::any_cast<yggr::charset::utf8_string>(astr) << std::endl;
	}

	{
		yggr::charset::utf8_string str1("abc");
		std::string str2("def");
		boost::any astr(str1);
		boost::any bstr(str2);

		any_val_op_type::minus_set(astr, bstr);

		std::cout << boost::any_cast<yggr::charset::utf8_string>(astr) << std::endl;
	}

	{
		std::string str1("abc");
		yggr::charset::utf8_string str2("def");
		boost::any astr(str1);
		boost::any bstr(str2);

		any_val_op_type::minus_set(astr, bstr);

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

		boost::any c = any_val_op_type::multiplies(a, b);

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

		boost::any cstr = any_val_op_type::multiplies(astr, bstr);

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

		boost::any cstr = any_val_op_type::multiplies(astr, bstr);

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

		boost::any cstr = any_val_op_type::multiplies(astr, bstr);

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

		boost::any cstr = any_val_op_type::multiplies(astr, bstr);

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

		any_val_op_type::multiplies_set(a, b);

		std::cout << boost::any_cast<int>(a) << std::endl;
	}

	{
		std::string str1("abc");
		std::string str2("def");
		boost::any astr(str1);
		boost::any bstr(str2);

		any_val_op_type::multiplies_set(astr, bstr);

		std::cout << boost::any_cast<std::string>(astr) << std::endl;
	}

	{
		yggr::charset::utf8_string str1("abc");
		yggr::charset::utf8_string str2("def");
		boost::any astr(str1);
		boost::any bstr(str2);

		any_val_op_type::multiplies_set(astr, bstr);


		std::cout << boost::any_cast<yggr::charset::utf8_string>(astr) << std::endl;
	}

	{
		yggr::charset::utf8_string str1("abc");
		std::string str2("def");
		boost::any astr(str1);
		boost::any bstr(str2);

		any_val_op_type::multiplies_set(astr, bstr);

		std::cout << boost::any_cast<yggr::charset::utf8_string>(astr) << std::endl;
	}

	{
		std::string str1("abc");
		yggr::charset::utf8_string str2("def");
		boost::any astr(str1);
		boost::any bstr(str2);

		any_val_op_type::multiplies_set(astr, bstr);

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

		boost::any c = any_val_op_type::divides(a, b);

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

		boost::any cstr = any_val_op_type::divides(astr, bstr);

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

		any_val_op_type::divides_set(a, b);

		std::cout << boost::any_cast<int>(a) << std::endl;
	}
}


void test_modulus(void)
{
	{
		boost::any a(10);
		boost::any b(20);

		boost::any c = any_val_op_type::modulus(a, b);

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

		boost::any cstr = any_val_op_type::modulus(astr, bstr);

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

		any_val_op_type::modulus_set(a, b);

		std::cout << boost::any_cast<int>(a) << std::endl;
	}
}

void test_logic_and(void)
{
	boost::any a('a');
	boost::any b(20.05f);
	boost::any c(0.0f);

	std::cout << any_val_op_type::logic_and(a, b) << std::endl;
	std::cout << any_val_op_type::logic_and(a, c) << std::endl;
}

void test_logic_or(void)
{
	boost::any a('a');
	boost::any b(20.05f);
	boost::any c(0.0f);

	std::cout << any_val_op_type::logic_or(a, b) << std::endl;
	std::cout << any_val_op_type::logic_or(a, c) << std::endl;
}

void test_logic_not(void)
{
	boost::any a('a');
	boost::any c(0.0f);

	std::cout << any_val_op_type::logic_not(a) << std::endl;
	std::cout << any_val_op_type::logic_not(c) << std::endl;
}

void test_bit_and(void)
{
	yggr::s32 na = 1;
	yggr::u32 nb = 3;
	boost::any a(na);
	boost::any b(nb);

	boost::any c = any_val_op_type::bit_and(a, b);

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

	any_val_op_type::bit_and_set(a, b);

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

	boost::any c = any_val_op_type::bit_or(a, b);

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

	any_val_op_type::bit_or_set(a, b);

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

	boost::any c = any_val_op_type::bit_or(a, b);

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

	any_val_op_type::bit_or_set(a, b);

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

	boost::any c = any_val_op_type::bit_left_trans(a, b);

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

	any_val_op_type::bit_left_trans_set(a, b);

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

	boost::any c = any_val_op_type::bit_right_trans(a, b);

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

	any_val_op_type::bit_right_trans_set(a, b);

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

	std::cout << any_val_op_type::equal_to(a, b) << std::endl;
	std::cout << any_val_op_type::equal_to(a,c) << std::endl;
}

void test_not_equal_to(void)
{
	boost::any a(1.0f);
	boost::any b(1);
	boost::any c(2.0);

	std::cout << any_val_op_type::not_equal_to(a, b) << std::endl;
	std::cout << any_val_op_type::not_equal_to(a, c) << std::endl;
}


void test_less(void)
{
	boost::any a(1.0f);
	boost::any b(2);
	boost::any c(0.0);
	boost::any d('\1');

	std::cout << any_val_op_type::less(a, b) << std::endl;
	std::cout << any_val_op_type::less(a, c) << std::endl;
	std::cout << any_val_op_type::less(a, d) << std::endl;
}

void test_less_equal(void)
{
	boost::any a(1.0f);
	boost::any b(2);
	boost::any c(0.0);
	boost::any d('\1');

	std::cout << any_val_op_type::less_equal(a, b) << std::endl;
	std::cout << any_val_op_type::less_equal(a, c) << std::endl;
	std::cout << any_val_op_type::less_equal(a, d) << std::endl;
}

void test_greater(void)
{
	boost::any a(1.0f);
	boost::any b(2);
	boost::any c(0.0);
	boost::any d('\1');

	std::cout << any_val_op_type::greater(a, b) << std::endl;
	std::cout << any_val_op_type::greater(a, c) << std::endl;
	std::cout << any_val_op_type::greater(a, d) << std::endl;
}

void test_greater_equal(void)
{
	boost::any a(1.0f);
	boost::any b(2);
	boost::any c(0.0);
	boost::any d('\1');

	std::cout << any_val_op_type::greater_equal(a, b) << std::endl;
	std::cout << any_val_op_type::greater_equal(a, c) << std::endl;
	std::cout << any_val_op_type::greater_equal(a, d) << std::endl;
}


void test_set(void)
{
	boost::any a(10);
	boost::any b(20.5f);

	any_val_op_type::set(a, b);

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

	std::cout << any_val_op_type::logic_op(a, boost::bind(not_any, _1)) << std::endl;
}

void test_logic_op_2(void)
{
	boost::any a(ANY_SET(yggr::u32, 10));
	boost::any b(ANY_SET(yggr::u8, 20));

	std::cout << any_val_op_type::logic_op(a, b, boost::bind(less_any, _1, _2)) << std::endl;

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

	boost::any b = any_val_op_type::math_op(a, boost::bind(negate_any, _1));

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

	boost::any c = any_val_op_type::math_op(a, b, boost::bind(add_any, _1, _2));

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

	any_val_op_type::set_op(a, b, boost::bind(set_any, _1, _2));

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
	std::cout << "---------------" << std::endl;

	test_plus();
	test_plus_set();
	std::cout << "---------------" << std::endl;

	test_minus();
	test_minus_set();
	std::cout << "---------------" << std::endl;

	test_multiplies();
	test_multiplies_set();
	std::cout << "---------------" << std::endl;

	test_divides();
	test_divides_set();
	std::cout << "---------------" << std::endl;

	test_modulus();
	test_modulus_set();
	std::cout << "---------------" << std::endl;

	test_logic_and();
	test_logic_or();
	test_logic_not();
	std::cout << "---------------" << std::endl;

	test_bit_and();
	test_bit_and_set();
	std::cout << "---------------" << std::endl;

	test_bit_or();
	test_bit_or_set();
	std::cout << "---------------" << std::endl;

	test_bit_xor();
	test_bit_xor_set();
	std::cout << "---------------" << std::endl;

	test_bit_left_trans();
	test_bit_left_trans_set();
	std::cout << "---------------" << std::endl;

	test_bit_right_trans();
	test_bit_right_trans_set();
	std::cout << "---------------" << std::endl;

	test_equal_to();
	test_not_equal_to();
	std::cout << "---------------" << std::endl;

	test_less();
	test_less_equal();
	std::cout << "---------------" << std::endl;

	test_greater();
	test_greater_equal();
	std::cout << "---------------" << std::endl;

	test_set();
	std::cout << "---------------" << std::endl;

	test_logic_op_2();

	test_math_op_2();

	test_math_op_1_const();

	test_set_op();
	std::cout << "---------------" << std::endl;

	test_befor_plus_plus();

	test_after_plus_plus();

	test_befor_minus_minus();
	test_after_minus_minus();
	std::cout << "---------------" << std::endl;

	char cc = 0;
	std::cin >> cc;
	return 0;
}
