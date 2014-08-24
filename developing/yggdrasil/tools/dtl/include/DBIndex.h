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
// DBIndexes are used to allow the user to key off of fields in the database
// through the classes he defines
// Initial: 10/4/2000 - MG
// Revised: 11/12/2000 - CJ - upgraded IndexFields to contain only key fields
// Edited: 12/19/2000 - MG - added namespaces

#ifndef DTL_DB_INDEX_H
#define DTL_DB_INDEX_H

#include "DB_Base.h"
#include "DBView.h"
#include "BoundIO.h"
#include "DBException.h"
#include "Callback.h"
#include "vec_multiset.h"

#if defined(__SGI_STL_PORT)
#include "std_warn_off.h"
#include <hash_set>
#include "std_warn_on.h"
#endif

BEGIN_DTL_NAMESPACE

// *** code for underlying containers ***

// indicates whether IdxContainer is hashable or not
struct HASH { };
struct NO_HASH { };

// size representing number of buckets for hash table
enum HashSize { SMALL_HASH_TABLE = 191, MEDIUM_HASH_TABLE = 701,
LARGE_HASH_TABLE = 701, XLARGE_HASH_TABLE = 7019 };

template<class DataObj>
class ImplBase_func
{
public:

  virtual bool eq(const DataObj *key1, const DataObj *key2) const = 0;

  virtual bool lt(const DataObj *key1, const DataObj *key2) const = 0;

  virtual size_t hash(const DataObj *key1) const = 0;

  virtual ImplBase_func *clone() = 0;

  virtual ~ImplBase_func() { }
};

template<class DataObj, class Index>
class Impl_func : public ImplBase_func<DataObj>
{
private:

  Index *data;

public:

  Impl_func(Index *pDBIndex) : data(pDBIndex) { }

  virtual bool eq(const DataObj *key1, const DataObj *key2) const
  {
	return data->eq(key1, key2);
  }

  virtual bool lt(const DataObj *key1, const DataObj *key2) const
  {
	return data->lt(key1, key2);
  }

  virtual size_t hash(const DataObj *key1) const
  {
	return data->hash(key1);
  }


  virtual ImplBase_func<DataObj> *clone()
  {
	ImplBase_func<DataObj> *copy = new Impl_func(data);
	return copy;
  }

  // do NOT delete data here because the index is not owned by this object
  virtual ~Impl_func() { }
};

// functors needed for default container factory
template<class DataObj>
class lt_functor : public STD_::binary_function<const DataObj *,
												const DataObj *,
                                                bool>
{
private:
	ImplBase_func<DataObj> *data;

public:
	lt_functor() : data(NULL) { }

	template<class Index> lt_functor(Index *pDBIndex) :
		data(new Impl_func<DataObj,Index>(pDBIndex)) { }

	lt_functor(const lt_functor &other) :
		data(other.data ? other.data->clone() : NULL) { }

	void swap(lt_functor<DataObj> &lhs, lt_functor<DataObj> &rhs)
    {
		ImplBase_func<DataObj> *temp;
		temp = lhs.data;
		lhs.data = rhs.data;
		rhs.data = temp;
    }

    lt_functor<DataObj> &operator=(const lt_functor<DataObj> &rhs)
    {
		lt_functor<DataObj> temp(rhs);
		swap(temp, *this);
		return *this;
    }

	virtual ~lt_functor() { delete data; }

	bool operator()(const DataObj *key1, const DataObj *key2) const
	{
		return data->lt(key1, key2);
	}

};

template<class DataObj>
class eq_functor : public STD_::binary_function<const DataObj *,
												const DataObj *,
												bool>
{
private:
	ImplBase_func<DataObj> *data;

public:
	bool operator()(const DataObj *key1, const DataObj *key2) const
	{
		return data->eq(key1, key2);
	}

	eq_functor() : data(NULL) { }

	template<class Index> eq_functor(Index *pDBIndex) :
		data(new Impl_func<DataObj, Index>(pDBIndex)) { }

	eq_functor(const eq_functor &other) :
		data(other.data ? other.data->clone() : NULL) { }

	void swap(eq_functor<DataObj> &lhs, eq_functor<DataObj> &rhs)
    {
		ImplBase_func<DataObj> *temp;
		temp = lhs.data;
		lhs.data = rhs.data;
		rhs.data = temp;
    }

    eq_functor<DataObj> &operator=(const eq_functor<DataObj> &rhs)
    {
		eq_functor<DataObj> temp(rhs);
		swap(temp, *this);
		return *this;
    }

	virtual ~eq_functor() { delete data; }
};

template<class DataObj>
class hash_functor : public STD_::unary_function<const DataObj *, size_t>
{
public:
	size_t operator()(const DataObj *key1) const
	{
		return data->hash(key1);
	}

	hash_functor() : data(NULL) { }

	template<class Index> hash_functor(Index *pDBIndex) :
		data(new Impl_func<DataObj, Index>(pDBIndex)) { }

	hash_functor(const hash_functor &other) :
		data(other.data ? other.data->clone() : NULL) { }

	void swap(hash_functor<DataObj> &lhs, hash_functor<DataObj> &rhs)
    {
		ImplBase_func<DataObj> *temp;
		temp = lhs.data;
		lhs.data = rhs.data;
		rhs.data = temp;
    }

    hash_functor<DataObj> &operator=(const hash_functor<DataObj> &rhs)
    {
		hash_functor<DataObj> temp(rhs);
		swap(temp, *this);
		return *this;
    }

	virtual ~hash_functor() { delete data; }

private:
	ImplBase_func<DataObj> *data;
};

template <typename Index>
class ContainerFactory
{
public:

	// only the version that is actually called will be generated
	// based on Index::is_hashable_tag_type
	// calling the wrong function should cause a compile error
  typename Index::index_container_type operator()(Index *pDBIndex, NO_HASH DTL_DUMMY_PARAM_1)
  {
	dtl::lt_functor<TYPENAME_IN_TEMPLATE_PARAMS Index::DataObj>     functor(pDBIndex);
	typename Index::index_container_type    result(functor);

	return result;

  }

  typename Index::index_container_type operator()(Index *pDBIndex, HASH DTL_DUMMY_PARAM_1)
  {
	dtl::hash_functor<TYPENAME_IN_TEMPLATE_PARAMS Index::DataObj>   h_functor(pDBIndex);
	dtl::eq_functor  <TYPENAME_IN_TEMPLATE_PARAMS Index::DataObj>   e_functor(pDBIndex);
	typename Index::index_container_type    result(MEDIUM_HASH_TABLE,
												   h_functor,
												   e_functor);

	return result;
  }

};

// used to allow the user to key off of fields in the database through the classes he defines
template<typename View,
#if defined (DTL_USE_NO_HASH_CONTAINER)
#  ifdef __SUNPRO_CC
         typename IdxContainer,
#  else
         typename IdxContainer = DEFAULT_IDXCONTAINER
            <TYPENAME_IN_TEMPLATE_PARAMS View::DataObject *,
	         lt_functor<TYPENAME_IN_TEMPLATE_PARAMS View::DataObject> >,
#  endif
	     typename HashOrNoHash = NO_HASH>
#else
#  ifdef __SUNPRO_CC
         typename IdxContainer,
#  else
         typename IdxContainer  = DEFAULT_IDXCONTAINER
            <TYPENAME_IN_TEMPLATE_PARAMS View::DataObject *,
	         hash_functor<TYPENAME_IN_TEMPLATE_PARAMS View::DataObject>,
             eq_functor<TYPENAME_IN_TEMPLATE_PARAMS View::DataObject> >,
#  endif
	     typename HashOrNoHash = HASH>
#endif
class DBIndex
{
   public:

	   typedef typename View::DataObject DataObj;
	   typedef typename View::ParamObject ParamObj;
	   typedef BCAWrap<DataObj> BCA;
	   typedef BPAWrap<ParamObj> BPA;

	   typedef IdxContainer index_container_type;

	   typedef HashOrNoHash is_hashable_tag_type;

       // is the index unique or not
       enum Uniqueness { NONUNIQUE_INDEX = 0, UNIQUE_INDEX = 1 };

   private:
	   typedef DBIndex<View, IdxContainer, HashOrNoHash> my_type;
	   typedef ContainerFactory<my_type> container_factory_type;

	   // prohibit assignment?
	   DBIndex<View, IdxContainer, HashOrNoHash> &
		   operator=(const DBIndex<View, IdxContainer, HashOrNoHash> &idx);

	   View *pDBview;						  // referenced view???

	   BCA bca;

	   BoundIOs IndexFields;				  // the parts of the key are stored as BoundIOs

	   STD_::vector<tstring> orderOfFields;		  // which fields to compare on first, second, etc.

	   DataObj rowbuf;					      // used for filling IndexFields with rel. addr.

	   Uniqueness uniqueKey;				  // is this index unique or not


	   // typedef typename IdxContainer::key_compare Compare;
	   // typedef typename IdxContainer::hasher Hash;

   public:

	   // less than function to sort keys ... required for STD_::multiset<DataObj *, lt>
	   bool lt(const DataObj *key1, const DataObj *key2) const
	   {
      // must preserve order of fields to compare on in here
      for (STD_::vector<tstring>::const_iterator
					    order_it = orderOfFields.begin(); order_it != orderOfFields.end();
							order_it++)
      {
			     // grab wanted BoundIO from BoundIOs map, throw if it is not found
				 BoundIOs::const_iterator b_it = IndexFields.find(*order_it);

				 if (b_it == IndexFields.end())
						DTL_THROW DBException(_TEXT("DBIndex::lt()"),
							_TEXT("Unable to find index field ") + *order_it + _TEXT("!"), NULL, NULL);

				 const BoundIO &b = (*b_it).second;
				 int result = b.Compare(key1, key2);

				 if (result != 0)
					return (result < 0);

				 // these two fields is equal if we get here
				 // so continue to next field
			}

			// all fields equal if we get here, return false
			return false;
	   }

	      // used for equality ... would be used in something like hash_STD_::multiset<DataObj *, eq>
		bool eq(const DataObj *key1, const DataObj *key2) const
	   {
      // must preserve order of fields to compare on in here
      for (STD_::vector<tstring>::const_iterator
					    order_it = orderOfFields.begin(); order_it != orderOfFields.end();
							order_it++)
      {
			     // grab wanted BoundIO from BoundIOs map, throw if it is not found
				 BoundIOs::const_iterator b_it = IndexFields.find(*order_it);

				 if (b_it == IndexFields.end())
						DTL_THROW DBException(_TEXT("DBIndex::eq()"),
							_TEXT("Unable to find index field ") + *order_it + _TEXT("!"), NULL, NULL);

				 const BoundIO &b = (*b_it).second;
				 int result = b.Compare(key1, key2);

				 if (result != 0)
					return false;

				 // these two fields is equal if we get here
				 // so continue to next field
			}

			// all fields equal if we get here, return false
			return true;
	   }


	   // hash function to map keys to indexes in the hash table
	   size_t hash(const DataObj *key1) const
	   {
		   size_t result = 0;

		   // At this point, IndexFields should contain only the BoundIOs in the index
		   for (BoundIOs::const_iterator b_it = IndexFields.begin();
					b_it != IndexFields.end(); b_it++)
			{
			    const BoundIO &b = (*b_it).second;
				result += b.Hash(key1);
			}

		   return result;
	   }

   protected:

	   //STD_::multiset<DataObj *, Compare> index_data;    // refers to the data in the underlying view

	   	tstring idxname;
		IdxContainer index_data; // must be constructed last

   public:
	   // note that as multiset::iterator is a constant bidirectional iterator, so is
	   // DBIndex::iterator
	   //typedef typename STD_::multiset<DataObj *, Compare>::iterator const_iterator;
	   typedef typename IdxContainer::const_iterator const_iterator;
	   typedef typename IdxContainer::iterator iterator;
	   typedef typename IdxContainer::allocator_type::reference reference;
	   typedef typename IdxContainer::allocator_type::const_reference const_reference;
	   typedef typename IdxContainer::allocator_type::pointer pointer;
	   typedef typename IdxContainer::allocator_type::const_pointer const_pointer;
	   typedef typename IdxContainer::allocator_type::size_type size_type;
	   typedef typename IdxContainer::allocator_type::difference_type difference_type;

   public:
	   DBIndex() : pDBview(NULL), bca(DefaultBCA<DataObj>()),
		   //IndexFields(BoundIO::BIND_AS_INDICES),
		   IndexFields(),
		   orderOfFields(), rowbuf(), uniqueKey(NONUNIQUE_INDEX), idxname(_TEXT("")),
		   index_data(container_factory_type()(this, is_hashable_tag_type()))
	   {
            	   // Below is a goofy way to construct index_data.
		   // We have to do it this way, however, because the BCC 5.5 Rogue Wave
		   // libraries do not copy the compare function on copy construction or assignment!
		   // Hence we cannot just construct index_data above.
		   container_factory_type cf;
		   IdxContainer temp(cf(this, is_hashable_tag_type()));
		   index_data.swap(temp);


	   }

	   // copy constructor must take care to bind functor to this object
	   DBIndex(const DBIndex<View, IdxContainer, HashOrNoHash> &idx) : pDBview(idx.pDBview), bca(idx.bca),
		   IndexFields(), orderOfFields(idx.orderOfFields),
		   rowbuf(idx.rowbuf), uniqueKey(idx.uniqueKey), idxname(idx.idxname),
		index_data()

	   {
		   // Below is a goofy way to construct index_data.
		   // We have to do it this way, however, because the BCC 5.5 Rogue Wave
		   // libraries do not copy the compare function on copy construction or assignment!
		   // Hence we cannot just construct index_data above.

		   container_factory_type cf;
		   IdxContainer temp(cf(this, is_hashable_tag_type()));
		   index_data.swap(temp);

		   // record base address of rowbuf so we can compute offsets when the bca is called
		   IndexFields.BindAsBase(rowbuf);

		   bca(IndexFields, rowbuf); // generate relative addresses and RTTI for this index

		   BoundIOs tmp(ExtractKey());
		   IndexFields.swap(tmp);
		   IndexFields.BindAsBase(rowbuf);

		   // now must copy over the multiset
		   // should be inexpensive as we're just inserting pointers
           for (typename IdxContainer::const_iterator it = idx.index_data.begin();
		         it != idx.index_data.end(); it++)
			{
				index_data.insert(*it);
			}


	   }

	   // orderOfFields must take on the order specified in fieldNames
	   DBIndex(const tstring &nm, const tstring &fieldNames, const View &view,
		   Uniqueness isUnique) : pDBview(NULL), bca(view.GetBCA()), // IndexFields(BoundIO::BIND_AS_INDICES),
		   IndexFields(), // orderOfFields(ParseCommaDelimitedList(EliminateSpaces(fieldNames))),
		   orderOfFields(), rowbuf(),
		   uniqueKey(isUnique), idxname(EliminateSpaces(nm)), index_data()
	   {
		    // Below is a goofy way to construct index_data.
		    // We have to do it this way, however, because the BCC 5.5 Rogue Wave
		    // libraries do not copy the compare function on copy construction or assignment!
		    // Hence we cannot just construct index_data above.
		    container_factory_type cf;
		    IdxContainer temp(cf(this, is_hashable_tag_type()));
		    index_data.swap(temp);

		    TCHAR *field_cstr = new TCHAR[fieldNames.length() + 1];
		    std_tstrncpy(field_cstr, fieldNames.c_str(), fieldNames.length() + 1);
		    trim(field_cstr);

		    orderOfFields = ParseCommaDelimitedList(field_cstr);

		    delete[] field_cstr;

		    View &view_ref = const_cast<View &>(view);
		    pDBview = &view_ref;

		    // record base address of rowbuf so we can compute offsets when the bca is called
		    IndexFields.BindAsBase(rowbuf);

		    bca(IndexFields, rowbuf);   // generate relative addresses and RTTI for this index

			if (orderOfFields.empty())
				DTL_THROW DBException(_TEXT("DBIndex::DBIndex()"),
				_TEXT("Invalid FieldList \"") +
					fieldNames + _TEXT("\"!  FieldList is empty!"), NULL, NULL);

			STD_::set<tstring> processedFieldNames;
			STD_::vector<tstring> colNames = pDBview->GetColNames();

			for (STD_::vector<tstring>::iterator it = orderOfFields.begin(); it != orderOfFields.end(); it++)
			{
				tstring &fieldName = *it;

				// fieldName may still have leading or trailing spaces so we need
				// to clean it up
				TCHAR *cleanup_field_cstr = new TCHAR[fieldName.length() + 1];

				std_tstrncpy(cleanup_field_cstr, fieldName.c_str(), fieldName.length() + 1);

				trim(cleanup_field_cstr);

				// leading and trailing spaces eliminated, so copy back into fieldName
				fieldName = cleanup_field_cstr;

				delete[] cleanup_field_cstr;

				if (fieldName.length() == 0)
					DTL_THROW DBException(_TEXT("DBIndex::DBIndex()"),
					_TEXT("Invalid FieldList \"") +
					  fieldNames + _TEXT("\"!  Field \"") + fieldName + _TEXT("\" is an empty name!"),
					  NULL, NULL);

				// field name must refer to an existing column in the view
				// if it's not found, throw an exception
				if (STD_::find_if(colNames.begin(), colNames.end(),
					STD_::bind1st(NonCaseSensitiveEq(), fieldName)) == colNames.end())
					DTL_THROW DBException(_TEXT("DBIndex::DBIndex()"),
					_TEXT("Invalid FieldList \"") +
					   fieldNames + _TEXT("\"!  Field \"") + fieldName +
					   _TEXT("\" does not exist in view for tables \"") +
					   MakeDelimitedList(pDBview->GetTableNames()) +
					   _TEXT("\"!"), NULL, NULL);

                // check to see if this fieldName has been processed
				// if so, this is a duplicate and throw an exception
				// otherwise record it
				STD_::pair<STD_::set<tstring>::iterator, bool> pr = processedFieldNames.insert(fieldName);

				if (!(pr.second))
					DTL_THROW DBException(_TEXT("DBIndex::DBIndex()"),
					_TEXT("Invalid FieldList \"") +
					   fieldNames + _TEXT("\"!  Field \"") + fieldName +
					   _TEXT("\" redefined"), NULL, NULL);
			}

			BoundIOs tmp(ExtractKey());
			IndexFields.swap(tmp);
			IndexFields.BindAsBase(rowbuf);
	   }

	   // exception-safe swap()
	   void swap(DBIndex<View, IdxContainer, HashOrNoHash> &other)
	   {
		   bca.swap(other.bca);
		   index_data.swap(other.index_data);
		   IndexFields.swap(other.IndexFields);
		   orderOfFields.swap(other.orderOfFields);
		   STD_::swap(pDBview, other.pDBview);
		   STD_::swap(rowbuf, other.rowbuf);
		   STD_::swap(uniqueKey, other.uniqueKey);
	   }

	   // extract BoundIOs representing the key from the complete list of BoundIO's
	   BoundIOs ExtractKey()
	   {
		   // BoundIOs boundIOs(IndexFields);
		   BoundIOs boundIOs;

		   boundIOs.clear();

		   // the key fields are listed in orderOfFields
		   // so iterate over those column names and copy them over as BoundIO objects
		   for (STD_::vector<tstring>::iterator field_it = orderOfFields.begin(); field_it != orderOfFields.end(); field_it++)
		   {
			   BoundIOs::iterator b_it = IndexFields.find(*field_it);

			   if (b_it == IndexFields.end())
			   {
					DTL_THROW DBException(_TEXT("DBIndex::ExtractKey()"),
						_TEXT("Unable to find index field's BoundIO \"") + *field_it
						+ _TEXT("\" in IndexFields!"), NULL, NULL);
			   }

			   boundIOs[*field_it] = (*b_it).second;
		   }
		   return boundIOs;
	   }


	   // see if inserting dataObj into index keeps the index
	   // satisfying uniqueness constraints

	   // returns pair<iterator, bool> pr
	   // pr.first = if dataObj doesn't violate uniqueness constraints, returns
	   //			 insertion hint for where to insert dataObj into the index's multiset
	   //			 otherwise, returns iterator to first element that violates
	   //			 uniqueness constraints
	   // pr.first = does dataObj satisfy uniqueness constraints for this index?
	   //			 (also returns true if index is nonunique or bCheckUniqueness is true)
	   STD_::pair<iterator, bool> CheckUniqueness(const DataObj &dataObj,
		   bool bCheckUniqueness = true)
	   {
			// ************ new, more efficient implementation ***************

			// if we don't care about uniqueness, just say we'd insert onto end of container
			if (!bCheckUniqueness)
				return STD_::pair<iterator, bool>(end(), true);

			// get range of iterators referring to elements == dataObj
			STD_::pair<iterator, iterator> pr =
					equal_range(dataObj);

			// pr.first: gives us where we would insert dataObj if it satisfies
			// uniqueness constraints
			// pr.second: dataObj satisfies uniqueness constraints if equal_range()
			// returns an empty range or if the index is nonunique,
			// we also return true if bCheckUniqueness is false because
			// the caller doesn't care about uniqueness for dataObj
			return STD_::pair<iterator, bool>(pr.first,
				(pr.first == pr.second) || !IsUnique() || !bCheckUniqueness);
	   }

	   iterator begin()
	   {
		  return index_data.begin();
	   }


	   iterator end()
	   {
		  return index_data.end();
	   }

	   const_iterator begin() const
	   {
		  return index_data.begin();
	   }

	   const_iterator end() const
	   {
		  return index_data.end();
	   }

	   size_t size() const
	   {
		  return index_data.size();
	   }

	   size_t max_size() const
	   {
		  return index_data.max_size();
	   }

	   bool empty() const
	   {
		  return index_data.empty();
	   }

	   // comparison operators
	   bool equals(const my_type& idx) const
       {
		 return this->index_data == idx.index_data;
	   }

#if 0
	   friend bool operator<(const DBIndex<View, IdxContainer, HashOrNoHash> &idx1,
							 const DBIndex<View, IdxContainer, HashOrNoHash> &idx2)
	   {
		  return idx1.index_data < idx2.index_data;

	   }

	   friend bool operator>(const DBIndex<View, IdxContainer, HashOrNoHash> &idx1,
							 const DBIndex<View, IdxContainer, HashOrNoHash> &idx2)
	   {
		  return idx1.index_data > idx2.index_data;
	   }

	   friend bool operator<=(const DBIndex<View, IdxContainer, HashOrNoHash> &idx1,
							  const DBIndex<View, IdxContainer, HashOrNoHash> &idx2)
	   {
		  return idx1.index_data <= idx2.index_data;
	   }

	   friend bool operator>=(const DBIndex<View, IdxContainer, HashOrNoHash> &idx1,
							  const DBIndex<View, IdxContainer, HashOrNoHash> &idx2)
	   {
		  return idx1.index_data >= idx2.index_data;
	   }
#endif


	   // hashed containers in STLport (and possibly other STL implementations)
	   // do not allow insertion with a hint

	   // so disregard the hint for all hashed containers
	   // ideally, we would partially specialize the template method as
	   // template<class View, class IdxContainer> DBIndex<View, IdxContainer, HASH>::insert()
	   // unfortunately, most compilers don't support partial specialization or the
	   // specialization of a method within a template class without wholesale specialization
	   // of the entire class

	   // workaround involves declaring wrapper function overloads differing
	   // only on the HASH/NO_HASH parameter ... only the proper version to call
	   // will get built for any given DBIndex instantiation due to 14.7.1 in the C++ standard
	   // since only the method that is used will get instantiated

	   // NO_HASH version makes use of the hint
	   iterator index_insert(DBIndex<View, IdxContainer, NO_HASH> &idx,
		   iterator hint, const typename View::DataObject &data)
	   {
		    typename View::DataObject &dataRef = (typename View::DataObject &) data;
			typename View::DataObject *const pData = (typename View::DataObject *const) &dataRef;
			return idx.index_data.insert(hint, pData);
	   }

	   // HASH version ignores the hint
	   iterator index_insert(DBIndex<View, IdxContainer, HASH> &idx,
		   iterator hint, const typename View::DataObject &data)
	   {
		    typename View::DataObject &dataRef = (typename View::DataObject &) data;
			typename View::DataObject *const pData = (typename View::DataObject *const) &dataRef;
			return idx.index_data.insert(pData);
	   }
#if 0
     // HashOrNoHash version ignores the hint
	   NOT_FRIEND_ON_HP iterator index_insert(DBIndex<View, IdxContainer, HashOrNoHash> &idx,
		   iterator hint, typename const View::DataObject &data)
	   {
		    typename View::DataObject &dataRef = (typename View::DataObject &) data;
			typename View::DataObject *const pData = (typename View::DataObject *const) &dataRef;
            return idx.index_data.insert(hint, pData);

	   }
#endif

	   // insert the DataObj (actually its address) into the indexed data
	   // exception safe - atomic and consistent - only one mutating operation
	   iterator insert(const DataObj &data)
	   {
		    DataObj &dataRef = (DataObj &) data;
		    DataObj *const pData = (DataObj *const) &dataRef;
		    return index_data.insert(pData);
	   }

	   // insert the DataObj (actually its address) into the indexed data
	   // using the hint passed in as a guide
	   // exception safe - atomic and consistent - only one mutating operation
	   iterator insert(iterator hint, const DataObj &data)
	   {
	     // Unused.
	     // DataObj &dataRef = (DataObj &) data;

			// call to wrapper template function needed
			// to allow for the effect of partial specialization
			// Hashed containers ignore the hint while
			// nonhashed ones use it
			return index_insert(*this, hint, data);
	   }

	   // return an iterator to an element based on its address
	   // (calling find() would do a search based on value rather than the address passed in)
	   // the element will appear once in the list
	   iterator find_by_addr(const DataObj &data)
	   {
		if (empty())
			return end();

		    DataObj &dataRef = (DataObj &) data;
			DataObj *const pData = (DataObj *const) &dataRef;

			// we should be able to just say: index_data.find(pData);
			// however, that statement would find all elements which have the identical
			// key value based on lt(), not the same pointer

			// the element is guaranteed to be in the range returned by equal_range()
			// as the method will find all elements which have the same key as *pData

			STD_::pair<iterator, iterator> pr = equal_range(*pData);

			// now we just need to find which element has the same address as *pData
			for (iterator find_it = pr.first; find_it != pr.second; find_it++)
			{
				// if addresses match, this is the element to return from the list
				if (pData == *find_it)  // *find_it returns a DataObj *
				{
					return find_it;
				}
			}

			// in the odd event the element is not found, return end()
			return end();
	   }

	   // erase the DataObj (actually its address) from the indexed data
	   // (note this differs from the standard form of erase() which
	   // removes all elements with the value passed in)


	   // exception safe - atomic and consistent - only one mutating operation
	   // overall
	   void erase(const DataObj &data)
	   {
		    DataObj &dataRef = (DataObj &) data;
			DataObj *const pData = (DataObj *const) &dataRef;

			// we should be able to just say: index_data.erase(pData);
			// however, that statement would erase all elements which have the identical
			// key value based on lt(), not the same pointer

			// the element is guaranteed to be in the range returned by equal_range()
			// as the method will find all elements which have the same key as *pData
			STD_::pair<iterator, iterator> pr = equal_range(*pData);


			// now we just need to find which element has the same address as *pData
			for (iterator find_it = pr.first; find_it != pr.second; find_it++)
			{
				// if addresses match, this is the element to remove from the list
				if (pData == *find_it)  // *find_it returns a DataObj *
				{
					index_data.erase(find_it);
					break;
				}
			}
	   }

	   // erase by iterator
	   void erase(iterator it)
	   {
			index_data.erase(it);
	   }

	   // clear out whole container
	   void clear()
	   {
			index_data.clear();
	   }

   // ------------- equal_range methods ------------------------------------

     // return an iterator to objects that have the given value
	   // based on a key composed of a single field
	   template<class DataField> STD_::pair<iterator, iterator> equal_range(const DataField &df)
	   {

	   	// it's an error if we use this function on a key that has more than one field
		    if (orderOfFields.size() != 1)
		  	{
			  	tostringstream errstr;

			  	errstr << _TEXT("Expected one key field in index!  Instead found ") <<
						orderOfFields.size() << _TEXT("!") << STD_::ends;

				tstring errstring = errstr.str();

				// errstr.freeze(false);

			  	DTL_THROW DBException(_TEXT("DBIndex::equal_range(const DataField &df)"),
				  	errstring, NULL, NULL);
		 	 	}

		  	// copy field df to key via BoundIO's ... we want orderOfFields[0]
		  	BoundIO &boundIO = IndexFields[orderOfFields[0]];

		  	boundIO.CopyMember(rowbuf, df);

		  	return equal_range(rowbuf);
	   }


	   // return an iterator to objects that have the given key
	  STD_::pair<iterator, iterator> equal_range (const DataObj &key)
	   {
		    DataObj &keyRef = (DataObj &) key;
			  return index_data.equal_range(&keyRef);
	   }


     // ------------- find methods ------------------------------------

     // return an iterator to object that has the given value
	   // based on a key composed of a single field
	   template<class DataField> iterator find(const DataField &df)
	   {

		  // it's an error if we use this function on a key that has more than one field
		  if (orderOfFields.size() != 1)
		  {
			  tostringstream errstr;

			  errstr << _TEXT("Expected one key field in index!  Instead found ") <<
					orderOfFields.size() << _TEXT("!") << STD_::ends;
			  tstring errstring = errstr.str();
			  DTL_THROW DBException(_TEXT("DBIndex::find(const DataField &df)"),
				  errstring, NULL, NULL);
		  }

		  // DataObj key;

		  // copy field df to key via BoundIO's ... we want orderOfFields[0]
		  BoundIO &boundIO = IndexFields[orderOfFields[0]];

		  boundIO.CopyMember(rowbuf, df);

		  return find(rowbuf);
	   }

	   // return an iterator to object that has the given key
	   iterator find(const DataObj &key)
	   {
		    DataObj &keyRef = (DataObj &) key;
			return index_data.find(&keyRef);
	   }


	   // same as the single DataField version, but based on two key fields
	   template<class DataField1, class DataField2> STD_::pair<iterator, iterator>
		   equal_range(const DataField1 &df1, const DataField2 &df2)
	   {
		  // it's an error if we use this function on a key that has less than or
		  // more than two fields
		  if (orderOfFields.size() != 2)
		  {
			  tostringstream errstr;

			  errstr << _TEXT("Expected two key fields in index!  Instead found ") <<
					orderOfFields.size() << _TEXT("!") << STD_::ends;

			  tstring errstring = errstr.str();
			  DTL_THROW DBException(_TEXT("DBIndex::equal_range(const DataField &df)"),
				  errstring, NULL, NULL);
		  }

		  // copy fields df1 and df2 via BoundIO's ... we want to match off of
		  // orderOfFields[0] and orderOfFields[1]
		  BoundIO &df1_boundIO = IndexFields[orderOfFields[0]];
		  df1_boundIO.CopyMember(rowbuf, df1);

		  BoundIO &df2_boundIO = IndexFields[orderOfFields[1]];
		  df2_boundIO.CopyMember(rowbuf, df2);

		  return equal_range(rowbuf);
	   }

	   // same as the single DataField version, but based on three key fields
	   template<class DataField1, class DataField2, class DataField3>
		   STD_::pair<iterator, iterator> equal_range(const DataField1 &df1,
				const DataField2 &df2, const DataField3 &df3)
	   {
		  // it's an error if we use this function on a key that has less than or
		  // more than three fields
		  if (orderOfFields.size() != 3)
		  {
			  tostringstream errstr;

			  errstr << _TEXT("Expected three key fields in index!  Instead found ") <<
					orderOfFields.size() << _TEXT("!") << STD_::ends;

			  tstring errstring = errstr.str();
			  DTL_THROW DBException(_TEXT("DBIndex::equal_range(const DataField &df)"),
				  errstring, NULL, NULL);
		  }

		  // copy fields df1, df2, and df3 via BoundIO's ... we want to match off of
		  // orderOfFields[0], orderOfFields[1], orderOfFields[2]
		  BoundIO &df1_boundIO = IndexFields[orderOfFields[0]];
		  df1_boundIO.CopyMember(rowbuf, df1);

		  BoundIO &df2_boundIO = IndexFields[orderOfFields[1]];
		  df2_boundIO.CopyMember(rowbuf, df2);

		  BoundIO &df3_boundIO = IndexFields[orderOfFields[2]];
		  df3_boundIO.CopyMember(rowbuf, df3);

		  return equal_range(rowbuf);
	   }

	   // same as the single DataField version, but based on four key fields
	   template<class DataField1, class DataField2, class DataField3, class DataField4>
		   STD_::pair<iterator, iterator> equal_range(const DataField1 &df1,
				const DataField2 &df2, const DataField3 &df3, const DataField4 &df4)
	   {
		  // it's an error if we use this function on a key that has less than or
		  // more than three fields
		  if (orderOfFields.size() != 4)
		  {
			  tostringstream errstr;

			  errstr << _TEXT("Expected four key fields in index!  Instead found ") <<
					orderOfFields.size() << _TEXT("!") << STD_::ends;

			  tstring errstring = errstr.str();
			  DTL_THROW DBException(_TEXT("DBIndex::equal_range(const DataField &df)"),
				  errstring, NULL, NULL);
		  }

		  // copy fields df1, df2, df3, and df4 via BoundIO's ... we want to match off of
		  // orderOfFields[0], orderOfFields[1], orderOfFields[2], orderOfFields[3]
		  BoundIO &df1_boundIO = IndexFields[orderOfFields[0]];
		  df1_boundIO.CopyMember(rowbuf, df1);

		  BoundIO &df2_boundIO = IndexFields[orderOfFields[1]];
		  df2_boundIO.CopyMember(rowbuf, df2);

		  BoundIO &df3_boundIO = IndexFields[orderOfFields[2]];
		  df3_boundIO.CopyMember(rowbuf, df3);

		  BoundIO &df4_boundIO = IndexFields[orderOfFields[3]];
		  df4_boundIO.CopyMember(rowbuf, df4);

		  return equal_range(rowbuf);
	   }

   	   // same as the single DataField version, but based on two key fields
	   template<class DataField1, class DataField2> iterator
		  find(const DataField1 &df1, const DataField2 &df2)
	   {
		  // it's an error if we use this function on a key that has less than or
		  // more than two fields
		  if (orderOfFields.size() != 2)
		  {
			  tostringstream errstr;

			  errstr << _TEXT("Expected two key fields in index!  Instead found ") <<
					orderOfFields.size() << _TEXT("!") << STD_::ends;

			  tstring errstring = errstr.str();

			  DTL_THROW DBException(_TEXT("DBIndex::find(const DataField &df)"),
				  errstring, NULL, NULL);
		  }

		  // need to copy construct DataObj?
		  // DataObj key;

		  // copy fields df1 and df2 via BoundIO's ... we want to match off of
		  // orderOfFields[0] and orderOfFields[1]
		  BoundIO &df1_boundIO = IndexFields[orderOfFields[0]];
		  df1_boundIO.CopyMember(rowbuf, df1);

		  BoundIO &df2_boundIO = IndexFields[orderOfFields[1]];
		  df2_boundIO.CopyMember(rowbuf, df2);

		  return find(rowbuf);
	   }

	   // same as the single DataField version, but based on three key fields
	   template<class DataField1, class DataField2, class DataField3> iterator
		  find(const DataField1 &df1, const DataField2 &df2, const DataField3 &df3)
	   {
		  // it's an error if we use this function on a key that has less than or
		  // more than three fields
		  if (orderOfFields.size() != 3)
		  {
			  tostringstream errstr;

			  errstr << _TEXT("Expected three key fields in index!  Instead found ") <<
					orderOfFields.size() << _TEXT("!") << STD_::ends;

			  tstring errstring = errstr.str();

			  DTL_THROW DBException(_TEXT("DBIndex::find(const DataField &df)"),
				  errstring, NULL, NULL);
		  }

		  // need to copy construct DataObj?
		  // DataObj key;

		  // copy fields df1, df2, and df3 via BoundIO's ... we want to match off of
		  // orderOfFields[0], orderOfFields[1], and orderOfFields[2]
		  BoundIO &df1_boundIO = IndexFields[orderOfFields[0]];
		  df1_boundIO.CopyMember(rowbuf, df1);

		  BoundIO &df2_boundIO = IndexFields[orderOfFields[1]];
		  df2_boundIO.CopyMember(rowbuf, df2);

  		  BoundIO &df3_boundIO = IndexFields[orderOfFields[2]];
		  df3_boundIO.CopyMember(rowbuf, df3);

		  return find(rowbuf);
	   }

	   // same as the single DataField version, but based on three key fields
	   template<class DataField1, class DataField2, class DataField3, class DataField4>
		  iterator find(const DataField1 &df1, const DataField2 &df2,
						const DataField3 &df3, const DataField4 &df4)
	   {
		  // it's an error if we use this function on a key that has less than or
		  // more than four fields
		  if (orderOfFields.size() != 4)
		  {
			  tostringstream errstr;

			  errstr << _TEXT("Expected four key fields in index!  Instead found ") <<
					orderOfFields.size() << _TEXT("!") << STD_::ends;

			  tstring errstring = errstr.str();

			  DTL_THROW DBException(_TEXT("DBIndex::find(const DataField &df)"),
				  errstring, NULL, NULL);
		  }

		  // need to copy construct DataObj?
		  // DataObj key;

		  // copy fields df1, df2, df3, and df4 via BoundIO's ... we want to match off of
		  // orderOfFields[0], orderOfFields[1], and orderOfFields[2], and orderOfFields[3]
		  BoundIO &df1_boundIO = IndexFields[orderOfFields[0]];
		  df1_boundIO.CopyMember(rowbuf, df1);

		  BoundIO &df2_boundIO = IndexFields[orderOfFields[1]];
		  df2_boundIO.CopyMember(rowbuf, df2);

  		  BoundIO &df3_boundIO = IndexFields[orderOfFields[2]];
		  df3_boundIO.CopyMember(rowbuf, df3);

		  BoundIO &df4_boundIO = IndexFields[orderOfFields[3]];
		  df4_boundIO.CopyMember(rowbuf, df4);

		  iterator   temp = find(rowbuf);
		  return     temp;
	   }

	   // return whether the key is unique or not
	   bool IsUnique()
	   {
		  return uniqueKey == UNIQUE_INDEX;
	   }


	   BoundIOs &GetIndexFields()
	   {
		  return IndexFields;
	   }

	   STD_::vector<tstring> GetOrderOfFields() const
	   {
		  return orderOfFields;
	   }

	   tstring GetName()
	   {
		  return idxname;
	   }

#ifdef DATAOBJ_PRINTABLE
	   // debugging operator<<() .., DataObj must have an operator<<()
	   friend tostream &operator<<(tostream &o, const DBIndex<View, IdxContainer, HashOrNoHash> &idx)
	   {
		    DBIndex<View, IdxContainer, HashOrNoHash>::const_iterator it;

			for (it = idx.begin(); it != idx.end(); it++)
			{
			   o << **it << STD_::endl;

			}

			o << _TEXT("DBIndex.size() = ") << idx.size() << STD_::endl;
			return o;
	   }
#endif

};

template<typename View, typename IdxContainer, typename HashOrNoHash>
bool operator==(const DBIndex<View, IdxContainer, HashOrNoHash> &idx1,
				const DBIndex<View, IdxContainer, HashOrNoHash> &idx2)
{
  return idx1.equals(idx2);
}

template<typename View, typename IdxContainer, typename HashOrNoHash>
bool operator!=(const DBIndex<View, IdxContainer, HashOrNoHash> &idx1,
				const DBIndex<View, IdxContainer, HashOrNoHash> &idx2)
{
  return !(idx1.equals(idx2));
}


END_DTL_NAMESPACE

#endif
