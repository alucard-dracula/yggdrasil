//safe_options_vector.hpp

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

#ifndef __YGGR_SAFE_CONTAINER_SAFE_OPTIONS_VECTOR_HPP__
#define __YGGR_SAFE_CONTAINER_SAFE_OPTIONS_VECTOR_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/bind/bind.hpp>

#include <yggr/thread/boost_thread.hpp>
#include <yggr/thread/mutex_def_helper.hpp>

#include <yggr/move/move.hpp>

#include <yggr/ppex/friend.hpp>
#include <yggr/func/foo_t_info.hpp>

#include <yggr/mplex/tag_sfinae.hpp>

#include <yggr/container/options_vector.hpp>
#include <yggr/safe_container/detail/swap_def.hpp>
#include <yggr/safe_container/detail/comparer_def.hpp>

#include <yggr/safe_container/detail/safe_element_reference.hpp>
#include <yggr/safe_container/detail/front_back_at_impl.hpp>
#include <yggr/safe_container/detail/begin_rbegin_value_impl.hpp>
#include <yggr/safe_container/detail/assign_impl.hpp>
#include <yggr/safe_container/detail/operator_set_impl.hpp>
#include <yggr/safe_container/detail/swap_impl.hpp>
#include <yggr/safe_container/detail/push_pop_impl.hpp>
#include <yggr/safe_container/detail/safe_other_impl.hpp>
#include <yggr/safe_container/detail/erase_impl.hpp>
#include <yggr/safe_container/detail/insert_impl.hpp>
#include <yggr/safe_container/detail/emplace_impl.hpp>
#include <yggr/safe_container/detail/using_handler_impl.hpp>
#include <yggr/safe_container/detail/for_each_impl.hpp>
#include <yggr/safe_container/detail/range_size_def.hpp>
#include <yggr/safe_container/safe_container_error.hpp>

#include <boost/thread/mutex.hpp>
#include <boost/range/functions.hpp>
#include <boost/function.hpp>
#include <boost/ref.hpp>
#include <boost/utility/enable_if.hpp>

#include <iterator>
#include <algorithm>
#include <utility>

/* 
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!!! All safe containers can not guarantee the creation and destruction of multi-threaded safe,		!!!
!!! in different threads to create and destroy the safe container, please use swap to deal with,	!!!
!!! and																								!!! 
!!!  it is forbidden to pass any "iterator reference pointer" inside the safe_container				!!!
!!!	 through the callback function to the outside of the safe_container and save or use it!			!!!
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
*/

namespace yggr
{
namespace safe_container
{

template <typename Val,
			typename Mutex = void,
			typename Ax = typename mplex::typename_expand_get<container::options_vector<Val>, 1>::type,
			typename Options = typename mplex::typename_expand_get<container::options_vector<Val>, 2>::type,
			template<typename _Ty, typename _Ax, typename _O> class Vector = container::options_vector
		>
class safe_options_vector
{
public:
	typedef Ax tpl_arg_allocator_type;
	typedef Options tpl_arg_options_type;

public:
	typedef Vector<Val, Ax, Options> base_type;

public:
	typedef typename base_type::options_type options_type;
	typedef typename base_type::allocator_type allocator_type;
	typedef typename base_type::value_type value_type;
	typedef typename base_type::size_type size_type;
	typedef typename base_type::difference_type difference;

	typedef typename base_type::reference reference;
	typedef typename base_type::const_reference const_reference;

	typedef typename base_type::pointer pointer;
	typedef typename base_type::const_pointer const_pointer;

	typedef typename base_type::iterator iterator;
	typedef typename base_type::const_iterator const_iterator;

	typedef typename base_type::reverse_iterator reverse_iterator;
	typedef typename base_type::const_reverse_iterator const_reverse_iterator;

	typedef safe_container_error::error_type error_type;

private:
	typedef safe_container_error::error_maker_type error_maker_type;

private:
	typedef thread::mutex_def_helper<Mutex> mutex_def_helper_type;
	typedef typename mutex_def_helper_type::mutex_type mutex_type;
	typedef typename mutex_def_helper_type::read_lock_type read_lock_type;
	typedef typename mutex_def_helper_type::write_lock_type write_lock_type;

private:
	typedef safe_options_vector this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	safe_options_vector(void)
	{
	}

	explicit safe_options_vector(const allocator_type& alloc)
		: _base(alloc)
	{
	}

	safe_options_vector(size_type count)
		: _base(count, value_type())
	{
	}

	safe_options_vector(size_type count, const value_type& val)
		: _base(count, val)
	{
	}

	safe_options_vector(size_type count, const value_type& val, const allocator_type& alloc)
		: _base(count, val, alloc)
	{
	}

	template<typename InputIterator>
	safe_options_vector(InputIterator first, InputIterator last)
		: _base(first, last)
	{
	}

	template<typename InputIterator>
	safe_options_vector(InputIterator first, InputIterator last, const allocator_type& alloc)
		: _base(first, last, alloc)
	{
	}

#ifndef YGGR_NO_CXX11_HDR_INITIALIZER_LIST

    safe_options_vector(std::initializer_list<value_type> l)
		: _base(l.begin(), l.end())
    {
    }

	safe_options_vector(std::initializer_list<value_type> l, const allocator_type& alloc)
		: _base(l.begin(), l.end(), alloc)
    {
    }

#endif // YGGR_NO_CXX11_HDR_INITIALIZER_LIST

	safe_options_vector(BOOST_RV_REF(base_type) right)
		: _base(right.get_allocator())
	{
	    base_type& right_ref = right;
	    _base.swap(right_ref);
	}

	safe_options_vector(const base_type& right)
		: _base(right)
	{
	}

	safe_options_vector(BOOST_RV_REF(base_type) right, const allocator_type& alloc)
		: _base(alloc)
	{
	    base_type& right_ref = right;
	    _base.swap(right_ref);
	}

	safe_options_vector(const base_type& right, const allocator_type& alloc)
		: _base(right.begin(), right.end(), alloc)
	{
	}

	safe_options_vector(BOOST_RV_REF(this_type) right)
		: _base(right.get_allocator())
	{
		this_type& right_ref = right;
		right_ref.swap(_base);
	}

	safe_options_vector(const this_type& right)
		: _base(right.get_allocator())
	{
		right.copy_to_base(_base);
	}

	safe_options_vector(BOOST_RV_REF(this_type) right, const allocator_type& alloc)
		: _base(alloc)
	{
		this_type& right_ref = right;
		right_ref.swap(_base);
	}

	safe_options_vector(const this_type& right, const allocator_type& alloc)
		: _base(alloc)
	{
		right.copy_to_base(_base);
	}

	// isomeric_same
	template<typename Ax2, typename O2>
	safe_options_vector(BOOST_RV_REF_BEG 
							Vector<value_type, Ax2, O2> 
						BOOST_RV_REF_END right,
						typename 
							boost::enable_if
							<
								container::is_isomeric_same_options_vector
								<
									Vector<value_type, Ax2, O2>,
									base_type
								>,
								mplex::sfinae_type
							>::type sfinae = 0)
	{
		typedef Vector<value_type, Ax2, O2> right_type;
		right_type& right_ref = right;
	    _base.swap(reinterpret_cast<base_type&>(right_ref));
	}

	template<typename Ax2, typename O2>
	safe_options_vector(const Vector<value_type, Ax2, O2> & right,
						typename 
							boost::enable_if
							<
								container::is_isomeric_same_options_vector
								<
									Vector<value_type, Ax2, O2>,
									base_type
								>,
								mplex::sfinae_type
							>::type sfinae = 0)
		: _base(reinterpret_cast<const base_type&>(right))
	{
	}

	template<typename Ax2, typename O2>
	safe_options_vector(BOOST_RV_REF_BEG 
							Vector<value_type, Ax2, O2> 
						BOOST_RV_REF_END right, 
						const allocator_type& alloc,
						typename 
							boost::enable_if
							<
								container::is_isomeric_same_options_vector
								<
									Vector<value_type, Ax2, O2>,
									base_type
								>,
								mplex::sfinae_type
							>::type sfinae = 0)
		: _base(alloc)
	{
		typedef Vector<value_type, Ax2, O2> right_type;
	    right_type& right_ref = right;
	    _base.swap(reinterpret_cast<base_type&>(right_ref));
	}

	template<typename Ax2, typename O2>
	safe_options_vector(const Vector<value_type, Ax2, O2>& right,
						const allocator_type& alloc,
						typename 
							boost::enable_if
							<
								container::is_isomeric_same_options_vector
								<
									Vector<value_type, Ax2, O2>,
									base_type
								>,
								mplex::sfinae_type
							>::type sfinae = 0)
		: _base(right.begin(), right.end(), alloc)
	{
	}

	template<typename Mtx2, typename Ax2, typename O2>
	safe_options_vector(BOOST_RV_REF_BEG
							safe_options_vector<value_type, Mtx2, Ax2, O2, Vector> 
						BOOST_RV_REF_END right,
						typename 
							boost::enable_if
							<
								boost::is_same
								<
									typename safe_options_vector<value_type, Mtx2, Ax2, O2, Vector>::base_type,
									base_type
								>,
								mplex::sfinae_type
							>::type sfinae = 0)
		: _base(right.get_allocator())
	{
		typedef safe_options_vector<value_type, Mtx2, Ax2, O2, Vector> right_type;

		right_type& right_ref = right;
		right_ref.swap(_base);
	}

	template<typename Mtx2, typename Ax2, typename O2>
	safe_options_vector(const safe_options_vector<value_type, Mtx2, Ax2, O2, Vector>& right,
						typename 
							boost::enable_if
							<
								boost::is_same
								<
									typename safe_options_vector<value_type, Mtx2, Ax2, O2, Vector>::base_type,
									base_type
								>,
								mplex::sfinae_type
							>::type sfinae = 0)
		: _base(right.get_allocator())
	{
		right.copy_to_base(_base);
	}

	template<typename Mtx2, typename Ax2, typename O2>
	safe_options_vector(BOOST_RV_REF_BEG 
							safe_options_vector<value_type, Mtx2, Ax2, O2, Vector>
						BOOST_RV_REF_END right, 
						const allocator_type& alloc,
						typename 
							boost::enable_if
							<
								boost::is_same
								<
									typename safe_options_vector<value_type, Mtx2, Ax2, O2, Vector>::base_type,
									base_type
								>,
								mplex::sfinae_type
							>::type sfinae = 0)
		: _base(alloc)
	{
		typedef safe_options_vector<value_type, Mtx2, Ax2, O2, Vector> right_type;

		right_type& right_ref = right;
		right_ref.swap(_base);
	}

	template<typename Mtx2, typename Ax2, typename O2>
	safe_options_vector(const safe_options_vector<value_type, Mtx2, Ax2, O2, Vector>& right, 
						const allocator_type& alloc,
						typename 
							boost::enable_if
							<
								boost::is_same
								<
									typename safe_options_vector<value_type, Mtx2, Ax2, O2, Vector>::base_type,
									base_type
								>,
								mplex::sfinae_type
							>::type sfinae = 0)
		: _base(alloc)
	{
		right.copy_to_base(_base);
	}

	template<typename Mtx2, typename Ax2, typename O2>
	safe_options_vector(BOOST_RV_REF_BEG
							safe_options_vector<value_type, Mtx2, Ax2, O2, Vector> 
						BOOST_RV_REF_END right,
						typename 
							boost::enable_if
							<
								container::is_isomeric_same_options_vector
								<
									typename safe_options_vector<value_type, Mtx2, Ax2, O2, Vector>::base_type,
									base_type
								>,
								mplex::sfinae_type
							>::type sfinae = 0)
	{
		typedef safe_options_vector<value_type, Mtx2, Ax2, O2, Vector> right_type;
		typedef typename right_type::base_type right_base_type;

		right_type& right_ref = right;
		right_ref.swap(reinterpret_cast<right_base_type&>(_base));
	}

	template<typename Mtx2, typename Ax2, typename O2>
	safe_options_vector(const safe_options_vector<value_type, Mtx2, Ax2, O2, Vector>& right,
						typename 
							boost::enable_if
							<
								container::is_isomeric_same_options_vector
								<
									typename safe_options_vector<value_type, Mtx2, Ax2, O2, Vector>::base_type,
									base_type
								>,
								mplex::sfinae_type
							>::type sfinae = 0)
	{
		typedef safe_options_vector<value_type, Mtx2, Ax2, O2, Vector> right_type;
		typedef typename right_type::base_type right_base_type;

		right.copy_to_base(reinterpret_cast<right_base_type&>(_base));
	}

	template<typename Mtx2, typename Ax2, typename O2>
	safe_options_vector(BOOST_RV_REF_BEG 
							safe_options_vector<value_type, Mtx2, Ax2, O2, Vector>
						BOOST_RV_REF_END right, 
						const allocator_type& alloc,
						typename 
							boost::enable_if
							<
								container::is_isomeric_same_options_vector
								<
									typename safe_options_vector<value_type, Mtx2, Ax2, O2, Vector>::base_type,
									base_type
								>,
								mplex::sfinae_type
							>::type sfinae = 0)
		: _base(alloc)
	{
		typedef safe_options_vector<value_type, Mtx2, Ax2, O2, Vector> right_type;
		typedef typename right_type::base_type right_base_type;

		right_type& right_ref = right;
		right_ref.swap(reinterpret_cast<right_base_type&>(_base));
	}

	template<typename Mtx2, typename Ax2, typename O2>
	safe_options_vector(const safe_options_vector<value_type, Mtx2, Ax2, O2, Vector>& right, 
						const allocator_type& alloc,
						typename 
							boost::enable_if
							<
								container::is_isomeric_same_options_vector
								<
									typename safe_options_vector<value_type, Mtx2, Ax2, O2, Vector>::base_type,
									base_type
								>,
								mplex::sfinae_type
							>::type sfinae = 0)
		: _base(alloc)
	{
		typedef safe_options_vector<value_type, Mtx2, Ax2, O2, Vector> right_type;
		typedef typename right_type::base_type right_base_type;

		right.copy_to_base(reinterpret_cast<right_base_type&>(_base));
	}

	~safe_options_vector(void)
	{
	}

public:
	//inline this_type& operator=(BOOST_RV_REF(base_type) right);
	//this_type& operator=(const base_type& right);

	//inline this_type& operator=(BOOST_RV_REF(this_type) right);
	//this_type& operator=(const this_type& right);

//#ifndef YGGR_NO_CXX11_HDR_INITIALIZER_LIST
	//this_type& operator=(std::initializer_list<value_type> il);
//#endif // YGGR_NO_CXX11_HDR_INITIALIZER_LIST

	YGGR_PP_SAFE_SEQ_OPERATOR_SET_IMPL()

	//inline this_type& operator=(BOOST_RV_REF(isomeric_same_base_type) right);
	//this_type& operator=(const isomeric_same_base_type& right);

	//inline this_type& operator=(BOOST_RV_REF(isomeric_same_this_type) right);
	//this_type& operator=(const isomeric_same_this_type& right);

	YGGR_PP_SAFE_CONTAINER_ISOMERIC_SAME_OPERATOR_SET_IMPL(
		4, Vector, safe_options_vector, container::is_isomeric_same_options_vector)

public:
	//inline void swap(BOOST_RV_REF(base_type) right);
	//void swap(base_type& right);

	//inline void swap(BOOST_RV_REF(this_type) right);
	//void swap(this_type& right);

	YGGR_PP_SAFE_CONTAINER_SWAP_IMPL()

	//inline void swap(BOOST_RV_REF(isomeric_same_base_type) right);
	//void swap(isomeric_same_base_type& right);

	//inline void swap(BOOST_RV_REF(isomeric_same_this_type) right);
	//void swap(isomeric_same_this_type& right);

	YGGR_PP_SAFE_CONTAINER_ISOMERIC_SAME_SWAP_IMPL(
		4, Vector, safe_options_vector, container::is_isomeric_same_options_vector)

public:
	//operator base_type(void) const;

	YGGR_PP_SAFE_CONTAINER_OPERATOR_BASE_IMPL()

	//operator isomeric_same_base_type(void) const;

	YGGR_PP_SAFE_CONTAINER_ISOMERIC_SAME_OPERATOR_BASE_IMPL(
		3, Vector, container::is_isomeric_same_options_vector)

public:
	// capacity:
	size_type size(void) const
	{
		read_lock_type lk(_mutex);
		return _base.size();
	}

	size_type max_size(void) const
	{
		read_lock_type lk(_mutex);
		return _base.max_size();
	}

	void resize(size_type size)
	{
		write_lock_type lk(_mutex);
		_base.resize(size);
	}

	void resize(size_type size, const value_type& val)
	{
		write_lock_type lk(_mutex);
		_base.resize(size, val);
	}

	size_type capacity(void) const
	{
		read_lock_type lk(_mutex);
		return _base.capacity();
	}

	bool empty(void) const
	{
		read_lock_type lk(_mutex);
		return _base.empty();
	}

	bool reserve(size_type size)
	{
		write_lock_type lk(_mutex);
		try
		{
			_base.reserve(size);
			return true;
		}
		catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR())
		{
			__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what()))
			return false;
		}
	}

	void shrink_to_fit(void)
	{
		write_lock_type lk(_mutex);
		::yggr::container::shrink_to_fit(_base);
	}

	//element access:
private:
	
	inline static reference unlock_at(base_type& base, size_type idx)
	{
		if(!(idx < base.size()))
		{
			throw error_maker_type::make_error(error_maker_type::E_element_not_exists);
		}

		return base.at(idx);
	}

	value_type unsafe_get_at(size_type idx) const
	{
		read_lock_type lk(_mutex);
		if(!(idx < _base.size()))
		{
			throw error_maker_type::make_error(error_maker_type::E_element_not_exists);
		}

		return _base.at(idx);
	}

	template<typename Handler>
	typename func::foo_t_info<Handler>::result_type
		unsafe_set_at(size_type idx, const Handler& handler)
	{
		write_lock_type lk(_mutex);
		if(!(idx < _base.size()))
		{
			throw error_maker_type::make_error(error_maker_type::E_element_not_exists);
		}

		return handler(_base.at(idx));
	}

	// front()
	inline static reference unlock_front(base_type& base)
	{
		if(base.empty())
		{
			throw error_maker_type::make_error(error_maker_type::E_element_not_exists);
		}

		return base.front();
	}

	value_type unsafe_get_front(void) const
	{
		read_lock_type lk(_mutex);
		if(_base.empty())
		{
			throw error_maker_type::make_error(error_maker_type::E_element_not_exists);
		}

		return _base.front();
	}

	template<typename Handler>
	typename func::foo_t_info<Handler>::result_type
		unsafe_set_front(const Handler& handler)
	{
		write_lock_type lk(_mutex);
		if(_base.empty())
		{
			throw error_maker_type::make_error(error_maker_type::E_element_not_exists);
		}

		return handler(_base.front());
	}

	// back()

	inline static reference unlock_back(base_type& base)
	{
		if(base.empty())
		{
			throw error_maker_type::make_error(error_maker_type::E_element_not_exists);
		}

		return base.back();
	}

	value_type unsafe_get_back(void) const
	{
		read_lock_type lk(_mutex);
		if(_base.empty())
		{
			throw error_maker_type::make_error(error_maker_type::E_element_not_exists);
		}

		return _base.back();
	}

	template<typename Handler>
	typename func::foo_t_info<Handler>::result_type
		unsafe_set_back(const Handler& handler)
	{
		write_lock_type lk(_mutex);
		if(_base.empty())
		{
			throw error_maker_type::make_error(error_maker_type::E_element_not_exists);
		}

		return handler(_base.back());
	}

	class bridge_at;
	class bridge_front;
	class bridge_back;

	friend class bridge_at;
	friend class bridge_front;
	friend class bridge_back;

	class bridge_at
		: public detail::basic_bridge<safe_options_vector>
	{
	private:
		typedef detail::basic_bridge<safe_options_vector> bridge_base_type;

	public:
		typedef typename bridge_base_type::container_type container_type;
		typedef typename bridge_base_type::base_container_type base_container_type;
		typedef typename bridge_base_type::value_type value_type;

	private:
		typedef bridge_at this_type;

	public:
		bridge_at(container_type& cont, size_type idx)
			: bridge_base_type(cont), 
				_idx(idx)
		{
		}

		bridge_at(const bridge_at& right)
			: bridge_base_type(right),
				_idx(right._idx)
		{
		}

		~bridge_at(void)
		{
		}

	public:
		inline size_type index(void) const
		{
			return _idx;
		}

		inline bool operator==(const bridge_at& right) const
		{
			return 
				(this == boost::addressof(right))
				|| (bridge_base_type::compare_eq(right)
						&& _idx == right._idx);
		}

		inline bool operator==(const bridge_front& right) const
		{
			return bridge_base_type::compare_eq(right)
					&& _idx == 0;
		}

		inline bool operator==(const bridge_back& right) const
		{
			const container_type& cont = bridge_base_type::_wrap_cont;
			return bridge_base_type::compare_eq(right)
					&& _idx == cont.size() - 1;
		}

		template<typename OtherBridge> inline
		bool operator==(const OtherBridge&) const
		{
			return false;
		}

		template<typename OtherBridge> inline
		bool operator!=(const OtherBridge& right) const
		{
			return !this_type::operator==(right);
		}

	public:
		inline value_type operator()(void) const
		{
			const container_type& cont = bridge_base_type::_wrap_cont;
			return cont.unsafe_get_at(_idx);
		}

		template<typename Handler> inline
		typename func::foo_t_info<Handler>::result_type
			operator()(const Handler& handler) const
		{
			container_type& cont = bridge_base_type::_wrap_cont;
			return cont.unsafe_set_at(_idx, handler);
		}

	public:
		inline boost::function1<value_type&, base_container_type&>
			native_handler(void) const
		{
			return boost::bind(&container_type::unlock_at, _1, _idx);
		}

		template<typename Op_Handler, typename OtherBridge> inline
		boost::function1
		<
			typename func::foo_t_info<Op_Handler>::result_type, 
			base_container_type&
		>
			link(const Op_Handler& op_handler, const OtherBridge& other_bridge) const
		{
			return
				bridge_base_type::pro_s_link_handler(
					op_handler, 
					this_type::native_handler(), 
					other_bridge.native_handler());
		}

		template<typename Op_Handler, typename Handler> inline
		boost::function1
		<
			typename func::foo_t_info<Op_Handler>::result_type, 
			base_container_type&
		>
			link_handler(const Op_Handler& op_handler, const Handler& handler) const
		{
			return bridge_base_type::pro_s_link_handler(op_handler, this_type::native_handler(), handler);
		}

	private:
		const size_type _idx;
	};

	class bridge_front
		: public detail::basic_bridge<safe_options_vector>
	{
	private:
		typedef detail::basic_bridge<safe_options_vector> bridge_base_type;

	public:
		typedef typename bridge_base_type::container_type container_type;
		typedef typename bridge_base_type::base_container_type base_container_type;
		typedef typename bridge_base_type::value_type value_type;

	private:
		typedef bridge_front this_type;

	public:
		bridge_front(container_type& cont)
			: bridge_base_type(cont)
		{
		}

		bridge_front(const bridge_front& right)
			: bridge_base_type(right)
		{
		}

		~bridge_front(void)
		{
		}

	public:
		inline bool operator==(const bridge_at& right) const
		{
			return 
				bridge_base_type::compare_eq(right)
				&& 0 == right.index();
		}

		inline bool operator==(const bridge_front& right) const
		{
			return bridge_base_type::compare_eq(right);
		}

		template<typename OtherBridge> inline
		bool operator==(const OtherBridge&) const
		{
			return false;
		}

		template<typename OtherBridge> inline
		bool operator!=(const OtherBridge& right) const
		{
			return !this_type::operator==(right);
		}

	public:
		inline value_type operator()(void) const
		{
			const container_type& cont = bridge_base_type::_wrap_cont;
			return cont.unsafe_get_front();
		}

		template<typename Handler> inline
		typename func::foo_t_info<Handler>::result_type
			operator()(const Handler& handler) const
		{
			container_type& cont = bridge_base_type::_wrap_cont;
			return cont.unsafe_set_front(handler);
		}

	public:
		inline boost::function1<value_type&, base_container_type&>
			native_handler(void) const
		{
			return boost::bind(&container_type::unlock_front, _1);
		}

		template<typename Op_Handler, typename OtherBridge> inline
		boost::function1
		<
			typename func::foo_t_info<Op_Handler>::result_type, 
			base_container_type&
		>
			link(const Op_Handler& op_handler, const OtherBridge& other_bridge) const
		{
			return 
				bridge_base_type::pro_s_link_handler(
					op_handler, 
					this_type::native_handler(), 
					other_bridge.native_handler());
		}

		template<typename Op_Handler, typename Handler> inline
		boost::function1
		<
			typename func::foo_t_info<Op_Handler>::result_type, 
			base_container_type&
		>
			link_handler(const Op_Handler& op_handler, const Handler& handler) const
		{
			return bridge_base_type::pro_s_link_handler(op_handler, this_type::native_handler(), handler);
		}
	};

	class bridge_back
		: public detail::basic_bridge<safe_options_vector>
	{
	private:
		typedef detail::basic_bridge<safe_options_vector> bridge_base_type;

	public:
		typedef typename bridge_base_type::container_type container_type;
		typedef typename bridge_base_type::base_container_type base_container_type;
		typedef typename bridge_base_type::value_type value_type;

	private:
		typedef bridge_back this_type;

	public:
		bridge_back(container_type& cont)
			: bridge_base_type(cont)
		{
		}

		bridge_back(const bridge_back& right)
			: bridge_base_type(right)
		{
		}

		~bridge_back(void)
		{
		}

	public:
		inline bool operator==(const bridge_at& right) const
		{
			const container_type& cont = bridge_base_type::_wrap_cont;
			return bridge_base_type::compare_eq(right)
					&& (cont.size() - 1) == right.index();
		}

		inline bool operator==(const bridge_back& right) const
		{
			return bridge_base_type::compare_eq(right);
		}

		template<typename OtherBridge> inline
		bool operator==(const OtherBridge&) const
		{
			return false;
		}

		template<typename OtherBridge> inline
		bool operator!=(const OtherBridge& right) const
		{
			return !this_type::operator==(right);
		}

	public:
		inline value_type operator()(void) const
		{
			const container_type& cont = bridge_base_type::_wrap_cont;
			return cont.unsafe_get_back();
		}

		template<typename Handler> inline
		typename func::foo_t_info<Handler>::result_type
			operator()(const Handler& handler) const
		{
			container_type& cont = bridge_base_type::_wrap_cont;
			return cont.unsafe_set_back(handler);
		}

	public:
		inline boost::function1<value_type&, base_container_type&>
			native_handler(void) const
		{
			return boost::bind(&container_type::unlock_back, _1);
		}

		template<typename Op_Handler, typename OtherBridge> inline
		boost::function1
		<
			typename func::foo_t_info<Op_Handler>::result_type, 
			base_container_type&
		>
			link(const Op_Handler& op_handler, const OtherBridge& other_bridge) const
		{
			return 
				bridge_base_type::pro_s_link_handler(
					op_handler, 
					this_type::native_handler(), 
					other_bridge.native_handler());
		}

		template<typename Op_Handler, typename Handler> inline
		boost::function1
		<
			typename func::foo_t_info<Op_Handler>::result_type, 
			base_container_type&
		>
			link_handler(const Op_Handler& op_handler, const Handler& handler) const
		{
			return bridge_base_type::pro_s_link_handler(op_handler, this_type::native_handler(), handler);
		}
	};

public:

	// operator[]
	//warning "operator is not safe because idx is not safe, so need using try-catch"
	// example:
	// trd1 vt[vt.size() - 1];
	// trd2 vt.clear();
	// order vt.size() vt.clear() vt[size] // now the vt.size is zero assert fail

	// try-catch get and set 

	// operator[]
	inline detail::safe_element_reference<bridge_at> operator[](size_type idx)
	{
		return detail::safe_element_reference<bridge_at>(bridge_at(*this, idx));
	}

	inline value_type operator[](size_type idx) const
	{
		return this_type::unsafe_get_at(idx);
	}

	// at
	inline detail::safe_element_reference<bridge_at> at(size_type idx)
	{
		return detail::safe_element_reference<bridge_at>(bridge_at(*this, idx));
	}

	inline value_type at(size_type idx) const
	{
		return this_type::unsafe_get_at(idx);
	}

	// front
	inline detail::safe_element_reference<bridge_front> front(void)
	{
		return detail::safe_element_reference<bridge_front>(bridge_front(*this));
	}

	inline value_type front(void) const
	{
		return this_type::unsafe_get_front();
	}

	// back
	inline detail::safe_element_reference<bridge_back> back(void)
	{
		return detail::safe_element_reference<bridge_back>(bridge_back(*this));
	}

	inline value_type back(void) const
	{
		return this_type::unsafe_get_back();
	}

	
	// non-try-catch get and set 
	// operator[] at()
	//bool get_value(size_type idx, value_type& val) const;
	//bool set_value(size_type idx, BOOST_RV_REF(value_type) val);
	//bool set_value(size_type idx, const value_type& val);

	//bool get_at(size_type idx, value_type& val) const;
	//bool set_at(size_type idx, BOOST_RV_REF(value_type) val);
	//bool set_at(size_type idx, const value_type& val);

	YGGR_PP_SAFE_SEQ_AT_IMPL()

	//// front
	//bool get_front(value_type& val) const;
	//bool set_front(BOOST_RV_REF(value_type) val);
	//bool set_front(const value_type& val);

	YGGR_PP_SAFE_SEQ_FRONT_IMPL()

	//// back()
	//bool get_back(value_type& val) const;
	//bool set_back(BOOST_RV_REF(value_type) val);
	//bool set_back(const value_type& val);

	YGGR_PP_SAFE_SEQ_BACK_IMPL()

	template<typename Handler>
	typename func::foo_t_info<Handler>::result_type
		data(const Handler& handler)
	{
		write_lock_type lk(_mutex);
		return handler(_base, (_base.empty()? static_cast<value_type*>(0) : &_base[0]));
	}

	template<typename Handler>
	typename func::foo_t_info<Handler>::result_type
		data(const Handler& handler) const
	{
		read_lock_type lk(_mutex);
		return handler(_base, (_base.empty()? static_cast<const value_type*>(0) : &_base[0]));
	}

	//modifiers:
	////assign
	//template<typename InputIterator>
	//void assign(InputIterator first, InputIterator last);
	
	//void assign(size_type size, const value_type& val);

//#ifndef YGGR_NO_CXX11_HDR_INITIALIZER_LIST
	//void assign(std::initializer_list<value_type> list);
//#endif // YGGR_NO_CXX11_HDR_INITIALIZER_LIST
	
	YGGR_PP_SAFE_SEQ_ASSIGN_IMPL()

	//bool push_back(BOOST_RV_REF(value_type) val);
	//bool push_back(const value_type& val);

	//template<typename Handler> 
	//typename Handler::result_type
	//	push_back(BOOST_RV_REF(value_type) val, const Handler& handler);
	
	//template<typename Handler> 
	//typename Handler::result_type
	//	push_back(const value_type& val, const Handler& handler);

	YGGR_PP_SAFE_SEQ_PUSH_BACK_IMPL()

	//bool pop_back(void);
	//bool pop_back(value_type& val);

	//template<typename Handler>
	//typename Handler::result_type
	//	pop_back(const Handler& handler);
	
	//template<typename Handler>
	//typename Handler::result_type
	//	pop_back(value_type& val, const Handler& handler);

	YGGR_PP_SAFE_SEQ_POP_BACK_IMPL()

	// insert
	//template<typename IterHandler>
	//bool insert(const IterHandler& iter_handler, 
	//			BOOST_RV_REF(value_type) val);

	//template<typename IterHandler>
	//bool insert(const IterHandler& iter_handler, 
	//			const value_type& val);

	//template<typename IterHandler, typename Handler>
	//typename Handler::result_type
	//	insert(const IterHandler& iter_handler, 
	//			BOOST_RV_REF(value_type) val,
	//			const Handler& handler);

	//template<typename IterHandler, typename Handler>
	//typename Handler::result_type
	//	insert(const IterHandler& iter_handler, 
	//			const value_type& val,
	//			const Handler& handler);

	//template<typename IterHandler>
	//bool insert(const IterHandler& iter_handler,
	//				size_type n,
	//				const value_type& val);

	//template<typename IterHandler, typename InputIter>
	//bool insert(const IterHandler& iter_handler,
	//				InputIter s, InputIter e );

	//template<typename IterHandler, typename InputIter>
	//typename Handler::result_type
	//	insert(const IterHandler& iter_handler,
	//				InputIter s, InputIter e,
	//				const Handler& handler);

//#ifndef YGGR_NO_CXX11_HDR_INITIALIZER_LIST

	//template<typename IterHandler>
	//bool insert(const IterHandler& iter_handler, 
	//				std::initializer_list<value_type> list);

	//template<typename IterHandler, typename Handler>
	//typename Handler::result_type
	//	insert(const IterHandler& iter_handler,
	//				std::initializer_list<value_type> list,
	//				const Handler& handler );

//#endif // YGGR_NO_CXX11_HDR_INITIALIZER_LIST

	YGGR_PP_SAFE_SEQ_INSERT_FUNCTION_IMPL()

	//// erase(iterator) or  erase(std::pair<iterator, iterator>)
	//template<typename IterHandler>
	//bool erase(const IterHandler&);

	//// erase(iterator, iterator)
	//template<typename EHandler, typename ZHandler>
	//bool erase(const EHandler& eins_handler, const ZHandler& zwei_iter_handler);

	//// handler(erase(std::pair<iterator, iterator>))
	//template<typename EHandler, typename ZHandler>
	//typename Handler::result_type erase(const EHandler& iter_handler, const Handler& handler);

	//// handler(erase(iterator, iterator))
	//template<typename EIterHandler, typename ZIterHandler, typename Handler>
	//typename Handler::result_type erase(const EIterHandler&, const ZIterHandler&, const Handler&)

	YGGR_PP_SAFE_SEQ_ERASE_FUNCTION_IMPL()

	void clear(void)
	{
		base_type tmp(this_type::get_allocator());
		{
			write_lock_type lk(_mutex);
			_base.swap(tmp);
		}
	}

	//// emplace
	//template <typename IterHandler, typename ...Args>
	//bool emplace(const IterHandler& iter_handler, non_handler
	//				Args&&... args);

	//template <typename IterHandler, typename Handler, typename ...Args>
	//typename Handler::result_type
	//	emplace(const IterHandler& iter_handler,
	//				const Handler& handler,
	//				Args&&... args);

	//template<typename ...Args>
	//bool emplace_back(Args&&... args);

#if !defined(YGGR_NO_CXX11_VARIADIC_TEMPLATES)
	
	YGGR_PP_SAFE_SEQ_EMPLACE_FUNCTION_IMPL( _ )
	YGGR_PP_SAFE_SEQ_EMPLACE_BACK_FUNCTION_IMPL( _ )

#else

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		YGGR_PP_SAFE_SEQ_EMPLACE_FUNCTION_IMPL( __n__ ) \
		YGGR_PP_SAFE_SEQ_EMPLACE_BACK_FUNCTION_IMPL( __n__ )

#	define YGGR_PP_FOO_ARG_NAME() arg

#	define BOOST_PP_LOCAL_LIMITS (0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN)
#	include BOOST_PP_LOCAL_ITERATE()

#	undef YGGR_PP_FOO_ARG_NAME

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

	// allocator:
	allocator_type get_allocator(void) const
	{
		read_lock_type lk(_mutex);
		return _base.get_allocator();
	}

	// safe other:
	bool is_exists(const value_type& val) const
	{
		read_lock_type lk(_mutex);
		return std::find(_base.begin(), _base.end(), val) != _base.end();
	}

	template<typename Pred>
	bool is_exists(const Pred& pred) const
	{
		read_lock_type lk(_mutex);
		return std::find_if(_base.begin(), _base.end(), pred) != _base.end();
	}

	//value_type begin_value(void) const;
	//bool get_begin_value(value_type& val) const;
	//bool set_begin_value(BOOST_RV_REF(value_type) val);
	//bool set_begin_value(const value_type& val);

	YGGR_PP_SAFE_SEQ_BEGIN_VALUE_FUCTION_IMPL()

	//value_type rbegin_value(void) const;
	//bool get_rbegin_value(value_type& val) const;
	//bool set_rbegin_value(BOOST_RV_REF(value_type) val);
	//bool set_rbegin_value(const value_type& val);

	YGGR_PP_SAFE_SEQ_RBEGIN_VALUE_FUCTION_IMPL()

	//base_type& copy_to_base(base_type& out, size_type s = 0) const;
	//base_type& copy_to_base(base_type& out, std::size_t s, std::size_t e) const;

	YGGR_PP_SAFE_CONTAINER_COPY_TO_BASE_RA_IMPL()

	//isomeric_same_base_type& copy_to_base(isomeric_same_base_type& out, size_type s = 0) const;
	//isomeric_same_base_type& copy_to_base(isomeric_same_base_type& out, std::size_t s, std::size_t e) const;

	YGGR_PP_SAFE_CONTAINER_ISOMERIC_SAME_COPY_TO_BASE_RA_IMPL(
		3, Vector, container::is_isomeric_same_options_vector)

	//base_type load(void) const;
	//base_type load(const allocator_type& alloc) const;

	YGGR_PP_SAFE_CONTAINER_LOAD_IMPL()

	// isomeric_same_base_type load(void) const;

	YGGR_PP_SAFE_CONTAINER_ISOMERIC_SAME_LOAD_IMPL(container::is_isomeric_same_options_vector)

	//void store(BOOST_RV_REF(base_type) base);
	//void store(const base_type& base);

	YGGR_PP_SAFE_CONTAINER_STONE_IMPL()

	//void store(BOOST_RV_REF(isomeric_same_base_type) base);
	//void store(const isomeric_same_base_type& base);

	YGGR_PP_SAFE_CONTAINER_ISOMERIC_SAME_STONE_IMPL(
		3, Vector, container::is_isomeric_same_options_vector)

	// use handler:
	//template<typename Handler>
	//typename Handler::result_type using_handler(const Handler& handler);

	//template<typename Handler>
	//typename Handler::result_type using_handler(const Handler& handler) const;

	//template<typename Handler, typename Return_Handler>
	//typename Handler::result_type 
	//	using_handler(const Handler& handler, const Return_Handler& ret_handler);

	//template<typename Handler, typename Return_Handler>
	//typename Handler::result_type 
	//	using_handler(const Handler& handler, const Return_Handler& ret_handler) const;

	YGGR_PP_SAFE_CONTAINER_USING_HANDLER_IMPL()

	// using_hook_handler
	//template<typename Handler> inline
	//typename Handler::result_type 
	//	using_hook_handler(const Handler& handler);
	//
	//template<typename Handler> inline
	//typename Handler::result_type
	//	using_hook_handler(const Handler& handler) const;
	//
	//template<typename Handler, typename Return_Handler> inline
	//typename Handler::result_type 
	//	using_hook_handler(const Handler& handler, 
	//						const Return_Handler& ret_handler);
	//
	//template<typename Handler, typename Return_Handler> inline 
	//typename Handler::result_type
	//	using_hook_handler(const Handler& handler,
	//						const Return_Handler& ret_handler) const;

	YGGR_PP_SAFE_CONTAINER_USING_HOOK_HANDLER_IMPL()

	// for_each_iterator
	//template<typename Handler>
	//void for_each_iterator(const Handler& handler);

	//template<typename Handler>
	//void for_each_iterator(const Handler& handler) const;

	//template<typename Handler, typename Filter_Handler>
	//void for_each_iterator(const Handler& handler, 
	//							const Filter_Handler& filter_handler);

	//template<typename Handler, typename Filter_Handler>
	//void for_each_iterator(const Handler& handler, 
	//							const Filter_Handler& filter_handler) const;

	YGGR_PP_SAFE_CONTAINER_FOR_EACH_ITERATOR_IMPL()

	//template<typename Handler>
	//void for_each_iterator_reverse(const Handler& handler);

	//template<typename Handler>
	//void for_each_iterator_reverse(const Handler& handler) const;

	//template<typename Handler, typename Filter_Handler>
	//void for_each_iterator_reverse(const Handler& handler, 
	//									const Filter_Handler& filter_handler);

	//template<typename Handler, typename Filter_Handler>
	//void for_each_iterator_reverse(const Handler& handler, 
	//									const Filter_Handler& filter_handler) const;

	YGGR_PP_SAFE_CONTAINER_FOR_EACH_ITERATOR_REVERSE_IMPL()

	// for_each
	//template<typename Handler>
	//void for_each(const Handler& handler);

	//template<typename Handler>
	//void for_each(const Handler& handler) const;

	//template<typename Handler, typename Filter_Handler>
	//void for_each(const Handler& handler, 
	//				const Filter_Handler& filter_handler);

	//template<typename Handler, typename Filter_Handler>
	//void for_each(const Handler& handler, 
	//				const Filter_Handler& filter_handler) const;

	YGGR_PP_SAFE_CONTAINER_FOR_EACH_IMPL()

	//template<typename Handler>
	//void for_each_reverse(const Handler& handler);

	//template<typename Handler>
	//void for_each_reverse(const Handler& handler) const;

	//template<typename Handler, typename Filter_Handler>
	//void for_each_reverse(const Handler& handler, 
	//							const Filter_Handler& filter_handler);

	//template<typename Handler, typename Filter_Handler>
	//void for_each_reverse(const Handler& handler, 
	//						const Filter_Handler& filter_handler) const;

	YGGR_PP_SAFE_CONTAINER_FOR_EACH_REVERSE_IMPL()

private:
	mutable mutex_type _mutex;
	base_type _base;
};

YGGR_PP_SAFE_CONTAINER_COMPARER_FULL(4, safe_options_vector)

YGGR_PP_SAFE_CONTAINER_ISOMERIC_SAME_COMPARER_FULL(4, safe_options_vector, container::is_isomeric_same_options_vector)

YGGR_PP_SAFE_CONTAINER_RANGE_SIZE_SUPPORT(4, safe_options_vector)

} // namespace safe_container
} // namespace yggr

YGGR_PP_SAFE_CONTAINER_RANGE_SIZE_CHECK_PASS_SUPPORT(4, yggr::safe_container::safe_options_vector)

//-------------------------------------------------non-member_foo cpp20-------------------------------------
namespace yggr
{
namespace safe_container
{

// erase
template<template<typename _T, typename _A, typename _O> class Vt,
			typename T, typename M, typename A, typename O, typename Val> inline
typename safe_options_vector<T, M, A, O, Vt>::size_type
	erase(safe_options_vector<T, M, A, O, Vt>& c, const Val& val)
{
	return detail::erase_value_20_rnd_container(c, val);
}

// erase_if
template<template<typename _T, typename _A, typename _O> class Vt,
			typename T, typename M, typename A, typename O, typename Pred> inline
typename safe_options_vector<T, M, A, O, Vt>::size_type
	erase_if(safe_options_vector<T, M, A, O, Vt>& c, Pred pred)
{
	return detail::erase_if_20_rnd_container(c, pred);
}

} // namespace safe_container
} // namespace yggr

//--------------------------------------------------support swap-------------------------------------
namespace yggr
{
namespace safe_container
{
namespace swap_support
{
	YGGR_PP_SAFE_CONTAINER_SWAP(4, safe_options_vector)

	YGGR_PP_SAFE_CONTAINER_ISOMERIC_SAME_SWAP(
		4, safe_options_vector, container::is_isomeric_same_options_vector)

} // namespace swap_support

using swap_support::swap;

} // namespace safe_container
} // namespace yggr

//--------------------------------------------------support std::swap-------------------------------------
namespace std
{
	using ::yggr::safe_container::swap_support::swap;
} // namespace std

//-------------------------------------------------support boost::swap-----------------------------------
namespace boost
{
	using ::yggr::safe_container::swap_support::swap;
} // namespace boost

#endif //__YGGR_SAFE_CONTAINER_SAFE_OPTIONS_VECTOR_HPP__
