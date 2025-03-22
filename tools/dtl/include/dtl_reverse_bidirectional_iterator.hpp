//dtl_reverse_bidirectional_iterator.hpp

#ifndef __DTL_REVERSE_BIDIRECTIONAL_ITERATOR_HPP__
#define __DTL_REVERSE_BIDIRECTIONAL_ITERATOR_HPP__

#if defined(_MSC_VER) && _MSC_VER > 1600
#include <iterator>

namespace std
{
		// TEMPLATE CLASS reverse_bidirectional_iterator (retained)
template<class _BidIt,
	class _Ty,
	class _Reference = _Ty&,
	class _Pointer = _Ty *,
	class _Diff = ptrdiff_t>
	class reverse_bidirectional_iterator
		: public iterator<bidirectional_iterator_tag, _Ty, _Diff,
			_Pointer, _Reference>
	{	// wrap bidirectional iterator to run it backwards
public:
	typedef reverse_bidirectional_iterator<_BidIt, _Ty, _Reference,
		_Pointer, _Diff> _Myt;
	typedef _BidIt iterator_type;

	reverse_bidirectional_iterator()
		{	// construct with default wrapped iterator
		}

	explicit reverse_bidirectional_iterator(_BidIt _Right)
		: current(_Right)
		{	// construct wrapped iterator from _Right
		}

	_BidIt base() const
		{	// return wrapped iterator
		return (current);
		}

	_Reference operator*() const
		{	// return designated value
		_BidIt _Tmp = current;
		return (*--_Tmp);
		}

	_Pointer operator->() const
		{	// return pointer to class object
		_Reference _Tmp = **this;
		return (&_Tmp);
		}

	_Myt& operator++()
		{	// preincrement
		--current;
		return (*this);
		}

	_Myt operator++(int)
		{	// postincrement
		_Myt _Tmp = *this;
		--current;
		return (_Tmp);
		}

	_Myt& operator--()
		{	// predecrement
		++current;
		return (*this);
		}

	_Myt operator--(int)
		{	// postdecrement
		_Myt _Tmp = *this;
		++current;
		return (_Tmp);
		}

	bool operator==(const _Myt& _Right) const
		{	// test for iterator equality
		return (current == _Right.current);
		}

	bool operator!=(const _Myt& _Right) const
		{	// test for iterator inequality
		return (!(*this == _Right));
		}

protected:
	_BidIt current;	// the wrapped iterator
	};

} // namespace std

#endif // _MSC_VER

#endif  //__DTL_REVERSE_BIDIRECTIONAL_ITERATOR_HPP__
