//database_connect_st.hpp

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

#ifndef __YGGR_DATABASE_SYSTEM_DATABASE_CONNECT_ST_HPP__
#define __YGGR_DATABASE_SYSTEM_DATABASE_CONNECT_ST_HPP__

#ifndef YGGR_DATABASE_CONNECT
#	error "include error database_connect_st.hpp please input database_connect.hpp"
#endif //YGGR_DATABASE_CONNECT

namespace yggr
{
namespace database_system
{

template<typename Real_Conn>
class database_connect<Real_Conn, void> : public Real_Conn,  private nonable::noncopyable
{
private:
	typedef Real_Conn base_type;
	typedef std::string init_type;

	typedef database_connect<base_type> this_type;
public:
	database_connect(void)
	{
	}

	database_connect(const init_type& init)
		: base_type(init)
	{
	}

	~database_connect(void)
	{
	}

	operator base_type&(void)
	{
		return *this;
	}

	operator const base_type&(void) const
	{
		return *this;
	}

	template<typename Handler>
	bool use_handler(const Handler& handler)
	{
		base_type& base = *this;
		return handler(base);
	}

	template<typename Handler>
	bool use_handler(const Handler& handler) const
	{
		const base_type& base = *this;
		return handler(base);
	}
};

} // namespace database_system
} // namespace yggr

#endif //__YGGR_DATABASE_SYSTEM_DATABASE_CONNECT_ST_HPP__