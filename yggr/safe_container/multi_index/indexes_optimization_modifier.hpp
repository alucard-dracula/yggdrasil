// indexes_optimization_modifier.hpp

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

#ifndef __YGGR_SAFE_CONTAINER_MULTI_INDEX_INDEXES_OPTIMIZATION_MODIFIER_HPP__
#define __YGGR_SAFE_CONTAINER_MULTI_INDEX_INDEXES_OPTIMIZATION_MODIFIER_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/bind/bind.hpp>
#include <yggr/utility/copy_or_move_or_swap.hpp>

#ifndef YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION
#	include <yggr/safe_container/detail/boost_midx_container_opt_decl.hpp>
#endif // YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION

#include <boost/type_traits/remove_cv.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/function.hpp>

namespace yggr
{
namespace safe_container
{

namespace multi_index_detail
{

template<typename KeyInfoHandler, 
			typename ModifyHandler>
class optimization_modifier
{
private:
	typedef optimization_modifier this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	typedef KeyInfoHandler keyinfo_handler_type;
	typedef ModifyHandler modify_handler_type;

#ifndef YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION
	typedef typename 
		boost::remove_reference
		<
			typename
				boost::remove_cv
				<
					typename keyinfo_handler_type::result_type
				>::type
		>::type keyinfo_params_type;
#	else
	typedef keyinfo_handler_type keyinfo_params_type;
#endif // YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION

public:
	optimization_modifier(const keyinfo_handler_type& keyinfo_handler,
							const modify_handler_type& modify_handler)
		: _keyinfo_handler(keyinfo_handler),
			_modify_handler(modify_handler)
	{
	}

	optimization_modifier(BOOST_RV_REF(this_type) right)
		: _keyinfo_handler(boost::move(right._keyinfo_handler)),
			_modify_handler(boost::move(right._modify_handler))
	{
	}

	optimization_modifier(const this_type& right)
		: _keyinfo_handler(right._keyinfo_handler),
			_modify_handler(right._modify_handler)
	{
	}

	~optimization_modifier(void)
	{
	}

public:
	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		if(this == &right_ref)
		{
			return *this;
		}

		_keyinfo_handler = boost::move(right._keyinfo_handler);
		_modify_handler = boost::move(right._modify_handler);

		return *this;
	}

	this_type& operator=(const this_type& right)
	{
		if(this == &right)
		{
			return *this;
		}

		_keyinfo_handler = right._keyinfo_handler;
		_modify_handler = right._modify_handler;

		return *this;
	}

public:
	inline void swap(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		this_type::swap(right_ref);
	}

	void swap(this_type& right)
	{
		if(this == &right)
		{
			return;
		}

		::yggr::swap(_keyinfo_handler, right._keyinfo_handler);
		::yggr::swap(_modify_handler, right._modify_handler);
	}

#ifndef YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION

	template<typename Container> inline
	keyinfo_params_type keyinfo_params(Container& cont) const
	{
		return _keyinfo_handler(cont);
	}

#else

	template<typename Container> inline
	const keyinfo_params_type& keyinfo_params(Container& cont) const
	{
		return _keyinfo_handler;
	}

#endif // YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION

	template<typename T> inline
	void operator()(T& val) const
	{
		_modify_handler(val);
	}

private:
	keyinfo_handler_type _keyinfo_handler;
	modify_handler_type _modify_handler;
};

} // namespace multi_index_detail

using multi_index_detail::optimization_modifier;

#ifndef YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION

template<typename Container, typename Modify_Handler> inline
optimization_modifier
	<
		boost::function1
		<
			typename
				detail::cast_to_midx_container_opt
				<
					Container
				>::type::keyinfo_params_type,
			const Container&
		>, 
		Modify_Handler>
	make_optimization_multi_index_modifier(const typename Container::value_type& val, 
											const Modify_Handler& modify_handler)
{
	typedef Container cont_type;
	typedef Modify_Handler modify_handler_type;
	typedef typename detail::cast_to_midx_container_opt<cont_type>::type opt_cont_type;
	typedef typename opt_cont_type::keyinfo_params_type keyinfo_params_type;
	typedef boost::function1<keyinfo_params_type, const cont_type&> keyinfo_handler_type;
	typedef optimization_modifier<keyinfo_handler_type, modify_handler_type> result_type;
	return result_type(
				keyinfo_handler_type(
					boost::bind(&opt_cont_type::s_gen_opt_keyinfo_params, _1, boost::cref(val))), 
					modify_handler);
}

#else

template<typename Container, typename Modify_Handler> inline
optimization_modifier<const typename Container::value_type&, Modify_Handler>
	make_optimization_multi_index_modifier(const typename Container::value_type& val,
											const Modify_Handler& modify_handler)
{
	typedef Container cont_type;
	typedef Modify_Handler modify_handler_type;
	typedef typename cont_type::value_type value_type;
	typedef optimization_modifier<const value_type&, modify_handler_type> result_type;
	return result_type(val, modify_handler);
}

#endif // YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION

template<typename Container, typename KeyInfo_Handler, typename Modify_Handler> inline
optimization_modifier<KeyInfo_Handler, Modify_Handler>
	make_optimization_multi_index_modifier(const KeyInfo_Handler& keyinfo_handler,
											const Modify_Handler& modify_handler)
{
	typedef optimization_modifier<KeyInfo_Handler, Modify_Handler> result_type;
	return result_type(keyinfo_handler, modify_handler);
}

} // namespace safe_container
} // namespace yggr

namespace yggr
{
namespace safe_container
{
namespace multi_index_detail
{
namespace swap_support
{

template<typename KeyInfoHandler, typename ModifyHandler> inline
void swap(optimization_modifier<KeyInfoHandler, ModifyHandler>& l,
			optimization_modifier<KeyInfoHandler, ModifyHandler>& r)
{
	l.swap(r);
}

} // namespace swap_support

using swap_support::swap;

} // namespace multi_index_detail
} // namespace safe_container
} // namespace yggr

namespace std
{
	using ::yggr::safe_container::multi_index_detail::swap_support::swap;
} // namespace std

namespace boost
{
	using ::yggr::safe_container::multi_index_detail::swap_support::swap;
} // namespace boost

#endif // __YGGR_SAFE_CONTAINER_MULTI_INDEX_INDEXES_OPTIMIZATION_MODIFIER_HPP__
