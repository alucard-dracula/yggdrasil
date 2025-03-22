// swap_adl_iss.cpp

#include <algorithm> //for std::swap
#include <iostream>
#include <vector>

#include <boost/utility/swap.hpp>
#include <boost/move/move.hpp>

template <typename Val,
			typename Ax = std::allocator<Val>,
			template<typename _Ty, typename _Ax> class Vector = std::vector
		>
class child_vector
	: protected Vector<Val, Ax>
{

public:
	typedef Vector<Val, Ax> base_type;

private:

	typedef child_vector this_type;

public:
	child_vector(void)
	{
	}

	child_vector(BOOST_RV_REF(base_type) right)
	{
	}

	child_vector(BOOST_RV_REF(this_type) right)
	{
	}

	child_vector(const this_type& right)
		: base_type(right)
	{
	}

	~child_vector(void)
	{
	}

	this_type& operator=(BOOST_RV_REF(base_type) right)
	{
		return *this;
	}

	this_type& operator=(const base_type& right)
	{
		return *this;
	}

    this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		return *this;
	}

	this_type& operator=(const this_type& right)
	{
		return *this;
	}

//#ifndef _MSC_VER
    void swap(BOOST_RV_REF(base_type) right)
    {
		base_type& right_ref = right;
		this_type::swap(right_ref);
        //std::cout << "swap base" << std::endl;
    }
//#endif // _MSC_VER

	void swap(base_type& right)
	{
		std::cout << "swap base" << std::endl;
	}

//#ifndef _MSC_VER
    void swap(BOOST_RV_REF(this_type) right)
    {
		this_type& right_ref = right;
		this_type::swap(right_ref);
       // std::cout << "swap base" << std::endl;
    }
//#endif // _MSC_VER

	void swap(this_type& right)
	{
		std::cout << "swap this" << std::endl;
	}
};

namespace test
{
namespace detail
{

template<typename T0, typename T1,
			template<typename _T0, typename T1> class Base >
void swap(Base<T0, T1>& l, child_vector<T0, T1, Base>& r)
{
	r.swap(l);
}

template<typename T0, typename T1,
			template<typename _T0, typename T1> class Base >
void swap(child_vector<T0, T1, Base>& l, child_vector<T0, T1, Base>& r)
{
	r.swap(l);
}


} // namespace detail
} // namespace test

namespace std
{
	using test::detail::swap;

} // namespace std

namespace boost
{
	using test::detail::swap;

} // namespace boost

int main(int argc, char* argv)
{
	typedef child_vector<int> vt_type;
	typedef vt_type::base_type base_vt_type;

	vt_type vt;
	base_vt_type base_vt;

	std::swap(base_vt, vt);
	boost::swap(base_vt, vt);

	std::cout << "-----------------" << std::endl;

	boost::swap(boost::move(base_vt), vt);
	std::swap(boost::move(base_vt), vt);

	std::cout << "-----------------" << std::endl;
	vt.swap(base_vt);
	vt.swap(boost::move(base_vt));
	vt.swap(vt);
	vt.swap(boost::move(vt));

	char cc = 0;
	std::cin >> cc;

	return 0;
}
