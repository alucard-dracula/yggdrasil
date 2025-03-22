// mongo_bulk_operation_helper.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_MONGO_BULK_OPERATION_HELPER_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_MONGO_BULK_OPERATION_HELPER_HPP__

#include <yggr/base/yggrdef.h>

#include <yggr/nsql_database_system/mongodb_config.hpp>
#include <yggr/nsql_database_system/c_bson.hpp>
#include <yggr/nsql_database_system/c_bson_value.hpp>
#include <yggr/nsql_database_system/c_bson_error.hpp>
#include <yggr/nsql_database_system/c_mongo_write_concern.hpp>

#include <yggr/nsql_database_system/detail/to_pointer.hpp>

#include <boost/ref.hpp>

namespace yggr
{
namespace nsql_database_system
{

class mongo_bulk_operation_helper
{
public:

	typedef c_bson bson_type;
	typedef bson_type::org_type org_bson_type;

	typedef c_bson_value bson_value_type;
	typedef bson_value_type::org_type org_bson_value_type;

	typedef c_bson_error bosn_error_type;
	typedef bosn_error_type::org_type org_bson_error_type;

	typedef c_mongo_write_concern write_concern_type;
	typedef write_concern_type::org_type org_write_concern_type;

	typedef mongoc_bulk_operation_t org_bulk_opt_type;

private:
	typedef mongo_bulk_operation_helper this_type;

	// op insert
private:
	inline static void s_op_insert_wrap(org_bulk_opt_type* pbulk, const org_bson_type* pdoc)
	{
		assert((pbulk && pdoc));
		if(!(pbulk && pdoc))
		{
			return;
		}
		::mongoc_bulk_operation_insert(pbulk, pdoc);
	}

	inline static bool s_op_insert_wrap(org_bulk_opt_type* pbulk, 
										const org_bson_type* pdoc,
										const org_bson_type* popts, 
										org_bson_error_type* perr)
	{
		assert((pbulk && pdoc));
		return 
			pbulk && pdoc
			&& ::mongoc_bulk_operation_insert_with_opts(pbulk, pdoc, popts, perr);
	}

public:
	template<typename BsonDoc> inline
	static void s_op_insert(org_bulk_opt_type* pbulk, const BsonDoc& doc)
	{
		this_type::s_op_insert_wrap(pbulk, detail::to_const_pointer<org_bson_type>(doc));
	}

	template<typename BsonDoc, typename BsonOpts> inline
	static bool s_op_insert(org_bulk_opt_type* pbulk, const BsonDoc& doc, 
							const BsonOpts& opts, org_bson_error_type* perr)
	{
		return 
			this_type::s_op_insert_wrap(
				pbulk, 
				detail::to_const_pointer<org_bson_type>(doc), 
				detail::to_const_pointer<org_bson_type>(opts),
				perr);
	}

	template<typename BsonDoc, typename BsonOpts> inline
	static bool s_op_insert(org_bulk_opt_type* pbulk, const BsonDoc& doc, 
							const BsonOpts& opts, org_bson_error_type& err)
	{
		return 
			this_type::s_op_insert_wrap(
				pbulk, 
				detail::to_const_pointer<org_bson_type>(doc), 
				detail::to_const_pointer<org_bson_type>(opts),
				boost::addressof(err));
	}

	// op remove
private:
	inline static void s_op_remove_wrap(org_bulk_opt_type* pbulk, const org_bson_type* pselector)
	{
		assert((pbulk && pselector));
		if(!(pbulk && pselector))
		{
			return;
		}

		::mongoc_bulk_operation_remove(pbulk, pselector);
	}

	inline static bool s_op_remove_wrap(org_bulk_opt_type* pbulk,
										const org_bson_type* pselector,
										const org_bson_type* popts,
										org_bson_error_type* perr)
	{
		assert((pbulk && pselector));

		return 
			pbulk && pselector
			&& mongoc_bulk_operation_remove_many_with_opts(pbulk, pselector, popts, perr);
	}

public:
	template<typename Bson> inline
	static void s_op_remove(org_bulk_opt_type* pbulk, const Bson& selector)
	{
		this_type::s_op_remove_wrap(pbulk, detail::to_const_pointer<org_bson_type>(selector));
	}

	template<typename Bson, typename BsonOpts> inline
	static bool s_op_remove(org_bulk_opt_type* pbulk,
							const Bson& selector,
							const BsonOpts& opts,
							org_bson_error_type* perr)
	{
		return 
			this_type::s_op_remove_wrap(
				pbulk, 
				detail::to_const_pointer<org_bson_type>(selector),
				detail::to_const_pointer<org_bson_type>(opts),
				perr);
	}

	template<typename Bson, typename BsonOpts> inline
	static bool s_op_remove(org_bulk_opt_type* pbulk,
							const Bson& selector,
							const BsonOpts& opts,
							org_bson_error_type& err)
	{
		return 
			this_type::s_op_remove_wrap(
				pbulk, 
				detail::to_const_pointer<org_bson_type>(selector),
				detail::to_const_pointer<org_bson_type>(opts),
				boost::addressof(err));
	}

	// op remove one
private:
	inline static void s_op_remove_one_wrap(org_bulk_opt_type* pbulk, const org_bson_type* pselector)
	{
		assert((pbulk && pselector));
		if(!(pbulk && pselector))
		{
			return;
		}

		::mongoc_bulk_operation_remove_one(pbulk, pselector);
	}

	inline static bool s_op_remove_one_wrap(org_bulk_opt_type* pbulk, 
											const org_bson_type* pselector,
											const org_bson_type* popts,
											org_bson_error_type* perr)
	{
		assert((pbulk && pselector));

		return
			pbulk && pselector
			&& mongoc_bulk_operation_remove_one_with_opts(pbulk, pselector, popts, perr);
	}

public:
	template<typename Bson> inline
	static void s_op_remove_one(org_bulk_opt_type* pbulk, const Bson& selector)
	{
		this_type::s_op_remove_one_wrap(pbulk, detail::to_const_pointer<org_bson_type>(selector));
	}

	template<typename Bson, typename BsonOpts> inline
	static bool s_op_remove_one(org_bulk_opt_type* pbulk, const Bson& selector,
								const BsonOpts& opts, org_bson_error_type* perr)
	{
		return
			this_type::s_op_remove_one_wrap(
				pbulk, 
				detail::to_const_pointer<org_bson_type>(selector),
				detail::to_const_pointer<org_bson_type>(opts),
				perr);
	}

	template<typename Bson, typename BsonOpts> inline
	static bool s_op_remove_one(org_bulk_opt_type* pbulk, const Bson& selector,
								const BsonOpts& opts, org_bson_error_type& err)
	{
		return
			this_type::s_op_remove_one_wrap(
				pbulk, 
				detail::to_const_pointer<org_bson_type>(selector),
				detail::to_const_pointer<org_bson_type>(opts),
				boost::addressof(err));
	}

	// op update
private:
	inline static void s_op_update_wrap(org_bulk_opt_type* pbulk, 
										const org_bson_type* pselector,
										const org_bson_type* pdoc,
										bool upsert)
	{
		assert((pbulk && pselector && pdoc));
		if(!(pbulk && pselector && pdoc))
		{
			return;
		}

		::mongoc_bulk_operation_update(pbulk, pselector, pdoc, upsert);
	}

	inline static bool s_op_update_wrap(org_bulk_opt_type* pbulk, 
										const org_bson_type* pselector,
										const org_bson_type* pdoc,
										const org_bson_type* popts,
										org_bson_error_type* perr)
	{
		assert((pbulk && pselector && pdoc));
		return 
			pbulk && pselector && pdoc
			&& mongoc_bulk_operation_update_many_with_opts(pbulk, pselector, pdoc, popts, perr);
	}


public:
	template<typename BsonSel, typename BsonDoc> inline
	static void s_op_update(org_bulk_opt_type* pbulk, 
							const BsonSel& selector,
							const BsonDoc& doc,
							bool upsert)
	{
		this_type::s_op_update_wrap(
			pbulk,
			detail::to_const_pointer<org_bson_type>(selector), 
			detail::to_const_pointer<org_bson_type>(doc), 
			upsert);
	}

	template<typename BsonSel, typename BsonDoc, typename BsonOpts> inline
	static bool s_op_update(org_bulk_opt_type* pbulk, 
							const BsonSel& selector,
							const BsonDoc& doc,
							const BsonOpts& opts,
							org_bson_error_type* perr)
	{
		return
			this_type::s_op_update_wrap(
				pbulk,
				detail::to_const_pointer<org_bson_type>(selector), 
				detail::to_const_pointer<org_bson_type>(doc), 
				detail::to_const_pointer<org_bson_type>(opts),
				perr);
	}

	template<typename BsonSel, typename BsonDoc, typename BsonOpts> inline
	static bool s_op_update(org_bulk_opt_type* pbulk, 
							const BsonSel& selector,
							const BsonDoc& doc,
							const BsonOpts& opts,
							org_bson_error_type& err)
	{
		return
			this_type::s_op_update_wrap(
				pbulk,
				detail::to_const_pointer<org_bson_type>(selector), 
				detail::to_const_pointer<org_bson_type>(doc), 
				detail::to_const_pointer<org_bson_type>(opts),
				boost::addressof(err));
	}

	// op update one
private:
	inline static void s_op_update_one_wrap(org_bulk_opt_type* pbulk, 
											const org_bson_type* pselector,
											const org_bson_type* pdoc,
											bool upsert)
	{
		assert((pbulk && pselector && pdoc));
		if(!(pbulk && pselector && pdoc))
		{
			return;
		}

		::mongoc_bulk_operation_update_one(pbulk, pselector, pdoc, upsert);
	}

	inline static bool s_op_update_one_wrap(org_bulk_opt_type* pbulk, 
											const org_bson_type* pselector,
											const org_bson_type* pdoc,
											const org_bson_type* popts,
											org_bson_error_type* perr)
	{
		assert((pbulk && pselector && pdoc));

		return 
			pbulk && pselector && pdoc
			&& mongoc_bulk_operation_update_one_with_opts(pbulk, pselector, pdoc, popts, perr);
	}

public:
	template<typename BsonSel, typename BsonDoc> inline
	static void s_op_update_one(org_bulk_opt_type* pbulk, 
								const BsonSel& selector,
								const BsonDoc& doc,
								bool upsert)
	{
		this_type::s_op_update_one_wrap(
			pbulk,
			detail::to_const_pointer<org_bson_type>(selector), 
			detail::to_const_pointer<org_bson_type>(doc), 
			upsert);
	}

	template<typename BsonSel, typename BsonDoc, typename BsonOpts> inline
	static bool s_op_update_one(org_bulk_opt_type* pbulk, 
								const BsonSel& selector,
								const BsonDoc& doc,
								const BsonOpts& opts,
								org_bson_error_type* perr)
	{
		return
			this_type::s_op_update_one_wrap(
				pbulk,
				detail::to_const_pointer<org_bson_type>(selector), 
				detail::to_const_pointer<org_bson_type>(doc),
				detail::to_const_pointer<org_bson_type>(opts), 
				perr);
	}

	template<typename BsonSel, typename BsonDoc, typename BsonOpts> inline
	static bool s_op_update_one(org_bulk_opt_type* pbulk, 
								const BsonSel& selector,
								const BsonDoc& doc,
								const BsonOpts& opts,
								org_bson_error_type& err)
	{
		return
			this_type::s_op_update_one_wrap(
				pbulk,
				detail::to_const_pointer<org_bson_type>(selector), 
				detail::to_const_pointer<org_bson_type>(doc),
				detail::to_const_pointer<org_bson_type>(opts), 
				boost::addressof(err));
	}

	// replace one
private:
	inline static void s_op_replace_one_wrap(org_bulk_opt_type* pbulk, 
												const org_bson_type* pselector,
												const org_bson_type* pdoc,
												bool upsert)
	{
		assert((pbulk && pselector && pdoc));
		if(!(pbulk && pselector && pdoc))
		{
			return;
		}

		::mongoc_bulk_operation_replace_one(pbulk, pselector, pdoc, upsert);
	}

	inline static bool s_op_replace_one_wrap(org_bulk_opt_type* pbulk, 
												const org_bson_type* pselector,
												const org_bson_type* pdoc,
												const org_bson_type* popts,
												org_bson_error_type* perr)
	{
		assert((pbulk && pselector && pdoc));

		return
			pbulk && pselector && pdoc
			&& mongoc_bulk_operation_replace_one_with_opts(pbulk, pselector, pdoc, popts, perr);
	}

public:
	template<typename BsonSel, typename BsonDoc> inline
	static void s_op_replace_one(org_bulk_opt_type* pbulk, 
									const BsonSel& selector,
									const BsonDoc& doc,
									bool upsert)
	{
		this_type::s_op_replace_one_wrap(
			pbulk, 
			detail::to_const_pointer<org_bson_type>(selector), 
			detail::to_const_pointer<org_bson_type>(doc),
			upsert);
	}

	template<typename BsonSel, typename BsonDoc, typename BsonOpts> inline
	static bool s_op_replace_one(org_bulk_opt_type* pbulk, 
									const BsonSel& selector,
									const BsonDoc& doc,
									const BsonOpts& opts,
									org_bson_error_type* perr)
	{
		return
			this_type::s_op_replace_one_wrap(
				pbulk, 
				detail::to_const_pointer<org_bson_type>(selector), 
				detail::to_const_pointer<org_bson_type>(doc),
				detail::to_const_pointer<org_bson_type>(opts),
				perr);
	}

	template<typename BsonSel, typename BsonDoc, typename BsonOpts> inline
	static bool s_op_replace_one(org_bulk_opt_type* pbulk, 
									const BsonSel& selector,
									const BsonDoc& doc,
									const BsonOpts& opts,
									org_bson_error_type& err)
	{
		return
			this_type::s_op_replace_one_wrap(
				pbulk, 
				detail::to_const_pointer<org_bson_type>(selector), 
				detail::to_const_pointer<org_bson_type>(doc),
				detail::to_const_pointer<org_bson_type>(opts),
				boost::addressof(err));
	}

public:
	// get_write_concern
	inline static u32 s_op_get_hint(const org_bulk_opt_type* pbulk)
	{
		return ::mongoc_bulk_operation_get_hint(pbulk);
	}

	// get_write_concern
	inline static const write_concern_type* s_op_get_write_concern(const org_bulk_opt_type* pbulk)
	{
		const org_write_concern_type* pwcon = ::mongoc_bulk_operation_get_write_concern(pbulk);
		return reinterpret_cast<const write_concern_type*>(pwcon);
	}

public:
	// set_bypass_document_validation
	inline static void s_op_set_bypass_document_validation(org_bulk_opt_type* pbulk, bool bypass)
	{
		assert((pbulk));
		if(!(pbulk))
		{
			return;
		}

		::mongoc_bulk_operation_set_bypass_document_validation(pbulk, bypass);
	}

	// set_client_session
	inline static void s_op_set_client_session(org_bulk_opt_type* pbulk, 
												mongoc_client_session_t* pclient_session)
	{
		assert((pbulk));
		if(!(pbulk))
		{
			return;
		}

		::mongoc_bulk_operation_set_client_session(pbulk, pclient_session);
	}

	// set_comment
	template<typename BsonValue> inline
	static void s_op_set_comment(org_bulk_opt_type* pbulk, const BsonValue& comment)
	{
		assert((pbulk));
		if(!(pbulk))
		{
			return;
		}

		::mongoc_bulk_operation_set_comment(pbulk, detail::to_const_pointer<org_bson_value_type>(comment));
	}

	//set_hint
	inline static void s_op_set_hint(org_bulk_opt_type* pbulk, uint32_t server_id)
	{
		assert((pbulk));
		if(!(pbulk))
		{
			return;
		}

		::mongoc_bulk_operation_set_hint(pbulk, server_id);
	}

	// set_let
	template<typename BsonLet> inline
	static void s_op_set_let(org_bulk_opt_type* pbulk, const BsonLet& let)
	{
		assert((pbulk));
		if(!(pbulk))
		{
			return;
		}

		::mongoc_bulk_operation_set_hint(pbulk, detail::to_const_pointer<org_bson_type>(let));
	}

};

} // namespace nsql_database_system
} // namespace yggr

#endif // __YGGR_NSQL_DATABASE_SYSTEM_MONGO_BULK_OPERATION_HELPER_HPP__