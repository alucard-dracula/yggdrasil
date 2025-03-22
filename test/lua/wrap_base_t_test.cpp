//lua_wrap_boost_size_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>

#include <yggr/type_traits/half_types.hpp>
#include <yggr/charset/string.hpp>
#include <yggr/lua_wrap/wrap_base_t.hpp>

#include <boost/type_traits/is_signed.hpp>
#include <boost/type_traits/make_signed.hpp>
#include <boost/type_traits/make_unsigned.hpp>

#include <cassert>
#include <typeinfo>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

template<typename T>
struct other_value
	: public
		boost::mpl::if_
		<
			boost::is_signed<T>,
			typename boost::make_unsigned<T>::type,
			typename boost::make_signed<T>::type
		>
{
};

template<typename T>
void test_construct(void)
{
	typedef T org_val_type;
	typedef yggr::lua_wrap::wrap_base_t<org_val_type> wrap_base_type;
	typedef typename yggr::half_t<org_val_type>::type half_org_val_type;
	typedef typename other_value<org_val_type>::type other_val_type;
	typedef yggr::lua_wrap::wrap_base_t<other_val_type> other_wrap_base_type;

	{
		wrap_base_type wrap_val;
		yggr_test_assert(wrap_val == 0);
	}

	{
		wrap_base_type wrap_val = 10;
		yggr_test_assert(wrap_val == 10);
	}

	{
		wrap_base_type wrap_val = org_val_type(10);
		yggr_test_assert(wrap_val == 10);
	}

	{
		wrap_base_type wrap_val(0x00000010, 0x00000010);
		yggr_test_assert(wrap_val == 0x0000001000000010L);
	}

	{
		wrap_base_type wrap_val = "10";
		yggr_test_assert(wrap_val == 10);
	}

	{
		wrap_base_type wrap_val = boost::container::string("10");
		yggr_test_assert(wrap_val == 10);
	}

	{
		wrap_base_type wrap_val = yggr::string("10");
		yggr_test_assert(wrap_val == 10);
	}

	{
		wrap_base_type wrap_val = yggr::string("10");
		yggr_test_assert(wrap_val == 10);
	}

	{
		wrap_base_type wrap_val = yggr::string("10");
		yggr_test_assert(wrap_val == 10);
	}

	{
		wrap_base_type wrap_val = other_wrap_base_type("10");
		yggr_test_assert(wrap_val == 10);
	}

	{
		wrap_base_type wrap_val = wrap_base_type("10");
		yggr_test_assert(wrap_val == 10);
	}

	std::cout << "------test_construct [" << typeid(org_val_type).name() << "] end------" << std::endl;
}

template<typename T>
void test_operator_val(void)
{
	typedef T org_val_type;
	typedef yggr::lua_wrap::wrap_base_t<org_val_type> wrap_base_type;
	typedef typename yggr::half_t<org_val_type>::type half_org_val_type;
	typedef typename other_value<org_val_type>::type other_val_type;
	typedef typename yggr::half_t<other_val_type>::type half_other_val_type;
	typedef yggr::lua_wrap::wrap_base_t<other_val_type> other_wrap_base_type;


	// operator bool
	{
		bool b1 = wrap_base_type(10);
		bool b2 = wrap_base_type();

		yggr_test_assert(b1);
		yggr_test_assert(!b2);
	}

	{
		org_val_type val = wrap_base_type(10);
		yggr_test_assert(val == 10);
	}

	{
		other_val_type val = wrap_base_type(10);
		yggr_test_assert(val == 10);
	}

	{
		half_org_val_type val = static_cast<org_val_type>(wrap_base_type(10));
		yggr_test_assert(val == 10);
	}

	{
		half_other_val_type val = static_cast<other_val_type>(wrap_base_type(10));
		yggr_test_assert(val == 10);
	}

	std::cout << "------test_operator_val [" << typeid(org_val_type).name() << "] end------" << std::endl;
}

template<typename T>
void test_operator_op(void)
{
	typedef T org_val_type;
	typedef yggr::lua_wrap::wrap_base_t<org_val_type> wrap_base_type;
	typedef typename yggr::half_t<org_val_type>::type half_org_val_type;
	typedef typename other_value<org_val_type>::type other_val_type;
	typedef yggr::lua_wrap::wrap_base_t<other_val_type> other_wrap_base_type;

	{
		wrap_base_type wrap_val = 10;
		wrap_base_type wrap_val2 = +wrap_val;
		yggr_test_assert(wrap_val2 == wrap_val);
	}

	{
		wrap_base_type wrap_val = 10;
		wrap_base_type wrap_val2 = -wrap_val;
		yggr_test_assert(wrap_val2 == -10);
	}

	{
		yggr_test_assert((wrap_base_type(10) + 10 == 20));
		yggr_test_assert((10 + wrap_base_type(10) == 20));
		yggr_test_assert((wrap_base_type(10) + other_wrap_base_type(10) == 20));
		yggr_test_assert(((wrap_base_type(10) + wrap_base_type(10)) == 20));

		{
			wrap_base_type wrap_val = 10;
			wrap_val += 10;
			yggr_test_assert(wrap_val == 20);
		}

		{
			half_org_val_type half_val = 10;
			half_val += wrap_base_type(10);
			yggr_test_assert(half_val == 20);
		}

		{
			wrap_base_type wrap_val = 10;
			wrap_val += other_wrap_base_type(10);
			yggr_test_assert(wrap_val == 20);
		}

		{
			wrap_base_type wrap_val = 10;
			wrap_val += wrap_base_type(10);
			yggr_test_assert(wrap_val == 20);
		}
	}

	{
		yggr_test_assert((wrap_base_type(10) - 10 == 0));
		yggr_test_assert((10 - wrap_base_type(10) == 0));
		yggr_test_assert((wrap_base_type(10) - other_wrap_base_type(10) == 0));
		yggr_test_assert((wrap_base_type(10) - wrap_base_type(10) == 0));

		{
			wrap_base_type wrap_val = 10;
			wrap_val -= 10;
			yggr_test_assert(wrap_val == 0);
		}

		{
			half_org_val_type half_val = 10;
			half_val -= wrap_base_type(10);
			yggr_test_assert(half_val == 0);
		}

		{
			wrap_base_type wrap_val = 10;
			wrap_val -= other_wrap_base_type(10);
			yggr_test_assert(wrap_val == 0);
		}

		{
			wrap_base_type wrap_val = 10;
			wrap_val -= wrap_base_type(10);
			yggr_test_assert(wrap_val == 0);
		}
	}

	{
		yggr_test_assert((wrap_base_type(10) * 10 == 100));
		yggr_test_assert((10 * wrap_base_type(10) == 100));
		yggr_test_assert((wrap_base_type(10) * other_wrap_base_type(10) == 100));
		yggr_test_assert((wrap_base_type(10) * wrap_base_type(10) == 100));

		{
			wrap_base_type wrap_val = 10;
			wrap_val *= 10;
			yggr_test_assert(wrap_val == 100);
		}

		{
			half_org_val_type half_val = 10;
			half_val *= wrap_base_type(10);
			yggr_test_assert(half_val == 100);
		}

		{
			wrap_base_type wrap_val = 10;
			wrap_val *= other_wrap_base_type(10);
			yggr_test_assert(wrap_val == 100);
		}

		{
			wrap_base_type wrap_val = 10;
			wrap_val *= wrap_base_type(10);
			yggr_test_assert(wrap_val == 100);
		}
	}

	{
		yggr_test_assert((wrap_base_type(10) / 10 == 1));
		yggr_test_assert((10 / wrap_base_type(10) == 1));
		yggr_test_assert((wrap_base_type(10) / other_wrap_base_type(10) == 1));
		yggr_test_assert((wrap_base_type(10) / wrap_base_type(10) == 1));

		{
			wrap_base_type wrap_val = 10;
			wrap_val /= 10;
			yggr_test_assert(wrap_val == 1);
		}

		{
			half_org_val_type half_val = 10;
			half_val /= wrap_base_type(10);
			yggr_test_assert(half_val == 1);
		}

		{
			wrap_base_type wrap_val = 10;
			wrap_val /= other_wrap_base_type(10);
			yggr_test_assert(wrap_val == 1);
		}

		{
			wrap_base_type wrap_val = 10;
			wrap_val /= wrap_base_type(10);
			yggr_test_assert(wrap_val == 1);
		}
	}

	{
		yggr_test_assert((wrap_base_type(10) % 3 == 1));
		yggr_test_assert((10 % wrap_base_type(3) == 1));
		yggr_test_assert((wrap_base_type(10) % other_wrap_base_type(3) == 1));
		yggr_test_assert((wrap_base_type(10) % wrap_base_type(3) == 1));

		{
			wrap_base_type wrap_val = 10;
			wrap_val %= 3;
			yggr_test_assert(wrap_val == 1);
		}

		{
			half_org_val_type half_val = 10;
			half_val %= wrap_base_type(3);
			yggr_test_assert(half_val == 1);
		}

		{
			wrap_base_type wrap_val = 10;
			wrap_val %= other_wrap_base_type(3);
			yggr_test_assert(wrap_val == 1);
		}

		{
			wrap_base_type wrap_val = 10;
			wrap_val %= wrap_base_type(3);
			yggr_test_assert(wrap_val == 1);
		}
	}

	{
		yggr_test_assert((wrap_base_type(10) && 10));
		yggr_test_assert(!(0 && wrap_base_type(10)));
		yggr_test_assert((wrap_base_type(10) && other_wrap_base_type(10)));
		yggr_test_assert(!(wrap_base_type(10) && wrap_base_type(0)));
	}

	{
		yggr_test_assert((wrap_base_type(10) || 10));
		yggr_test_assert(!(0 || wrap_base_type(0)));
		yggr_test_assert((wrap_base_type(10) || other_wrap_base_type(10)));
		yggr_test_assert(!(wrap_base_type(0) || wrap_base_type(0)));
	}

	{
		yggr_test_assert((wrap_base_type(10)));
		yggr_test_assert(!wrap_base_type(0));
	}

	{
		yggr_test_assert((wrap_base_type(10) == 10));
		yggr_test_assert((10 == wrap_base_type(10)));
		yggr_test_assert((wrap_base_type(10) == other_wrap_base_type(10)));
		yggr_test_assert((wrap_base_type(10) == wrap_base_type(10)));

		yggr_test_assert(!(wrap_base_type(10) == 1));
		yggr_test_assert(!(10 == wrap_base_type(1)));
		yggr_test_assert(!(wrap_base_type(10) == other_wrap_base_type(1)));
		yggr_test_assert(!(wrap_base_type(10) == wrap_base_type(1)));
	}

	{
		yggr_test_assert(!(wrap_base_type(10) != 10));
		yggr_test_assert(!(10 != wrap_base_type(10)));
		yggr_test_assert(!(wrap_base_type(10) != other_wrap_base_type(10)));
		yggr_test_assert(!(wrap_base_type(10) != wrap_base_type(10)));

		yggr_test_assert((wrap_base_type(10) != 1));
		yggr_test_assert((10 != wrap_base_type(1)));
		yggr_test_assert((wrap_base_type(10) != other_wrap_base_type(1)));
		yggr_test_assert((wrap_base_type(10) != wrap_base_type(1)));
	}

	{
		yggr_test_assert((wrap_base_type(10) < 20));
		yggr_test_assert((10 < wrap_base_type(20)));
		yggr_test_assert((wrap_base_type(10) < other_wrap_base_type(20)));
		yggr_test_assert((wrap_base_type(10) < wrap_base_type(20)));

		yggr_test_assert(!(wrap_base_type(20) < 20));
		yggr_test_assert(!(20 < wrap_base_type(20)));
		yggr_test_assert(!(wrap_base_type(20) < other_wrap_base_type(20)));
		yggr_test_assert(!(wrap_base_type(20) < wrap_base_type(20)));

		yggr_test_assert(!(wrap_base_type(20) < 20));
		yggr_test_assert(!(20 < wrap_base_type(10)));
		yggr_test_assert(!(wrap_base_type(20) < other_wrap_base_type(20)));
		yggr_test_assert(!(wrap_base_type(20) < wrap_base_type(10)));
	}

	{
		yggr_test_assert((wrap_base_type(10) <= 20));
		yggr_test_assert((10 <= wrap_base_type(20)));
		yggr_test_assert((wrap_base_type(10) <= other_wrap_base_type(20)));
		yggr_test_assert((wrap_base_type(10) <= wrap_base_type(20)));

		yggr_test_assert((wrap_base_type(20) <= 20));
		yggr_test_assert((20 <= wrap_base_type(20)));
		yggr_test_assert((wrap_base_type(20) <= other_wrap_base_type(20)));
		yggr_test_assert((wrap_base_type(20) <= wrap_base_type(20)));

		yggr_test_assert(!(wrap_base_type(30) <= 20));
		yggr_test_assert(!(30 <= wrap_base_type(20)));
		yggr_test_assert(!(wrap_base_type(30) <= other_wrap_base_type(20)));
		yggr_test_assert(!(wrap_base_type(30) <= wrap_base_type(20)));
	}

	{
		yggr_test_assert((wrap_base_type(20) > 10));
		yggr_test_assert((20 > wrap_base_type(10)));
		yggr_test_assert((wrap_base_type(20) > other_wrap_base_type(10)));
		yggr_test_assert((wrap_base_type(20) > wrap_base_type(10)));

		yggr_test_assert(!(wrap_base_type(20) > 20));
		yggr_test_assert(!(20 > wrap_base_type(20)));
		yggr_test_assert(!(wrap_base_type(20) > other_wrap_base_type(20)));
		yggr_test_assert(!(wrap_base_type(20) > wrap_base_type(20)));

		yggr_test_assert(!(wrap_base_type(10) > 20));
		yggr_test_assert(!(10 > wrap_base_type(20)));
		yggr_test_assert(!(wrap_base_type(10) > other_wrap_base_type(20)));
		yggr_test_assert(!(wrap_base_type(10) > wrap_base_type(20)));
	}

	{
		yggr_test_assert((wrap_base_type(20) >= 10));
		yggr_test_assert((20 >= wrap_base_type(10)));
		yggr_test_assert((wrap_base_type(20) >= other_wrap_base_type(10)));
		yggr_test_assert((wrap_base_type(20) >= wrap_base_type(10)));

		yggr_test_assert((wrap_base_type(20) >= 20));
		yggr_test_assert((20 >= wrap_base_type(20)));
		yggr_test_assert((wrap_base_type(20) >= other_wrap_base_type(20)));
		yggr_test_assert((wrap_base_type(20) >= wrap_base_type(20)));

		yggr_test_assert(!(wrap_base_type(10) >= 20));
		yggr_test_assert(!(10 > wrap_base_type(20)));
		yggr_test_assert(!(wrap_base_type(10) >= other_wrap_base_type(20)));
		yggr_test_assert(!(wrap_base_type(10) >= wrap_base_type(20)));
	}

	{
		yggr_test_assert((wrap_base_type(10) << 1 == 20));
		yggr_test_assert((10 << wrap_base_type(1) == 20));
		yggr_test_assert((wrap_base_type(10) << other_wrap_base_type(1) == 20));
		yggr_test_assert((wrap_base_type(10) << wrap_base_type(1) == 20));

		{
			wrap_base_type wrap_val = 10;
			wrap_val <<= 1;
			yggr_test_assert(wrap_val == 20);
		}

		{
			half_org_val_type half_val = 10;
			half_val <<= wrap_base_type(1);
			yggr_test_assert(half_val == 20);
		}

		{
			wrap_base_type wrap_val = 10;
			wrap_val <<= other_wrap_base_type(1);
			yggr_test_assert(wrap_val == 20);
		}

		{
			wrap_base_type wrap_val = 10;
			wrap_val <<= wrap_base_type(1);
			yggr_test_assert(wrap_val == 20);
		}
	}

	{
		yggr_test_assert((wrap_base_type(10) >> 1 == 5));
		yggr_test_assert((10 >> wrap_base_type(1) == 5));
		yggr_test_assert((wrap_base_type(10) >> other_wrap_base_type(1) == 5));
		yggr_test_assert((wrap_base_type(10) >> wrap_base_type(1) == 5));

		{
			wrap_base_type wrap_val = 10;
			wrap_val >>= 1;
			yggr_test_assert(wrap_val == 5);
		}

		{
			half_org_val_type half_val = 10;
			half_val >>= wrap_base_type(1);
			yggr_test_assert(half_val == 5);
		}

		{
			wrap_base_type wrap_val = 10;
			wrap_val >>= other_wrap_base_type(1);
			yggr_test_assert(wrap_val == 5);
		}

		{
			wrap_base_type wrap_val = 10;
			wrap_val >>= wrap_base_type(1);
			yggr_test_assert(wrap_val == 5);
		}
	}

	{
		yggr_test_assert(((wrap_base_type(3) & 2) == 2));
		yggr_test_assert(((3 & wrap_base_type(2)) == 2));
		yggr_test_assert(((wrap_base_type(3) & other_wrap_base_type(2)) == 2));
		yggr_test_assert(((wrap_base_type(3) & wrap_base_type(2)) == 2));

		{
			wrap_base_type wrap_val = 3;
			wrap_val &= 2;
			yggr_test_assert(wrap_val == 2);
		}

		{
			half_org_val_type half_val = 3;
			half_val &= wrap_base_type(2);
			yggr_test_assert(half_val == 2);
		}

		{
			wrap_base_type wrap_val = 3;
			wrap_val &= other_wrap_base_type(2);
			yggr_test_assert(wrap_val == 2);
		}

		{
			wrap_base_type wrap_val = 3;
			wrap_val &= wrap_base_type(2);
			yggr_test_assert(wrap_val == 2);
		}
	}

	{
		yggr_test_assert(((wrap_base_type(1) | 2) == 3));
		yggr_test_assert(((1 | wrap_base_type(2)) == 3));
		yggr_test_assert(((wrap_base_type(1) | other_wrap_base_type(2)) == 3));
		yggr_test_assert(((wrap_base_type(1) | wrap_base_type(2)) == 3));

		{
			wrap_base_type wrap_val = 1;
			wrap_val |= 2;
			yggr_test_assert(wrap_val == 3);
		}

		{
			half_org_val_type half_val = 1;
			half_val |= wrap_base_type(2);
			yggr_test_assert(half_val == 3);
		}

		{
			wrap_base_type wrap_val = 1;
			wrap_val |= other_wrap_base_type(2);
			yggr_test_assert(wrap_val == 3);
		}

		{
			wrap_base_type wrap_val = 1;
			wrap_val |= wrap_base_type(2);
			yggr_test_assert(wrap_val == 3);
		}
	}

	{
		yggr_test_assert(((~wrap_base_type(1)) == (0xffffffffffffffffL - 1)));
	}

	{
		yggr_test_assert(((wrap_base_type(1) ^ 2) == 3));
		yggr_test_assert(((1 ^ wrap_base_type(2)) == 3));
		yggr_test_assert(((wrap_base_type(1) ^ other_wrap_base_type(2)) == 3));
		yggr_test_assert(((wrap_base_type(1) ^ wrap_base_type(2)) == 3));

		{
			wrap_base_type wrap_val = 1;
			wrap_val ^= 2;
			yggr_test_assert(wrap_val == 3);
		}

		{
			half_org_val_type half_val = 1;
			half_val ^= wrap_base_type(2);
			yggr_test_assert(half_val == 3);
		}

		{
			wrap_base_type wrap_val = 1;
			wrap_val ^= other_wrap_base_type(2);
			yggr_test_assert(wrap_val == 3);
		}

		{
			wrap_base_type wrap_val = 1;
			wrap_val ^= wrap_base_type(2);
			yggr_test_assert(wrap_val == 3);
		}
	}

	std::cout << "------test_operator_op [" << typeid(org_val_type).name() << "] end------" << std::endl;
}

int main(int argc, char* argv[])
{
	test_construct<yggr::s64>();
	test_construct<yggr::u64>();

	test_operator_val<yggr::s64>();
	test_operator_val<yggr::u64>();

	test_operator_op<yggr::s64>();
	test_operator_op<yggr::u64>();

	wait_any_key(argc, argv);
	return 0;
}
