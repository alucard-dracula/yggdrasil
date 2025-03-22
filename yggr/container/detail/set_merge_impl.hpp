//set_merge_impl.hpp

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

#ifndef __YGGR_CONTAINER_DETAIL_SET_MERGE_IMPL_HPP__
#define __YGGR_CONTAINER_DETAIL_SET_MERGE_IMPL_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/utility/swap.hpp>
#include <yggr/iterator_ex/iterator.hpp>

#include <boost/ref.hpp>

namespace yggr
{
namespace container
{
namespace detail
{

// set_merge
// eei -> "erase and insert" 

#if !defined(_MSC_VER) && (YGGR_CPP_VERSION < YGGR_CPP_VER_11)

template<typename L, typename R>
void set_merge_set_impl_eei(L& l, R& r)
{
	typedef R right_set_type;
	typedef typename right_set_type::iterator right_set_iter_type;
	typedef typename right_set_type::size_type right_set_size_type;

	if(static_cast<void*>(boost::addressof(l)) == static_cast<void*>(boost::addressof(r)))
	{
		return;
	}

	right_set_size_type risize = 0;
	right_set_iter_type iter_chk = r.begin();
	right_set_iter_type iter_next;

	for(;r.size() != risize;)
	{
		if(l.find(*iter_chk) == l.end())
		{
			l.insert(boost::move(*iter_chk));
			iter_next = yggr::advance_copy(iter_chk, 1);
			r.erase(iter_chk);
			iter_chk = iter_next;
		}
		else
		{
			++risize;
			++iter_chk;
		}
	}
}

#else

template<typename L, typename R>
void set_merge_set_impl_eei(L& l, R& r)
{
	typedef R right_set_type;
	typedef typename right_set_type::iterator right_set_iter_type;
	typedef typename right_set_type::size_type right_set_size_type;

	if(static_cast<void*>(boost::addressof(l)) == static_cast<void*>(boost::addressof(r)))
	{
		return;
	}

	right_set_size_type risize = 0;
	right_set_iter_type iter_chk = r.begin();

	for(;r.size() != risize;)
	{
		if(l.find(*iter_chk) == l.end())
		{
			l.insert(boost::move(*iter_chk));
			iter_chk = r.erase(iter_chk);
		}
		else
		{
			++risize;
			++iter_chk;
		}
	}
}

#endif // #if !defined(_MSC_VER) && (YGGR_CPP_VERSION < YGGR_CPP_VER_11)

template<typename L, typename R>
void set_merge_multiset_impl_eei(L& l, R& r)
{
	typedef R right_set_type;
	typedef typename right_set_type::iterator right_set_iter_type;
	typedef typename right_set_type::size_type right_set_size_type;
	typedef typename right_set_type::value_compare right_value_compare_type;

	if(static_cast<void*>(boost::addressof(l)) == static_cast<void*>(boost::addressof(r)))
	{
		return;
	}

	right_set_size_type risize = 0;
	right_set_iter_type iter_chk = r.begin();
	right_set_iter_type iter_next;

	right_value_compare_type cmper = r.value_comp();

	for(;r.size() != risize;)
	{
		if(l.find(*iter_chk) == l.end())
		{
			for(iter_next = advance_copy(iter_chk, 1);
				(iter_next != r.end()) && (!cmper(*iter_chk, *iter_next)); ++risize, ++iter_next);

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
				// now < next -> true,
				// now == next -> false,
				// now > next -> notexist, so using !cmper(now, next) check same value
			} while((iter_next != r.end()) && (!cmper(*iter_chk, *iter_next)));

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
template<typename L, typename R>
void set_merge_set_impl_eti(L& l, R& r)
{
	typedef R right_set_type;
	typedef typename right_set_type::iterator right_set_iter_type;
	typedef typename right_set_type::iterator right_set_citer_type;
	typedef typename right_set_type::size_type right_set_size_type;

	if(static_cast<void*>(boost::addressof(l)) == static_cast<void*>(boost::addressof(r)))
	{
		return;
	}

	right_set_size_type risize = 0;
	right_set_citer_type iter_chk = r.begin();
	right_set_citer_type iter_next;

	for(;r.size() != risize;)
	{
		if(l.find(*iter_chk) == l.end())
		{
			iter_next = advance_copy(iter_chk, 1);
			insert(l, extract(r, iter_chk));
			iter_chk = iter_next;
		}
		else
		{
			++risize;
			++iter_chk;
		}
	}
}

template<typename L, typename R>
void set_merge_multiset_impl_eti(L& l, R& r)
{
	typedef R right_set_type;
	typedef typename right_set_type::iterator right_set_iter_type;
	typedef typename right_set_type::iterator right_set_citer_type;
	typedef typename right_set_type::size_type right_set_size_type;
	typedef typename right_set_type::value_compare right_value_compare_type;

	if(static_cast<void*>(boost::addressof(l)) == static_cast<void*>(boost::addressof(r)))
	{
		return;
	}

	right_set_size_type risize = 0;
	right_set_citer_type iter_chk = r.begin();
	right_set_citer_type iter_next;

	right_value_compare_type cmper = r.value_comp();

	for(;r.size() != risize;)
	{
		if(l.find(*iter_chk) == l.end())
		{
			//iter_next = advance_copy(iter_chk, 1);
			for(iter_next = advance_copy(iter_chk, 1);
				(iter_next != r.end()) && (!cmper(*iter_chk, *iter_next)); ++risize, ++iter_next);
			insert(l, extract(r, iter_chk));
			//int n = r.size();
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
			}while(iter_next != r.end() && !cmper(*iter_chk, *iter_next));

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

// multiset
// eei -> "erase and insert" 
template<typename L, typename R> inline
void multiset_merge_impl_eei(L& l, R& r)
{
	typedef R right_set_type;

	if(static_cast<void*>(boost::addressof(l)) == static_cast<void*>(boost::addressof(r)))
	{
		return;
	}

	if(r.size())
	{
		right_set_type r_tmp(r.key_comp(), r.get_allocator());
		::yggr::swap(r_tmp, r);
		l.insert(r_tmp.begin(), r_tmp.end());
	}
}

// eti -> "extract and insert" 
template<typename L, typename R>
void multiset_merge_impl_eti(L& l, R& r)
{
	if(static_cast<void*>(boost::addressof(l)) == static_cast<void*>(boost::addressof(r)))
	{
		return;
	}

	for(;r.size();)
	{
		insert(l, extract(r, r.begin()));
	}
}

} // namespace detail
} // namespace container
} // namespace yggr

#endif // __YGGR_CONTAINER_DETAIL_SET_MERGE_IMPL_HPP__
