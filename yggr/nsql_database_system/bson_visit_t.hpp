// bson_visit_t.hpp

/****************************************************************************
Copyright (c) 2010-2024 yggdrasil

author: xu yang

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_BSON_VISIT_T_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_BSON_VISIT_T_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/move/move.hpp>
#include <yggr/utility/swap.hpp>
#include <yggr/mplex/static_assert.hpp>
#include <yggr/nsql_database_system/bson_json_opts.hpp>

#include <boost/ref.hpp>
#include <boost/type_traits/add_reference.hpp>
#include <boost/mpl/integral_c.hpp>
#include <boost/fusion/include/pair.hpp>

namespace yggr
{
namespace nsql_database_system
{

template<typename TVal, 
			bson_json_mode_def::mode_type TMode = bson_json_mode_def::E_BSON_JSON_MODE_LEGACY>
struct bson_visit_t
	: public 
		boost::fusion::pair
		<
			boost::mpl::integral_c<bson_json_mode_def::mode_type, TMode>,
			boost::reference_wrapper<const TVal>
		>
{
private:
	BOOST_MPL_ASSERT((
		boost::mpl::bool_<
			(bson_json_mode_def::E_BSON_JSON_MODE_LEGACY <= TMode 
				&& TMode <= bson_json_mode_def::E_BSON_JSON_MODE_RELAXED) >));

public:
	typedef 
		boost::fusion::pair
		<
			boost::mpl::integral_c<bson_json_mode_def::mode_type, TMode>,
			boost::reference_wrapper<const TVal>
		> base_type;

	typedef bson_json_mode_def::mode_type mode_type;

	typedef typename base_type::first_type tag_mode_type;
	typedef typename base_type::second_type value_cref_wrap_type;
	typedef typename boost::add_reference<const TVal>::type value_cref_type;

private:
	typedef bson_visit_t this_type;

public:
	bson_visit_t(value_cref_type val_cref)
		: base_type(boost::fusion::make_pair<tag_mode_type>(value_cref_wrap_type(val_cref)))
	{
	}

	template<bson_json_mode_def::mode_type Mode>
	bson_visit_t(BOOST_RV_REF_BEG 
						boost::fusion::pair
						<
							boost::mpl::integral_c<bson_json_mode_def::mode_type, Mode>,
							boost::reference_wrapper<const TVal>
						>
					BOOST_RV_REF_END right)
		: base_type(
			boost::fusion::make_pair<tag_mode_type>(
				static_cast<
					boost::fusion::pair
					<
						boost::mpl::integral_c<bson_json_mode_def::mode_type, Mode>,
						boost::reference_wrapper<const TVal>
					>&
				>(right).second))
	{
	}

	template<bson_json_mode_def::mode_type Mode>
	bson_visit_t(const 
					boost::fusion::pair
					<
						boost::mpl::integral_c<bson_json_mode_def::mode_type, Mode>,
						boost::reference_wrapper<const TVal>
					>& right)
		: base_type(boost::fusion::make_pair<tag_mode_type>(right.second))
	{
	}

	template<bson_json_mode_def::mode_type Mode>
	bson_visit_t(BOOST_RV_REF_BEG bson_visit_t<TVal, Mode> BOOST_RV_REF_END right)
		: base_type(
			boost::fusion::make_pair<tag_mode_type>(
				static_cast<bson_visit_t<TVal, Mode>&>(right).second))
	{
	}

	template<bson_json_mode_def::mode_type Mode>
	bson_visit_t(const bson_visit_t<TVal, Mode>& right)
		: base_type(boost::fusion::make_pair<tag_mode_type>(right.second))
	{
	}

	bson_visit_t(BOOST_RV_REF(base_type) right)
		: base_type(boost::move(static_cast<base_type&>(right)))
	{
	}

	bson_visit_t(const base_type& right)
		: base_type(right)
	{
	}

	bson_visit_t(BOOST_RV_REF(this_type) right)
		: base_type(boost::move(static_cast<base_type&>(right)))
	{
	}

	bson_visit_t(const this_type& right)
		: base_type(static_cast<const base_type&>(right))
	{
	}

	~bson_visit_t(void)
	{
	}

public:
	template<bson_json_mode_def::mode_type Mode> inline
	this_type& operator=(BOOST_RV_REF_BEG 
								boost::fusion::pair
								<
									boost::mpl::integral_c<bson_json_mode_def::mode_type, Mode>,
									boost::reference_wrapper<const TVal>
								> 
							BOOST_RV_REF_END right)
	{
		typedef 
			boost::fusion::pair
			<
				boost::mpl::integral_c<bson_json_mode_def::mode_type, Mode>,
				boost::reference_wrapper<const TVal>
			> right_type;

		right_type& right_ref = right;
		base_type::second = boost::move(right_ref.second);
		return *this;
	}

	template<bson_json_mode_def::mode_type Mode> inline
	this_type& operator=(const 
							boost::fusion::pair
							<
								boost::mpl::integral_c<bson_json_mode_def::mode_type, Mode>,
								boost::reference_wrapper<const TVal>
							>& right)
	{
		base_type::second = right.second;
		return *this;
	}

	template<bson_json_mode_def::mode_type Mode> inline
	this_type& operator=(BOOST_RV_REF_BEG bson_visit_t<TVal, Mode> BOOST_RV_REF_END right)
	{
		typedef bson_visit_t<TVal, Mode> right_type;

		right_type& right_ref = right;
		base_type::second = right_ref.second;
		return *this;
	}

	template<bson_json_mode_def::mode_type Mode> inline
	this_type& operator=(const bson_visit_t<TVal, Mode>& right)
	{
		base_type::second = right.second;
		return *this;
	}

	inline this_type& operator=(BOOST_RV_REF(base_type) right)
	{
		base_type& right_ref = right;
		if(static_cast<base_type*>(this) == boost::addressof(right_ref))
		{
			return *this;
		}

		base_type::second = boost::move(right_ref.second);
		return *this;
	}

	inline this_type& operator=(const base_type& right)
	{
		if(static_cast<base_type*>(this) == boost::addressof(right))
		{
			return *this;
		}

		base_type::second = right.second;
		return *this;
	}

	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;

		if(this == boost::addressof(right_ref))
		{
			return *this;
		}

		base_type::second = boost::move(right_ref.second);
		return *this;
	}

	inline this_type& operator=(const this_type& right)
	{
		if(this == boost::addressof(right))
		{
			return *this;
		}

		base_type::second = right.second;
		return *this;
	}

public:
	template<bson_json_mode_def::mode_type Mode> inline
	void swap(BOOST_RV_REF_BEG
					boost::fusion::pair
					<
						boost::mpl::integral_c<bson_json_mode_def::mode_type, Mode>,
						boost::reference_wrapper<const TVal>
					>
				BOOST_RV_REF_END right)
	{
		typedef 
			boost::fusion::pair
			<
				boost::mpl::integral_c<bson_json_mode_def::mode_type, Mode>,
				boost::reference_wrapper<const TVal>
			> right_type;

		right_type& right_ref = right;
		this_type::swap(right_ref);
	}

	template<bson_json_mode_def::mode_type Mode> inline
	void swap(boost::fusion::pair
				<
					boost::mpl::integral_c<bson_json_mode_def::mode_type, Mode>,
					boost::reference_wrapper<const TVal>
				>& right)
	{
		::yggr::swap(base_type::second, right.second);
	}

	template<bson_json_mode_def::mode_type Mode> inline
	void swap(BOOST_RV_REF_BEG bson_visit_t<TVal, Mode> BOOST_RV_REF_END right)
	{
		typedef bson_visit_t<TVal, Mode> right_type;

		right_type& right_ref = right;
		this_type::swap(right_ref);
	}

	template<bson_json_mode_def::mode_type Mode> inline
	void swap(bson_visit_t<TVal, Mode>& right)
	{
		::yggr::swap(base_type::second, right.second);
	}

	inline void swap(BOOST_RV_REF(base_type) right)
	{
		base_type& right_ref = right;
		this_type::swap(right_ref);
	}

	inline void swap(base_type& right)
	{
		if(static_cast<base_type*>(this) == boost::addressof(right))
		{
			return;
		}

		::yggr::swap(base_type::second, right.second);
	}

	inline void swap(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		this_type::swap(right_ref);
	}

	inline void swap(this_type& right)
	{
		if(this == boost::addressof(right))
		{
			return;
		}

		::yggr::swap(base_type::second, right.second);
	}

public:
	YGGR_CONSTEXPR_OR_INLINE static mode_type mode(void)
	{
		return tag_mode_type::value;
	}

	inline value_cref_type value(void) const
	{
		return base_type::second.get();
	}
};

template<bson_json_mode_def::mode_type Mode, typename T> inline
bson_visit_t<T, Mode> make_bson_visit(const T& val)
{
	typedef bson_visit_t<T, Mode> ret_type;
	return ret_type(val);
}

namespace detail
{

template<typename T>
struct bson_operator_outter
{
	template<typename Char, typename Traits> inline
	std::basic_ostream<Char, Traits>&
		operator()(std::basic_ostream<Char, Traits>& os, 
					const T& right, 
					bson_json_mode_def::mode_type) const
	{
		YGGR_MPLEX_FXIED_ASSERT_FALSE();
		return os;
	}
};

} // namespace detail

// operator<<
template<typename Char, typename Traits, typename T, bson_json_mode_def::mode_type Mode> inline
std::basic_ostream<Char, Traits>&
	operator<<(std::basic_ostream<Char, Traits>& os, const bson_visit_t<T, Mode>& right)
{
	typedef detail::bson_operator_outter<T> outter_type;

	outter_type outter;
	return outter(os, right.value(), right.mode());
}

} // namespace nsql_database_system
} // namespace yggr

namespace yggr
{
namespace nsql_database_system
{
namespace swap_support
{

template<typename T, bson_json_mode_def::mode_type M1, bson_json_mode_def::mode_type M2> inline
void swap(boost::fusion::pair
			<
				boost::mpl::integral_c<bson_json_mode_def::mode_type, M1>,
				boost::reference_wrapper<const T>
			>& l, 
			bson_visit_t<T, M2>& r)
{
	r.swap(l);
}

template<typename T, bson_json_mode_def::mode_type M1, bson_json_mode_def::mode_type M2> inline
void swap(bson_visit_t<T, M1>& l, 
			boost::fusion::pair
			<
				boost::mpl::integral_c<bson_json_mode_def::mode_type, M2>,
				boost::reference_wrapper<const T>
			>& r)
{
	l.swap(r);
}

template<typename T, bson_json_mode_def::mode_type M1, bson_json_mode_def::mode_type M2> inline
void swap(bson_visit_t<T, M1>& l, bson_visit_t<T, M2>& r)
{
	l.swap(r);
}

} // namespace swap_support

using swap_support::swap;

} // namespace nsql_database_system
} // namespace yggr

namespace std
{
	using ::yggr::nsql_database_system::swap_support::swap;
} // namespace std

namespace boost
{
	using ::yggr::nsql_database_system::swap_support::swap;
} // namespace boost

#endif // __YGGR_NSQL_DATABASE_SYSTEM_BSON_VISIT_T_HPP__