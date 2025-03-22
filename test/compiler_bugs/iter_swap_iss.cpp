//iter_swap_iss.cpp

#include <algorithm>
#include <utility>
#include <vector>

#include <boost/utility/enable_if.hpp>
#include <yggr/utility/copy_or_move_or_swap.hpp>
#include <yggr/ppex/swap_this_def.hpp>
#include <yggr/container/array.hpp>

struct A : public yggr::array<int, 3>
{
	typedef yggr::array<int, 3> base_type;
	A(void)
	{
	}

	A(int nn)
		: n(nn)
	{
	}

	void swap(base_type& right)
	{
		base_type::swap(right);
	}

	void swap(A& right)
	{
		yggr::swap(n, right.n);
	}

	int n;
};

bool operator<(const A& l, const A& r)
{
	return l.n < r.n;
}


namespace std
{

inline void swap(A::base_type& l, A& r)
{
	r.swap(l);
}

inline void swap(A& l, A::base_type& r)
{
	l.swap(r);
}

inline void swap(A& l, A& r)
{
	l.swap(r);
}

} // namespace std

namespace boost
{

inline void swap(A::base_type& l, A& r)
{
	r.swap(l);
}

inline void swap(A& l, A::base_type& r)
{
	l.swap(r);
}

inline void swap(A& l, A& r)
{
	l.swap(r);
}

} // namespace boost

void test_bug(void)
{
	typedef yggr::array<A, 3> point_array_type;

	point_array_type arr = {A(1), A(2), A(3)};
	std::reverse(arr.begin(), arr.end());
}

int main(int argc, char* argv[])
{
	test_bug();
	char cc = 0;
	std::cin >> cc;

	return 0;
}
