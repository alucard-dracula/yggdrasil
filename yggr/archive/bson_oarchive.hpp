// bson_oarchive.hpp

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

#ifndef __YGGR_ARCHIVE_BSON_OARCHIVE_HPP__
#define __YGGR_ARCHIVE_BSON_OARCHIVE_HPP__

#ifndef YGGR_HAS_BSON_OARCHIVE
#   define YGGR_HAS_BSON_OARCHIVE
#else
#   error "YGGR_HAS_BSON_OARCHIVE be defined"
#endif // YGGR_HAS_BSON_OARCHIVE

#include <yggr/base/yggrdef.h>

#ifndef BOOST_CONFIG_HPP
#  include <boost/config.hpp>
#endif // BOOST_CONFIG_HPP

#include  <boost/type_traits/remove_reference.hpp>
#include  <boost/type_traits/remove_const.hpp>

#include <yggr/ppex/friend.hpp>

#include <boost/archive/detail/auto_link_archive.hpp>
#include <yggr/serialization/nvp.hpp>

#include <yggr/archive/impl/archive_serializer_map.hpp>
#include <yggr/archive/basic_bson_oarchive.hpp>

#include <boost/archive/detail/register_archive.hpp>
#include <boost/serialization/item_version_type.hpp>

#include <yggr/serialization/collection_size_type.hpp>
#include <yggr/serialization/bson_inner_data.hpp>
#include <yggr/archive/archive_data_wrap_traits.hpp>

#include <yggr/ppex/friend.hpp>

#include <sstream>

#include <boost/archive/detail/abi_prefix.hpp> // must be the last header

#ifdef BOOST_MSVC
#  pragma warning(push)
#  pragma warning(disable : 4511 4512)
#endif // BOOST_MSVC

namespace yggr
{
namespace archive
{

template<typename Archive>
class bson_oarchive_impl
	: public basic_bson_oarchive<Archive>
{
public:
	typedef basic_bson_oarchive<Archive> base_type;

private:
	typedef nsql_database_system::c_bson buf_type;
	typedef nsql_database_system::c_bson_access_proxy proxy_type;
	typedef bson_oarchive_impl this_type;

#ifdef BOOST_NO_MEMBER_TEMPLATE_FRIENDS
public:
#else
	YGGR_PP_FRIEND_TYPENAME(boost::archive::detail::interface_oarchive<Archive>);
	YGGR_PP_FRIEND_TYPENAME(base_type);
	friend class boost::archive::save_access;
protected:
#endif // BOOST_NO_MEMBER_TEMPLATE_FRIENDS

	bson_oarchive_impl(buf_type& buf, unsigned int flags)
		: base_type(buf, flags)
	{
	}

	~bson_oarchive_impl(void)
	{
	}

	template<typename T> inline
	void save(const T& t) // inside type use this
	{
		BOOST_MPL_ASSERT(( nsql_database_system::is_bson_inside_type<T>));
		proxy_type::save_value(base_type::_buf, t);
	}

	inline void save(const boost::serialization::collection_size_type& t) //u64
	{
		if(Archive::is_nld())
		{
			return;
		}
		BOOST_MPL_ASSERT(( nsql_database_system::is_bson_inside_type<
								boost::serialization::collection_size_type>));
		const std::size_t tmp1 = t;
		ser_size_type tmp = static_cast<const ser_size_type>(tmp1);
		proxy_type::save_value(base_type::_buf, tmp);
	}

	inline void save(const yggr::serialization::collection_size_type& t) //u64
	{
		if(Archive::is_nld())
		{
			return;
		}
		BOOST_MPL_ASSERT(( nsql_database_system::is_bson_inside_type<
								yggr::serialization::collection_size_type>));
		const ser_size_type tmp = t;
		proxy_type::save_value(base_type::_buf, tmp);
	}

	inline void save(const boost::serialization::version_type& t) //unsigned int u32
	{
		if(Archive::is_nld())
		{
			return;
		}
		BOOST_MPL_ASSERT(( nsql_database_system::is_bson_inside_type<
									boost::serialization::version_type>));
		const u32 tmp = t;
		proxy_type::save_value(base_type::_buf, tmp);
	}

	inline void save(const boost::serialization::item_version_type& t) //u32
	{
		if(Archive::is_nld())
		{
			return;
		}
		BOOST_MPL_ASSERT(( nsql_database_system::is_bson_inside_type<
								boost::serialization::item_version_type>));
		const u32 tmp = t;
		proxy_type::save_value(base_type::_buf, tmp);
	}

public:
    using boost::archive::detail::interface_oarchive<Archive>::operator<<;

	struct use_array_optimization
	{
		template <typename T>
		#if defined(BOOST_NO_DEPENDENT_NESTED_DERIVATIONS)
			struct apply
			{
				typedef boost::mpl::bool_<false> type;
			};
		#else
			struct apply : public boost::mpl::bool_<false> {};
		#endif
	};
};

template<bool nld>
class bson_oarchive
	: public bson_oarchive_impl< bson_oarchive<nld> >
{
public:
	typedef yggr::nsql_database_system::c_bson buf_type;

	typedef bson_oarchive_impl< bson_oarchive<nld> > base_type;
private:
	typedef bson_oarchive this_type;

public:

	bson_oarchive(buf_type& buf, unsigned int flags = 0)
		:base_type(buf, flags)
	{
	}

	~bson_oarchive(void)
	{
	}

	inline static bool is_nld(void)
	{
		return nld;
	}
};

template<bool nld>
class naked_bson_oarchive
	: public bson_oarchive_impl< naked_bson_oarchive<nld> >
{
public:
	typedef yggr::nsql_database_system::c_bson buf_type;

	typedef bson_oarchive_impl< naked_bson_oarchive<nld> > base_type;
private:
	typedef naked_bson_oarchive this_type;

public:

	naked_bson_oarchive(buf_type& buf, unsigned int flags = 0)
		:base_type(buf, flags)
	{
	}

	~naked_bson_oarchive(void)
	{
	}

	inline static bool is_nld(void)
	{
		return nld;
	}
};

} // namespace archive
} // namespace boost

// required by export
BOOST_SERIALIZATION_REGISTER_ARCHIVE(yggr::archive::bson_oarchive<true>)
BOOST_SERIALIZATION_REGISTER_ARCHIVE(yggr::archive::bson_oarchive<false>)

//YGGR_PP_ARCHIVE_BINARY_DATA_WRAP_TRAITS_DEF(bson_oarchive<true>, yggr::nsql_database_system::c_bson_u8_binary_buffer)
//YGGR_PP_ARCHIVE_BINARY_DATA_WRAP_TRAITS_DEF(bson_oarchive<false>, yggr::nsql_database_system::c_bson_u8_binary_buffer)
//
//YGGR_PP_ARCHIVE_BINARY_DATA_WRAP_TRAITS_DEF(naked_bson_oarchive<true>, yggr::nsql_database_system::c_bson_u8_binary_buffer)
//YGGR_PP_ARCHIVE_BINARY_DATA_WRAP_TRAITS_DEF(naked_bson_oarchive<false>, yggr::nsql_database_system::c_bson_u8_binary_buffer)

namespace yggr
{
namespace archive
{

template<bool nld, typename T>
struct binary_data_wrap_traits<bson_oarchive<nld>, T>
	: public boost::mpl::true_
{
	typedef 
		yggr::nsql_database_system::c_bson_binary_buffer_ref
		<
			yggr::nsql_database_system::bson_binary_typeid_def::E_BSON_BIN_TYPE_U8_BUFFER,
			T
		> wrap_type;
};

template<bool nld, typename T>
struct binary_data_wrap_traits<naked_bson_oarchive<nld>, T>
	: public boost::mpl::true_
{
	typedef 
		yggr::nsql_database_system::c_bson_binary_buffer_ref
		<
			yggr::nsql_database_system::bson_binary_typeid_def::E_BSON_BIN_TYPE_U8_BUFFER,
			T
		> wrap_type;
};

} // namespace archive
} // namespace yggr

#ifdef BOOST_MSVC
#	pragma warning(pop)
#endif // BOOST_MSVC

#include <boost/archive/detail/abi_suffix.hpp> // pops abi_suffix.hpp pragmas

// boost::archive::detail::save_non_pointer_type patch
#include <yggr/archive/detail/oserializer_patch.hpp>

namespace boost
{
namespace archive
{ 
namespace detail 
{

template< bool nld > 
struct save_non_pointer_type< ::yggr::archive::bson_oarchive<nld> >
	: public save_non_pointer_patch_type< ::yggr::archive::bson_oarchive<nld> >
{
};

template< bool nld > 
struct save_non_pointer_type< ::yggr::archive::naked_bson_oarchive<nld> >
	: public save_non_pointer_patch_type< ::yggr::archive::naked_bson_oarchive<nld> >
{
};


} // namespace detail
} // namespace archive
} // namespace boost


#include <yggr/serialization/bson_inner_data.hpp>

#include <yggr/archive/impl/oserializer_bson_impl.hpp>
#include <yggr/serialization/detail/bson_impl_reinclue.hpp>

#endif // __YGGR_ARCHIVE_BSON_OARCHIVE_HPP__
