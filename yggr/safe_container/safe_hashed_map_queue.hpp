//safe_hashed_map_queue.hpp

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

#ifndef __YGGR_SAFE_CONTAINER_SAFE_HASHED_MAP_QUEUE_HPP__
#define __YGGR_SAFE_CONTAINER_SAFE_HASHED_MAP_QUEUE_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/base/static_constant.hpp>

#include <yggr/thread/boost_thread.hpp>

#include <yggr/compatibility/stl_exception.hpp>
#include <yggr/nonable/nonable.hpp>

#include <yggr/move/move.hpp>
#include <yggr/utility/swap.hpp>

#include <yggr/type_traits/has_mapped_t.hpp>
#include <yggr/mplex/typename_replaceor.hpp>
#include <yggr/mplex/limits.hpp>

#include <yggr/smart_ptr_ex/shared_ptr.hpp>
#include <yggr/smart_ptr_ex/default_deleter.hpp>

#include <yggr/container_ex/is_allocator.hpp>

#include <yggr/safe_container/safe_wrap.hpp>
#include <yggr/safe_container/safe_deque.hpp>
#include <yggr/safe_container/safe_multi_index_container.hpp>
#include <yggr/safe_container/safe_multi_index_hashed_index.hpp>
#include <yggr/safe_container/safe_multi_index_random_access_index.hpp>

#include <yggr/safe_container/safe_buffered_object.hpp>

#include <boost/utility/enable_if.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/ref.hpp>
#include <boost/mpl/size_t.hpp>
#include <boost/multi_index/global_fun.hpp>

#include <cassert>
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

/* !!! not support yggr::segment and boost::interprocess !!!*/

namespace yggr
{
namespace safe_container
{

template<typename Key, typename Val,
			typename Mutex = void,
			typename Alloc = std::allocator<Val> >
class safe_hashed_map_queue
	: private nonable::noncopyable,
		private nonable::nonmoveable

{
public:
	typedef Key key_type;
	typedef Val mapped_type;

private:
	typedef Mutex tpl_arg_mutex_type;

private:
	typedef Alloc deque_value_alloc_type;
	typedef safe_deque<mapped_type, tpl_arg_mutex_type, deque_value_alloc_type> safe_deque_type;
	typedef typename safe_deque_type::base_type unsafe_deque_type;

	typedef ::yggr::shared_ptr<safe_deque_type> safe_deque_ptr_type;
	typedef typename
		mplex::typename_replaceor
		<
			deque_value_alloc_type,
			safe_deque_type
		>::type deque_alloc_type;


private:
	typedef std::pair<key_type, safe_deque_ptr_type> hash_store_value_type;

	typedef boost::mpl::size_t<0> tag_rnd_type;
	typedef boost::mpl::size_t<1> tag_hash_type;

private:
	inline static const key_type&
		prv_s_midx_value_get_key(const hash_store_value_type& data)
	{
		return data.first;
	}

	inline static safe_deque_ptr_type
		prv_s_midx_value_get_deque(const hash_store_value_type& data)
	{
		return data.second;
	}

private:
	typedef typename
		mplex::typename_replaceor
		<
			deque_value_alloc_type,
			hash_store_value_type
		>::type midx_alloc_type;

	typedef
		::yggr::safe_container::safe_multi_index_container
		<
			hash_store_value_type,

			boost::multi_index::indexed_by
			<
				boost::multi_index::random_access
				<
					boost::multi_index::tag<tag_rnd_type>
				>,

				boost::multi_index::hashed_unique
				<
					boost::multi_index::tag<tag_hash_type>,
					boost::multi_index::global_fun
					<
						const hash_store_value_type&,
						const key_type&,
						&safe_hashed_map_queue::prv_s_midx_value_get_key
					>
				>
			>,
			tpl_arg_mutex_type,
			midx_alloc_type
		> safe_midx_type;

	typedef typename safe_midx_type::base_type unsafe_midx_type;

	typedef typename
		::yggr::safe_container::index
		<
			safe_midx_type,
			tag_rnd_type
		>::type safe_idx_rnd_type;

	typedef typename safe_idx_rnd_type::base_type unsafe_idx_rnd_type;
	typedef typename safe_idx_rnd_type::index_id_type idx_rnd_index_id_type;
	typedef typename safe_idx_rnd_type::iterator idx_rnd_iter_type;

	typedef typename
		::yggr::safe_container::index
		<
			safe_midx_type,
			tag_hash_type
		>::type safe_idx_hash_type;

	typedef typename safe_idx_hash_type::base_type unsafe_idx_hash_type;
	typedef typename safe_idx_hash_type::index_id_type idx_hash_index_id_type;
	typedef typename safe_idx_hash_type::iterator idx_hash_iter_type;

public:
	typedef typename safe_midx_type::size_type size_type;

private:
	typedef typename
		mplex::typename_replaceor
		<
			deque_value_alloc_type,
			size_type
		>::type size_alloc_type;

private:
	typedef safe_wrap<size_type> wrap_size_type;
	typedef smart_ptr_ex::default_deleter<size_type> size_deleter_type;

	YGGR_STATIC_CONSTANT(size_type, npos = static_cast<size_type>(-1));

public:
	typedef std::pair<key_type, mapped_type> value_type;
	typedef deque_value_alloc_type allocator_type;

private:
	typedef safe_hashed_map_queue this_type;

public:
	safe_hashed_map_queue(void)
		: _size_wrap(::yggr::allocate_shared<size_type>(size_alloc_type(), 0)),
			_now_idx_wrap(::yggr::allocate_shared<size_type>(size_alloc_type(), 0))

	{
	}

	safe_hashed_map_queue(const allocator_type& alloc)
		: _midx(midx_alloc_type(alloc)),
			_size_wrap(static_cast<size_type>(0), size_deleter_type(), size_alloc_type(alloc)),
			_now_idx_wrap(static_cast<size_type>(0), size_deleter_type(), size_alloc_type(alloc))

	{
	}

	~safe_hashed_map_queue(void)
	{
	}

public:
	// push
	size_type push(const key_type& key, BOOST_RV_REF(mapped_type) val)
	{
		safe_deque_ptr_type ptr = this_type::prv_get_queue(key);

		if(!ptr)
		{
			const mapped_type& val_cref = val;
			safe_deque_ptr_type tmp_ptr = this_type::prv_s_create_deque(_midx.get_allocator(), 1, val_cref);

			if(!tmp_ptr)
			{
				return 0;
			}

			ptr = _midx.template get<tag_rnd_type>().push_back(
					std::make_pair(key, tmp_ptr),
					boost::bind(&this_type::prv_s_handler_midx_push_back, _1, _2));

			if(ptr != tmp_ptr)
			{
				unsafe_deque_type tmp_deque;
				tmp_ptr->swap(tmp_deque);

				assert(tmp_deque.size() == 1);

				return
					this_type::prv_plus_size(
						tmp_deque.size() && ptr->push_back(boost::move(tmp_deque.front())),
						1);
			}
			else
			{
				return this_type::prv_plus_size(true, 1);
			}
		}
		else
		{
			return
				this_type::prv_plus_size(
					ptr->push_back(boost::move(val)), 1);
		}
	}

	size_type push(const key_type& key, const mapped_type& val)
	{
		safe_deque_ptr_type ptr = this_type::prv_get_queue(key);

		if(!ptr)
		{
			safe_deque_ptr_type tmp_ptr = this_type::prv_s_create_deque(_midx.get_allocator(), 1, val);

			if(!tmp_ptr)
			{
				return 0;
			}

			ptr = _midx.template get<tag_rnd_type>().push_back(
					std::make_pair(key, tmp_ptr),
					boost::bind(&this_type::prv_s_handler_midx_push_back, _1, _2));

			if(ptr != tmp_ptr)
			{
				unsafe_deque_type tmp_deque;
				tmp_ptr->swap(tmp_deque);

				assert(tmp_deque.size() == 1);

				return
					this_type::prv_plus_size(
						tmp_deque.size() && ptr->push_back(boost::move(tmp_deque.front())),
						1);

			}
			else
			{
				return this_type::prv_plus_size(true, 1);
			}
		}
		else
		{
			return
				this_type::prv_plus_size(
					ptr->push_back(boost::move(val)), 1);
		}
	}

	inline size_type push(BOOST_RV_REF(value_type) val)
	{
		return this_type::push(val.first, boost::move(val.second));
	}

	inline size_type push(const value_type& val)
	{
		return this_type::push(val.first, val.second);
	}


public:
	// pop
	size_type pop(mapped_type& val)
	{
		typedef std::pair<safe_deque_ptr_type, size_type> get_queue_rst_type;

		//safe_idx_rnd_type& safe_rnd_cont = _midx.template get<tag_rnd_type>();

		//size_type pop_midx_idx = this_type::npos;
		get_queue_rst_type rst;

		for(;_size_wrap.load();)
		{
			rst = this_type::prv_get_queue();

			if(rst.second == this_type::npos)
			{
				break;
			}

			if(!(rst.first && rst.first->size()))
			{
				continue;
			}

			if(!rst.first->pop_front(val))
			{
				continue;
			}

			this_type::prv_minus_size(true, 1);

			if(rst.first->size() == _size_wrap.load())
			{
				this_type::prv_set_pop_index(rst.second);
			}

			return 1;
		}

		return 0;
	}

	bool front(mapped_type& val) const
	{
		typedef std::pair<safe_deque_ptr_type, size_type> get_queue_rst_type;

		safe_idx_rnd_type& safe_rnd_cont = _midx.template get<tag_rnd_type>();

		size_type pop_midx_idx = this_type::npos;
		get_queue_rst_type rst;

		for(;_size_wrap.load();)
		{
			rst = this_type::prv_get_queue();

			if(rst.second == this_type::npos)
			{
				break;
			}

			if(!(rst.first && rst.first->size()))
			{
				continue;
			}

			if(!rst.first->get_front(val))
			{
				continue;
			}

			return true;
		}

		return false;
	}


	inline size_type remove(const key_type& key)
	{
		safe_deque_ptr_type ptr = this_type::prv_drop_queue(key);

		if(!ptr)
		{
			return 0;
		}

#	ifdef _DEBUG
		size_type rm_size = ptr->size();
		size_type chk_size = this_type::prv_minus_size(true, rm_size);
		assert(rm_size == chk_size);
		return chk_size;
#	else
		return this_type::prv_minus_size(true, ptr->size());
#	endif // _DEBUG
	}

	inline void clear(void)
	{
		unsafe_midx_type tmp(_midx.get_allocator());
		_midx.using_handler(
			boost::bind(&this_type::prv_handler_clear_midx, this, _1, boost::ref(tmp)));
	}

	inline bool empty(void) const
	{
		return !_size_wrap.load();
	}

	inline size_type size(void) const
	{
		return _size_wrap.load();
	}

private:
	inline size_type prv_get_pop_index(void) const
	{
		return
			_now_idx_wrap.using_handler(
				boost::bind(&this_type::prv_s_handler_get_pop_index, _1, _midx.size()));
	}

	inline static size_type prv_s_handler_get_pop_index(typename wrap_size_type::base_type& base,
														size_type now_midx_size)
	{
		assert(base);
		if(!base)
		{
			return this_type::npos;
		}
		else
		{
			size_type now_idx = *base;
			size_type next_idx = now_idx + 1;
			(*base) = next_idx < now_midx_size? next_idx : 0;
			return now_idx;
		}
	}

	inline void prv_set_pop_index(size_type idx)
	{
		_now_idx_wrap.store(idx);
	}

private:
	inline void prv_handler_clear_midx(unsafe_midx_type& final, unsafe_midx_type& empty_final)
	{
		assert(empty_final.empty());
		_size_wrap.using_handler(
			boost::bind(
				&this_type::prv_handler_clear_size,
				this, _1, boost::ref(final), boost::ref(empty_final)));
	}

	inline void prv_handler_clear_size(typename wrap_size_type::base_type& pval,
										unsafe_midx_type& final,
										unsafe_midx_type& empty_final)
	{
		assert(pval);
		_now_idx_wrap.using_handler(
			boost::bind(
				&this_type::prv_s_handler_clear_index,
				_1, boost::ref(pval), boost::ref(final), boost::ref(empty_final)));
	}

	inline void static prv_s_handler_clear_index(typename wrap_size_type::base_type& pval,
													typename wrap_size_type::base_type& pval_size,
													unsafe_midx_type& final,
													unsafe_midx_type& empty_final)
	{
		assert(pval);
		assert(pval_size);
		assert(empty_final.empty());

		if(pval)
		{
			*pval = 0;
		}

		if(pval_size)
		{
			*pval_size = 0;
		}

		::yggr::swap(final, empty_final);
	}

	inline safe_deque_ptr_type prv_drop_queue(const key_type& key)
	{
		return
			_midx.template get<tag_hash_type>().find(
				key,
				boost::bind(
					&this_type::prv_s_handler_drop_queue,
					_1, _2) );
	}

	inline static safe_deque_ptr_type
		prv_s_handler_drop_queue(unsafe_midx_type& final,
									const
										boost::fusion::pair
										<
											idx_hash_index_id_type,
											idx_hash_iter_type
										>& rst)
	{
		safe_deque_ptr_type ptr =
			rst.second == final.template get<idx_hash_index_id_type::first::value>().end()?
				safe_deque_ptr_type()
				: this_type::prv_s_midx_value_get_deque(*rst.second);

		if(ptr)
		{
			final.template get<idx_hash_index_id_type::first::value>().erase(rst.second);
		}

		return ptr;
	}

	inline safe_deque_ptr_type prv_get_queue(const key_type& key) const
	{
		return
			_midx.template get<tag_hash_type>().find(
				key,
				boost::bind(&this_type::prv_s_handler_get_queue_of_hash, _1, _2));
	}

	inline static safe_deque_ptr_type
		prv_s_handler_get_queue_of_hash(unsafe_midx_type& final,
										const
											boost::fusion::pair
											<
												idx_hash_index_id_type,
												idx_hash_iter_type
											>& rst)
	{
		return
			rst.second == final.template get<idx_hash_index_id_type::first::value>().end()?
				safe_deque_ptr_type() : this_type::prv_s_midx_value_get_deque(*rst.second);
	}

	inline std::pair<safe_deque_ptr_type, size_type>
		prv_get_queue(void) const
	{
		size_type now_idx = this_type::prv_get_pop_index();
		return
			std::make_pair(
				_midx.using_handler(
					boost::bind(&this_type::prv_s_handler_get_queue, _1, now_idx)),
				now_idx);
	}

	inline static safe_deque_ptr_type
		prv_s_handler_get_queue(const unsafe_midx_type& final, size_type now_idx)
	{
		return
			(now_idx < final.size())?
				this_type::prv_s_midx_value_get_deque(final.template get<tag_rnd_type>()[now_idx])
				: safe_deque_ptr_type();
	}

private:
	inline static safe_deque_ptr_type
		prv_s_handler_midx_push_back(unsafe_midx_type& final,
										const
											boost::fusion::pair
											<
												idx_rnd_index_id_type,
												std::pair<idx_rnd_iter_type, bool>
											>& rst)
	{
		if(rst.second.first == final.template get<idx_rnd_index_id_type::first::value>().end())
		{
			return safe_deque_ptr_type();
		}
		else
		{
			return
				this_type::prv_s_midx_value_get_deque(*rst.second.first)?
					this_type::prv_s_midx_value_get_deque(*rst.second.first)
					: (final.template get<idx_rnd_index_id_type::first::value>().erase(rst.second.first),
						safe_deque_ptr_type());
		}
	}

	inline size_type prv_plus_size(bool bsucceed, size_type size)
	{
		return
			bsucceed?
				(_size_wrap.using_handler(
					boost::bind(&this_type::prv_s_handler_plus_size, _1, size)), size)
				: 0;
	}

	inline size_type prv_minus_size(bool bsucceed, size_type size)
	{
		return
			bsucceed?
				(_size_wrap.using_handler(
					boost::bind(&this_type::prv_s_handler_minus_size, _1, size)), size)
				: 0;
	}

	template<typename Ax> inline
	static
	typename
		boost::enable_if
		<
			container_ex::is_allocator<Ax>,
			safe_deque_ptr_type
		>::type
		prv_s_create_deque(const Ax& nalloc, size_type n, const mapped_type& val)
	{
		deque_alloc_type alloc(nalloc);
		return ::yggr::allocate_shared<safe_deque_type>(alloc, n, val);
	}

	inline static size_type prv_s_handler_plus_size(typename wrap_size_type::base_type& pval, size_type size)
	{
		assert(pval);
		if(!pval)
		{
			return 0;
		}

		size_type old = (*pval);
		assert(old <= mplex::numeric_limits<size_type>::max_type::value - size);
		(*pval) += size;
		return old;
	}

	inline static size_type prv_s_handler_minus_size(typename wrap_size_type::base_type& pval, size_type size)
	{
		assert(pval);
		if(!pval)
		{
			return 0;
		}

		size_type old = (*pval);
		assert(size <= old);
		(*pval) -= size;
		return old;
	}


private:
	safe_midx_type _midx;
	wrap_size_type _size_wrap;
	mutable wrap_size_type _now_idx_wrap;

};

} // namesapce safe_container
} // namesapce yggr

#endif //__YGGR_SAFE_CONTAINER_SAFE_HASHED_MAP_QUEUE_HPP__
