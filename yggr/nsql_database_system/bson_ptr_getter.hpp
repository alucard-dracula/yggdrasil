// bson_ptr_getter.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_C_BSON_PTR_GETTER_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_C_BSON_PTR_GETTER_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/nsql_database_system/c_bson.hpp>

#include <boost/ref.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_base_of.hpp>

#include <utility>

namespace yggr
{
namespace nsql_database_system
{

struct bson_ptr_getter
{
public:
	typedef c_bson bson_type;
	typedef bson_type::base_type base_bson_type;

public:
	template<typename Bson> inline
	typename boost::enable_if<boost::is_base_of<base_bson_type, Bson>, const base_bson_type*>::type
		operator()(const Bson* pbs) const
	{
		return pbs;
	}

	template<typename Bson> inline
	typename boost::enable_if<boost::is_base_of<base_bson_type, Bson>, base_bson_type*>::type
		operator()(Bson* pbs) const
	{
		return pbs;
	}

	template<typename Bson> inline
	typename boost::enable_if<boost::is_base_of<base_bson_type, Bson>, const base_bson_type*>::type
		operator()(const Bson& bs) const
	{
		return boost::addressof(bs);
	}

	template<typename Bson> inline
	typename boost::enable_if<boost::is_base_of<base_bson_type, Bson>, base_bson_type*>::type
		operator()(Bson& bs) const
	{
		return boost::addressof(bs);
	}

	template<typename Key, typename Bson,
				template<typename K, typename V> class Pair > inline
	typename boost::enable_if<boost::is_base_of<base_bson_type, Bson>, const base_bson_type*>::type
		operator()(const Pair<Key, Bson*>& val) const
	{
		return val.second;
	}

	template<typename Key, typename Bson,
				template<typename K, typename V> class Pair > inline
	typename boost::enable_if<boost::is_base_of<base_bson_type, Bson>, base_bson_type*>::type
		operator()(Pair<Key, Bson*>& val) const
	{
		return val.second;
	}

	template<typename Key, typename Bson,
				template<typename K, typename V> class Pair > inline
	typename boost::enable_if<boost::is_base_of<base_bson_type, Bson>, const base_bson_type*>::type
		operator()(const Pair<Key, Bson>& val) const
	{
		return boost::addressof(val.second);
	}

	template<typename Key, typename Bson,
				template<typename K, typename V> class Pair > inline
	typename boost::enable_if<boost::is_base_of<base_bson_type, Bson>, base_bson_type*>::type
		operator()(Pair<Key, Bson>& val) const
	{
		return boost::addressof(val.second);
	}
};

} // namespace nsql_database_system
} // namespace yggr

#endif // __YGGR_NSQL_DATABASE_SYSTEM_C_BSON_PTR_GETTER_HPP__
