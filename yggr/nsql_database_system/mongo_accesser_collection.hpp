//mongo_accesser_collection.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_MONGO_ACCESSER_COLLECTION_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_MONGO_ACCESSER_COLLECTION_HPP__

#include <yggr/range_ex/range_iterator_ex.hpp>
#include <yggr/container/empty.hpp>
#include <yggr/container/size.hpp>

#include <yggr/iterator_ex/iterator.hpp>

#include <yggr/nsql_database_system/basic_mongo_accesser_collection.hpp>
#include <yggr/nsql_database_system/bson_ptr_getter.hpp> 
#include <yggr/nsql_database_system/detail/to_pointer.hpp>

namespace yggr
{
namespace nsql_database_system
{
namespace detail
{

template<typename BsonContainer, typename HandlerBsonPtrGetter>
struct mongo_accesser_bson_array_maker
{
public:
	typedef bson_t org_bson_type;
	typedef BsonContainer bson_container_type;
	typedef HandlerBsonPtrGetter handler_bson_ptr_getter_type;
	typedef ::yggr::vector<const org_bson_type*> bson_ptr_vt_type;

	bson_ptr_vt_type operator()(const bson_container_type& bs_cont, 
								const handler_bson_ptr_getter_type& handler_bson_ptr_getter) const
	{
		typedef typename range_ex::range_const_iterator_ex<bson_container_type>::type bson_cont_iter_type;

		if(container::empty(bs_cont))
		{
			return bson_ptr_vt_type();
		}

		bson_ptr_vt_type vt(container::size(bs_cont), 0);
		std::size_t idx = 0;
		for(bson_cont_iter_type i = boost::begin(bs_cont), isize = boost::end(bs_cont); 
				i != isize; iterator_ex::advance(i, 1), ++idx)
		{
			vt[idx] = handler_bson_ptr_getter(*i);
		}

		return vt;
	}
};

} // namespace detail
} // namespace nsql_database_system
} // namespace yggr

namespace yggr
{
namespace nsql_database_system
{

class mongo_accesser_collection
	: public basic_mongo_accesser_collection
{
public:
	typedef basic_mongo_accesser_collection base_type;

	typedef base_type::bson_type bson_type;
	typedef base_type::org_bson_type org_bson_type;

	typedef base_type::bson_value_type bson_value_type;
	typedef base_type::org_bson_value_type org_bson_value_type;

	typedef base_type::bson_error_type bson_error_type;
	typedef base_type::org_bson_error_type org_bson_error_type;

	typedef base_type::write_concern_type write_concern_type;
	typedef base_type::org_write_concern_type org_write_concern_type;

	typedef base_type::read_prefs_type read_prefs_type;
	typedef base_type::org_read_prefs_type org_read_prefs_type;

	typedef base_type::index_model_type index_model_type;
	typedef base_type::org_index_model_type org_index_model_type;

	typedef base_type::find_and_modify_opts_type find_and_modify_opts_type;
	typedef base_type::org_find_and_modify_opts_type org_find_and_modify_opts_type;

	typedef base_type::read_concern_type read_concern_type;
	typedef base_type::org_read_concern_type org_read_concern_type;

	typedef base_type::mongoc_error_domain_type mongoc_error_domain_type;
	typedef base_type::mongoc_error_code_type mongoc_error_code_type;

	typedef bson_ptr_getter bson_ptr_getter_type;

private:
	typedef mongo_accesser_collection this_type;

public:
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	// don't coding get_param or set_params function, because that is call time used only, 
	// example get_read_concern(...)
	// if you want to use that, please call foo "using_handler" execute you custom native operations
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	
	//------------------------collection s------------------------

	// insert
	// insert one version
private:
	template<typename Connection_T, typename String1, typename String2> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_collection_insert_one_wrap(Connection_T& conn, 
										const String1& str_db, 
										const String2& str_coln,
										const org_bson_type* pdoc,
										const org_bson_type* popts,
										org_bson_type* preply,
										org_bson_error_type* perr)
	{
		typedef typename native_t<String1>::type string1_type;
		typedef typename native_t<String2>::type string2_type;

		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string1_type, utf8_string1_type);
		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string2_type, utf8_string2_type);

		utf8_string1_type utf8_str_db((charset::string_charset_helper_data(str_db)));
		utf8_string2_type utf8_str_coln((charset::string_charset_helper_data(str_coln)));

		return 
			base_type::s_org_collection_insert_one(
				conn, 
				utf8_str_db.data(), 
				utf8_str_coln.data(), 
				pdoc, popts,
				preply, perr);
	}

public:
	template<typename Connection_T, typename String1, typename String2,
				typename BsonDoc, typename BsonOpts> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>,
				boost::is_base_of<org_bson_type, typename original_t<BsonDoc>::type>
			>,
			bool
		>::type
		s_collection_insert(Connection_T& conn, 
							const String1& str_db, 
							const String2& str_coln,
							const BsonDoc& doc,
							const BsonOpts& opts,
							org_bson_type* preply,
							org_bson_error_type* perr = 0)
	{
		return 
			this_type::s_collection_insert_one_wrap(
				conn, str_db, str_coln,
				detail::to_const_pointer<org_bson_type>(doc),
				detail::to_const_pointer<org_bson_type>(opts),
				preply, perr);
	}

	template<typename Connection_T, typename String1, typename String2,
				typename BsonDoc, typename BsonOpts> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>,
				boost::is_base_of<org_bson_type, typename original_t<BsonDoc>::type>
			>,
			bool
		>::type
		s_collection_insert(Connection_T& conn, 
							const String1& str_db, 
							const String2& str_coln,
							const BsonDoc& doc,
							const BsonOpts& opts,
							org_bson_type& reply,
							org_bson_error_type* perr = 0)
	{
		return 
			this_type::s_collection_insert_one_wrap(
				conn, str_db, str_coln,
				detail::to_const_pointer<org_bson_type>(doc),
				detail::to_const_pointer<org_bson_type>(opts),
				boost::addressof(reply), 
				perr);
	}

	template<typename Connection_T, typename String1, typename String2,
				typename BsonDoc, typename BsonOpts> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>,
				boost::is_base_of<org_bson_type, typename original_t<BsonDoc>::type>
			>,
			bool
		>::type
		s_collection_insert(Connection_T& conn, 
							const String1& str_db, 
							const String2& str_coln,
							const BsonDoc& doc,
							const BsonOpts& opts,
							org_bson_type* preply,
							org_bson_error_type& err)
	{
		return 
			this_type::s_collection_insert_one_wrap(
				conn, str_db, str_coln,
				detail::to_const_pointer<org_bson_type>(doc),
				detail::to_const_pointer<org_bson_type>(opts),
				preply, 
				boost::addressof(err));
	}

	template<typename Connection_T, typename String1, typename String2,
				typename BsonDoc, typename BsonOpts> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>,
				boost::is_base_of<org_bson_type, typename original_t<BsonDoc>::type>
			>,
			bool
		>::type
		s_collection_insert(Connection_T& conn, 
							const String1& str_db, 
							const String2& str_coln,
							const BsonDoc& doc,
							const BsonOpts& opts,
							org_bson_type& reply,
							org_bson_error_type& err)
	{
		return 
			this_type::s_collection_insert_one_wrap(
				conn, str_db, str_coln,
				detail::to_const_pointer<org_bson_type>(doc),
				detail::to_const_pointer<org_bson_type>(opts),
				boost::addressof(reply), 
				boost::addressof(err) );
	}

	// insert many version
private:
	template<typename Connection_T, typename String1, typename String2, typename Bson> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_collection_insert_many_wrap(Connection_T& conn, 
										const String1& str_db, 
										const String2& str_coln,
										Bson** ppbs, 
										u32 n_documents,
										const org_bson_type* popts,
										org_bson_type* preply,
										org_bson_error_type* perr)
	{
		typedef Bson now_bson_type;
		typedef typename native_t<String1>::type string1_type;
		typedef typename native_t<String2>::type string2_type;

		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string1_type, utf8_string1_type);
		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string2_type, utf8_string2_type);

		utf8_string1_type utf8_str_db((charset::string_charset_helper_data(str_db)));
		utf8_string2_type utf8_str_coln((charset::string_charset_helper_data(str_coln)));

		return 
			base_type::s_org_collection_insert_many(
				conn, 
				utf8_str_db.data(), 
				utf8_str_coln.data(), 
				reinterpret_cast<const org_bson_type**>(const_cast<const now_bson_type**>(ppbs)),
				n_documents, popts, preply, perr);
	}

public:
	template<typename Connection_T, typename String1, typename String2, 
				typename Bson, typename BsonOpts> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_collection_insert(Connection_T& conn, 
							const String1& str_db, 
							const String2& str_coln,
							Bson** ppbs, 
							u32 n_documents,
							const BsonOpts& opts,
							org_bson_type* preply,
							org_bson_error_type* perr = 0)
	{
		return
			this_type::s_collection_insert_many_wrap(
				conn, str_db, str_coln, 
				ppbs, n_documents,
				detail::to_const_pointer<org_bson_type>(opts),
				preply, perr);
	}

	template<typename Connection_T, typename String1, typename String2, 
				typename Bson, typename BsonOpts> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_collection_insert(Connection_T& conn, 
							const String1& str_db, 
							const String2& str_coln,
							Bson** ppbs, 
							u32 n_documents,
							const BsonOpts& opts,
							org_bson_type& reply,
							org_bson_error_type* perr = 0)
	{
		return
			this_type::s_collection_insert_many_wrap(
				conn, str_db, str_coln, 
				ppbs, n_documents,
				detail::to_const_pointer<org_bson_type>(opts),
				boost::addressof(reply), 
				perr);
	}

	template<typename Connection_T, typename String1, typename String2, 
				typename Bson, typename BsonOpts> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_collection_insert(Connection_T& conn, 
							const String1& str_db, 
							const String2& str_coln,
							Bson** ppbs, 
							u32 n_documents,
							const BsonOpts& opts,
							org_bson_type* preply,
							org_bson_error_type& err)
	{
		return
			this_type::s_collection_insert_many_wrap(
				conn, str_db, str_coln, 
				ppbs, n_documents,
				detail::to_const_pointer<org_bson_type>(opts),
				preply,
				boost::addressof(err));
	}

	template<typename Connection_T, typename String1, typename String2, 
				typename Bson, typename BsonOpts> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_collection_insert(Connection_T& conn, 
							const String1& str_db, 
							const String2& str_coln,
							Bson** ppbs, 
							u32 n_documents,
							const BsonOpts& opts,
							org_bson_type& reply,
							org_bson_error_type& err)
	{
		return
			this_type::s_collection_insert_many_wrap(
				conn, str_db, str_coln, 
				ppbs, n_documents,
				detail::to_const_pointer<org_bson_type>(opts),
				boost::addressof(reply), 
				boost::addressof(err));
	}

private:
	template<typename Connection_T, typename String1, typename String2, typename BsonContainer> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_collection_insert_many_wrap(Connection_T& conn, 
										const String1& str_db, 
										const String2& str_coln,
										const BsonContainer& bson_container, 
										const org_bson_type* popts,
										org_bson_type* preply,
										org_bson_error_type* perr)
	{
		typedef BsonContainer bson_container_type;

		typedef typename native_t<String1>::type string1_type;
		typedef typename native_t<String2>::type string2_type;

		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string1_type, utf8_string1_type);
		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string2_type, utf8_string2_type);

		typedef 
			detail::mongo_accesser_bson_array_maker
			<
				bson_container_type,
				bson_ptr_getter_type
			> bson_array_maker_type;
		typedef typename bson_array_maker_type::bson_ptr_vt_type bson_ptr_vt_type;
		
		utf8_string1_type utf8_str_db((charset::string_charset_helper_data(str_db)));
		utf8_string2_type utf8_str_coln((charset::string_charset_helper_data(str_coln)));
		
		bson_array_maker_type maker;
		bson_ptr_vt_type vt(maker(bson_container, bson_ptr_getter_type()));

		const org_bson_type** ppbs = vt.empty()? 0 : &vt[0];

		return 
			base_type::s_org_collection_insert_many(
				conn, 
				utf8_str_db.data(), 
				utf8_str_coln.data(), 
				ppbs, static_cast<u32>(vt.size()), popts, preply, perr);
	}

public:
	template<typename Connection_T, typename String1, typename String2, 
				typename BsonContainer, 
				typename BsonOpts> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>,
				container_ex::is_container<BsonContainer>
			>,
			bool
		>::type
		s_collection_insert(Connection_T& conn, 
							const String1& str_db, 
							const String2& str_coln,
							const BsonContainer& bson_container, 
							const BsonOpts& opts,
							org_bson_type* preply,
							org_bson_error_type* perr = 0)
	{
		return
			this_type::s_collection_insert_many_wrap(
				conn, str_db, str_coln, 
				bson_container, 
				detail::to_const_pointer<org_bson_type>(opts), 
				preply, perr);
	}

	template<typename Connection_T, typename String1, typename String2, 
				typename BsonContainer, 
				typename BsonOpts> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>,
				container_ex::is_container<BsonContainer>
			>,
			bool
		>::type
		s_collection_insert(Connection_T& conn, 
							const String1& str_db, 
							const String2& str_coln,
							const BsonContainer& bson_container, 
							const BsonOpts& opts,
							org_bson_type& reply,
							org_bson_error_type* perr = 0)
	{
		return
			this_type::s_collection_insert_many_wrap(
				conn, str_db, str_coln, 
				bson_container, 
				detail::to_const_pointer<org_bson_type>(opts), 
				boost::addressof(reply), 
				perr);
	}

	template<typename Connection_T, typename String1, typename String2, 
				typename BsonContainer, 
				typename BsonOpts> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>,
				container_ex::is_container<BsonContainer>
			>,
			bool
		>::type
		s_collection_insert(Connection_T& conn, 
							const String1& str_db, 
							const String2& str_coln,
							const BsonContainer& bson_container, 
							const BsonOpts& opts,
							org_bson_type* preply,
							org_bson_error_type& err)
	{
		return
			this_type::s_collection_insert_many_wrap(
				conn, str_db, str_coln, 
				bson_container, 
				detail::to_const_pointer<org_bson_type>(opts), 
				preply, 
				boost::addressof(err));
	}

	template<typename Connection_T, typename String1, typename String2, 
				typename BsonContainer, 
				typename BsonOpts> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>,
				container_ex::is_container<BsonContainer>
			>,
			bool
		>::type
		s_collection_insert(Connection_T& conn, 
							const String1& str_db, 
							const String2& str_coln,
							const BsonContainer& bson_container, 
							const BsonOpts& opts,
							org_bson_type& reply,
							org_bson_error_type& err)
	{
		return
			this_type::s_collection_insert_many_wrap(
				conn, str_db, str_coln, 
				bson_container, 
				detail::to_const_pointer<org_bson_type>(opts), 
				boost::addressof(reply), 
				boost::addressof(err));
	}

private:
	template<typename Connection_T, typename String1, typename String2, 
				typename BsonContainer, typename HandlerBsonPtrGetter> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_collection_insert_many_wrap(Connection_T& conn, 
										const String1& str_db, 
										const String2& str_coln,
										const BsonContainer& bson_container,
										const HandlerBsonPtrGetter& handler_bson_ptr_getter,
										const org_bson_type* popts,
										org_bson_type* preply,
										org_bson_error_type* perr)
	{
		typedef BsonContainer bson_container_type;
		typedef HandlerBsonPtrGetter handler_bson_ptr_getter_type;

		typedef typename native_t<String1>::type string1_type;
		typedef typename native_t<String2>::type string2_type;

		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string1_type, utf8_string1_type);
		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string2_type, utf8_string2_type);

		typedef 
			detail::mongo_accesser_bson_array_maker
			<
				bson_container_type, 
				handler_bson_ptr_getter_type
			> bson_array_maker_type;
		typedef typename bson_array_maker_type::bson_ptr_vt_type bson_ptr_vt_type;
		
		utf8_string1_type utf8_str_db((charset::string_charset_helper_data(str_db)));
		utf8_string2_type utf8_str_coln((charset::string_charset_helper_data(str_coln)));
		
		bson_array_maker_type maker;
		bson_ptr_vt_type vt(maker(bson_container, handler_bson_ptr_getter));

		const org_bson_type** ppbs = vt.empty()? 0 : &vt[0];

		return 
			base_type::s_org_collection_insert_many(
				conn, 
				utf8_str_db.data(), 
				utf8_str_coln.data(), 
				ppbs, vt.size(), popts, preply, perr);
	}

public:
	template<typename Connection_T, typename String1, typename String2, 
				typename BsonContainer, typename HandlerBsonPtrGetter,
				typename BsonOpts> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>,
				container_ex::is_container<BsonContainer>
			>,
			bool
		>::type
		s_collection_insert(Connection_T& conn, 
							const String1& str_db, 
							const String2& str_coln,
							const BsonContainer& bson_container,
							const HandlerBsonPtrGetter& handler_bson_ptr_getter,
							const BsonOpts& opts,
							org_bson_type* preply,
							org_bson_error_type* perr = 0)
	{
		return
			this_type::s_collection_insert_many_wrap(
				conn, str_db, str_coln, 
				bson_container,
				handler_bson_ptr_getter,
				detail::to_const_pointer<org_bson_type>(opts),
				preply, perr);
	}

	template<typename Connection_T, typename String1, typename String2, 
				typename BsonContainer, typename HandlerBsonPtrGetter,
				typename BsonOpts> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>,
				container_ex::is_container<BsonContainer>
			>,
			bool
		>::type
		s_collection_insert(Connection_T& conn, 
							const String1& str_db, 
							const String2& str_coln,
							const BsonContainer& bson_container,
							const HandlerBsonPtrGetter& handler_bson_ptr_getter,
							const BsonOpts& opts,
							org_bson_type& reply,
							org_bson_error_type* perr = 0)
	{
		return
			this_type::s_collection_insert_many_wrap(
				conn, str_db, str_coln, 
				bson_container,
				handler_bson_ptr_getter,
				detail::to_const_pointer<org_bson_type>(opts),
				boost::addressof(reply), 
				perr);
	}

	template<typename Connection_T, typename String1, typename String2, 
				typename BsonContainer, typename HandlerBsonPtrGetter,
				typename BsonOpts> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>,
				container_ex::is_container<BsonContainer>
			>,
			bool
		>::type
		s_collection_insert(Connection_T& conn, 
							const String1& str_db, 
							const String2& str_coln,
							const BsonContainer& bson_container,
							const HandlerBsonPtrGetter& handler_bson_ptr_getter,
							const BsonOpts& opts,
							org_bson_type* preply,
							org_bson_error_type& err)
	{
		return
			this_type::s_collection_insert_many_wrap(
				conn, str_db, str_coln, 
				bson_container,
				handler_bson_ptr_getter,
				detail::to_const_pointer<org_bson_type>(opts),
				preply, 
				boost::addressof(err));
	}

	template<typename Connection_T, typename String1, typename String2, 
				typename BsonContainer, typename HandlerBsonPtrGetter,
				typename BsonOpts> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>,
				container_ex::is_container<BsonContainer>
			>,
			bool
		>::type
		s_collection_insert(Connection_T& conn, 
							const String1& str_db, 
							const String2& str_coln,
							const BsonContainer& bson_container,
							const HandlerBsonPtrGetter& handler_bson_ptr_getter,
							const BsonOpts& opts,
							org_bson_type& reply,
							org_bson_error_type& err)
	{
		return
			this_type::s_collection_insert_many_wrap(
				conn, str_db, str_coln, 
				bson_container,
				handler_bson_ptr_getter,
				detail::to_const_pointer<org_bson_type>(opts),
				boost::addressof(reply), 
				boost::addressof(err));
	}

	// delete_one
private:
	template<typename Connection_T, typename String1, typename String2> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_collection_delete_one_wrap(Connection_T& conn, 
										const String1& str_db, 
										const String2& str_coln,
										const org_bson_type* psel,
										const org_bson_type* popts,
										org_bson_type* preply,
										org_bson_error_type* perr)
	{
		typedef typename native_t<String1>::type string1_type;
		typedef typename native_t<String2>::type string2_type;

		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string1_type, utf8_string1_type);
		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string2_type, utf8_string2_type);
		
		utf8_string1_type utf8_str_db((charset::string_charset_helper_data(str_db)));
		utf8_string2_type utf8_str_coln((charset::string_charset_helper_data(str_coln)));
		
		return 
			base_type::s_org_collection_delete_one(
				conn, 
				utf8_str_db.data(), 
				utf8_str_coln.data(), 
				psel, popts, 
				preply, perr);
	}

public:
	template<typename Connection_T, typename String1, typename String2, 
				typename BsonSel, typename BsonOpts> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_collection_delete_one(Connection_T& conn, 
								const String1& str_db, 
								const String2& str_coln,
								const BsonSel& sel,
								const BsonOpts& opts,
								org_bson_type* preply,
								org_bson_error_type* perr = 0)
	{
		return 
			this_type::s_collection_delete_one_wrap(
				conn, str_db, str_coln, 
				detail::to_const_pointer<org_bson_type>(sel),
				detail::to_const_pointer<org_bson_type>(opts),
				preply, perr);
	}

	template<typename Connection_T, typename String1, typename String2, 
				typename BsonSel, typename BsonOpts> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_collection_delete_one(Connection_T& conn, 
								const String1& str_db, 
								const String2& str_coln,
								const BsonSel& sel,
								const BsonOpts& opts,
								org_bson_type& reply,
								org_bson_error_type* perr = 0)
	{
		return 
			this_type::s_collection_delete_one_wrap(
				conn, str_db, str_coln, 
				detail::to_const_pointer<org_bson_type>(sel),
				detail::to_const_pointer<org_bson_type>(opts),
				boost::addressof(reply), 
				perr);
	}

	template<typename Connection_T, typename String1, typename String2, 
				typename BsonSel, typename BsonOpts> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_collection_delete_one(Connection_T& conn, 
								const String1& str_db, 
								const String2& str_coln,
								const BsonSel& sel,
								const BsonOpts& opts,
								org_bson_type* preply,
								org_bson_error_type& err)
	{
		return 
			this_type::s_collection_delete_one_wrap(
				conn, str_db, str_coln, 
				detail::to_const_pointer<org_bson_type>(sel),
				detail::to_const_pointer<org_bson_type>(opts),
				preply, 
				boost::addressof(err));
	}

	template<typename Connection_T, typename String1, typename String2, 
				typename BsonSel, typename BsonOpts> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_collection_delete_one(Connection_T& conn, 
								const String1& str_db, 
								const String2& str_coln,
								const BsonSel& sel,
								const BsonOpts& opts,
								org_bson_type& reply,
								org_bson_error_type& err)
	{
		return 
			this_type::s_collection_delete_one_wrap(
				conn, str_db, str_coln,
				detail::to_const_pointer<org_bson_type>(sel),
				detail::to_const_pointer<org_bson_type>(opts),
				boost::addressof(reply), 
				boost::addressof(err));
	}

	// delete_many
private:
	template<typename Connection_T, typename String1, typename String2> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_collection_delete_many_wrap(Connection_T& conn, 
										const String1& str_db, 
										const String2& str_coln,
										const org_bson_type* psel,
										const org_bson_type* popts,
										org_bson_type* preply,
										org_bson_error_type* perr)
	{
		typedef typename native_t<String1>::type string1_type;
		typedef typename native_t<String2>::type string2_type;

		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string1_type, utf8_string1_type);
		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string2_type, utf8_string2_type);
		
		utf8_string1_type utf8_str_db((charset::string_charset_helper_data(str_db)));
		utf8_string2_type utf8_str_coln((charset::string_charset_helper_data(str_coln)));
		
		return 
			base_type::s_org_collection_delete_many(
				conn, 
				utf8_str_db.data(), 
				utf8_str_coln.data(), 
				psel, popts, 
				preply, perr);
	}

public:
	template<typename Connection_T, typename String1, typename String2, 
				typename BsonSel, typename BsonOpts> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_collection_delete_many(Connection_T& conn, 
								const String1& str_db, 
								const String2& str_coln,
								const BsonSel& sel,
								const BsonOpts& opts,
								org_bson_type* preply,
								org_bson_error_type* perr = 0)
	{
		return 
			this_type::s_collection_delete_many_wrap(
				conn, str_db, str_coln,
				detail::to_const_pointer<org_bson_type>(sel),
				detail::to_const_pointer<org_bson_type>(opts),
				preply, perr);
	}

	template<typename Connection_T, typename String1, typename String2, 
				typename BsonSel, typename BsonOpts> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_collection_delete_many(Connection_T& conn, 
								const String1& str_db, 
								const String2& str_coln,
								const BsonSel& sel,
								const BsonOpts& opts,
								org_bson_type& reply,
								org_bson_error_type* perr = 0)
	{
		return 
			this_type::s_collection_delete_many_wrap(
				conn, str_db, str_coln,
				detail::to_const_pointer<org_bson_type>(sel),
				detail::to_const_pointer<org_bson_type>(opts),
				boost::addressof(reply), 
				perr);
	}

	template<typename Connection_T, typename String1, typename String2, 
				typename BsonSel, typename BsonOpts> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_collection_delete_many(Connection_T& conn, 
								const String1& str_db, 
								const String2& str_coln,
								const BsonSel& sel,
								const BsonOpts& opts,
								org_bson_type* preply,
								org_bson_error_type& err)
	{
		return 
			this_type::s_collection_delete_many_wrap(
				conn, str_db, str_coln,
				detail::to_const_pointer<org_bson_type>(sel),
				detail::to_const_pointer<org_bson_type>(opts),
				preply, 
				boost::addressof(err));
	}

	template<typename Connection_T, typename String1, typename String2, 
				typename BsonSel, typename BsonOpts> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_collection_delete_many(Connection_T& conn, 
								const String1& str_db, 
								const String2& str_coln,
								const BsonSel& sel,
								const BsonOpts& opts,
								org_bson_type& reply,
								org_bson_error_type& err)
	{
		return 
			this_type::s_collection_delete_many_wrap(
				conn, str_db, str_coln,
				detail::to_const_pointer<org_bson_type>(sel),
				detail::to_const_pointer<org_bson_type>(opts),
				boost::addressof(reply), 
				boost::addressof(err));
	}

	// remove
private:
	template<typename Connection_T, typename String1, typename String2> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_collection_remove_wrap(Connection_T& conn, 
									const String1& str_db, 
									const String2& str_coln, 
									u32 flags, 
									const org_bson_type* psel, 
									const org_bson_type* popts, 
									org_bson_type* preply, 
									org_bson_error_type* perr)
	{
		typedef typename native_t<String1>::type string1_type;
		typedef typename native_t<String2>::type string2_type;

		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string1_type, utf8_string1_type);
		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string2_type, utf8_string2_type);

		utf8_string1_type utf8_str_db((charset::string_charset_helper_data(str_db)));
		utf8_string2_type utf8_str_coln((charset::string_charset_helper_data(str_coln)));

		return
			base_type::s_org_collection_remove(
				conn,
				utf8_str_db.data(), 
				utf8_str_coln.data(),
				flags, 
				psel, popts, 
				preply, perr);
	}

public:
	template<typename Connection_T, typename String1, typename String2,
				typename BsonSel, typename BsonOpts> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_collection_remove(Connection_T& conn, 
							const String1& str_db, 
							const String2& str_coln, 
							u32 flags, 
							const BsonSel& sel, 
							const BsonOpts& opts, 
							org_bson_type* preply, 
							org_bson_error_type* perr = 0)
	{
		return
			this_type::s_collection_remove_wrap(
				conn, str_db, str_coln,
				flags, 
				detail::to_const_pointer<org_bson_type>(sel), 
				detail::to_const_pointer<org_bson_type>(opts), 
				preply, perr);
	}

	template<typename Connection_T, typename String1, typename String2,
				typename BsonSel, typename BsonOpts> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_collection_remove(Connection_T& conn, 
							const String1& str_db, 
							const String2& str_coln, 
							u32 flags, 
							const BsonSel& sel, 
							const BsonOpts& opts, 
							org_bson_type& reply, 
							org_bson_error_type* perr = 0)
	{
		return
			this_type::s_collection_remove_wrap(
				conn, str_db, str_coln,
				flags, 
				detail::to_const_pointer<org_bson_type>(sel), 
				detail::to_const_pointer<org_bson_type>(opts), 
				boost::addressof(reply), 
				perr);
	}

	template<typename Connection_T, typename String1, typename String2,
				typename BsonSel, typename BsonOpts> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_collection_remove(Connection_T& conn, 
							const String1& str_db, 
							const String2& str_coln, 
							u32 flags, 
							const BsonSel& sel, 
							const BsonOpts& opts, 
							org_bson_type* preply, 
							org_bson_error_type& err)
	{
		return
			this_type::s_collection_remove_wrap(
				conn, str_db, str_coln,
				flags, 
				detail::to_const_pointer<org_bson_type>(sel), 
				detail::to_const_pointer<org_bson_type>(opts), 
				preply, 
				boost::addressof(err));
	}

	template<typename Connection_T, typename String1, typename String2,
				typename BsonSel, typename BsonOpts> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_collection_remove(Connection_T& conn, 
							const String1& str_db, 
							const String2& str_coln, 
							u32 flags, 
							const BsonSel& sel, 
							const BsonOpts& opts, 
							org_bson_type& reply, 
							org_bson_error_type& err)
	{
		return
			this_type::s_collection_remove_wrap(
				conn, str_db, str_coln,
				flags, 
				detail::to_const_pointer<org_bson_type>(sel), 
				detail::to_const_pointer<org_bson_type>(opts), 
				boost::addressof(reply), 
				boost::addressof(err));
	}

	// find
private:
	template<typename Connection_T, 
				typename String1,
				typename String2,
				typename HandlerCollector> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_collection_find_wrap(Connection_T& conn, 
								const String1& str_db, 
								const String2& str_coln, 
								const org_bson_type* pfilter, 
								const org_bson_type* popts,
								const org_read_prefs_type* pread_prefs,
								org_bson_error_type* perr,
								const HandlerCollector& handler_collector)
	{
		typedef typename native_t<String1>::type string1_type;
		typedef typename native_t<String2>::type string2_type;

		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string1_type, utf8_string1_type);
		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string2_type, utf8_string2_type);

		utf8_string1_type utf8_str_db((charset::string_charset_helper_data(str_db)));
		utf8_string2_type utf8_str_coln((charset::string_charset_helper_data(str_coln)));

		return 
			base_type::s_org_collection_find(
				conn, 
				utf8_str_db.data(), 
				utf8_str_coln.data(),
				pfilter, popts, pread_prefs,
				perr,
				handler_collector);
	}

public:
	template<typename Connection_T,
				typename String1, typename String2,
				typename BsonFileter, typename BsonOpts,
				typename ReadPrefs,
				typename HandlerCollector> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_collection_find(Connection_T& conn, 
							const String1& str_db, 
							const String2& str_coln,
							const BsonFileter& filter, 
							const BsonOpts& opts,
							const ReadPrefs& read_prefs,
							org_bson_error_type* perr,
							const HandlerCollector& handler_collector)
	{
		return 
			this_type::s_collection_find_wrap(
				conn, str_db, str_coln,
				detail::to_const_pointer<org_bson_type>(filter),
				detail::to_const_pointer<org_bson_type>(opts),
				detail::to_const_pointer<org_read_prefs_type>(read_prefs),
				perr,
				handler_collector);
	}

	template<typename Connection_T,
				typename String1, typename String2,
				typename BsonFileter, typename BsonOpts,
				typename ReadPrefs,
				typename HandlerCollector> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_collection_find(Connection_T& conn, 
							const String1& str_db, 
							const String2& str_coln,
							const BsonFileter& filter, 
							const BsonOpts& opts,
							const ReadPrefs& read_prefs,
							org_bson_error_type& err,
							const HandlerCollector& handler_collector)
	{
		return 
			this_type::s_collection_find_wrap(
				conn, str_db, str_coln,
				detail::to_const_pointer<org_bson_type>(filter),
				detail::to_const_pointer<org_bson_type>(opts),
				detail::to_const_pointer<org_read_prefs_type>(read_prefs),
				boost::addressof(err),
				handler_collector);
	}

	// aggregate
private:
	template<typename Connection_T,
				typename String1,
				typename String2,
				typename HandlerCollector> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_collection_aggregate_wrap(Connection_T& conn, 
									const String1& str_db, 
									const String2& str_coln, 
									u32 flags, 
									const org_bson_type* ppipeline, 
									const org_bson_type* popts,
									const org_read_prefs_type* pread_prefs,
									org_bson_error_type* perr, 
									const HandlerCollector& handler_collector)
	{
		typedef typename native_t<String1>::type string1_type;
		typedef typename native_t<String2>::type string2_type;

		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string1_type, utf8_string1_type);
		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string2_type, utf8_string2_type);

		utf8_string1_type utf8_str_db((charset::string_charset_helper_data(str_db)));
		utf8_string2_type utf8_str_coln((charset::string_charset_helper_data(str_coln)));

		return 
			base_type::s_org_collection_aggregate(
				conn, utf8_str_db.data(), utf8_str_coln.data(),
				flags, ppipeline, popts,
				pread_prefs, perr,
				handler_collector);
	}

public:
	template<typename Connection_T,
				typename String1, typename String2,
				typename Bson1, typename Bson2,
				typename ReadPrefs,
				typename HandlerCollector> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_collection_aggregate(Connection_T& conn, 
								const String1& str_db, 
								const String2& str_coln, 
								u32 flags, 
								const Bson1& pipeline, 
								const Bson2& opts,
								const ReadPrefs& read_prefs,
								org_bson_error_type* err, 
								const HandlerCollector& handler_collector)
	{
		return 
			this_type::s_collection_aggregate_wrap(
				conn, str_db, str_coln,
				flags, 
				detail::to_const_pointer<org_bson_type>(pipeline),
				detail::to_const_pointer<org_bson_type>(opts),
				detail::to_const_pointer<org_read_prefs_type>(read_prefs), 
				err,
				handler_collector);
	}

	template<typename Connection_T,
				typename String1, typename String2,
				typename Bson1, typename Bson2,
				typename ReadPrefs,
				typename HandlerCollector> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_collection_aggregate(Connection_T& conn, 
								const String1& str_db, 
								const String2& str_coln, 
								u32 flags, 
								const Bson1& pipeline, 
								const Bson2& opts,
								const ReadPrefs& read_prefs,
								org_bson_error_type& err, 
								const HandlerCollector& handler_collector)
	{
		return 
			this_type::s_collection_aggregate_wrap(
				conn, str_db, str_coln,
				flags, 
				detail::to_const_pointer<org_bson_type>(pipeline), 
				detail::to_const_pointer<org_bson_type>(opts),
				detail::to_const_pointer<org_read_prefs_type>(read_prefs), 
				boost::addressof(err),
				handler_collector);
	}

	// update

	// replace one
private:
	template<typename Connection_T,
				typename String1, typename String2> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_collection_replace_one_wrap(Connection_T& conn, 
										const String1& str_db, 
										const String2& str_coln,
										const org_bson_type* pselector, 
										const org_bson_type* preplacement,
										const org_bson_type* popts,
										org_bson_type* preply, 
										org_bson_error_type* perr)
	{
		typedef typename native_t<String1>::type string1_type;
		typedef typename native_t<String2>::type string2_type;

		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string1_type, utf8_string1_type);
		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string2_type, utf8_string2_type);

		utf8_string1_type utf8_str_db((charset::string_charset_helper_data(str_db)));
		utf8_string2_type utf8_str_coln((charset::string_charset_helper_data(str_coln)));

		return 
			base_type::s_org_collection_replace_one(
				conn, utf8_str_db.data(), utf8_str_coln.data(),
				pselector, preplacement, popts, preply, perr);
	}

public:
	template<typename Connection_T,
				typename String1, typename String2,
				typename BsonSel, typename BsonReplace, typename BsonOpts> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_collection_replace_one(Connection_T& conn, 
									const String1& str_db, 
									const String2& str_coln,
									const BsonSel& selector, 
									const BsonReplace& replacement,
									const BsonOpts& opts,
									org_bson_type* preply, 
									org_bson_error_type* perr = 0)
	{
		return 
			this_type::s_collection_replace_one_wrap(
				conn,
				str_db, str_coln,
				detail::to_const_pointer<org_bson_type>(selector),
				detail::to_const_pointer<org_bson_type>(replacement),
				detail::to_const_pointer<org_bson_type>(opts),
				preply, perr);
	}

	template<typename Connection_T,
				typename String1, typename String2,
				typename BsonSel, typename BsonReplace, typename BsonOpts> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_collection_replace_one(Connection_T& conn, 
									const String1& str_db, 
									const String2& str_coln,
									const BsonSel& selector, 
									const BsonReplace& replacement,
									const BsonOpts& opts,
									org_bson_type& reply, 
									org_bson_error_type* perr = 0)
	{
		return 
			this_type::s_collection_replace_one_wrap(
				conn,
				str_db, str_coln,
				detail::to_const_pointer<org_bson_type>(selector),
				detail::to_const_pointer<org_bson_type>(replacement),
				detail::to_const_pointer<org_bson_type>(opts),
				boost::addressof(reply), 
				perr);
	}

	template<typename Connection_T,
				typename String1, typename String2,
				typename BsonSel, typename BsonReplace, typename BsonOpts> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_collection_replace_one(Connection_T& conn, 
									const String1& str_db, 
									const String2& str_coln,
									const BsonSel& selector, 
									const BsonReplace& replacement,
									const BsonOpts& opts,
									org_bson_type* preply, 
									org_bson_error_type& err)
	{
		return 
			this_type::s_collection_replace_one_wrap(
				conn,
				str_db, str_coln,
				detail::to_const_pointer<org_bson_type>(selector),
				detail::to_const_pointer<org_bson_type>(replacement),
				detail::to_const_pointer<org_bson_type>(opts),
				preply, 
				boost::addressof(err) );
	}

	template<typename Connection_T,
				typename String1, typename String2,
				typename BsonSel, typename BsonReplace, typename BsonOpts> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_collection_replace_one(Connection_T& conn, 
									const String1& str_db, 
									const String2& str_coln,
									const BsonSel& selector, 
									const BsonReplace& replacement,
									const BsonOpts& opts,
									org_bson_type& reply, 
									org_bson_error_type& err)
	{
		return 
			this_type::s_collection_replace_one_wrap(
				conn,
				str_db, str_coln,
				detail::to_const_pointer<org_bson_type>(selector),
				detail::to_const_pointer<org_bson_type>(replacement),
				detail::to_const_pointer<org_bson_type>(opts),
				boost::addressof(reply), 
				boost::addressof(err) );
	}

	// update_one
private:
	template<typename Connection_T,
				typename String1, typename String2> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_collection_update_one_wrap(Connection_T& conn, 
										const String1& str_db, 
										const String2& str_coln,
										const org_bson_type* pselector, 
										const org_bson_type* pupdate,
										const org_bson_type* popts,
										org_bson_type* preply, 
										org_bson_error_type* perr)
	{
		typedef typename native_t<String1>::type string1_type;
		typedef typename native_t<String2>::type string2_type;

		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string1_type, utf8_string1_type);
		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string2_type, utf8_string2_type);

		utf8_string1_type utf8_str_db((charset::string_charset_helper_data(str_db)));
		utf8_string2_type utf8_str_coln((charset::string_charset_helper_data(str_coln)));

		return 
			base_type::s_org_collection_update_one(
				conn, utf8_str_db.data(), utf8_str_coln.data(),
				pselector, pupdate, popts, preply, perr);
	}

public:
	template<typename Connection_T,
				typename String1, typename String2,
				typename BsonSel, typename BsonUpdate, typename BsonOpts> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_collection_update_one(Connection_T& conn, 
									const String1& str_db, 
									const String2& str_coln,
									const BsonSel& selector, 
									const BsonUpdate& update,
									const BsonOpts& opts,
									org_bson_type* preply, 
									org_bson_error_type* perr = 0)
	{
		return 
			this_type::s_collection_update_one_wrap(
				conn,
				str_db, str_coln,
				detail::to_const_pointer<org_bson_type>(selector),
				detail::to_const_pointer<org_bson_type>(update),
				detail::to_const_pointer<org_bson_type>(opts),
				preply, perr);
	}

	template<typename Connection_T,
				typename String1, typename String2,
				typename BsonSel, typename BsonUpdate, typename BsonOpts> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_collection_update_one(Connection_T& conn, 
									const String1& str_db, 
									const String2& str_coln,
									const BsonSel& selector, 
									const BsonUpdate& update,
									const BsonOpts& opts,
									org_bson_type& reply, 
									org_bson_error_type* perr = 0)
	{
		return 
			this_type::s_collection_update_one_wrap(
				conn,
				str_db, str_coln,
				detail::to_const_pointer<org_bson_type>(selector),
				detail::to_const_pointer<org_bson_type>(update),
				detail::to_const_pointer<org_bson_type>(opts),
				boost::addressof(reply), 
				perr);
	}

	template<typename Connection_T,
				typename String1, typename String2,
				typename BsonSel, typename BsonUpdate, typename BsonOpts> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_collection_update_one(Connection_T& conn, 
									const String1& str_db, 
									const String2& str_coln,
									const BsonSel& selector, 
									const BsonUpdate& update,
									const BsonOpts& opts,
									org_bson_type* preply, 
									org_bson_error_type& err)
	{
		return 
			this_type::s_collection_update_one_wrap(
				conn,
				str_db, str_coln,
				detail::to_const_pointer<org_bson_type>(selector),
				detail::to_const_pointer<org_bson_type>(update),
				detail::to_const_pointer<org_bson_type>(opts),
				preply, 
				boost::addressof(err) );
	}

	template<typename Connection_T,
				typename String1, typename String2,
				typename BsonSel, typename BsonUpdate, typename BsonOpts> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_collection_update_one(Connection_T& conn, 
									const String1& str_db, 
									const String2& str_coln,
									const BsonSel& selector, 
									const BsonUpdate& update,
									const BsonOpts& opts,
									org_bson_type& reply, 
									org_bson_error_type& err)
	{
		return 
			this_type::s_collection_update_one_wrap(
				conn,
				str_db, str_coln,
				detail::to_const_pointer<org_bson_type>(selector),
				detail::to_const_pointer<org_bson_type>(update),
				detail::to_const_pointer<org_bson_type>(opts),
				boost::addressof(reply), 
				boost::addressof(err) );
	}

	// update_many
private:
	template<typename Connection_T,
				typename String1, typename String2> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_collection_update_many_wrap(Connection_T& conn, 
										const String1& str_db, 
										const String2& str_coln,
										const org_bson_type* pselector, 
										const org_bson_type* pupdate,
										const org_bson_type* popts,
										org_bson_type* preply, 
										org_bson_error_type* perr)
	{
		typedef typename native_t<String1>::type string1_type;
		typedef typename native_t<String2>::type string2_type;

		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string1_type, utf8_string1_type);
		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string2_type, utf8_string2_type);

		utf8_string1_type utf8_str_db((charset::string_charset_helper_data(str_db)));
		utf8_string2_type utf8_str_coln((charset::string_charset_helper_data(str_coln)));

		return 
			base_type::s_org_collection_update_many(
				conn, utf8_str_db.data(), utf8_str_coln.data(),
				pselector, pupdate, popts, preply, perr);
	}

public:
	template<typename Connection_T,
				typename String1, typename String2,
				typename BsonSel, typename BsonUpdate, typename BsonOpts> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_collection_update_many(Connection_T& conn, 
									const String1& str_db, 
									const String2& str_coln,
									const BsonSel& selector, 
									const BsonUpdate& update,
									const BsonOpts& opts,
									org_bson_type* preply, 
									org_bson_error_type* perr = 0)
	{
		return 
			this_type::s_collection_update_many_wrap(
				conn,
				str_db, str_coln,
				detail::to_const_pointer<org_bson_type>(selector),
				detail::to_const_pointer<org_bson_type>(update),
				detail::to_const_pointer<org_bson_type>(opts),
				preply, perr);
	}

	template<typename Connection_T,
				typename String1, typename String2,
				typename BsonSel, typename BsonUpdate, typename BsonOpts> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_collection_update_many(Connection_T& conn, 
									const String1& str_db, 
									const String2& str_coln,
									const BsonSel& selector, 
									const BsonUpdate& update,
									const BsonOpts& opts,
									org_bson_type& reply, 
									org_bson_error_type* perr = 0)
	{
		return 
			this_type::s_collection_update_many_wrap(
				conn,
				str_db, str_coln,
				detail::to_const_pointer<org_bson_type>(selector),
				detail::to_const_pointer<org_bson_type>(update),
				detail::to_const_pointer<org_bson_type>(opts),
				boost::addressof(reply), 
				perr);
	}

	template<typename Connection_T,
				typename String1, typename String2,
				typename BsonSel, typename BsonUpdate, typename BsonOpts> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_collection_update_many(Connection_T& conn, 
									const String1& str_db, 
									const String2& str_coln,
									const BsonSel& selector, 
									const BsonUpdate& update,
									const BsonOpts& opts,
									org_bson_type* preply, 
									org_bson_error_type& err)
	{
		return 
			this_type::s_collection_update_many_wrap(
				conn,
				str_db, str_coln,
				detail::to_const_pointer<org_bson_type>(selector),
				detail::to_const_pointer<org_bson_type>(update),
				detail::to_const_pointer<org_bson_type>(opts),
				preply, 
				boost::addressof(err) );
	}

	template<typename Connection_T,
				typename String1, typename String2,
				typename BsonSel, typename BsonUpdate, typename BsonOpts> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_collection_update_many(Connection_T& conn, 
									const String1& str_db, 
									const String2& str_coln,
									const BsonSel& selector, 
									const BsonUpdate& update,
									const BsonOpts& opts,
									org_bson_type& reply, 
									org_bson_error_type& err)
	{
		return 
			this_type::s_collection_update_many_wrap(
				conn,
				str_db, str_coln,
				detail::to_const_pointer<org_bson_type>(selector),
				detail::to_const_pointer<org_bson_type>(update),
				detail::to_const_pointer<org_bson_type>(opts),
				boost::addressof(reply), 
				boost::addressof(err) );
	}

	// update
private:
	template<typename Connection_T,
				typename String1, typename String2> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_collection_update_wrap(Connection_T& conn, 
										const String1& str_db, 
										const String2& str_coln,
										u32 flags,
										const org_bson_type* pselector, 
										const org_bson_type* pupdate,
										const org_bson_type* popts,
										org_bson_type* preply, 
										org_bson_error_type* perr)
	{
		typedef typename native_t<String1>::type string1_type;
		typedef typename native_t<String2>::type string2_type;

		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string1_type, utf8_string1_type);
		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string2_type, utf8_string2_type);

		utf8_string1_type utf8_str_db((charset::string_charset_helper_data(str_db)));
		utf8_string2_type utf8_str_coln((charset::string_charset_helper_data(str_coln)));

		return 
			base_type::s_org_collection_update(
				conn, utf8_str_db.data(), utf8_str_coln.data(),
				flags, pselector, pupdate, popts, preply, perr);
	}

public:
	template<typename Connection_T,
				typename String1, typename String2,
				typename BsonSel, typename BsonUpdate, typename BsonOpts> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_collection_update(Connection_T& conn, 
									const String1& str_db, 
									const String2& str_coln,
									u32 flags,
									const BsonSel& selector, 
									const BsonUpdate& update,
									const BsonOpts& opts,
									org_bson_type* preply, 
									org_bson_error_type* perr = 0)
	{
		return 
			this_type::s_collection_update_wrap(
				conn,
				str_db, str_coln,
				flags,
				detail::to_const_pointer<org_bson_type>(selector),
				detail::to_const_pointer<org_bson_type>(update),
				detail::to_const_pointer<org_bson_type>(opts),
				preply, perr);
	}

	template<typename Connection_T,
				typename String1, typename String2,
				typename BsonSel, typename BsonUpdate, typename BsonOpts> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_collection_update(Connection_T& conn, 
									const String1& str_db, 
									const String2& str_coln,
									u32 flags,
									const BsonSel& selector, 
									const BsonUpdate& update,
									const BsonOpts& opts,
									org_bson_type& reply, 
									org_bson_error_type* perr = 0)
	{
		return 
			this_type::s_collection_update_wrap(
				conn,
				str_db, str_coln,
				flags,
				detail::to_const_pointer<org_bson_type>(selector),
				detail::to_const_pointer<org_bson_type>(update),
				detail::to_const_pointer<org_bson_type>(opts),
				boost::addressof(reply), 
				perr);
	}

	template<typename Connection_T,
				typename String1, typename String2,
				typename BsonSel, typename BsonUpdate, typename BsonOpts> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_collection_update(Connection_T& conn, 
									const String1& str_db, 
									const String2& str_coln,
									u32 flags,
									const BsonSel& selector, 
									const BsonUpdate& update,
									const BsonOpts& opts,
									org_bson_type* preply, 
									org_bson_error_type& err)
	{
		return 
			this_type::s_collection_update_wrap(
				conn,
				str_db, str_coln,
				flags,
				detail::to_const_pointer<org_bson_type>(selector),
				detail::to_const_pointer<org_bson_type>(update),
				detail::to_const_pointer<org_bson_type>(opts),
				preply, 
				boost::addressof(err) );
	}

	template<typename Connection_T,
				typename String1, typename String2,
				typename BsonSel, typename BsonUpdate, typename BsonOpts> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_collection_update(Connection_T& conn, 
									const String1& str_db, 
									const String2& str_coln,
									u32 flags,
									const BsonSel& selector, 
									const BsonUpdate& update,
									const BsonOpts& opts,
									org_bson_type& reply, 
									org_bson_error_type& err)
	{
		return 
			this_type::s_collection_update_wrap(
				conn,
				str_db, str_coln,
				flags,
				detail::to_const_pointer<org_bson_type>(selector),
				detail::to_const_pointer<org_bson_type>(update),
				detail::to_const_pointer<org_bson_type>(opts),
				boost::addressof(reply), 
				boost::addressof(err) );
	}

	//count
private:
	template<typename Connection_T, typename String1, typename String2> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			s64
		>::type
		s_collecion_count_wrap(Connection_T& conn, 
								const String1& str_db, 
								const String2& str_coln, 
								const org_bson_type* pfilter,
								const org_bson_type* popts,
								const org_read_prefs_type* pread_prefs,
								org_bson_type* preply,
								org_bson_error_type* perr)
	{
		typedef typename native_t<String1>::type string1_type;
		typedef typename native_t<String2>::type string2_type;

		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string1_type, utf8_string1_type);
		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string2_type, utf8_string2_type);

		utf8_string1_type utf8_str_db((charset::string_charset_helper_data(str_db)));
		utf8_string2_type utf8_str_coln((charset::string_charset_helper_data(str_coln)));

		return 
			base_type::s_org_collection_count(
				conn, utf8_str_db.data(), utf8_str_coln.data(),
				pfilter, popts, pread_prefs, preply, perr);
	}

public:
	template<typename Connection_T,
				typename String1, typename String2,
				typename BsonFilter, typename BsonOpts,
				typename ReadPrefs > inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			s64
		>::type
		s_collection_count(Connection_T& conn, 
							const String1& str_db, 
							const String2& str_coln, 
							const BsonFilter& filter,
							const BsonOpts& opts,
							const ReadPrefs& read_prefs,
							org_bson_type* preply,
							org_bson_error_type* perr = 0)
	{
		return 
			this_type::s_collecion_count_wrap(
				conn, str_db, str_coln,
				detail::to_const_pointer<org_bson_type>(filter),
				detail::to_const_pointer<org_bson_type>(opts),
				detail::to_const_pointer<org_read_prefs_type>(read_prefs),
				preply,
				perr);
	}

	template<typename Connection_T,
				typename String1, typename String2,
				typename BsonFilter, typename BsonOpts,
				typename ReadPrefs> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			s64
		>::type
		s_collection_count(Connection_T& conn, 
							const String1& str_db, 
							const String2& str_coln, 
							const BsonFilter& filter,
							const BsonOpts& opts,
							const ReadPrefs& read_prefs,
							org_bson_type& reply,
							org_bson_error_type* perr = 0)
	{
		return 
			this_type::s_collecion_count_wrap(
				conn, str_db, str_coln,
				detail::to_const_pointer<org_bson_type>(filter),
				detail::to_const_pointer<org_bson_type>(opts),
				detail::to_const_pointer<org_read_prefs_type>(read_prefs),
				boost::addressof(reply),
				perr);
	}

	template<typename Connection_T,
				typename String1, typename String2,
				typename BsonFilter, typename BsonOpts,
				typename ReadPrefs> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			s64
		>::type
		s_collection_count(Connection_T& conn, 
							const String1& str_db, 
							const String2& str_coln, 
							const BsonFilter& filter,
							const BsonOpts& opts,
							const ReadPrefs& read_prefs,
							org_bson_type* preply,
							org_bson_error_type& err)
	{
		return 
			this_type::s_collecion_count_wrap(
				conn, str_db, str_coln,
				detail::to_const_pointer<org_bson_type>(filter),
				detail::to_const_pointer<org_bson_type>(opts),
				detail::to_const_pointer<org_read_prefs_type>(read_prefs),
				preply,
				boost::addressof(err));
	}

	template<typename Connection_T,
				typename String1, typename String2,
				typename BsonFilter, typename BsonOpts,
				typename ReadPrefs> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			s64
		>::type
		s_collection_count(Connection_T& conn, 
							const String1& str_db, 
							const String2& str_coln, 
							const BsonFilter& filter,
							const BsonOpts& opts,
							const ReadPrefs& read_prefs,
							org_bson_type& reply,
							org_bson_error_type& err)
	{
		return 
			this_type::s_collecion_count_wrap(
				conn, str_db, str_coln,
				detail::to_const_pointer<org_bson_type>(filter),
				detail::to_const_pointer<org_bson_type>(opts),
				detail::to_const_pointer<org_read_prefs_type>(read_prefs),
				boost::addressof(reply),
				boost::addressof(err));
	}

	//estimated_count
private:
	template<typename Connection_T, typename String1, typename String2> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			s64
		>::type
		s_collecion_estimated_count_wrap(Connection_T& conn, 
											const String1& str_db, 
											const String2& str_coln, 
											const org_bson_type* popts,
											const org_read_prefs_type* pread_prefs,
											org_bson_type* preply,
											org_bson_error_type* perr)
	{
		typedef typename native_t<String1>::type string1_type;
		typedef typename native_t<String2>::type string2_type;

		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string1_type, utf8_string1_type);
		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string2_type, utf8_string2_type);

		utf8_string1_type utf8_str_db((charset::string_charset_helper_data(str_db)));
		utf8_string2_type utf8_str_coln((charset::string_charset_helper_data(str_coln)));

		return 
			base_type::s_org_collection_estimated_count(
				conn, utf8_str_db.data(), utf8_str_coln.data(),
				popts, pread_prefs, preply, perr);
	}

public:
	template<typename Connection_T,
				typename String1, typename String2,
				typename BsonOpts,
				typename ReadPrefs > inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			s64
		>::type
		s_collecion_estimated_count(Connection_T& conn, 
									const String1& str_db, 
									const String2& str_coln, 
									const BsonOpts& opts,
									const ReadPrefs& read_prefs,
									org_bson_type* preply,
									org_bson_error_type* perr = 0)
	{
		return 
			this_type::s_collecion_estimated_count_wrap(
				conn, str_db, str_coln,
				detail::to_const_pointer<org_bson_type>(opts),
				detail::to_const_pointer<org_read_prefs_type>(read_prefs),
				preply,
				perr);
	}

	template<typename Connection_T,
				typename String1, typename String2,
				typename BsonOpts,
				typename ReadPrefs> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			s64
		>::type
		s_collecion_estimated_count(Connection_T& conn, 
									const String1& str_db, 
									const String2& str_coln, 
									const BsonOpts& opts,
									const ReadPrefs& read_prefs,
									org_bson_type& reply,
									org_bson_error_type* perr = 0)
	{
		return 
			this_type::s_collecion_estimated_count_wrap(
				conn, str_db, str_coln,
				detail::to_const_pointer<org_bson_type>(opts),
				detail::to_const_pointer<org_read_prefs_type>(read_prefs),
				boost::addressof(reply),
				perr);
	}

	template<typename Connection_T,
				typename String1, typename String2,
				typename BsonOpts,
				typename ReadPrefs> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			s64
		>::type
		s_collecion_estimated_count(Connection_T& conn, 
									const String1& str_db, 
									const String2& str_coln,
									const BsonOpts& opts,
									const ReadPrefs& read_prefs,
									org_bson_type* preply,
									org_bson_error_type& err)
	{
		return 
			this_type::s_collecion_estimated_count_wrap(
				conn, str_db, str_coln,
				detail::to_const_pointer<org_bson_type>(opts),
				detail::to_const_pointer<org_read_prefs_type>(read_prefs),
				preply,
				boost::addressof(err));
	}

	template<typename Connection_T,
				typename String1, typename String2,
				typename BsonOpts,
				typename ReadPrefs> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			s64
		>::type
		s_collecion_estimated_count(Connection_T& conn, 
									const String1& str_db, 
									const String2& str_coln, 
									const BsonOpts& opts,
									const ReadPrefs& read_prefs,
									org_bson_type& reply,
									org_bson_error_type& err)
	{
		return 
			this_type::s_collecion_estimated_count_wrap(
				conn, str_db, str_coln,
				detail::to_const_pointer<org_bson_type>(opts),
				detail::to_const_pointer<org_read_prefs_type>(read_prefs),
				boost::addressof(reply),
				boost::addressof(err));
	}

	// drop 
private:
	template<typename Connection_T,
				typename String1, typename String2> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_collection_drop_wrap(Connection_T& conn, 
								const String1& str_db, 
								const String2& str_coln, 
								const org_bson_type* popts,
								org_bson_error_type* perr)
	{
		typedef typename native_t<String1>::type string1_type;
		typedef typename native_t<String2>::type string2_type;

		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string1_type, utf8_string1_type);
		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string2_type, utf8_string2_type);

		utf8_string1_type utf8_str_db((charset::string_charset_helper_data(str_db)));
		utf8_string2_type utf8_str_coln((charset::string_charset_helper_data(str_coln)));

		return 
			base_type::s_org_collecion_drop(
				conn, utf8_str_db.data(), utf8_str_coln.data(), 
				popts, perr);
	}

public:
	template<typename Connection_T,
				typename String1, typename String2,
				typename BsonOpts> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_collection_drop(Connection_T& conn, 
							const String1& str_db, 
							const String2& str_coln, 
							const BsonOpts& opts,
							org_bson_error_type* perr = 0)
	{
		return 
			this_type::s_collection_drop_wrap(
				conn, str_db, str_coln, 
				detail::to_const_pointer<org_bson_type>(opts),
				perr);
	}

	template<typename Connection_T,
				typename String1, typename String2,
				typename BsonOpts> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_collection_drop(Connection_T& conn, 
							const String1& str_db, 
							const String2& str_coln, 
							const BsonOpts& opts,
							org_bson_error_type& err)
	{
		return 
			this_type::s_collection_drop_wrap(
				conn, str_db, str_coln, 
				detail::to_const_pointer<org_bson_type>(opts),
				boost::addressof(err));
	}

	template<typename Connection_T,
				typename String1, typename String2> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_collection_drop(Connection_T& conn, 
							const String1& str_db, 
							const String2& str_coln, 
							org_bson_error_type* perr = 0)
	{
		return 
			this_type::s_collection_drop_wrap(
				conn, str_db, str_coln, 
				0,
				perr);
	}

	template<typename Connection_T,
				typename String1, typename String2> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_collection_drop(Connection_T& conn, 
							const String1& str_db, 
							const String2& str_coln,
							org_bson_error_type& err)
	{
		return 
			this_type::s_collection_drop_wrap(
				conn, str_db, str_coln, 
				0,
				boost::addressof(err));
	}

	// rename
private:
	template<typename Connection_T,
				typename String1, typename String2,
				typename String3, typename String4> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::mpl::and_
				<
					charset::utf8_string_constructable<String1>,
					charset::utf8_string_constructable<String2>
				>,
				boost::mpl::and_
				<
					charset::utf8_string_constructable<String3>,
					charset::utf8_string_constructable<String4>
				>
			>,
			bool
		>::type
		s_collection_rename_wrap(Connection_T& conn, 
									const String1& str_db, 
									const String2& str_coln,
									const String3& new_str_db, 
									const String4& new_str_coln,
									bool drop_taget_before_rename,
									const org_bson_type* popts,
									org_bson_error_type* perr)
	{
		typedef typename native_t<String1>::type string1_type;
		typedef typename native_t<String2>::type string2_type;
		typedef typename native_t<String3>::type string3_type;
		typedef typename native_t<String4>::type string4_type;

		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string1_type, utf8_string1_type);
		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string2_type, utf8_string2_type);
		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string3_type, utf8_string3_type);
		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string4_type, utf8_string4_type);

		utf8_string1_type utf8_str_db((charset::string_charset_helper_data(str_db)));
		utf8_string2_type utf8_str_coln((charset::string_charset_helper_data(str_coln)));
		utf8_string3_type utf8_new_str_db((charset::string_charset_helper_data(new_str_db)));
		utf8_string4_type utf8_new_str_coln((charset::string_charset_helper_data(new_str_coln)));

		return 
			base_type::s_org_collection_rename(
				conn, utf8_str_db.data(), utf8_str_coln.data(),
				utf8_new_str_db.data(), utf8_new_str_coln.data(),
				drop_taget_before_rename, popts, perr);
	}

public:
	template<typename Connection_T,
				typename String1, typename String2,
				typename String3, typename String4,
				typename BsonOpts> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::mpl::and_
				<
					charset::utf8_string_constructable<String1>,
					charset::utf8_string_constructable<String2>
				>,
				boost::mpl::and_
				<
					charset::utf8_string_constructable<String3>,
					charset::utf8_string_constructable<String4>
				>,
				boost::mpl::or_
				<
					boost::is_base_of<org_bson_type, typename native_t<BsonOpts>::type >,
					boost::is_integral<BsonOpts>
				>
			>,
			bool
		>::type
		s_collection_rename(Connection_T& conn, 
							const String1& str_db, 
							const String2& str_coln,
							const String3& new_str_db, 
							const String4& new_str_coln,
							bool drop_taget_before_rename,
							const BsonOpts& opts,
							org_bson_error_type* perr = 0)
	{
		return 
			this_type::s_collection_rename_wrap(
				conn, str_db, str_coln,
				new_str_db,  new_str_coln,
				drop_taget_before_rename,
				detail::to_const_pointer<org_bson_type>(opts),
				perr);
	}

	template<typename Connection_T,
				typename String1, typename String2,
				typename String3, typename String4,
				typename BsonOpts> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::mpl::and_
				<
					charset::utf8_string_constructable<String1>,
					charset::utf8_string_constructable<String2>
				>,
				boost::mpl::and_
				<
					charset::utf8_string_constructable<String3>,
					charset::utf8_string_constructable<String4>
				>,
				boost::mpl::or_
				<
					boost::is_base_of<org_bson_type, typename native_t<BsonOpts>::type >,
					boost::is_integral<BsonOpts>
				>
			>,
			bool
		>::type
		s_collection_rename(Connection_T& conn, 
							const String1& str_db, 
							const String2& str_coln,
							const String3& new_str_db, 
							const String4& new_str_coln,
							bool drop_taget_before_rename,
							const BsonOpts& opts,
							org_bson_error_type& err)
	{
		return 
			this_type::s_collection_rename_wrap(
				conn, str_db, str_coln,
				new_str_db,  new_str_coln,
				drop_taget_before_rename,
				detail::to_const_pointer<org_bson_type>(opts),
				boost::addressof(err));
	}

	template<typename Connection_T,
				typename String1, typename String2,
				typename String3, typename String4> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::mpl::and_
				<
					charset::utf8_string_constructable<String1>,
					charset::utf8_string_constructable<String2>
				>,
				boost::mpl::and_
				<
					charset::utf8_string_constructable<String3>,
					charset::utf8_string_constructable<String4>
				>
			>,
			bool
		>::type
		s_collection_rename(Connection_T& conn, 
							const String1& str_db, 
							const String2& str_coln,
							const String3& new_str_db, 
							const String4& new_str_coln,
							bool drop_taget_before_rename,
							org_bson_error_type* perr = 0)
	{
		return 
			this_type::s_collection_rename_wrap(
				conn, str_db, str_coln,
				new_str_db,  new_str_coln,
				drop_taget_before_rename,
				0,
				perr);
	}

	template<typename Connection_T,
				typename String1, typename String2,
				typename String3, typename String4> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::mpl::and_
				<
					charset::utf8_string_constructable<String1>,
					charset::utf8_string_constructable<String2>
				>,
				boost::mpl::and_
				<
					charset::utf8_string_constructable<String3>,
					charset::utf8_string_constructable<String4>
				>
			>,
			bool
		>::type
		s_collection_rename(Connection_T& conn, 
							const String1& str_db, 
							const String2& str_coln,
							const String3& new_str_db, 
							const String4& new_str_coln,
							bool drop_taget_before_rename,
							org_bson_error_type& err)
	{
		return 
			this_type::s_collection_rename_wrap(
				conn, str_db, str_coln,
				new_str_db,  new_str_coln,
				drop_taget_before_rename,
				0,
				boost::addressof(err));
	}

	// save
private:
	template<typename Connection_T,
				typename String1, typename String2> inline 
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_collection_save_wrap(Connection_T& conn, 
								const String1& str_db, 
								const String2& str_coln,
								const org_bson_type* pdoc, 
								const org_bson_type* popts,
								org_bson_type* preply,
								org_bson_error_type* perr)
	{
		typedef typename native_t<String1>::type string1_type;
		typedef typename native_t<String2>::type string2_type;

		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string1_type, utf8_string1_type);
		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string2_type, utf8_string2_type);

		utf8_string1_type utf8_str_db((charset::string_charset_helper_data(str_db)));
		utf8_string2_type utf8_str_coln((charset::string_charset_helper_data(str_coln)));

		return 
			base_type::s_org_collection_save(
				conn, utf8_str_db.data(), utf8_str_coln.data(), 
				pdoc, popts, preply, perr);
	}

public:
	template<typename Connection_T,
				typename String1, typename String2,
				typename BsonDoc, typename BsonOpts> inline 
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_collection_save(Connection_T& conn, 
							const String1& str_db, 
							const String2& str_coln,
							const BsonDoc& doc, 
							const BsonOpts& opts,
							org_bson_type* preply,
							org_bson_error_type* perr = 0)
	{
		return 
			this_type::s_collection_save_wrap(
				conn, 
				str_db, str_coln, 
				detail::to_const_pointer<org_bson_type>(doc), 
				detail::to_const_pointer<org_bson_type>(opts), 
				preply, perr);
	}

	template<typename Connection_T,
				typename String1, typename String2,
				typename BsonDoc, typename BsonOpts> inline 
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_collection_save(Connection_T& conn, 
							const String1& str_db, 
							const String2& str_coln,
							const BsonDoc& doc, 
							const BsonOpts& opts,
							org_bson_type& reply,
							org_bson_error_type* perr = 0)
	{
		return 
			this_type::s_collection_save_wrap(
				conn, 
				str_db, str_coln, 
				detail::to_const_pointer<org_bson_type>(doc), 
				detail::to_const_pointer<org_bson_type>(opts), 
				boost::addressof(reply), 
				perr);
	}

	template<typename Connection_T,
				typename String1, typename String2,
				typename BsonDoc, typename BsonOpts> inline 
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_collection_save(Connection_T& conn, 
							const String1& str_db, 
							const String2& str_coln,
							const BsonDoc& doc, 
							const BsonOpts& opts,
							org_bson_type* preply,
							org_bson_error_type& err)
	{
		return 
			this_type::s_collection_save_wrap(
				conn, 
				str_db, str_coln, 
				detail::to_const_pointer<org_bson_type>(doc), 
				detail::to_const_pointer<org_bson_type>(opts), 
				preply,
				boost::addressof(err) );
	}

	template<typename Connection_T,
				typename String1, typename String2,
				typename BsonDoc, typename BsonOpts> inline 
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_collection_save(Connection_T& conn, 
							const String1& str_db, 
							const String2& str_coln,
							const BsonDoc& doc, 
							const BsonOpts& opts,
							org_bson_type& reply,
							org_bson_error_type& err)
	{
		return 
			this_type::s_collection_save_wrap(
				conn, 
				str_db, str_coln, 
				detail::to_const_pointer<org_bson_type>(doc), 
				detail::to_const_pointer<org_bson_type>(opts), 
				boost::addressof(reply), 
				boost::addressof(err) );
	}

	// stats
private:
	template<typename Connection_T,
				typename String1, typename String2 > inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_collection_stats_wrap(Connection_T& conn, 
								const String1& str_db, 
								const String2& str_coln,
								const org_bson_type* popts, 
								org_bson_type* preply, 
								org_bson_error_type* perr)
	{
		typedef typename native_t<String1>::type string1_type;
		typedef typename native_t<String2>::type string2_type;

		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string1_type, utf8_string1_type);
		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string2_type, utf8_string2_type);

		utf8_string1_type utf8_str_db((charset::string_charset_helper_data(str_db)));
		utf8_string2_type utf8_str_coln((charset::string_charset_helper_data(str_coln)));

		return 
			base_type::s_org_collection_stats(
				conn, utf8_str_db.data(), utf8_str_coln.data(), 
				popts, preply, perr);
	}

public:
	template<typename Connection_T,
				typename String1, typename String2, 
				typename BsonOpts> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_collection_stats(Connection_T& conn, 
							const String1& str_db, 
							const String2& str_coln,
							const BsonOpts& opts, 
							org_bson_type* preply, 
							org_bson_error_type* perr = 0)
	{
		return 
			this_type::s_collection_stats_wrap(
				conn, str_db, str_coln, 
				detail::to_const_pointer<org_bson_type>(opts), 
				preply, perr);
	}

	template<typename Connection_T,
				typename String1, typename String2, 
				typename BsonOpts> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_collection_stats(Connection_T& conn, 
							const String1& str_db, 
							const String2& str_coln,
							const BsonOpts& opts, 
							org_bson_type& reply, 
							org_bson_error_type* perr = 0)
	{
		return 
			this_type::s_collection_stats_wrap(
				conn, str_db, str_coln, 
				detail::to_const_pointer<org_bson_type>(opts), 
				boost::addressof(reply), 
				perr);
	}

	template<typename Connection_T,
				typename String1, typename String2, 
				typename BsonOpts> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_collection_stats(Connection_T& conn, 
							const String1& str_db, 
							const String2& str_coln,
							const BsonOpts& opts, 
							org_bson_type* preply, 
							org_bson_error_type& err)
	{
		return 
			this_type::s_collection_stats_wrap(
				conn, str_db, str_coln, 
				detail::to_const_pointer<org_bson_type>(opts), 
				preply, 
				boost::addressof(err));
	}

	template<typename Connection_T,
				typename String1, typename String2, 
				typename BsonOpts> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_collection_stats(Connection_T& conn, 
							const String1& str_db, 
							const String2& str_coln,
							const BsonOpts& opts, 
							org_bson_type& reply, 
							org_bson_error_type& err)
	{
		return 
			this_type::s_collection_stats_wrap(
				conn, str_db, str_coln, 
				detail::to_const_pointer<org_bson_type>(opts), 
				boost::addressof(reply), 
				boost::addressof(err));
	}

	// validate
private:
	template<typename Connection_T, typename String1, typename String2> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_collection_validate_wrap(Connection_T& conn, 
									const String1& str_db, 
									const String2& str_coln,
									const org_bson_type* popts, 
									org_bson_type* preply, 
									org_bson_error_type* perr)
	{
		typedef typename native_t<String1>::type string1_type;
		typedef typename native_t<String2>::type string2_type;

		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string1_type, utf8_string1_type);
		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string2_type, utf8_string2_type);

		utf8_string1_type utf8_str_db((charset::string_charset_helper_data(str_db)));
		utf8_string2_type utf8_str_coln((charset::string_charset_helper_data(str_coln)));

		return 
			base_type::s_org_collections_validate(
				conn, utf8_str_db.data(), utf8_str_coln.data(), 
				popts, preply, perr);
	}

public:
	template<typename Connection_T, 
				typename String1, typename String2,
				typename BsonOpts> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_collection_validate(Connection_T& conn, 
								const String1& str_db, 
								const String2& str_coln,
								const BsonOpts& opts, 
								org_bson_type* preply, 
								org_bson_error_type* perr = 0)
	{
		return 
			this_type::s_collection_validate_wrap(
				conn, str_db, str_coln, 
				detail::to_const_pointer<org_bson_type>(opts),
				preply, 
				perr);
	}

	template<typename Connection_T, 
				typename String1, typename String2,
				typename BsonOpts> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_collection_validate(Connection_T& conn, 
								const String1& str_db, 
								const String2& str_coln,
								const BsonOpts& opts, 
								org_bson_type& reply, 
								org_bson_error_type* perr = 0)
	{
		return 
			this_type::s_collection_validate_wrap(
				conn, str_db, str_coln, 
				detail::to_const_pointer<org_bson_type>(opts),
				boost::addressof(reply), 
				perr);
	}

	template<typename Connection_T, 
				typename String1, typename String2,
				typename BsonOpts> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_collection_validate(Connection_T& conn, 
								const String1& str_db, 
								const String2& str_coln,
								const BsonOpts& opts, 
								org_bson_type* preply, 
								org_bson_error_type& err)
	{
		return 
			this_type::s_collection_validate_wrap(
				conn, str_db, str_coln, 
				detail::to_const_pointer<org_bson_type>(opts),
				preply, 
				boost::addressof(err));
	}

	template<typename Connection_T, 
				typename String1, typename String2,
				typename BsonOpts> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_collection_validate(Connection_T& conn, 
								const String1& str_db, 
								const String2& str_coln,
								const BsonOpts& opts, 
								org_bson_type& reply, 
								org_bson_error_type& err)
	{
		return 
			this_type::s_collection_validate_wrap(
				conn, str_db, str_coln, 
				detail::to_const_pointer<org_bson_type>(opts),
				boost::addressof(reply), 
				boost::addressof(err));
	}

	// index
	// create_index

	// mongoc_index_model_t* const *, n_models version  
private:
	template<typename Connection_T,
				typename String1, typename String2> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_collection_create_indexes_wrap(Connection_T& conn, 
											const String1& str_db, 
											const String2& str_coln,
											org_index_model_type* const* pmodels,
											size_t n_models,
											const org_bson_type* popts,
											org_bson_type* preply,
											org_bson_error_type* perr)
	{
		typedef typename native_t<String1>::type string1_type;
		typedef typename native_t<String2>::type string2_type;

		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string1_type, utf8_string1_type);
		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string2_type, utf8_string2_type);

		if(!(pmodels && n_models))
		{
			if(perr)
			{
				bson_set_error(
					perr, 
					mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND, 
					mongoc_error_code_type::E_MONGOC_ERROR_COMMAND_INVALID_ARG,
					"mongo_accesser create_index, command invalid args");
			}
			
			return false;
		}

		utf8_string1_type utf8_str_db((charset::string_charset_helper_data(str_db)));
		utf8_string2_type utf8_str_coln((charset::string_charset_helper_data(str_coln)));

		return 
			base_type::s_org_collection_create_indexes(
				conn, utf8_str_db.data(), utf8_str_coln.data(), 
				pmodels, n_models, popts, preply, perr);
	}

public:
	template<typename Connection_T,
				typename String1, typename String2,
				typename BsonOpts> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_collection_create_indexes(Connection_T& conn, 
									const String1& str_db, 
									const String2& str_coln,
									org_index_model_type* const* pmodels,
									size_t n_models,
									const BsonOpts& opts, 
									org_bson_type* preply,
									org_bson_error_type* perr = 0)
	{
		return 
			this_type::s_collection_create_indexes_wrap(
				conn, str_db, str_coln,
				pmodels, n_models,
				detail::to_const_pointer<org_bson_type>(opts),
				preply, perr);
	}

	template<typename Connection_T,
				typename String1, typename String2,
				typename BsonOpts> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_collection_create_indexes(Connection_T& conn, 
									const String1& str_db, 
									const String2& str_coln,
									org_index_model_type* const* pmodels,
									size_t n_models,
									const BsonOpts& opts, 
									org_bson_type& reply,
									org_bson_error_type* perr = 0)
	{
		return 
			this_type::s_collection_create_indexes_wrap(
				conn, str_db, str_coln,
				pmodels, n_models,
				detail::to_const_pointer<org_bson_type>(opts),
				boost::addressof(reply), 
				perr);
	}

	template<typename Connection_T,
				typename String1, typename String2,
				typename BsonOpts> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_collection_create_indexes(Connection_T& conn, 
									const String1& str_db, 
									const String2& str_coln,
									org_index_model_type* const* pmodels,
									size_t n_models,
									const BsonOpts& opts, 
									org_bson_type* preply,
									org_bson_error_type& err)
	{
		return 
			this_type::s_collection_create_indexes_wrap(
				conn, str_db, str_coln,
				pmodels, n_models,
				detail::to_const_pointer<org_bson_type>(opts),
				preply, 
				boost::addressof(err));
	}

	template<typename Connection_T,
				typename String1, typename String2,
				typename BsonOpts> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_collection_create_indexes(Connection_T& conn, 
									const String1& str_db, 
									const String2& str_coln,
									org_index_model_type* const* pmodels,
									size_t n_models,
									const BsonOpts& opts, 
									org_bson_type& reply,
									org_bson_error_type& err)
	{
		return 
			this_type::s_collection_create_indexes_wrap(
				conn, str_db, str_coln,
				pmodels, n_models,
				detail::to_const_pointer<org_bson_type>(opts),
				boost::addressof(reply), 
				boost::addressof(err));
	}

	// create single index version
private:
	template<typename Connection_T,
				typename String1, typename String2> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_collection_create_indexes_wrap(Connection_T& conn, 
											const String1& str_db, 
											const String2& str_coln,
											const org_index_model_type* pmodel,
											const org_bson_type* popts,
											org_bson_type* preply,
											org_bson_error_type* perr)
	{
		typedef typename native_t<String1>::type string1_type;
		typedef typename native_t<String2>::type string2_type;

		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string1_type, utf8_string1_type);
		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string2_type, utf8_string2_type);

		if(!pmodel)
		{
			if(perr)
			{
				bson_set_error(
					perr, 
					mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND, 
					mongoc_error_code_type::E_MONGOC_ERROR_COMMAND_INVALID_ARG,
					"mongo_accesser create_index, command invalid args");
			}
			
			return false;
		}

		utf8_string1_type utf8_str_db((charset::string_charset_helper_data(str_db)));
		utf8_string2_type utf8_str_coln((charset::string_charset_helper_data(str_coln)));

		org_index_model_type* pmodel_org = const_cast<org_index_model_type*>(pmodel);
		org_index_model_type** ppmodel_org = boost::addressof(pmodel_org);

		return 
			base_type::s_org_collection_create_indexes(
				conn, utf8_str_db.data(), utf8_str_coln.data(), 
				ppmodel_org, 1, popts, preply, perr);
	}

	template<typename Connection_T,
				typename String1, typename String2> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_collection_create_indexes_wrap(Connection_T& conn, 
											const String1& str_db, 
											const String2& str_coln,
											const index_model_type* pmodel,
											const org_bson_type* popts,
											org_bson_type* preply,
											org_bson_error_type* perr)
	{
		typedef typename native_t<String1>::type string1_type;
		typedef typename native_t<String2>::type string2_type;

		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string1_type, utf8_string1_type);
		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string2_type, utf8_string2_type);

		if(!pmodel)
		{
			if(perr)
			{
				bson_set_error(
					perr, 
					mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND, 
					mongoc_error_code_type::E_MONGOC_ERROR_COMMAND_INVALID_ARG,
					"mongo_accesser create_index, command invalid args");
			}
			
			return false;
		}

		utf8_string1_type utf8_str_db((charset::string_charset_helper_data(str_db)));
		utf8_string2_type utf8_str_coln((charset::string_charset_helper_data(str_coln)));

		const org_index_model_type* pmodel_org_c = *pmodel;
		org_index_model_type* pmodel_org = const_cast<org_index_model_type*>(pmodel_org_c);
		org_index_model_type** ppmodel_org = boost::addressof(pmodel_org);

		return 
			base_type::s_org_collection_create_indexes(
				conn, utf8_str_db.data(), utf8_str_coln.data(), 
				ppmodel_org, 1, popts, preply, perr);
	}

	// container version
private:
	template<typename Connection_T,
				typename String1, typename String2,
				typename ModelsContainer> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>,
				container_ex::is_container<ModelsContainer>
			>,
			bool
		>::type
		s_collection_create_indexes_wrap(Connection_T& conn, 
											const String1& str_db, 
											const String2& str_coln,
											const ModelsContainer& models_cont,
											const org_bson_type* popts,
											org_bson_type* preply,
											org_bson_error_type* perr)
	{
		typedef ModelsContainer models_cont_type;
		typedef typename range_ex::range_const_iterator_ex<models_cont_type>::type models_cont_citer_type;
		typedef container::vector<const org_index_model_type*> org_index_models_vt_type;

		std::size_t n_models = container::size(models_cont);

		if(!n_models)
		{
			if(perr)
			{
				bson_set_error(
					perr, 
					mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND, 
					mongoc_error_code_type::E_MONGOC_ERROR_COMMAND_INVALID_ARG,
					"mongo_accesser create_index, command invalid args");
			}
			
			return false;
		}

		org_index_models_vt_type models_vt;

		container::reserve(models_vt, n_models);

		std::copy(container::cbegin(models_cont), container::cend(models_cont), std::inserter(models_vt, container::end(models_vt)));
		assert(models_vt.size() == n_models);

		return 
			this_type::s_collection_create_indexes_wrap(
				conn, str_db, str_coln,
				const_cast<mongoc_index_model_t**>(models_vt.data()), models_vt.size(), popts,
				preply, perr);
	}

	// create single index version or contianer version
public:
	template<typename Connection_T,
				typename String1, typename String2,
				typename ModelOrModelsContainer,
				typename BsonOpts> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_collection_create_indexes(Connection_T& conn, 
									const String1& str_db, 
									const String2& str_coln,
									const ModelOrModelsContainer& model_or_models_cont,
									const BsonOpts& opts, 
									org_bson_type* preply,
									org_bson_error_type* perr = 0)
	{
		return 
			this_type::s_collection_create_indexes_wrap(
				conn, str_db, str_coln,
				model_or_models_cont, 
				detail::to_const_pointer<org_bson_type>(opts),
				preply, perr);
	}

	template<typename Connection_T,
				typename String1, typename String2,
				typename ModelOrModelsContainer,
				typename BsonOpts> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_collection_create_indexes(Connection_T& conn, 
									const String1& str_db, 
									const String2& str_coln,
									const ModelOrModelsContainer& model_or_models_cont,
									const BsonOpts& opts, 
									org_bson_type* preply,
									org_bson_error_type& err)
	{
		return 
			this_type::s_collection_create_indexes_wrap(
				conn, str_db, str_coln,
				model_or_models_cont, 
				detail::to_const_pointer<org_bson_type>(opts),
				preply, 
				boost::addressof(err));
	}

	template<typename Connection_T,
				typename String1, typename String2,
				typename ModelOrModelsContainer,
				typename BsonOpts> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_collection_create_indexes(Connection_T& conn, 
									const String1& str_db, 
									const String2& str_coln,
									const ModelOrModelsContainer& model_or_models_cont,
									const BsonOpts& opts, 
									org_bson_type& reply,
									org_bson_error_type* perr = 0)
	{
		return 
			this_type::s_collection_create_indexes_wrap(
				conn, str_db, str_coln,
				model_or_models_cont, 
				detail::to_const_pointer<org_bson_type>(opts),
				boost::addressof(reply), 
				perr);
	}

	template<typename Connection_T,
				typename String1, typename String2,
				typename ModelOrModelsContainer,
				typename BsonOpts> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_collection_create_indexes(Connection_T& conn, 
									const String1& str_db, 
									const String2& str_coln,
									const ModelOrModelsContainer& model_or_models_cont,
									const BsonOpts& opts, 
									org_bson_type& reply,
									org_bson_error_type& err)
	{
		return 
			this_type::s_collection_create_indexes_wrap(
				conn, str_db, str_coln,
				model_or_models_cont, 
				detail::to_const_pointer<org_bson_type>(opts),
				boost::addressof(reply),
				boost::addressof(err));
	}

	// iter_ver
private:
	template<typename Connection_T,
				typename String1, typename String2,
				typename ModelsIter> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>,
				iterator_ex::is_iterator<ModelsIter>
			>,
			bool
		>::type
		s_collection_create_indexes_wrap(Connection_T& conn, 
											const String1& str_db, 
											const String2& str_coln,
											ModelsIter models_iter_s,
											ModelsIter models_iter_e,
											const org_bson_type* popts,
											org_bson_type* preply,
											org_bson_error_type* perr)
	{
		typedef container::vector<const org_index_model_type*> org_index_models_vt_type;

		std::size_t n_models = std::distance(models_iter_s, models_iter_e);

		if(!n_models)
		{
			if(perr)
			{
				bson_set_error(
					perr, 
					mongoc_error_domain_type::E_MONGOC_ERROR_COMMAND, 
					mongoc_error_code_type::E_MONGOC_ERROR_COMMAND_INVALID_ARG,
					"mongo_accesser create_index, command invalid args");
			}
			
			return false;
		}

		org_index_models_vt_type models_vt;

		container::reserve(models_vt, n_models);

		std::copy(models_iter_s, models_iter_e, std::inserter(models_vt, container::end(models_vt)));
		assert(models_vt.size() == n_models);

		return 
			this_type::s_collection_create_indexes_wrap(
				conn, str_db, str_coln,
				const_cast<mongoc_index_model_t**>(models_vt.data()), models_vt.size(), popts,
				preply, perr);
	}

	//iter ver
public:
	template<typename Connection_T,
				typename String1, typename String2,
				typename ModelsIter,
				typename BsonOpts> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>,
				iterator_ex::is_iterator<ModelsIter>
			>,
			bool
		>::type
		s_collection_create_indexes(Connection_T& conn, 
									const String1& str_db, 
									const String2& str_coln,
									ModelsIter models_iter_s,
									ModelsIter models_iter_e,
									const BsonOpts& opts, 
									org_bson_type* preply,
									org_bson_error_type* perr = 0)
	{
		return 
			this_type::s_collection_create_indexes_wrap(
				conn, str_db, str_coln,
				models_iter_s, models_iter_e, detail::to_const_pointer<org_bson_type>(opts),
				preply, perr);
	}

	template<typename Connection_T,
				typename String1, typename String2,
				typename ModelsIter,
				typename BsonOpts> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>,
				iterator_ex::is_iterator<ModelsIter>
			>,
			bool
		>::type
		s_collection_create_indexes(Connection_T& conn, 
									const String1& str_db, 
									const String2& str_coln,
									ModelsIter models_iter_s,
									ModelsIter models_iter_e,
									const BsonOpts& opts, 
									org_bson_type* preply,
									org_bson_error_type& err)
	{
		return 
			this_type::s_collection_create_indexes_wrap(
				conn, str_db, str_coln,
				models_iter_s, models_iter_e, detail::to_const_pointer<org_bson_type>(opts),
				preply, boost::addressof(err));
	}

	template<typename Connection_T,
				typename String1, typename String2,
				typename ModelsIter,
				typename BsonOpts> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>,
				iterator_ex::is_iterator<ModelsIter>
			>,
			bool
		>::type
		s_collection_create_indexes(Connection_T& conn, 
									const String1& str_db, 
									const String2& str_coln,
									ModelsIter models_iter_s,
									ModelsIter models_iter_e,
									const BsonOpts& opts, 
									org_bson_type& reply,
									org_bson_error_type* perr = 0)
	{
		return 
			this_type::s_collection_create_indexes_wrap(
				conn, str_db, str_coln,
				models_iter_s, models_iter_e, detail::to_const_pointer<org_bson_type>(opts),
				boost::addressof(reply), perr);
	}

	template<typename Connection_T,
				typename String1, typename String2,
				typename ModelsIter,
				typename BsonOpts> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>,
				iterator_ex::is_iterator<ModelsIter>
			>,
			bool
		>::type
		s_collection_create_indexes(Connection_T& conn, 
									const String1& str_db, 
									const String2& str_coln,
									ModelsIter models_iter_s,
									ModelsIter models_iter_e,
									const BsonOpts& opts, 
									org_bson_type& reply,
									org_bson_error_type& err)
	{
		return 
			this_type::s_collection_create_indexes_wrap(
				conn, str_db, str_coln,
				models_iter_s, models_iter_e, detail::to_const_pointer<org_bson_type>(opts),
				boost::addressof(reply), boost::addressof(err));
	}

	// drop_index
private:
	template<typename Connection_T,
				typename String1, typename String2, typename String3> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>,
				charset::utf8_string_constructable<String3>
			>,
			bool
		>::type
		s_collection_drop_index_wrap(Connection_T& conn, 
										const String1& str_db, 
										const String2& str_coln,
										const String3& str_idx_name, 
										const org_bson_type* popts,
										org_bson_error_type* perr)
	{
		typedef typename native_t<String1>::type string1_type;
		typedef typename native_t<String2>::type string2_type;
		typedef typename native_t<String3>::type string3_type;

		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string1_type, utf8_string1_type);
		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string2_type, utf8_string2_type);
		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string3_type, utf8_string3_type);

		utf8_string1_type utf8_str_db((charset::string_charset_helper_data(str_db)));
		utf8_string2_type utf8_str_coln((charset::string_charset_helper_data(str_coln)));
		utf8_string3_type utf8_str_idx_name((charset::string_charset_helper_data(str_idx_name)));

		return 
			base_type::s_org_collection_drop_index(
				conn, 
				utf8_str_db.data(), utf8_str_coln.data(), 
				utf8_str_idx_name.data(), 
				popts, perr);
	}

public:
	template<typename Connection_T,
				typename String1, typename String2, typename String3,
				typename BsonOpts> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>,
				charset::utf8_string_constructable<String3>
			>,
			bool
		>::type
		s_collection_drop_index(Connection_T& conn, 
								const String1& str_db, 
								const String2& str_coln,
								const String3& str_idx_name, 
								const BsonOpts& opts,
								org_bson_error_type* perr = 0)
	{
		return 
			this_type::s_collection_drop_index_wrap(
				conn, 
				str_db, str_coln, 
				str_idx_name,
				detail::to_const_pointer<org_bson_type>(opts),
				perr);
	}

	template<typename Connection_T,
				typename String1, typename String2, typename String3,
				typename BsonOpts> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>,
				charset::utf8_string_constructable<String3>
			>,
			bool
		>::type
		s_collection_drop_index(Connection_T& conn, 
								const String1& str_db, 
								const String2& str_coln,
								const String3& str_idx_name, 
								const BsonOpts& opts,
								org_bson_error_type& err)
	{
		return 
			this_type::s_collection_drop_index_wrap(
				conn, 
				str_db, str_coln, 
				str_idx_name,
				detail::to_const_pointer<org_bson_type>(opts),
				boost::addressof(err));
	}

	template<typename Connection_T,
				typename String1, typename String2, typename String3> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>,
				charset::utf8_string_constructable<String3>
			>,
			bool
		>::type
		s_collection_drop_index(Connection_T& conn, 
								const String1& str_db, 
								const String2& str_coln,
								const String3& str_idx_name, 
								org_bson_error_type* perr = 0)
	{
		return 
			this_type::s_collection_drop_index_wrap(
				conn, 
				str_db, str_coln, 
				str_idx_name,
				0,
				perr);
	}

	template<typename Connection_T,
				typename String1, typename String2, typename String3> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>,
				charset::utf8_string_constructable<String3>
			>,
			bool
		>::type
		s_collection_drop_index(Connection_T& conn, 
								const String1& str_db, 
								const String2& str_coln,
								const String3& str_idx_name, 
								org_bson_error_type& err)
	{
		return 
			this_type::s_collection_drop_index_wrap(
				conn, 
				str_db, str_coln, 
				str_idx_name,
				0,
				boost::addressof(err));
	}

	// find_indexes
private:
	template<typename Connection_T, typename HandlerCollector,
				typename String1, typename String2> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_collection_find_indexes_wrap(Connection_T& conn, 
										const String1& str_db, 
										const String2& str_coln, 
										const org_bson_type* popts,
										org_bson_error_type* perr, 
										const HandlerCollector& handler_collector)
	{
		typedef typename native_t<String1>::type string1_type;
		typedef typename native_t<String2>::type string2_type;

		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string1_type, utf8_string1_type);
		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string2_type, utf8_string2_type);

		utf8_string1_type utf8_str_db((charset::string_charset_helper_data(str_db)));
		utf8_string2_type utf8_str_coln((charset::string_charset_helper_data(str_coln)));

		return 
			base_type::s_org_collection_find_indexes(
				conn, utf8_str_db.data(), utf8_str_coln.data(), 
				popts, perr, handler_collector);
	}

public:
	template<typename Connection_T, typename HandlerCollector,
				typename String1, typename String2,
				typename BsonOpts> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_collection_find_indexes(Connection_T& conn, 
									const String1& str_db, 
									const String2& str_coln, 
									const BsonOpts& opts,
									org_bson_error_type* perr, 
									const HandlerCollector& handler_collector)
	{
		return 
			this_type::s_collection_find_indexes_wrap(
				conn, str_db, str_coln, 
				detail::to_const_pointer<org_bson_type>(opts), 
				perr, 
				handler_collector);
	}

	template<typename Connection_T, typename HandlerCollector,
				typename String1, typename String2,
				typename BsonOpts> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_collection_find_indexes(Connection_T& conn, 
									const String1& str_db, 
									const String2& str_coln, 
									const BsonOpts& opts,
									org_bson_error_type& err, 
									const HandlerCollector& handler_collector)
	{
		return 
			this_type::s_collection_find_indexes_wrap(
				conn, str_db, str_coln, 
				detail::to_const_pointer<org_bson_type>(opts), 
				boost::addressof(err), 
				handler_collector);
	}

	// find_and_modify
private:
	// find_and_modify
	// doc say: findAndModify will only select one document to modify
	template<typename Connection_T,
				typename String1, typename String2 > inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_collection_find_and_modify_wrap(Connection_T& conn, 
											const String1& str_db, 
											const String2& str_coln, 
											const org_bson_type* pquery, 
											const org_bson_type* psort,
											const org_bson_type* pupdate, 
											const org_bson_type* pfields,
											bool bremove, bool bupsert, bool bnew, 
											org_bson_type* preply, 
											org_bson_error_type* perr)
	{
		typedef typename native_t<String1>::type string1_type;
		typedef typename native_t<String2>::type string2_type;

		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string1_type, utf8_string1_type);
		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string2_type, utf8_string2_type);

		utf8_string1_type utf8_str_db((charset::string_charset_helper_data(str_db)));
		utf8_string2_type utf8_str_coln((charset::string_charset_helper_data(str_coln)));

		return 
			base_type::s_org_collection_find_and_modify(
				conn, utf8_str_db.data(), utf8_str_coln.data(), 
				pquery, psort, pupdate, pfields, 
				bremove, bupsert, bnew, 
				preply, perr);
	}

public:
	template<typename Connection_T, 
				typename String1, typename String2,
				typename BsonQuery, typename BsonSort, 
				typename BsonUpdate, typename BsonFields > inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_collection_find_and_modify(Connection_T& conn, 
										const String1& str_db, 
										const String2& str_coln, 
										const BsonQuery& query, 
										const BsonSort& sort,
										const BsonUpdate& update, 
										const BsonFields& fields,
										bool bremove, bool bupsert, bool bnew, 
										org_bson_type* preply, 
										org_bson_error_type* perr = 0)
	{
		return 
			this_type::s_collection_find_and_modify_wrap(
				conn, str_db, str_coln, 
				detail::to_const_pointer<org_bson_type>(query),
				detail::to_const_pointer<org_bson_type>(sort),
				detail::to_const_pointer<org_bson_type>(update),
				detail::to_const_pointer<org_bson_type>(fields),
				bremove, bupsert, bnew, 
				preply, 
				perr);
	}

	template<typename Connection_T, 
				typename String1, typename String2,
				typename BsonQuery, typename BsonSort, 
				typename BsonUpdate, typename BsonFields> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_collection_find_and_modify(Connection_T& conn, 
										const String1& str_db, 
										const String2& str_coln, 
										const BsonQuery& query, 
										const BsonSort& sort,
										const BsonUpdate& update, 
										const BsonFields& fields,
										bool bremove, bool bupsert, bool bnew, 
										org_bson_type& reply, 
										org_bson_error_type* perr = 0)
	{
		return 
			this_type::s_collection_find_and_modify_wrap(
				conn, str_db, str_coln, 
				detail::to_const_pointer<org_bson_type>(query),
				detail::to_const_pointer<org_bson_type>(sort),
				detail::to_const_pointer<org_bson_type>(update),
				detail::to_const_pointer<org_bson_type>(fields),
				bremove, bupsert, bnew, 
				boost::addressof(reply), 
				perr);
	}

	template<typename Connection_T, 
				typename String1, typename String2,
				typename BsonQuery, typename BsonSort, 
				typename BsonUpdate, typename BsonFields> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_collection_find_and_modify(Connection_T& conn, 
										const String1& str_db, 
										const String2& str_coln, 
										const BsonQuery& query, 
										const BsonSort& sort,
										const BsonUpdate& update, 
										const BsonFields& fields,
										bool bremove, bool bupsert, bool bnew, 
										org_bson_type* preply, 
										org_bson_error_type& err)
	{
		return 
			this_type::s_collection_find_and_modify_wrap(
				conn, str_db, str_coln, 
				detail::to_const_pointer<org_bson_type>(query),
				detail::to_const_pointer<org_bson_type>(sort),
				detail::to_const_pointer<org_bson_type>(update),
				detail::to_const_pointer<org_bson_type>(fields),
				bremove, bupsert, bnew, 
				preply, 
				boost::addressof(err));
	}

	template<typename Connection_T, 
				typename String1, typename String2,
				typename BsonQuery, typename BsonSort, 
				typename BsonUpdate, typename BsonFields> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_collection_find_and_modify(Connection_T& conn, 
										const String1& str_db, 
										const String2& str_coln, 
										const BsonQuery& query, 
										const BsonSort& sort,
										const BsonUpdate& update, 
										const BsonFields& fields,
										bool bremove, bool bupsert, bool bnew, 
										org_bson_type& reply, 
										org_bson_error_type& err)
	{
		return 
			this_type::s_collection_find_and_modify_wrap(
				conn, str_db, str_coln, 
				detail::to_const_pointer<org_bson_type>(query),
				detail::to_const_pointer<org_bson_type>(sort),
				detail::to_const_pointer<org_bson_type>(update),
				detail::to_const_pointer<org_bson_type>(fields),
				bremove, bupsert, bnew, 
				boost::addressof(reply), 
				boost::addressof(err));
	}

	// find_and_modify_with_opt
private:
	template<typename Connection_T,
				typename String1, typename String2 > inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_collection_find_and_modify_with_opt_wrap(Connection_T& conn, 
													const String1& str_db, 
													const String2& str_coln, 
													const org_bson_type* pquery,
													const org_find_and_modify_opts_type* popts,
													org_bson_type* preply, 
													org_bson_error_type* perr)
	{
		typedef typename native_t<String1>::type string1_type;
		typedef typename native_t<String2>::type string2_type;

		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string1_type, utf8_string1_type);
		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string2_type, utf8_string2_type);

		utf8_string1_type utf8_str_db((charset::string_charset_helper_data(str_db)));
		utf8_string2_type utf8_str_coln((charset::string_charset_helper_data(str_coln)));

		return 
			base_type::s_org_collection_find_and_modify_with_opt(
				conn, utf8_str_db.data(), utf8_str_coln.data(), 
				pquery, popts, preply, perr);
	}

public:
	template<typename Connection_T,
				typename String1, typename String2,
				typename BsonQuery, typename FindOpts > inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_collection_find_and_modify(Connection_T& conn, 
										const String1& str_db, 
										const String2& str_coln, 
										const BsonQuery& query,
										const FindOpts& opts,
										org_bson_type* preply,
										org_bson_error_type* perr = 0)
	{
		return 
			this_type::s_collection_find_and_modify_with_opt_wrap(
				conn, str_db, str_coln, 
				detail::to_const_pointer<org_bson_type>(query),
				detail::to_const_pointer<org_find_and_modify_opts_type>(opts),
				preply, 
				perr);
	}

	template<typename Connection_T,
				typename String1, typename String2,
				typename BsonQuery, typename FindOpts> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_collection_find_and_modify(Connection_T& conn, 
										const String1& str_db, 
										const String2& str_coln, 
										const BsonQuery& query,
										const FindOpts& opts,
										org_bson_type& reply,
										org_bson_error_type* perr = 0)
	{
		return 
			this_type::s_collection_find_and_modify_with_opt_wrap(
				conn, str_db, str_coln, 
				detail::to_const_pointer<org_bson_type>(query),
				detail::to_const_pointer<org_find_and_modify_opts_type>(opts),
				boost::addressof(reply), 
				perr);
	}

	template<typename Connection_T,
				typename String1, typename String2,
				typename BsonQuery, typename FindOpts> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_collection_find_and_modify(Connection_T& conn, 
										const String1& str_db, 
										const String2& str_coln, 
										const BsonQuery& query,
										const FindOpts& opts,
										org_bson_type* preply,
										org_bson_error_type& err)
	{
		return 
			this_type::s_collection_find_and_modify_with_opt_wrap(
				conn, str_db, str_coln, 
				detail::to_const_pointer<org_bson_type>(query),
				detail::to_const_pointer<org_find_and_modify_opts_type>(opts),
				preply, 
				boost::addressof(err));
	}

	template<typename Connection_T,
				typename String1, typename String2,
				typename BsonQuery, typename FindOpts> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_collection_find_and_modify(Connection_T& conn, 
										const String1& str_db, 
										const String2& str_coln, 
										const BsonQuery& query,
										const FindOpts& opts,
										org_bson_type& reply,
										org_bson_error_type& err)
	{
		return 
			this_type::s_collection_find_and_modify_with_opt_wrap(
				conn, str_db, str_coln, 
				detail::to_const_pointer<org_bson_type>(query),
				detail::to_const_pointer<org_find_and_modify_opts_type>(opts),
				boost::addressof(reply), 
				boost::addressof(err));
	}

	// bulk_execute
private:
	template<typename Connection_T, 
				typename String1, typename String2,
				typename HandlerBulkOptMaker> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			u32
		>::type
		s_collection_bulk_execute_wrap(Connection_T& conn,
										const String1& str_db,
										const String2& str_coln,
										const org_bson_type* popts,
										const HandlerBulkOptMaker& handler_bulk_opt_maker, // commends maker
										org_bson_type* preply,
										org_bson_error_type* perr) // bulk execute params
	{
		typedef typename native_t<String1>::type string1_type;
		typedef typename native_t<String2>::type string2_type;

		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string1_type, utf8_string1_type);
		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string2_type, utf8_string2_type);

		utf8_string1_type utf8_str_db((charset::string_charset_helper_data(str_db)));
		utf8_string2_type utf8_str_coln((charset::string_charset_helper_data(str_coln)));

		return 
			base_type::s_org_collection_bulk_execute(
				conn, utf8_str_db.data(), utf8_str_coln.data(), 
				popts, handler_bulk_opt_maker, 
				preply, perr);
	}

public:
	template<typename Connection_T, 
				typename String1, typename String2,
				typename BsonOpts,
				typename HandlerBulkOptMaker> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			u32
		>::type
		s_collection_bulk_execute(Connection_T& conn, 
									const String1& str_db, 
									const String2& str_coln, 
									const BsonOpts& opts,
									const HandlerBulkOptMaker& handler_bulk_opt_maker, // commends maker
									org_bson_type* preply,
									org_bson_error_type* perr = 0)
	{
		return 
			this_type::s_collection_bulk_execute_wrap(
				conn, str_db, str_coln,
				detail::to_const_pointer<org_bson_type>(opts),
				handler_bulk_opt_maker, 
				preply, 
				perr);
	}

	template<typename Connection_T, 
				typename String1, typename String2,
				typename BsonOpts,
				typename HandlerBulkOptMaker> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			u32
		>::type
		s_collection_bulk_execute(Connection_T& conn, 
									const String1& str_db, 
									const String2& str_coln, 
									const BsonOpts& opts,
									const HandlerBulkOptMaker& handler_bulk_opt_maker, // commends maker
									org_bson_type& reply,
									org_bson_error_type* perr = 0)
	{
		return 
			this_type::s_collection_bulk_execute_wrap(
				conn, str_db, str_coln,
				detail::to_const_pointer<org_bson_type>(opts),
				handler_bulk_opt_maker, 
				boost::addressof(reply), 
				perr);
	}

	template<typename Connection_T, 
				typename String1, typename String2,
				typename BsonOpts,
				typename HandlerBulkOptMaker> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			u32
		>::type
		s_collection_bulk_execute(Connection_T& conn, 
									const String1& str_db, 
									const String2& str_coln, 
									const BsonOpts& opts,
									const HandlerBulkOptMaker& handler_bulk_opt_maker, // commends maker
									org_bson_type* preply,
									org_bson_error_type& err)
	{
		return 
			this_type::s_collection_bulk_execute_wrap(
				conn, str_db, str_coln,
				detail::to_const_pointer<org_bson_type>(opts),
				handler_bulk_opt_maker, 
				preply, 
				boost::addressof(err));
	}

	template<typename Connection_T, 
				typename String1, typename String2,
				typename BsonOpts,
				typename HandlerBulkOptMaker> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			u32
		>::type
		s_collection_bulk_execute(Connection_T& conn, 
									const String1& str_db, 
									const String2& str_coln, 
									const BsonOpts& opts,
									const HandlerBulkOptMaker& handler_bulk_opt_maker, // commends maker
									org_bson_type& reply,
									org_bson_error_type& err)
	{
		return 
			this_type::s_collection_bulk_execute_wrap(
				conn, str_db, str_coln,
				detail::to_const_pointer<org_bson_type>(opts),
				handler_bulk_opt_maker, 
				boost::addressof(reply), 
				boost::addressof(err));
	}

	// collection_watch
private:
	template<typename Connection_T,
				typename String1, typename String2,
				typename HandlerCollector> inline
	static bool s_collection_watch_wrap(Connection_T& conn, 
										const String1& str_db, 
										const String2& str_coln, 
										const org_bson_type* ppipeline,
										const org_bson_type* popts,
										org_bson_type* preply,
										org_bson_error_type* perr,
										const HandlerCollector& handler_collector)
	{
		typedef typename native_t<String1>::type string1_type;
		typedef typename native_t<String2>::type string2_type;

		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string1_type, utf8_string1_type);
		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string2_type, utf8_string2_type);

		utf8_string1_type utf8_str_db((charset::string_charset_helper_data(str_db)));
		utf8_string2_type utf8_str_coln((charset::string_charset_helper_data(str_coln)));

		return 
			base_type::s_org_collection_watch(
				conn, utf8_str_db.data(), utf8_str_coln.data(), 
				ppipeline, popts, 
				preply, perr, 
				handler_collector);
	}

public:
	template<typename Connection_T,
				typename String1, typename String2,
				typename BsonPipeline, typename BsonOpts,
				typename HandlerCollector> inline
	static bool s_collection_watch(Connection_T& conn, 
									const String1& str_db, 
									const String2& str_coln, 
									const BsonPipeline& pipeline,
									const BsonOpts& opts,
									org_bson_type* preply,
									org_bson_error_type* perr,
									const HandlerCollector& handler_collector)
	{
		return 
			this_type::s_collection_watch_wrap(
				conn, 
				str_db, str_coln,
				detail::to_const_pointer<org_bson_type>(pipeline),
				detail::to_const_pointer<org_bson_type>(opts),
				preply, perr, 
				handler_collector);
	}

	template<typename Connection_T,
				typename String1, typename String2,
				typename BsonPipeline, typename BsonOpts,
				typename HandlerCollector> inline
	static bool s_collection_watch(Connection_T& conn, 
									const String1& str_db, 
									const String2& str_coln, 
									const BsonPipeline& pipeline,
									const BsonOpts& opts,
									org_bson_type& reply,
									org_bson_error_type* perr,
									const HandlerCollector& handler_collector)
	{
		return 
			this_type::s_collection_watch_wrap(
				conn, 
				str_db, str_coln,
				detail::to_const_pointer<org_bson_type>(pipeline),
				detail::to_const_pointer<org_bson_type>(opts),
				boost::addressof(reply), 
				perr, 
				handler_collector);
	}

	template<typename Connection_T,
				typename String1, typename String2,
				typename BsonPipeline, typename BsonOpts,
				typename HandlerCollector> inline
	static bool s_collection_watch(Connection_T& conn, 
									const String1& str_db, 
									const String2& str_coln, 
									const BsonPipeline& pipeline,
									const BsonOpts& opts,
									org_bson_type* preply,
									org_bson_error_type& err,
									const HandlerCollector& handler_collector)
	{
		return 
			this_type::s_collection_watch_wrap(
				conn, 
				str_db, str_coln,
				detail::to_const_pointer<org_bson_type>(pipeline),
				detail::to_const_pointer<org_bson_type>(opts),
				preply, 
				boost::addressof(err), 
				handler_collector);
	}

	template<typename Connection_T,
				typename String1, typename String2,
				typename BsonPipeline, typename BsonOpts,
				typename HandlerCollector> inline
	static bool s_collection_watch(Connection_T& conn, 
									const String1& str_db, 
									const String2& str_coln, 
									const BsonPipeline& pipeline,
									const BsonOpts& opts,
									org_bson_type& reply,
									org_bson_error_type& err,
									const HandlerCollector& handler_collector)
	{
		return 
			this_type::s_collection_watch_wrap(
				conn, 
				str_db, str_coln,
				detail::to_const_pointer<org_bson_type>(pipeline),
				detail::to_const_pointer<org_bson_type>(opts),
				boost::addressof(reply), 
				boost::addressof(err), 
				handler_collector);
	}

public:
	template<typename Connection_T,
				typename String1, typename String2,
				typename BsonPipeline, typename BsonOpts,
				typename HandlerCollector> inline
	static bool s_watch(Connection_T& conn, 
						const String1& str_db, 
						const String2& str_coln, 
						const BsonPipeline& pipeline,
						const BsonOpts& opts,
						org_bson_type* preply,
						org_bson_error_type* perr,
						const HandlerCollector& handler_collector)
	{
		return 
			this_type::s_collection_watch_wrap(
				conn, 
				str_db, str_coln,
				detail::to_const_pointer<org_bson_type>(pipeline),
				detail::to_const_pointer<org_bson_type>(opts),
				preply, perr, 
				handler_collector);
	}

	template<typename Connection_T,
				typename String1, typename String2,
				typename BsonPipeline, typename BsonOpts,
				typename HandlerCollector> inline
	static bool s_watch(Connection_T& conn, 
						const String1& str_db, 
						const String2& str_coln, 
						const BsonPipeline& pipeline,
						const BsonOpts& opts,
						org_bson_type& reply,
						org_bson_error_type* perr,
						const HandlerCollector& handler_collector)
	{
		return 
			this_type::s_collection_watch_wrap(
				conn, 
				str_db, str_coln,
				detail::to_const_pointer<org_bson_type>(pipeline),
				detail::to_const_pointer<org_bson_type>(opts),
				boost::addressof(reply), 
				perr, 
				handler_collector);
	}

	template<typename Connection_T,
				typename String1, typename String2,
				typename BsonPipeline, typename BsonOpts,
				typename HandlerCollector> inline
	static bool s_watch(Connection_T& conn, 
						const String1& str_db, 
						const String2& str_coln, 
						const BsonPipeline& pipeline,
						const BsonOpts& opts,
						org_bson_type* preply,
						org_bson_error_type& err,
						const HandlerCollector& handler_collector)
	{
		return 
			this_type::s_collection_watch_wrap(
				conn, 
				str_db, str_coln,
				detail::to_const_pointer<org_bson_type>(pipeline),
				detail::to_const_pointer<org_bson_type>(opts),
				preply, 
				boost::addressof(err), 
				handler_collector);
	}

	template<typename Connection_T,
				typename String1, typename String2,
				typename BsonPipeline, typename BsonOpts,
				typename HandlerCollector> inline
	static bool s_watch(Connection_T& conn, 
						const String1& str_db, 
						const String2& str_coln, 
						const BsonPipeline& pipeline,
						const BsonOpts& opts,
						org_bson_type& reply,
						org_bson_error_type& err,
						const HandlerCollector& handler_collector)
	{
		return 
			this_type::s_collection_watch_wrap(
				conn, 
				str_db, str_coln,
				detail::to_const_pointer<org_bson_type>(pipeline),
				detail::to_const_pointer<org_bson_type>(opts),
				boost::addressof(reply), 
				boost::addressof(err), 
				handler_collector);
	}

	// collection_using_handler
private:
	template<typename Connection_T,
				typename String1, typename String2,
				typename Handler> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			typename func::foo_t_info<Handler>::result_type
		>::type
		s_collection_using_handler_wrap(Connection_T& conn, 
										const String1& str_db, 
										const String2& str_coln,
										org_bson_error_type* perr, 
										const Handler& handler)
	{
		typedef typename native_t<String1>::type string1_type;
		typedef typename native_t<String2>::type string2_type;

		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string1_type, utf8_string1_type);
		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string2_type, utf8_string2_type);

		utf8_string1_type utf8_str_db((charset::string_charset_helper_data(str_db)));
		utf8_string2_type utf8_str_coln((charset::string_charset_helper_data(str_coln)));

		return 
			base_type::s_org_collection_using_handler(
				conn, utf8_str_db.data(), utf8_str_coln.data(), 
				perr, handler);
	}

public:
	template<typename Connection_T, 
				typename String1, typename String2,
				typename Handler > inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			typename func::foo_t_info<Handler>::result_type
		>::type
		s_collection_using_handler(Connection_T& conn, 
									const String1& str_db, 
									const String2& str_coln,
									org_bson_error_type* perr, 
									const Handler& handler)
	{
		return this_type::s_collection_using_handler_wrap(conn, str_db, str_coln, perr, handler);
	}

	template<typename Connection_T, 
				typename String1, typename String2,
				typename Handler> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			typename func::foo_t_info<Handler>::result_type
		>::type
		s_collection_using_handler(Connection_T& conn, 
									const String1& str_db, 
									const String2& str_coln,
									org_bson_error_type& err, 
									const Handler& handler)
	{
		return this_type::s_collection_using_handler_wrap(conn, str_db, str_coln, boost::addressof(err), handler);
	}

public:
	template<typename Connection_T, 
				typename String1, typename String2,
				typename Handler > inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			typename func::foo_t_info<Handler>::result_type
		>::type
		s_using_handler(Connection_T& conn, 
						const String1& str_db, 
						const String2& str_coln,
						org_bson_error_type* perr, 
						const Handler& handler)
	{
		return this_type::s_collection_using_handler(conn, str_db, str_coln, perr, handler);
	}

	template<typename Connection_T, 
				typename String1, typename String2,
				typename Handler> inline
	static
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			typename func::foo_t_info<Handler>::result_type
		>::type
		s_using_handler(Connection_T& conn, 
						const String1& str_db, 
						const String2& str_coln,
						org_bson_error_type& err, 
						const Handler& handler)
	{
		return this_type::s_collection_using_handler(conn, str_db, str_coln, err, handler);
	}

	//------------------------collection e------------------------
};

} // namespace nsql_database_system
} // namespace yggr

#endif //__YGGR_NSQL_DATABASE_SYSTEM_MONGO_ACCESSER_COLLECTION_HPP__
