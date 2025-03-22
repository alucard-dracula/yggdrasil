//p2p_ekeys_generator.hpp

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

#ifndef __YGGR_P2P_P2P_EKEYS_GENERATOR_HPP__
#define __YGGR_P2P_P2P_EKEYS_GENERATOR_HPP__

#include <yggr/ids/id_generator.hpp>
#include <utility>

namespace yggr
{
namespace p2p
{

template<typename EKeys>
class p2p_ekeys_generator
{
public:
	typedef EKeys ekeys_type;
	typedef yggr::ids::id_generator<ekeys_type> gen_type;
	typedef std::pair<ekeys_type, ekeys_type> result_type;

private:
	typedef p2p_ekeys_generator this_type;

public:
	inline void operator()(ekeys_type& src, ekeys_type& dst) const
	{
		this_type::pro_s_gen(src, dst);
	}

	inline result_type& operator()(result_type& rst) const
	{
		this_type::pro_s_gen(rst.first, rst.second);
		return rst;
	}

	inline result_type operator()(void) const
	{
		result_type rst;
		this_type::pro_s_gen(rst.first, rst.second);
		return rst;
	}

protected:
	inline static void pro_s_gen(ekeys_type& src, ekeys_type& dst)
	{
		gen_type gen;
		src = dst = gen();
	}

};

} // namespace p2p
} // namespace yggr

#endif // __YGGR_P2P_P2P_EKEYS_GENERATOR_HPP__
