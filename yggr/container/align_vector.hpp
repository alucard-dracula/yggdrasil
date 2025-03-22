//align_vector.hpp

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

#ifndef __YGGR_CONTAINER_ALIGN_VECTOR_HPP__
#define __YGGR_CONTAINER_ALIGN_VECTOR_HPP__

#include <yggr/base/yggrdef.h>
#include <vector>

#if defined(_MSC_VER) && !(_MSC_VER >1600)

#include <yggr/move/move.hpp>
#include <yggr/mplex/static_assert.hpp>
#include <yggr/container/basic_function_adapter.hpp>

#include <boost/type_traits/is_same.hpp>

namespace yggr
{
namespace container
{

template<typename T, typename Nil = void>
struct align_wrapper
{
public:
	typedef T value_type;
};

} // namespace container
} // namespace yggr

#	define YGGR_ALIGN_WRAPPER( __x__ ) yggr::container::align_wrapper< __x__ >
#	define YGGR_ALIGN_VECTOR( __x__, __alloc__ ) vector< YGGR_ALIGN_WRAPPER( __x__ ), __alloc__ > 

namespace std
{
			// TEMPLATE CLASS vector
template< typename Val, typename Nil, typename _Ax  >
class vector< yggr::container::align_wrapper< Val, Nil >, _Ax >
	: public _Vector_val<Val, _Ax>
	{	// varying size array of values
public:
	BOOST_MPL_ASSERT((boost::is_same<Val, typename _Ax::value_type>));
	typedef Val _Ty;
	typedef vector< yggr::container::align_wrapper< Val, Nil >, _Ax > _Myt;
	typedef _Vector_val<_Ty, _Ax> _Mybase;
	typedef typename _Mybase::_Alty _Alloc;

	typedef _Alloc allocator_type;
	typedef typename _Alloc::size_type size_type;
	typedef typename _Alloc::difference_type difference_type;
	typedef typename _Alloc::pointer pointer;
	typedef typename _Alloc::const_pointer const_pointer;
	typedef typename _Alloc::reference reference;
	typedef typename _Alloc::const_reference const_reference;
	typedef typename _Alloc::value_type value_type;

#	if !defined(_VICONT)
#		define _VICONT(it)	it._Getcont()
#	endif // _VICONT

#	if !defined(_VIPTR)
#		define _VIPTR(it)	(it)._Ptr
#	endif // _VIPTR

	typedef _Vector_iterator<_Mybase> iterator;
	typedef _Vector_const_iterator<_Mybase> const_iterator;

	typedef _STD reverse_iterator<iterator> reverse_iterator;
	typedef _STD reverse_iterator<const_iterator> const_reverse_iterator;

	BOOST_COPYABLE_AND_MOVABLE(_Myt)

	vector()
		: _Mybase()
		{	// construct empty vector
		}

	explicit vector(const _Alloc& _Al)
		: _Mybase(_Al)
		{	// construct empty vector with allocator
		}

	explicit vector(size_type _Count)
		: _Mybase()
		{	// construct from _Count * _Ty()
		resize(_Count);
		}

	vector(size_type _Count, const _Ty& _Val)
		: _Mybase()
		{	// construct from _Count * _Val
		_Construct_n(_Count, _STD addressof(_Val));
		}

	vector(size_type _Count, const _Ty& _Val, const _Alloc& _Al)
		: _Mybase(_Al)
		{	// construct from _Count * _Val, with allocator
		_Construct_n(_Count, _STD addressof(_Val));
		}

	vector(const _Myt& _Right)
		: _Mybase(_Right._Alval)
		{	// construct by copying _Right
		if(_Buy(_Right.size()))
			_TRY_BEGIN
			this->_Mylast = _Ucopy(_Right.begin(), _Right.end(),
				this->_Myfirst);
			_CATCH_ALL
			_Tidy();
			_RERAISE;
			_CATCH_END
		}

	template<class _Iter>
		vector(_Iter _First, _Iter _Last)
		: _Mybase()
		{	// construct from [_First, _Last)
		_Construct(_First, _Last, _Iter_cat(_First));
		}

	template<class _Iter>
		vector(_Iter _First, _Iter _Last, const _Alloc& _Al)
		: _Mybase(_Al)
		{	// construct from [_First, _Last), with allocator
		_Construct(_First, _Last, _Iter_cat(_First));
		}

	template<class _Iter>
		void _Construct(_Iter _Count, _Iter _Val, _Int_iterator_tag)
		{	// initialize with _Count * _Val
		size_type _Size = (size_type)_Count;
		_Ty _Newval = (_Ty)_Val;
		_Construct_n(_Size, _STD addressof(_Newval));
		}

	template<class _Iter>
		void _Construct(_Iter _First,
			_Iter _Last, input_iterator_tag)
		{	// initialize with [_First, _Last), input iterators
		_TRY_BEGIN
		insert(begin(), _First, _Last);
		_CATCH_ALL
		_Tidy();
		_RERAISE;
		_CATCH_END
		}

	void _Construct_n(size_type _Count, const _Ty *_Pval)
		{	// construct from _Count * *_Pval
		if(_Buy(_Count))
			{	// nonzero, fill it
			_TRY_BEGIN
			this->_Mylast = _Ufill(this->_Myfirst, _Count, _Pval);
			_CATCH_ALL
			_Tidy();
			_RERAISE;
			_CATCH_END
			}
		}

	vector(BOOST_RV_REF(_Myt) _Right)
		: _Mybase(_Right._Alval)
		{	// construct by moving _Right
		_Assign_rv(boost::forward<_Myt>(_Right));
		}

	_Myt& operator=(BOOST_RV_REF(_Myt) _Right)
		{	// assign by moving _Right
		_Assign_rv(boost::forward<_Myt>(_Right));
		return (*this);
		}

	void _Assign_rv(BOOST_RV_REF(_Myt) _Right)
		{	// assign by moving _Right
			_Myt& right_chk = _Right;
		if(this == &right_chk)
			;
		else if(get_allocator() != _Right.get_allocator())
			{	// move construct a copy
			clear();
			for (iterator _Next = _Right.begin(); _Next != _Right.end();
				++_Next)
				push_back(boost::forward<_Ty>(*_Next));
			}
		else
			{	// clear this and steal from _Right
			_Tidy();
			this->_Swap_all((_Myt&)_Right);
			this->_Myfirst = _Right._Myfirst;
			this->_Mylast = _Right._Mylast;
			this->_Myend = _Right._Myend;

			_Right._Myfirst = 0;
			_Right._Mylast = 0;
			_Right._Myend = 0;
			}
		}

	void push_back(BOOST_RV_REF(_Ty) _Val)
		{	// insert element at end
		if(_Inside(_STD addressof(_Val)))
			{	// push back an element
			size_type _Idx = _STD addressof(_Val) - this->_Myfirst;
			if(this->_Mylast == this->_Myend)
				_Reserve(1);
			_Orphan_range(this->_Mylast, this->_Mylast);
			_Cons_val(this->_Alval,
				this->_Mylast,
				boost::forward<_Ty>(this->_Myfirst[_Idx]));
			++this->_Mylast;
			}
		else
			{	// push back a non-element
			if(this->_Mylast == this->_Myend)
				_Reserve(1);
			_Orphan_range(this->_Mylast, this->_Mylast);
			_Cons_val(this->_Alval,
				this->_Mylast,
				_STD forward<_Ty>(_Val));
			++this->_Mylast;
			}
		}

	reference emplace_back(BOOST_RV_REF(_Ty) _Val)
		{	// insert element at end
		push_back(boost::forward<_Ty>(_Val));
		return back();
		}

	template<class _Valty>
		reference emplace_back(BOOST_RV_REF(_Valty) _Val)
		{	// insert element at end
		if(this->_Mylast == this->_Myend)
			_Reserve(1);
		_Orphan_range(this->_Mylast, this->_Mylast);
		_Cons_val(this->_Alval,
			this->_Mylast,
			boost::forward<_Valty>(_Val));
		++this->_Mylast;
		return back();
		}

	template<class _Valty>
		iterator insert(const_iterator _Where, BOOST_RV_REF(_Valty) _Val)
		{	// insert _Val at _Where
		return (emplace(_Where, boost::forward<_Valty>(_Val)));
		}

	template<class _Valty>
		iterator emplace(const_iterator _Where, BOOST_RV_REF(_Valty) _Val)
		{	// insert _Val at _Where
		size_type _Off = _VIPTR(_Where) - this->_Myfirst;

 #if _ITERATOR_DEBUG_LEVEL == 2
		if(size() < _Off)
			_DEBUG_ERROR("vector emplace iterator outside range");
 #endif /* _ITERATOR_DEBUG_LEVEL == 2 */

		emplace_back(boost::forward<_Valty>(_Val));
		_STD rotate(begin() + _Off, end() - 1, end());
		return (begin() + _Off);
		}

	inline
	void swap(BOOST_RV_REF(_Myt) _Right)
		{	// exchange contents with movable _Right
		if(this != &_Right)
			{	// swap with emptied container
			clear();
			this->_Swap_all((_Myt&)_Right);
			_Assign_rv(boost::forward<_Myt>(_Right));
			}
		}

	~vector()
		{	// destroy the object
		_Tidy();
		}

	_Myt& operator=(const _Myt& _Right)
		{	// assign _Right
		if(this != &_Right)
			{	// worth doing
			this->_Orphan_all();

			if(_Right.size() == 0)
				clear();	// new sequence empty, erase existing sequence
			else if(_Right.size() <= size())
				{	// enough elements, copy new and destroy old
				pointer _Ptr = _STD _Copy_impl(_Right._Myfirst, _Right._Mylast,
					this->_Myfirst);	// copy new
				_Destroy(_Ptr, this->_Mylast);	// destroy old
				this->_Mylast = this->_Myfirst + _Right.size();
				}
			else if(_Right.size() <= capacity())
				{	// enough room, copy and construct new
				pointer _Ptr = _Right._Myfirst + size();
				_STD _Copy_impl(_Right._Myfirst, _Ptr, this->_Myfirst);
				this->_Mylast = _Ucopy(_Ptr, _Right._Mylast, this->_Mylast);
				}
			else
				{	// not enough room, allocate new array and construct new
				if(this->_Myfirst != 0)
					{	// discard old array
					_Destroy(this->_Myfirst, this->_Mylast);
					this->_Alval.deallocate(this->_Myfirst,
						this->_Myend - this->_Myfirst);
					}
				if(_Buy(_Right.size()))
					this->_Mylast = _Ucopy(_Right._Myfirst, _Right._Mylast,
						this->_Myfirst);
				}
			}
		return (*this);
		}

	void reserve(size_type _Count)
		{	// determine new minimum length of allocated storage
		if(max_size() < _Count)
			_Xlen();	// result too long
		else if(capacity() < _Count)
			{	// not enough room, reallocate
			pointer _Ptr = this->_Alval.allocate(_Count);

			_TRY_BEGIN
			_Umove(this->_Myfirst, this->_Mylast, _Ptr);
			_CATCH_ALL
			this->_Alval.deallocate(_Ptr, _Count);
			_RERAISE;
			_CATCH_END

			size_type _Size = size();
			if(this->_Myfirst != 0)
				{	// destroy and deallocate old array
				_Destroy(this->_Myfirst, this->_Mylast);
				this->_Alval.deallocate(this->_Myfirst,
					this->_Myend - this->_Myfirst);
				}

			this->_Orphan_all();
			this->_Myend = _Ptr + _Count;
			this->_Mylast = _Ptr + _Size;
			this->_Myfirst = _Ptr;
			}
		}

	size_type capacity() const
		{	// return current length of allocated storage
		return (this->_Myend - this->_Myfirst);
		}

	iterator begin()
		{	// return iterator for beginning of mutable sequence
		return (iterator(this->_Myfirst, this));
		}

	const_iterator begin() const
		{	// return iterator for beginning of nonmutable sequence
		return (const_iterator(this->_Myfirst, this));
		}

	iterator end()
		{	// return iterator for end of mutable sequence
		return (iterator(this->_Mylast, this));
		}

	const_iterator end() const
		{	// return iterator for end of nonmutable sequence
		return (const_iterator(this->_Mylast, this));
		}

	iterator _Make_iter(const_iterator _Where) const
		{	// make iterator from const_iterator
		return (iterator(_Where._Ptr, this));
		}

	reverse_iterator rbegin()
		{	// return iterator for beginning of reversed mutable sequence
		return (reverse_iterator(end()));
		}

	const_reverse_iterator rbegin() const
		{	// return iterator for beginning of reversed nonmutable sequence
		return (const_reverse_iterator(end()));
		}

	reverse_iterator rend()
		{	// return iterator for end of reversed mutable sequence
		return (reverse_iterator(begin()));
		}

	const_reverse_iterator rend() const
		{	// return iterator for end of reversed nonmutable sequence
		return (const_reverse_iterator(begin()));
		}

 #if _HAS_CPP0X
	const_iterator cbegin() const
		{	// return iterator for beginning of nonmutable sequence
		return (((const _Myt *)this)->begin());
		}

	const_iterator cend() const
		{	// return iterator for end of nonmutable sequence
		return (((const _Myt *)this)->end());
		}

	const_reverse_iterator crbegin() const
		{	// return iterator for beginning of reversed nonmutable sequence
		return (((const _Myt *)this)->rbegin());
		}

	const_reverse_iterator crend() const
		{	// return iterator for ebd of reversed nonmutable sequence
		return (((const _Myt *)this)->rend());
		}

	void shrink_to_fit()
		{	// reduce capacity
		if(size() < capacity())
			{	// worth shrinking, do it
			_Myt _Tmp(*this);
			swap(_Tmp);
			}
		}
 #endif /* _HAS_CPP0X */

	void resize(size_type _Newsize)
		{	// determine new length, padding with _Ty() elements as needed
		if(_Newsize < size())
			erase(begin() + _Newsize, end());
		else if(size() < _Newsize)
			{	// pad as needed
			_Reserve(_Newsize - size());
			_Uninitialized_default_fill_n(this->_Mylast, _Newsize - size(),
				(_Ty *)0, this->_Alval);
			this->_Mylast += _Newsize - size();
			}
		}

	void resize(size_type _Newsize, const _Ty& _Val)
		{	// determine new length, padding with _Val elements as needed
		if(size() < _Newsize)
			_Insert_n(end(), _Newsize - size(), _Val);
		else if(_Newsize < size())
			erase(begin() + _Newsize, end());
		}

	size_type size() const
		{	// return length of sequence
		return (this->_Mylast - this->_Myfirst);
		}

	size_type max_size() const
		{	// return maximum possible length of sequence
		return (this->_Alval.max_size());
		}

	bool empty() const
		{	// test if sequence is empty
		return (this->_Myfirst == this->_Mylast);
		}

	_Alloc get_allocator() const
		{	// return allocator object for values
		return (this->_Alval);
		}

	const_reference at(size_type _Pos) const
		{	// subscript nonmutable sequence with checking
		if(size() <= _Pos)
			_Xran();
		return (*(this->_Myfirst + _Pos));
		}

	reference at(size_type _Pos)
		{	// subscript mutable sequence with checking
		if(size() <= _Pos)
			_Xran();
		return (*(this->_Myfirst + _Pos));
		}

	const_reference operator[](size_type _Pos) const
		{	// subscript nonmutable sequence
 #if _ITERATOR_DEBUG_LEVEL == 2
		if(size() <= _Pos)
			{	// report error
			_DEBUG_ERROR("vector subscript out of range");
			_SCL_SECURE_OUT_OF_RANGE;
			}

 #elif _ITERATOR_DEBUG_LEVEL == 1
		_SCL_SECURE_VALIDATE_RANGE(_Pos < size());
 #endif /* _ITERATOR_DEBUG_LEVEL */

		return (*(this->_Myfirst + _Pos));
		}

	reference operator[](size_type _Pos)
		{	// subscript mutable sequence
 #if _ITERATOR_DEBUG_LEVEL == 2
		if(size() <= _Pos)
			{	// report error
			_DEBUG_ERROR("vector subscript out of range");
			_SCL_SECURE_OUT_OF_RANGE;
			}

 #elif _ITERATOR_DEBUG_LEVEL == 1
		_SCL_SECURE_VALIDATE_RANGE(_Pos < size());
 #endif /* _ITERATOR_DEBUG_LEVEL */

		return (*(this->_Myfirst + _Pos));
		}

 #if _HAS_CPP0X
	pointer data()
		{	// return address of first element
		return (this->_Myfirst);
		}

	const_pointer data() const
		{	// return address of first element
		return (this->_Myfirst);
		}
 #endif /* _HAS_CPP0X */

	reference front()
		{	// return first element of mutable sequence
		return (*begin());
		}

	const_reference front() const
		{	// return first element of nonmutable sequence
		return (*begin());
		}

	reference back()
		{	// return last element of mutable sequence
		return (*(end() - 1));
		}

	const_reference back() const
		{	// return last element of nonmutable sequence
		return (*(end() - 1));
		}

	void push_back(const _Ty& _Val)
		{	// insert element at end
		if(_Inside(_STD addressof(_Val)))
			{	// push back an element
			size_type _Idx = _STD addressof(_Val) - this->_Myfirst;
			if(this->_Mylast == this->_Myend)
				_Reserve(1);
			_Orphan_range(this->_Mylast, this->_Mylast);
			_Cons_val(this->_Alval,
				this->_Mylast,
				this->_Myfirst[_Idx]);
			++this->_Mylast;
			}
		else
			{	// push back a non-element
			if(this->_Mylast == this->_Myend)
				_Reserve(1);
			_Orphan_range(this->_Mylast, this->_Mylast);
			_Cons_val(this->_Alval,
				this->_Mylast,
				_Val);
			++this->_Mylast;
			}
		}

 #if _ITERATOR_DEBUG_LEVEL == 2
	void pop_back()
		{	// erase element at end
		if(empty())
			_DEBUG_ERROR("vector empty before pop");
		else
			{	// erase last element
			_Orphan_range(this->_Mylast - 1, this->_Mylast);
			_Dest_val(this->_Alval,
				this->_Mylast - 1);
			--this->_Mylast;
			}
		}

 #else /* _ITERATOR_DEBUG_LEVEL == 2 */
	void pop_back()
		{	// erase element at end
		if(!empty())
			{	// erase last element
			_Dest_val(this->_Alval,
				this->_Mylast - 1);
			--this->_Mylast;
			}
		}
 #endif /* _ITERATOR_DEBUG_LEVEL == 2 */

	template<class _Iter>
		void assign(_Iter _First, _Iter _Last)
		{	// assign [_First, _Last)
		_Assign(_First, _Last, _Iter_cat(_First));
		}

	template<class _Iter>
		void _Assign(_Iter _Count, _Iter _Val, _Int_iterator_tag)
		{	// assign _Count * _Val
		_Assign_n((size_type)_Count, (_Ty)_Val);
		}

	template<class _Iter>
		void _Assign(_Iter _First, _Iter _Last, input_iterator_tag)
		{	// assign [_First, _Last), input iterators
		erase(begin(), end());
		insert(begin(), _First, _Last);
		}

	void assign(size_type _Count, const _Ty& _Val)
		{	// assign _Count * _Val
		_Assign_n(_Count, _Val);
		}

	iterator insert(const_iterator _Where, const _Ty& _Val)
		{	// insert _Val at _Where
		size_type _Off = size() == 0 ? 0 : _Where - begin();
		_Insert_n(_Where, (size_type)1, _Val);
		return (begin() + _Off);
		}

	void insert(const_iterator _Where, size_type _Count, const _Ty& _Val)
		{	// insert _Count * _Val at _Where
		_Insert_n(_Where, _Count, _Val);
		}

	template<class _Iter>
		void insert(const_iterator _Where, _Iter _First, _Iter _Last)
		{	// insert [_First, _Last) at _Where
		_Insert(_Where, _First, _Last, _Iter_cat(_First));
		}

	template<class _Iter>
		void _Insert(const_iterator _Where, _Iter _First, _Iter _Last,
			_Int_iterator_tag)
		{	// insert _Count * _Val at _Where
		_Insert_n(_Where, (size_type)_First, (_Ty)_Last);
		}

	template<class _Iter>
		void _Insert(const_iterator _Where, _Iter _First, _Iter _Last,
			input_iterator_tag)
		{	// insert [_First, _Last) at _Where, input iterators
		size_type _Off = _VIPTR(_Where) - this->_Myfirst;

 #if _ITERATOR_DEBUG_LEVEL == 2
		if(size() < _Off)
			_DEBUG_ERROR("vector insert iterator outside range");
 #endif /* _ITERATOR_DEBUG_LEVEL == 2 */

		if(_First != _Last)
			{	// worth doing, gather at end and rotate into place
			size_type _Oldsize = size();

			_TRY_BEGIN
			for (; _First != _Last; ++_First)
				push_back(*_First);	// append

			_CATCH_ALL
			erase(begin() + _Oldsize, end());
			_RERAISE;
			_CATCH_END

			_STD rotate(begin() + _Off, begin() + _Oldsize, end());
			}
		}

	template<class _Iter>
		void _Insert(const_iterator _Where,
			_Iter _First, _Iter _Last, forward_iterator_tag)
		{	// insert [_First, _Last) at _Where, forward iterators
 #if _ITERATOR_DEBUG_LEVEL == 2
		if(_VICONT(_Where) != this
			|| _VIPTR(_Where) < this->_Myfirst
			|| this->_Mylast < _VIPTR(_Where))
			_DEBUG_ERROR("vector insert iterator outside range");
		_DEBUG_RANGE(_First, _Last);
 #endif /* _ITERATOR_DEBUG_LEVEL == 2 */

		size_type _Count = 0;
		_Distance(_First, _Last, _Count);

		if(_Count == 0)
			;
		else if(max_size() - size() < _Count)
			_Xlen();	// result too long
		else if(capacity() < size() + _Count)
			{	// not enough room, reallocate
			size_type _Capacity = _Grow_to(size() + _Count);
			pointer _Newvec = this->_Alval.allocate(_Capacity);
			pointer _Ptr = _Newvec;

			_TRY_BEGIN
			_Ptr = _Umove(this->_Myfirst, _VIPTR(_Where),
				_Newvec);	// copy prefix
			_Ptr = _Ucopy(_First, _Last, _Ptr);	// add new stuff
			_Umove(_VIPTR(_Where), this->_Mylast,
				_Ptr);	// copy suffix
			_CATCH_ALL
			_Destroy(_Newvec, _Ptr);
			this->_Alval.deallocate(_Newvec, _Capacity);
			_RERAISE;
			_CATCH_END

			_Count += size();
			if(this->_Myfirst != 0)
				{	// destroy and deallocate old array
				_Destroy(this->_Myfirst, this->_Mylast);
				this->_Alval.deallocate(this->_Myfirst,
					this->_Myend - this->_Myfirst);
				}

			this->_Orphan_all();
			this->_Myend = _Newvec + _Capacity;
			this->_Mylast = _Newvec + _Count;
			this->_Myfirst = _Newvec;
			}
		else
			{	// new stuff fits, append and rotate into place
			_Ucopy(_First, _Last, this->_Mylast);
			_STD rotate(_VIPTR(_Where), this->_Mylast,
				this->_Mylast + _Count);
			this->_Mylast += _Count;
			_Orphan_range(_VIPTR(_Where), this->_Mylast);
			}
		}

 #if _ITERATOR_DEBUG_LEVEL == 2
	iterator erase(const_iterator _Where)
		{	// erase element at where
		if(_VICONT(_Where) != this
			|| _VIPTR(_Where) < this->_Myfirst
			|| this->_Mylast <= _VIPTR(_Where))
			_DEBUG_ERROR("vector erase iterator outside range");
		_Move(_VIPTR(_Where) + 1, this->_Mylast, _VIPTR(_Where));
		_Destroy(this->_Mylast - 1, this->_Mylast);
		_Orphan_range(_VIPTR(_Where), this->_Mylast);
		--this->_Mylast;
		return (_Make_iter(_Where));
		}

 #else /* _ITERATOR_DEBUG_LEVEL == 2 */
	iterator erase(const_iterator _Where)
		{	// erase element at where
		_Move(_VIPTR(_Where) + 1, this->_Mylast,
			_VIPTR(_Where));
		_Destroy(this->_Mylast - 1, this->_Mylast);
		--this->_Mylast;
		return (_Make_iter(_Where));
		}
 #endif /* _ITERATOR_DEBUG_LEVEL == 2 */

	iterator erase(const_iterator _First_arg,
		const_iterator _Last_arg)
		{	// erase [_First, _Last)
		iterator _First = _Make_iter(_First_arg);
		iterator _Last = _Make_iter(_Last_arg);

		if(_First != _Last)
			{	// worth doing, copy down over hole
 #if _ITERATOR_DEBUG_LEVEL == 2
			if(_Last < _First || _VICONT(_First) != this
				|| _VIPTR(_First) < this->_Myfirst
				|| this->_Mylast < _VIPTR(_Last))
				_DEBUG_ERROR("vector erase iterator outside range");
			pointer _Ptr = _Move(_VIPTR(_Last), this->_Mylast,
				_VIPTR(_First));
			_Orphan_range(_VIPTR(_First), this->_Mylast);

 #else /* _ITERATOR_DEBUG_LEVEL == 2 */
			pointer _Ptr = _Move(_VIPTR(_Last), this->_Mylast,
				_VIPTR(_First));
 #endif /* _ITERATOR_DEBUG_LEVEL == 2 */

			_Destroy(_Ptr, this->_Mylast);
			this->_Mylast = _Ptr;
			}
		return (_Make_iter(_First));
		}

	void clear()
		{	// erase all
		erase(begin(), end());
		}

	void swap(_Myt& _Right)
		{	// exchange contents with _Right
		if(this == &_Right)
			;	// same object, do nothing
		else if(this->_Alval == _Right._Alval)
			{	// same allocator, swap control information
			this->_Swap_all(_Right);
			_STD swap(this->_Myfirst, _Right._Myfirst);
			_STD swap(this->_Mylast, _Right._Mylast);
			_STD swap(this->_Myend, _Right._Myend);
			}
		else
			{	// different allocator, do multiple assigns
			_Myt _Ts = _Move(*this);

			*this = _Move(_Right);
			_Right = _Move(_Ts);
			}
		}

protected:
	void _Assign_n(size_type _Count, const _Ty& _Val)
		{	// assign _Count * _Val
		_Ty _Tmp = _Val;	// in case _Val is in sequence
		erase(begin(), end());
		insert(begin(), _Count, _Tmp);
		}

	bool _Buy(size_type _Capacity)
		{	// allocate array with _Capacity elements
		this->_Myfirst = 0;
		this->_Mylast = 0;
		this->_Myend = 0;

		if(_Capacity == 0)
			return (false);
		else if(max_size() < _Capacity)
			_Xlen();	// result too long
		else
			{	// nonempty array, allocate storage
			this->_Myfirst = this->_Alval.allocate(_Capacity);
			this->_Mylast = this->_Myfirst;
			this->_Myend = this->_Myfirst + _Capacity;
			}
		return (true);
		}

	void _Destroy(pointer _First, pointer _Last)
		{	// destroy [_First, _Last) using allocator
		_Destroy_range(_First, _Last, this->_Alval);
		}

	size_type _Grow_to(size_type _Count) const
		{	// grow by 50% or at least to _Count
		size_type _Capacity = capacity();

		_Capacity = max_size() - _Capacity / 2 < _Capacity
			? 0 : _Capacity + _Capacity / 2;	// try to grow by 50%
		if(_Capacity < _Count)
			_Capacity = _Count;
		return (_Capacity);
		}

	bool _Inside(const _Ty *_Ptr) const
		{	// test if _Ptr points inside vector
		return (_Ptr < this->_Mylast && this->_Myfirst <= _Ptr);
		}

	void _Reserve(size_type _Count)
		{	// ensure room for _Count new elements, grow exponentially
		size_type _Size = size();
		if(max_size() - _Count < _Size)
			_Xlen();
		else if((_Size += _Count) <= capacity())
			;
		else
			reserve(_Grow_to(_Size));
		}

	void _Tidy()
		{	// free all storage
		if(this->_Myfirst != 0)
			{	// something to free, destroy and deallocate it
			this->_Orphan_all();
			_Destroy(this->_Myfirst, this->_Mylast);
			this->_Alval.deallocate(this->_Myfirst,
				this->_Myend - this->_Myfirst);
			}
		this->_Myfirst = 0;
		this->_Mylast = 0;
		this->_Myend = 0;
		}

	template<class _Iter>
		pointer _Ucopy(_Iter _First, _Iter _Last, pointer _Ptr)
		{	// copy initializing [_First, _Last), using allocator
		return (_Uninitialized_copy(_First, _Last,
			_Ptr, this->_Alval));
		}

	template<class _Iter>
		pointer _Umove(_Iter _First, _Iter _Last, pointer _Ptr)
		{	// move initializing [_First, _Last), using allocator
		return (_Uninitialized_move(_First, _Last,
			_Ptr, this->_Alval));
		}

	void _Insert_n(const_iterator _Where,
		size_type _Count, const _Ty& _Val)
		{	// insert _Count * _Val at _Where
 #if _ITERATOR_DEBUG_LEVEL == 2
		if(_VICONT(_Where) != this
			|| _VIPTR(_Where) < this->_Myfirst
			|| this->_Mylast < _VIPTR(_Where))
			_DEBUG_ERROR("vector insert iterator outside range");
 #endif /* _ITERATOR_DEBUG_LEVEL == 2 */

		if(_Count == 0)
			;
		else if(max_size() - size() < _Count)
			_Xlen();	// result too long
		else if(capacity() < size() + _Count)
			{	// not enough room, reallocate
			size_type _Capacity = _Grow_to(size() + _Count);
			pointer _Newvec = this->_Alval.allocate(_Capacity);
			size_type _Whereoff = _VIPTR(_Where) - this->_Myfirst;
			int _Ncopied = 0;

			_TRY_BEGIN
			_Ufill(_Newvec + _Whereoff, _Count,
				_STD addressof(_Val));	// add new stuff
			++_Ncopied;
			_Umove(this->_Myfirst, _VIPTR(_Where),
				_Newvec);	// copy prefix
			++_Ncopied;
			_Umove(_VIPTR(_Where), this->_Mylast,
				_Newvec + (_Whereoff + _Count));	// copy suffix
			_CATCH_ALL
			if(1 < _Ncopied)
				_Destroy(_Newvec, _Newvec + _Whereoff);
			if(0 < _Ncopied)
				_Destroy(_Newvec + _Whereoff, _Newvec + _Whereoff + _Count);
			this->_Alval.deallocate(_Newvec, _Capacity);
			_RERAISE;
			_CATCH_END

			_Count += size();
			if(this->_Myfirst != 0)
				{	// destroy and deallocate old array
				_Destroy(this->_Myfirst, this->_Mylast);
				this->_Alval.deallocate(this->_Myfirst,
					this->_Myend - this->_Myfirst);
				}

			this->_Orphan_all();
			this->_Myend = _Newvec + _Capacity;
			this->_Mylast = _Newvec + _Count;
			this->_Myfirst = _Newvec;
			}
		else if((size_type)(this->_Mylast - _VIPTR(_Where))
			< _Count)
			{	// new stuff spills off end
			_Ty _Tmp = _Val;	// in case _Val is in sequence

			_Umove(_VIPTR(_Where), this->_Mylast,
				_VIPTR(_Where) + _Count);	// copy suffix

			_TRY_BEGIN
			_Ufill(this->_Mylast,
				_Count - (this->_Mylast - _VIPTR(_Where)),
				_STD addressof(_Tmp));	// insert new stuff off end
			_CATCH_ALL
			_Destroy(_VIPTR(_Where) + _Count,
				this->_Mylast + _Count);
			_RERAISE;
			_CATCH_END

			this->_Mylast += _Count;
			_Orphan_range(_VIPTR(_Where), this->_Mylast);
			_STD fill(_VIPTR(_Where), this->_Mylast - _Count,
				_Tmp);	// insert up to old end
			}
		else
			{	// new stuff can all be assigned
			_Ty _Tmp = _Val;	// in case _Val is in sequence

			pointer _Oldend = this->_Mylast;
			this->_Mylast = _Umove(_Oldend - _Count, _Oldend,
				this->_Mylast);	// copy suffix

			_Orphan_range(_VIPTR(_Where), this->_Mylast);
			_STD _Copy_backward(_VIPTR(_Where), _Oldend - _Count,
				_Oldend);	// copy hole
			_STD fill(_VIPTR(_Where),
				_VIPTR(_Where) + _Count, _Tmp);	// insert into hole
			}
		}

	pointer _Ufill(pointer _Ptr, size_type _Count, const _Ty *_Pval)
		{	// copy initializing _Count * _Val, using allocator
		_Uninitialized_fill_n(_Ptr, _Count, _Pval, this->_Alval);
		return (_Ptr + _Count);
		}

	__declspec(noreturn) void _Xlen() const
		{	// report a length_error
		_Xlength_error("vector<T> too long");
		}

	__declspec(noreturn) void _Xran() const
		{	// report an out_of_range error
		_Xout_of_range("invalid vector<T> subscript");
		}

 #if _VECTOR_ORPHAN_RANGE
	void _Orphan_range(pointer _First, pointer _Last) const
		{	// orphan iterators within specified (inclusive) range
		_Lockit _Lock(_LOCK_DEBUG);
		const_iterator **_Pnext = (const_iterator **)this->_Getpfirst();
		if(_Pnext != 0)
			while (*_Pnext != 0)
				if((*_Pnext)->_Ptr < _First || _Last < (*_Pnext)->_Ptr)
					_Pnext = (const_iterator **)(*_Pnext)->_Getpnext();
				else
					{	// orphan the iterator
					(*_Pnext)->_Clrcont();
					*_Pnext = *(const_iterator **)(*_Pnext)->_Getpnext();
					}
		}

 #else /* _VECTOR_ORPHAN_RANGE */
	void _Orphan_range(pointer, pointer) const
		{	// orphan iterators within specified (inclusive) range
		}
 #endif /* _VECTOR_ORPHAN_RANGE */
	};
} // namespace std

#else

#	define YGGR_ALIGN_WRAPPER( __x__ ) __x__
#	define YGGR_ALIGN_VECTOR( __x__, __alloc__ ) vector< __x__, __alloc__ > 

#endif // defined(_MSC_VER) && !(_MSC_VER >1600)

#endif // __YGGR_CONTAINER_ALIGN_VECTOR_HPP__