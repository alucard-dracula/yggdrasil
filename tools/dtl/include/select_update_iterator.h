// -*- C++ -*-
// iterator used for SELECTS
// nested class of DBView
// Initial: 9/8/2000 - MG
/* Copyright � 2000
Michael Gradman and Corwin Joy

Permission to use, copy, modify, distribute and sell this software and
its documentation for any purpose is hereby granted without fee, provided
that the above copyright notice appears in all copies and that both that
copyright notice and this permission notice appear in supporting documentation.
Corwin Joy and Michael Gradman make no representations about the suitability
of this software for any purpose.
It is provided "as is" without express or implied warranty.
*/
// Edited: 12/19/2000 - MG - added namespaces

#ifndef DTL_SELECT_UPDATE_ITERATOR_H
#define DTL_SELECT_UPDATE_ITERATOR_H

#include "select_iterator.h"

BEGIN_DTL_NAMESPACE

// abstraction for SELECT-UPDATES

// these can be free-functions, so let them be free. it simplifies the class.
// extract the BoundIO's representing the columns that make up the key

// Note the 'inline' avoid linker breakage from multiple instances during compiling
inline BoundIOs ExtractKey(BoundIOs const& indexFields, STD_::vector<tstring> const& keyFields)
{
   // we must do a copy so the user-validations are also copied
   BoundIOs boundIOs(indexFields);
   boundIOs.clear();

   // the key fields are listed in orderOfFields
   // so iterate over those column names and copy them over as BoundIO objects
   for (STD_::vector<tstring>::const_iterator field_it = keyFields.begin(); field_it != keyFields.end(); ++field_it)
   {
      BoundIOs::const_iterator b_it = indexFields.find(*field_it);

      if (b_it == indexFields.end())
      {
         DTL_THROW DBException(_TEXT("DBView::select_update_iterator::ExtractKey()"),
            _TEXT("Unable to find index field's BoundIO \"") + *field_it
            + _TEXT("\" in indexFields!"), NULL, NULL);
      }

      boundIOs[*field_it] = (*b_it).second;
   }
   return boundIOs;
}



template<class DataObj, class ParamObj = DefaultParamObj<DataObj> >
  class DB_replacer
{
private:
	   typedef BCAWrap<DataObj> BCA;

	   BoundIOs bpabios;  // BPA used for where clause in update iterator
	   tstring postfix;   // where clause for update
	   STD_::vector<tstring> orderOfColumns ; // columns in where clause
	   KeyMode useWhichFields;	// do we USE_PK_ONLY or USE_ALL_FIELDS for writing changes to DB
	   const DBView<DataObj, ParamObj> *pview;



public:
	   // exception safe!
	   void Init_bpabios(STD_::vector<tstring> &KeyFields, DataObj & rowbuf)
      {
			// we can update any field whether or not it is in the index
			BoundIOs bpas;
			BCA bca = pview->GetBCA();

			// coerce to USE_ALL_FIELDS if autokey not enabled or supported in view
			if (useWhichFields == USE_AUTO_KEY && pview->GetKey().empty())
			{
				useWhichFields = USE_ALL_FIELDS;
			}

			// get a copy of the proper BoundIOs for where clause in update
			switch (useWhichFields)
			{
			     case USE_PK_FIELDS_ONLY: // get BoundIOs representing PK only
					 {
					   bpas.BindAsBase(rowbuf);
					   bca(bpas, rowbuf);
				      bpas = ExtractKey(bpas, KeyFields);
					   break;
					 }
				 case USE_ALL_FIELDS: // get all BoundIOs
					 {
					   bpas.BindAsBase(rowbuf);
					   bca(bpas, rowbuf);
					   break;
					 }
				 case USE_AUTO_KEY: // get BoundIOs for autokey
					 {
                  BoundIOs tempbios;
                  tempbios.BindAsBase(rowbuf);
                  bca(tempbios, rowbuf);
					   bpas = ExtractKey(tempbios, pview->GetKey());
					   break;
					 }
			}

			tstring QuoteChar = pview->GetConnection().GetQuoteChar();

         bpas.InsUserValidate();
			STD_::pair<tstring, STD_::vector<tstring> > pr =
				MakePostfixForUpdate(bpas, QuoteChar, (const DataObj *) NULL);

			bpabios.swap(bpas);
			postfix.swap(pr.first);
			orderOfColumns.swap(pr.second);
	   }

	   const BoundIOs &Get_bpabios() {
		   return bpabios;
	   }

	DB_replacer() : bpabios(), postfix(), orderOfColumns(),
		useWhichFields(USE_AUTO_KEY), pview(NULL) { }

	DB_replacer(const DBView<DataObj, ParamObj> &view) : bpabios(), postfix(), orderOfColumns(),
		useWhichFields(view.GetKeyMode()),
		pview(&view) { }

	// copy constructor and assignment operator required for Assignable property
	DB_replacer(const DB_replacer<DataObj, ParamObj> &other) :
	    bpabios(other.bpabios), postfix(other.postfix), orderOfColumns(other.orderOfColumns),
		useWhichFields(other.useWhichFields), pview(other.pview) { }


	// make constructable from a select iterator so we can assign begin() or end() to this
    DB_replacer(const	DB_select_iterator<DataObj, ParamObj> &it) :
		bpabios(), postfix(), orderOfColumns(),
		useWhichFields(it.GetView().GetKeyMode()),
		pview(&(it.GetView()))
		{ }

	// exception-safe swap()
	void swap(DB_replacer<DataObj, ParamObj> &other)
	{
		bpabios.swap(other.bpabios);
		STD_::swap(postfix, other.postfix);
		STD_::swap(orderOfColumns, other.orderOfColumns);
		STD_::swap(useWhichFields, other.useWhichFields);
		STD_::swap(pview, other.pview);
	}

	// exception-safe assignment
	DB_replacer<DataObj, ParamObj> &
		operator=(const DB_replacer<DataObj, ParamObj> &other)
	{
		if (this != &other)
		{
			DB_replacer<DataObj, ParamObj> temp(other);
			swap(temp);
		}

		return *this;
	}


	   // attempts to replace the object old_dataObj with
	   // a new set of data as specified by dataObj
 	   void replace(const DataObj &old_dataObj1, const DataObj &dataObj)
	   {
         DataObj old_dataObj(old_dataObj1);

		  // try the update in the DB

		  // TODO: two tricky things need to be done to make replace work efficiently
		  // 1.  BCA should be just the list of changed columns between *it and dataObj
		  //  so that only the minimum set of changes are sent to the database.
		  //  The changed columns should be found by doing comparisons using BoundIOs.
		  //
		  // 2.  A where clause needs to be setup along with a BCA functor to specify
		  //  which rows in the database are to be replaced.  If the mode is
		  //  USE_PK_ONLY         --> WHERE db_field1 = key_field1 AND db_field2 = key_field2 AND...
		  //  USE_ALL_FIELDS      --> WHERE db_field1 = field1 AND ...(use all fields here)
		  //  USE_AUTO_KEY		  --> WHERE db_field = autokey_field1 AND ... (use all autokey fields here)
		  // use pImpl so we can do the work to create the view only if
		  // in bound mode
		  CountedPtr<DBView<DataObj, DataObj> > pNewView = NULL;

        // first, deal with postfix
		  if (postfix == _TEXT("")  && useWhichFields != USE_PK_FIELDS_ONLY)
        {
           // use all fields or autokey if nothing set
			// useWhichFields = USE_ALL_FIELDS;
			STD_::vector<tstring> tmp_fields;

         // note, we pass old_dataObj
			Init_bpabios(tmp_fields,old_dataObj);
		  }

        // also bind the old dataobj, so we have a boundios with NULL state
        // to compare against for ChangedFields
        // I think this is pretty inefficent, we should a better way.
        BoundIOs old_bcabios;
    	  DataObj old_rowbuf(old_dataObj);
		  old_bcabios.BindAsBase(old_rowbuf);
		  BCA old_bca = pview->GetBCA();
         old_bca(old_bcabios, old_rowbuf);
		  old_bcabios.InsUserValidate();


		  // we can update any field whether or not it is in the index
		  BoundIOs bcabios;

        // take a copy of the data obj, allow insuservalidate()
        // to change the contents... TODO this should be used below too?
    	  DataObj rowbuf(dataObj);
		  bcabios.BindAsBase(rowbuf);
		  BCA bca = pview->GetBCA();
	      bca(bcabios, rowbuf);

         // insert-user-validate, so ChangedFields() will return a
         // correct test result.
		  bcabios.InsUserValidate();

		  bcabios = bcabios.ChangedFields(rowbuf, old_bcabios, old_dataObj);

		  // if no fields tied to the BoundIOs changed,
		  // then *it == dataObj as far as the replace() is concerned
		  // so just return

		  if (bcabios.empty())
		  {
			return;
		  }


		  // in order to set the IOHandler we have to convert the indexed view's
		  // IOHandler<DataObj, ParamObj> to IOHandler<DataObj, DataObj>
		  // monomorphize() does that for us
		  pNewView = new DBView<DataObj, DataObj>(MakeDelimitedList(pview->GetTableNames()),
				DynamicBCA<DataObj, BCA>(bcabios, bca),
            postfix,
				DynamicBCA<DataObj, BCA>(bpabios, bca, orderOfColumns),
            pview->GetSelVal(),
            pview->GetInsVal(),
				monomorphize(pview->get_io_handler()),
				pview->GetConnection(),
            MakeDelimitedList(pview->GetKey()),
				useWhichFields);

		  // bPrepare = false as stmt. only executed once
		  typename DBView<DataObj, DataObj>::update_iterator up_it = SPEC_CASE_TYPENAME_IN_TEMPLATE_PARAMS DBView<DataObj, DataObj>::update_iterator(*pNewView, false);

		  up_it.Params(old_dataObj);  // old version of object gives params for replace

		  *up_it = rowbuf; // InsValidate logic performed in proxy assignment

		  ++up_it; // execute update


		  // if iterator encountered a problem, that means deletion failed, so throw
	      if (up_it.bad() || up_it.fail()  || up_it.GetLastCount() == 0)
			  DTL_THROW DBException(_TEXT("DBReplacer::replace(const DataObj &old_dataObj, const DataObj &dataObj)"),
			  _TEXT("Update of row failed! Record may have been changed by another user."),
					NULL, NULL);
	}




	// set key fields to use for update clause
	// ****** note this overrides DBView behavior!!! *****
	void SetKey(const tstring &KeyFields) {
		STD_::vector<tstring> KF = ParseCommaDelimitedList(KeyFields);
		useWhichFields = USE_PK_FIELDS_ONLY;
      DataObj rowbuf;   // no need for a real rowbuf here
		Init_bpabios(KF,rowbuf);
	}

};




template<class DataObj, class ParamObj = DefaultParamObj<DataObj> >
  class DB_select_update_iterator : public DB_replacer<DataObj, ParamObj>,
#ifndef __SGI_STL_PORT
	 // really this is not a forward_iterator, only a input + output iterator
     // forward_iterator is as close as we can get to this STL notion
	 public STD_::iterator<STD_::forward_iterator_tag, DataObj, std::ptrdiff_t>
#else
	 // not sure why STLport not recognizing the standard iterator tag
	 // when used in algorithms like copy(_InputIter __first, _InputIter __last, _OutputIter __result)
	 // use STLport specific alternative
	 public STD_::forward_iterator<DataObj, std::ptrdiff_t>
#endif
{
	   DataObj updated_dataObj; // data object for updates
	   bool    is_dirty;        // flag to indicate if above data object was modified
	   DB_select_iterator<DataObj, ParamObj> sel_it;


	   // constructor needed for BCC 5.5
	   static inline DataObj ConstructDataObj(const DB_select_iterator<DataObj, ParamObj> &it) {
		   if (it.pDBview)
				return it.pDBview->GetDataObj();
		   else {
				DataObj tmp;
				return tmp;
		   }
	   }

public:

	DB_select_update_iterator() :  DB_replacer<DataObj, ParamObj>(),
		updated_dataObj(), is_dirty(false),
		sel_it() { }

	DB_select_update_iterator(const DBView<DataObj, ParamObj> &view) :
		DB_replacer<DataObj, ParamObj>(view),
		updated_dataObj(view.GetDataObj()), is_dirty(false),
		sel_it(view) { }

	// copy constructor and assignment operator required for Assignable property
	DB_select_update_iterator(const DB_select_update_iterator &other) :
		DB_replacer<DataObj, ParamObj>(other),
	    updated_dataObj(other.updated_dataObj), is_dirty(other.is_dirty),
		sel_it(other.sel_it) { }


	/////////////////////////////////////////////////////////
	// compatibility operators for begin(), end()
	//

	// make constructable from a select iterator so we can assign begin() or end() to this
    DB_select_update_iterator(const	DB_select_iterator<DataObj, ParamObj> &it) :
		DB_replacer<DataObj, ParamObj>(it),
		updated_dataObj(ConstructDataObj(it)),
		is_dirty(false), sel_it(it) { }

	// exception-safe swap()
	void swap(DB_select_update_iterator<DataObj, ParamObj> &other)
	{
		DB_replacer<DataObj, ParamObj>::swap(other);
		STD_::swap(updated_dataObj, other.updated_dataObj);
		STD_::swap(is_dirty, other.is_dirty);
		sel_it.swap(other.sel_it);
	}

	// exception-safe assignment
	DB_select_update_iterator<DataObj, ParamObj> &
		operator=(const DB_select_update_iterator<DataObj, ParamObj> &other)
	{
		if (this != &other)
		{
			DB_select_update_iterator<DataObj, ParamObj> temp(other);
			swap(temp);
		}

		return *this;
	}


	// for the following comparisons we are only looking to see if they are
	// conceptually at 'the same place' in the record set
	// thus we may have
    //		iterator a,b;
	//		a = b = view.begin(); *a=5; *b=6;
	//      a == b // true
	//      *a != *b // true
	//
	// this conceptual equivalence only is allowed under the standard 24.1.1
	// also, we can only give conceptual equivalence becuase this is all that
	// the select_iterator gives us.

	// are the iterators pointing to the same place
	// only applies at begin and end of
	// recordset
	bool operator==(const DB_select_iterator<DataObj, ParamObj> &i2) const
	{
		return sel_it == i2;
	}

	// are the iterators pointing to different records?
	// only meaningful if both iterators are at the start or end of the same recordset
	bool operator!=(const DB_select_iterator<DataObj, ParamObj> &i2) const
	{
		return !(*this == i2);
	}

	// are the iterators pointing to the same place
	// only applies at begin and end of
	// recordset
	bool operator==(const DB_select_update_iterator<DataObj, ParamObj> &i2) const
	{
		return sel_it == i2.sel_it;
	}

	// are the iterators pointing to different records?
	// only meaningful if both iterators are at the start or end of the same recordset
	bool operator!=(const DB_select_update_iterator<DataObj, ParamObj> &i2) const
	{
		return !(*this == i2);
	}

	/////////////////////////////////////////////////////////



public:

	// return a proxy
	DB_select_update_iterator<DataObj, ParamObj> &
		operator*()
	{
		return *this;
	}

	// proxy, cache the row data to be modified
	DB_select_update_iterator<DataObj, ParamObj> &
		operator=(const DataObj &data)
	{
		//dtl_assign(data, updated_dataObj);
		dtl_assign_t<DataObj> dtl_assign;
		dtl_assign(data, updated_dataObj);

        // commit on operator=()
		if (sel_it != sel_it.GetView().end())
			this->replace(*sel_it, updated_dataObj);

		is_dirty = true;
		return *this;
	}

	// notice, this operator is non-const!
	operator DataObj() {
		if (!is_dirty)
			return *sel_it;
		else
			return updated_dataObj;
	}

	// not exception safe here -- should discuss
	// logic here is succeed entirely or not advance
	DB_select_update_iterator<DataObj, ParamObj> &operator++()
	{
		++sel_it;
		is_dirty = false;

		return *this;
	}

#ifdef __hpux

  inline void operator++(int)
  {
	++(*this);
  }

#else
	template <class DataObjType, class ItType> class DataObjPtr {
		private:
			DataObjType d;
			ItType *pit;
		public:
			DataObjPtr (const DataObjType &dob, ItType *it)  : d(dob), pit(it) {}
			DataObjPtr &operator*() {return *this;}
			operator DataObjType() {return d;}
			operator ItType() {return *pit;}

			DataObjPtr &
			operator=(const DataObj &data)
			{
				pit->replace(d, data);
				d = data;
				return *this;
			}


	};

	// execute the update for the referenced DataObj and postincrement
	inline DataObjPtr<DataObj, DB_select_update_iterator<DataObj, ParamObj> > operator++(int)
	{
	   DataObjPtr<DataObj, DB_select_update_iterator<DataObj, ParamObj> > d(**this, this);
       ++(*this);
	   return d;
	}
#endif

};

// needed for comparison where select_iterator given first
template<class DataObj, class ParamObj>
   bool operator==(const DB_select_iterator<DataObj, ParamObj> &sel_it, const DB_select_update_iterator<DataObj, ParamObj> &sel_up_it)
{
	return (sel_up_it == sel_it);
}

template<class DataObj, class ParamObj>
   bool operator!=(const DB_select_iterator<DataObj, ParamObj> &sel_it, const DB_select_update_iterator<DataObj, ParamObj> &sel_up_it)
{
	return !(sel_it == sel_up_it);
}


END_DTL_NAMESPACE

#endif

