// bson_iarchive.hpp
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

#ifndef __YGGR_ARCHIVE_BSON_IARCHIVE_HPP__
#define __YGGR_ARCHIVE_BSON_IARCHIVE_HPP__

#ifndef YGGR_HAS_BSON_IARCHIVE
#	define YGGR_HAS_BSON_IARCHIVE
#else
#	error "YGGR_HAS_BSON_IARCHIVE be defined" 
#endif // YGGR_HAS_BSON_IARCHIVE

#include <boost/archive/detail/auto_link_archive.hpp>
#include <yggr/archive/basic_bson_iarchive.hpp>
#include <boost/archive/detail/register_archive.hpp>
#include <boost/serialization/item_version_type.hpp>
#include <yggr/serialization/collection_size_type.hpp>
#include <yggr/serialization/bson_data_type.hpp>
#include <yggr/archive/archive_data_wrap_traits.hpp>

#include <boost/archive/detail/abi_prefix.hpp> // must be the last header

#ifdef BOOST_MSVC
#  pragma warning(push)
#  pragma warning(disable : 4511 4512)
#endif

namespace yggr
{
namespace archive
{

template<class Archive>
class bson_iarchive_impl
	: public basic_bson_iarchive<Archive>
{
public:
	typedef basic_bson_iarchive<Archive> base_type;

private:
	typedef nsql_database_system::c_bson buf_type;
	typedef buf_type::const_iterator buf_iter_type;
	typedef nsql_database_system::c_bson_access_proxy proxy_type;

	typedef bson_iarchive_impl this_type;

#ifdef BOOST_NO_MEMBER_TEMPLATE_FRIENDS
public:
#else
	friend class boost::archive::detail::interface_iarchive<Archive>;
	friend class basic_bson_iarchive<Archive>;
	friend class boost::archive::load_access;
protected:
#endif

	bson_iarchive_impl(const buf_type& buf, unsigned int flags)
		: base_type(buf, flags)
	{
	}

	~bson_iarchive_impl(void)
	{
	}

	template<class T>
	void load(T& t)
	{
		BOOST_MPL_ASSERT(( nsql_database_system::is_bson_inside_type<T>));
		if(base_type::_start == base_type::_end) return;
		proxy_type::load_value(base_type::_start, t);
		++base_type::_start;
	}

	void load(boost::serialization::collection_size_type& t) //size_type
	{
		BOOST_MPL_ASSERT(( nsql_database_system::is_bson_inside_type<
								boost::serialization::collection_size_type>));
		if(base_type::_start == base_type::_end) return;
		ser_size_type tmp = t;
		proxy_type::load_value(base_type::_start, tmp);
		++base_type::_start;
		t = boost::serialization::collection_size_type(tmp);
	}

	void load(yggr::serialization::collection_size_type& t)
	{
		BOOST_MPL_ASSERT(( nsql_database_system::is_bson_inside_type<
								yggr::serialization::collection_size_type>));
		if(base_type::_start == base_type::_end) return;
		ser_size_type tmp = t;
		proxy_type::load_value(base_type::_start, tmp);
		++base_type::_start;
		t = yggr::serialization::collection_size_type(tmp);
	}

	void load(boost::serialization::version_type& t)
	{
		BOOST_MPL_ASSERT(( nsql_database_system::is_bson_inside_type<
								 boost::serialization::version_type>));
		if(base_type::_start == base_type::_end) return;
		u32 tmp = 0;
		proxy_type::load_value(base_type::_start, tmp);
		++base_type::_start;
		t = boost::serialization::version_type(tmp);
	}

	void load(boost::serialization::item_version_type& t)
	{
		BOOST_MPL_ASSERT(( nsql_database_system::is_bson_inside_type<
								boost::serialization::item_version_type>));
		if(base_type::_start == base_type::_end) return;
		u32 tmp = 0;
		proxy_type::load_value(base_type::_start, tmp);
		++base_type::_start;
		t = boost::serialization::item_version_type(tmp);
	}

public:
    using boost::archive::detail::interface_iarchive<Archive>::operator>>;

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

// do not derive from the classes below.  If you want to extend this functionality
// via inhertance, derived from text_iarchive_impl instead.  This will
// preserve correct static polymorphism.

// same as bson_iarchive below - without the shared_ptr_helper

template<bool auto_id_skip>
class naked_bson_iarchive
	: public bson_iarchive_impl< naked_bson_iarchive<auto_id_skip> >
{
public:
	typedef nsql_database_system::c_bson buf_type;
	typedef bson_iarchive_impl<naked_bson_iarchive> base_type;
private:
	typedef naked_bson_iarchive this_type;
public:
	naked_bson_iarchive(buf_type& buf, unsigned int flags = 0)
		: base_type(buf, flags)
	{
	}

	~naked_bson_iarchive(void)
	{
	}

	bool is_skip_auto_id(void) const
	{
		return auto_id_skip;
	}

	static bool s_is_skip_auto_id(void)
	{
		return auto_id_skip;
	}
};

} // namespace archive
} // namespace yggr

#ifdef BOOST_MSVC
#pragma warning(pop)
#endif

#include <boost/archive/detail/abi_suffix.hpp> // pops abi_suffix.hpp pragmas

// note special treatment of shared_ptr. This type needs a special
// structure associated with every archive.  We created a "mix-in"
// class to provide this functionality.  Since shared_ptr holds a
// special esteem in the boost library - we included it here by default.
#include <boost/archive/shared_ptr_helper.hpp>

#ifdef BOOST_MSVC
#  pragma warning(push)
#  pragma warning(disable : 4511 4512)
#endif

namespace yggr
{
namespace archive
{

template<bool auto_id_skip>
class bson_iarchive
	: public bson_iarchive_impl< bson_iarchive<auto_id_skip> >,
	public boost::archive::detail::shared_ptr_helper
{
public:
	typedef nsql_database_system::c_bson buf_type;
	typedef bson_iarchive_impl<bson_iarchive> base_type;
private:
	typedef bson_iarchive this_type;
public:
	bson_iarchive(const buf_type& buf, unsigned int flags = 0)
		: base_type(buf, flags)
	{
	}

	~bson_iarchive(void)
	{
	}

	bool is_skip_auto_id(void) const
	{
		return auto_id_skip;
	}

	static bool s_is_skip_auto_id(void)
	{
		return auto_id_skip;
	}
};

} // namespace archive
} // namespace yggr

// required by export
BOOST_SERIALIZATION_REGISTER_ARCHIVE(yggr::archive::bson_iarchive<true>)
BOOST_SERIALIZATION_REGISTER_ARCHIVE(yggr::archive::bson_iarchive<false>)

YGGR_PP_ARCHIVE_BINARY_DATA_WRAP_TRAITS_DEF(bson_iarchive<true>, yggr::nsql_database_system::c_bson_u8_binary_buffer)
YGGR_PP_ARCHIVE_BINARY_DATA_WRAP_TRAITS_DEF(bson_iarchive<false>, yggr::nsql_database_system::c_bson_u8_binary_buffer)

YGGR_PP_ARCHIVE_BINARY_DATA_WRAP_TRAITS_DEF(naked_bson_iarchive<true>, yggr::nsql_database_system::c_bson_u8_binary_buffer)
YGGR_PP_ARCHIVE_BINARY_DATA_WRAP_TRAITS_DEF(naked_bson_iarchive<false>, yggr::nsql_database_system::c_bson_u8_binary_buffer)

#ifdef BOOST_MSVC
#pragma warning(pop)
#endif

#include <yggr/serialization/bson_data_type.hpp>
#include <yggr/archive/impl/iserializer_bson_impl.hpp>
#include <yggr/serialization/detail/bson_impl_reinclue.hpp>

#endif // __YGGR_ARCHIVE_BSON_IARCHIVE_HPP__
