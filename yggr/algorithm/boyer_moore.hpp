// boyer_moore.hpp

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

#ifndef __YGGR_ALGORITHM_BOYER_MOORE_HPP__
#define __YGGR_ALGORITHM_BOYER_MOORE_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/bind/bind.hpp>
#include <yggr/utility/copy_or_move_or_swap.hpp>

#include <yggr/container/index_generator.hpp>
#include <yggr/container/vector.hpp>
#include <yggr/any_val/operator_wrap.hpp>

#include <yggr/algorithm/detail/bm_skip_table.hpp>
#include <yggr/algorithm/detail/bm_traits.hpp>

#include <boost/ref.hpp>
#include <boost/iterator/iterator_traits.hpp>

#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>

#include <boost/mpl/if.hpp>
#include <boost/mpl/size_t.hpp>

#include <utility>
#include <limits>
#include <iterator>
#include <functional>

namespace yggr
{
namespace algorithm
{

template<typename PatIter,
			typename PatTraits = detail::bm_traits<PatIter>,
			typename Key_Extractor = container::identity<typename PatTraits::value_type>,
			typename PatPred = std::equal_to<typename PatTraits::value_type> >
class boyer_moore;

} // namespace algorithm
} // namespace yggr

namespace yggr
{
namespace algorithm
{


template<typename PatIter, typename PatTraits, typename Key_Extractor, typename PatPred>
class boyer_moore
{
public:
	typedef PatIter pat_iterator_type;
	typedef PatTraits pat_traits_type;
	typedef Key_Extractor key_extor_type;
	typedef PatPred pat_pred_type;

#ifdef _DEBUG
public:
#else
private:
#endif // _DEBUG

	typedef typename pat_traits_type::iterator_type store_iterator_type;
	typedef typename pat_traits_type::difference_type difference_type;
	typedef typename pat_traits_type::value_type pat_value_type;
	typedef typename pat_traits_type::key_type key_type;

	typedef
		detail::bm_skip_table
		<
			pat_traits_type,
			key_extor_type,
			boost::mpl::if_c
			<
				!!(pat_traits_type::array_size_type::value),
				typename pat_traits_type::array_size_type,
				typename
					boost::mpl::if_c
					<
						sizeof(key_type) == 1,
						boost::mpl::size_t<256>,
						boost::mpl::size_t<0>
					>::type
			>::type::value
		> skip_table_type;

	typedef ::yggr::vector<difference_type> suffix_table_type;

private:
	typedef typename skip_table_type::size_type size_type;

private:
	typedef boyer_moore this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	boyer_moore(pat_iterator_type first, pat_iterator_type last)
		: _pat_first(first), _pat_last(last),
			_k_pattern_length(std::distance(first, last)),
			_skip(_k_pattern_length, difference_type(-1)),
			_suffix(_k_pattern_length + 1)
	{
		assert(std::distance(first, last) >= 0);

		this_type::build_skip_table();
		this_type::build_suffix_table();
	}

	boyer_moore(pat_iterator_type first, pat_iterator_type last, const pat_pred_type& pred)
		: _pat_first(first), _pat_last(last),
			_k_pattern_length(std::distance(first, last)),
			_pat_pred(pred),
			_skip(_k_pattern_length, difference_type(-1)),
			_suffix(_k_pattern_length + 1)
	{
		assert(std::distance(first, last) >= 0);

		this_type::build_skip_table();
		this_type::build_suffix_table();
	}

	boyer_moore(pat_iterator_type first, pat_iterator_type last,
				 const key_extor_type& key_extor)
		: _pat_first(first), _pat_last(last),
			_k_pattern_length(std::distance(first, last)),
			_skip(_k_pattern_length, difference_type(-1), key_extor),
			_suffix(_k_pattern_length + 1)
	{
		assert(std::distance(first, last) >= 0);

		this_type::build_skip_table();
		this_type::build_suffix_table();
	}

	boyer_moore(pat_iterator_type first, pat_iterator_type last,
					const key_extor_type& key_extor, const pat_pred_type& pred)
		: _pat_first(first), _pat_last(last),
			_k_pattern_length(std::distance(first, last)),
			_pat_pred(pred),
			_skip(_k_pattern_length, difference_type(-1), key_extor),
			_suffix(_k_pattern_length + 1)
	{
		assert(std::distance(first, last) >= 0);

		this_type::build_skip_table();
		this_type::build_suffix_table();
	}

	boyer_moore(BOOST_RV_REF(this_type) right)
		: _pat_first(boost::move(right._pat_first)),
			_pat_last(boost::move(right._pat_last)),
			_k_pattern_length(boost::move(right._k_pattern_length)),
			_pat_pred(boost::move(right._pat_pred)),
			_skip(boost::move(right._skip)),
			_suffix(boost::move(right._suffix))
	{
	}

	boyer_moore(const this_type& right)
		: _pat_first(right._pat_first), _pat_last(right._pat_last),
			_k_pattern_length(right._k_pattern_length),
			_pat_pred(right._pat_pred),
			_skip(right._skip),
			_suffix(right._suffix)
	{
	}

	~boyer_moore(void)
	{
	}

public:
	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		this_type& right_chk = right;
		if(this == &right_chk)
		{
			return *this;
		}

		_pat_first = boost::move(right._pat_first);
		_pat_last = boost::move(right._pat_last);
		_k_pattern_length = boost::move(right._k_pattern_length);
		_pat_pred = boost::move(right._pat_pred);

		copy_or_move_or_swap(_skip, boost::move(right._skip));
		copy_or_move_or_swap(_suffix, boost::move(right._suffix));

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
		_k_pattern_length = right._k_pattern_length;
		_pat_pred = right._pat_pred;

		_skip = right._skip;
		_suffix = right._suffix;

		return *this;
	}

public:
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
		::yggr::swap(_k_pattern_length, right._k_pattern_length);
		::yggr::swap(_pat_pred, right._pat_pred);

		_skip.swap(right._skip);
		_suffix.swap(right._suffix);
	}

public:
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
		operator()(CorpusIter corpus_first, CorpusIter corpus_last, Pred pred) const
	{
		typedef std::pair<CorpusIter, CorpusIter> ret_type;

		if(corpus_first == corpus_last)
		{
			return ret_type(corpus_last, corpus_last);
		}
		else if(_pat_first == _pat_last)
		{
			return ret_type(corpus_first, corpus_first);
		}
		else
		{
			difference_type corpus_size = std::distance(corpus_first, corpus_last);
			assert(corpus_size > 0);

			if(corpus_size < _k_pattern_length)
			{
				return ret_type(corpus_last, corpus_last);
			}
			else
			{
				return this_type::do_search(corpus_first, corpus_last, corpus_size, pred);
			}
		}
	}

	inline void reinit(pat_iterator_type first, pat_iterator_type last)
	{
		assert(std::distance(first, last) >= 0);
		_pat_first = first;
		_pat_last = last;
		_k_pattern_length = std::distance(first, last);

		_skip.reinit(_k_pattern_length, difference_type(-1));

		{
			suffix_table_type tmp(_k_pattern_length + 1);
			tmp.swap(_suffix);
		}

		this_type::build_skip_table();
		this_type::build_suffix_table();
	}

	inline void reinit(pat_iterator_type first, pat_iterator_type last, const pat_pred_type& pred)
	{
		assert(std::distance(first, last) >= 0);
		_pat_first = first;
		_pat_last = last;
		_k_pattern_length = std::distance(first, last);
		_pat_pred = pred;

		_skip.reinit(_k_pattern_length, difference_type(-1));

		{
			suffix_table_type tmp(_k_pattern_length + 1);
			tmp.swap(_suffix);
		}

		this_type::build_skip_table();
		this_type::build_suffix_table();
	}

	inline void reinit(pat_iterator_type first, pat_iterator_type last,
				 const key_extor_type& key_extor)
	{
		assert(std::distance(first, last) >= 0);
		_pat_first = first;
		_pat_last = last;
		_k_pattern_length = std::distance(first, last);

		_skip.reinit(_k_pattern_length, difference_type(-1), key_extor);

		{
			suffix_table_type tmp(_k_pattern_length + 1);
			tmp.swap(_suffix);
		}

		this_type::build_skip_table();
		this_type::build_suffix_table();
	}

	inline void reinit(pat_iterator_type first, pat_iterator_type last,
					const key_extor_type& key_extor, const pat_pred_type& pred)
	{
		assert(std::distance(first, last) >= 0);
		_pat_first = first;
		_pat_last = last;
		_k_pattern_length = std::distance(first, last);
		_pat_pred = pred;

		_skip.reinit(_k_pattern_length, difference_type(-1), key_extor);

		{
			suffix_table_type tmp(_k_pattern_length + 1);
			tmp.swap(_suffix);
		}

		this_type::build_skip_table();
		this_type::build_suffix_table();
	}

	inline const key_extor_type& key_extor(void) const
	{
		return _skip.key_extor();
	}

	inline const pat_pred_type& pat_comp(void) const
	{
		return _pat_pred;
	}

#ifdef _DEBUG

	inline const skip_table_type& skip_table(void) const
	{
		return _skip;
	}

	inline const suffix_table_type& suffix_table(void) const
	{
		return _suffix;
	}

#endif // _DEBUG


private:

	template<typename CorpusIter, typename Pred>
	std::pair<CorpusIter, CorpusIter>
		do_search (CorpusIter corpus_first, CorpusIter corpus_last,
					difference_type corpus_size, Pred pred) const
	{
		typedef CorpusIter corpus_iter_type;
		typedef std::pair<corpus_iter_type, corpus_iter_type> ret_type;
		typedef std::reverse_iterator<corpus_iter_type> corpus_riter_type;
		typedef std::reverse_iterator<store_iterator_type> pat_riter_type;

		assert(!(corpus_first == corpus_last));
		assert(std::distance(corpus_first, corpus_last) == corpus_size);

		difference_type cur_idx = 0;
		difference_type last_idx = corpus_size - _k_pattern_length;

		corpus_iter_type cur_pos = corpus_first;
		corpus_iter_type cur_pos_last = cur_pos;
		std::advance(cur_pos_last, _k_pattern_length);
		corpus_riter_type cur_pos_cmp;

		pat_riter_type pat_s(_pat_last);
		pat_riter_type pat_j = pat_s;

		difference_type j = 0, k = 0, m = 0, sfx_j = 0;
		const difference_type* psuffix = &_suffix[0];

		assert(psuffix);

		while(cur_idx <= last_idx)
		{
			j = _k_pattern_length;
			pat_j = pat_s;
			std::advance(cur_pos_last, sfx_j);
			cur_pos_cmp = corpus_riter_type(cur_pos_last);

			while(pred(*cur_pos_cmp, *pat_j))
			{
				--j;
				++cur_pos_cmp;
				++pat_j;
				if(j == 0)
				{
					return ret_type(cur_pos, cur_pos_last);
				}
			}

			k = _skip[*(cur_pos_cmp)];
			m = j - k - 1;
			sfx_j = k < j && m > psuffix[j]? m : psuffix[j];

			cur_idx += sfx_j;
			std::advance(cur_pos, sfx_j);
		}

		return ret_type(corpus_last, corpus_last);
	}

	void build_skip_table(void)
	{
		store_iterator_type first = _pat_first;
		store_iterator_type last = _pat_last;
		for(difference_type i = 0; first != last; ++first, ++i)
		{
			_skip.insert(*first, i);
		}
	}


	template<typename Iter, typename Container>
	void compute_bm_prefix(Iter first, Iter last, Container &prefix)
	{
		difference_type count = static_cast<difference_type>(prefix.size());
		assert(count > 0);
		assert(count == std::distance(first, last));

		prefix[0] = 0;
		difference_type k = 0;

		for(difference_type i = 1; i < count; ++i)
		{
			assert(k < count);
			for(;(k > 0) && (!_pat_pred(*(first[k]), *(first[i])));)
			{
				assert(k < count);
				k = prefix[k - 1];
			}

			if(_pat_pred(*(first[k]), *(first[i])))
			{
				++k;
			}

			prefix[i] = k;
		}
	}

	void build_suffix_table(void)
	{
		typedef ::yggr::vector<store_iterator_type> prefx_datas_type;
		typedef typename prefx_datas_type::iterator prefx_datas_iter_type;
		typedef typename std::reverse_iterator<prefx_datas_iter_type> prefx_datas_riter_type;

		if(!_k_pattern_length)
		{
			return;
		}

		difference_type count = _k_pattern_length;
		store_iterator_type first = _pat_first;
		store_iterator_type last = _pat_last;

		assert(std::distance(first, last) == _k_pattern_length);

		prefx_datas_type prefx_datas(static_cast<size_type>(count), store_iterator_type());
		{
			store_iterator_type s = first;
			for(prefx_datas_iter_type i = prefx_datas.begin(), isize = prefx_datas.end(); i != isize; ++i, ++s)
			{
				*i = s;
			}
		}

		const prefx_datas_type& prefx_datas_cref = prefx_datas;

		suffix_table_type prefix(static_cast<size_type>(count), difference_type(0));
		this_type::compute_bm_prefix(prefx_datas_cref.begin(), prefx_datas_cref.end(), prefix);

		suffix_table_type prefix_reversed(static_cast<size_type>(count), difference_type(0));
		this_type::compute_bm_prefix(prefx_datas_cref.rbegin(), prefx_datas_cref.rend(), prefix_reversed);

		const difference_type* pprefx = &prefix[0];
		const difference_type* pprefix_reversed = &prefix_reversed[0];
		difference_type* psuffix = &_suffix[0];

		{
			for(difference_type i = 0; i <= count; ++i)
			{
				psuffix[i] = count - pprefx[count - 1];
			}
		}

		{
			for(difference_type i = 0; i != count; ++i)
			{
				difference_type j = count - pprefix_reversed[i];
				difference_type k = i - pprefix_reversed[i] + 1;

				if(psuffix[j] > k)
				{
					psuffix[j] = k;
				}
			}
		}
	}

private:
	store_iterator_type _pat_first;
	store_iterator_type _pat_last;
	difference_type _k_pattern_length;
	pat_pred_type _pat_pred;

	skip_table_type _skip;
	suffix_table_type _suffix;
};

// search function
template<typename CorpusIter, typename PatIter> inline
std::pair<CorpusIter, CorpusIter>
	boyer_moore_search(CorpusIter corpus_first, CorpusIter corpus_last,
						PatIter _pat_first, PatIter _pat_last)
{
	typedef boyer_moore<PatIter> bm_type;
	bm_type bm(_pat_first, _pat_last);
	return bm(corpus_first, corpus_last);
}

template<typename CorpusIter, typename CorpusPred, typename PatIter> inline
std::pair<CorpusIter, CorpusIter>
	boyer_moore_search(CorpusIter corpus_first, CorpusIter corpus_last,
						PatIter _pat_first, PatIter _pat_last,
						const CorpusPred& corpus_pred)
{
	typedef boyer_moore<PatIter> bm_type;
	bm_type bm(_pat_first, _pat_last);
	return bm(corpus_first, corpus_last, corpus_pred);
}

template<typename CorpusIter, typename CorpusPred, typename PatIter, typename PatPred> inline
std::pair<CorpusIter, CorpusIter>
	boyer_moore_search(CorpusIter corpus_first, CorpusIter corpus_last,
						PatIter _pat_first, PatIter _pat_last,
						const CorpusPred& corpus_pred,
						const PatPred& pat_pred)
{
	typedef
		boyer_moore
		<
			PatIter,
			detail::bm_traits<PatIter>,
			container::identity<typename detail::bm_traits<PatIter>::value_type>,
			PatPred
		> bm_type;
	bm_type bm(_pat_first, _pat_last, pat_pred);
	return bm(corpus_first, corpus_last, corpus_pred);
}

template<typename PatIter, typename CorpusIter,
			typename Key_Extractor, typename CorpusPred, typename PatPred> inline
std::pair<CorpusIter, CorpusIter>
	boyer_moore_search(CorpusIter corpus_first, CorpusIter corpus_last,
						PatIter _pat_first, PatIter _pat_last,
						const CorpusPred& corpus_pred,
						const Key_Extractor& key_extor,
						const PatPred& pat_pred)
{
	typedef boyer_moore<PatIter, detail::bm_traits<PatIter>, Key_Extractor, PatPred> bm_type;
	bm_type bm(_pat_first, _pat_last, pat_pred);
	return bm(corpus_first, corpus_last, corpus_pred);
}

template<typename CorpusIter,
			typename PatIter, typename PatTraits, typename Key_Extractor, typename PatPred> inline
std::pair<CorpusIter, CorpusIter>
	boyer_moore_search(CorpusIter corpus_first, CorpusIter corpus_last,
						const boyer_moore<PatIter, PatTraits, Key_Extractor, PatPred>& pattern)
{
	return pattern(corpus_first, corpus_last);
}

template<typename CorpusIter, typename CorpusPred,
			typename PatIter, typename PatTraits, typename Key_Extractor, typename PatPred> inline
std::pair<CorpusIter, CorpusIter>
	boyer_moore_search(CorpusIter corpus_first, CorpusIter corpus_last,
						const boyer_moore<PatIter, PatTraits, Key_Extractor, PatPred>& pattern,
						const CorpusPred& corpus_pred)
{
	return pattern(corpus_first, corpus_last, corpus_pred);
}

} // namespace algorithm

using algorithm::boyer_moore;
using algorithm::boyer_moore_search;

} // namespace yggr

namespace yggr
{
namespace algorithm
{
namespace swap_support
{

template<typename PatIter, typename PatTraits, typename Key_Extractor, typename PatPred> inline
void swap(boyer_moore<PatIter, PatTraits, Key_Extractor, PatPred>& l,
			boyer_moore<PatIter, PatTraits, Key_Extractor, PatPred>& r)
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


#endif  //  __YGGR_ALGORITHM_BOYER_MOORE_HPP__

