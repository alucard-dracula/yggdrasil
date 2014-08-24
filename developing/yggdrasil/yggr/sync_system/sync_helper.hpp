//sync_helper.hpp

/****************************************************************************
Copyright (c) 2014-2018 yggdrasil

author: yang xu

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

#ifndef __YGGR_SYNC_SYSTEM_SYNC_HELPER_HPP__
#define __YGGR_SYNC_SYSTEM_SYNC_HELPER_HPP__

#include <utility>
#include <boost/utility/swap.hpp>
#include <yggr/move/move.hpp>

namespace yggr
{
namespace sync_system
{

template<typename Params>
class sync_helper 
	: private std::pair<Params, Params>
{
public:
	typedef Params params_type;

private:
	typedef std::pair<params_type, params_type> base_type;
	typedef sync_helper this_type;
	
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	sync_helper(void)
	{
	}

	template<typename First>
	sync_helper(const First& f)
	{
		f >> *this;
	}

	template<typename First>
	sync_helper(BOOST_RV_REF(First) f)
	{
		f >> *this;
	}

	template<typename First, typename Second>
	sync_helper(const First& f, const Second& s)
	{
		f >> *this;
		s >> *this;
	}

	template<typename First, typename Second>
	sync_helper(BOOST_RV_REF(First) f, const Second& s)
	{
		f >> *this;
		s >> *this;
	}

	template<typename First, typename Second>
	sync_helper(const First& f, BOOST_RV_REF(Second) s)
	{
		f >> *this;
		s >> *this;
	}

	template<typename First, typename Second>
	sync_helper(BOOST_RV_REF(First) f, BOOST_RV_REF(Second) s)
	{
		f >> *this;
		s >> *this;
	}

	template<typename First>
	sync_helper(const First& f, const params_type& s)
	{
		f >> *this;
		base_type::second.copy_from(s);
	}

	template<typename First>
	sync_helper(const First& f, BOOST_RV_REF(params_type) s)
	{
		f >> *this;
		base_type::second.copy_from(s);
	}

	template<typename First>
	sync_helper(BOOST_RV_REF(First) f, const params_type& s)
	{
		f >> *this;
		base_type::second.copy_from(s);
	}

	template<typename First>
	sync_helper(BOOST_RV_REF(First) f, BOOST_RV_REF(params_type) s)
	{
		f >> *this;
		base_type::second.copy_from(s);
	}

	template<typename Second>
	sync_helper(const params_type& f, const Second& s)
	{
		base_type::first.copy_from(f);
		s >> *this;
	}

	template<typename Second>
	sync_helper(BOOST_RV_REF(params_type) f, const Second& s)
	{
		base_type::first.copy_from(f);
		s >> *this;
	}

	template<typename Second>
	sync_helper(const params_type& f, BOOST_RV_REF(Second) s)
	{
		base_type::first.copy_from(f);
		s >> *this;
	}

	template<typename Second>
	sync_helper(BOOST_RV_REF(params_type) f, BOOST_RV_REF(Second) s)
	{
		base_type::first.copy_from(f);
		s >> *this;
	}

	sync_helper(const params_type& f, const params_type& s)
	{
		base_type::first.copy_from(f);
		base_type::second.copy_from(s);
	}

	sync_helper(BOOST_RV_REF(params_type) f, const params_type& s)
	{
		base_type::first.copy_from(f);
		base_type::second.copy_from(s);
	}

	sync_helper(const params_type& f, BOOST_RV_REF(params_type) s)
	{
		base_type::first.copy_from(f);
		base_type::second.copy_from(s);
	}

	sync_helper(BOOST_RV_REF(params_type) f, BOOST_RV_REF(params_type) s)
	{
		base_type::first.copy_from(f);
		base_type::second.copy_from(s);
	}
	
	sync_helper(const this_type& right)
	{
		base_type::first.copy_from(right.first);
		base_type::second.copy_from(right.second);
	}

	sync_helper(BOOST_RV_REF(this_type) right)
	{
		base_type::first.copy_from(boost::forward<base_type::first_type>(right.first));
		base_type::second.copy_from(boost::forward<base_type::second_type>(right.second));
	}

	~sync_helper(void)
	{
	}

	this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		base_type::first.copy_from(boost::forward<base_type::first_type>(right.first));
		base_type::second.copy_from(boost::forward<base_type::second_type>(right.second));
		return *this;
	}

	this_type& operator=(const this_type& right)
	{
		base_type::first = right.first;
		base_type::second = right.second;
		return *this;
	}

	params_type& condition(void)
	{
		return base_type::first;
	}

	const params_type& condition(void) const
	{
		return base_type::first;
	}

	params_type& result(void)
	{
		return base_type::second;
	}

	const params_type& result (void) const
	{
		return base_type::second;
	}

	template<typename First>
	bool recv(const First& f)
	{
		return f >> *this;
	}

	template<typename First>
	bool recv(First& f)
	{
		return f >> *this;
	}
};

} // namespace sync_system
} // namespace yggr

#endif //__YGGR_SYNC_SYSTEM_SYNC_HELPER_HPP__