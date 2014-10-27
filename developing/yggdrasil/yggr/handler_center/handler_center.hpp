//handler_center.hpp

#ifndef __YGGR_HANDLER_CENTER_HANDLER_CENTER_HPP__
#define __YGGR_HANDLER_CENTER_HANDLER_CENTER_HPP__

#include <cassert>

#include <yggr/utility/swap.hpp>
#include <boost/any.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/bind.hpp>
#include <boost/thread/mutex.hpp>

#include <boost/smart_ptr/enable_shared_from_this.hpp>
#include <boost/smart_ptr/enable_shared_from_raw.hpp>

#include <boost/type_traits/is_same.hpp>

#include <boost/mpl/assert.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/not.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/logical.hpp>

#include <yggr/move/move.hpp>
#include <yggr/base/error_make.hpp>
#include <yggr/base/exception_sort.hpp>
#include <yggr/base/yggrdef.h>
#include <yggr/base/interface_ptr.hpp>

#include <yggr/charset/string.hpp>
#include <yggr/ppex/foo_params.hpp>

//#include <yggr/mplex/conv_to_tuple.hpp>

#include <yggr/packet/packet_info.hpp>

#include <yggr/safe_container/safe_unordered_map.hpp>


namespace yggr
{
namespace handler_center
{

template<typename Key, typename Mutex = boost::mutex>
class handler_center
{
public:
	//class error S----------------------------------------------------------

	ERROR_MAKER_BEGIN("handler_center")
		ERROR_CODE_DEF_BEGIN(exception::exception_sort::E_code_start_9)
			ERROR_CODE_DEF(E_handler_invoke_return_failed)
			ERROR_CODE_DEF(E_handler_invoke_failed)
			ERROR_CODE_DEF(E_handler_not_found)
		ERROR_CODE_DEF_END(exception::exception_sort::E_code_end_9)

		ERROR_CODE_MSG_BEGIN()
			ERROR_CODE_MSG(E_handler_invoke_return_failed, "handler invoke return failed")
			ERROR_CODE_MSG(E_handler_invoke_failed, "handler invoke failed")
			ERROR_CODE_MSG(E_handler_not_found, "handler not found")
		ERROR_CODE_MSG_END()
	ERROR_MAKER_END()

	//class error E----------------------------------------------------------
public:
	class basic_handler_container
	{
	public:
		virtual ~basic_handler_container(void)
		{
		}

		template<typename Ret>
		Ret invoke(void) const
		{
			typedef Ret ret_type;
			boost::any params;
			boost::any tmp_ret;
			bool bret = this->detail_invoke(tmp_ret, params);
			if(!bret)
			{
				throw error_maker_type::make_error(error_maker_type::E_handler_invoke_failed);
			}
			ret_type* pret = boost::any_cast<ret_type>(&tmp_ret);
			if(!pret)
			{
				throw error_maker_type::make_error(error_maker_type::E_handler_invoke_return_failed);
			}

			return *pret;
		}

		void invoke(error_type& e) const
		{
			assert(!e);
			boost::any params;
			boost::any tmp_ret;
			bool bret = this->detail_invoke(tmp_ret, params);
			if(!bret)
			{
				e = error_maker_type::make_error(error_maker_type::E_handler_invoke_failed);
				return;
			}

			if(!tmp_ret.empty())
			{
				e = error_maker_type::make_error(error_maker_type::E_handler_invoke_return_failed);
				return;
			}
		}

		template<typename Ret>
		void invoke(Ret& ret, error_type& e) const
		{
			typedef Ret ret_type;
			boost::any params;
			boost::any tmp_ret;
			bool bret = this->detail_invoke(tmp_ret, params);
			if(!bret)
			{
				e = error_maker_type::make_error(error_maker_type::E_handler_invoke_failed);
				return;
			}
			ret_type* pret = boost::any_cast<ret_type>(&tmp_ret);
			if(!pret)
			{
				e = error_maker_type::make_error(error_maker_type::E_handler_invoke_failed);
				return;
			}

			boost::swap(ret, *pret);
		}

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		template< typename Ret, YGGR_PP_FOO_TYPES_DEF( __n__ ) > \
		Ret invoke( YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_VAR_PARAMS ) ) const { \
			typedef Ret ret_type; \
			typedef packet::packet_info< YGGR_PP_TEMPLATE_PARAMS_TYPES(__n__, T) > packet_info_type; \
			packet_info_type pak_info(YGGR_PP_FOO_PARAMS_OP(__n__, YGGR_PP_SYMBOL_COMMA)); \
			boost::any params(pak_info); \
			boost::any tmp_ret; \
			bool bret = this->detail_invoke(tmp_ret, params); \
			if(!bret) { \
				throw error_maker_type::make_error(error_maker_type::E_handler_invoke_failed); } \
			ret_type* pret = boost::any_cast<ret_type>(&tmp_ret); \
			if(!pret) { \
				throw error_maker_type::make_error(error_maker_type::E_handler_invoke_return_failed); } \
			return *pret; } \
		\
		template< YGGR_PP_FOO_TYPES_DEF( __n__ ) > \
		void invoke(error_type& e, YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_VAR_PARAMS ) ) const { \
			typedef packet::packet_info< YGGR_PP_TEMPLATE_PARAMS_TYPES(__n__, T) > packet_info_type; \
			packet_info_type pak_info(YGGR_PP_FOO_PARAMS_OP(__n__, YGGR_PP_SYMBOL_COMMA)); \
			boost::any params(pak_info); \
			boost::any tmp_ret; \
			bool bret = this->detail_invoke(tmp_ret, params); \
			if(!bret) { \
				e = error_maker_type::make_error(error_maker_type::E_handler_invoke_failed); \
				return; } \
			if(!tmp_ret.empty()) { \
				e = error_maker_type::make_error(error_maker_type::E_handler_invoke_return_failed); \
				} } \
		\
		template< typename Ret, YGGR_PP_FOO_TYPES_DEF( __n__ ) > \
		void invoke(Ret& ret, error_type& e, YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_VAR_PARAMS ) ) const { \
			typedef Ret ret_type; \
			typedef packet::packet_info< YGGR_PP_TEMPLATE_PARAMS_TYPES(__n__, T) > packet_info_type; \
			packet_info_type pak_info(YGGR_PP_FOO_PARAMS_OP(__n__, YGGR_PP_SYMBOL_COMMA)); \
			boost::any params(pak_info); \
			boost::any tmp_ret; \
			bool bret = this->detail_invoke(tmp_ret, params); \
			if(!bret) { \
				e = error_maker_type::make_error(error_maker_type::E_handler_invoke_failed); \
				return; } \
			ret_type* pret = boost::any_cast<ret_type>(&tmp_ret); \
			if(!pret) { \
				e = error_maker_type::make_error(error_maker_type::E_handler_invoke_return_failed); \
				return; } \
			boost::swap(ret, *pret); }

#	define YGGR_PP_FOO_ARG_NAME(  ) arg_name
#	define BOOST_PP_LOCAL_LIMITS ( 1, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#	include BOOST_PP_LOCAL_ITERATE(  )
#	undef YGGR_PP_FOO_ARG_NAME
	protected:
		virtual bool detail_invoke(boost::any& ret, boost::any& params) const = 0;

	};

	typedef interface_ptr<basic_handler_container> i_handler_container_type;

#define YGGR_PP_TEMPLATE_DEFAULT_TYPE() boost::tuples::null_type
	template< typename Handler,
				YGGR_PP_TEMPLATE_PARAMS_TYPES_AND_DEFAULT_TYPE( YGGR_PP_TEMPLATE_DEFAULT_PARAMS_LEN, typename Val ) >
#undef YGGR_PP_TEMPLATE_DEFAULT_TYPE
	class handler_container
		: public basic_handler_container
	{
	public:
		typedef packet::packet_info< YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_PP_TEMPLATE_DEFAULT_PARAMS_LEN, Val) > params_type;
		typedef typename params_type::params_type check_data_type;

	private:
		enum
		{
			E_params_length = params_type::E_length,
			E_compile_u32 = 0xffffffff
		};

		typedef Handler handler_type;
		typedef typename handler_type::result_type result_type;

	private:
		// ret_type foo(...) version
		template<typename T, typename Nil_T = int>
		struct detail_invoke_helper
		{
			template<std::size_t N, typename _Nil = int>
			struct call_t
			{
				inline bool operator()(boost::any& ret, const handler_type& handler, boost::any& params) const
				{
					assert(false);
					return false;
				}
			};

			template<typename _Nil>
			struct call_t<0, _Nil>
			{
				bool operator()(boost::any& ret, const handler_type& handler, boost::any& params) const
				{
					boost::any tmp_ret(handler());
					ret.swap(tmp_ret);
					return true;
				}
			};

#		define BOOST_PP_LOCAL_MACRO( __n__ ) \
			template< typename _Nil > \
			struct call_t< __n__, _Nil > { \
				bool operator()(boost::any& ret, const handler_type& handler, boost::any& params) const { \
					params_type* pparams = boost::any_cast<params_type>(&params); \
						if(!pparams) return false; \
						params_type& YGGR_PP_PACKET_INFO_VALUE() = *pparams; \
					boost::any tmp_ret( \
						handler( \
							YGGR_PP_FOO_PARAMS_CUSTOM_OP_NOW( \
								__n__, YGGR_PP_TYPENAME_PACKET_INFO_TO_FOO_PARAMS_OP, YGGR_PP_SYMBOL_COMMA ) )); \
					ret.swap(tmp_ret); \
				return true; } };

#		define YGGR_PP_PACKET_INFO_VALUE() ref_params
#		define BOOST_PP_LOCAL_LIMITS ( 1, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#		include BOOST_PP_LOCAL_ITERATE(  )
#		undef YGGR_PP_PACKET_INFO_VALUE

			template<std::size_t N>
			bool call(boost::any& ret, const handler_type& handler, boost::any& params) const
			{
				call_t<N> caller;
				return caller(ret, handler, params);
			}
		};

		// void foo(...) version
		template<typename Nil_T>
		struct detail_invoke_helper<void, Nil_T>
		{
			template<std::size_t N, typename _Nil = int>
			struct call_t
			{
				inline bool operator()(boost::any& ret, const handler_type& handler, boost::any& params) const
				{
					assert(false);
					return false;
				}
			};

			template<typename _Nil>
			struct call_t<0, _Nil>
			{
				bool operator()(boost::any& ret, const handler_type& handler, boost::any& params) const
				{
					handler();
					boost::any tmp;
					ret.swap(tmp);
					return true;
				}
			};

#		define BOOST_PP_LOCAL_MACRO( __n__ ) \
			template< typename _Nil > \
			struct call_t< __n__, _Nil > { \
				bool operator()(boost::any& ret, const handler_type& handler, boost::any& params) const { \
						params_type* pparams = boost::any_cast<params_type>(&params); \
						if(!pparams) return false; \
						params_type& YGGR_PP_PACKET_INFO_VALUE() = *pparams; \
						handler( \
							YGGR_PP_FOO_PARAMS_CUSTOM_OP_NOW( \
								__n__, YGGR_PP_TYPENAME_PACKET_INFO_TO_FOO_PARAMS_OP, YGGR_PP_SYMBOL_COMMA ) ); \
						boost::any tmp; \
						ret.swap(tmp); \
						return true; } };

#		define YGGR_PP_PACKET_INFO_VALUE() ref_params
#		define BOOST_PP_LOCAL_LIMITS ( 1, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#		include BOOST_PP_LOCAL_ITERATE(  )
#		undef YGGR_PP_PACKET_INFO_VALUE

			template<std::size_t N>
			bool call(boost::any& ret, const handler_type& handler, boost::any& params) const
			{
				call_t<N> caller;
				return caller(ret, handler, params);
			}
		};

		typedef detail_invoke_helper<result_type> detail_invoke_helper_type;

	public:
		handler_container(const handler_type& handler)
			: _handler(handler)
		{
		}

		virtual ~handler_container(void)
		{
		}

	protected:
		virtual bool detail_invoke(boost::any& ret, boost::any& params) const
		{
			detail_invoke_helper_type helper;
			handler_type handler(_handler);
			return helper.template call<E_params_length>(ret, handler, params);
		}

	private:
		handler_type _handler;
	};

private:
	typedef Key key_type;
	typedef Mutex mutex_type;
	typedef safe_container::safe_unordered_map<key_type, i_handler_container_type, mutex_type> map_type;


private:
	typedef handler_center this_type;

public:
	handler_center(void)
	{
	}

	~handler_center(void)
	{
	}

	bool empty(void) const
	{
		return _map.empty();
	}

	std::size_t size(void) const
	{
		return _map.size();
	}

	void clear(void)
	{
		_map.clear();
	}

	template<typename Obj>
	bool register_handler_object(Obj& obj)
	{
		return obj.register_handler(*this);
	}

private:
	template<typename Sequence,
				std::size_t i = 0,
				std::size_t isize = boost::mpl::size<Sequence>::value >
	struct check_has_node_ref_impl
	{
		typedef Sequence sequence_type;
		typedef typename check_has_node_ref_impl<sequence_type, i + 1, isize>::type next_type;
		typedef typename boost::mpl::or_
							<
								typename boost::is_reference<typename boost::mpl::at_c<sequence_type, i>::type >::type,
								next_type
							>::type type;
	};

	template<typename Sequence, std::size_t i>
	struct check_has_node_ref_impl<Sequence, i, i>
	{
		typedef boost::mpl::false_ type;
	};

	template<typename Sequence>
	struct check_has_nude_ref
	{
		typedef Sequence sequence_type;
		typedef typename check_has_node_ref_impl<sequence_type>::type check_type;
		typedef typename boost::mpl::not_<check_type>::type type;
		BOOST_MPL_ASSERT_MSG(type::value, please_use_boost_reference_wrapper, (sequence_type));
	};

public:
	template<typename Handler>
	bool register_handler(const key_type& key, const Handler& handler)
	{
		typedef Handler handler_type;
		typedef handler_container<handler_type> handler_container_type;
		return _map.insert(key, i_handler_container_type(new handler_container_type(handler)));
	}

#define BOOST_PP_LOCAL_MACRO( __n__ ) \
	template< YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, typename T ), typename Handler> \
	bool register_handler(const key_type& key, const Handler& handler) { \
		typedef Handler handler_type; \
		typedef handler_container<handler_type, \
									YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, T ) > handler_container_type; \
		typedef typename check_has_nude_ref<typename handler_container_type::check_data_type>::type check_type; \
		return _map.insert( key, i_handler_container_type( new handler_container_type( handler ) ) ); }

#define BOOST_PP_LOCAL_LIMITS ( 1, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#include BOOST_PP_LOCAL_ITERATE(  )

	template<typename Obj>
	bool unregister_handler_object(Obj& obj)
	{
		return obj.unregister_handler(*this);
	}

	bool unregister_handler(const key_type& key)
	{
		return _map.erase(key);
	}

	bool call(const key_type& key) const
	{
		i_handler_container_type ptr;
		_map.get_value(key, ptr);
		if(!ptr)
		{
			return false;
		}

		error_type e(error_maker_type::make_error(0));
		ptr->invoke(e);
		return !e;
	}

	template<typename Ret>
	bool call(Ret& ret, const key_type& key) const
	{
		i_handler_container_type ptr;
		_map.get_value(key, ptr);
		if(!ptr)
		{
			return false;
		}

		error_type e(error_maker_type::make_error(0));
		ret = ptr->invoke(e, ret);

		return !e;
	}

#define BOOST_PP_LOCAL_MACRO( __n__ ) \
	template< YGGR_PP_FOO_TYPES_DEF( __n__ ) > \
	bool call(const key_type& key, YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_VAR_PARAMS) ) const { \
		i_handler_container_type ptr; \
		_map.get_value(key, ptr); \
		if(!ptr) { \
			return false; } \
		error_type e(error_maker_type::make_error(0)); \
		ptr->invoke(e, YGGR_PP_FOO_PARAMS_OP(__n__, YGGR_PP_SYMBOL_COMMA)); \
		return !e; } \
	\
	template<typename Ret, YGGR_PP_FOO_TYPES_DEF( __n__ ) > \
	bool call(Ret& ret, const key_type& key, YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_VAR_PARAMS)) const { \
		i_handler_container_type ptr; \
		_map.get_value(key, ptr); \
		if(!ptr) { \
			return false; } \
		error_type e(error_maker_type::make_error(0)); \
		ptr->invoke(ret, e, YGGR_PP_FOO_PARAMS_OP(__n__, YGGR_PP_SYMBOL_COMMA)); \
		return !e; }

#define YGGR_PP_FOO_ARG_NAME(  ) arg_name
#define BOOST_PP_LOCAL_LIMITS ( 1, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#include BOOST_PP_LOCAL_ITERATE(  )
#undef YGGR_PP_FOO_ARG_NAME

private:
	map_type _map;
};

} // namespace handler_center
} // namespace yggr

#include <boost/preprocessor/cat.hpp>
#include <yggr/ppex/cast.hpp>
#include <yggr/ppex/cast_to_string.hpp>


#define YGGR_PP_HANDLER_NAME( __foo__ ) \
	YGGR_PP_CAST( __foo__, YGGR_PP_CAST_TO_STRING )

#define YGGR_PP_HANDLER_NAME_T( __type__, __foo__ ) \
	__type__( YGGR_PP_CAST( __foo__, YGGR_PP_CAST_TO_STRING ) )

#define YGGR_PP_MEM_HANDLER_NAME(__class__, __foo__) \
	YGGR_PP_CAST(BOOST_PP_CAT(__class__, BOOST_PP_CAT(YGGR_PP_SYMBOL_AREA(), __foo__)), YGGR_PP_CAST_TO_STRING )

#define YGGR_PP_REF_T( __type__ ) \
	boost::reference_wrapper< __type__ >

#define YGGR_PP_CREF_T( __type__ ) \
	boost::reference_wrapper< __type__ const>


#endif // __YGGR_HANDLER_CENTER_HANDLER_CENTER_HPP__
