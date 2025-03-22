//knuth_morris_pratt.hpp

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

#ifndef __YGGR_ALGORITHM_KNUTH_MORRIS_PRATT_HPP__
#define __YGGR_ALGORITHM_KNUTH_MORRIS_PRATT_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/bind/bind.hpp>
#include <yggr/move/move.hpp>
#include <yggr/utility/copy_or_move_or_swap.hpp>

#include <yggr/mplex/static_assert.hpp>
#include <yggr/iterator_ex/is_iterator.hpp>

#include <yggr/container/vector.hpp>
#include <yggr/any_val/operator_wrap.hpp>

#include <yggr/algorithm/detail/kmp_traits.hpp>

#include <boost/static_assert.hpp>

#include <boost/iterator/iterator_traits.hpp>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>

#include <boost/type_traits/is_same.hpp>

#include <cassert>
#include <utility>
#include <functional>

namespace yggr
{
namespace algorithm
{

template<typename PatIter,
			typename PatTraits = detail::kmp_traits<PatIter>,
			typename PatPred = std::equal_to<typename PatTraits::value_type> >
class knuth_morris_pratt;

} // namespace algorithm
} // namespace yggr

namespace yggr
{
namespace algorithm
{

template<typename PatIter, typename PatTraits, typename PatPred>
class knuth_morris_pratt
{
public:
	typedef PatIter pat_iterator_type;
	typedef PatTraits pat_traits_type;
	typedef PatPred pat_pred_type;

#ifdef _DEBUG
public:
#else
private:
#endif // _DEBUG

	typedef typename pat_traits_type::iterator_type store_iterator_type;
	typedef typename pat_traits_type::difference_type difference_type;
	typedef typename pat_traits_type::value_type pat_value_type;
	typedef std::pair<store_iterator_type, difference_type> skip_value_type;
	typedef ::yggr::vector<skip_value_type> skip_type;

private:
	typedef typename skip_type::size_type size_type;

private:
	typedef knuth_morris_pratt this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	knuth_morris_pratt(pat_iterator_type first, pat_iterator_type last)
		: _pat_first(first), _pat_last(last),
			_skip(static_cast<size_type>(std::distance(first, last)),
					skip_value_type(last, difference_type(-1)))
	{
		assert(std::distance(first, last) > 0);

		init_skip_table();
	}

	knuth_morris_pratt(pat_iterator_type first, pat_iterator_type last,
							const pat_pred_type& pred)
		: _pat_first(first), _pat_last(last), _pat_pred(pred),
			_skip(static_cast<size_type>(std::distance(first, last)),
					skip_value_type(last, difference_type(-1)))
	{
		assert(std::distance(first, last) > 0);

		init_skip_table();
	}

	knuth_morris_pratt(BOOST_RV_REF(this_type) right)
		: _pat_first(boost::move(right._pat_first)), _pat_last(boost::move(right._pat_last)),
			_pat_pred(boost::move(right._pat_pred)), _skip(boost::move(right._skip))
	{
	}

	knuth_morris_pratt(const this_type& right)
		: _pat_first(right._pat_first), _pat_last(right._pat_last),
			_pat_pred(right._pat_pred), _skip(right._skip)
	{
	}

	~knuth_morris_pratt(void)
	{
	}

public:
	// operator=
	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		this_type& right_chk = right;
		if(this == &right_chk)
		{
			return *this;
		}

		_pat_first = boost::move(right._pat_first);
		_pat_last = boost::move(right._pat_last);
		copy_or_move_or_swap(_pat_pred, boost::move(right._pat_pred));
		copy_or_move_or_swap(_skip, boost::move(right._skip));

		return *this;
	}

	this_type& operator=(const this_type& right)
	{
		if(this == &right)
		{
			return *this;
		}

		_pat_first = right._pat_first;
		_pat_last = right._pat_last;
		_pat_pred = right._pat_pred;
		_skip = right._skip;

		return *this;
	}

public:
	// swap
	inline void swap(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		this_type::swap(right_ref);
	}

	void swap(this_type& right)
	{
		if(this == &right)
		{
			return;
		}

		::yggr::swap(_pat_first, right._pat_first);
		::yggr::swap(_pat_last, right._pat_last);
		::yggr::swap(_pat_pred, right._pat_pred);
		_skip.swap(right._skip);
	}

public:
	// operator()
	template<typename CorpusIter> inline
	std::pair<CorpusIter, CorpusIter>
		operator()(CorpusIter corpus_first, CorpusIter corpus_last) const
	{
		typedef CorpusIter corpus_iter_type;
		typedef typename detail::corpus_iterator_value<corpus_iter_type>::type value_type;

		return this_type::operator()(
					corpus_first, corpus_last,
					boost::bind(&any_val::operator_wrap::equal_to<value_type, pat_value_type>, _1, _2));
	}

	template<typename CorpusIter, typename Pred> inline
	std::pair<CorpusIter, CorpusIter>
		operator()(CorpusIter corpus_first, CorpusIter corpus_last, const Pred& pred) const
	{
		typedef CorpusIter corpus_iter_type;
		typedef std::pair<corpus_iter_type, corpus_iter_type> ret_type;

		if(corpus_first == corpus_last)
		{
			return ret_type(corpus_last, corpus_last);
		}
		else if(_skip.empty())
		{
			return ret_type(corpus_first, corpus_first);
		}
		else if(std::distance(corpus_first, corpus_last) < _skip.size())
		{
			return ret_type(corpus_last, corpus_last);
		}
		else
		{
			return do_search(corpus_first, corpus_last, pred);
		}
	}

	inline void reinit(pat_iterator_type first, pat_iterator_type last)
	{
		assert(std::distance(first, last) > 0);
		_pat_first = first;
		_pat_last = last;

		{
			skip_type tmp(static_cast<size_type>(std::distance(first, last)),
							skip_value_type(last, difference_type(-1)));
			_skip.swap(tmp);
		}

		init_skip_table();
	}

	inline void reinit(pat_iterator_type first, pat_iterator_type last,
						const pat_pred_type& pred)
	{
		assert(std::distance(first, last) > 0);
		_pat_first = first;
		_pat_last = last;
		_pat_pred = pred;

		{
			skip_type tmp(static_cast<size_type>(std::distance(first, last)),
							skip_value_type(last, difference_type(-1)));
			_skip.swap(tmp);
		}

		init_skip_table();
	}

	inline const pat_pred_type& pat_comp(void) const
	{
		return _pat_pred;
	}

#ifdef _DEBUG

	inline const skip_type& skip_table(void) const
	{
		return _skip;
	}

#endif //_DEBUG

private:
	template<typename CorpusIter, typename Pred>
	std::pair<CorpusIter, CorpusIter>
		do_search(CorpusIter s, CorpusIter e, const Pred& pred) const
	{
		typedef CorpusIter corpus_iter_type;
		typedef std::pair<corpus_iter_type, corpus_iter_type> ret_type;
		typedef const skip_value_type* const_pointer_type;

		assert(!(s == e));
		assert(!_skip.empty());

		const_pointer_type tab = &_skip[0];
		const_pointer_type pnext = tab;

		difference_type j = 0;
		store_iterator_type ds2 = _pat_first;
		corpus_iter_type ret_iter = s;

		while(s != e && ds2 != _pat_last)
		{
			if(pred(*s, *ds2))
			{
				++s;
				++ds2;
				++j;
			}
			else
			{
				if(pnext[j].second == difference_type(-1))
				{
					ds2 = pnext[j].first;
					j = 0;
					++s;
				}
				else
				{
					ds2 = pnext[j].first;
					j = pnext[j].second;
				}

				ret_iter = s;
			}
		}

		return ds2 == _pat_last? ret_type(ret_iter, s) : ret_type(e, s);
	}

	void init_skip_table(void)
	{
		typedef skip_value_type* pointer_type;

		if(_skip.empty())
		{
			return;
		}

		store_iterator_type s = _pat_first;
		store_iterator_type e = _pat_last;
		pat_pred_type pred(_pat_pred);
		difference_type tab_size = static_cast<difference_type>(_skip.size());
		pointer_type tab = &_skip[0];
		*tab = skip_value_type(s, difference_type(-1));
		pointer_type pnext = tab;
		assert(pnext);

		difference_type k = difference_type(-1);

		store_iterator_type si = s;
		store_iterator_type ki = e;

		for(difference_type i = 0, isize = tab_size - 1; i != isize;)
		{
			if(k == difference_type(-1))
			{
				++i;
				++k;
				++si;
				ki = s;

				if(pred(*ki, *si))
				{
					pnext[i] = pnext[k];
				}
				else
				{
					pnext[i].first = ki;
					pnext[i].second = k;
				}
			}
			else if(pred(*ki, *si))
			{
				++i;
				++k;
				++si;
				++ki;

				if(pred(*ki, *si))
				{
					pnext[i] = pnext[k];
				}
				else
				{
					pnext[i].first = ki;
					pnext[i].second = k;
				}
			}
			else
			{
				ki = pnext[k].first;
				k = pnext[k].second;
			}
		}

	}

private:
	store_iterator_type _pat_first;
	store_iterator_type _pat_last;
	pat_pred_type _pat_pred;
	skip_type _skip;
};

// search function
template<typename CorpusIter, typename PatIter> inline
std::pair<CorpusIter, CorpusIter>
	knuth_morris_pratt_search(CorpusIter corpus_first, CorpusIter corpus_last,
								PatIter _pat_first, PatIter _pat_last)
{
	typedef knuth_morris_pratt<PatIter> kmp_type;
	kmp_type kmp(_pat_first, _pat_last);
	return kmp(corpus_first, corpus_last);
}

template<typename CorpusIter, typename CorpusPred, typename PatIter> inline
std::pair<CorpusIter, CorpusIter>
	knuth_morris_pratt_search(CorpusIter corpus_first, CorpusIter corpus_last,
								PatIter _pat_first, PatIter _pat_last,
								const CorpusPred& corpus_pred)
{
	typedef knuth_morris_pratt<PatIter> kmp_type;
	kmp_type kmp(_pat_first, _pat_last);
	return kmp(corpus_first, corpus_last, corpus_pred);
}

template<typename CorpusIter, typename CorpusPred, typename PatIter, typename PatPred> inline
std::pair<CorpusIter, CorpusIter>
	knuth_morris_pratt_search(CorpusIter corpus_first, CorpusIter corpus_last,
								PatIter _pat_first, PatIter _pat_last,
								const CorpusPred& corpus_pred, const PatPred& pat_pred)
{
	typedef knuth_morris_pratt<PatIter, detail::kmp_traits<PatIter>, PatPred> kmp_type;
	kmp_type kmp(_pat_first, _pat_last, pat_pred);
	return kmp(corpus_first, corpus_last, corpus_pred);
}

template<typename CorpusIter,
			typename PatIter, typename PatTraits, typename PatPred> inline
std::pair<CorpusIter, CorpusIter>
	knuth_morris_pratt_search(CorpusIter corpus_first, CorpusIter corpus_last,
								const knuth_morris_pratt<PatIter, PatTraits, PatPred>& pattern)
{
	return pattern(corpus_first, corpus_last);
}

template<typename CorpusIter, typename CorpusPred,
			typename PatIter, typename PatTraits, typename PatPred> inline
std::pair<CorpusIter, CorpusIter>
	knuth_morris_pratt_search(CorpusIter corpus_first, CorpusIter corpus_last,
								const knuth_morris_pratt<PatIter, PatTraits, PatPred>& pattern,
								const CorpusPred& corpus_pred)
{
	return pattern(corpus_first, corpus_last, corpus_pred);
}

} // namespace algorithm

using algorithm::knuth_morris_pratt;
using algorithm::knuth_morris_pratt_search;

} // namespace yggr

//swap

namespace yggr
{
namespace algorithm
{
namespace swap_support
{

template<typename PatIter, typename PatTraits, typename PatPred> inline
void swap(knuth_morris_pratt<PatIter, PatTraits, PatPred>& l,
			knuth_morris_pratt<PatIter, PatTraits, PatPred>& r)
{
	l.swap(r);
}

} // namespace swap_support

using swap_support::swap;

} // namespace algorithm
} // namespace yggr

namespace std
{
	using ::yggr::algorithm::swap_support::swap;
} // namespace std

namespace boost
{
	using ::yggr::algorithm::swap_support::swap;
} // namespace boost

#endif  // __YGGR_ALGORITHM_KNUTH_MORRIS_PRATT_HPP__
