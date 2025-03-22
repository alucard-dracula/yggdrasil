//random.h

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

#ifndef __YGGR_RANDOM_H__
#define __YGGR_RANDOM_H__

#include <yggr/base/yggrdef.h>
#include <yggr/mplex/limits.hpp>
#include <yggr/type_traits/upper_types.hpp>
#include <yggr/nonable/noncreateable.hpp>

#include <boost/random/linear_congruential.hpp>
#include <boost/random/uniform_int.hpp>
#include <boost/random/uniform_real.hpp>
#include <boost/random/variate_generator.hpp>
#include <boost/type_traits/is_floating_point.hpp>

#ifdef _MSC_VER
#	pragma warning (push)
#	pragma warning (disable : 4624)
#endif //_MSC_VER

namespace yggr
{

namespace detail
{

template<typename T, bool is_float = boost::is_floating_point<T>::value>
class genner;

} // namespace detail

class random;

} // namespace yggr

namespace yggr
{

class random
	: private nonable::noncreateable
{
	typedef boost::minstd_rand base_generator_type;
	typedef random this_type;

private:
	template<typename T, bool is_float>
	friend class detail::genner;

public:
	static void init(void);

	template<typename T> inline
	static T gen_random(void)
	{
		typedef detail::genner<T> genner_type;
		static genner_type gen;
		return gen();
	}


	template<typename T> inline
	static T gen_random(const T& s_range, const T& e_range)
	{
		typedef detail::genner<T> genner_type;
		static genner_type gen;
		return gen(s_range, e_range);
	}

private:
	static base_generator_type _s_base_gen;
};

template<typename T> inline
T gen_random(void)
{
	return random::gen_random<T>();
}

template<typename T> inline
T gen_random(const T& s_range, const T& e_range)
{
	return random::gen_random(s_range, e_range);
}

} // namesapce yggr

namespace yggr
{
namespace detail
{

template<typename T>
class genner<T, true>
{
public:
	inline T operator()(void) const
	{
		typedef T val_type;
		typedef boost::uniform_real<val_type> uni_dist_type;
		typedef mplex::numeric_limits<val_type> limits_type;
		typedef boost::variate_generator<random::base_generator_type, uni_dist_type> gen_type;

		static uni_dist_type uni_dist(limits_type::min_type::value, limits_type::max_type::value);
		static gen_type gen(random::_s_base_gen, uni_dist);

		return gen();
	}

	inline T operator()(const T& s_range, const T& e_range) const
	{
		typedef T val_type;
		typedef boost::uniform_real<val_type> uni_dist_type;
		typedef boost::variate_generator<random::base_generator_type, uni_dist_type> gen_type;

		uni_dist_type uni_dist(s_range, e_range);
		gen_type gen(random::_s_base_gen, uni_dist);
		return gen();
	}
};

template<typename T>
class genner<T, false>
{
private:
	inline T operator()(const T& n) const
	{
		typedef T val_type;
		typedef boost::uniform_int<val_type> uni_dist_type;
		typedef mplex::numeric_limits<val_type> limits_type;
		typedef boost::variate_generator<random::base_generator_type, uni_dist_type> gen_type;

		static uni_dist_type uni_dist(limits_type::min_type::value, limits_type::max_type::value);
		static gen_type gen(random::_s_base_gen, uni_dist);

		return gen(n);
	}

public:
	inline T operator()(const T& s_range, const T& e_range) const
	{
		typedef T val_type;
		typedef mplex::numeric_limits<val_type> limits_type;

		assert((s_range >= limits_type::min_type::value)
				&& (e_range <= limits_type::max_type::value)
				&& (s_range < e_range));
		return s_range + operator()(e_range - s_range);
	}

	inline T operator()(void) const
	{
		typedef T val_type;
		typedef mplex::numeric_limits<val_type> limits_type;
		return operator()(limits_type::max_type::value);
	}
};

} // namespace detail
} // namespace yggr

#ifdef _MSC_VER
#	pragma warning ( pop )
#endif //_MSC_VER

#endif // __YGGR_RANDOM_H__
