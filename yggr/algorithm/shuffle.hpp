// shuffle.hpp

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

#ifndef __YGGR_ALGORITHM_SHUFFLE_HPP__
#define __YGGR_ALGORITHM_SHUFFLE_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/move/move.hpp>
#include <yggr/base/random.h>

#if !(defined(BOOST_NO_CXX98_RANDOM_SHUFFLE) || defined(BOOST_NO_CXX98_RANDOM_SHUFFLE)) \
		&& (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
#	include <algorithm>
#else
#	include <yggr/range_ex/range_difference_ex.hpp>
#	include <random>
#endif // BOOST_NO_CXX11_HDR_RANDOM

namespace yggr
{
namespace algorithm
{

#if !(defined(BOOST_NO_CXX98_RANDOM_SHUFFLE) || defined(BOOST_NO_CXX98_RANDOM_SHUFFLE)) \
		&& (YGGR_CPP_VERSION < YGGR_CPP_VER_17)

namespace detail
{

struct default_random_number_generator
{
public:
	template<typename Diff> inline
    Diff operator()(Diff n) const
    {
       return ::yggr::gen_random<Diff>(Diff(), n);
    }
};

} // namespace detail

template<typename Iter> inline
void shuffle(Iter s, Iter e)
{
	static detail::default_random_number_generator gen;
	::std::random_shuffle(s, e, gen);
}

template<typename Iter, typename Gen> inline
void shuffle(Iter s, Iter e, BOOST_FWD_REF(Gen) gen)
{
	::std::random_shuffle(s, e, gen);
}

#else

using std::shuffle;

template<typename Iter> inline
void shuffle(Iter s, Iter e)
{
	static std::random_device rdev;
    static std::mt19937 gen(rdev());
	::std::shuffle(s, e, gen);
}

#endif // BOOST_NO_CXX11_HDR_RANDOM

} // namespace algorithm

using algorithm::shuffle;

} // namespace yggr


#endif  // __YGGR_ALGORITHM_RANDOM_SHUFFLE_HPP__
