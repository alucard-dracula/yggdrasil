// c_mongo_client_encryption_datakey_opts.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_C_MONGO_CLIENT_ENCRYPTION_DATAKEY_OPTS_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_C_MONGO_CLIENT_ENCRYPTION_DATAKEY_OPTS_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/smart_ptr_ex/shared_ptr.hpp>

#include <yggr/ppex/swap_this_def.hpp>
#include <yggr/mplex/tag_sfinae.hpp>

#include <yggr/range_ex/range_value_ex.hpp>
#include <yggr/range_ex/range_iterator_ex.hpp>

#include <yggr/iterator_ex/is_iterator.hpp>
#include <yggr/iterator_ex/iterator.hpp>

#include <yggr/container/vector.hpp>

#include <yggr/charset/utf8_string.hpp>
#include <yggr/charset/utf8_string_view.hpp>

#include <yggr/utility/member_var_modify_helper.hpp>

#include <yggr/nsql_database_system/c_bson.hpp>
#include <yggr/nsql_database_system/mongoc_client_encryption_datakey_opts_native_ex.hpp>
#include <yggr/nsql_database_system/detail/binary_buffer_assign_helper.hpp>

#include <boost/utility/enable_if.hpp>
#include <boost/mpl/and.hpp>

namespace yggr
{
namespace nsql_database_system
{

//struct _mongoc_client_encryption_datakey_opts_t {
//   bson_t *masterkey;
//   char **keyaltnames;
//   uint32_t keyaltnames_count;
//   uint8_t *keymaterial;
//   uint32_t keymaterial_len;
//};

class c_mongo_client_encryption_datakey_opts
	: protected yggr_mongoc_client_encryption_datakey_opts
{
public:
	typedef yggr_mongoc_client_encryption_datakey_opts base_type;
	typedef mongoc_client_encryption_datakey_opts_t org_type;

	typedef charset::utf8_string inner_string_type;
	typedef charset::utf8_string_view inner_string_view_type;

	typedef inner_string_type::value_type inner_char_type;

	typedef c_bson bson_type;
	typedef charset::string bytes_buffer_type;

protected:
	typedef smart_ptr_ex::shared_ptr<inner_string_type> inner_string_ptr_type;
	typedef container::vector<inner_string_ptr_type> inner_string_ptr_vt_type;
	typedef container::vector<inner_char_type*> inner_char_ptr_vt_type;

private:
	typedef bytes_buffer_type::value_type byte_type;

private:
	typedef c_mongo_client_encryption_datakey_opts this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	typedef 
		utility::member_var_modify_helper
		<
			this_type,
			bson_type,
			bson_t,
			const bson_type&
		> bson_member_modify_helper_type;

	typedef 
		utility::member_var_modify_helper
		<
			this_type,
			bytes_buffer_type,
			bytes_buffer_type,
			const bytes_buffer_type&
		> bytes_buffer_member_modify_helper_type;

public:
	c_mongo_client_encryption_datakey_opts(void);

	template<typename KeyaltNameCont>
	explicit c_mongo_client_encryption_datakey_opts(const KeyaltNameCont& keyalt_name_cont,
													typename 
														boost::enable_if
														<
															container_ex::is_container<KeyaltNameCont>,
															mplex::sfinae_type
														>::type sfinae = 0)
	{
		mongoc_client_encryption_datakey_opts_native_ex::s_mongoc_client_encryption_datakey_opts_init(
			static_cast<base_type*>(this));
		this_type::set_keyaltnames(keyalt_name_cont);
	}

	template<typename KeyaltNameIter>
	c_mongo_client_encryption_datakey_opts(KeyaltNameIter s, KeyaltNameIter e,
											typename 
												boost::enable_if
												<
													iterator_ex::is_iterator<KeyaltNameIter>,
													mplex::sfinae_type
												>::type sfinae = 0)
	{
		mongoc_client_encryption_datakey_opts_native_ex::s_mongoc_client_encryption_datakey_opts_init(
			static_cast<base_type*>(this));
		this_type::set_keyaltnames(s, e);
	}

	template<typename BsonMasterKey, typename KeyaltNameCont, typename KeyMaterial>
	c_mongo_client_encryption_datakey_opts(const BsonMasterKey& bs_master_key, 
											const KeyaltNameCont& keyalt_name_cont,
											const KeyMaterial& key_material,
											typename 
												boost::enable_if
												<
													boost::mpl::and_
													<
														boost::is_base_of<bson_t, BsonMasterKey>,
														container_ex::is_container<KeyaltNameCont>,
														container_ex::is_container<KeyMaterial>
													>,
													mplex::sfinae_type
												>::type sfinae = 0)
	{
		mongoc_client_encryption_datakey_opts_native_ex::s_mongoc_client_encryption_datakey_opts_init(
			static_cast<base_type*>(this));

		this_type::pro_set_var_masterkey(bs_master_key);
		this_type::set_keyaltnames(keyalt_name_cont);
		this_type::set_keymaterial(key_material);
	}

	template<typename BsonMasterKey, typename KeyaltNameIter, typename KeyMaterial>
	c_mongo_client_encryption_datakey_opts(const BsonMasterKey& bs_master_key, 
											KeyaltNameIter keyalt_names_s,
											KeyaltNameIter keyalt_names_e,
											const KeyMaterial& key_material,
											typename 
												boost::enable_if
												<
													boost::mpl::and_
													<
														boost::is_base_of<bson_t, BsonMasterKey>,
														iterator_ex::is_iterator<KeyaltNameIter>,
														container_ex::is_container<KeyMaterial>
													>,
													mplex::sfinae_type
												>::type sfinae = 0)
	{
		mongoc_client_encryption_datakey_opts_native_ex::s_mongoc_client_encryption_datakey_opts_init(
			static_cast<base_type*>(this));

		this_type::pro_set_var_masterkey(bs_master_key);
		this_type::set_keyaltnames(keyalt_names_s, keyalt_names_e);
		this_type::set_keymaterial(key_material);
	}

	template<typename BsonMasterKey, typename KeyaltNameCont, typename KeyMaterialIter>
	c_mongo_client_encryption_datakey_opts(const BsonMasterKey& bs_master_key, 
											const KeyaltNameCont& keyalt_name_cont,
											KeyMaterialIter key_material_s,
											KeyMaterialIter key_material_e,
											typename 
												boost::enable_if
												<
													boost::mpl::and_
													<
														boost::is_base_of<bson_t, BsonMasterKey>,
														container_ex::is_container<KeyaltNameCont>,
														iterator_ex::is_iterator<KeyMaterialIter>
													>,
													mplex::sfinae_type
												>::type sfinae = 0)
	{
		mongoc_client_encryption_datakey_opts_native_ex::s_mongoc_client_encryption_datakey_opts_init(
			static_cast<base_type*>(this));

		this_type::pro_set_var_masterkey(bs_master_key);
		this_type::set_keyaltnames(keyalt_name_cont);
		this_type::set_keymaterial(key_material_s, key_material_e);
	}

	template<typename BsonMasterKey, typename KeyaltNameIter, typename KeyMaterialIter>
	c_mongo_client_encryption_datakey_opts(const BsonMasterKey& bs_master_key, 
											KeyaltNameIter keyalt_names_s,
											KeyaltNameIter keyalt_names_e,
											KeyMaterialIter key_material_s,
											KeyMaterialIter key_material_e,
											typename 
												boost::enable_if
												<
													boost::mpl::and_
													<
														boost::is_base_of<bson_t, BsonMasterKey>,
														iterator_ex::is_iterator<KeyaltNameIter>,
														iterator_ex::is_iterator<KeyMaterialIter>
													>,
													mplex::sfinae_type
												>::type sfinae = 0)
	{
		mongoc_client_encryption_datakey_opts_native_ex::s_mongoc_client_encryption_datakey_opts_init(
			static_cast<base_type*>(this));

		this_type::pro_set_var_masterkey(bs_master_key);
		this_type::set_keyaltnames(keyalt_names_s, keyalt_names_e);
		this_type::set_keymaterial(key_material_s, key_material_e);
	}

	explicit c_mongo_client_encryption_datakey_opts(const org_type* porg);
	c_mongo_client_encryption_datakey_opts(const org_type& org);

	c_mongo_client_encryption_datakey_opts(BOOST_RV_REF(this_type) right)
	{
		mongoc_client_encryption_datakey_opts_native_ex::s_mongoc_client_encryption_datakey_opts_init(
			static_cast<base_type*>(this));
		this_type& right_ref = right;
		this_type::pro_move(right_ref);
	}

	c_mongo_client_encryption_datakey_opts(const this_type& right);
	~c_mongo_client_encryption_datakey_opts(void);

public:
	this_type& operator=(const org_type& right);
	this_type& operator=(const base_type& right);

	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		return this_type::pro_move(right_ref);
	}

	this_type& operator=(const this_type& right);

public:
	inline void swap(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		this_type::swap(right_ref);
	}

	void swap(this_type& right);

public:
	inline org_type* org_pointer(void)
	{
		base_type& base = *this;
		return reinterpret_cast<org_type*>(boost::addressof(base));
	}

	inline const org_type* org_pointer(void) const
	{
		const base_type& base = *this;
		return reinterpret_cast<const org_type*>(boost::addressof(base));
	}

public:
	inline operator org_type&(void)
	{
		return *(this_type::org_pointer());
	}

	inline operator const org_type&(void) const
	{
		return *(this_type::org_pointer());
	}

	inline operator org_type*(void)
	{
		return this_type::org_pointer();
	}

	inline operator const org_type*(void) const
	{
		return this_type::org_pointer();
	}

	// var_masterkey
protected: 
	template<typename Bson> inline 
	typename boost::enable_if<boost::is_base_of<bson_t, Bson>, const Bson&>::type 
		pro_get_var_masterkey(void) const 
	{ 
		typedef Bson now_bson_type; 
		return reinterpret_cast<const now_bson_type&>(_masterkey); 
	} 
		
	inline void pro_set_var_masterkey(const bson_t& arg_masterkey)
	{ 
		_masterkey = arg_masterkey; 
		base_type::masterkey = _masterkey.empty()? 0 : _masterkey.org_pointer();
	} 
	
public: 
	inline bson_member_modify_helper_type var_masterkey(void) 
	{ 
		return 
			bson_member_modify_helper_type( 
				*this, _masterkey, 
				&this_type::pro_set_var_masterkey, 
				&this_type::pro_get_var_masterkey<bson_type>); 
	} 
		
	template<typename Bson> inline 
	typename 
		boost::enable_if
		< 
			boost::is_base_of<bson_t, Bson>, 
			utility::member_var_modify_helper
			< 
				this_type, 
				bson_type, 
				bson_t, 
				const Bson& 
			> 
		>::type 
		var_masterkey(void) 
	{ 
		typedef Bson now_bson_type; 
		typedef 
			utility::member_var_modify_helper
			< 
				this_type, 
				bson_type, 
				bson_t, 
				const now_bson_type& 
			> now_bson_member_modify_helper_type; 
			
		return 
			now_bson_member_modify_helper_type( 
				*this, _masterkey, 
				&this_type::pro_set_var_masterkey, 
				&this_type::pro_get_var_masterkey<now_bson_type>);
	} 
		
	inline const bson_type& var_masterkey(void) const 
	{ 
		return this_type::pro_get_var_masterkey<bson_type>(); 
	} 
		
	template<typename Bson> inline 
	typename boost::enable_if<boost::is_base_of<bson_t, Bson>, const Bson&>::type 
		var_masterkey(void) const 
	{ 
		typedef Bson now_bson_type; 
		return this_type::pro_get_var_masterkey<now_bson_type>(); 
	}

	// var_keyaltnames
public:
	inline const inner_string_ptr_vt_type& var_keyaltnames(void) const
	{
		return _keyaltnames_datas;
	}

	template<typename InputIter>
	std::size_t set_keyaltnames(InputIter s, InputIter e)
	{
		typedef InputIter iter_type;
		
		inner_string_ptr_vt_type datas;
		inner_char_ptr_vt_type views;

		assert(datas.size() == views.size());

		for(; s != e; ++s)
		{
			this_type::pro_s_append_keyaltname(datas, views, *s);
		}

		assert(datas.size() == views.size());

		_keyaltnames_datas.swap(datas);
		_keyaltnames.swap(views);
		base_type::keyaltnames = boost::addressof(_keyaltnames[0]);
		base_type::keyaltnames_count = _keyaltnames.size();

		return base_type::keyaltnames_count;
	}

	template<typename Container> inline
	std::size_t set_keyaltnames(const Container& cont)
	{
		return this_type::set_keyaltnames(boost::begin(cont), boost::end(cont));
	}

	template<typename String> inline
	bool append_keyaltname(const String& name)
	{
		return
			this_type::pro_s_append_keyaltname(_keyaltnames_datas, _keyaltnames, name)
			&& (base_type::keyaltnames = boost::addressof(_keyaltnames[0]), 
					base_type::keyaltnames_count = _keyaltnames.size(), 
					true);
	}

	inline void clear_keyaltnames(void)
	{
		base_type::keyaltnames = 0;
		base_type::keyaltnames_count = 0;
		_keyaltnames.clear();
		_keyaltnames_datas.clear();
	}

	inline u32 var_keyaltnames_count(void) const
	{
		assert(
			(_keyaltnames.size() == _keyaltnames_datas.size())
			&& (base_type::keyaltnames_count == _keyaltnames.size()) );

		return base_type::keyaltnames_count;
	}

	//var_keymaterial
protected:
	inline const bytes_buffer_type& pro_get_var_keymaterial(void) const 
	{
		return _keymaterial;
	} 
		
	inline void pro_set_var_keymaterial(const bytes_buffer_type& arg_keymaterial)
	{ 
		_keymaterial = arg_keymaterial; 
		base_type::keymaterial_len = _keymaterial.size();
		base_type::keymaterial = 
			reinterpret_cast<uint8_t*>(
				_keymaterial.size()? const_cast<byte_type*>(_keymaterial.data()) : 0);
	} 

public:
	inline bytes_buffer_member_modify_helper_type var_keymaterial(void) 
	{ 
		return 
			bytes_buffer_member_modify_helper_type( 
				*this, _keymaterial, 
				&this_type::pro_set_var_keymaterial, 
				&this_type::pro_get_var_keymaterial); 
	} 
		
	inline const bytes_buffer_type& var_keymaterial(void) const 
	{ 
		return _keymaterial; 
	}

public:
	inline void set_keymaterial(const u8* pdata, u64 len)
	{
		if(!(pdata && len))
		{
			container::clear(_keymaterial);
			return;
		}

		container::resize(_keymaterial, len);
		memcpy(boost::addressof(_keymaterial[0]), pdata, len);
		base_type::keymaterial_len = _keymaterial.size();
		base_type::keymaterial = 
			reinterpret_cast<uint8_t*>(
				_keymaterial.size()? const_cast<byte_type*>(_keymaterial.data()) : 0);
	}

	template<typename InputIter> inline
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_iterator<InputIter>,
				boost::mpl::not_
				<
					boost::is_class
					<
						typename
							boost::remove_cv
							<
								typename iterator_ex::iterator_value_ex<InputIter>::type
							>::type
					>
				>
			>,
			void
		>::type
		set_keymaterial(InputIter s, InputIter e)
	{
		typedef InputIter iter_type;
		typedef 
			detail::binary_buffer_assign_iterator_helper
			<
				bytes_buffer_type, 
				iter_type
			> h_type;

		h_type h;
		h(_keymaterial, s, e);
		base_type::keymaterial_len = _keymaterial.size();
		base_type::keymaterial = 
			reinterpret_cast<uint8_t*>(
				_keymaterial.size()? const_cast<byte_type*>(_keymaterial.data()) : 0);
	}

	inline void set_keymaterial(BOOST_RV_REF(bytes_buffer_type) arg_keymaterial)
	{
		copy_or_move_or_swap(_keymaterial, boost::move(arg_keymaterial));
		base_type::keymaterial_len = _keymaterial.size();
		base_type::keymaterial = 
			reinterpret_cast<uint8_t*>(
				_keymaterial.size()? const_cast<byte_type*>(_keymaterial.data()) : 0);
	}

	inline void set_keymaterial(const bytes_buffer_type& arg_keymaterial)
	{
		_keymaterial = arg_keymaterial;
		base_type::keymaterial_len = _keymaterial.size();
		base_type::keymaterial = 
			reinterpret_cast<uint8_t*>(
				_keymaterial.size()? const_cast<byte_type*>(_keymaterial.data()) : 0);
	}

	template<typename Container> inline
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				container_ex::is_container<Container>,
				boost::mpl::not_
				<
					boost::is_class
					<
						typename range_ex::range_value_ex<Container>::type
					>
				>
			>,
			void
		>::type
		set_keymaterial(const Container& cont)
	{
		typedef Container cv_cont_type;
		typedef typename boost::remove_cv<cv_cont_type>::type cont_type;
		
		typedef 
			detail::binary_buffer_assign_container_helper
			<
				bytes_buffer_type,
				cont_type
			> h_type;

		h_type h;
		h(_keymaterial, cont);
		base_type::keymaterial_len = _keymaterial.size();
		base_type::keymaterial = 
			reinterpret_cast<uint8_t*>(
				_keymaterial.size()? const_cast<byte_type*>(_keymaterial.data()) : 0);
	}

	inline u32 var_keymaterial_size(void) const
	{
		assert( (base_type::keymaterial_len == _keymaterial.size()) );

		return base_type::keymaterial_len;
	}

protected:
	void pro_rebuild_base(void);
	this_type& pro_copy(const org_type* porg);
	this_type& pro_copy(const this_type& right);
	void pro_swap(this_type& right);
	this_type& pro_move(this_type& right);

protected:
	template<typename String> inline
	static bool pro_s_append_keyaltname(inner_string_ptr_vt_type& datas, inner_char_ptr_vt_type& views, const String& name)
	{
		typedef String now_string_type;
		YGGR_TYPEDEF_CONST_UTF8_STRING_OR_STRING_VIEW_TYPE_TPL(now_string_type, now_inner_string_type);

		assert(datas.size() == views.size());

		if(datas.size() != views.size())
		{
			return false;
		}

		now_inner_string_type str_name((charset::string_charset_helper_data(name)));
		
		if(str_name.empty())
		{
			return false;
		}

		inner_string_ptr_type pstr_name(yggr_nothrow_new inner_string_type(str_name));
		assert(pstr_name);
		if(!pstr_name)
		{
			return false;
		}

		datas.push_back(pstr_name);
		views.push_back(pstr_name->data());

		assert(datas.size() == views.size());

		return (datas.size() == views.size());
	}

	static void pro_s_copy_keyaltname(inner_string_ptr_vt_type& dst_datas, inner_char_ptr_vt_type& dst_views, 
										const inner_string_ptr_vt_type& src_datas);

	static void pro_s_swap_keyaltname(inner_string_ptr_vt_type& ldatas, inner_char_ptr_vt_type& lviews, 
										inner_string_ptr_vt_type& rdatas, inner_char_ptr_vt_type& rviews);

private:
	bson_type _masterkey;

	inner_string_ptr_vt_type _keyaltnames_datas;
	inner_char_ptr_vt_type _keyaltnames;
	//uint32_t keyaltnames_count; // using base_type::keyaltnames_count
	
	bytes_buffer_type _keymaterial;
	//uint32_t keymaterial_len; // using base_type::keymaterial_len
};

} // namespace nsql_database_system
} // namespace yggr

namespace yggr
{
namespace nsql_database_system
{
namespace swap_support
{
	YGGR_PP_INLINE_SWAP_THIS_REF_DEF_IMPL(c_mongo_client_encryption_datakey_opts)
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

#endif // __YGGR_NSQL_DATABASE_SYSTEM_C_MONGO_CLIENT_ENCRYPTION_DATAKEY_OPTS_HPP__
