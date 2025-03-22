//erase_impl.hpp

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

#ifndef __YGGR_SAFE_CONTAINER_DETAIL_ERASE_IMPL_HPP__
#define __YGGR_SAFE_CONTAINER_DETAIL_ERASE_IMPL_HPP__

#include <yggr/base/yggrdef.h>

#include <yggr/func/foo_t_info.hpp>
#include <yggr/range_ex/range_iterator_ex.hpp>
#include <yggr/safe_container/detail/destroy_holder_selector.hpp>

#include <boost/utility/enable_if.hpp>
#include <algorithm>

namespace yggr
{
namespace safe_container
{
namespace detail
{

// return iterator version
template<typename Base>
struct erase_caller
{
public:
	typedef Base base_type;
	typedef typename base_type::iterator now_iterator_type;
	typedef now_iterator_type erase_result_type;
	typedef std::pair<erase_result_type, bool> result_type;

private:
	typedef erase_caller this_type;

public:

	template<typename Iter> inline
	result_type operator()(base_type& base, Iter i) const
	{
		return
			(i == base.end())?
				std::make_pair(i, false)
				: std::make_pair(container::erase(base, i), true);
	}

	template<typename Iter1, typename Iter2> inline
	result_type operator()(base_type& base, Iter1 first, Iter2 last) const
	{
		return
			(first == base.end())?
				std::make_pair(first, false)
				: std::make_pair(container::erase(base, first, last), true);
	}

	template<typename Iter1, typename Iter2> inline
	result_type operator()(base_type& base,
									const std::pair<Iter1, Iter2>& pos_pair) const
	{
		return this_type::operator()(base, pos_pair.first, pos_pair.second);
	}

	inline bool result_bool_adapter(const result_type& rst) const
	{
		return rst.second;
	}
};

} // namespace detail
} // namespace safe_container
} // namespace yggr

#define YGGR_PP_SAFE_CONTAINER_ERASE_ITER_FUNCTION_IMPL() \
	template<typename IterHandler> \
	bool erase(const IterHandler& iter_handler) { \
		typedef detail::erase_caller<base_type> caller_type; \
		caller_type caller; \
		write_lock_type lk(_mutex); \
		return caller.result_bool_adapter(caller(_base, iter_handler(_base))); } \
	\
	template<typename EHandler, typename ZHandler> \
	typename \
		boost::disable_if_c< \
			func::foo_t_info<ZHandler>::arg_holder_list_size == 2, \
			bool >::type \
		erase(const EHandler& eins_handler, const ZHandler& zwei_handler) { \
		typedef detail::erase_caller<base_type> caller_type; \
		caller_type caller; \
		write_lock_type lk(_mutex); \
		return caller.result_bool_adapter(caller(_base, eins_handler(_base), zwei_handler(_base))); } \
	\
	template<typename EHandler, typename ZHandler> \
	typename \
		boost::enable_if_c< \
			func::foo_t_info<ZHandler>::arg_holder_list_size == 2, \
			typename ::yggr::func::foo_t_info<ZHandler>::result_type >::type \
		erase(const EHandler& eins_handler, const ZHandler& zwei_handler) { \
		typedef detail::erase_caller<base_type> caller_type; \
		caller_type caller; \
		write_lock_type lk(_mutex); \
		return zwei_handler(_base, caller(_base, eins_handler(_base))); } \
	\
	template<typename EIterHandler, typename ZIterHandler, typename Handler> \
	typename ::yggr::func::foo_t_info<Handler>::result_type \
		erase(const EIterHandler& eins_iter_handler, const ZIterHandler& zwei_iter_handler, \
				const Handler& handler) { \
		typedef Handler handler_type; \
		typedef detail::erase_caller<base_type> caller_type; \
		caller_type caller; \
		write_lock_type lk(_mutex); \
		return handler(_base, caller(_base, eins_iter_handler(_base), zwei_iter_handler(_base))); }


#define YGGR_PP_SAFE_SEQ_ERASE_FUNCTION_IMPL() \
	YGGR_PP_SAFE_CONTAINER_ERASE_ITER_FUNCTION_IMPL()

#define YGGR_PP_SAFE_KV_ERASE_FUNCTION_IMPL() \
	YGGR_PP_SAFE_CONTAINER_ERASE_ITER_FUNCTION_IMPL()

#define YGGR_PP_SAFE_KV_ERASE_FUNCTION_SP_IMPL() \
	size_type erase(const key_type& key) { \
		write_lock_type lk(_mutex); \
		return _base.erase(key); } \
	\
	template<typename Handler> \
	typename ::yggr::func::foo_t_info<Handler>::result_type \
		erase(const key_type& key, const Handler& handler) { \
		write_lock_type lk(_mutex); \
		return handler(_base, _base.erase(key)); }


// ukv

#define YGGR_PP_SAFE_UKV_ERASE_FUNCTION_SP_IMPL() \
	YGGR_PP_SAFE_KV_ERASE_FUNCTION_SP_IMPL()

#define YGGR_PP_SAFE_UKV_ERASE_FUNCTION_IMPL() \
	YGGR_PP_SAFE_KV_ERASE_FUNCTION_IMPL()

// ----------------------opt version---------------------------------------
#ifndef YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION

namespace yggr
{
namespace safe_container
{
namespace detail
{

template<typename Base>
struct erase_opt_caller
{

public:
	typedef Base base_type;
	typedef typename base_type::iterator iterator;
	typedef iterator erase_result_type;
	typedef std::pair<erase_result_type, bool> result_type;

private:
	typedef erase_opt_caller this_type;

public:
	// reference version
	template<typename Container_Opt,
				typename Destroy_Holder, typename Iter> inline
	result_type operator()(base_type& base, Container_Opt& opt_base,
							Destroy_Holder& holder, Iter s) const
	{
		return
			(s == base.end())?
				std::make_pair(base.end(), false)
				: std::make_pair(opt_base.erase(holder, s), true);
	}

	template<typename Container_Opt,
				typename Destroy_Holder, typename Iter1, typename Iter2> inline
	result_type operator()(base_type& base, Container_Opt& opt_base,
							Destroy_Holder& holder, Iter1 s, Iter2 e) const
	{
		return
			(s == base.end())?
				std::make_pair(base.end(), false)
				: std::make_pair(opt_base.erase(holder, s, e), true);
	}

	template<typename Container_Opt,
				typename Destroy_Holder, typename Iter1, typename Iter2> inline
	result_type operator()(base_type& base, Container_Opt& opt_base,
							Destroy_Holder& holder,
							const std::pair<Iter1, Iter2>& iter_pair) const
	{
		return this_type::operator()(base, opt_base, holder, iter_pair.first, iter_pair.second);
	}

	// reference_wrapper_version
	template<template<typename _DH> class DHRef,
				typename Container_Opt,
				typename Destroy_Holder, typename Iter> inline
	result_type operator()(base_type& base, Container_Opt& opt_base,
							const DHRef<Destroy_Holder>& holder, Iter s) const
	{
		return
			(s == base.end())?
				std::make_pair(base.end(), false)
				: std::make_pair(opt_base.erase(holder, s), true);
	}

	template<template<typename _DH> class DHRef,
				typename Container_Opt,
				typename Destroy_Holder, typename Iter1, typename Iter2> inline
	result_type operator()(base_type& base, Container_Opt& opt_base,
							const DHRef<Destroy_Holder>& holder, Iter1 s, Iter2 e) const
	{
		return
			(s == base.end())?
				std::make_pair(base.end(), false)
				: std::make_pair(opt_base.erase(holder, s, e), true);
	}

	template<template<typename _DH> class DHRef,
				typename Container_Opt,
				typename Destroy_Holder, typename Iter1, typename Iter2> inline
	result_type operator()(base_type& base, Container_Opt& opt_base,
									const DHRef<Destroy_Holder>& holder,
									const std::pair<Iter1, Iter2>& iter_pair) const
	{
		return this_type::operator()(base, opt_base, holder, iter_pair.first, iter_pair.second);
	}

public:
	inline bool result_bool_adapter(const result_type& rst) const
	{
		return rst.second;
	}
};

} // namespace detail
} // namespace safe_container
} // namespace yggr

#	define YGGR_PP_SAFE_CONTAINER_OPT_A_ERASE_ITER_FUNCTION_IMPL() \
		template<typename IterHandler> \
		bool erase(const IterHandler& iter_handler) { \
			typedef detail::destroy_holder_selector< \
						destroy_holder_type, \
						multi_destroy_holder_type, \
						typename ::yggr::func::foo_t_info<IterHandler>::result_type > dh_sel_type; \
			typedef typename dh_sel_type::type dh_type; \
			dh_sel_type dh_sel; \
			dh_type holder(this_type::get_allocator()); \
			dh_sel(*this, holder); \
			detail::erase_opt_caller<base_type> caller; \
			write_lock_type lk(_mutex); \
			return caller.result_bool_adapter( \
					caller(_base, opt_cast(), \
							detail::allocator_destroy_holder_ref(holder), \
							iter_handler(_base))); } \
		\
		template<typename EHandler, typename ZHandler> \
		typename \
			boost::disable_if_c< \
				func::foo_t_info<ZHandler>::arg_holder_list_size == 2, \
				bool >::type \
			erase(const EHandler& eins_handler, const ZHandler& zwei_handler) { \
			typedef detail::destroy_holder_selector< \
						destroy_holder_type, \
						multi_destroy_holder_type, \
						std::pair< int, int >  > dh_sel_type; \
			typedef typename dh_sel_type::type dh_type; \
			dh_sel_type dh_sel; \
			dh_type holder(this_type::get_allocator()); \
			dh_sel(*this, holder); \
			detail::erase_opt_caller<base_type> caller; \
			write_lock_type lk(_mutex); \
			return caller.result_bool_adapter( \
					caller(_base, opt_cast(), \
							detail::allocator_destroy_holder_ref(holder), \
							eins_handler(_base), zwei_handler(_base))); } \
		\
		template<typename EHandler, typename ZHandler> \
		typename \
			boost::enable_if_c< \
				func::foo_t_info<ZHandler>::arg_holder_list_size == 2, \
				typename ::yggr::func::foo_t_info<ZHandler>::result_type >::type \
			erase(const EHandler& eins_handler, const ZHandler& zwei_handler) { \
			typedef detail::destroy_holder_selector< \
						destroy_holder_type, \
						multi_destroy_holder_type, \
						typename ::yggr::func::foo_t_info<EHandler>::result_type > dh_sel_type; \
			typedef typename dh_sel_type::type dh_type; \
			dh_sel_type dh_sel; \
			dh_type holder(this_type::get_allocator()); \
			dh_sel(*this, holder); \
			detail::erase_opt_caller<base_type> caller; \
			write_lock_type lk(_mutex); \
			return zwei_handler(_base, \
					caller(_base, opt_cast(), \
							detail::allocator_destroy_holder_ref(holder), \
							eins_handler(_base))); } \
		\
		template<typename EIterHandler, typename ZIterHandler, typename Handler> \
		typename ::yggr::func::foo_t_info<Handler>::result_type \
			erase(const EIterHandler& eins_iter_handler, const ZIterHandler& zwei_iter_handler, \
					const Handler& handler) { \
			typedef detail::destroy_holder_selector < \
						destroy_holder_type, \
						multi_destroy_holder_type, \
						std::pair<int, int> > dh_sel_type; \
			typedef typename dh_sel_type::type dh_type; \
			dh_sel_type dh_sel; \
			dh_type holder(this_type::get_allocator()); \
			dh_sel(*this, holder); \
			detail::erase_opt_caller<base_type> caller; \
			write_lock_type lk(_mutex); \
			return handler(_base, \
							caller(_base, opt_cast(), \
									detail::allocator_destroy_holder_ref(holder), \
									eins_iter_handler(_base), \
									zwei_iter_handler(_base))); }
//seq
#define YGGR_PP_SAFE_SEQ_OPT_ERASE_FUNCTION_IMPL() \
	YGGR_PP_SAFE_CONTAINER_OPT_A_ERASE_ITER_FUNCTION_IMPL()

//kv
#define YGGR_PP_SAFE_KV_OPT_ERASE_FUNCTION_IMPL() \
	YGGR_PP_SAFE_CONTAINER_OPT_A_ERASE_ITER_FUNCTION_IMPL()

#define YGGR_PP_SAFE_KV_OPT_UNIQUE_ERASE_FUNCTION_IMPL() \
	size_type erase(const key_type& key) { \
		destroy_holder_type holder(this_type::get_allocator()); \
		write_lock_type lk(_mutex); \
		return opt_cast().erase(detail::allocator_destroy_holder_ref(holder), key); } \
	\
	template<typename Handler> \
	typename ::yggr::func::foo_t_info<Handler>::result_type \
		erase(const key_type& key, const Handler& handler) { \
		destroy_holder_type holder(this_type::get_allocator()); \
		write_lock_type lk(_mutex); \
		return handler(_base, opt_cast().erase(detail::allocator_destroy_holder_ref(holder), key)); } \
	\
	YGGR_PP_SAFE_KV_OPT_ERASE_FUNCTION_IMPL()

#define YGGR_PP_SAFE_KV_OPT_NON_UNIQUE_ERASE_FUNCTION_IMPL() \
	size_type erase(const key_type& key) { \
		multi_destroy_holder_type holder(this_type::get_allocator()); \
		write_lock_type lk(_mutex); \
		return opt_cast().erase(detail::allocator_destroy_holder_ref(holder), key); } \
	\
	template<typename Handler> \
	typename ::yggr::func::foo_t_info<Handler>::result_type \
		erase(const key_type& key, const Handler& handler) { \
		multi_destroy_holder_type holder(this_type::get_allocator()); \
		write_lock_type lk(_mutex); \
		return handler(_base, opt_cast().erase(detail::allocator_destroy_holder_ref(holder), key)); } \
	\
	YGGR_PP_SAFE_KV_OPT_ERASE_FUNCTION_IMPL()

// ukv_h

#define YGGR_PP_SAFE_UKV_HA_OPT_UNIQUE_ERASE_FUNCTION_SP_IMPL() \
	size_type erase(const key_type& key) { \
		std::size_t key_hash = opt_type::s_hash(this_type::hash_function(), key); \
		destroy_holder_type holder(this_type::get_allocator()); \
		write_lock_type lk(_mutex); \
		return \
			opt_cast().erase( \
				detail::allocator_destroy_holder_ref(holder), \
				key_hash, key); } \
	\
	template<typename Handler> \
	typename ::yggr::func::foo_t_info<Handler>::result_type \
		erase(const key_type& key, const Handler& handler) { \
		std::size_t key_hash = opt_type::s_hash(this_type::hash_function(), key); \
		destroy_holder_type holder(this_type::get_allocator()); \
		write_lock_type lk(_mutex); \
		return \
			handler( \
				_base, \
				opt_cast().erase( \
					detail::allocator_destroy_holder_ref(holder), \
					key_hash, key) ); }

#define YGGR_PP_SAFE_UKV_HA_OPT_UNIQUE_ERASE_FUNCTION_IMPL() \
	YGGR_PP_SAFE_UKV_HA_OPT_UNIQUE_ERASE_FUNCTION_SP_IMPL() \
	\
	YGGR_PP_SAFE_KV_OPT_ERASE_FUNCTION_IMPL()

#define YGGR_PP_SAFE_UKV_HA_OPT_NON_UNIQUE_ERASE_FUNCTION_SP_IMPL() \
	size_type erase(const key_type& key) { \
		std::size_t key_hash = opt_type::s_hash(this_type::hash_function(), key); \
		multi_destroy_holder_type holder(this_type::get_allocator()); \
		write_lock_type lk(_mutex); \
		return \
			opt_cast().erase( \
				detail::allocator_destroy_holder_ref(holder), \
				key_hash, key); } \
	\
	template<typename Handler> \
	typename ::yggr::func::foo_t_info<Handler>::result_type \
		erase(const key_type& key, const Handler& handler) { \
		std::size_t key_hash = opt_type::s_hash(this_type::hash_function(), key); \
		multi_destroy_holder_type holder(this_type::get_allocator()); \
		write_lock_type lk(_mutex); \
		return \
			handler( \
				_base, \
				opt_cast().erase( \
				detail::allocator_destroy_holder_ref(holder), \
				key_hash, key) ); }

#define YGGR_PP_SAFE_UKV_HA_OPT_NON_UNIQUE_ERASE_FUNCTION_IMPL() \
	YGGR_PP_SAFE_UKV_HA_OPT_NON_UNIQUE_ERASE_FUNCTION_SP_IMPL() \
	\
	YGGR_PP_SAFE_KV_OPT_ERASE_FUNCTION_IMPL()


#endif // YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION

namespace yggr
{
namespace safe_container
{
namespace detail
{

template<typename UnSafeCont, typename Val>
struct erase_foo_erase_value_20_rnd_container
{
	typedef UnSafeCont unsafe_cont_type;
	typedef typename unsafe_cont_type::size_type now_size_type;
	typedef typename range_ex::range_iterator_ex<unsafe_cont_type>::type now_iter_type;
	typedef Val rm_val_type;

private:
    typedef erase_foo_erase_value_20_rnd_container this_type;

public:
    erase_foo_erase_value_20_rnd_container(const rm_val_type& val)
        : rm_val_(val)
    {
    }

    erase_foo_erase_value_20_rnd_container(const this_type& right)
        : rm_val_(right.rm_val_)
    {
    }

    ~erase_foo_erase_value_20_rnd_container(void)
    {
    }

public:
    inline now_size_type operator()(unsafe_cont_type& uc) const
    {
        now_iter_type it = std::remove(boost::begin(uc), boost::end(uc), rm_val_);
        now_size_type ret = std::distance(it, boost::end(uc));
        uc.erase(it, boost::end(uc));
        return ret;
    }

public:
    const rm_val_type& rm_val_;
};

template<typename SafeCont, typename Val> inline
typename SafeCont::size_type erase_value_20_rnd_container(SafeCont& c, const Val& val)
{
	typedef SafeCont safe_cont_type;
	typedef typename safe_cont_type::base_type unsafe_cont_type;
	typedef Val rm_val_type;
    typedef erase_foo_erase_value_20_rnd_container<unsafe_cont_type, rm_val_type> erase_foo_type;

	erase_foo_type foo(val);
	return c.using_handler(foo);

};

template<typename UnSafeCont, typename Pred>
struct erase_foo_erase_if_20_rnd_container
{
	typedef UnSafeCont unsafe_cont_type;
	typedef typename unsafe_cont_type::size_type now_size_type;
	typedef typename range_ex::range_iterator_ex<unsafe_cont_type>::type now_iter_type;
	typedef Pred pred_type;

private:
    typedef erase_foo_erase_if_20_rnd_container this_type;

public:
    erase_foo_erase_if_20_rnd_container(pred_type pred)
        : pred_(pred)
    {
    }

    erase_foo_erase_if_20_rnd_container(const this_type& right)
        : pred_(right.pred_)
    {
    }

    ~erase_foo_erase_if_20_rnd_container(void)
    {
    }

public:
    inline now_size_type operator()(unsafe_cont_type& uc) const
    {
        now_iter_type it = std::remove_if(boost::begin(uc), boost::end(uc), pred_);
        now_size_type ret = std::distance(it, boost::end(uc));
        uc.erase(it, boost::end(uc));
        return ret;
    }

public:
    pred_type pred_;
};

template<typename SafeCont, typename Pred> inline
typename SafeCont::size_type erase_if_20_rnd_container(SafeCont& c, Pred pred)
{
	typedef SafeCont safe_cont_type;
	typedef typename safe_cont_type::base_type unsafe_cont_type;
	typedef Pred pred_type;
    typedef erase_foo_erase_if_20_rnd_container<unsafe_cont_type, pred_type> erase_foo_type;

	erase_foo_type foo(pred);
	return c.using_handler(foo);

};

} // namespace detail
} // namespace safe_container
} // namespace yggr


#define YGGR_PP_SAFE_KV_ERASE_IF_MEMBER_FUNCTION_IMPL() \
	template<typename Pred> \
	size_type erase_if(Pred pred) { \
		write_lock_type lk(_mutex); \
		return container::erase_if(_base, pred); }

#define YGGR_PP_SAFE_KV_ERASE_IF_OPT_MEMBER_FUNCTION_IMPL() \
	template<typename Pred> \
	size_type erase_if(Pred pred) { \
		multi_destroy_holder_type holder(this_type::get_allocator()); \
		{ write_lock_type lk(_mutex); \
			for(iterator i = _base.begin(), isize = _base.end(); i != isize;) { \
				if(pred(*i)) { \
					i = opt_cast().erase(detail::allocator_destroy_holder_ref(holder), i); } \
				else { ++i; } } } \
		return holder.size(); }

#define YGGR_PP_SAFE_UKV_ERASE_IF_MEMBER_FUNCTION_IMPL YGGR_PP_SAFE_KV_ERASE_IF_MEMBER_FUNCTION_IMPL
#define YGGR_PP_SAFE_UKV_ERASE_IF_OPT_MEMBER_FUNCTION_IMPL YGGR_PP_SAFE_KV_ERASE_IF_OPT_MEMBER_FUNCTION_IMPL


#endif //__YGGR_SAFE_CONTAINER_DETAIL_ERASE_IMPL_HPP__
