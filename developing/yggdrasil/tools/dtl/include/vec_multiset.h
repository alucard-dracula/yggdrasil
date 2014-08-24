// -*- C++ -*-
/* Copyright ?2000 
Michael Gradman and Corwin Joy 

Permission to use, copy, modify, distribute and sell this software and 
its documentation for any purpose is hereby granted without fee, provided 
that the above copyright notice appears in all copies and that both that 
copyright notice and this permission notice appear in supporting documentation. 
Corwin Joy and Michael Gradman make no representations about the suitability 
of this software for any purpose. 
It is provided "as is" without express or implied warranty.
*/ 

// the vec_multiset<Key, Pred, A, Container> template is an STL container 
// that emulates a std::multiset<Key, Pred, A>
// using an underlying sequence container, an std::vector<Key, A> by default

// this container optimizes the common case of inserting many elements into the container
// without any lookups followed by many lookups with very few modifications in the container

#ifndef DTL_VEC_MULTISET_H
#define DTL_VEC_MULTISET_H

#include "std_warn_off.h"

#include <algorithm>
#include <vector>
#include <list>
#include <deque>
#include <functional>
#include <exception>
#include <stdexcept>
#include <iostream>
#include <utility>
#include <iterator>
#include "std_warn_on.h"

#include "dtl_config.h"

BEGIN_DTL_NAMESPACE

// *************** vec_multiset structures *********
template<class Key, 
         class Pred = STD_::less<Key>, 
         class A = STD_::allocator<Key>,
         class Container = STD_::vector<Key, A> >
class vec_multiset 
{
public:
	// ************** change log structures ***********

	// these structures record inserts and erases that have occurred in a vec_multiset
	// since it was sorted
	enum ChangeType {CHANGE_INVALID = 0, CHANGE_INSERT, CHANGE_ERASE};

	// this structure serves as an entry in the change log
	struct iter_change {

	  ChangeType type;     // indicates operation was an insert or erase
	  size_t	   index;    // index at which elements were being inserted or erased
	  size_t	   num;      // number of elements inserted or erased
	};
private:
	typedef vec_multiset<Key, Pred, A> my_type;
public:

	// required typedefs
    typedef Key key_type;
    typedef Pred key_compare;
    typedef Key value_type;
    typedef Pred value_compare;
    typedef A allocator_type;
    typedef typename A::size_type size_type;
    typedef typename A::difference_type difference_type;
    typedef typename A::reference reference;
    typedef typename A::const_reference const_reference;
	typedef typename A::pointer pointer;
	typedef typename A::const_pointer const_pointer;

	typedef Container container_type;

	typedef typename container_type::iterator container_iterator;
	typedef typename container_type::const_iterator container_const_iterator;

	// ************** const_iterator definition ************
	class const_iterator :
#if defined(DTL_USE_HP_ITERATOR_TYPES)
	 // not sure why STLport not recognizing the standard iterator tag
	 // when used in algorithms like copy(_InputIter __first, _InputIter __last, _OutputIter __result)
	 // use STLport specific alternative
	 public STD_::bidirectional_iterator<value_type, difference_type>
#else
	 public STD_::iterator<STD_::bidirectional_iterator_tag, value_type, difference_type>
#endif
	{
	protected:
		size_type index; // index of the element in *pContainer we're pointing to
		bool bValid;	 // is this iterator pointing to a valid element
		my_type *pvec_ms; // pointer to the vec_multiset iterating over
		size_t idxVersion; // iterator into change log of ins./erase

	public:

		typedef typename A::reference reference;
		typedef typename A::const_reference const_reference;
		typedef typename A::pointer pointer;
		typedef typename A::const_pointer const_pointer;


		const_iterator() : index(0), bValid(false), pvec_ms(NULL), idxVersion(0)
		{ }
		
		const_iterator(const const_iterator &other) :
		index(other.index), bValid(other.bValid), pvec_ms(other.bValid ? other.pvec_ms : NULL),
			idxVersion(other.idxVersion)
		{
			index_adjust();
		}

		// exception-safe swap and assignment
		virtual void swap(const_iterator &other)
		{
			STD_::swap(index, other.index);
			STD_::swap(bValid, other.bValid);
			STD_::swap(pvec_ms, other.pvec_ms);
			STD_::swap(idxVersion, other.idxVersion);
		}

		const_iterator &operator=(const const_iterator &other)
		{
			if (this != &other)
			{
				const_iterator tmp(other);
				swap(tmp);
			}

			return *this;
		}

		// is this iterator valid?
		bool valid() const
		{
			return bValid;
		}

		virtual ~const_iterator()
		{
			bValid = false;
			index = 0;
		}


		// for use only by vec_multiset
		
		// construct an iterator pointing to (*pContainer)[idx]
		// invalidate if idx out of range

		// allow end() to be a valid iterator
		const_iterator(size_type idx, my_type &vec_ms) :
			index(idx), 
			bValid(true), 
			pvec_ms(&vec_ms),
			idxVersion(vec_ms.changes.size())
		{ 

		}		

		// adjust index as a result of an insert into *pvec_ms
		void insert_adjust(size_type idx, size_type num_positions)
		{
			// if index < idx ... element not shifted ... do nothing
			// if index >= idx ... element shifted by num_positions
			if (index >= idx)
				index += num_positions;

		}

		// adjust index as a result of an erase from *pvec_ms
		void erase_adjust(size_type idx, size_type num_positions)
		{
			// if index < idx ... element not shifted ... do nothing
			// if index >= idx ... element shifted by num_positions
			if (index >= idx && index < idx + num_positions) {// element erased
				invalidate();
				return;
			}

			if (index >= idx)
				index -= num_positions;

		}

		// invalidate this iterator because of an erase
		void invalidate()
		{
			bValid = false;
			
			pvec_ms = NULL;

		}

		// checks the index to makes sure it's in range,
		// if not, the iterator is invalidated
		void check_index()
		{
			if (bValid && pvec_ms != NULL)
			{
				// allow end() to be valid index
            // note: it can't be < 0
				if (index > pvec_ms->size())
					invalidate();
			}

		}

		void index_adjust(void) const
		{
			if (pvec_ms && pvec_ms->bSorted)
			{
			const_iterator *this_mutant = const_cast<const_iterator *>(this);
			typename STD_::vector<iter_change>::iterator it = pvec_ms->changes.begin();
			STD_::advance(it, idxVersion);
			for (; it != pvec_ms->changes.end(); ++it) {
				if (it->type == CHANGE_INSERT)
					this_mutant->insert_adjust(it->index, it->num);
				else
					this_mutant->erase_adjust(it->index, it->num);
				this_mutant->idxVersion++;
			}

			this_mutant->check_index();

			if (!valid())
				DTL_THROW RootException(_TEXT("vec_multiset::index_adjust()"),						
					_TEXT("Unable to dereference invalidated iterator!"));
			}
		}

		const_reference operator*() const
		{
			index_adjust();
			return pvec_ms->vec[index];
		}
		
		const_pointer operator->() const
		{
			return &**this;
		}

		// these will result in a throw on dereference if the iterator is invalid
		const_iterator &operator++()
		{
			index_adjust();
			++index;
			return *this;
		}

		const const_iterator operator++(int)
		{
			const_iterator tmp(*this);
			++(*this);
			return tmp;
		}

		const_iterator &operator--()
		{
			index_adjust();
			--index;	
			return *this;
		}

		const const_iterator operator--(int)
		{
			iterator tmp(*this);
			--(*this);
			return tmp;
		}

		container_const_iterator base() const
		{
			index_adjust();
			container_const_iterator beg = pvec_ms->vec.begin();
			typedef typename container_type::difference_type dt;
			STD_::advance(beg, static_cast<dt>(index));
			return beg;
		}

		bool operator==(const const_iterator &other) const
		{
			index_adjust();
			other.index_adjust();
			return (pvec_ms == other.pvec_ms) &&
				   (index == other.index) &&
				   valid() && other.valid();
		}

		bool operator!=(const const_iterator &other) const
		{
			return !(*this == other);
		}


		friend class vec_multiset<Key, Pred, A>; // make vec_multiset a friend
	};

	// ******************* iterator definition *******************
	class iterator : public const_iterator
	{
#if !defined(__BORLANDC__)
	using const_iterator::index_adjust;
	using const_iterator::pvec_ms;
	using const_iterator::index;
#endif
	protected:
		// for use only by vec_multiset
		// construct an iterator pointing to (*pContainer)[idx]
		iterator(size_type idx, my_type &vec_ms) :
			const_iterator(idx, vec_ms)
		{ }
			
	public:
		iterator() : const_iterator() { }
		
		iterator(const iterator &other) :
			const_iterator(other)
		{ 
			
		}

		// exception-safe swap and assignment
		virtual void swap(iterator &other)
		{
			const_iterator::swap(other);
		}

		iterator &operator=(const iterator &other)
		{
			if (this != &other)
			{
				iterator tmp(other);
				swap(tmp);
			}

			return *this;
		}

		virtual ~iterator() { }


		// dereferencing operators ... these are the only operations
		// that really need to throw
		reference operator*() const
		{
			index_adjust();
			return pvec_ms->vec[index];
		}
				
		pointer operator->() const
		{
			return &**this;
		}

		// these will result in a throw on dereference if the iterator is invalid
		iterator &operator++()
		{
			index_adjust();
			++index;
			return *this;
		}

		const iterator operator++(int)
		{
			iterator tmp(*this);
			++(*this);
			return tmp;
		}

		iterator &operator--()
		{
			index_adjust();
			--index;	
			return *this;
		}

		const iterator operator--(int)
		{
			iterator tmp(*this);
			--(*this);
			return tmp;
		}

		// return an actual container_type::iterator to the element in the
		// underlying container
		container_iterator base() const
		{
			index_adjust();
			container_iterator raw_iter = pvec_ms->vec.begin();
		    typedef typename container_type::difference_type dt;
			STD_::advance(raw_iter, static_cast<dt>(index));
			return raw_iter;
		}

#if 0	
		friend bool operator==(const iterator &i1,
                           const iterator &i2)
		{
      return const_iterator(i1) == const_iterator(i2);
		}

		friend bool operator!=(const iterator &i1, const iterator &i2)
		{
			return !(i1 == i2);
		}

    friend bool operator==(const const_iterator &ci, const iterator &i)
    {
      return ci == const_iterator(i);
    }

    friend bool operator!=(const const_iterator &ci, const iterator &i)
    {
      return !(ci == i);
    }

    friend bool operator==(const iterator &i, const const_iterator &ci)
    {
      return ci == i;
    }

    friend bool operator!=(const iterator &i, const const_iterator &ci)
    {
      return !(i == ci);
    }
#endif

		friend class vec_multiset<Key, Pred, A>; // make the vec_multiset a friend of this class
	};
    
#if defined(__BORLANDC__) || defined(__GNUC__) || defined (__hpux) 
	typedef STD_::reverse_iterator<iterator> reverse_iterator;
	typedef STD_::reverse_iterator<const_iterator> const_reverse_iterator;
#elif defined (__SGI_STL_PORT)
	#if __SGI_STL_PORT <= 0x452 
		typedef STD_::reverse_bidirectional_iterator<iterator, value_type> reverse_iterator;
		typedef STD_::reverse_bidirectional_iterator<const_iterator, value_type> const_reverse_iterator;
	#else
		typedef STD_::reverse_iterator<iterator> reverse_iterator;
		typedef STD_::reverse_iterator<const_iterator> const_reverse_iterator;
	#endif

#elif defined (__SUNPRO_CC)
    typedef STD_::reverse_iterator
           	           <iterator,
		                STD_::bidirectional_iterator_tag,
					    value_type>                 reverse_iterator;
	typedef STD_::reverse_iterator
                       <const_iterator,
		                STD_::bidirectional_iterator_tag,
					    value_type>                 const_reverse_iterator;
// #elif defined (__hpux)
//     typedef _STD_RW::reverse_iterator
//            	       <iterator>                      reverse_iterator;
//     typedef _STD_RW::reverse_iterator
//                        <const_iterator>                const_reverse_iterator;
#else
	// definition of std::reverse_bidirectional_iterator slightly different between compilers
	// just use the common denominator here
	typedef STD_::reverse_bidirectional_iterator<iterator, value_type> reverse_iterator;
	typedef STD_::reverse_bidirectional_iterator<const_iterator, value_type> const_reverse_iterator;
#endif


private:
	container_type vec;		// underlying vector of data which vec_multiset 
							// emulates as a multiset
	Pred pred;				// comparison function used by sorting routine
	bool bSorted;			// is the underlying vector sorted

	// utility function to sort the underlying vector
	void sort()
	{
		// sort the vector only if it's not sorted already
		if (!bSorted && !empty())
		{
			STD_::stable_sort(vec.begin(), vec.end(), pred);
		}
		bSorted = true;
	}
	
	STD_::vector<iter_change> changes;


	// adjusters that are called on insert() and erase()

	// does not record changes until container is sorted
	// as iterators are meaningless until that point

	void insert_adjust(size_type elem_index, size_type num_elems)
	{
		if (bSorted)
		{
			iter_change c = {CHANGE_INSERT, elem_index, num_elems};
			changes.push_back(c);
		}
	}

	// adjusters that are called on insert() and erase()
	void erase_adjust(size_type elem_index, size_type num_elems)
	{
		if (bSorted)
		{
			iter_change c = {CHANGE_ERASE, elem_index, num_elems};
			changes.push_back(c);
		}
	}


public:

	void force_sort()
	{
		sort();
	}


	// constructors
	explicit vec_multiset(const Pred& comp = Pred(), const A& al = A()) :
	   vec(al), pred(comp), bSorted(false), changes() { }

	vec_multiset(const vec_multiset<Key, Pred, A, Container> & x) : 
	   vec(x.vec), pred(x.pred), bSorted(x.bSorted), changes(x.changes) { }

    template<class InputIterator> vec_multiset(InputIterator first, 
		InputIterator last, const Pred& comp = Pred(), const A& al = A()) : 
			vec(al), pred(comp), bSorted(false) , changes()
	{
		vec.insert(vec.end(), first, last); 
			// call instead of vector range constructor as
			// in M$, that form of the constructor takes raw pointers
	}

    const_iterator begin() const
	{	
		my_type *this_ptr = const_cast<my_type *>(this);
		this_ptr->sort();
		return const_iterator(0, *this_ptr);
	}

	const_iterator end() const
	{
		my_type *this_ptr = const_cast<my_type *>(this);
		this_ptr->sort();
		return const_iterator(size(), *this_ptr);
	}

	iterator begin()
	{
		sort();
		return iterator(0, *this);
	}

	iterator end()
	{
		sort();
		return iterator(size(), *this);
	}

    const_reverse_iterator rbegin() const
	{
		return const_reverse_iterator(end());
	}

    const_reverse_iterator rend() const
	{
		return const_reverse_iterator(begin());
	}

	reverse_iterator rbegin()
	{
		return reverse_iterator(end());
	}

	reverse_iterator rend()
	{
		return reverse_iterator(begin());
	}

    size_type size() const
	{
		return vec.size();
	}

    size_type max_size() const
	{
		return vec.max_size();
	}

    bool empty() const
	{
		return vec.empty();
	}

    A get_allocator() const
	{
		return vec.get_allocator();
	}

	// must use vector::insert() in order to get the iterator to return,
	// but insert at end() of vector as it's the most efficient
	// (like vector::push_back())
    iterator insert(const value_type& x)
	{
		// if not sorted, just push back onto end of container
		container_iterator result;

		if (!bSorted)
		{
			result = vec.insert(vec.end(), x);
		}
		else  // find proper place to insert and place there
			  // insert into last possible place where the element would belong
			  // so make call to upper_bound() :P
		{
			result = vec.insert(upper_bound(x).base(), x);			
		}
		
		size_type elem_index = static_cast<size_type>(std_distance(vec.begin(), result));

		// update change log
		insert_adjust(elem_index, 1);

		// return newly constructed iterator to this element
		return iterator(elem_index, *this);
	}

	// ignore iterator passed in as inserting at end() still more efficient
	// and we sort anyway on a lookup
    iterator insert(iterator DTL_DUMMY_PARAM_1, const value_type& x)
	{
		return insert(x);
	}

    template<class InputIterator> 
    void insert(InputIterator first, InputIterator last)
	{
		// if not sorted, just push back onto end of container
		// using range insert for efficiency
		if (!bSorted)
		{
			size_type old_size = size();

			vec.insert(vec.end(), first, last);
			size_type num_elems = static_cast<size_type>(std_distance(first, last));
			size_type elem_index = static_cast<size_type>(old_size);

			// record change
			insert_adjust(elem_index, num_elems);
		}
		else  // if sorted, we've got no choice but to insert the elements one by one
		{
			while (first != last)
					insert(*first++); // the forwarded call will record the appropriate
									  // change log entry
		}
	}

    iterator erase(iterator it)
	{
		// we have a no throw guarantee to enforce here
		// so return end() if any iterator invalid
		if (!it.valid())
			return end();

		container_iterator result = 
			vec.erase(it.base()); // if vector sorted, it's still sorted on erase

		size_type elem_index = static_cast<size_type>(std_distance(vec.begin(), result));

		// update the iterator registry
		erase_adjust(elem_index, 1);

		// even if underlying container empty, MUST KEEP bSorted flag set.
		// otherwise, any old iterators against the container will become invalid / give wrong information

		return iterator(elem_index, *this);
	}

    iterator erase(iterator first, iterator last)
	{
		if (!first.valid() || !last.valid())
				return end();

		size_type num_elems = static_cast<size_type>(std_distance(first, last));

		container_iterator result =
			vec.erase(first.base(), last.base()); // if vector sorted, it's still 
												  // sorted on erase

		size_type elem_index = static_cast<size_type>(std_distance(vec.begin(), result));
		
		// update change log
		erase_adjust(elem_index, num_elems);

		// even if underlying container empty, MUST KEEP bSorted flag set.
		// otherwise, any old iterators against the container will become invalid / give wrong information


		return iterator(elem_index, *this);
	}

	// erase all elements with the given Key
    size_type erase(const Key& key)
	{
		// find elements to erase using equal_range
		STD_::pair<iterator, iterator> pr = equal_range(key);

		// if no elements to erase, do nothing and return 0
		if (pr.first == pr.second)
			return static_cast<size_type>(0);

		// figure out how many elements we're erasing for our return value
		size_type elements_erased = static_cast<size_type>(
			std_distance(pr.first, pr.second)
		);

		erase(pr.first, pr.second); // change log updated by forwarded call

		return elements_erased;
	}
    
	void clear()
	{
		vec.clear(); // if vector sorted, it's still sorted on clear
	
		// even if underlying container empty, MUST KEEP bSorted flag set.
		// otherwise, any old iterators against the container will become invalid / give wrong information

	}
    
	// exception-safe swap and assignment
	void swap(vec_multiset<Key, Pred, A, Container> &x)
	{
		vec.swap(x.vec);
		STD_::swap(pred, x.pred);
		STD_::swap(bSorted, x.bSorted);
	}

	my_type &operator=(const my_type &other)
	{
		if (this != &other)
		{
			my_type tmp(other);
			swap(tmp);
		}

		return *this;
	}

    key_compare key_comp() const
	{
		return pred;
	}

    value_compare value_comp() const
	{
		return pred;
	}

    iterator find(const Key& key)
	{
		sort();
		
		// careful here, need to use check based on pred!
		// so use equal_range() and return end() if no elements found
		// otherwise return iterator to first element in that range
		container_iterator vbegin = vec.begin();

		container_iterator lower = STD_::lower_bound(vbegin, vec.end(), key, pred);
		
		if (lower != vec.end() && !pred(key, *lower))
		{	
		   size_type elem_index =
				static_cast<size_type>(std_distance(vbegin, lower));

		   return iterator(elem_index, *this);
		}

		return end();
	}

    const_iterator find(const Key& key) const
	{
		my_type *this_ptr = const_cast<my_type *>(this);
		this_ptr->sort();
		
		// careful here, need to use check based on pred!
		// so use equal_range() and return end() if no elements found
		// otherwise return iterator to first element in that range
		container_const_iterator vbegin = vec.begin();

		container_const_iterator lower = STD_::lower_bound(vbegin, vec.end(), key, pred);
		
		if (lower != vec.end() && !pred(key, *lower))
		{	
		   size_type elem_index =
				static_cast<size_type>(std_distance(vbegin, lower));

		   return const_iterator(elem_index, *this_ptr);
		}

		return end();
	}
  
    size_type count(const Key& key) const
	{
		const_cast<my_type *>(this)->sort();

		// careful here, need to use check based on pred!
		STD_::pair<const_iterator, const_iterator> pr = equal_range(key);

		return static_cast<size_type>(std_distance(pr.first, pr.second));
	}

    const_iterator lower_bound(const Key& key) const
	{
		my_type *this_ptr = const_cast<my_type *>(this);
		
		this_ptr->sort();

		container_const_iterator result = 
				STD_::lower_bound(vec.begin(), vec.end(), key, pred);

		size_type elem_index =
			static_cast<size_type>(std_distance(vec.begin(), result));

		return const_iterator(elem_index, *this_ptr);
	}

	iterator lower_bound(const Key& key)
	{
		sort();

		container_iterator result = 
				STD_::lower_bound(vec.begin(), vec.end(), key, pred);

		size_type elem_index =
			static_cast<size_type>(std_distance(vec.begin(), result));

		return iterator(elem_index, *this);
	}

    const_iterator upper_bound(const Key& key) const
	{
		my_type *this_ptr = const_cast<my_type *>(this);
		
		this_ptr->sort();

		container_const_iterator result = 
				STD_::upper_bound(vec.begin(), vec.end(), key, pred);

		size_type elem_index =
			static_cast<size_type>(std_distance(vec.begin(), result));

		return const_iterator(elem_index, *this_ptr);
	}

	iterator upper_bound(const Key& key)
	{
		sort();
		
		container_iterator result = 
				STD_::upper_bound(vec.begin(), vec.end(), key, pred);

		size_type elem_index =
			static_cast<size_type>(std_distance(vec.begin(), result));

		return iterator(elem_index, *this);
	}
    
	STD_::pair<const_iterator, const_iterator>
        equal_range(const Key& key) const
	{
		my_type *this_ptr = const_cast<my_type *>(this);
		
		this_ptr->sort();
			
		STD_::pair<container_const_iterator, container_const_iterator> 
				result(STD_::equal_range(vec.begin(), vec.end(), key, pred));

		container_const_iterator vbegin = vec.begin();
		
		size_type elem_index1 =
			static_cast<size_type>(std_distance(vbegin, result.first));
		size_type elem_index2 =
			static_cast<size_type>(std_distance(vbegin, result.second));

		return STD_::pair<const_iterator, const_iterator>(const_iterator(elem_index1, *this_ptr),
			const_iterator(elem_index2, *this_ptr));

	}

	STD_::pair<iterator, iterator>
        equal_range(const Key& key) 
	{
		sort();
			
		STD_::pair<container_iterator, container_iterator> 
				result(STD_::equal_range(vec.begin(), vec.end(), key, pred));

		container_iterator vbegin = vec.begin();
		
		size_type elem_index1 =
			static_cast<size_type>(std_distance(vbegin, result.first));
		size_type elem_index2 =
			static_cast<size_type>(std_distance(vbegin, result.second));

		return STD_::pair<iterator, iterator>(iterator(elem_index1, *this),
			iterator(elem_index2, *this));

	}

	
	// comparison operators
	bool equals (const vec_multiset<Key, Pred, A, Container> &vec_ms) const
	{
	  vec_multiset<Key, Pred, A, Container>*  this_ptr  =
	    const_cast<vec_multiset<Key, Pred, A, Container>* >(this);
	  vec_multiset<Key, Pred, A, Container>&  that_sort =
	    const_cast<vec_multiset<Key, Pred, A, Container>& >(vec_ms);

	  this_ptr->sort();
	  that_sort.sort();

	  return this_ptr->vec == that_sort.vec;
	}

	bool lessThan (const vec_multiset<Key, Pred, A, Container> &vec_ms) const
	{
	  vec_multiset<Key, Pred, A, Container>*  this_ptr  =
	    const_cast<vec_multiset<Key, Pred, A, Container>* >(this);
	  vec_multiset<Key, Pred, A, Container>&  that_sort =
	    const_cast<vec_multiset<Key, Pred, A, Container>& >(vec_ms);

	  this_ptr->sort();
	  that_sort.sort();

	  return this_ptr->vec < that_sort.vec;
	}

	bool moreThan (const vec_multiset<Key, Pred, A, Container> &vec_ms) const
	{
	  vec_multiset<Key, Pred, A, Container>*  this_ptr  =
	    const_cast<vec_multiset<Key, Pred, A, Container>* >(this);
	  vec_multiset<Key, Pred, A, Container>&  that_sort =
	    const_cast<vec_multiset<Key, Pred, A, Container>& >(vec_ms);

	  this_ptr->sort();
	  that_sort.sort();

	  return this_ptr->vec > that_sort.vec;
	}

	// declare our iterator classes as friends
	friend class iterator;
	friend class const_iterator;

}; // end class vec_multiset


// comparison functions moved out of body of class
// Note - default template parameters are not allowed by HP aCC compiler for
// functions so several versions of each function ate provided  
 
template<class Key, class Pred, class A, class Container>
bool operator==(const vec_multiset<Key, Pred, A, Container> &vec1,
				const vec_multiset<Key, Pred, A, Container> &vec2)
{
  return vec1.equals(vec2);
}

template<class Key, class Pred, class A, class Container>
bool operator!=(const vec_multiset<Key, Pred, A, Container> &vec1,
				const vec_multiset<Key, Pred, A, Container> &vec2)
{
  return !(vec1.equals(vec2));
}

template<class Key, class Pred, class A, class Container>
bool operator<(const vec_multiset<Key, Pred, A, Container> &vec1,
			   const vec_multiset<Key, Pred, A, Container> &vec2)
{
  return vec1.lessThan(vec2);
}

template<class Key, class Pred, class A, class Container>
bool operator>=(const vec_multiset<Key, Pred, A, Container> &vec1,
				const vec_multiset<Key, Pred, A, Container> &vec2)
{
  return !(vec1.lessThan(vec2));
}

template<class Key, class Pred, class A, class Container>
bool operator>(const vec_multiset<Key, Pred, A, Container> &vec1,
			   const vec_multiset<Key, Pred, A, Container> &vec2)
{
  return vec1.moreThan(vec2);
}

template<class Key, class Pred, class A, class Container>
bool operator<=(const vec_multiset<Key, Pred, A, Container> &vec1,
				const vec_multiset<Key, Pred, A, Container> &vec2)
{
  return !(vec1.moreThan(vec2));
}

END_DTL_NAMESPACE
#endif
