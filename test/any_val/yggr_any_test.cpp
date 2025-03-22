// yggr_any_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <cassert>
#include <utility>
#include <yggr/any_val/any.hpp>

#ifdef _MSC_VER
#include <vld.h>
#endif // _MSC_VER

void test_construct_small(void)
{
	typedef yggr::u64 data_type;
	// any()
	{
		boost::any any;
		yggr_test_assert(any.empty());
	}

	// any<ValueType>
	{
		boost::any any(data_type(10));
		yggr_test_assert(10 == yggr::any_cast<data_type>(any));
	}

	// any(BOOST_RV_REF(this_type))
	{
		yggr::any any1(data_type(10));
		yggr::any any2(boost::move(any1));
		yggr_test_assert(data_type(10) == yggr::any_cast<data_type>(any2));
	}

	// any(const this_type&)
	{
		yggr::any any(data_type(10));

		yggr::any any2(any);
		yggr_test_assert(10 == yggr::any_cast<data_type>(any));
		yggr_test_assert(10 == yggr::any_cast<data_type>(any2));

		yggr_test_assert(!yggr::any_cast<float>(&any));
		yggr_test_assert(yggr::any_cast<data_type>(&any));

		const yggr::any& cref_any = any; 
		yggr_test_assert(yggr::any_cast<data_type>(cref_any) == 10);
		yggr_test_assert(!yggr::any_cast<float>(&cref_any));
		yggr_test_assert(yggr::any_cast<data_type>(&cref_any));

		data_type& n = yggr::any_cast<data_type&>(any);
		n = 100;
		yggr_test_assert(100 == yggr::any_cast<data_type>(any));
	}

	std::cout << "test_construct_small succeed" << std::endl;
}

void test_construct_big(void)
{
	typedef std::pair<yggr::u64, yggr::u64> data_type;


	// any<ValueType>
	{
		boost::any any(data_type(10, 20));
		yggr_test_assert(data_type(10, 20) == yggr::any_cast<data_type>(any));
	}

	// any(BOOST_RV_REF(this_type))
	{
		yggr::any any1(data_type(10, 20));
		yggr::any any2(boost::move(any1));
		yggr_test_assert(data_type(10, 20) == yggr::any_cast<data_type>(any2));
	}

	// any(const this_type&)
	{
		yggr::any any(data_type(10, 20));

		yggr::any any2(any);
		yggr_test_assert(data_type(10, 20) == yggr::any_cast<data_type>(any));
		yggr_test_assert(data_type(10, 20) == yggr::any_cast<data_type>(any2));

		yggr_test_assert(!yggr::any_cast<float>(&any));
		yggr_test_assert(yggr::any_cast<data_type>(&any));

		const yggr::any& cref_any = any; 
		yggr_test_assert(yggr::any_cast<data_type>(cref_any) == data_type(10, 20));
		yggr_test_assert(!yggr::any_cast<float>(&cref_any));
		yggr_test_assert(yggr::any_cast<data_type>(&cref_any));

		data_type& n = yggr::any_cast<data_type&>(any);
		n = data_type(100, 200);
		yggr_test_assert(data_type(100, 200) == yggr::any_cast<data_type>(any));
	}

	std::cout << "test_construct_big succeed" << std::endl;
}

void test_set_small(void)
{
	typedef yggr::u64 data_type;

	{
		// operator=(value_type)
		yggr::any any;
		any = data_type(10);
		yggr_test_assert(10 == yggr::any_cast<data_type>(any));

		any = data_type(20);
		yggr_test_assert(20 == yggr::any_cast<data_type>(any));

		// operator=(const this_type&)
		any = yggr::any();
		yggr_test_assert(any.empty());
		yggr_test_assert(!yggr::any_cast<data_type>(&any));


		// operator=(BOOST_RV_REF(this_type))
		yggr::any any2(data_type(100));
		any = boost::move(any2);
		yggr_test_assert(data_type(100) == yggr::any_cast<data_type>(any));

		any2 = boost::move(any);
		yggr_test_assert(data_type(100) == yggr::any_cast<data_type>(any2));
	}

	std::cout << "test_equal_small succeed" << std::endl;
}

void test_set_big(void)
{
	typedef std::pair<yggr::u64, yggr::u64> data_type;

	{
		// operator=(value_type)
		yggr::any any;
		any = data_type(data_type(10, 20));
		yggr_test_assert(data_type(10, 20) == yggr::any_cast<data_type>(any));

		any = data_type(30, 40);
		yggr_test_assert(data_type(30, 40) == yggr::any_cast<data_type>(any));

		// operator=(const this_type&)
		any = yggr::any();
		yggr_test_assert(any.empty());
		yggr_test_assert(!yggr::any_cast<data_type>(&any));


		// operator=(BOOST_RV_REF(this_type))
		yggr::any any2(data_type(100, 200));
		any = boost::move(any2);
		yggr_test_assert(data_type(100, 200) == yggr::any_cast<data_type>(any));

		any2 = boost::move(any);
		yggr_test_assert(data_type(100, 200) == yggr::any_cast<data_type>(any2));
	}

	std::cout << "test_set_big succeed" << std::endl;
}

void test_set_midx(void)
{
	typedef yggr::u64 small_data_type;
	typedef std::pair<yggr::u64, yggr::u64> big_data_type;

	// operator=(const ValueType&)
	{
		yggr::any any;
		any = small_data_type(10);
		yggr_test_assert(small_data_type(10) == yggr::any_cast<small_data_type>(any));
		
		any = big_data_type(10, 20);
		yggr_test_assert(big_data_type(10, 20) == yggr::any_cast<big_data_type>(any));
	}

	// operator=(const this_type&)
	{
		yggr::any small_any = small_data_type(10);
		yggr::any big_any = big_data_type(10, 20);

		yggr_test_assert(small_data_type(10) == yggr::any_cast<small_data_type>(small_any));
		yggr_test_assert(big_data_type(10, 20) == yggr::any_cast<big_data_type>(big_any));

		yggr::any tmp;

		tmp = small_any;
		small_any = big_any;
		big_any = tmp;

		yggr_test_assert(small_data_type(10) == yggr::any_cast<small_data_type>(big_any));
		yggr_test_assert(big_data_type(10, 20) == yggr::any_cast<big_data_type>(small_any));
		yggr_test_assert(small_data_type(10) == yggr::any_cast<small_data_type>(tmp));
	}

	// operator=(BOOST_RV_REF(this_type))
	{
		yggr::any small_any = small_data_type(10);
		yggr::any big_any = big_data_type(10, 20);

		yggr_test_assert(small_data_type(10) == yggr::any_cast<small_data_type>(small_any));
		yggr_test_assert(big_data_type(10, 20) == yggr::any_cast<big_data_type>(big_any));

		yggr::any tmp;

		tmp = boost::move(small_any);
		small_any = boost::move(big_any);
		big_any = boost::move(tmp);

		yggr_test_assert(small_data_type(10) == yggr::any_cast<small_data_type>(big_any));
		yggr_test_assert(big_data_type(10, 20) == yggr::any_cast<big_data_type>(small_any));
	}

	std::cout << "test_set_midx succeed" << std::endl;
}

void test_swap_small(void)
{
	typedef yggr::u64 data_type;

	{
		yggr::any any1 = data_type(10);
		yggr::any any2 = data_type(20);

		yggr_test_assert(data_type(10) == yggr::any_cast<data_type>(any1));
		yggr_test_assert(data_type(20) == yggr::any_cast<data_type>(any2));

		any1.swap(any2);

		yggr_test_assert(data_type(20) == yggr::any_cast<data_type>(any1));
		yggr_test_assert(data_type(10) == yggr::any_cast<data_type>(any2));

		any1.swap(boost::move(any2));

		yggr_test_assert(data_type(10) == yggr::any_cast<data_type>(any1));
		yggr_test_assert(data_type(20) == yggr::any_cast<data_type>(any2));

		yggr::swap(any1, any2);

		yggr_test_assert(data_type(20) == yggr::any_cast<data_type>(any1));
		yggr_test_assert(data_type(10) == yggr::any_cast<data_type>(any2));

		yggr::swap(any1, boost::move(any2));

		yggr_test_assert(data_type(10) == yggr::any_cast<data_type>(any1));
		yggr_test_assert(data_type(20) == yggr::any_cast<data_type>(any2));

		yggr::any any3;
		yggr::swap(any1, any3);
		yggr_test_assert(data_type(10) == yggr::any_cast<data_type>(any3));
		yggr_test_assert(any1.empty());
	}

	std::cout << "test_swap_small succeed" << std::endl;
}

void test_swap_big(void)
{
	typedef std::pair<yggr::u64, yggr::u64> data_type;

	{
		yggr::any any1 = data_type(10, 20);
		yggr::any any2 = data_type(30, 40);

		yggr_test_assert(data_type(10, 20) == yggr::any_cast<data_type>(any1));
		yggr_test_assert(data_type(30, 40) == yggr::any_cast<data_type>(any2));

		any1.swap(any2);

		yggr_test_assert(data_type(30, 40) == yggr::any_cast<data_type>(any1));
		yggr_test_assert(data_type(10, 20) == yggr::any_cast<data_type>(any2));

		any1.swap(boost::move(any2));

		yggr_test_assert(data_type(10, 20) == yggr::any_cast<data_type>(any1));
		yggr_test_assert(data_type(30, 40) == yggr::any_cast<data_type>(any2));

		yggr::swap(any1, any2);

		yggr_test_assert(data_type(30, 40) == yggr::any_cast<data_type>(any1));
		yggr_test_assert(data_type(10, 20) == yggr::any_cast<data_type>(any2));

		yggr::swap(any1, boost::move(any2));

		yggr_test_assert(data_type(10, 20) == yggr::any_cast<data_type>(any1));
		yggr_test_assert(data_type(30, 40) == yggr::any_cast<data_type>(any2));

		yggr::any any3;
		yggr::swap(any1, any3);
		yggr_test_assert(data_type(10, 20) == yggr::any_cast<data_type>(any3));
		yggr_test_assert(any1.empty());
	}

	std::cout << "test_swap_big succeed" << std::endl;
}

void test_swap_mix(void)
{
	typedef yggr::u64 small_data_type;
	typedef std::pair<yggr::u64, yggr::u64> big_data_type;

	{
		yggr::any any1 = small_data_type(10);
		yggr::any any2 = big_data_type(30, 40);

		yggr_test_assert(small_data_type(10) == yggr::any_cast<small_data_type>(any1));
		yggr_test_assert(big_data_type(30, 40) == yggr::any_cast<big_data_type>(any2));

		any1.swap(any2);

		yggr_test_assert(big_data_type(30, 40) == yggr::any_cast<big_data_type>(any1));
		yggr_test_assert(small_data_type(10) == yggr::any_cast<small_data_type>(any2));

		any1.swap(boost::move(any2));

		yggr_test_assert(small_data_type(10) == yggr::any_cast<small_data_type>(any1));
		yggr_test_assert(big_data_type(30, 40) == yggr::any_cast<big_data_type>(any2));

		yggr::swap(any1, any2);

		yggr_test_assert(big_data_type(30, 40) == yggr::any_cast<big_data_type>(any1));
		yggr_test_assert(small_data_type(10) == yggr::any_cast<small_data_type>(any2));

		yggr::swap(any1, boost::move(any2));

		yggr_test_assert(small_data_type(10) == yggr::any_cast<small_data_type>(any1));
		yggr_test_assert(big_data_type(30, 40) == yggr::any_cast<big_data_type>(any2));

		yggr::any any3;
		yggr::swap(any1, any3);
		yggr_test_assert(small_data_type(10) == yggr::any_cast<small_data_type>(any3));
		yggr_test_assert(any1.empty());

		yggr::swap(any1, boost::move(any2));
		yggr_test_assert(big_data_type(30, 40) == yggr::any_cast<big_data_type>(any1));
		yggr_test_assert(any2.empty());
	}

	std::cout << "test_swap_mix succeed" << std::endl;
}
	
int main(int argc, char* argv[])
{
	test_construct_small();
	test_construct_big();

	test_set_small();
	test_set_big();
	test_set_midx();

	test_swap_small();
	test_swap_big();
	test_swap_mix();

	wait_any_key(argc, argv);
	return 0;
}