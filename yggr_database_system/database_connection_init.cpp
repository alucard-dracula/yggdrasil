//database_connection_init.cpp

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

#include <yggr/database_system/database_connection_init.hpp>
#include <yggr/utility/swap.hpp>

#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

namespace yggr
{
namespace database_system
{

database_connection_init::database_connection_init(void)
	: _bprompt(false), _bauto_commit(false)
{
}


database_connection_init::database_connection_init(const this_type& right)
	: _dsn(right._dsn), _bprompt(right._bprompt), _bauto_commit(right._bauto_commit)
{
}

database_connection_init::~database_connection_init(void)
{
}

database_connection_init::this_type& 
	database_connection_init::operator=(const this_type& right)
{
	if(this == &right)
	{
		return *this;
	}

	_dsn = right._dsn;
	_bprompt = right._bprompt;
	_bauto_commit = right._bauto_commit;

	return *this;
}

void database_connection_init::swap(this_type& right)
{
	if(this == &right)
	{
		return;
	}

	yggr::swap(_dsn, right._dsn);
	yggr::swap(_bprompt, right._bprompt);
	yggr::swap(_bauto_commit, right._bauto_commit);
}

} // database_system
} // namespace yggr


