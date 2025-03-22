//mongo_account_info.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_MONGO_ACCOUNT_INFO_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_MONGO_ACCOUNT_INFO_HPP__

#include <yggr/charset/string.hpp>
#include <yggr/smart_ptr_ex/shared_ptr.hpp>
#include <yggr/move/move.hpp>
#include <yggr/utility/copy_or_move_or_swap.hpp>

namespace yggr
{
namespace nsql_database_system
{

template<typename Conn, typename String = ::yggr::string>
class mongo_account_info
{
public:
	typedef Conn conn_type;
	typedef String string_type;

	typedef typename conn_type::init_type conn_init_type;
	typedef ::yggr::shared_ptr<conn_type> conn_ptr_type;


private:
	typedef mongo_account_info this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	mongo_account_info(void)
	{
	}

	explicit mongo_account_info(const conn_init_type& init)
		: _pconn(construct_shared<conn_type>(yggr_nothrow_new conn_type(init)))
	{
	}

	mongo_account_info(const conn_init_type& init, const string_type& uname, const string_type& pwd)
		: _pconn(construct_shared<conn_type>(yggr_nothrow_new conn_type(init))),
			_uname(uname),
			_pwd(pwd)
	{
	}

	explicit mongo_account_info(const conn_ptr_type& ptr)
		: _pconn(ptr)
	{
	}

	mongo_account_info(const conn_ptr_type& ptr, const string_type& uname, const string_type& pwd)
		: _pconn(construct_shared<conn_type>(yggr_nothrow_new conn_type(ptr))),
			_uname(uname),
			_pwd(pwd)
	{
	}

	mongo_account_info(BOOST_RV_REF(this_type) right)
		: _pconn(boost::move(right._pconn)),
			_uname(boost::move(right._uname)),
			_pwd(boost::move(right._pwd))
	{
	}

	mongo_account_info(const this_type& right)
		: _pconn(right._pconn),
			_uname(right._uname),
			_pwd(right._pwd)
	{
	}

	~mongo_account_info(void)
	{
	}

public:
	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		this_type& right_chk = right;
		if(this == &right_chk)
		{
			return *this;
		}

		copy_or_move_or_swap(_pconn, boost::move(right._pconn));
		copy_or_move_or_swap(_uname, boost::move(right._uname));
		copy_or_move_or_swap(_pwd, boost::move(right._pwd));

		return *this;
	}

	this_type& operator=(const this_type& right)
	{
		if(this == &right)
		{
			return *this;
		}

		_pconn = right._pconn;
		_uname = right._uname;
		_pwd = right._pwd;

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
		
		_pconn.swap(right._pconn);
		_uname.swap(right._uname);
		_pwd.swap(right._pwd);
	}

public:
	inline operator const conn_ptr_type&(void) const
	{
		return _pconn;
	}

	inline operator conn_ptr_type&(void)
	{
		return _pconn;
	}

	inline operator conn_type*(void) const
	{
		return _pconn.get();
	}

private:
	conn_ptr_type _pconn;
	string_type _uname;
	string_type _pwd;
};

} // namespace nsql_database_system
} // namespace yggr

namespace yggr
{
namespace nsql_database_system
{
namespace swap_support
{

template<typename Conn, typename String> inline
void swap(mongo_account_info<Conn, String>& l, mongo_account_info<Conn, String>& r)
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

#endif // __YGGR_NSQL_DATABASE_SYSTEM_MONGO_ACCOUNT_INFO_HPP__
