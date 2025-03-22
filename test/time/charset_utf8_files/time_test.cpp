//time_test.cpp

#if defined(_MSC_VER)
#	error "this file not supports MSVC only, if you are using other compilers, please use the same name file in the ../"
#endif // _MSC_VER

#include <test/wait_any_key/wait_any_key.hpp>

#include <yggr/base/yggrdef.h>
#include <yggr/mplex/limits.hpp>
#include <yggr/thread/this_thread.hpp>

#include <yggr/time/time.hpp>

#include <yggr/math/flaw_comparer.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(time)
#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(base)

void test(void)
{
	yggr_test_assert((yggr::time::time::min_value() < yggr::time::time::zero_value()
			&& yggr::time::time::zero_value() < yggr::time::time::max_value()));

	std::time_t t = std::time(0);
	yggr::time::time ttm;

	std::time_t nttm = ttm.sec;
	std::cout << nttm << std::endl;

	char tmp[64] = {0};
	std::strftime( tmp, sizeof(tmp), "%Y-%m-%d %X %A",localtime(&t) );
	std::cout << tmp << std::endl;


	yggr::time::time yggr_tm;

	std::cout << "s = " << yggr_tm.ex_to_second<yggr::u64>() << std::endl;
	std::cout << "ms = " << yggr_tm.ex_to_millisecond<yggr::u64>() << std::endl;

	std::tm tm = yggr_tm.to_tm();

	std::cout << tm.tm_year << ", " << tm.tm_mon << ", " << tm.tm_mday << ", "
		<< tm.tm_hour << ", " << tm.tm_min << ", " << tm.tm_sec << std::endl;

	std::tm tm2 = yggr_tm.to_local_tm();

	std::cout << tm2.tm_year << ", " << tm2.tm_mon << ", " << tm2.tm_mday << ", "
		<< tm2.tm_hour << ", " << tm2.tm_min << ", " << tm2.tm_sec << std::endl;


	std::cout << yggr_tm.to_string("") << std::endl;

	std::cout << yggr_tm.to_string("%Y-%m-%d-%H-%M-%S") << std::endl;
	std::cout << yggr_tm.to_local_string("%Y-%m-%d-%H-%M-%S") << std::endl;

#ifndef YGGR_NO_CWCHAR
	std::cout << yggr_tm.to_string(L"%Y年%m月%d日 %H时:%M分:%S秒") << std::endl;
	std::cout << yggr_tm.to_local_string(L"%Y年%m月%d日 %H时:%M分:%S秒") << std::endl;
#endif // YGGR_NO_CWCHAR

#if 0

	yggr::time::time t1;

	yggr::thread::default_this_thread_type::sleep(yggr::time::time(1, 0));

	yggr::time::time t2;

	yggr_test_assert(1000 <= (t2 - t1).ex_to_millisecond<yggr::u64>());
	std::cout << (t2 - t1).ex_to_millisecond<yggr::u64>() << std::endl;

#endif // 0, 1

	std::cout << "test end" << std::endl;
}

void test2(void)
{
	typedef ::yggr::time::time time_type;
	// validate
	{
		yggr_test_assert((time_type(0, -1000).validate()));
		std::cout << (time_type(0, -1000).validate()) << std::endl;

		yggr_test_assert((time_type(0, 1000).validate()));
		std::cout << (time_type(0, 1000).validate()) << std::endl;

		yggr_test_assert((time_type(1, -1000).validate()));
		std::cout << (time_type(1, -1000).validate()) << std::endl;

		yggr_test_assert((time_type(-1, 1000).validate()));
		std::cout << (time_type(-1, 1000).validate()) << std::endl;

		{
			time_type tm(0);
			tm.sec = -1;
			tm.nsec = 1000;
			yggr_test_assert(!tm.validate());
			yggr_test_assert(!tm.formatable());
			yggr_test_assert(tm.decomposable());
			tm.decompose();
			yggr_test_assert(tm.validate());
			yggr_test_assert(tm == time_type(0, -time_type::E_S_AND_NS_RATIO + 1000));
		}

		{
			time_type tm(0);
			tm.sec = 1;
			tm.nsec = -1000;
			yggr_test_assert(!tm.validate());
			yggr_test_assert(!tm.formatable());
			yggr_test_assert(tm.decomposable());
			tm.decompose();
			yggr_test_assert(tm.validate());
			yggr_test_assert(tm == time_type(0, time_type::E_S_AND_NS_RATIO + -1000));
		}
	}

	// positive
	{
		time_type tm;
		time_type chk_tm = +tm;

		yggr_test_assert(tm.validate());
		yggr_test_assert(chk_tm.validate());
		yggr_test_assert(tm == chk_tm);
	}

	// negate
	{
		time_type tm;
		time_type chk_tm = -tm;

		yggr_test_assert(tm.validate());
		yggr_test_assert(chk_tm.validate());
		yggr_test_assert(tm + chk_tm == time_type(0, 0));
	}

	// plus
	{
		time_type tm1(10, 0);
		time_type tm2(-10, 1000);

		const time_type::base_type& base_tm1 = tm1;
		const time_type::base_type& base_tm2 = tm2;

		yggr_test_assert(time_type(0, 1000) == base_tm1 + tm2);
		yggr_test_assert(time_type(0, 1000) == tm1 + base_tm2);

		yggr_test_assert(time_type(0, 1000) == tm1 + tm2);
		yggr_test_assert(time_type(-1000, 1000) == time_type(-1000, 1000) + time_type(0, 0));
		yggr_test_assert(time_type(-1000, 1000) == time_type(-2000, 2000) + time_type(1000, -1000));
		yggr_test_assert(time_type(1, 1000) == time_type(0, 2000) + time_type(1, -1000));
		yggr_test_assert(time_type(1, 1000) == time_type(1, 0) + time_type(0, 1000));
		yggr_test_assert(time_type(2, 1000) == time_type(1, time_type::E_S_AND_NS_RATIO - 1000) + time_type(0, 2000));

		// has mistake
		//	yggr_test_assert(time_type(1, 1000) == time_type(1, 0) + time_type(0, 1000).ex_to_second<yggr::f32>());
		//	yggr_test_assert(time_type(1, 1000) == time_type(0, 1000).ex_to_second<yggr::f32>() + time_type(1, 0));

		yggr_test_assert(
			std::abs(
				(time_type(1, 1000) - (time_type(1, 0) + time_type(0, 1000).ex_to_second<yggr::f32>())).ex_to_second<yggr::f32>())
				< time_type::default_precision<yggr::f32>() );

		yggr_test_assert(
			std::abs(
				(time_type(1, 1000) - (time_type(0, 1000).ex_to_second<yggr::f32>() + time_type(1, 0))).ex_to_second<yggr::f32>())
				< time_type::default_precision<yggr::f32>() );

		yggr_test_assert(time_type(1, 1000) == time_type(1, 0).ex_to_second<yggr::s32>() + time_type(0, 1000));
		yggr_test_assert(time_type(1, 1000) == time_type(0, 1000) + time_type(1, 0).ex_to_second<yggr::s32>());

		{
			time_type tm11 = time_type(10, 0);
			yggr_test_assert((tm11 += tm2) == time_type(0, 1000));

			tm11 = time_type(10, 0);
			yggr_test_assert((tm11 += base_tm2) == time_type(0, 1000));

			tm11 = time_type(10, 0);
			yggr_test_assert((tm11 += tm2.ex_to_second()) == time_type(0, 1000));
		}
	}

	// minus
	{
		time_type tm1(10, 0);
		time_type tm2(-10, 1000);

		const time_type::base_type& base_tm1 = tm1;
		const time_type::base_type& base_tm2 = tm2;

		yggr_test_assert(time_type(20, -1000) == base_tm1 - tm2);
		yggr_test_assert(time_type(20, -1000) == tm1 - base_tm2);

		yggr_test_assert(time_type(20, -1000) == tm1 - tm2);
		yggr_test_assert(time_type(-1000, 1000) == time_type(-1000, 1000) - time_type(0, 0));
		yggr_test_assert(time_type(-1000, 1000) == time_type(-2000, 2000) - time_type(-1000, 1000));
		yggr_test_assert(time_type(1, 1000) == time_type(0, 2000) - time_type(-1, 1000));
		yggr_test_assert(time_type(1, 1000) == time_type(1, 0) - time_type(0, -1000));
		yggr_test_assert(time_type(2, 1000) == time_type(1, time_type::E_S_AND_NS_RATIO - 1000) - time_type(0, -2000));


		// has mistake
		//yggr_test_assert(time_type(1, 1000) == time_type(1, 0) - time_type(0, -1000).ex_to_second<yggr::f32>());
		//yggr_test_assert(time_type(1, 1000) == time_type(1, 0).ex_to_second<yggr::f32>() - time_type(0, -1000));

		yggr_test_assert(
			std::abs(
				(time_type(1, 1000) - (time_type(1, 0) - time_type(0, -1000).ex_to_second<yggr::f32>())).ex_to_second<yggr::f32>())
				< time_type::default_precision<yggr::f32>() );

		yggr_test_assert(
			std::abs(
				(time_type(1, 1000) - (time_type(1, 0).ex_to_second<yggr::f32>() - time_type(0, -1000))).ex_to_second<yggr::f32>())
				< time_type::default_precision<yggr::f32>() );

		yggr_test_assert(time_type(1, 1000) == time_type(1, 0).ex_to_second<yggr::s32>() - time_type(0, -1000));
		yggr_test_assert(time_type(1, 1000) == time_type(2, 1000) - time_type(1, 0).ex_to_second<yggr::s32>());

		{
			time_type tm11 = time_type(10, 0);
			yggr_test_assert((tm11 -= tm2) == time_type(20, -1000));

			tm11 = time_type(10, 0);
			yggr_test_assert((tm11 -= base_tm2) == time_type(20, -1000));

			tm11 = time_type(10, 0);
			yggr_test_assert((tm11 -= tm2.ex_to_second()) == time_type(20, -1000));
		}
	}

	// mul
	{
		time_type tm1(10, 2);
		time_type tm2(-10, 1000);

		const time_type::base_type& base_tm1 = tm1;
		const time_type::base_type& base_tm2 = tm2;

		// mul int
		yggr_test_assert(time_type(20, 4) == tm1 * 2);
		yggr_test_assert(time_type(-20, 2000) == 2 * tm2);

		yggr_test_assert(time_type(3, 0) == 2 * time_type(1, time_type::E_S_AND_NS_HALF_RATIO));

		yggr_test_assert(time_type(20, 4) == time_type(10, 2) * time_type(2, 0));
		yggr_test_assert(time_type(20, 4) == time_type(2, 0) * time_type(10, 2));
		yggr_test_assert(time_type(20, 0) == time_type(2, 0) * time_type(10, 0));

		{
			time_type tm3(-10, 0);
			const time_type::base_type& base_tm3 = tm3;

			time_type tm11 = time_type(10, 0);
			yggr_test_assert((tm11 *= tm3) == time_type(-100, 0));

			tm11 = time_type(10, 0);
			yggr_test_assert((tm11 *= base_tm3) == time_type(-100, 0));

			tm11 = time_type(10, 0);
			yggr_test_assert((tm11 *= tm3.ex_to_second()) == time_type(-100, 0));
		}

		// f32 loss of floating point precision

		yggr_test_assert(
			std::abs(
				time_type(tm1.ex_to_second<yggr::f32>() * tm2.ex_to_second<yggr::f32>())
				- base_tm1 * tm2)
			< time_type::default_precision<yggr::f32>());

		yggr_test_assert(
			std::abs(
				time_type(tm1.ex_to_second<yggr::f32>() * tm2.ex_to_second<yggr::f32>())
				- tm1 * base_tm2)
			< time_type::default_precision<yggr::f32>());

		yggr_test_assert(
			std::abs(
				time_type(tm1.ex_to_second<yggr::f64>() * tm2.ex_to_second<yggr::f64>())
				- base_tm1 * tm2)
			< time_type::default_precision<yggr::f64>());

		yggr_test_assert(
			std::abs(
				time_type(tm1.ex_to_second<yggr::f64>() * tm2.ex_to_second<yggr::f64>())
				- tm1 * base_tm2)
			< time_type::default_precision<yggr::f64>());

		yggr_test_assert(
			std::abs(
				time_type(tm1.ex_to_second<yggr::f64>() * tm2.ex_to_second<yggr::f64>())
				- tm1 * tm2)
			< time_type::default_precision<yggr::f64>());

		yggr_test_assert(
			std::abs(time_type(-1000, 1000) - 0.5 * time_type(-2000, 2000))
			< time_type::default_precision<yggr::f64>());

		yggr_test_assert(
			std::abs(time_type(-1000, 1000) - time_type(-2000, 2000) * 0.5)
			< time_type::default_precision<yggr::f64>());

		yggr_test_assert(
			std::abs(time_type(1000, 1000) -  0.5 * time_type(2000, 2000))
			< time_type::default_precision<yggr::f64>());

		yggr_test_assert(
			std::abs(time_type(1000, 1000) - time_type(2000, 2000) * 0.5)
			< time_type::default_precision<yggr::f64>());
	}

	// div
	{
		time_type tm1(10, 2);
		time_type tm2(-10, 1000);

		const time_type::base_type& base_tm1 = tm1;
		const time_type::base_type& base_tm2 = tm2;

		// div int
		yggr_test_assert(time_type(5, 1) == tm1 / 2);
		yggr_test_assert(time_type(5) == 10 / time_type(2));

		yggr_test_assert(time_type(100) == 10 / time_type(0.1));

		yggr_test_assert(time_type(1, time_type::E_S_AND_NS_HALF_RATIO) == time_type(3, 0) / 2);
		yggr_test_assert(time_type(1, time_type::E_S_AND_NS_HALF_RATIO) == 3 / time_type(2, 0));

		yggr_test_assert(time_type(20, 4) / time_type(10, 2) == time_type(2, 0));
		yggr_test_assert(time_type(20, 4) / time_type(2, 0) == time_type(10, 2));
		yggr_test_assert(time_type(20, 0) / time_type(2, 0) == time_type(10, 0));

		yggr_test_assert(0.5 == yggr::time::time_float_divides_cref(time_type(10, 0)) / time_type(20, 0));
		yggr_test_assert(0.5 == time_type(10, 0) / yggr::time::time_float_divides_cref(time_type(20, 0)));
		yggr_test_assert(0.5 == yggr::time::time_float_divides_cref(time_type(10, 0))
						/ yggr::time::time_float_divides_cref(time_type(20, 0)));

		{
			time_type tm(10, 0);
			time_type d(20, 0);

			const time_type::base_type& dcref = d;

			tm /= yggr::time::time_float_divides_cref(time_type(20, 0));
			yggr_test_assert(tm == time_type(0, time_type::E_S_AND_NS_HALF_RATIO));

			tm = time_type(10, 0);
			tm /= yggr::time::time_float_divides_cref(dcref);
			yggr_test_assert(tm == time_type(0, time_type::E_S_AND_NS_HALF_RATIO));

			tm = time_type(10, 0);
			yggr::time::time_float_divides_ref(tm) /= d;
			yggr_test_assert(tm == time_type(0, time_type::E_S_AND_NS_HALF_RATIO));

			tm = time_type(10, 0);
			yggr::time::time_float_divides_ref(tm) /= yggr::time::time_float_divides_cref(d);
			yggr_test_assert(tm == time_type(0, time_type::E_S_AND_NS_HALF_RATIO));

		}

		// f32 loss of floating point precision

		yggr_test_assert(
			std::abs(
				time_type(tm1.ex_to_second<yggr::f32>() / tm2.ex_to_second<yggr::f32>())
				- base_tm1 / tm2)
			< time_type::default_precision<yggr::f32>());

		yggr_test_assert(
			std::abs(
				time_type(tm1.ex_to_second<yggr::f32>() / tm2.ex_to_second<yggr::f32>())
				- tm1 / base_tm2)
			< time_type::default_precision<yggr::f32>());

		yggr_test_assert(
			std::abs(
				time_type(tm1.ex_to_second<yggr::f64>() / tm2.ex_to_second<yggr::f64>())
				- base_tm1 / tm2)
			< time_type::default_precision<yggr::f64>());

		yggr_test_assert(
			std::abs(
				time_type(tm1.ex_to_second<yggr::f64>() / tm2.ex_to_second<yggr::f64>())
				- tm1 / base_tm2)
			< time_type::default_precision<yggr::f64>());

		yggr_test_assert(
			std::abs(
				time_type(tm1.ex_to_second<yggr::f64>() / tm2.ex_to_second<yggr::f64>())
				- tm1 / tm2)
			< time_type::default_precision<yggr::f64>());

		yggr_test_assert(
			std::abs(time_type(-1000, 1000) - time_type(-2000, 2000) / 2.0)
			< time_type::default_precision<yggr::f64>());

		yggr_test_assert(
			std::abs(time_type(1000, 1000) - time_type(2000, 2000) / 2.0)
			< time_type::default_precision<yggr::f64>());
	}

	// mod
	{
		time_type tm1(10, 2);
		time_type tm2(-10, 1000);

		const time_type::base_type& base_tm1 = tm1;
		const time_type::base_type& base_tm2 = tm2;

		// mod int
		yggr_test_assert(time_type(0, 2) == tm1 % 2);
		yggr_test_assert(yggr::math::make_flaw_comparer(yggr::math::modulus(tm1.ex_to_second(), 2)) == (tm1 % 2).ex_to_second());

		yggr_test_assert(time_type(0) == 10 % time_type(2));
		yggr_test_assert(
			yggr::math::make_flaw_comparer(yggr::math::modulus(10, time_type(2).ex_to_second()))
			== (10 % time_type(2)).ex_to_second());

		yggr_test_assert(time_type(1) == 11 % time_type(2));
		yggr_test_assert(
			yggr::math::make_flaw_comparer(yggr::math::modulus(11, time_type(2).ex_to_second()))
			== (11 % time_type(2)).ex_to_second());

		yggr_test_assert(
			std::abs(
				time_type(0, time_type::E_S_AND_NS_RATIO / 10) - 10 % time_type(0.1))
			< time_type::default_precision<yggr::f64>());

		yggr_test_assert(time_type(1, 0) == time_type(3, 0) % 2);
		yggr_test_assert(
			yggr::math::make_flaw_comparer(yggr::math::modulus(time_type(3, 0).ex_to_second(), 2))
			== (time_type(3, 0) % 2).ex_to_second());

		yggr_test_assert(time_type(1, 0) == 3 % time_type(2, 0));

		yggr_test_assert(
			yggr::math::make_flaw_comparer(yggr::math::modulus(3, time_type(2, 0).ex_to_second()))
			== (3 % time_type(2, 0)).ex_to_second());

		yggr_test_assert(time_type(20, 4) % time_type(10, 2) == time_type(0, 0));
		yggr_test_assert(time_type(20, 4) % time_type(2, 0) == time_type(0, 4));
		yggr_test_assert(time_type(20, 0) % time_type(2, 0) == time_type(0, 0));


		{
			time_type tm(10, 0);
			time_type d(20, 0);

			const time_type::base_type& dcref = d;

			yggr_test_assert(yggr::math::modulus(tm, d) == time_type(10, 0));

			tm %= d;
			yggr_test_assert(tm == time_type(10, 0));

			tm = time_type(10, 0);
			tm %= dcref;
			yggr_test_assert(tm == time_type(10, 0));

			tm = time_type(10, 0);
			tm %= d.ex_to_second();
			yggr_test_assert(tm == time_type(10, 0));

		}

		// f32 loss of floating point precision

		yggr_test_assert(
			yggr::math::make_flaw_comparer(
				yggr::math::modulus(
					tm1.ex_to_second<yggr::f32>(),
					tm2.ex_to_second<yggr::f32>()) )
			== (base_tm1 % tm2).ex_to_second<yggr::f32>());

		yggr_test_assert(
			yggr::math::make_flaw_comparer(
				yggr::math::modulus(
					tm1.ex_to_second<yggr::f32>(),
					tm2.ex_to_second<yggr::f32>()) )
			== (tm1 % base_tm2).ex_to_second<yggr::f32>());

		yggr_test_assert(
			yggr::math::make_flaw_comparer(
				yggr::math::modulus(
					tm1.ex_to_second<yggr::f32>(),
					tm2.ex_to_second<yggr::f32>()) )
			== (tm1 % tm2).ex_to_second<yggr::f32>());

		yggr::f64 t = (base_tm1 % tm2).ex_to_second();
		std::cout << t << std::endl;
		yggr::f64 ft = yggr::math::modulus(
					tm1.ex_to_second<yggr::f64>(),
					tm2.ex_to_second<yggr::f64>());
        std::cout << ft << std::endl;

		yggr::f64 f64miss = 10.0 / time_type::E_S_AND_NS_RATIO;

		yggr_test_assert(
			std::abs(
				yggr::math::modulus(
					tm1.ex_to_second<yggr::f64>(),
					tm2.ex_to_second<yggr::f64>())
				- (base_tm1 % tm2).ex_to_second<yggr::f64>())
			< f64miss);

		yggr_test_assert(
			std::abs(
				yggr::math::modulus(
					tm1.ex_to_second<yggr::f64>(),
					tm2.ex_to_second<yggr::f64>())
				- (tm1 % base_tm2).ex_to_second<yggr::f64>())
			< f64miss);

		yggr_test_assert(
			std::abs(
				yggr::math::modulus(
					tm1.ex_to_second<yggr::f64>(),
					tm2.ex_to_second<yggr::f64>())
				- (tm1 % tm2).ex_to_second<yggr::f64>())
			< f64miss);

		yggr_test_assert(
			std::abs(
				yggr::math::modulus(
					time_type(-2000, 2000).ex_to_second<yggr::f64>(),
					2.0)
				- (time_type(-2000, 2000) % 2.0).ex_to_second<yggr::f64>())
			< f64miss);

		yggr_test_assert(
			std::abs(
				yggr::math::modulus(
					time_type(2000, 2000).ex_to_second<yggr::f64>(),
					2.0)
				- (time_type(2000, 2000) % 2.0).ex_to_second<yggr::f64>())
			< f64miss);
	}


	std::cout << "test2 succeed" << std::endl;
}

void test3(void)
{
	typedef yggr::time::time time_type;

	yggr::time::time tm;

	{
		yggr::utf8_string utf8_alloc_tmp;
		yggr::string yggr_alloc_tmp;
		std::string stl_alloc_tmp;

		char str_fmt[] = "%Y-%m-%d-%H-%M-%S";
		yggr::utf8_string utf8_fmt(str_fmt);
		yggr::string yggr_fmt(str_fmt);
		std::string stl_fmt(str_fmt);
		yggr::string_view yggr_view_fmt(str_fmt);
		yggr::utf8_string_view yggr_utf8_view_fmt(str_fmt);

		std::cout << tm.to_string() << std::endl;
		std::cout << tm.to_string(0) << std::endl;
		std::cout << tm.to_string(utf8_alloc_tmp.get_allocator()) << std::endl;

		std::cout << "-----------------------" << std::endl;

		std::cout << tm.to_string(str_fmt) << std::endl;
		std::cout << tm.to_string(str_fmt, utf8_alloc_tmp.get_allocator()) << std::endl;

		std::cout << tm.to_string<yggr::utf8_string>(str_fmt) << std::endl;
		std::cout << tm.to_string<yggr::utf8_string>(str_fmt, utf8_alloc_tmp.get_allocator()) << std::endl;
		std::cout << tm.to_string<yggr::utf8_string>() << std::endl;
		std::cout << tm.to_string<yggr::utf8_string>(utf8_alloc_tmp.get_allocator()) << std::endl;

		std::cout << tm.to_string<yggr::string>(str_fmt) << std::endl;
		std::cout << tm.to_string<yggr::string>(str_fmt, yggr_alloc_tmp.get_allocator()) << std::endl;
		std::cout << tm.to_string<yggr::string>() << std::endl;
		std::cout << tm.to_string<yggr::string>(yggr_alloc_tmp.get_allocator()) << std::endl;

		std::cout << tm.to_string<std::string>(str_fmt) << std::endl;
		std::cout << tm.to_string<std::string>(str_fmt, stl_alloc_tmp.get_allocator()) << std::endl;
		std::cout << tm.to_string<std::string>() << std::endl;
		std::cout << tm.to_string<std::string>(stl_alloc_tmp.get_allocator()) << std::endl;

		std::cout << "-----------------------" << std::endl;

		std::cout << tm.to_string(utf8_fmt) << std::endl;
		std::cout << tm.to_string(utf8_fmt, utf8_alloc_tmp.get_allocator()) << std::endl;

		std::cout << tm.to_string<yggr::utf8_string>(utf8_fmt) << std::endl;
		std::cout << tm.to_string<yggr::utf8_string>(utf8_fmt, utf8_alloc_tmp.get_allocator()) << std::endl;

		std::cout << tm.to_string<yggr::string>(utf8_fmt) << std::endl;
		std::cout << tm.to_string<yggr::string>(utf8_fmt, yggr_alloc_tmp.get_allocator()) << std::endl;

		std::cout << tm.to_string<std::string>(utf8_fmt) << std::endl;
		std::cout << tm.to_string<std::string>(utf8_fmt, stl_alloc_tmp.get_allocator()) << std::endl;

		std::cout << "-----------------------" << std::endl;

		std::cout << tm.to_string(yggr_fmt) << std::endl;
		std::cout << tm.to_string(yggr_fmt, utf8_alloc_tmp.get_allocator()) << std::endl;

		std::cout << tm.to_string<yggr::utf8_string>(yggr_fmt) << std::endl;
		std::cout << tm.to_string<yggr::utf8_string>(yggr_fmt, utf8_alloc_tmp.get_allocator()) << std::endl;

		std::cout << tm.to_string<yggr::string>(yggr_fmt) << std::endl;
		std::cout << tm.to_string<yggr::string>(yggr_fmt, yggr_alloc_tmp.get_allocator()) << std::endl;

		std::cout << tm.to_string<std::string>(yggr_fmt) << std::endl;
		std::cout << tm.to_string<std::string>(yggr_fmt, stl_alloc_tmp.get_allocator()) << std::endl;

		std::cout << "-----------------------" << std::endl;

		std::cout << tm.to_string(stl_fmt) << std::endl;
		std::cout << tm.to_string(stl_fmt, utf8_alloc_tmp.get_allocator()) << std::endl;

		std::cout << tm.to_string<yggr::utf8_string>(stl_fmt) << std::endl;
		std::cout << tm.to_string<yggr::utf8_string>(stl_fmt, utf8_alloc_tmp.get_allocator()) << std::endl;

		std::cout << tm.to_string<yggr::string>(stl_fmt) << std::endl;
		std::cout << tm.to_string<yggr::string>(stl_fmt, yggr_alloc_tmp.get_allocator()) << std::endl;

		std::cout << tm.to_string<std::string>(stl_fmt) << std::endl;
		std::cout << tm.to_string<std::string>(stl_fmt, stl_alloc_tmp.get_allocator()) << std::endl;

		std::cout << "-----------------------" << std::endl;

		std::cout << tm.to_string(yggr_view_fmt) << std::endl;
		std::cout << tm.to_string(yggr_view_fmt, utf8_alloc_tmp.get_allocator()) << std::endl;

		std::cout << tm.to_string<yggr::utf8_string>(yggr_view_fmt) << std::endl;
		std::cout << tm.to_string<yggr::utf8_string>(yggr_view_fmt, utf8_alloc_tmp.get_allocator()) << std::endl;

		std::cout << tm.to_string<yggr::string>(yggr_view_fmt) << std::endl;
		std::cout << tm.to_string<yggr::string>(yggr_view_fmt, yggr_alloc_tmp.get_allocator()) << std::endl;

		std::cout << tm.to_string<std::string>(yggr_view_fmt) << std::endl;
		std::cout << tm.to_string<std::string>(yggr_view_fmt, stl_alloc_tmp.get_allocator()) << std::endl;

		std::cout << "-----------------------" << std::endl;

		std::cout << tm.to_string(yggr_utf8_view_fmt) << std::endl;
		std::cout << tm.to_string(yggr_utf8_view_fmt, utf8_alloc_tmp.get_allocator()) << std::endl;

		std::cout << tm.to_string<yggr::utf8_string>(yggr_utf8_view_fmt) << std::endl;
		std::cout << tm.to_string<yggr::utf8_string>(yggr_utf8_view_fmt, utf8_alloc_tmp.get_allocator()) << std::endl;

		std::cout << tm.to_string<yggr::string>(yggr_utf8_view_fmt) << std::endl;
		std::cout << tm.to_string<yggr::string>(yggr_utf8_view_fmt, yggr_alloc_tmp.get_allocator()) << std::endl;

		std::cout << tm.to_string<std::string>(yggr_utf8_view_fmt) << std::endl;
		std::cout << tm.to_string<std::string>(yggr_utf8_view_fmt, stl_alloc_tmp.get_allocator()) << std::endl;
	}

	{
		yggr::utf8_string utf8_alloc_tmp;
		yggr::string yggr_alloc_tmp;
		std::string stl_alloc_tmp;

		char str_fmt[] = "%Y-%m-%d-%H-%M-%S";
		yggr::utf8_string utf8_fmt(str_fmt);
		yggr::string yggr_fmt(str_fmt);
		std::string stl_fmt(str_fmt);
		yggr::string_view yggr_view_fmt(str_fmt);
		yggr::utf8_string_view yggr_utf8_view_fmt(str_fmt);

		std::cout << time_type::time_string() << std::endl;
		std::cout << time_type::time_string(0) << std::endl;
		std::cout << time_type::time_string(utf8_alloc_tmp.get_allocator()) << std::endl;

		std::cout << "-----------------------" << std::endl;

		std::cout << time_type::time_string(str_fmt) << std::endl;
		std::cout << time_type::time_string(str_fmt, utf8_alloc_tmp.get_allocator()) << std::endl;

		std::cout << time_type::time_string<yggr::utf8_string>(str_fmt) << std::endl;
		std::cout << time_type::time_string<yggr::utf8_string>(str_fmt, utf8_alloc_tmp.get_allocator()) << std::endl;
		std::cout << time_type::time_string<yggr::utf8_string>() << std::endl;
		std::cout << time_type::time_string<yggr::utf8_string>(utf8_alloc_tmp.get_allocator()) << std::endl;

		std::cout << time_type::time_string<yggr::string>(str_fmt) << std::endl;
		std::cout << time_type::time_string<yggr::string>(str_fmt, yggr_alloc_tmp.get_allocator()) << std::endl;
		std::cout << time_type::time_string<yggr::string>() << std::endl;
		std::cout << time_type::time_string<yggr::string>(yggr_alloc_tmp.get_allocator()) << std::endl;

		std::cout << time_type::time_string<std::string>(str_fmt) << std::endl;
		std::cout << time_type::time_string<std::string>(str_fmt, stl_alloc_tmp.get_allocator()) << std::endl;
		std::cout << time_type::time_string<std::string>() << std::endl;
		std::cout << time_type::time_string<std::string>(stl_alloc_tmp.get_allocator()) << std::endl;

		std::cout << "-----------------------" << std::endl;

		std::cout << time_type::time_string(utf8_fmt) << std::endl;
		std::cout << time_type::time_string(utf8_fmt, utf8_alloc_tmp.get_allocator()) << std::endl;

		std::cout << time_type::time_string<yggr::utf8_string>(utf8_fmt) << std::endl;
		std::cout << time_type::time_string<yggr::utf8_string>(utf8_fmt, utf8_alloc_tmp.get_allocator()) << std::endl;

		std::cout << time_type::time_string<yggr::string>(utf8_fmt) << std::endl;
		std::cout << time_type::time_string<yggr::string>(utf8_fmt, yggr_alloc_tmp.get_allocator()) << std::endl;

		std::cout << time_type::time_string<std::string>(utf8_fmt) << std::endl;
		std::cout << time_type::time_string<std::string>(utf8_fmt, stl_alloc_tmp.get_allocator()) << std::endl;

		std::cout << "-----------------------" << std::endl;

		std::cout << time_type::time_string(yggr_fmt) << std::endl;
		std::cout << time_type::time_string(yggr_fmt, utf8_alloc_tmp.get_allocator()) << std::endl;

		std::cout << time_type::time_string<yggr::utf8_string>(yggr_fmt) << std::endl;
		std::cout << time_type::time_string<yggr::utf8_string>(yggr_fmt, utf8_alloc_tmp.get_allocator()) << std::endl;

		std::cout << time_type::time_string<yggr::string>(yggr_fmt) << std::endl;
		std::cout << time_type::time_string<yggr::string>(yggr_fmt, yggr_alloc_tmp.get_allocator()) << std::endl;

		std::cout << time_type::time_string<std::string>(yggr_fmt) << std::endl;
		std::cout << time_type::time_string<std::string>(yggr_fmt, stl_alloc_tmp.get_allocator()) << std::endl;

		std::cout << "-----------------------" << std::endl;

		std::cout << time_type::time_string(stl_fmt) << std::endl;
		std::cout << time_type::time_string(stl_fmt, utf8_alloc_tmp.get_allocator()) << std::endl;

		std::cout << time_type::time_string<yggr::utf8_string>(stl_fmt) << std::endl;
		std::cout << time_type::time_string<yggr::utf8_string>(stl_fmt, utf8_alloc_tmp.get_allocator()) << std::endl;

		std::cout << time_type::time_string<yggr::string>(stl_fmt) << std::endl;
		std::cout << time_type::time_string<yggr::string>(stl_fmt, yggr_alloc_tmp.get_allocator()) << std::endl;

		std::cout << time_type::time_string<std::string>(stl_fmt) << std::endl;
		std::cout << time_type::time_string<std::string>(stl_fmt, stl_alloc_tmp.get_allocator()) << std::endl;

		std::cout << "-----------------------" << std::endl;

		std::cout << time_type::time_string(yggr_view_fmt) << std::endl;
		std::cout << time_type::time_string(yggr_view_fmt, utf8_alloc_tmp.get_allocator()) << std::endl;

		std::cout << time_type::time_string<yggr::utf8_string>(yggr_view_fmt) << std::endl;
		std::cout << time_type::time_string<yggr::utf8_string>(yggr_view_fmt, utf8_alloc_tmp.get_allocator()) << std::endl;

		std::cout << time_type::time_string<yggr::string>(yggr_view_fmt) << std::endl;
		std::cout << time_type::time_string<yggr::string>(yggr_view_fmt, yggr_alloc_tmp.get_allocator()) << std::endl;

		std::cout << time_type::time_string<std::string>(yggr_view_fmt) << std::endl;
		std::cout << time_type::time_string<std::string>(yggr_view_fmt, stl_alloc_tmp.get_allocator()) << std::endl;

		std::cout << "-----------------------" << std::endl;

		std::cout << time_type::time_string(yggr_utf8_view_fmt) << std::endl;
		std::cout << time_type::time_string(yggr_utf8_view_fmt, utf8_alloc_tmp.get_allocator()) << std::endl;

		std::cout << time_type::time_string<yggr::utf8_string>(yggr_utf8_view_fmt) << std::endl;
		std::cout << time_type::time_string<yggr::utf8_string>(yggr_utf8_view_fmt, utf8_alloc_tmp.get_allocator()) << std::endl;

		std::cout << time_type::time_string<yggr::string>(yggr_utf8_view_fmt) << std::endl;
		std::cout << time_type::time_string<yggr::string>(yggr_utf8_view_fmt, yggr_alloc_tmp.get_allocator()) << std::endl;

		std::cout << time_type::time_string<std::string>(yggr_utf8_view_fmt) << std::endl;
		std::cout << time_type::time_string<std::string>(yggr_utf8_view_fmt, stl_alloc_tmp.get_allocator()) << std::endl;
	}

#ifndef YGGR_NO_CWCHAR
	{
		yggr::utf8_string utf8_alloc_tmp;
		yggr::wstring yggr_alloc_tmp;
		std::wstring stl_alloc_tmp;

		wchar_t str_fmt[] = L"%Y年%m月%d日 %H时:%M分:%S秒";
		yggr::utf8_string utf8_fmt(str_fmt);
		yggr::wstring yggr_fmt(str_fmt);
		std::wstring stl_fmt(str_fmt);
		yggr::wstring_view yggr_view_fmt(str_fmt);

		std::wcout << tm.to_string() << std::endl;
		std::wcout << tm.to_string(0) << std::endl;
		std::wcout << tm.to_string(utf8_alloc_tmp.get_allocator()) << std::endl;

		std::wcout << L"-----------------------" << std::endl;

		std::wcout << tm.to_string(str_fmt) << std::endl;
		std::wcout << tm.to_string(str_fmt, utf8_alloc_tmp.get_allocator()) << std::endl;

		std::wcout << tm.to_string<yggr::utf8_string>(str_fmt) << std::endl;
		std::wcout << tm.to_string<yggr::utf8_string>(str_fmt, utf8_alloc_tmp.get_allocator()) << std::endl;
		std::wcout << tm.to_string<yggr::utf8_string>() << std::endl;
		std::wcout << tm.to_string<yggr::utf8_string>(utf8_alloc_tmp.get_allocator()) << std::endl;

		std::wcout << tm.to_string<yggr::wstring>(str_fmt) << std::endl;
		std::wcout << tm.to_string<yggr::wstring>(str_fmt, yggr_alloc_tmp.get_allocator()) << std::endl;
		std::wcout << tm.to_string<yggr::wstring>() << std::endl;
		std::wcout << tm.to_string<yggr::wstring>(yggr_alloc_tmp.get_allocator()) << std::endl;

		std::wcout << tm.to_string<std::wstring>(str_fmt) << std::endl;
		std::wcout << tm.to_string<std::wstring>(str_fmt, stl_alloc_tmp.get_allocator()) << std::endl;
		std::wcout << tm.to_string<std::wstring>() << std::endl;
		std::wcout << tm.to_string<std::wstring>(stl_alloc_tmp.get_allocator()) << std::endl;

		std::wcout << L"-----------------------" << std::endl;

		std::wcout << tm.to_string(utf8_fmt) << std::endl;
		std::wcout << tm.to_string(utf8_fmt, utf8_alloc_tmp.get_allocator()) << std::endl;

		std::wcout << tm.to_string<yggr::utf8_string>(utf8_fmt) << std::endl;
		std::wcout << tm.to_string<yggr::utf8_string>(utf8_fmt, utf8_alloc_tmp.get_allocator()) << std::endl;

		std::wcout << tm.to_string<yggr::wstring>(utf8_fmt) << std::endl;
		std::wcout << tm.to_string<yggr::wstring>(utf8_fmt, yggr_alloc_tmp.get_allocator()) << std::endl;

		std::wcout << tm.to_string<std::wstring>(utf8_fmt) << std::endl;
		std::wcout << tm.to_string<std::wstring>(utf8_fmt, stl_alloc_tmp.get_allocator()) << std::endl;

		std::wcout << L"-----------------------" << std::endl;

		std::wcout << tm.to_string(yggr_fmt) << std::endl;
		std::wcout << tm.to_string(yggr_fmt, utf8_alloc_tmp.get_allocator()) << std::endl;

		std::wcout << tm.to_string<yggr::utf8_string>(yggr_fmt) << std::endl;
		std::wcout << tm.to_string<yggr::utf8_string>(yggr_fmt, utf8_alloc_tmp.get_allocator()) << std::endl;

		std::wcout << tm.to_string<yggr::wstring>(yggr_fmt) << std::endl;
		std::wcout << tm.to_string<yggr::wstring>(yggr_fmt, yggr_alloc_tmp.get_allocator()) << std::endl;

		std::wcout << tm.to_string<std::wstring>(yggr_fmt) << std::endl;
		std::wcout << tm.to_string<std::wstring>(yggr_fmt, stl_alloc_tmp.get_allocator()) << std::endl;

		std::wcout << L"-----------------------" << std::endl;

		std::wcout << tm.to_string(stl_fmt) << std::endl;
		std::wcout << tm.to_string(stl_fmt, utf8_alloc_tmp.get_allocator()) << std::endl;

		std::wcout << tm.to_string<yggr::utf8_string>(stl_fmt) << std::endl;
		std::wcout << tm.to_string<yggr::utf8_string>(stl_fmt, utf8_alloc_tmp.get_allocator()) << std::endl;

		std::wcout << tm.to_string<yggr::wstring>(stl_fmt) << std::endl;
		std::wcout << tm.to_string<yggr::wstring>(stl_fmt, yggr_alloc_tmp.get_allocator()) << std::endl;

		std::wcout << tm.to_string<std::wstring>(stl_fmt) << std::endl;
		std::wcout << tm.to_string<std::wstring>(stl_fmt, stl_alloc_tmp.get_allocator()) << std::endl;

		std::wcout << L"-----------------------" << std::endl;

		std::wcout << tm.to_string(yggr_view_fmt) << std::endl;
		std::wcout << tm.to_string(yggr_view_fmt, utf8_alloc_tmp.get_allocator()) << std::endl;

		std::wcout << tm.to_string<yggr::utf8_string>(yggr_view_fmt) << std::endl;
		std::wcout << tm.to_string<yggr::utf8_string>(yggr_view_fmt, utf8_alloc_tmp.get_allocator()) << std::endl;

		std::wcout << tm.to_string<yggr::wstring>(yggr_view_fmt) << std::endl;
		std::wcout << tm.to_string<yggr::wstring>(yggr_view_fmt, yggr_alloc_tmp.get_allocator()) << std::endl;

		std::wcout << tm.to_string<std::wstring>(yggr_view_fmt) << std::endl;
		std::wcout << tm.to_string<std::wstring>(yggr_view_fmt, stl_alloc_tmp.get_allocator()) << std::endl;
	}

	{
		yggr::utf8_string utf8_alloc_tmp;
		yggr::wstring yggr_alloc_tmp;
		std::wstring stl_alloc_tmp;

		wchar_t str_fmt[] = L"%Y年%m月%d日 %H时:%M分:%S秒";
		yggr::utf8_string utf8_fmt(str_fmt);
		yggr::wstring yggr_fmt(str_fmt);
		std::wstring stl_fmt(str_fmt);
		yggr::wstring_view yggr_view_fmt(str_fmt);

		std::wcout << time_type::time_string() << std::endl;
		std::wcout << time_type::time_string(0) << std::endl;
		std::wcout << time_type::time_string(utf8_alloc_tmp.get_allocator()) << std::endl;

		std::wcout << L"-----------------------" << std::endl;

		std::wcout << time_type::time_string(str_fmt) << std::endl;
		std::wcout << time_type::time_string(str_fmt, utf8_alloc_tmp.get_allocator()) << std::endl;

		std::wcout << time_type::time_string<yggr::utf8_string>(str_fmt) << std::endl;
		std::wcout << time_type::time_string<yggr::utf8_string>(str_fmt, utf8_alloc_tmp.get_allocator()) << std::endl;
		std::wcout << time_type::time_string<yggr::utf8_string>() << std::endl;
		std::wcout << time_type::time_string<yggr::utf8_string>(utf8_alloc_tmp.get_allocator()) << std::endl;

		std::wcout << time_type::time_string<yggr::wstring>(str_fmt) << std::endl;
		std::wcout << time_type::time_string<yggr::wstring>(str_fmt, yggr_alloc_tmp.get_allocator()) << std::endl;
		std::wcout << time_type::time_string<yggr::wstring>() << std::endl;
		std::wcout << time_type::time_string<yggr::wstring>(yggr_alloc_tmp.get_allocator()) << std::endl;

		std::wcout << time_type::time_string<std::wstring>(str_fmt) << std::endl;
		std::wcout << time_type::time_string<std::wstring>(str_fmt, stl_alloc_tmp.get_allocator()) << std::endl;
		std::wcout << time_type::time_string<std::wstring>() << std::endl;
		std::wcout << time_type::time_string<std::wstring>(stl_alloc_tmp.get_allocator()) << std::endl;

		std::wcout << L"-----------------------" << std::endl;

		std::wcout << time_type::time_string(utf8_fmt) << std::endl;
		std::wcout << time_type::time_string(utf8_fmt, utf8_alloc_tmp.get_allocator()) << std::endl;

		std::wcout << time_type::time_string<yggr::utf8_string>(utf8_fmt) << std::endl;
		std::wcout << time_type::time_string<yggr::utf8_string>(utf8_fmt, utf8_alloc_tmp.get_allocator()) << std::endl;

		std::wcout << time_type::time_string<yggr::wstring>(utf8_fmt) << std::endl;
		std::wcout << time_type::time_string<yggr::wstring>(utf8_fmt, yggr_alloc_tmp.get_allocator()) << std::endl;

		std::wcout << time_type::time_string<std::wstring>(utf8_fmt) << std::endl;
		std::wcout << time_type::time_string<std::wstring>(utf8_fmt, stl_alloc_tmp.get_allocator()) << std::endl;

		std::wcout << L"-----------------------" << std::endl;

		std::wcout << time_type::time_string(yggr_fmt) << std::endl;
		std::wcout << time_type::time_string(yggr_fmt, utf8_alloc_tmp.get_allocator()) << std::endl;

		std::wcout << time_type::time_string<yggr::utf8_string>(yggr_fmt) << std::endl;
		std::wcout << time_type::time_string<yggr::utf8_string>(yggr_fmt, utf8_alloc_tmp.get_allocator()) << std::endl;

		std::wcout << time_type::time_string<yggr::wstring>(yggr_fmt) << std::endl;
		std::wcout << time_type::time_string<yggr::wstring>(yggr_fmt, yggr_alloc_tmp.get_allocator()) << std::endl;

		std::wcout << time_type::time_string<std::wstring>(yggr_fmt) << std::endl;
		std::wcout << time_type::time_string<std::wstring>(yggr_fmt, stl_alloc_tmp.get_allocator()) << std::endl;

		std::wcout << L"-----------------------" << std::endl;

		std::wcout << time_type::time_string(stl_fmt) << std::endl;
		std::wcout << time_type::time_string(stl_fmt, utf8_alloc_tmp.get_allocator()) << std::endl;

		std::wcout << time_type::time_string<yggr::utf8_string>(stl_fmt) << std::endl;
		std::wcout << time_type::time_string<yggr::utf8_string>(stl_fmt, utf8_alloc_tmp.get_allocator()) << std::endl;

		std::wcout << time_type::time_string<yggr::wstring>(stl_fmt) << std::endl;
		std::wcout << time_type::time_string<yggr::wstring>(stl_fmt, yggr_alloc_tmp.get_allocator()) << std::endl;

		std::wcout << time_type::time_string<std::wstring>(stl_fmt) << std::endl;
		std::wcout << time_type::time_string<std::wstring>(stl_fmt, stl_alloc_tmp.get_allocator()) << std::endl;

		std::wcout << L"-----------------------" << std::endl;

		std::wcout << time_type::time_string(yggr_view_fmt) << std::endl;
		std::wcout << time_type::time_string(yggr_view_fmt, utf8_alloc_tmp.get_allocator()) << std::endl;

		std::wcout << time_type::time_string<yggr::utf8_string>(yggr_view_fmt) << std::endl;
		std::wcout << time_type::time_string<yggr::utf8_string>(yggr_view_fmt, utf8_alloc_tmp.get_allocator()) << std::endl;

		std::wcout << time_type::time_string<yggr::wstring>(yggr_view_fmt) << std::endl;
		std::wcout << time_type::time_string<yggr::wstring>(yggr_view_fmt, yggr_alloc_tmp.get_allocator()) << std::endl;

		std::wcout << time_type::time_string<std::wstring>(yggr_view_fmt) << std::endl;
		std::wcout << time_type::time_string<std::wstring>(yggr_view_fmt, stl_alloc_tmp.get_allocator()) << std::endl;
	}
#endif // YGGR_NO_CWCHAR

	std::cout << "test3 succeed" << std::endl;
}

void test4(void)
{
	typedef yggr::time::time time_type;

	yggr::time::time tm;

	{
		yggr::utf8_string utf8_alloc_tmp;
		yggr::string yggr_alloc_tmp;
		std::string stl_alloc_tmp;

		char str_fmt[] = "%Y-%m-%d-%H-%M-%S";
		yggr::utf8_string utf8_fmt(str_fmt);
		yggr::string yggr_fmt(str_fmt);
		std::string stl_fmt(str_fmt);
		yggr::string_view yggr_view_fmt(str_fmt);
		yggr::utf8_string_view yggr_utf8_view_fmt(str_fmt);

		std::cout << tm.to_local_string() << std::endl;
		std::cout << tm.to_local_string(0) << std::endl;
		std::cout << tm.to_local_string(utf8_alloc_tmp.get_allocator()) << std::endl;

		std::cout << "-----------------------" << std::endl;

		std::cout << tm.to_local_string(str_fmt) << std::endl;
		std::cout << tm.to_local_string(str_fmt, utf8_alloc_tmp.get_allocator()) << std::endl;

		std::cout << tm.to_local_string<yggr::utf8_string>(str_fmt) << std::endl;
		std::cout << tm.to_local_string<yggr::utf8_string>(str_fmt, utf8_alloc_tmp.get_allocator()) << std::endl;
		std::cout << tm.to_local_string<yggr::utf8_string>() << std::endl;
		std::cout << tm.to_local_string<yggr::utf8_string>(utf8_alloc_tmp.get_allocator()) << std::endl;

		std::cout << tm.to_local_string<yggr::string>(str_fmt) << std::endl;
		std::cout << tm.to_local_string<yggr::string>(str_fmt, yggr_alloc_tmp.get_allocator()) << std::endl;
		std::cout << tm.to_local_string<yggr::string>() << std::endl;
		std::cout << tm.to_local_string<yggr::string>(yggr_alloc_tmp.get_allocator()) << std::endl;

		std::cout << tm.to_local_string<std::string>(str_fmt) << std::endl;
		std::cout << tm.to_local_string<std::string>(str_fmt, stl_alloc_tmp.get_allocator()) << std::endl;
		std::cout << tm.to_local_string<std::string>() << std::endl;
		std::cout << tm.to_local_string<std::string>(stl_alloc_tmp.get_allocator()) << std::endl;

		std::cout << "-----------------------" << std::endl;

		std::cout << tm.to_local_string(utf8_fmt) << std::endl;
		std::cout << tm.to_local_string(utf8_fmt, utf8_alloc_tmp.get_allocator()) << std::endl;

		std::cout << tm.to_local_string<yggr::utf8_string>(utf8_fmt) << std::endl;
		std::cout << tm.to_local_string<yggr::utf8_string>(utf8_fmt, utf8_alloc_tmp.get_allocator()) << std::endl;

		std::cout << tm.to_local_string<yggr::string>(utf8_fmt) << std::endl;
		std::cout << tm.to_local_string<yggr::string>(utf8_fmt, yggr_alloc_tmp.get_allocator()) << std::endl;

		std::cout << tm.to_local_string<std::string>(utf8_fmt) << std::endl;
		std::cout << tm.to_local_string<std::string>(utf8_fmt, stl_alloc_tmp.get_allocator()) << std::endl;

		std::cout << "-----------------------" << std::endl;

		std::cout << tm.to_local_string(yggr_fmt) << std::endl;
		std::cout << tm.to_local_string(yggr_fmt, utf8_alloc_tmp.get_allocator()) << std::endl;

		std::cout << tm.to_local_string<yggr::utf8_string>(yggr_fmt) << std::endl;
		std::cout << tm.to_local_string<yggr::utf8_string>(yggr_fmt, utf8_alloc_tmp.get_allocator()) << std::endl;

		std::cout << tm.to_local_string<yggr::string>(yggr_fmt) << std::endl;
		std::cout << tm.to_local_string<yggr::string>(yggr_fmt, yggr_alloc_tmp.get_allocator()) << std::endl;

		std::cout << tm.to_local_string<std::string>(yggr_fmt) << std::endl;
		std::cout << tm.to_local_string<std::string>(yggr_fmt, stl_alloc_tmp.get_allocator()) << std::endl;

		std::cout << "-----------------------" << std::endl;

		std::cout << tm.to_local_string(stl_fmt) << std::endl;
		std::cout << tm.to_local_string(stl_fmt, utf8_alloc_tmp.get_allocator()) << std::endl;

		std::cout << tm.to_local_string<yggr::utf8_string>(stl_fmt) << std::endl;
		std::cout << tm.to_local_string<yggr::utf8_string>(stl_fmt, utf8_alloc_tmp.get_allocator()) << std::endl;

		std::cout << tm.to_local_string<yggr::string>(stl_fmt) << std::endl;
		std::cout << tm.to_local_string<yggr::string>(stl_fmt, yggr_alloc_tmp.get_allocator()) << std::endl;

		std::cout << tm.to_local_string<std::string>(stl_fmt) << std::endl;
		std::cout << tm.to_local_string<std::string>(stl_fmt, stl_alloc_tmp.get_allocator()) << std::endl;

		std::cout << "-----------------------" << std::endl;

		std::cout << tm.to_local_string(yggr_view_fmt) << std::endl;
		std::cout << tm.to_local_string(yggr_view_fmt, utf8_alloc_tmp.get_allocator()) << std::endl;

		std::cout << tm.to_local_string<yggr::utf8_string>(yggr_view_fmt) << std::endl;
		std::cout << tm.to_local_string<yggr::utf8_string>(yggr_view_fmt, utf8_alloc_tmp.get_allocator()) << std::endl;

		std::cout << tm.to_local_string<yggr::string>(yggr_view_fmt) << std::endl;
		std::cout << tm.to_local_string<yggr::string>(yggr_view_fmt, yggr_alloc_tmp.get_allocator()) << std::endl;

		std::cout << tm.to_local_string<std::string>(yggr_view_fmt) << std::endl;
		std::cout << tm.to_local_string<std::string>(yggr_view_fmt, stl_alloc_tmp.get_allocator()) << std::endl;

		std::cout << "-----------------------" << std::endl;

		std::cout << tm.to_local_string(yggr_utf8_view_fmt) << std::endl;
		std::cout << tm.to_local_string(yggr_utf8_view_fmt, utf8_alloc_tmp.get_allocator()) << std::endl;

		std::cout << tm.to_local_string<yggr::utf8_string>(yggr_utf8_view_fmt) << std::endl;
		std::cout << tm.to_local_string<yggr::utf8_string>(yggr_utf8_view_fmt, utf8_alloc_tmp.get_allocator()) << std::endl;

		std::cout << tm.to_local_string<yggr::string>(yggr_utf8_view_fmt) << std::endl;
		std::cout << tm.to_local_string<yggr::string>(yggr_utf8_view_fmt, yggr_alloc_tmp.get_allocator()) << std::endl;

		std::cout << tm.to_local_string<std::string>(yggr_utf8_view_fmt) << std::endl;
		std::cout << tm.to_local_string<std::string>(yggr_utf8_view_fmt, stl_alloc_tmp.get_allocator()) << std::endl;
	}

	{
		yggr::utf8_string utf8_alloc_tmp;
		yggr::string yggr_alloc_tmp;
		std::string stl_alloc_tmp;

		char str_fmt[] = "%Y-%m-%d-%H-%M-%S";
		yggr::utf8_string utf8_fmt(str_fmt);
		yggr::string yggr_fmt(str_fmt);
		std::string stl_fmt(str_fmt);
		yggr::string_view yggr_view_fmt(str_fmt);
		yggr::utf8_string_view yggr_utf8_view_fmt(str_fmt);

		std::cout << time_type::local_time_string() << std::endl;
		std::cout << time_type::local_time_string(0) << std::endl;
		std::cout << time_type::local_time_string(utf8_alloc_tmp.get_allocator()) << std::endl;

		std::cout << "-----------------------" << std::endl;

		std::cout << time_type::local_time_string(str_fmt) << std::endl;
		std::cout << time_type::local_time_string(str_fmt, utf8_alloc_tmp.get_allocator()) << std::endl;

		std::cout << time_type::local_time_string<yggr::utf8_string>(str_fmt) << std::endl;
		std::cout << time_type::local_time_string<yggr::utf8_string>(str_fmt, utf8_alloc_tmp.get_allocator()) << std::endl;
		std::cout << time_type::local_time_string<yggr::utf8_string>() << std::endl;
		std::cout << time_type::local_time_string<yggr::utf8_string>(utf8_alloc_tmp.get_allocator()) << std::endl;

		std::cout << time_type::local_time_string<yggr::string>(str_fmt) << std::endl;
		std::cout << time_type::local_time_string<yggr::string>(str_fmt, yggr_alloc_tmp.get_allocator()) << std::endl;
		std::cout << time_type::local_time_string<yggr::string>() << std::endl;
		std::cout << time_type::local_time_string<yggr::string>(yggr_alloc_tmp.get_allocator()) << std::endl;

		std::cout << time_type::local_time_string<std::string>(str_fmt) << std::endl;
		std::cout << time_type::local_time_string<std::string>(str_fmt, stl_alloc_tmp.get_allocator()) << std::endl;
		std::cout << time_type::local_time_string<std::string>() << std::endl;
		std::cout << time_type::local_time_string<std::string>(stl_alloc_tmp.get_allocator()) << std::endl;

		std::cout << "-----------------------" << std::endl;

		std::cout << time_type::local_time_string(utf8_fmt) << std::endl;
		std::cout << time_type::local_time_string(utf8_fmt, utf8_alloc_tmp.get_allocator()) << std::endl;

		std::cout << time_type::local_time_string<yggr::utf8_string>(utf8_fmt) << std::endl;
		std::cout << time_type::local_time_string<yggr::utf8_string>(utf8_fmt, utf8_alloc_tmp.get_allocator()) << std::endl;

		std::cout << time_type::local_time_string<yggr::string>(utf8_fmt) << std::endl;
		std::cout << time_type::local_time_string<yggr::string>(utf8_fmt, yggr_alloc_tmp.get_allocator()) << std::endl;

		std::cout << time_type::local_time_string<std::string>(utf8_fmt) << std::endl;
		std::cout << time_type::local_time_string<std::string>(utf8_fmt, stl_alloc_tmp.get_allocator()) << std::endl;

		std::cout << "-----------------------" << std::endl;

		std::cout << time_type::local_time_string(yggr_fmt) << std::endl;
		std::cout << time_type::local_time_string(yggr_fmt, utf8_alloc_tmp.get_allocator()) << std::endl;

		std::cout << time_type::local_time_string<yggr::utf8_string>(yggr_fmt) << std::endl;
		std::cout << time_type::local_time_string<yggr::utf8_string>(yggr_fmt, utf8_alloc_tmp.get_allocator()) << std::endl;

		std::cout << time_type::local_time_string<yggr::string>(yggr_fmt) << std::endl;
		std::cout << time_type::local_time_string<yggr::string>(yggr_fmt, yggr_alloc_tmp.get_allocator()) << std::endl;

		std::cout << time_type::local_time_string<std::string>(yggr_fmt) << std::endl;
		std::cout << time_type::local_time_string<std::string>(yggr_fmt, stl_alloc_tmp.get_allocator()) << std::endl;

		std::cout << "-----------------------" << std::endl;

		std::cout << time_type::local_time_string(stl_fmt) << std::endl;
		std::cout << time_type::local_time_string(stl_fmt, utf8_alloc_tmp.get_allocator()) << std::endl;

		std::cout << time_type::local_time_string<yggr::utf8_string>(stl_fmt) << std::endl;
		std::cout << time_type::local_time_string<yggr::utf8_string>(stl_fmt, utf8_alloc_tmp.get_allocator()) << std::endl;

		std::cout << time_type::local_time_string<yggr::string>(stl_fmt) << std::endl;
		std::cout << time_type::local_time_string<yggr::string>(stl_fmt, yggr_alloc_tmp.get_allocator()) << std::endl;

		std::cout << time_type::local_time_string<std::string>(stl_fmt) << std::endl;
		std::cout << time_type::local_time_string<std::string>(stl_fmt, stl_alloc_tmp.get_allocator()) << std::endl;

		std::cout << "-----------------------" << std::endl;

		std::cout << time_type::local_time_string(yggr_view_fmt) << std::endl;
		std::cout << time_type::local_time_string(yggr_view_fmt, utf8_alloc_tmp.get_allocator()) << std::endl;

		std::cout << time_type::local_time_string<yggr::utf8_string>(yggr_view_fmt) << std::endl;
		std::cout << time_type::local_time_string<yggr::utf8_string>(yggr_view_fmt, utf8_alloc_tmp.get_allocator()) << std::endl;

		std::cout << time_type::local_time_string<yggr::string>(yggr_view_fmt) << std::endl;
		std::cout << time_type::local_time_string<yggr::string>(yggr_view_fmt, yggr_alloc_tmp.get_allocator()) << std::endl;

		std::cout << time_type::local_time_string<std::string>(yggr_view_fmt) << std::endl;
		std::cout << time_type::local_time_string<std::string>(yggr_view_fmt, stl_alloc_tmp.get_allocator()) << std::endl;

		std::cout << "-----------------------" << std::endl;

		std::cout << time_type::local_time_string(yggr_utf8_view_fmt) << std::endl;
		std::cout << time_type::local_time_string(yggr_utf8_view_fmt, utf8_alloc_tmp.get_allocator()) << std::endl;

		std::cout << time_type::local_time_string<yggr::utf8_string>(yggr_utf8_view_fmt) << std::endl;
		std::cout << time_type::local_time_string<yggr::utf8_string>(yggr_utf8_view_fmt, utf8_alloc_tmp.get_allocator()) << std::endl;

		std::cout << time_type::local_time_string<yggr::string>(yggr_utf8_view_fmt) << std::endl;
		std::cout << time_type::local_time_string<yggr::string>(yggr_utf8_view_fmt, yggr_alloc_tmp.get_allocator()) << std::endl;

		std::cout << time_type::local_time_string<std::string>(yggr_utf8_view_fmt) << std::endl;
		std::cout << time_type::local_time_string<std::string>(yggr_utf8_view_fmt, stl_alloc_tmp.get_allocator()) << std::endl;
	}

#ifndef YGGR_NO_CWCHAR
	{
		yggr::utf8_string utf8_alloc_tmp;
		yggr::wstring yggr_alloc_tmp;
		std::wstring stl_alloc_tmp;

		wchar_t str_fmt[] = L"%Y年%m月%d日 %H时:%M分:%S秒";
		yggr::utf8_string utf8_fmt(str_fmt);
		yggr::wstring yggr_fmt(str_fmt);
		std::wstring stl_fmt(str_fmt);
		yggr::wstring_view yggr_view_fmt(str_fmt);

		std::wcout << tm.to_local_string() << std::endl;
		std::wcout << tm.to_local_string(0) << std::endl;
		std::wcout << tm.to_local_string(utf8_alloc_tmp.get_allocator()) << std::endl;

		std::wcout << L"-----------------------" << std::endl;

		std::wcout << tm.to_local_string(str_fmt) << std::endl;
		std::wcout << tm.to_local_string(str_fmt, utf8_alloc_tmp.get_allocator()) << std::endl;

		std::wcout << tm.to_local_string<yggr::utf8_string>(str_fmt) << std::endl;
		std::wcout << tm.to_local_string<yggr::utf8_string>(str_fmt, utf8_alloc_tmp.get_allocator()) << std::endl;
		std::wcout << tm.to_local_string<yggr::utf8_string>() << std::endl;
		std::wcout << tm.to_local_string<yggr::utf8_string>(utf8_alloc_tmp.get_allocator()) << std::endl;

		std::wcout << tm.to_local_string<yggr::wstring>(str_fmt) << std::endl;
		std::wcout << tm.to_local_string<yggr::wstring>(str_fmt, yggr_alloc_tmp.get_allocator()) << std::endl;
		std::wcout << tm.to_local_string<yggr::wstring>() << std::endl;
		std::wcout << tm.to_local_string<yggr::wstring>(yggr_alloc_tmp.get_allocator()) << std::endl;

		std::wcout << tm.to_local_string<std::wstring>(str_fmt) << std::endl;
		std::wcout << tm.to_local_string<std::wstring>(str_fmt, stl_alloc_tmp.get_allocator()) << std::endl;
		std::wcout << tm.to_local_string<std::wstring>() << std::endl;
		std::wcout << tm.to_local_string<std::wstring>(stl_alloc_tmp.get_allocator()) << std::endl;

		std::wcout << L"-----------------------" << std::endl;

		std::wcout << tm.to_local_string(utf8_fmt) << std::endl;
		std::wcout << tm.to_local_string(utf8_fmt, utf8_alloc_tmp.get_allocator()) << std::endl;

		std::wcout << tm.to_local_string<yggr::utf8_string>(utf8_fmt) << std::endl;
		std::wcout << tm.to_local_string<yggr::utf8_string>(utf8_fmt, utf8_alloc_tmp.get_allocator()) << std::endl;

		std::wcout << tm.to_local_string<yggr::wstring>(utf8_fmt) << std::endl;
		std::wcout << tm.to_local_string<yggr::wstring>(utf8_fmt, yggr_alloc_tmp.get_allocator()) << std::endl;

		std::wcout << tm.to_local_string<std::wstring>(utf8_fmt) << std::endl;
		std::wcout << tm.to_local_string<std::wstring>(utf8_fmt, stl_alloc_tmp.get_allocator()) << std::endl;

		std::wcout << L"-----------------------" << std::endl;

		std::wcout << tm.to_local_string(yggr_fmt) << std::endl;
		std::wcout << tm.to_local_string(yggr_fmt, utf8_alloc_tmp.get_allocator()) << std::endl;

		std::wcout << tm.to_local_string<yggr::utf8_string>(yggr_fmt) << std::endl;
		std::wcout << tm.to_local_string<yggr::utf8_string>(yggr_fmt, utf8_alloc_tmp.get_allocator()) << std::endl;

		std::wcout << tm.to_local_string<yggr::wstring>(yggr_fmt) << std::endl;
		std::wcout << tm.to_local_string<yggr::wstring>(yggr_fmt, yggr_alloc_tmp.get_allocator()) << std::endl;

		std::wcout << tm.to_local_string<std::wstring>(yggr_fmt) << std::endl;
		std::wcout << tm.to_local_string<std::wstring>(yggr_fmt, stl_alloc_tmp.get_allocator()) << std::endl;

		std::wcout << L"-----------------------" << std::endl;

		std::wcout << tm.to_local_string(stl_fmt) << std::endl;
		std::wcout << tm.to_local_string(stl_fmt, utf8_alloc_tmp.get_allocator()) << std::endl;

		std::wcout << tm.to_local_string<yggr::utf8_string>(stl_fmt) << std::endl;
		std::wcout << tm.to_local_string<yggr::utf8_string>(stl_fmt, utf8_alloc_tmp.get_allocator()) << std::endl;

		std::wcout << tm.to_local_string<yggr::wstring>(stl_fmt) << std::endl;
		std::wcout << tm.to_local_string<yggr::wstring>(stl_fmt, yggr_alloc_tmp.get_allocator()) << std::endl;

		std::wcout << tm.to_local_string<std::wstring>(stl_fmt) << std::endl;
		std::wcout << tm.to_local_string<std::wstring>(stl_fmt, stl_alloc_tmp.get_allocator()) << std::endl;

		std::wcout << L"-----------------------" << std::endl;

		std::wcout << tm.to_local_string(yggr_view_fmt) << std::endl;
		std::wcout << tm.to_local_string(yggr_view_fmt, utf8_alloc_tmp.get_allocator()) << std::endl;

		std::wcout << tm.to_local_string<yggr::utf8_string>(yggr_view_fmt) << std::endl;
		std::wcout << tm.to_local_string<yggr::utf8_string>(yggr_view_fmt, utf8_alloc_tmp.get_allocator()) << std::endl;

		std::wcout << tm.to_local_string<yggr::wstring>(yggr_view_fmt) << std::endl;
		std::wcout << tm.to_local_string<yggr::wstring>(yggr_view_fmt, yggr_alloc_tmp.get_allocator()) << std::endl;

		std::wcout << tm.to_local_string<std::wstring>(yggr_view_fmt) << std::endl;
		std::wcout << tm.to_local_string<std::wstring>(yggr_view_fmt, stl_alloc_tmp.get_allocator()) << std::endl;
	}

	{
		yggr::utf8_string utf8_alloc_tmp;
		yggr::wstring yggr_alloc_tmp;
		std::wstring stl_alloc_tmp;

		wchar_t str_fmt[] = L"%Y年%m月%d日 %H时:%M分:%S秒";
		yggr::utf8_string utf8_fmt(str_fmt);
		yggr::wstring yggr_fmt(str_fmt);
		std::wstring stl_fmt(str_fmt);
		yggr::wstring_view yggr_view_fmt(str_fmt);

		std::wcout << time_type::local_time_string() << std::endl;
		std::wcout << time_type::local_time_string(0) << std::endl;
		std::wcout << time_type::local_time_string(utf8_alloc_tmp.get_allocator()) << std::endl;

		std::wcout << L"-----------------------" << std::endl;

		std::wcout << time_type::local_time_string(str_fmt) << std::endl;
		std::wcout << time_type::local_time_string(str_fmt, utf8_alloc_tmp.get_allocator()) << std::endl;

		std::wcout << time_type::local_time_string<yggr::utf8_string>(str_fmt) << std::endl;
		std::wcout << time_type::local_time_string<yggr::utf8_string>(str_fmt, utf8_alloc_tmp.get_allocator()) << std::endl;
		std::wcout << time_type::local_time_string<yggr::utf8_string>() << std::endl;
		std::wcout << time_type::local_time_string<yggr::utf8_string>(utf8_alloc_tmp.get_allocator()) << std::endl;

		std::wcout << time_type::local_time_string<yggr::wstring>(str_fmt) << std::endl;
		std::wcout << time_type::local_time_string<yggr::wstring>(str_fmt, yggr_alloc_tmp.get_allocator()) << std::endl;
		std::wcout << time_type::local_time_string<yggr::wstring>() << std::endl;
		std::wcout << time_type::local_time_string<yggr::wstring>(yggr_alloc_tmp.get_allocator()) << std::endl;

		std::wcout << time_type::local_time_string<std::wstring>(str_fmt) << std::endl;
		std::wcout << time_type::local_time_string<std::wstring>(str_fmt, stl_alloc_tmp.get_allocator()) << std::endl;
		std::wcout << time_type::local_time_string<std::wstring>() << std::endl;
		std::wcout << time_type::local_time_string<std::wstring>(stl_alloc_tmp.get_allocator()) << std::endl;

		std::wcout << L"-----------------------" << std::endl;

		std::wcout << time_type::local_time_string(utf8_fmt) << std::endl;
		std::wcout << time_type::local_time_string(utf8_fmt, utf8_alloc_tmp.get_allocator()) << std::endl;

		std::wcout << time_type::local_time_string<yggr::utf8_string>(utf8_fmt) << std::endl;
		std::wcout << time_type::local_time_string<yggr::utf8_string>(utf8_fmt, utf8_alloc_tmp.get_allocator()) << std::endl;

		std::wcout << time_type::local_time_string<yggr::wstring>(utf8_fmt) << std::endl;
		std::wcout << time_type::local_time_string<yggr::wstring>(utf8_fmt, yggr_alloc_tmp.get_allocator()) << std::endl;

		std::wcout << time_type::local_time_string<std::wstring>(utf8_fmt) << std::endl;
		std::wcout << time_type::local_time_string<std::wstring>(utf8_fmt, stl_alloc_tmp.get_allocator()) << std::endl;

		std::wcout << L"-----------------------" << std::endl;

		std::wcout << time_type::local_time_string(yggr_fmt) << std::endl;
		std::wcout << time_type::local_time_string(yggr_fmt, utf8_alloc_tmp.get_allocator()) << std::endl;

		std::wcout << time_type::local_time_string<yggr::utf8_string>(yggr_fmt) << std::endl;
		std::wcout << time_type::local_time_string<yggr::utf8_string>(yggr_fmt, utf8_alloc_tmp.get_allocator()) << std::endl;

		std::wcout << time_type::local_time_string<yggr::wstring>(yggr_fmt) << std::endl;
		std::wcout << time_type::local_time_string<yggr::wstring>(yggr_fmt, yggr_alloc_tmp.get_allocator()) << std::endl;

		std::wcout << time_type::local_time_string<std::wstring>(yggr_fmt) << std::endl;
		std::wcout << time_type::local_time_string<std::wstring>(yggr_fmt, stl_alloc_tmp.get_allocator()) << std::endl;

		std::wcout << L"-----------------------" << std::endl;

		std::wcout << time_type::local_time_string(stl_fmt) << std::endl;
		std::wcout << time_type::local_time_string(stl_fmt, utf8_alloc_tmp.get_allocator()) << std::endl;

		std::wcout << time_type::local_time_string<yggr::utf8_string>(stl_fmt) << std::endl;
		std::wcout << time_type::local_time_string<yggr::utf8_string>(stl_fmt, utf8_alloc_tmp.get_allocator()) << std::endl;

		std::wcout << time_type::local_time_string<yggr::wstring>(stl_fmt) << std::endl;
		std::wcout << time_type::local_time_string<yggr::wstring>(stl_fmt, yggr_alloc_tmp.get_allocator()) << std::endl;

		std::wcout << time_type::local_time_string<std::wstring>(stl_fmt) << std::endl;
		std::wcout << time_type::local_time_string<std::wstring>(stl_fmt, stl_alloc_tmp.get_allocator()) << std::endl;

		std::wcout << L"-----------------------" << std::endl;

		std::wcout << time_type::local_time_string(yggr_view_fmt) << std::endl;
		std::wcout << time_type::local_time_string(yggr_view_fmt, utf8_alloc_tmp.get_allocator()) << std::endl;

		std::wcout << time_type::local_time_string<yggr::utf8_string>(yggr_view_fmt) << std::endl;
		std::wcout << time_type::local_time_string<yggr::utf8_string>(yggr_view_fmt, utf8_alloc_tmp.get_allocator()) << std::endl;

		std::wcout << time_type::local_time_string<yggr::wstring>(yggr_view_fmt) << std::endl;
		std::wcout << time_type::local_time_string<yggr::wstring>(yggr_view_fmt, yggr_alloc_tmp.get_allocator()) << std::endl;

		std::wcout << time_type::local_time_string<std::wstring>(yggr_view_fmt) << std::endl;
		std::wcout << time_type::local_time_string<std::wstring>(yggr_view_fmt, stl_alloc_tmp.get_allocator()) << std::endl;
	}

#endif // YGGR_NO_CWCHAR

	std::cout << "test4 succeed" << std::endl;
}

template<typename String, typename Char>
void test_string_time_formatting_combe(void)
{
	typedef String now_string_type;
	typedef Char now_char_type;
	typedef yggr::time::time time_type;

	yggr::time::time tm(0, 1000);

	yggr::charset::utf8_string chk_str_utc = "19700101000000";
	yggr::charset::utf8_string chk_str_local = "19700101080000";

	yggr::charset::utf8_string chk_str_iso8601_utc = "1970-01-01T00:00:00.000001";
	yggr::charset::utf8_string chk_str_iso8601_local = "1970-01-01T08:00:00.000001";

	// string version
	{
		// utc
		{
			now_string_type str;
			yggr::time::time_formatting_utc(str, 0, tm);
			std::cout << yggr::charset::utf8_string(str) << std::endl;
			yggr_test_assert(str == chk_str_utc);
		}

		{
			now_string_type str;
			yggr::time::time_formatting_utc(str, yggr::time::time_format::tm_fmt_default<now_char_type>(), tm);
			std::cout << yggr::charset::utf8_string(str) << std::endl;
			yggr_test_assert(str == chk_str_utc);
		}

		{
			now_string_type str;
			yggr::time::time_formatting_utc(str, yggr::time::time_format::tm_fmt_iso8601<now_char_type>(), tm);
			std::cout << yggr::charset::utf8_string(str) << std::endl;
			yggr_test_assert(str == chk_str_iso8601_utc);
		}

		{
			now_string_type str;
			yggr::time::time_formatting_utc(str, yggr::charset::utf8_string(yggr::time::time_format::tm_fmt_iso8601<now_char_type>()), tm);
			std::cout << yggr::charset::utf8_string(str) << std::endl;
			yggr_test_assert(str == chk_str_iso8601_utc);
		}

		{
			now_string_type str;
			yggr::time::time_formatting_utc(str, "%Y-%m-%dT%H:%M:%S%FZ", tm);
			std::cout << yggr::charset::utf8_string(str) << std::endl;
			yggr_test_assert(str == yggr::charset::utf8_string("1970-01-01T00:00:00.000001Z"));
		}

		// local
		{
			now_string_type str;
			yggr::time::time_formatting_local(str, 0, tm);
			std::cout << yggr::charset::utf8_string(str) << std::endl;
			yggr_test_assert(str == chk_str_local);
		}

		{
			now_string_type str;
			yggr::time::time_formatting_local(str, yggr::time::time_format::tm_fmt_default<now_char_type>(), tm);
			std::cout << yggr::charset::utf8_string(str) << std::endl;
			yggr_test_assert(str == chk_str_local);
		}

		{
			now_string_type str;
			yggr::time::time_formatting_local(str, yggr::time::time_format::tm_fmt_iso8601<now_char_type>(), tm);
			std::cout << yggr::charset::utf8_string(str) << std::endl;
			yggr_test_assert(str== chk_str_iso8601_local);
		}

		{
			now_string_type str;
			yggr::time::time_formatting_local(str, yggr::charset::utf8_string(yggr::time::time_format::tm_fmt_iso8601<now_char_type>()), tm);
			std::cout << yggr::charset::utf8_string(str) << std::endl;
			yggr_test_assert(str == chk_str_iso8601_local);
		}

		{
			now_string_type str;
			yggr::time::time_formatting_local(str, "%Y-%m-%dT%H:%M:%S%F%Z", tm);
			std::cout << yggr::charset::utf8_string(str) << std::endl;
#if defined(YGGR_AT_WINDOWS)
			yggr_test_assert(str == yggr::charset::utf8_string("1970-01-01T08:00:00.000001中国夏令时", YGGR_STR_UTF8_STRING_CHARSET_NAME()));
#elif defined(YGGR_AT_LINUX)
            yggr_test_assert(str == yggr::charset::utf8_string("1970-01-01T08:00:00.000001", YGGR_STR_UTF8_STRING_CHARSET_NAME()));
#elif defined(YGGR_AT_DARWIN)
            yggr_test_assert(str == yggr::charset::utf8_string("1970-01-01T08:00:00.000001", YGGR_STR_UTF8_STRING_CHARSET_NAME()));
#else
#   error "append other platfrom test code"
#endif // defined(YGGR_AT_WINDOWS));
		}
	}
}

void test5(void)
{
	typedef yggr::time::time time_type;

	yggr::time::time tm(0, 1000);

	yggr::charset::utf8_string chk_str_utc = "19700101000000";
	yggr::charset::utf8_string chk_str_local = "19700101080000";

	yggr::charset::utf8_string chk_str_iso8601_utc = "1970-01-01T00:00:00.000001";
	yggr::charset::utf8_string chk_str_iso8601_local = "1970-01-01T08:00:00.000001";

	// stream version
	{
		// utc
		{
			std::stringstream ss;
			yggr::time::time_formatting_utc(ss, 0, tm);
			std::cout << ss.str() << std::endl;
			yggr_test_assert(ss.str() == chk_str_utc);
		}

		{
			std::stringstream ss;
			yggr::time::time_formatting_utc(ss, yggr::time::time_format::tm_fmt_default<char>(), tm);
			std::cout << ss.str() << std::endl;
			yggr_test_assert(ss.str() == chk_str_utc);
		}

		{
			std::stringstream ss;
			yggr::time::time_formatting_utc(ss, yggr::time::time_format::tm_fmt_iso8601<char>(), tm);
			std::cout << ss.str() << std::endl;
			yggr_test_assert(ss.str() == chk_str_iso8601_utc);
		}

#if !defined(YGGR_NO_CWCHAR)
		{
			std::wstringstream ss;
			TEST_PP_WOS_IMBUE(ss, "chs");
			yggr::time::time_formatting_utc(ss, yggr::time::time_format::tm_fmt_iso8601<wchar_t>(), tm);
			std::wcout << ss.str() << std::endl;
			yggr_test_assert(ss.str() == chk_str_iso8601_utc);
		}
#endif // YGGR_NO_CWCHAR

		{
			std::stringstream ss;
			yggr::time::time_formatting_utc(ss, "%Y-%m-%dT%H:%M:%S%FZ", tm);
			std::cout << ss.str() << std::endl;
			yggr_test_assert(ss.str() == yggr::charset::utf8_string("1970-01-01T00:00:00.000001Z"));
		}

		// local
		{
			std::stringstream ss;
			yggr::time::time_formatting_local(ss, 0, tm);
			std::cout << ss.str() << std::endl;
			yggr_test_assert(ss.str() == chk_str_local);
		}

		{
			std::stringstream ss;
			yggr::time::time_formatting_local(ss, yggr::time::time_format::tm_fmt_default<char>(), tm);
			std::cout << ss.str() << std::endl;
			yggr_test_assert(ss.str() == chk_str_local);
		}



		{
			std::stringstream ss;
			yggr::time::time_formatting_local(ss, yggr::time::time_format::tm_fmt_iso8601<char>(), tm);
			std::cout << ss.str() << std::endl;
			yggr_test_assert(ss.str() == chk_str_iso8601_local);
		}

#if !defined(YGGR_NO_CWCHAR)
		{
			std::wstringstream ss;
			TEST_PP_WOS_IMBUE(ss, "chs");
			yggr::time::time_formatting_local(ss, yggr::time::time_format::tm_fmt_iso8601<wchar_t>(), tm);
			std::wcout << ss.str() << std::endl;
			yggr_test_assert(ss.str() == chk_str_iso8601_local);
		}
#endif // YGGR_NO_CWCHAR

		{
			std::stringstream ss;
			yggr::time::time_formatting_local(ss, "%Y-%m-%dT%H:%M:%S%F%Z", tm);
			std::cout << ss.str() << std::endl;
#if defined(YGGR_AT_WINDOWS)
			yggr_test_assert(ss.str() == yggr::charset::utf8_string("1970-01-01T08:00:00.000001中国夏令时", YGGR_STR_UTF8_STRING_CHARSET_NAME()));
#elif defined(YGGR_AT_LINUX)
            yggr_test_assert(ss.str() == yggr::charset::utf8_string("1970-01-01T08:00:00.000001", YGGR_STR_UTF8_STRING_CHARSET_NAME()));
#elif defined(YGGR_AT_DARWIN)
            yggr_test_assert(ss.str() == yggr::charset::utf8_string("1970-01-01T08:00:00.000001", YGGR_STR_UTF8_STRING_CHARSET_NAME()));
#else
#   error "append other platfrom test code"
#endif // defined(YGGR_AT_WINDOWS)
		}

#if !defined(YGGR_NO_CWCHAR)
		{
			std::wstringstream ss;
			TEST_PP_WOS_IMBUE(ss, "chs");
			//warning time zone %Z must app ' ' (e.g " %Z"), if no ' ' time_formatting_local end ss.str() is failed

#   if defined(_MSC_VER)
            yggr::time::time_formatting_local(ss, L"%Y-%m-%dT%H:%M:%S%F%Z", tm);
#   else
			yggr::time::time_formatting_local(ss, L"%Y-%m-%dT%H:%M:%S%F", tm);
#   endif // if !defined(_MSC_VER)
			std::wcout << ss.str() << std::endl;
#   if defined(YGGR_AT_WINDOWS) // the file not using _MSC_VER, so result is "1970-01-01T08:00:00.000001"
			yggr_test_assert(ss.str() == yggr::charset::utf8_string("1970-01-01T08:00:00.000001", YGGR_STR_UTF8_STRING_CHARSET_NAME()));
#   elif defined(YGGR_AT_LINUX)
            yggr_test_assert(ss.str() == yggr::charset::utf8_string("1970-01-01T08:00:00.000001", YGGR_STR_UTF8_STRING_CHARSET_NAME()));
#   elif defined(YGGR_AT_DARWIN)
            yggr_test_assert(ss.str() == yggr::charset::utf8_string("1970-01-01T08:00:00.000001", YGGR_STR_UTF8_STRING_CHARSET_NAME()));
#   else
#       error "append other platfrom test code"
#   endif // defined(YGGR_AT_WINDOWS)
		}
#endif // YGGR_NO_CWCHAR
	}

	std::cout << "------------" << std::endl;
	// string version
	test_string_time_formatting_combe<std::string, char>();
	test_string_time_formatting_combe<boost::container::string, char>();
	test_string_time_formatting_combe<yggr::string, char>();
	test_string_time_formatting_combe<yggr::utf8_string, char>();

#if !defined(YGGR_NO_CWCHAR)
	test_string_time_formatting_combe<std::string, wchar_t>();
	test_string_time_formatting_combe<boost::container::string, wchar_t>();
	test_string_time_formatting_combe<yggr::string, wchar_t>();
	test_string_time_formatting_combe<yggr::utf8_string, wchar_t>();
#endif // YGGR_NO_CWCHAR

#if !defined(YGGR_NO_CWCHAR)
	test_string_time_formatting_combe<std::wstring, char>();
	test_string_time_formatting_combe<boost::container::wstring, char>();
	test_string_time_formatting_combe<yggr::wstring, char>();
#endif // YGGR_NO_CWCHAR

}

void test6(void)
{
	yggr::time::time tm(0, 1000);

	yggr::utf8_string fmt_default = yggr::time::time_format::tm_fmt_default<char>();
	yggr::utf8_string fmt_iso8601 = yggr::time::time_format::tm_fmt_iso8601<char>();


	yggr::utf8_string chk_str_utc = "19700101000000";
	yggr::utf8_string chk_str_local = "19700101080000";

	yggr::charset::utf8_string chk_str_iso8601_utc = "1970-01-01T00:00:00.000001";
	yggr::charset::utf8_string chk_str_iso8601_local = "1970-01-01T08:00:00.000001";

	// utc
	// const char*
	yggr_test_assert(tm.to_string() == chk_str_utc);
	yggr_test_assert(tm.to_string(0) == chk_str_utc);
	yggr_test_assert(tm.to_string(yggr::time::time_format::tm_fmt_default<char>()) == chk_str_utc);
	yggr_test_assert(tm.to_string(yggr::time::time_format::tm_fmt_iso8601<char>()) == chk_str_iso8601_utc);

	// const char*,  alloc
	yggr_test_assert(tm.to_string(chk_str_utc.get_allocator()) == chk_str_utc);
	yggr_test_assert(tm.to_string(0, chk_str_utc.get_allocator()) == chk_str_utc);
	yggr_test_assert(tm.to_string(yggr::time::time_format::tm_fmt_default<char>(), chk_str_utc.get_allocator()) == chk_str_utc);
	yggr_test_assert(tm.to_string(yggr::time::time_format::tm_fmt_iso8601<char>(), chk_str_utc.get_allocator()) == chk_str_iso8601_utc);

#if !defined(YGGR_NO_CWCHAR)
	// const wchar_t*
	yggr_test_assert(tm.to_string(yggr::time::time_format::tm_fmt_default<wchar_t>()) == chk_str_utc);
	yggr_test_assert(tm.to_string(yggr::time::time_format::tm_fmt_iso8601<wchar_t>()) == chk_str_iso8601_utc);

	// const wchar_t*,  alloc
	yggr_test_assert(tm.to_string(yggr::time::time_format::tm_fmt_default<wchar_t>(), chk_str_utc.get_allocator()) == chk_str_utc);
	yggr_test_assert(tm.to_string(yggr::time::time_format::tm_fmt_iso8601<wchar_t>(), chk_str_utc.get_allocator()) == chk_str_iso8601_utc);
#endif // YGGR_NO_CWCHAR

	// std::string
	yggr_test_assert(tm.to_string(fmt_default.str<std::string>()) == chk_str_utc);
	yggr_test_assert(tm.to_string(fmt_iso8601.str<std::string>()) == chk_str_iso8601_utc);

	// std::string,  alloc
	yggr_test_assert(tm.to_string(fmt_default.str<std::string>(), chk_str_utc.get_allocator()) == chk_str_utc);
	yggr_test_assert(tm.to_string(fmt_iso8601.str<std::string>(), chk_str_utc.get_allocator()) == chk_str_iso8601_utc);

	// boost::container::string
	yggr_test_assert(tm.to_string(fmt_default.str<boost::container::string>()) == chk_str_utc);
	yggr_test_assert(tm.to_string(fmt_iso8601.str<boost::container::string>()) == chk_str_iso8601_utc);

	// boost::container::string,  alloc
	yggr_test_assert(tm.to_string(fmt_default.str<boost::container::string>(), chk_str_utc.get_allocator()) == chk_str_utc);
	yggr_test_assert(tm.to_string(fmt_iso8601.str<boost::container::string>(), chk_str_utc.get_allocator()) == chk_str_iso8601_utc);

	// yggr::string
	yggr_test_assert(tm.to_string(fmt_default.str<yggr::string>()) == chk_str_utc);
	yggr_test_assert(tm.to_string(fmt_iso8601.str<yggr::string>()) == chk_str_iso8601_utc);

	// yggr::string,  alloc
	yggr_test_assert(tm.to_string(fmt_default.str<yggr::string>(), chk_str_utc.get_allocator()) == chk_str_utc);
	yggr_test_assert(tm.to_string(fmt_iso8601.str<yggr::string>(), chk_str_utc.get_allocator()) == chk_str_iso8601_utc);

	// yggr::utf8_string
	yggr_test_assert(tm.to_string(fmt_default) == chk_str_utc);
	yggr_test_assert(tm.to_string(fmt_iso8601) == chk_str_iso8601_utc);

	// yggr::utf8_string,  alloc
	yggr_test_assert(tm.to_string(fmt_default, chk_str_utc.get_allocator()) == chk_str_utc);
	yggr_test_assert(tm.to_string(fmt_iso8601, chk_str_utc.get_allocator()) == chk_str_iso8601_utc);

	// yggr::string_charset_helper
	yggr_test_assert(tm.to_string(yggr::charset::make_string_charset_helper(fmt_default)) == chk_str_utc);
	yggr_test_assert(tm.to_string(fmt_iso8601) == chk_str_iso8601_utc);

	// yggr::utf8_string,  alloc
	yggr_test_assert(tm.to_string(fmt_default, chk_str_utc.get_allocator()) == chk_str_utc);
	yggr_test_assert(tm.to_string(fmt_iso8601, chk_str_utc.get_allocator()) == chk_str_iso8601_utc);

#if !defined(YGGR_NO_CWCHAR)
	// std::wstring
	yggr_test_assert(tm.to_string(fmt_default.str<std::wstring>()) == chk_str_utc);
	yggr_test_assert(tm.to_string(fmt_iso8601.str<std::wstring>()) == chk_str_iso8601_utc);

	// std::wstring,  alloc
	yggr_test_assert(tm.to_string(fmt_default.str<std::wstring>(), chk_str_utc.get_allocator()) == chk_str_utc);
	yggr_test_assert(tm.to_string(fmt_iso8601.str<std::wstring>(), chk_str_utc.get_allocator()) == chk_str_iso8601_utc);

	// boost::container::wstring
	yggr_test_assert(tm.to_string(fmt_default.str<boost::container::wstring>()) == chk_str_utc);
	yggr_test_assert(tm.to_string(fmt_iso8601.str<boost::container::wstring>()) == chk_str_iso8601_utc);

	// boost::container::wstring,  alloc
	yggr_test_assert(tm.to_string(fmt_default.str<boost::container::wstring>(), chk_str_utc.get_allocator()) == chk_str_utc);
	yggr_test_assert(tm.to_string(fmt_iso8601.str<boost::container::wstring>(), chk_str_utc.get_allocator()) == chk_str_iso8601_utc);

	// yggr::wstring
	yggr_test_assert(tm.to_string(fmt_default.str<yggr::wstring>()) == chk_str_utc);
	yggr_test_assert(tm.to_string(fmt_iso8601.str<yggr::wstring>()) == chk_str_iso8601_utc);

	// yggr::wstring,  alloc
	yggr_test_assert(tm.to_string(fmt_default.str<yggr::wstring>(), chk_str_utc.get_allocator()) == chk_str_utc);
	yggr_test_assert(tm.to_string(fmt_iso8601.str<yggr::wstring>(), chk_str_utc.get_allocator()) == chk_str_iso8601_utc);

#endif // YGGR_NO_CWCHAR

	// return wstring
#if !defined(YGGR_NO_CWCHAR)
	// const char*
	yggr_test_assert(tm.to_string<std::wstring>() == chk_str_utc);
	yggr_test_assert(tm.to_string<std::wstring>(0) == chk_str_utc);
	yggr_test_assert(tm.to_string<std::wstring>(yggr::time::time_format::tm_fmt_default<char>()) == chk_str_utc);
	yggr_test_assert(tm.to_string<std::wstring>(yggr::time::time_format::tm_fmt_iso8601<char>()) == chk_str_iso8601_utc);

	// const char*,  alloc
	yggr_test_assert(tm.to_string<std::wstring>(chk_str_utc.str<std::wstring>().get_allocator()) == chk_str_utc);
	yggr_test_assert(tm.to_string<std::wstring>(0, chk_str_utc.str<std::wstring>().get_allocator()) == chk_str_utc);
	yggr_test_assert(tm.to_string<std::wstring>(yggr::time::time_format::tm_fmt_default<char>(), chk_str_utc.str<std::wstring>().get_allocator()) == chk_str_utc);
	yggr_test_assert(tm.to_string<std::wstring>(yggr::time::time_format::tm_fmt_iso8601<char>(), chk_str_utc.str<std::wstring>().get_allocator()) == chk_str_iso8601_utc);

	// const wchar_t*
	yggr_test_assert(tm.to_string<std::wstring>(yggr::time::time_format::tm_fmt_default<wchar_t>()) == chk_str_utc);
	yggr_test_assert(tm.to_string<std::wstring>(yggr::time::time_format::tm_fmt_iso8601<wchar_t>()) == chk_str_iso8601_utc);

	// const wchar_t*,  alloc
	yggr_test_assert(tm.to_string<std::wstring>(yggr::time::time_format::tm_fmt_default<wchar_t>(), chk_str_utc.str<std::wstring>().get_allocator()) == chk_str_utc);
	yggr_test_assert(tm.to_string<std::wstring>(yggr::time::time_format::tm_fmt_iso8601<wchar_t>(), chk_str_utc.str<std::wstring>().get_allocator()) == chk_str_iso8601_utc);

	// std::string
	yggr_test_assert(tm.to_string<std::wstring>(fmt_default.str<std::string>()) == chk_str_utc);
	yggr_test_assert(tm.to_string<std::wstring>(fmt_iso8601.str<std::string>()) == chk_str_iso8601_utc);

	// std::string,  alloc
	yggr_test_assert(tm.to_string<std::wstring>(fmt_default.str<std::string>(), chk_str_utc.str<std::wstring>().get_allocator()) == chk_str_utc);
	yggr_test_assert(tm.to_string<std::wstring>(fmt_iso8601.str<std::string>(), chk_str_utc.str<std::wstring>().get_allocator()) == chk_str_iso8601_utc);

	// boost::container::string
	yggr_test_assert(tm.to_string<std::wstring>(fmt_default.str<boost::container::string>()) == chk_str_utc);
	yggr_test_assert(tm.to_string<std::wstring>(fmt_iso8601.str<boost::container::string>()) == chk_str_iso8601_utc);

	// boost::container::string,  alloc
	yggr_test_assert(tm.to_string<std::wstring>(fmt_default.str<boost::container::string>(), chk_str_utc.str<std::wstring>().get_allocator()) == chk_str_utc);
	yggr_test_assert(tm.to_string<std::wstring>(fmt_iso8601.str<boost::container::string>(), chk_str_utc.str<std::wstring>().get_allocator()) == chk_str_iso8601_utc);

	// yggr::string
	yggr_test_assert(tm.to_string<std::wstring>(fmt_default.str<yggr::string>()) == chk_str_utc);
	yggr_test_assert(tm.to_string<std::wstring>(fmt_iso8601.str<yggr::string>()) == chk_str_iso8601_utc);

	// yggr::string,  alloc
	yggr_test_assert(tm.to_string<std::wstring>(fmt_default.str<yggr::string>(), chk_str_utc.str<std::wstring>().get_allocator()) == chk_str_utc);
	yggr_test_assert(tm.to_string<std::wstring>(fmt_iso8601.str<yggr::string>(), chk_str_utc.str<std::wstring>().get_allocator()) == chk_str_iso8601_utc);

	// yggr::utf8_string
	yggr_test_assert(tm.to_string<std::wstring>(fmt_default) == chk_str_utc);
	yggr_test_assert(tm.to_string<std::wstring>(fmt_iso8601) == chk_str_iso8601_utc);

	// yggr::utf8_string,  alloc
	yggr_test_assert(tm.to_string<std::wstring>(fmt_default, chk_str_utc.str<std::wstring>().get_allocator()) == chk_str_utc);
	yggr_test_assert(tm.to_string<std::wstring>(fmt_iso8601, chk_str_utc.str<std::wstring>().get_allocator()) == chk_str_iso8601_utc);

	// yggr::string_charset_helper
	yggr_test_assert(tm.to_string<std::wstring>(yggr::charset::make_string_charset_helper(fmt_default)) == chk_str_utc);
	yggr_test_assert(tm.to_string<std::wstring>(fmt_iso8601) == chk_str_iso8601_utc);

	// yggr::utf8_string,  alloc
	yggr_test_assert(tm.to_string<std::wstring>(fmt_default, chk_str_utc.str<std::wstring>().get_allocator()) == chk_str_utc);
	yggr_test_assert(tm.to_string<std::wstring>(fmt_iso8601, chk_str_utc.str<std::wstring>().get_allocator()) == chk_str_iso8601_utc);

	// std::wstring
	yggr_test_assert(tm.to_string<std::wstring>(fmt_default.str<std::wstring>()) == chk_str_utc);
	yggr_test_assert(tm.to_string<std::wstring>(fmt_iso8601.str<std::wstring>()) == chk_str_iso8601_utc);

	// std::wstring,  alloc
	yggr_test_assert(tm.to_string<std::wstring>(fmt_default.str<std::wstring>(), chk_str_utc.str<std::wstring>().get_allocator()) == chk_str_utc);
	yggr_test_assert(tm.to_string<std::wstring>(fmt_iso8601.str<std::wstring>(), chk_str_utc.str<std::wstring>().get_allocator()) == chk_str_iso8601_utc);

	// boost::container::wstring
	yggr_test_assert(tm.to_string<std::wstring>(fmt_default.str<boost::container::wstring>()) == chk_str_utc);
	yggr_test_assert(tm.to_string<std::wstring>(fmt_iso8601.str<boost::container::wstring>()) == chk_str_iso8601_utc);

	// boost::container::wstring,  alloc
	yggr_test_assert(tm.to_string<std::wstring>(fmt_default.str<boost::container::wstring>(), chk_str_utc.str<std::wstring>().get_allocator()) == chk_str_utc);
	yggr_test_assert(tm.to_string<std::wstring>(fmt_iso8601.str<boost::container::wstring>(), chk_str_utc.str<std::wstring>().get_allocator()) == chk_str_iso8601_utc);

	// yggr::wstring
	yggr_test_assert(tm.to_string<std::wstring>(fmt_default.str<yggr::wstring>()) == chk_str_utc);
	yggr_test_assert(tm.to_string<std::wstring>(fmt_iso8601.str<yggr::wstring>()) == chk_str_iso8601_utc);

	// yggr::wstring,  alloc
	yggr_test_assert(tm.to_string<std::wstring>(fmt_default.str<yggr::wstring>(), chk_str_utc.str<std::wstring>().get_allocator()) == chk_str_utc);
	yggr_test_assert(tm.to_string<std::wstring>(fmt_iso8601.str<yggr::wstring>(), chk_str_utc.str<std::wstring>().get_allocator()) == chk_str_iso8601_utc);

#endif // YGGR_NO_CWCHAR

	// local
	// const char*
	yggr_test_assert(tm.to_local_string() == chk_str_local);
	yggr_test_assert(tm.to_local_string(0) == chk_str_local);
	yggr_test_assert(tm.to_local_string(yggr::time::time_format::tm_fmt_default<char>()) == chk_str_local);
	yggr_test_assert(tm.to_local_string(yggr::time::time_format::tm_fmt_iso8601<char>()) == chk_str_iso8601_local);

	// const char*,  alloc
	yggr_test_assert(tm.to_local_string(chk_str_local.get_allocator()) == chk_str_local);
	yggr_test_assert(tm.to_local_string(0, chk_str_local.get_allocator()) == chk_str_local);
	yggr_test_assert(tm.to_local_string(yggr::time::time_format::tm_fmt_default<char>(), chk_str_local.get_allocator()) == chk_str_local);
	yggr_test_assert(tm.to_local_string(yggr::time::time_format::tm_fmt_iso8601<char>(), chk_str_local.get_allocator()) == chk_str_iso8601_local);

#if !defined(YGGR_NO_CWCHAR)
	// const wchar_t*
	yggr_test_assert(tm.to_local_string(yggr::time::time_format::tm_fmt_default<wchar_t>()) == chk_str_local);
	yggr_test_assert(tm.to_local_string(yggr::time::time_format::tm_fmt_iso8601<wchar_t>()) == chk_str_iso8601_local);

	// const wchar_t*,  alloc
	yggr_test_assert(tm.to_local_string(yggr::time::time_format::tm_fmt_default<wchar_t>(), chk_str_local.get_allocator()) == chk_str_local);
	yggr_test_assert(tm.to_local_string(yggr::time::time_format::tm_fmt_iso8601<wchar_t>(), chk_str_local.get_allocator()) == chk_str_iso8601_local);
#endif // YGGR_NO_CWCHAR

	// std::string
	yggr_test_assert(tm.to_local_string(fmt_default.str<std::string>()) == chk_str_local);
	yggr_test_assert(tm.to_local_string(fmt_iso8601.str<std::string>()) == chk_str_iso8601_local);

	// std::string,  alloc
	yggr_test_assert(tm.to_local_string(fmt_default.str<std::string>(), chk_str_local.get_allocator()) == chk_str_local);
	yggr_test_assert(tm.to_local_string(fmt_iso8601.str<std::string>(), chk_str_local.get_allocator()) == chk_str_iso8601_local);

	// boost::container::string
	yggr_test_assert(tm.to_local_string(fmt_default.str<boost::container::string>()) == chk_str_local);
	yggr_test_assert(tm.to_local_string(fmt_iso8601.str<boost::container::string>()) == chk_str_iso8601_local);

	// boost::container::string,  alloc
	yggr_test_assert(tm.to_local_string(fmt_default.str<boost::container::string>(), chk_str_local.get_allocator()) == chk_str_local);
	yggr_test_assert(tm.to_local_string(fmt_iso8601.str<boost::container::string>(), chk_str_local.get_allocator()) == chk_str_iso8601_local);

	// yggr::string
	yggr_test_assert(tm.to_local_string(fmt_default.str<yggr::string>()) == chk_str_local);
	yggr_test_assert(tm.to_local_string(fmt_iso8601.str<yggr::string>()) == chk_str_iso8601_local);

	// yggr::string,  alloc
	yggr_test_assert(tm.to_local_string(fmt_default.str<yggr::string>(), chk_str_local.get_allocator()) == chk_str_local);
	yggr_test_assert(tm.to_local_string(fmt_iso8601.str<yggr::string>(), chk_str_local.get_allocator()) == chk_str_iso8601_local);

	// yggr::utf8_string
	yggr_test_assert(tm.to_local_string(fmt_default) == chk_str_local);
	yggr_test_assert(tm.to_local_string(fmt_iso8601) == chk_str_iso8601_local);

	// yggr::utf8_string,  alloc
	yggr_test_assert(tm.to_local_string(fmt_default, chk_str_local.get_allocator()) == chk_str_local);
	yggr_test_assert(tm.to_local_string(fmt_iso8601, chk_str_local.get_allocator()) == chk_str_iso8601_local);

	// yggr::string_charset_helper
	yggr_test_assert(tm.to_local_string(yggr::charset::make_string_charset_helper(fmt_default)) == chk_str_local);
	yggr_test_assert(tm.to_local_string(fmt_iso8601) == chk_str_iso8601_local);

	// yggr::utf8_string,  alloc
	yggr_test_assert(tm.to_local_string(fmt_default, chk_str_local.get_allocator()) == chk_str_local);
	yggr_test_assert(tm.to_local_string(fmt_iso8601, chk_str_local.get_allocator()) == chk_str_iso8601_local);

#if !defined(YGGR_NO_CWCHAR)
	// std::wstring
	yggr_test_assert(tm.to_local_string(fmt_default.str<std::wstring>()) == chk_str_local);
	yggr_test_assert(tm.to_local_string(fmt_iso8601.str<std::wstring>()) == chk_str_iso8601_local);

	// std::wstring,  alloc
	yggr_test_assert(tm.to_local_string(fmt_default.str<std::wstring>(), chk_str_local.get_allocator()) == chk_str_local);
	yggr_test_assert(tm.to_local_string(fmt_iso8601.str<std::wstring>(), chk_str_local.get_allocator()) == chk_str_iso8601_local);

	// boost::container::wstring
	yggr_test_assert(tm.to_local_string(fmt_default.str<boost::container::wstring>()) == chk_str_local);
	yggr_test_assert(tm.to_local_string(fmt_iso8601.str<boost::container::wstring>()) == chk_str_iso8601_local);

	// boost::container::wstring,  alloc
	yggr_test_assert(tm.to_local_string(fmt_default.str<boost::container::wstring>(), chk_str_local.get_allocator()) == chk_str_local);
	yggr_test_assert(tm.to_local_string(fmt_iso8601.str<boost::container::wstring>(), chk_str_local.get_allocator()) == chk_str_iso8601_local);

	// yggr::wstring
	yggr_test_assert(tm.to_local_string(fmt_default.str<yggr::wstring>()) == chk_str_local);
	yggr_test_assert(tm.to_local_string(fmt_iso8601.str<yggr::wstring>()) == chk_str_iso8601_local);

	// yggr::wstring,  alloc
	yggr_test_assert(tm.to_local_string(fmt_default.str<yggr::wstring>(), chk_str_local.get_allocator()) == chk_str_local);
	yggr_test_assert(tm.to_local_string(fmt_iso8601.str<yggr::wstring>(), chk_str_local.get_allocator()) == chk_str_iso8601_local);

#endif // YGGR_NO_CWCHAR

	// return wstring
#if !defined(YGGR_NO_CWCHAR)
	// const char*
	yggr_test_assert(tm.to_local_string<std::wstring>() == chk_str_local);
	yggr_test_assert(tm.to_local_string<std::wstring>(0) == chk_str_local);
	yggr_test_assert(tm.to_local_string<std::wstring>(yggr::time::time_format::tm_fmt_default<char>()) == chk_str_local);
	yggr_test_assert(tm.to_local_string<std::wstring>(yggr::time::time_format::tm_fmt_iso8601<char>()) == chk_str_iso8601_local);

	// const char*,  alloc
	yggr_test_assert(tm.to_local_string<std::wstring>(chk_str_local.str<std::wstring>().get_allocator()) == chk_str_local);
	yggr_test_assert(tm.to_local_string<std::wstring>(0, chk_str_local.str<std::wstring>().get_allocator()) == chk_str_local);
	yggr_test_assert(tm.to_local_string<std::wstring>(yggr::time::time_format::tm_fmt_default<char>(), chk_str_local.str<std::wstring>().get_allocator()) == chk_str_local);
	yggr_test_assert(tm.to_local_string<std::wstring>(yggr::time::time_format::tm_fmt_iso8601<char>(), chk_str_local.str<std::wstring>().get_allocator()) == chk_str_iso8601_local);

	// const wchar_t*
	yggr_test_assert(tm.to_local_string<std::wstring>(yggr::time::time_format::tm_fmt_default<wchar_t>()) == chk_str_local);
	yggr_test_assert(tm.to_local_string<std::wstring>(yggr::time::time_format::tm_fmt_iso8601<wchar_t>()) == chk_str_iso8601_local);

	// const wchar_t*,  alloc
	yggr_test_assert(tm.to_local_string<std::wstring>(yggr::time::time_format::tm_fmt_default<wchar_t>(), chk_str_local.str<std::wstring>().get_allocator()) == chk_str_local);
	yggr_test_assert(tm.to_local_string<std::wstring>(yggr::time::time_format::tm_fmt_iso8601<wchar_t>(), chk_str_local.str<std::wstring>().get_allocator()) == chk_str_iso8601_local);

	// std::string
	yggr_test_assert(tm.to_local_string<std::wstring>(fmt_default.str<std::string>()) == chk_str_local);
	yggr_test_assert(tm.to_local_string<std::wstring>(fmt_iso8601.str<std::string>()) == chk_str_iso8601_local);

	// std::string,  alloc
	yggr_test_assert(tm.to_local_string<std::wstring>(fmt_default.str<std::string>(), chk_str_local.str<std::wstring>().get_allocator()) == chk_str_local);
	yggr_test_assert(tm.to_local_string<std::wstring>(fmt_iso8601.str<std::string>(), chk_str_local.str<std::wstring>().get_allocator()) == chk_str_iso8601_local);

	// boost::container::string
	yggr_test_assert(tm.to_local_string<std::wstring>(fmt_default.str<boost::container::string>()) == chk_str_local);
	yggr_test_assert(tm.to_local_string<std::wstring>(fmt_iso8601.str<boost::container::string>()) == chk_str_iso8601_local);

	// boost::container::string,  alloc
	yggr_test_assert(tm.to_local_string<std::wstring>(fmt_default.str<boost::container::string>(), chk_str_local.str<std::wstring>().get_allocator()) == chk_str_local);
	yggr_test_assert(tm.to_local_string<std::wstring>(fmt_iso8601.str<boost::container::string>(), chk_str_local.str<std::wstring>().get_allocator()) == chk_str_iso8601_local);

	// yggr::string
	yggr_test_assert(tm.to_local_string<std::wstring>(fmt_default.str<yggr::string>()) == chk_str_local);
	yggr_test_assert(tm.to_local_string<std::wstring>(fmt_iso8601.str<yggr::string>()) == chk_str_iso8601_local);

	// yggr::string,  alloc
	yggr_test_assert(tm.to_local_string<std::wstring>(fmt_default.str<yggr::string>(), chk_str_local.str<std::wstring>().get_allocator()) == chk_str_local);
	yggr_test_assert(tm.to_local_string<std::wstring>(fmt_iso8601.str<yggr::string>(), chk_str_local.str<std::wstring>().get_allocator()) == chk_str_iso8601_local);

	// yggr::utf8_string
	yggr_test_assert(tm.to_local_string<std::wstring>(fmt_default) == chk_str_local);
	yggr_test_assert(tm.to_local_string<std::wstring>(fmt_iso8601) == chk_str_iso8601_local);

	// yggr::utf8_string,  alloc
	yggr_test_assert(tm.to_local_string<std::wstring>(fmt_default, chk_str_local.str<std::wstring>().get_allocator()) == chk_str_local);
	yggr_test_assert(tm.to_local_string<std::wstring>(fmt_iso8601, chk_str_local.str<std::wstring>().get_allocator()) == chk_str_iso8601_local);

	// yggr::string_charset_helper
	yggr_test_assert(tm.to_local_string<std::wstring>(yggr::charset::make_string_charset_helper(fmt_default)) == chk_str_local);
	yggr_test_assert(tm.to_local_string<std::wstring>(fmt_iso8601) == chk_str_iso8601_local);

	// yggr::utf8_string,  alloc
	yggr_test_assert(tm.to_local_string<std::wstring>(fmt_default, chk_str_local.str<std::wstring>().get_allocator()) == chk_str_local);
	yggr_test_assert(tm.to_local_string<std::wstring>(fmt_iso8601, chk_str_local.str<std::wstring>().get_allocator()) == chk_str_iso8601_local);

	// std::wstring
	yggr_test_assert(tm.to_local_string<std::wstring>(fmt_default.str<std::wstring>()) == chk_str_local);
	yggr_test_assert(tm.to_local_string<std::wstring>(fmt_iso8601.str<std::wstring>()) == chk_str_iso8601_local);

	// std::wstring,  alloc
	yggr_test_assert(tm.to_local_string<std::wstring>(fmt_default.str<std::wstring>(), chk_str_local.str<std::wstring>().get_allocator()) == chk_str_local);
	yggr_test_assert(tm.to_local_string<std::wstring>(fmt_iso8601.str<std::wstring>(), chk_str_local.str<std::wstring>().get_allocator()) == chk_str_iso8601_local);

	// boost::container::wstring
	yggr_test_assert(tm.to_local_string<std::wstring>(fmt_default.str<boost::container::wstring>()) == chk_str_local);
	yggr_test_assert(tm.to_local_string<std::wstring>(fmt_iso8601.str<boost::container::wstring>()) == chk_str_iso8601_local);

	// boost::container::wstring,  alloc
	yggr_test_assert(tm.to_local_string<std::wstring>(fmt_default.str<boost::container::wstring>(), chk_str_local.str<std::wstring>().get_allocator()) == chk_str_local);
	yggr_test_assert(tm.to_local_string<std::wstring>(fmt_iso8601.str<boost::container::wstring>(), chk_str_local.str<std::wstring>().get_allocator()) == chk_str_iso8601_local);

	// yggr::wstring
	yggr_test_assert(tm.to_local_string<std::wstring>(fmt_default.str<yggr::wstring>()) == chk_str_local);
	yggr_test_assert(tm.to_local_string<std::wstring>(fmt_iso8601.str<yggr::wstring>()) == chk_str_iso8601_local);

	// yggr::wstring,  alloc
	yggr_test_assert(tm.to_local_string<std::wstring>(fmt_default.str<yggr::wstring>(), chk_str_local.str<std::wstring>().get_allocator()) == chk_str_local);
	yggr_test_assert(tm.to_local_string<std::wstring>(fmt_iso8601.str<yggr::wstring>(), chk_str_local.str<std::wstring>().get_allocator()) == chk_str_iso8601_local);

#endif // YGGR_NO_CWCHAR

}

void test7(void)
{
	yggr::utf8_string fmt_default = yggr::time::time_format::tm_fmt_default<char>();
	yggr::utf8_string fmt_iso8601 = yggr::time::time_format::tm_fmt_iso8601<char>();

	// utc
	// const char*
	std::cout << yggr::time::time::time_string() << std::endl;
	std::cout << yggr::time::time::time_string(0) << std::endl;
	std::cout << yggr::time::time::time_string(yggr::time::time_format::tm_fmt_default<char>()) << std::endl;
	std::cout << yggr::time::time::time_string(yggr::time::time_format::tm_fmt_iso8601<char>()) << std::endl;


	// const char*,  alloc
	std::cout << yggr::time::time::time_string(fmt_default.get_allocator()) << std::endl;
	std::cout << yggr::time::time::time_string(0, fmt_default.get_allocator()) << std::endl;
	std::cout << yggr::time::time::time_string(yggr::time::time_format::tm_fmt_default<char>(), fmt_default.get_allocator()) << std::endl;
	std::cout << yggr::time::time::time_string(yggr::time::time_format::tm_fmt_iso8601<char>(), fmt_iso8601.get_allocator()) << std::endl;

#if !defined(YGGR_NO_CWCHAR)
	// const wchar_t*
	std::cout << yggr::time::time::time_string(yggr::time::time_format::tm_fmt_default<wchar_t>()) << std::endl;
	std::cout << yggr::time::time::time_string(yggr::time::time_format::tm_fmt_iso8601<wchar_t>()) << std::endl;

	// const wchar_t*,  alloc
	std::cout << yggr::time::time::time_string(yggr::time::time_format::tm_fmt_default<wchar_t>(), fmt_default.get_allocator()) << std::endl;
	std::cout << yggr::time::time::time_string(yggr::time::time_format::tm_fmt_iso8601<wchar_t>(), fmt_iso8601.get_allocator()) << std::endl;
#endif // YGGR_NO_CWCHAR

	// std::string
	std::cout << yggr::time::time::time_string(fmt_default.str<std::string>()) << std::endl;
	std::cout << yggr::time::time::time_string(fmt_iso8601.str<std::string>()) << std::endl;

		// std::string,  alloc
	std::cout << yggr::time::time::time_string(fmt_default.str<std::string>(), fmt_default.get_allocator()) << std::endl;
	std::cout << yggr::time::time::time_string(fmt_iso8601.str<std::string>(), fmt_iso8601.get_allocator()) << std::endl;

	// boost::container::string
	std::cout << yggr::time::time::time_string(fmt_default.str<boost::container::string>()) << std::endl;
	std::cout << yggr::time::time::time_string(fmt_iso8601.str<boost::container::string>()) << std::endl;

	// boost::container::string,  alloc
	std::cout << yggr::time::time::time_string(fmt_default.str<boost::container::string>(), fmt_default.get_allocator()) << std::endl;
	std::cout << yggr::time::time::time_string(fmt_iso8601.str<boost::container::string>(), fmt_iso8601.get_allocator()) << std::endl;

	// yggr::string
	std::cout << yggr::time::time::time_string(fmt_default.str<yggr::string>()) << std::endl;
	std::cout << yggr::time::time::time_string(fmt_iso8601.str<yggr::string>()) << std::endl;

	// yggr::string,  alloc
	std::cout << yggr::time::time::time_string(fmt_default.str<yggr::string>(), fmt_default.get_allocator()) << std::endl;
	std::cout << yggr::time::time::time_string(fmt_iso8601.str<yggr::string>(), fmt_iso8601.get_allocator()) << std::endl;

	// yggr::utf8_string
	std::cout << yggr::time::time::time_string(fmt_default) << std::endl;
	std::cout << yggr::time::time::time_string(fmt_iso8601) << std::endl;

	// yggr::utf8_string,  alloc
	std::cout << yggr::time::time::time_string(fmt_default, fmt_default.get_allocator()) << std::endl;
	std::cout << yggr::time::time::time_string(fmt_iso8601, fmt_iso8601.get_allocator()) << std::endl;

	// yggr::string_charset_helper
	std::cout << yggr::time::time::time_string(yggr::charset::make_string_charset_helper(fmt_default)) << std::endl;
	std::cout << yggr::time::time::time_string(fmt_iso8601) << std::endl;

	// yggr::utf8_string,  alloc
	std::cout << yggr::time::time::time_string(fmt_default, fmt_default.get_allocator()) << std::endl;
	std::cout << yggr::time::time::time_string(fmt_iso8601, fmt_iso8601.get_allocator()) << std::endl;

#if !defined(YGGR_NO_CWCHAR)
	// std::wstring
	std::cout << yggr::time::time::time_string(fmt_default.str<std::wstring>()) << std::endl;
	std::cout << yggr::time::time::time_string(fmt_iso8601.str<std::wstring>()) << std::endl;

	// std::wstring,  alloc
	std::cout << yggr::time::time::time_string(fmt_default.str<std::wstring>(), fmt_default.get_allocator()) << std::endl;
	std::cout << yggr::time::time::time_string(fmt_iso8601.str<std::wstring>(), fmt_iso8601.get_allocator()) << std::endl;

	// boost::container::wstring
	std::cout << yggr::time::time::time_string(fmt_default.str<boost::container::wstring>()) << std::endl;
	std::cout << yggr::time::time::time_string(fmt_iso8601.str<boost::container::wstring>()) << std::endl;

	// boost::container::wstring,  alloc
	std::cout << yggr::time::time::time_string(fmt_default.str<boost::container::wstring>(), fmt_default.get_allocator()) << std::endl;
	std::cout << yggr::time::time::time_string(fmt_iso8601.str<boost::container::wstring>(), fmt_iso8601.get_allocator()) << std::endl;

	// yggr::wstring
	std::cout << yggr::time::time::time_string(fmt_default.str<yggr::wstring>()) << std::endl;
	std::cout << yggr::time::time::time_string(fmt_iso8601.str<yggr::wstring>()) << std::endl;

	// yggr::wstring,  alloc
	std::cout << yggr::time::time::time_string(fmt_default.str<yggr::wstring>(), fmt_default.get_allocator()) << std::endl;
	std::cout << yggr::time::time::time_string(fmt_iso8601.str<yggr::wstring>(), fmt_iso8601.get_allocator()) << std::endl;

#endif // YGGR_NO_CWCHAR

#if !defined(YGGR_NO_CWCHAR)
	// const char*
	std::wcout << yggr::time::time::time_string<std::wstring>() << std::endl;
	std::wcout << yggr::time::time::time_string<std::wstring>(0) << std::endl;
	std::wcout << yggr::time::time::time_string<std::wstring>(yggr::time::time_format::tm_fmt_default<char>()) << std::endl;
	std::wcout << yggr::time::time::time_string<std::wstring>(yggr::time::time_format::tm_fmt_iso8601<char>()) << std::endl;

	// const char*,  alloc
	std::wcout << yggr::time::time::time_string<std::wstring>(fmt_default.str<std::wstring>().get_allocator()) << std::endl;
	std::wcout << yggr::time::time::time_string<std::wstring>(0, fmt_default.str<std::wstring>().get_allocator()) << std::endl;
	std::wcout << yggr::time::time::time_string<std::wstring>(yggr::time::time_format::tm_fmt_default<char>(), fmt_default.str<std::wstring>().get_allocator()) << std::endl;
	std::wcout << yggr::time::time::time_string<std::wstring>(yggr::time::time_format::tm_fmt_iso8601<char>(), fmt_iso8601.str<std::wstring>().get_allocator()) << std::endl;

	// const wchar_t*
	std::wcout << yggr::time::time::time_string<std::wstring>(yggr::time::time_format::tm_fmt_default<wchar_t>()) << std::endl;
	std::wcout << yggr::time::time::time_string<std::wstring>(yggr::time::time_format::tm_fmt_iso8601<wchar_t>()) << std::endl;

	// const wchar_t*,  alloc
	std::wcout << yggr::time::time::time_string<std::wstring>(yggr::time::time_format::tm_fmt_default<wchar_t>(), fmt_default.str<std::wstring>().get_allocator()) << std::endl;
	std::wcout << yggr::time::time::time_string<std::wstring>(yggr::time::time_format::tm_fmt_iso8601<wchar_t>(), fmt_iso8601.str<std::wstring>().get_allocator()) << std::endl;

	// std::string
	std::wcout << yggr::time::time::time_string<std::wstring>(fmt_default.str<std::string>()) << std::endl;
	std::wcout << yggr::time::time::time_string<std::wstring>(fmt_iso8601.str<std::string>()) << std::endl;

		// std::string,  alloc
	std::wcout << yggr::time::time::time_string<std::wstring>(fmt_default.str<std::string>(), fmt_default.str<std::wstring>().get_allocator()) << std::endl;
	std::wcout << yggr::time::time::time_string<std::wstring>(fmt_iso8601.str<std::string>(), fmt_iso8601.str<std::wstring>().get_allocator())  << std::endl;

	// boost::container::string
	std::wcout << yggr::time::time::time_string<std::wstring>(fmt_default.str<boost::container::string>()) << std::endl;
	std::wcout << yggr::time::time::time_string<std::wstring>(fmt_iso8601.str<boost::container::string>())  << std::endl;

	// boost::container::string,  alloc
	std::wcout << yggr::time::time::time_string<std::wstring>(fmt_default.str<boost::container::string>(), fmt_default.str<std::wstring>().get_allocator()) << std::endl;
	std::wcout << yggr::time::time::time_string<std::wstring>(fmt_iso8601.str<boost::container::string>(), fmt_iso8601.str<std::wstring>().get_allocator())  << std::endl;

	// yggr::string
	std::wcout << yggr::time::time::time_string<std::wstring>(fmt_default.str<yggr::string>()) << std::endl;
	std::wcout << yggr::time::time::time_string<std::wstring>(fmt_iso8601.str<yggr::string>())  << std::endl;

	// yggr::string,  alloc
	std::wcout << yggr::time::time::time_string<std::wstring>(fmt_default.str<yggr::string>(), fmt_default.str<std::wstring>().get_allocator()) << std::endl;
	std::wcout << yggr::time::time::time_string<std::wstring>(fmt_iso8601.str<yggr::string>(), fmt_iso8601.str<std::wstring>().get_allocator())  << std::endl;

	// yggr::utf8_string
	std::wcout << yggr::time::time::time_string<std::wstring>(fmt_default) << std::endl;
	std::wcout << yggr::time::time::time_string<std::wstring>(fmt_iso8601)  << std::endl;

	// yggr::utf8_string,  alloc
	std::wcout << yggr::time::time::time_string<std::wstring>(fmt_default, fmt_default.str<std::wstring>().get_allocator()) << std::endl;
	std::wcout << yggr::time::time::time_string<std::wstring>(fmt_iso8601, fmt_iso8601.str<std::wstring>().get_allocator())  << std::endl;

	// yggr::string_charset_helper
	std::wcout << yggr::time::time::time_string<std::wstring>(yggr::charset::make_string_charset_helper(fmt_default)) << std::endl;
	std::wcout << yggr::time::time::time_string<std::wstring>(fmt_iso8601)  << std::endl;

	// yggr::utf8_string,  alloc
	std::wcout << yggr::time::time::time_string<std::wstring>(fmt_default, fmt_default.str<std::wstring>().get_allocator()) << std::endl;
	std::wcout << yggr::time::time::time_string<std::wstring>(fmt_iso8601, fmt_iso8601.str<std::wstring>().get_allocator())  << std::endl;

	// std::wstring
	std::wcout << yggr::time::time::time_string<std::wstring>(fmt_default.str<std::wstring>()) << std::endl;
	std::wcout << yggr::time::time::time_string<std::wstring>(fmt_iso8601.str<std::wstring>())  << std::endl;

	// std::wstring,  alloc
	std::wcout << yggr::time::time::time_string<std::wstring>(fmt_default.str<std::wstring>(), fmt_default.str<std::wstring>().get_allocator()) << std::endl;
	std::wcout << yggr::time::time::time_string<std::wstring>(fmt_iso8601.str<std::wstring>(), fmt_iso8601.str<std::wstring>().get_allocator())  << std::endl;

	// boost::container::wstring
	std::wcout << yggr::time::time::time_string<std::wstring>(fmt_default.str<boost::container::wstring>()) << std::endl;
	std::wcout << yggr::time::time::time_string<std::wstring>(fmt_iso8601.str<boost::container::wstring>())  << std::endl;

	// boost::container::wstring,  alloc
	std::wcout << yggr::time::time::time_string<std::wstring>(fmt_default.str<boost::container::wstring>(), fmt_default.str<std::wstring>().get_allocator()) << std::endl;
	std::wcout << yggr::time::time::time_string<std::wstring>(fmt_iso8601.str<boost::container::wstring>(), fmt_iso8601.str<std::wstring>().get_allocator())  << std::endl;

	// yggr::wstring
	std::wcout << yggr::time::time::time_string<std::wstring>(fmt_default.str<yggr::wstring>()) << std::endl;
	std::wcout << yggr::time::time::time_string<std::wstring>(fmt_iso8601.str<yggr::wstring>())  << std::endl;

	// yggr::wstring,  alloc
	std::wcout << yggr::time::time::time_string<std::wstring>(fmt_default.str<yggr::wstring>(), fmt_default.str<std::wstring>().get_allocator()) << std::endl;
	std::wcout << yggr::time::time::time_string<std::wstring>(fmt_iso8601.str<yggr::wstring>(), fmt_iso8601.str<std::wstring>().get_allocator())  << std::endl;

#endif // YGGR_NO_CWCHAR

	// local
	// const char*
	std::cout << yggr::time::time::local_time_string() << std::endl;
	std::cout << yggr::time::time::local_time_string(0) << std::endl;
	std::cout << yggr::time::time::local_time_string(yggr::time::time_format::tm_fmt_default<char>()) << std::endl;
	std::cout << yggr::time::time::local_time_string(yggr::time::time_format::tm_fmt_iso8601<char>()) << std::endl;

	// const char*,  alloc
	std::cout << yggr::time::time::local_time_string(fmt_default.get_allocator()) << std::endl;
	std::cout << yggr::time::time::local_time_string(0, fmt_default.get_allocator()) << std::endl;
	std::cout << yggr::time::time::local_time_string(yggr::time::time_format::tm_fmt_default<char>(), fmt_default.get_allocator()) << std::endl;
	std::cout << yggr::time::time::local_time_string(yggr::time::time_format::tm_fmt_iso8601<char>(), fmt_iso8601.get_allocator()) << std::endl;

#if !defined(YGGR_NO_CWCHAR)
	// const wchar_t*
	std::cout << yggr::time::time::local_time_string(yggr::time::time_format::tm_fmt_default<wchar_t>()) << std::endl;
	std::cout << yggr::time::time::local_time_string(yggr::time::time_format::tm_fmt_iso8601<wchar_t>()) << std::endl;

	// const wchar_t*,  alloc
	std::cout << yggr::time::time::local_time_string(yggr::time::time_format::tm_fmt_default<wchar_t>(), fmt_default.get_allocator()) << std::endl;
	std::cout << yggr::time::time::local_time_string(yggr::time::time_format::tm_fmt_iso8601<wchar_t>(), fmt_iso8601.get_allocator()) << std::endl;
#endif // YGGR_NO_CWCHAR

	// std::string
	std::cout << yggr::time::time::local_time_string(fmt_default.str<std::string>()) << std::endl;
	std::cout << yggr::time::time::local_time_string(fmt_iso8601.str<std::string>()) << std::endl;

		// std::string,  alloc
	std::cout << yggr::time::time::local_time_string(fmt_default.str<std::string>(), fmt_default.get_allocator()) << std::endl;
	std::cout << yggr::time::time::local_time_string(fmt_iso8601.str<std::string>(), fmt_iso8601.get_allocator()) << std::endl;

	// boost::container::string
	std::cout << yggr::time::time::local_time_string(fmt_default.str<boost::container::string>()) << std::endl;
	std::cout << yggr::time::time::local_time_string(fmt_iso8601.str<boost::container::string>()) << std::endl;

	// boost::container::string,  alloc
	std::cout << yggr::time::time::local_time_string(fmt_default.str<boost::container::string>(), fmt_default.get_allocator()) << std::endl;
	std::cout << yggr::time::time::local_time_string(fmt_iso8601.str<boost::container::string>(), fmt_iso8601.get_allocator()) << std::endl;

	// yggr::string
	std::cout << yggr::time::time::local_time_string(fmt_default.str<yggr::string>()) << std::endl;
	std::cout << yggr::time::time::local_time_string(fmt_iso8601.str<yggr::string>()) << std::endl;

	// yggr::string,  alloc
	std::cout << yggr::time::time::local_time_string(fmt_default.str<yggr::string>(), fmt_default.get_allocator()) << std::endl;
	std::cout << yggr::time::time::local_time_string(fmt_iso8601.str<yggr::string>(), fmt_iso8601.get_allocator()) << std::endl;

	// yggr::utf8_string
	std::cout << yggr::time::time::local_time_string(fmt_default) << std::endl;
	std::cout << yggr::time::time::local_time_string(fmt_iso8601) << std::endl;

	// yggr::utf8_string,  alloc
	std::cout << yggr::time::time::local_time_string(fmt_default, fmt_default.get_allocator()) << std::endl;
	std::cout << yggr::time::time::local_time_string(fmt_iso8601, fmt_iso8601.get_allocator()) << std::endl;

	// yggr::string_charset_helper
	std::cout << yggr::time::time::local_time_string(yggr::charset::make_string_charset_helper(fmt_default)) << std::endl;
	std::cout << yggr::time::time::local_time_string(fmt_iso8601) << std::endl;

	// yggr::utf8_string,  alloc
	std::cout << yggr::time::time::local_time_string(fmt_default, fmt_default.get_allocator()) << std::endl;
	std::cout << yggr::time::time::local_time_string(fmt_iso8601, fmt_iso8601.get_allocator()) << std::endl;

#if !defined(YGGR_NO_CWCHAR)
	// std::wstring
	std::cout << yggr::time::time::local_time_string(fmt_default.str<std::wstring>()) << std::endl;
	std::cout << yggr::time::time::local_time_string(fmt_iso8601.str<std::wstring>()) << std::endl;

	// std::wstring,  alloc
	std::cout << yggr::time::time::local_time_string(fmt_default.str<std::wstring>(), fmt_default.get_allocator()) << std::endl;
	std::cout << yggr::time::time::local_time_string(fmt_iso8601.str<std::wstring>(), fmt_iso8601.get_allocator()) << std::endl;

	// boost::container::wstring
	std::cout << yggr::time::time::local_time_string(fmt_default.str<boost::container::wstring>()) << std::endl;
	std::cout << yggr::time::time::local_time_string(fmt_iso8601.str<boost::container::wstring>()) << std::endl;

	// boost::container::wstring,  alloc
	std::cout << yggr::time::time::local_time_string(fmt_default.str<boost::container::wstring>(), fmt_default.get_allocator()) << std::endl;
	std::cout << yggr::time::time::local_time_string(fmt_iso8601.str<boost::container::wstring>(), fmt_iso8601.get_allocator()) << std::endl;

	// yggr::wstring
	std::cout << yggr::time::time::local_time_string(fmt_default.str<yggr::wstring>()) << std::endl;
	std::cout << yggr::time::time::local_time_string(fmt_iso8601.str<yggr::wstring>()) << std::endl;

	// yggr::wstring,  alloc
	std::cout << yggr::time::time::local_time_string(fmt_default.str<yggr::wstring>(), fmt_default.get_allocator()) << std::endl;
	std::cout << yggr::time::time::local_time_string(fmt_iso8601.str<yggr::wstring>(), fmt_iso8601.get_allocator()) << std::endl;

#endif // YGGR_NO_CWCHAR

#if !defined(YGGR_NO_CWCHAR)
	// const char*
	std::wcout << yggr::time::time::local_time_string<std::wstring>() << std::endl;
	std::wcout << yggr::time::time::local_time_string<std::wstring>(0) << std::endl;
	std::wcout << yggr::time::time::local_time_string<std::wstring>(yggr::time::time_format::tm_fmt_default<char>()) << std::endl;
	std::wcout << yggr::time::time::local_time_string<std::wstring>(yggr::time::time_format::tm_fmt_iso8601<char>()) << std::endl;

	// const char*,  alloc
	std::wcout << yggr::time::time::local_time_string<std::wstring>(fmt_default.str<std::wstring>().get_allocator()) << std::endl;
	std::wcout << yggr::time::time::local_time_string<std::wstring>(0, fmt_default.str<std::wstring>().get_allocator()) << std::endl;
	std::wcout << yggr::time::time::local_time_string<std::wstring>(yggr::time::time_format::tm_fmt_default<char>(), fmt_default.str<std::wstring>().get_allocator()) << std::endl;
	std::wcout << yggr::time::time::local_time_string<std::wstring>(yggr::time::time_format::tm_fmt_iso8601<char>(), fmt_iso8601.str<std::wstring>().get_allocator()) << std::endl;

	// const wchar_t*
	std::wcout << yggr::time::time::local_time_string<std::wstring>(yggr::time::time_format::tm_fmt_default<wchar_t>()) << std::endl;
	std::wcout << yggr::time::time::local_time_string<std::wstring>(yggr::time::time_format::tm_fmt_iso8601<wchar_t>()) << std::endl;

	// const wchar_t*,  alloc
	std::wcout << yggr::time::time::local_time_string<std::wstring>(yggr::time::time_format::tm_fmt_default<wchar_t>(), fmt_default.str<std::wstring>().get_allocator()) << std::endl;
	std::wcout << yggr::time::time::local_time_string<std::wstring>(yggr::time::time_format::tm_fmt_iso8601<wchar_t>(), fmt_iso8601.str<std::wstring>().get_allocator()) << std::endl;

	// std::string
	std::wcout << yggr::time::time::local_time_string<std::wstring>(fmt_default.str<std::string>()) << std::endl;
	std::wcout << yggr::time::time::local_time_string<std::wstring>(fmt_iso8601.str<std::string>()) << std::endl;

		// std::string,  alloc
	std::wcout << yggr::time::time::local_time_string<std::wstring>(fmt_default.str<std::string>(), fmt_default.str<std::wstring>().get_allocator()) << std::endl;
	std::wcout << yggr::time::time::local_time_string<std::wstring>(fmt_iso8601.str<std::string>(), fmt_iso8601.str<std::wstring>().get_allocator())  << std::endl;

	// boost::container::string
	std::wcout << yggr::time::time::local_time_string<std::wstring>(fmt_default.str<boost::container::string>()) << std::endl;
	std::wcout << yggr::time::time::local_time_string<std::wstring>(fmt_iso8601.str<boost::container::string>())  << std::endl;

	// boost::container::string,  alloc
	std::wcout << yggr::time::time::local_time_string<std::wstring>(fmt_default.str<boost::container::string>(), fmt_default.str<std::wstring>().get_allocator()) << std::endl;
	std::wcout << yggr::time::time::local_time_string<std::wstring>(fmt_iso8601.str<boost::container::string>(), fmt_iso8601.str<std::wstring>().get_allocator())  << std::endl;

	// yggr::string
	std::wcout << yggr::time::time::local_time_string<std::wstring>(fmt_default.str<yggr::string>()) << std::endl;
	std::wcout << yggr::time::time::local_time_string<std::wstring>(fmt_iso8601.str<yggr::string>())  << std::endl;

	// yggr::string,  alloc
	std::wcout << yggr::time::time::local_time_string<std::wstring>(fmt_default.str<yggr::string>(), fmt_default.str<std::wstring>().get_allocator()) << std::endl;
	std::wcout << yggr::time::time::local_time_string<std::wstring>(fmt_iso8601.str<yggr::string>(), fmt_iso8601.str<std::wstring>().get_allocator())  << std::endl;

	// yggr::utf8_string
	std::wcout << yggr::time::time::local_time_string<std::wstring>(fmt_default) << std::endl;
	std::wcout << yggr::time::time::local_time_string<std::wstring>(fmt_iso8601)  << std::endl;

	// yggr::utf8_string,  alloc
	std::wcout << yggr::time::time::local_time_string<std::wstring>(fmt_default, fmt_default.str<std::wstring>().get_allocator()) << std::endl;
	std::wcout << yggr::time::time::local_time_string<std::wstring>(fmt_iso8601, fmt_iso8601.str<std::wstring>().get_allocator())  << std::endl;

	// yggr::string_charset_helper
	std::wcout << yggr::time::time::local_time_string<std::wstring>(yggr::charset::make_string_charset_helper(fmt_default)) << std::endl;
	std::wcout << yggr::time::time::local_time_string<std::wstring>(fmt_iso8601)  << std::endl;

	// yggr::utf8_string,  alloc
	std::wcout << yggr::time::time::local_time_string<std::wstring>(fmt_default, fmt_default.str<std::wstring>().get_allocator()) << std::endl;
	std::wcout << yggr::time::time::local_time_string<std::wstring>(fmt_iso8601, fmt_iso8601.str<std::wstring>().get_allocator())  << std::endl;

	// std::wstring
	std::wcout << yggr::time::time::local_time_string<std::wstring>(fmt_default.str<std::wstring>()) << std::endl;
	std::wcout << yggr::time::time::local_time_string<std::wstring>(fmt_iso8601.str<std::wstring>())  << std::endl;

	// std::wstring,  alloc
	std::wcout << yggr::time::time::local_time_string<std::wstring>(fmt_default.str<std::wstring>(), fmt_default.str<std::wstring>().get_allocator()) << std::endl;
	std::wcout << yggr::time::time::local_time_string<std::wstring>(fmt_iso8601.str<std::wstring>(), fmt_iso8601.str<std::wstring>().get_allocator())  << std::endl;

	// boost::container::wstring
	std::wcout << yggr::time::time::local_time_string<std::wstring>(fmt_default.str<boost::container::wstring>()) << std::endl;
	std::wcout << yggr::time::time::local_time_string<std::wstring>(fmt_iso8601.str<boost::container::wstring>())  << std::endl;

	// boost::container::wstring,  alloc
	std::wcout << yggr::time::time::local_time_string<std::wstring>(fmt_default.str<boost::container::wstring>(), fmt_default.str<std::wstring>().get_allocator()) << std::endl;
	std::wcout << yggr::time::time::local_time_string<std::wstring>(fmt_iso8601.str<boost::container::wstring>(), fmt_iso8601.str<std::wstring>().get_allocator())  << std::endl;

	// yggr::wstring
	std::wcout << yggr::time::time::local_time_string<std::wstring>(fmt_default.str<yggr::wstring>()) << std::endl;
	std::wcout << yggr::time::time::local_time_string<std::wstring>(fmt_iso8601.str<yggr::wstring>())  << std::endl;

	// yggr::wstring,  alloc
	std::wcout << yggr::time::time::local_time_string<std::wstring>(fmt_default.str<yggr::wstring>(), fmt_default.str<std::wstring>().get_allocator()) << std::endl;
	std::wcout << yggr::time::time::local_time_string<std::wstring>(fmt_iso8601.str<yggr::wstring>(), fmt_iso8601.str<std::wstring>().get_allocator())  << std::endl;

#endif // YGGR_NO_CWCHAR

}


int main(int argc, char* argv[])
{
#ifndef YGGR_NO_CWCHAR
	TEST_PP_WIO_IMBUE("chs")
#endif // YGGR_NO_CWCHAR

	test();
	test2();
	test3();
	test4();
	test5();
	test6();
	test7();

	wait_any_key(argc, argv);
	return 0;
}
