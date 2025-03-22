// swap_adl_iss_fixex.cpp

#include <algorithm> //for std::swap
#include <iostream>
#include <vector>

#include <boost/utility/swap.hpp>
#include <boost/move/move.hpp>

// at gcc boost adl not support this namespace adl why?
// so swap foo don't append boost_swap_impl namespace, nedd append boost namespace 
//namespace boost_swap_impl 
//{
//	template<typename T1, typename T2>
//	void swap_impl(T1& left, T2& right)
//	{
//		using std::swap;
//		swap(left, right);
//	}
//} // namespace boost_swap_impl


// diff type swap at stl
namespace std
{

// stl container need these foos
template<typename T1, typename T2>
void swap(BOOST_RV_REF(T1) l, T2& r)
{
	T1& ref_l = l;
	using ::std::swap;
	swap(ref_l, r);
}

template<typename T1, typename T2>
void swap(T1& l, BOOST_RV_REF(T2) r)
{
	T2& ref_r = r;
	using ::std::swap;
	swap(l, ref_r);
}

// support diff type swap
template<typename T1, typename T2>
void swap(T1& l, T2& r)
{
	T1 tmp = boost::move(l);
	l = boost::move(r);
	r = boost::move(tmp);
}

} // namespace std

// diff type swap at boost
namespace boost
{

// gcc default conv t&& -> const t&
// but msvc default conv t&& -> t&, so msvc error C2668: ambiguous call to overloaded function occured,
// so msvc not need these foos

#ifndef _MSC_VER
template<typename T1, typename T2>
void swap(BOOST_RV_REF(T1) l, T2& r)
{
	T1& ref_l = l;

	using ::std::swap;
	swap(ref_l, r);
}

template<typename T1, typename T2>
void swap(T1& l, BOOST_RV_REF(T2) r)
{
	T2& ref_r = r;

	using ::std::swap;
	swap(l, ref_r);
}
#endif // _MSC_VER

template<typename T1>
void swap(T1& l, T1& r)
{
	using ::std::swap;
	swap(l, r);
}

} //namespace boost

// use test boost_swap_impl namespace gcc aways error 
//namespace boost_swap_impl
//{
//
//// gcc default conv t&& -> const t&
//// but msvc default conv t&& -> t&, so msvc error C2668: ambiguous call to overloaded function occured,
//// so msvc not need these foos
//
//#ifndef _MSC_VER
//template<typename T1, typename T2>
//void swap_impl(BOOST_RV_REF(T1) l, T2& r)
//{
//	T1& ref_l = l;
//
//	using ::std::swap;
//	swap(ref_l, r);
//}
//
//template<typename T1, typename T2>
//void swap_impl(T1& l, BOOST_RV_REF(T2) r)
//{
//	T2& ref_r = r;
//
//	using ::std::swap;
//	swap(l, ref_r);
//}
//#endif // _MSC_VER
//
//template<typename T1, typename T2>
//void swap_impl(T1& l, T2& r)
//{
//	using ::std::swap;
//	swap(l, r);
//}
//
//} //namespace boost_swap_impl
//
//namespace boost
//{
//
//// gcc default conv t&& -> const t&
//// but msvc default conv t&& -> t&, so msvc error C2668: ambiguous call to overloaded function occured,
//// so msvc not need these foos
//
//#ifndef _MSC_VER
//template<typename T1, typename T2>
//void swap(BOOST_RV_REF(T1) l, T2& r)
//{
//	T1& ref_l = l;
//
//	using ::std::swap;
//	swap(ref_l, r);
//}
//
//template<typename T1, typename T2>
//void swap(T1& l, BOOST_RV_REF(T2) r)
//{
//	T2& ref_r = r;
//
//	using ::std::swap;
//	swap(l, ref_r);
//}
//#endif // _MSC_VER
//
//template<typename T1>
//void swap(T1& l, T1& r)
//{
//	using ::std::swap;
//	swap(l, r);
//}
//
//} //namespace boost

namespace yggr
{

#ifndef _MSC_VER
template<typename T1, typename T2>
void swap(BOOST_RV_REF(T1) l, T2& r)
{
	T1& ref_l = l;

	using std::swap;
	swap(ref_l, r);
}

template<typename T1, typename T2>
void swap(T1& l, BOOST_RV_REF(T2) r)
{
	T2& ref_r = r;

	using std::swap;
	swap(l, ref_r);
}
#endif // _MSC_VER

template<typename T1, typename T2>
void swap(T1& l, T2& r)
{
	using std::swap;
	swap(l, r);
}

template<typename T>
void swap(T& l, T& r)
{
	using std::swap;
	swap(l, r);
}

} // namespace yggr

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
	yggr::swap(base_vt, vt);
	yggr::swap(vt, vt);


	std::cout << "-----------------" << std::endl;

	boost::swap(boost::move(base_vt), vt);
	std::swap(boost::move(base_vt), vt);
	yggr::swap(boost::move(base_vt), vt);
	yggr::swap(boost::move(vt), vt);

	std::cout << "-----------------" << std::endl;
	vt.swap(base_vt);
	vt.swap(boost::move(base_vt));
	vt.swap(vt);
	vt.swap(boost::move(vt));

	char cc = 0;
	std::cin >> cc;

	return 0;
}
