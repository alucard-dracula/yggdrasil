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
// Code to bind a dynamic row
// Initial: 11/18/2000 - CJ
// Edited: 12/19/2000 - MG - added namespaces
// Edited: 03/24/2004 - Alexander Motzkau, Optimized DefaultSelValidate<variant_row>

#ifndef DTL_DYNADBVIEW_H
#define DTL_DYNADBVIEW_H

#include "DB_Base.h"
#include "BoundIO.h"
#include "DBDefaults.h"
#include "DBView.h"
// #include "select_iterator.h"
// #include "insert_iterator.h"
// #include "update_iterator.h"
// #include "delete_iterator.h"
#include "IndexedDBView.h"
#include "variant_row.h"

BEGIN_DTL_NAMESPACE

// forward declaration, these are implemented in bind_basics.cpp
variant_row_fields GetFieldInfo(const tstring &TableName, tstring &TableFields, const tstring &Postfix,
								tstring &UniqueFields,
								STD_::vector<tstring> &AppendedFields,
								KeyMode keyMode,
								DBConnection &conn, HSTMT *p_have_stmt = NULL);
bool OnlyOneTable (tstring TableName);

class DynamicRowBCA
{
  private:
	  variant_row_fields fields;
	  STD_::vector<tstring> appendedFieldNames; 
						// fields appended by GetFieldInfo() for the key
	  KeyMode km;
	  bool bBindParams;
  public:
	  DynamicRowBCA() : fields(), appendedFieldNames(), km(USE_ALL_FIELDS), bBindParams(false) { }
	  DynamicRowBCA(const variant_row_fields &f, KeyMode mode = USE_AUTO_KEY, 
	  const STD_::vector<tstring> &appended = 
	  STD_::vector<tstring>()) : 
	  fields(f), appendedFieldNames(appended), 
	  km(mode), bBindParams(false) 
	  {}

	  // should be exception-safe as synthesized copy constructor used for fields
	  DynamicRowBCA(const DynamicRowBCA &bca) :
		fields(bca.fields), appendedFieldNames(bca.appendedFieldNames),
		km(bca.km), bBindParams(bca.bBindParams)
		{ }

	  DynamicRowBCA(const DynamicRowBCA &bca, bool BindParams) :
		fields(bca.fields), appendedFieldNames(bca.appendedFieldNames),
		km(bca.km), bBindParams(BindParams)
		{ 
			if (!bBindParams)
				return;

			// correct fields to only hold parameters
			variant_row_fields param_fields;
			int idx = 0;
			for (STD_::vector<tstring>::iterator i = fields.names.begin(); i != fields.names.end(); i++, idx++)
			{
					if (fields.types[idx].IsParam()) 
						param_fields.push_back(fields.types[idx], *i);
			}
			fields = param_fields;
		}
	  // exception-safe swap()
	  void swap(DynamicRowBCA &other)
	  {
		fields.swap(other.fields);
		appendedFieldNames.swap(other.appendedFieldNames);
		STD_::swap(km, other.km);
		STD_::swap(bBindParams, other.bBindParams);
	  }

	  // exception-safe assignment
	  DynamicRowBCA &operator=(const DynamicRowBCA &other)
	  {
		  if (this != &other)
		  {
			 DynamicRowBCA temp(other);
			 swap(temp);
		  }

		  return *this;
	  }

	  // bca applied here
	  void operator()(BoundIOs &boundIOs, variant_row &rowbuf)
	  {
		  // initialize progenitor row class
		  // all other rows must be assign constructed or copy constructed from this row
		  // to have the correct size & types

		  // only put the fields we need in the variant object
		  variant_row_fields filtered;
	
		  if (!rowbuf.IsInit()) {
			  int idx = 0;
			  for (STD_::vector<tstring>::iterator i = fields.names.begin(); i != fields.names.end(); i++, idx++)
			  {
				  TypeTranslation tt = fields.types[idx];
				  if (bBindParams) 
				  {
					if (fields.types[idx].IsParam()) 
					{
						filtered.push_back(tt, *i);
					}
				  }
				  else
				  {
					if (!fields.types[idx].IsParam()) 
					{
						filtered.push_back(tt, *i);
					}
				  }
			  }

			  variant_row row(filtered.types, filtered.names);
			  rowbuf = row;
		  }
		  else
		  {
			  filtered = rowbuf.GetVariantRowFields();
		  }

		  
		 
		  // boundIOs.clear();
		  boundIOs.BindAsBase(rowbuf.p_data, filtered.row_size, DTL_TYPEID_NAME (rowbuf));

	
		  // create boundIOs with full data
		  int param_no = 0;
		  int idx = 0;
		  for (STD_::vector<tstring>::iterator i = filtered.names.begin(); i != filtered.names.end(); i++, idx++)
		  {

				// Create full boundIO versus rowbuf
			    BoundIO boundIO(boundIOs, *i, filtered.types[idx], rowbuf.p_data + filtered.offsets[idx], rowbuf.p_data, filtered.row_size);
				boundIO.SetAsVariantRow(idx);
				TypeTranslation tt = filtered.types[idx];
				if (bBindParams) {
					if (filtered.types[idx].IsParam()) 
					{
						boundIO.SetParamType(SQL_PARAM_INPUT);
						boundIO.SetBindingType(BoundIO::PARAM);
						boundIOs[param_no++] = boundIO; // N.B.! We bind with integer here to indicate a paramter
					}
					else
						continue;
				}
				else {
					if (!filtered.types[idx].IsParam())
					{
						boundIO.SetColNo(idx);
						boundIOs[*i] = boundIO;
					}
				}
				
		  }

		  // we do NOT want the appended fields if the keyMode is not set to use autokey
		  if (km != USE_AUTO_KEY)
		  {
				RemoveAppendedFields(boundIOs, INSERT);

		  }
	  }

	  void SetAppendedFieldNames(const STD_::vector<tstring> &appended)
	  {
		appendedFieldNames = appended;
	  }

	  STD_::vector<tstring> GetAppendedFieldNames() const
	  {
		return appendedFieldNames;
	  }

	  // utility function used for DynamicRowBCA's to remove the appended fields 
	  // as reported by GetFieldInfo from the
      // boundIOs on an INSERT or UPDATE
	  // to clean up, this method fixes up the column and parameter # ordering
	  // to eat up any holes that form in the sequences of the numbering
	  STD_::vector<tstring> RemoveAppendedFields(BoundIOs &boundIOs, SQLQueryType sqlQryType)
	  {
		STD_::vector<tstring> removed;
		
		// nothing to do for other types of queries
		if (sqlQryType != INSERT && sqlQryType != UPDATE)
			 return removed;

		// also nothing to do if no appended fields
		if (appendedFieldNames.empty())
			 return removed;

		// for each appended field name we must:
		// 1. See if boundIO[name] exists, if not, 
		//    repeat this step on the next BoundIO name to look up
		// 2. Save the column number for boundIOs[name]
		// 3. Adjust the colNo field (decrement by 1)
		//    for each boundIO with a column # greater than
		//    that for boundIOs[name]
		// 4. Remove boundIOs[name] from the BoundIOs map
		// 5. Chalk up the column as removed!

		// copy BoundIOs for exception safety
		BoundIOs tmp_boundIOs(boundIOs);

		for (STD_::vector<tstring>::iterator app_it = appendedFieldNames.begin(); 
				app_it != appendedFieldNames.end(); ++app_it)
		{
			// step 1 above
			BoundIOs::iterator b_it = tmp_boundIOs.find(*app_it);

			if (b_it != tmp_boundIOs.end())
			{
				BoundIO &boundIO = b_it->second;

				// step 2 above
				int colNo = boundIO.GetColNo();

				// step 3 above
				for (BoundIOs::iterator col_it = tmp_boundIOs.begin();
						col_it != tmp_boundIOs.end(); ++col_it)
				{
					BoundIO &columnToAdjust = col_it->second;

					// Adjust column # if necessary!
					// decrement columns that appear after boundIOs[name]
					// by 1 to remove hole that will be left by boundIOs[name]
					// after we remove it
					if (columnToAdjust.IsColumn())
					{
						int adjColNo = columnToAdjust.GetColNo();

						if (adjColNo > colNo)
							columnToAdjust.SetColNo(adjColNo - 1);
					}
				}

				// step 4 above
				tmp_boundIOs.erase(b_it);
				tmp_boundIOs.cColumns--;

				// step 5 above
				removed.push_back(*app_it);
			} // end if block
		} // end for app_it in appendedFieldNames loop

		boundIOs.swap(tmp_boundIOs);
		return removed;
	  } // end RemoveAppendedFields()
};

// utility function used for DynamicRowBCA's to remove the appended fields 
// as reported by GetFieldInfo from the
// boundIOs on an INSERT or UPDATE
template<class DataObj> 
   STD_::vector<tstring> RemoveAppendedFieldsFromBCA(
         BCAWrap<DataObj> & DTL_DUMMY_PARAM_1,
         BoundIOs & DTL_DUMMY_PARAM_2, 
         SQLQueryType  DTL_DUMMY_PARAM_3)
{
   STD_::vector<tstring> removed;

   // for non-variant rows do not adjust fields
   return removed;
}
 
inline  STD_::vector<tstring> RemoveAppendedFieldsFromBCA(BCAWrap<variant_row> &bca, BoundIOs &boundIOs, 
									 SQLQueryType sqlQryType)
{
   STD_::vector<tstring> removed;

   try
   {
       // try to grab underlying BCA object
       // if get() succeeds, then we have a DynamicRowBCA to operate on
       DynamicRowBCA &bca_ref = const_cast<DynamicRowBCA &>(bca.get((DynamicRowBCA *) NULL));
	   removed = bca_ref.RemoveAppendedFields(boundIOs, sqlQryType); 
               // remove the appended fields as stored from the call to GetFieldInfo()      
   }
   catch (VariantException &)
   {
       // do nothing for BCA's of type other than DynamicRowBCA
   }

   return removed;
}


template<typename DataObj> 
	   STD_::vector<tstring> GetAppendedFieldNames(BCAWrap<DataObj> & DTL_DUMMY_PARAM_1) {

	   STD_::vector<tstring> keyNames;

	   return keyNames;
}


inline STD_::vector<tstring> GetAppendedFieldNames(BCAWrap<variant_row> &bca) {
   const DynamicRowBCA &bca_ref =
	  bca.get((DynamicRowBCA *) NULL);

   STD_::vector<tstring> keyNames = bca_ref.GetAppendedFieldNames();

   return keyNames;
}



// select default behavior ... record null columns in rowbuf
// if there are other checks you wish to make, put them in
// your own SelValidate function (of type DBView<DataObj, ParamObj>::SelVal)
// This function is specialized version of DefaultSelValidate to copy 
// information about NULL columns to the variant_row class
template<> class DefaultSelValidate<variant_row> {
public:
	bool operator()(BoundIOs &boundIOs, variant_row &rowbuf)
	{
		rowbuf.ClearNulls(); 
		for (BoundIOs::iterator b_it = boundIOs.begin();
				b_it != boundIOs.end(); b_it++)
		{
			BoundIO &boundIO = (*b_it).second;
			if ((boundIO.IsColumn() || boundIO.GetParamType() == SQL_PARAM_OUTPUT  || boundIO.GetParamType() == SQL_PARAM_INPUT_OUTPUT) 
				&& boundIO.IsNull())
				rowbuf.SetNull(boundIO.GetVariantRowIdx()); // found null column ... record null status in rowbuf 
		}

		return true;	// assume data is OK
	}
};

// here we want to make sure that any NULL data in the varaiant_row
// gets reflected in the resulting parameters that are sent to the database
template<> class DefaultInsValidate<variant_row>
{
public:

	bool operator()(BoundIOs &boundIOs, variant_row &rowbuf) {

	    boundIOs.ClearNull(); // clear null on all BoundIOs

		for (BoundIOs::iterator b_it = boundIOs.begin();
				b_it != boundIOs.end(); b_it++)
		{

			BoundIO &boundIO = (*b_it).second;

			if (boundIO.IsParam()) // rowbufs could have parameters in the case of sql_iterator
				try{
					if(rowbuf[(int)std_tatof(boundIO.GetName().c_str())].IsNull())
						boundIO.SetNull();
					else
						boundIO.ClearNull();
				}
				catch(...) {// This parameter # is not part of the rowbuf, lives in ParamBuf instead
					boundIO.ClearNull();
				}
			else
				if(rowbuf[boundIO.GetName()].IsNull())
					boundIO.SetNull();
				else
					boundIO.ClearNull();

			const variant_field &vf =
				   (boundIO.IsParam() ? rowbuf[(int)std_tatof(boundIO.GetName().c_str())]
									  : rowbuf[boundIO.GetName()]);

			// validate that a timestamp was set .. if not, throw with a clear error message
			if (!vf.IsNull() && vf.type() == C_TIMESTAMP)
			{
	   
			   TIMESTAMP_STRUCT TS = (TIMESTAMP_STRUCT) vf;

		           const timestamp_t InvalidTimestamp = {0, 0, 0, 0, 0, 0, 0};
			   if (TS == InvalidTimestamp)
				   DTL_THROW VariantException(_TEXT("DefaultInsValidate<variant_row>()"),
						_TEXT("Uninitialized date field '") + boundIO.GetName() + 
						_TEXT("' in variant_row!"));
			}
		}
		return true;
	}

	~DefaultInsValidate() {};
};


template<class ParamObj = variant_row >
class DynamicDBView : public DBView<variant_row, ParamObj> {
public:
	typedef BCAWrap<variant_row> BCA;
	typedef BPAWrap<ParamObj> BPA;
	typedef typename DBView<variant_row, ParamObj>::InsVal InsVal;
	typedef typename DBView<variant_row, ParamObj>::SelVal SelVal;

#if !defined (__BORLANDC__)
	using DBView<variant_row, ParamObj>::GetBCA; 
	using DBView<variant_row, ParamObj>::SetBPA; 
	using DBView<variant_row, ParamObj>::bca; 
	using DBView<variant_row, ParamObj>::rawSQL; 
	using DBView<variant_row, ParamObj>::keyMode; 
	using DBView<variant_row, ParamObj>::pConn; 
#endif

	DynamicDBView(typename DBView<variant_row, ParamObj>::Args args) : 
	DBView<variant_row, ParamObj>(
		args.bca(
		  DynamicRowBCA(
			GetFieldInfo(
			   args.tableList, args.fieldList, args.postfixClause,
		       args.keyList, args.appendedFieldNames, args.keyMode,
		       *(args.pDBconnection)
			), args.keyMode
		  )
		)
	)
		
	{
		SetBPA(DynamicRowBCA(GetBCA().get((DynamicRowBCA *)NULL), true));

		if (args.keyMode == USE_AUTO_KEY)
		{
			SetAutoKey(args.keyList);

			// unwrap BCAWrap to get the real underyling BCA and set appended field names
			DynamicRowBCA &bca_ref =
				const_cast<DynamicRowBCA &>(this->bca.get((DynamicRowBCA *) NULL));

			bca_ref.SetAppendedFieldNames(args.appendedFieldNames);
		}

	}

	typedef  DEFAULT_IO_HANDLER<variant_row, ParamObj> DIOH;
	
    // note that keyList and appendedFieldNames are for internal use only and
    // are not meant to be called by the user!!!!
	DynamicDBView(const tstring tableList, const tstring fieldList,
		const tstring postfix = _TEXT(""),	
		const SelVal sel_val = DefaultSelValidate<variant_row>(),
		const InsVal ins_val = DefaultInsValidate<variant_row>(),
		const IOHandler<variant_row, ParamObj> io_hand = DIOH(),
		DBConnection &connection = DBConnection::GetDefaultConnection(),
		const KeyMode km = USE_AUTO_KEY,
		const tstring &keyList = _TEXT(""), 
		const STD_::vector<tstring> &appendedFieldNames =
			STD_::vector<tstring>()) :
		DBView<variant_row, ParamObj>(tableList, 
			DynamicRowBCA(GetFieldInfo(tableList, const_cast<tstring &>(fieldList),  postfix,
				const_cast<tstring &>(keyList), 
				const_cast<STD_::vector<tstring> &>(appendedFieldNames),
				km,
				connection), km),
				postfix, DynamicRowBCA(), sel_val, ins_val, io_hand, connection, keyList, km)
		{
			SetBPA(DynamicRowBCA(GetBCA().get((DynamicRowBCA *)NULL), true));

			if (km == USE_AUTO_KEY)
			{
				this->SetAutoKey(keyList);
			
				// unwrap BCAWrap to get the real underyling BCA and set appended field names
				DynamicRowBCA &bca_ref =
					const_cast<DynamicRowBCA &>(this->bca.get((DynamicRowBCA *) NULL));

				bca_ref.SetAppendedFieldNames(appendedFieldNames);
			}

		}


	// exception-safe swap()
	void swap(DynamicDBView<variant_row> &other)
	{

		DBView<variant_row, ParamObj>::swap(other);
	}

	// exception-safe assignment
	DynamicDBView<variant_row> &
		operator=(const DynamicDBView<variant_row> &other)
	{
		if (this != &other)
		{
		   DynamicDBView<variant_row> temp(other);
		   swap(temp);
		}

		return *this;
	}


	// reset BCA to be built from a particular HSTMT
	// used only by sql_iterator
	const BCA & RefreshBCA(HSTMT *p_stmt) { 
		tstring dmy1, dmy2, dmy3;
		bca = DynamicRowBCA(GetFieldInfo(rawSQL, dmy1,  dmy2, dmy3, STD_::vector<tstring>(), 
			                             keyMode, *pConn, p_stmt), keyMode);
		return bca;			  
	}
};


template<class DynamicView,
#ifdef __SUNPRO_CC
         typename IdxContainer>
#else
#  ifdef DTL_USE_NO_HASH_CONTAINER
         typename IdxContainer = DEFAULT_IDXCONTAINER
           <TYPENAME_IN_TEMPLATE_PARAMS DynamicView::DataObject *, 
	        lt_functor<TYPENAME_IN_TEMPLATE_PARAMS DynamicView::DataObject> > >
#  else
         typename IdxContainer = DEFAULT_IDXCONTAINER
           <TYPENAME_IN_TEMPLATE_PARAMS DynamicView::DataObject *, 
	        hash_functor<TYPENAME_IN_TEMPLATE_PARAMS DynamicView::DataObject>,
	        eq_functor<TYPENAME_IN_TEMPLATE_PARAMS DynamicView::DataObject> > >
#  endif
#endif
class DynamicIndexedDBView : public IndexedDBView<DynamicView, IdxContainer>
{
private:
    // given a DynamicView, build the IndexNamesAndFields tstring for the
	// indexed view
	// this function constructs a general nonunique key composed of all fields
	// from the view
	static tstring BuildNamesAndFieldsStr(const DynamicView &view)
	{
	   tstring result;

	   result += _TEXT("PrimaryKey; "); // generate key name
	   result += MakeDelimitedList(view.GetColNames()); // tack on column names

	   return result;
	}

	typedef typename DynamicView::ParamObject ParamObj;
    typedef typename dtl::DynamicIndexedDBView
	                    <DynamicView, IdxContainer>  this_type;
    typedef typename dtl::IndexedDBView
                        <DynamicView, IdxContainer>  parent_type;

public:

	DynamicIndexedDBView(typename parent_type::Args args) : 
	  parent_type(args.indexes(
		      args.INF == _TEXT("") ? BuildNamesAndFieldsStr(*(args.pDBView)) :
									  args.INF
		  ).fetch_mode(BULK_FETCH)
	  )
	{ 
								  
	}

									  
	DynamicIndexedDBView(const DynamicView &view, 
						 const tstring IndexNamesAndFields = _TEXT(""),
						 BoundMode bm = UNBOUND, 
						 KeyMode km = USE_AUTO_KEY, 
						 typename parent_type::SetParamsFn SetPar
						     = DefaultSetParams<ParamObj>()) 
	  : parent_type(view, 
					(IndexNamesAndFields == _TEXT("")
					 ? BuildNamesAndFieldsStr(view) : IndexNamesAndFields), 
					bm, 
					km,
					SetPar,
					BULK_FETCH)
    {}


    // exception-safe swap()
	void swap(this_type &other)
	{
		parent_type::swap(other);
	}

	// exception-safe assignment
	this_type& operator=(const this_type &other)
	{
		if (this != &other)
		{
			this_type temp(other);
			swap(temp);
		}
		return *this;
	}
};


END_DTL_NAMESPACE
#endif
