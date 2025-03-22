//base_ids_def.hpp

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

#ifndef __YGGR_IDS_BASE_IDS_DEF_HPP__
#define __YGGR_IDS_BASE_IDS_DEF_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/mplex/static_assert.hpp>
#include <yggr/ids/id_generator.hpp>

#include <boost/atomic.hpp>
#include <boost/mpl/bool.hpp>

namespace yggr
{
namespace ids
{

typedef s64 sid64_type;
typedef u64 id64_type;

typedef size_type address_id_type;


template<>
class id_generator<id64_type, void> 
	: private nonable::noncopyable
{
public:
	typedef id64_type id_type;

private:
	typedef id_generator this_type;
public:

	id_type operator()(void) const
	{
		typedef mplex::numeric_limits<u16> limits_type;

		static boost::atomic<u32> fuzz_incr((random::gen_random<u32>()) & 0xffff0000);

		u32 t_fuzz_incr = 0;
		u32 t_fuzz = 0;
		u32 t_incr = 0;
		u32 re_fuzz_incr = 0;

		do
		{
			t_fuzz_incr = fuzz_incr.load();
			t_fuzz = t_fuzz_incr & 0xffff0000;
			t_incr = t_fuzz_incr & 0x0000ffff;

			if(!(t_incr = (t_incr + 1) % limits_type::max_type::value))
			{
				t_fuzz = (random::gen_random<u32>()) & 0xffff0000;
			}

			re_fuzz_incr = t_fuzz | t_incr;
		} while(!fuzz_incr.compare_exchange_weak(t_fuzz_incr, re_fuzz_incr));

		id_type use_fuzz_incr = static_cast<id_type>(t_fuzz_incr);
		id_type id(std::time(0));
		id <<= 32;
		id |= use_fuzz_incr;

		return id;
	}
};

template<>
class id_generator<sid64_type, void> 
	: private nonable::noncopyable
{
public:
	typedef sid64_type id_type;

private:
	typedef id64_type val_type;

private:
	BOOST_MPL_ASSERT((boost::mpl::bool_<(sizeof(id_type) == sizeof(val_type))>));

public:

	inline id_type operator()(void) const
	{
		id_generator<val_type> gen;
		val_type tid = gen();
		return *(reinterpret_cast<id_type*>(&tid));
	}
};

} // namespace ids
} // namespace yggr

#endif // __YGGR_IDS_BASE_IDS_DEF_HPP__
