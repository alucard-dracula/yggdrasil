//random.h

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

#ifndef __YGGR_RANDOM_H__
#define __YGGR_RANDOM_H__

#ifdef _MSC_VER
#	include <yggr/support/max_min_undef.ipp>
#endif //_MSC_VER

#include <limits>
#include <algorithm>
#include <yggr/base/yggrdef.h>

#include <boost/random/linear_congruential.hpp>
#include <boost/random/uniform_int.hpp>
#include <boost/random/uniform_real.hpp>
#include <boost/random/variate_generator.hpp>
#include <boost/type_traits/is_floating_point.hpp>

#include <yggr/type_traits/upper_types.hpp>
#include <yggr/nonable/noncreateable.hpp>

#ifdef _MSC_VER
#	pragma warning (push)
#	pragma warning (disable : 4624)
#endif //_MSC_VER

namespace yggr
{

class random
	: private nonable::noncreateable
{
	typedef boost::minstd_rand base_generator_type;
	typedef random this_type;

private:
	template<bool is_float, typename T>
	struct genner;

	template<typename T>
	struct genner<true, T>
	{
		T operator()(void) const
		{
			typedef T val_type;
			typedef boost::uniform_real<val_type> uni_dist_type;
			typedef std::numeric_limits<val_type> limits_type;
			typedef boost::variate_generator<base_generator_type, uni_dist_type> gen_type;

			static uni_dist_type uni_dist(limits_type::min(), limits_type::max());
			static gen_type gen(this_type::_s_base_gen, uni_dist);

			return gen();
		}

		T operator()(const T& s_range, const T& e_range) const
		{
			typedef T val_type;
			typedef boost::uniform_real<val_type> uni_dist_type;
			typedef std::numeric_limits<val_type> limits_type;
			typedef boost::variate_generator<base_generator_type, uni_dist_type> gen_type;

			uni_dist_type uni_dist(s_range, e_range);
			return uni_dist(_s_base_gen);
		}
	};

	template<typename T>
	struct genner<false, T>
	{
	private:
		T operator()(const T& n) const
		{
			typedef T val_type;
			typedef boost::uniform_int<val_type> uni_dist_type;
			typedef std::numeric_limits<val_type> limits_type;
			typedef boost::variate_generator<base_generator_type, uni_dist_type> gen_type;

			static uni_dist_type uni_dist(limits_type::min(), limits_type::max());
			static gen_type gen(this_type::_s_base_gen, uni_dist);

			return gen(n);
		}

	public:
		T operator()(const T& s_range, const T& e_range) const
		{
			typedef T val_type;
			typedef std::numeric_limits<val_type> limits_type;

			assert((s_range >= limits_type::min())
					&& (e_range <= limits_type::max())
					&& (s_range < e_range));
			return s_range + operator()(e_range - s_range);
		}

		T operator()(void) const
		{
			typedef T val_type;
			typedef std::numeric_limits<val_type> limits_type;
			return operator()(limits_type::max());
		}
	};

public:
	static void init(void);
	template<typename T>
	static const T gen_random(void)
	{
		typedef genner<(boost::is_floating_point<T>::value), T> genner_type;
		genner_type gen;
		return gen();
	}


	template<typename T>
	static const T gen_random(const T& s_range, const T& e_range)
	{
		typedef genner<(boost::is_floating_point<T>::value), T> genner_type;
		genner_type gen;
		return gen(s_range, e_range);
	}

private:
	static base_generator_type _s_base_gen;
};


template<typename Iter>
void random_shuffle(Iter s, Iter e)
{
	std::random_shuffle(s, e);
}

template<typename Iter, typename Gen>
void random_shuffle(Iter s, Iter e, Gen& gen)
{
	std::random_shuffle(s, e, gen);
}

template<typename Iter, typename Gen>
void random_shuffle(Iter s, Iter e, const Gen& gen)
{
	Gen tgen(gen);
	std::random_shuffle(s, e, tgen);
}

} // namesapce yggr

#ifdef _MSC_VER
#	pragma warning ( pop )
#endif //_MSC_VER

#endif // __YGGR_RANDOM_H__
