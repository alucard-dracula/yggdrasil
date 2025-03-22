//umap_merge_impl.hpp

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

#ifndef __YGGR_CONTAINER_DETAIL_UMAP_MERGE_IMPL_HPP__
#define __YGGR_CONTAINER_DETAIL_UMAP_MERGE_IMPL_HPP__

#include <yggr/container/detail/map_merge_impl.hpp>

namespace yggr
{
namespace container
{
namespace detail
{

// map_merge
// eei -> "erase and insert" 
template<typename L, typename R> inline
void umap_merge_umap_impl_eei(L& l, R& r)
{
	map_merge_map_impl_eei(l, r);
}

template<typename L, typename R>
void umap_merge_umultimap_impl_eei(L& l, R& r)
{
	typedef R right_umap_type;
	typedef typename right_umap_type::iterator right_umap_iter_type;
	typedef typename right_umap_type::size_type right_umap_size_type;
	typedef typename right_umap_type::key_equal right_value_compare_type;

	if(static_cast<void*>(boost::addressof(l)) == static_cast<void*>(boost::addressof(r)))
	{
		return;
	}

	right_umap_size_type risize = 0;
	right_umap_iter_type iter_chk = r.begin();
	right_umap_iter_type iter_next;

	right_value_compare_type cmper = r.key_eq();

	for(;r.size() != risize;)
	{
		if(l.find(iter_chk->first) == l.end())
		{
			for(iter_next = advance_copy(iter_chk, 1);
				(iter_next != r.end()) && (cmper(iter_chk->first, iter_next->first)); ++risize, ++iter_next);

			l.insert(boost::move(*iter_chk));
			r.erase(iter_chk);
			iter_chk = iter_next;
		}
		else
		{
			iter_next = iter_chk;
			do
			{
				++risize;
				++iter_next;
				// e.g std::less, 
				// now == next -> true,
				// now != next -> false
			} while((iter_next != r.end()) && (cmper(iter_chk->first, iter_next->first)));

			iter_chk = iter_next;

#		ifdef _DEBUG
			if(iter_chk == r.end())
			{
				assert(r.size() == risize);
			}
#		endif //_DEBUG
		}
	}
}

// eti -> "extract and insert" 
template<typename L, typename R> inline
void umap_merge_umap_impl_eti(L& l, R& r)
{
	map_merge_map_impl_eti(l, r);
}

template<typename L, typename R>
void umap_merge_umultimap_impl_eti(L& l, R& r)
{
	typedef R right_umap_type;
	typedef typename right_umap_type::iterator right_umap_iter_type;
	typedef typename right_umap_type::iterator right_umap_citer_type;
	typedef typename right_umap_type::size_type right_umap_size_type;
	typedef typename right_umap_type::key_equal right_value_compare_type;

	if(static_cast<void*>(boost::addressof(l)) == static_cast<void*>(boost::addressof(r)))
	{
		return;
	}

	right_umap_size_type risize = 0;
	right_umap_citer_type iter_chk = r.begin();
	right_umap_citer_type iter_next;

	right_value_compare_type cmper = r.key_eq();

	for(;r.size() != risize;)
	{
		if(l.find(iter_chk->first) == l.end())
		{
			for(iter_next = advance_copy(iter_chk, 1);
				(iter_next != r.end()) && (cmper(iter_chk->first, iter_next->first)); ++risize, ++iter_next);

			insert(l, extract(r, iter_chk));
			iter_chk = iter_next;
		}
		else
		{
			iter_next = iter_chk;
			do
			{
				++risize;
				++iter_next;
				// e.g std::less, 
				// now < next -> true,
				// now == next -> false,
				// now > next -> notexist, so using !cmper(now, next) check same value
			} while((iter_next != r.end()) && (!cmper(iter_chk->first, iter_next->first)));

			iter_chk = iter_next;

#		ifdef _DEBUG
			if(iter_chk == r.end())
			{
				assert(r.size() == risize);
			}
#		endif //_DEBUG
		}
	}
}

// multimap
// eei -> "erase and insert" 
template<typename L, typename R> inline
void umultimap_merge_impl_eei(L& l, R& r)
{
	typedef R right_umap_type;

	if(static_cast<void*>(boost::addressof(l)) == static_cast<void*>(boost::addressof(r)))
	{
		return;
	}

	if(r.size())
	{
		right_umap_type r_tmp(0, r.hash_function(), r.key_eq(), r.get_allocator());
		::yggr::swap(r_tmp, r);
		l.insert(r_tmp.begin(), r_tmp.end());
	}
}

// eti -> "extract and insert" 
template<typename L, typename R> inline
void umultimap_merge_impl_eti(L& l, R& r)
{
	multimap_merge_impl_eti(l, r);
}

} // namespace detail
} // namespace container
} // namespace yggr

#endif // __YGGR_CONTAINER_DETAIL_UMAP_MERGE_IMPL_HPP__
