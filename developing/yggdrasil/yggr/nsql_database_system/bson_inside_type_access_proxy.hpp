//bson_inside_type_access_proxy.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_BSON_INSIDE_TYPE_ACCESS_PROXY_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_BSON_INSIDE_TYPE_ACCESS_PROXY_HPP__

#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/if.hpp>
#include <yggr/nonable/noncreateable.hpp>
#include <yggr/nsql_database_system/bson_type_traits.hpp>

#ifdef _MSC_VER
#	pragma warning (push)
#	pragma warning (disable : 4624)
#endif //_MSC_VER

namespace yggr
{
namespace nsql_database_system
{

class bson_inside_type_access_proxy
    : private nonable::noncreateable
{
public:

//	template<bool is_inside, typename NIL = int>
//	struct access
//	{
//	public:
//		// now proxy
//		BOOST_MPL_ASSERT(( boost::is_same< boost::mpl::bool_<true>, boost::mpl::bool_<is_inside> > ));
//	};

    template<bool is_inside, typename NIL = int>
	struct access;

//private:
    template<typename NIL>
	struct access<false, NIL>
	{
	public:
		// now proxy
		//BOOST_MPL_ASSERT(( boost::mpl::bool_<false> ));
	};

	template<typename NIL>
	struct access<true, NIL>
	{
	public:
		access(void) {}
		~access(void){}

		template<typename Bson, typename T>
		bool save(Bson& bs, const std::string& name, const T& val) const
		{
			return val.save(bs, name);
		}

		template<typename Bson, typename T>
		bool save_value(Bson& bs, const T& val) const
		{
			return val.save_value(bs);
		}

		template<typename Bson_Iter, typename T>
		void load(const Bson_Iter& iter, const std::string& name, T& val) const
		{
			val.load(iter, name);
		}

		template<typename Bson_Iter, typename T>
		void load_value(const Bson_Iter& iter, T& val) const
		{
			val.load_value(iter);
		}
	};

private:
	typedef bson_inside_type_access_proxy this_type;

public:
	template<typename Bson, typename T>
	static bool save(Bson& bs, const std::string& name, const T& val)
	{
		typedef this_type::access<is_bson_inside_type<T>::value> access_type;
		access_type ass;
		return ass.save(bs, name, val);
	}

	template<typename Bson, typename T>
	static bool save_value(Bson& bs, const T& val)
	{
		typedef this_type::access<is_bson_inside_type<T>::value> access_type;
		access_type ass;
		return ass.save_value(bs, val);
	}

	template<typename Bson_Iter, typename T>
	static void load(const Bson_Iter& iter, const std::string& name, T& val)
	{
		typedef this_type::access<is_bson_inside_type<T>::value> access_type;
		access_type ass;
		ass.load(iter, name, val);
	}

	template<typename Bson_Iter, typename T>
	static void load_value(const Bson_Iter& iter, T& val)
	{
		typedef this_type::access<is_bson_inside_type<T>::value> access_type;
		access_type ass;
		ass.load_value(iter, val);
	}
};

} // namespace nsql_database_system
} // namespace yggr

#ifdef _MSC_VER
#	pragma warning (pop)
#endif //_MSC_VER

#endif // __YGGR_NSQL_DATABASE_SYSTEM_BSON_INSIDE_TYPE_ACCESS_PROXY_HPP__
