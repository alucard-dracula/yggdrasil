// copy_or_move_or_swap_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <yggr/utility/copy_or_move_or_swap.hpp>

#include <vector>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

struct A
{
	void swap(A& )
	{
	}
};

class B
{
public:
	B(void)
		: m(0)
	{
	}

	int m;
};

typedef std::vector<int> vt_type;

template<typename T>
T gen_rv(const T& t)
{
    return t;
}

template<typename T>
T& test_real_rv(T& l, BOOST_RV_REF(T) r)
{
	//return yggr::copy_or_move_or_swap(l, boost::forward<T>(r));
	return yggr::copy_or_move_or_swap(l, boost::move(r));
}

template<typename T>
T& test_real_rv(T& l, const T& r)
{
	return yggr::copy_or_move_or_swap(l, r);
}

int main(int argc, char* argv[])
{
	{
		int l = 0;
		int r = 0;

		yggr::copy_or_move_or_swap(l, r); // c98 copy c11 move
		yggr::copy_or_move_or_swap(l, boost::move(r)); // c98 copy, c11 move

		test_real_rv(l, gen_rv(r)); // c98 copy, c11 move
	}

	{
		A l;
		A r;

		yggr::copy_or_move_or_swap(l, r); // swap
		test_real_rv(l, gen_rv(r)); // c98 copy c11 move
	}

	{
		B l;
		B r;

		yggr::copy_or_move_or_swap(l, boost::forward<B>(r)); // c98 copy c11 move
		yggr::copy_or_move_or_swap(l, boost::move(r)); // move
		test_real_rv(l, gen_rv(r)); // c98 copy c11 move
	}

	{
		vt_type l;
		vt_type r;

		yggr::copy_or_move_or_swap(l, boost::move(r)); // swap
		test_real_rv(l, gen_rv(r)); // c98 copy c11 move
	}

	wait_any_key(argc, argv);
	return 0;
}
