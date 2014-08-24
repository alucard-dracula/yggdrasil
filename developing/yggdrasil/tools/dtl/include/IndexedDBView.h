// -*- C++ -*-
/* Copyright © 2000
Michael Gradman and Corwin Joy

Permission to use, copy, modify, distribute and sell this software and
its documentation for any purpose is hereby granted without fee, provided
that the above copyright notice appears in all copies and that both that
copyright notice and this permission notice appear in supporting documentation.
Corwin Joy and Michael Gradman make no representations about the suitability
of this software for any purpose.
It is provided "as is" without express or implied warranty.
*/
// provides indexing (keys) for views
// Initial: 10/5/2000 - MG
// Edited: 11/20/2000 - MG - using View parameter in IndexedDBView to
//							 replace explicit DBView<> decls.
// Edited: 12/19/2000 - MG - added namespaces

#ifndef DTL_INDEXEDDBVIEW_H
#define DTL_INDEXEDDBVIEW_H

#include "DBIndex.h"
#include "starit.h"
#include "string_util.h"
#include "dtl_algo.h"

#include "std_warn_off.h"
#include <list>
#include <assert.h>
#include "std_warn_on.h"

#include "update_iterator.h"
#include "delete_iterator.h"

BEGIN_DTL_NAMESPACE

// tags on whether to generate bulk_copy() call

struct GEN_BULK_FETCH { };

struct NO_GEN_BULK_FETCH { };

// if DataObj is variant_row, do NOT generate bulk_copy() call
// else compiler will complain
template <class DataObj> struct generate_bulk_fetch
{
	typedef GEN_BULK_FETCH result;
};

template<> struct generate_bulk_fetch<variant_row>
{
	typedef NO_GEN_BULK_FETCH result;
};

// empty InsValidate call
template<class DataObj> class NoInsValidate :
	public STD_::binary_function<BoundIOs &, DataObj &, bool>
{
public:
	bool operator()(BoundIOs & DTL_DUMMY_PARAM_1, DataObj & DTL_DUMMY_PARAM_2)
	{
		return true;
	}

};

template<class DynamicView, class IdxContainer> class DynamicIndexedDBView;

template<class DataObj, class BCA = DefaultBCA<DataObj> > class DynamicBCA
{
  private:
	  BoundIOs dynbios; // bound IO objects stored in memory
	  BoundIO::ColumnsOrParams bindMode; // what mode does this functor work in
	  BCA bca;
	  STD_::map<tstring, tstring> ParamToColumn;
  public:

	  DynamicBCA(const BoundIOs &boundIOs, const BCA &b = BCA()) :
        dynbios(boundIOs), bindMode(BoundIO::BIND_AS_COL), bca(b) { }

	  // used to bind as parameters
	  DynamicBCA(const BoundIOs &boundIOs, const BCA &b, STD_::vector<tstring> orderOfParams)
        : bindMode(BoundIO::BIND_AS_PARAM),
          bca(b)
	  {
			  // copy over column BoundIOs and convert them to parameters
			  for (size_t i = 0; i != orderOfParams.size(); ++i)
			  {
				 // proper ordering of column names for parameters (for postfix clause in updates)
				 // preserved in orderOfParams
				 tstring colName = orderOfParams[i];

				 // find parameter with given name
				 BoundIOs::const_iterator it2 = boundIOs.find(colName);

				 if (it2 == boundIOs.end())
                DTL_THROW DBException(
                      _TEXT("DynamicBCA::DynamicBCA"),
                      _TEXT("Column \"") + colName +
                      _TEXT("\" not found in BoundIOs!"),
                      NULL, NULL);

             // ensure we didn't find a parameter
				 assert(!(*it2).second.IsParam());

				 // convert boundIO over to a parameter
				 // complete with number and everything!
				 if ((*it2).second.IsColumn())
				 {
					tostringstream numstr;
					numstr << i << STD_::ends;

					// now insert, the fresh new BoundIO
               BoundIO boundIO = (*it2).second;
					boundIO.SetAsParam(i);
					dynbios.insert(STD_::pair<const tstring, BoundIO>(numstr.str(),boundIO));

					// note any columns that must be renamed for user validation
					ParamToColumn[numstr.str()] = colName;
				 }
             else
             {
                // Otherwise its 'UNKNOWN', do you really want them to stay?
             }
			  }
	  }

	  // exception-safe swap()
	  void swap(DynamicBCA<DataObj, BCA> &other)
	  {
		  bca.swap(other.bca);
		  STD_::swap(bindMode, other.bindMode);
		  dynbios.swap(other.dynbios);
	  }

	  // exception-safe assignment
	  DynamicBCA<DataObj, BCA> &operator=(const DynamicBCA<DataObj, BCA> &other)
	  {
		 if (this != &other)
		 {
			 DynamicBCA<DataObj, BCA> temp(other);
			 swap(temp);
		 }

		 return *this;
	  }

	  // bca applied here

	  void operator()(BoundIOs &boundIOs, DataObj &rowbuf)
	  {

		// We need this for two reasons:
		// 1. To initialize the variant_row properly
		// 2. To captute any user validation functions
		BoundIOs tmp;
      tmp.BindAsBase(rowbuf);
		bca(tmp, rowbuf);  // bind to a dummy BoundIOs to not screw up our params
		boundIOs.AddUserValidate(tmp.GetUserValidate());

		for (BoundIOs::iterator it = dynbios.begin(); it != dynbios.end(); it++)
		{
			BoundIO &boundIO = (*it).second;
			tstring name = (*it).first;
			BYTE *fieldAddr = (BYTE *) data_ptr(&rowbuf) + boundIO.GetOffset();
			boundIO.SetAddr(fieldAddr);
			boundIOs[name] = boundIO;

			if (boundIO.IsParam())
			{
				boundIOs.cColumns--;
				// update any user validations that depend on this
				boundIOs.RenameUserValidateColumn(ParamToColumn[name], name);
			}


#ifdef DTL_MEM_DEBUG
			tcout << _TEXT("DynamicRowBCA is binding name: ") << name <<
				_TEXT(" IsNull: ") << boundIO.IsNull() <<
				_TEXT(" Addr: ") << (void *)fieldAddr << STD_::endl;
#endif
		}





#ifdef DTL_MEM_DEBUG
		  	tcout << _TEXT("DynamicRowBCA done binding names ") << STD_::endl;
#endif

	  }

};

// function to set the underlying ParamObj
// generic SetParams callback that expects a signature of
// void SetParams(ParamObj &params)

template<class ParamObj> class ImplBaseParams
{
public:
  ImplBaseParams() { }

  virtual ~ImplBaseParams() { }

  virtual ImplBaseParams* clone() = 0;

  virtual void invoke(ParamObj &params) = 0;
};

// The clone() is implemented here.
template<class ParamObj, typename T>
    class  ImplParams : public ImplBaseParams<ParamObj>
    {
	public:
       ImplParams (const T &v ) : data ( v )
	   {}

       virtual ~ImplParams () {}
       virtual ImplBaseParams<ParamObj>* clone()
	   {
		   ImplBaseParams<ParamObj> *copy = new ImplParams(data);
		   return copy;
	   }

	   virtual void invoke(ParamObj &params) {
		  data(params);
	   }

    private:
	   T data ;

    } ;

template<class ParamObj> class SetParamsWrap : public STD_::unary_function<ParamObj &, void>
{

	private:


	public:
	  // signature of SetParamsWrap must be:
	  // void bca(BoundIOs &boundIOs, ParamObj &rowbuf)
	  // forward call
	  void operator()(ParamObj &params) {
			data->invoke(params);
		}


    private:

	ImplBaseParams<ParamObj>* data; // polymorphically stores the method object

    SetParamsWrap()  { } // Don't allow default constructor -- too dangerous

	template<typename T>
	   static ImplParams<ParamObj, T>*
         CastFromBase ( ImplBaseParams<ParamObj>* v , T const* =NULL )
   {
      ImplParams<ParamObj, T>* p = dynamic_cast<ImplParams<ParamObj, T>*> ( v ) ;
      if ( p == NULL )
	  {
		  STD_::string errmsg;
		  errmsg.reserve(512);
		  errmsg += typeid(T).name();
		  errmsg += " is not a valid type";

		  DTL_THROW VariantException(_TEXT("SetParamsWrap::CastFromBase()"),
			tstring_cast((tstring *)NULL, errmsg));
	  }
      return p ;
    }

public:

     template<typename T> SetParamsWrap ( T const& v )
      : data ( new ImplParams<ParamObj, T>(v) )
      {}

	  // * Copy the internal data instead of sharing it.
      SetParamsWrap( const SetParamsWrap & rhs )
      : data ( rhs.data != NULL ? rhs.data->clone()
                                : NULL
             )
      {}

	  // No need to test for concurrent users.
	  ~SetParamsWrap() { delete data ; }


	  // exception-safe swap
	  void swap(SetParamsWrap &h)
	  {
			std::swap(h.data, data);
	  }

	  // assignment operator
	  SetParamsWrap &operator=(const SetParamsWrap &method)
	  {
		   if (this != &method)
		   {
			  SetParamsWrap tmp(method);
			  swap(tmp);
		   }

		   return *this;
	  }

	  template<typename T> const T & get( T const* dmy =NULL ) const
      {
		  return CastFromBase( data , dmy )->data ;
	  }

};



template<class ParamObj> class DefaultSetParams :
	public STD_::unary_function<ParamObj &, void>
{
  public:
	  void operator()(ParamObj & DTL_DUMMY_PARAM_1)
	  { }
};

template<class ParamObj> class SetParamsFromClass :
	public STD_::unary_function<ParamObj &, void>
{
  private:
	  ParamObj *param_;
  public:
	  SetParamsFromClass(const ParamObj &p)  {param_ = new ParamObj(p);}
	  SetParamsFromClass(const SetParamsFromClass &p)  {param_ = new ParamObj(*(p.param_));}

	  ~SetParamsFromClass() {
		  delete param_;
		  param_=NULL;
	  }
	  void operator()(ParamObj &paramObj)
	  { paramObj = *param_;}
	  void swap(SetParamsFromClass &sp)
	  {
		  std::swap(sp, param_);
	  }
	  // assignment operator
	  SetParamsFromClass &operator=(const SetParamsFromClass &setpars)
		{
		   if (this != &setpars)
		   {
		      SetParamsFromClass tmp(setpars);
		      swap(tmp);
		   }

		   return *this;
		}
};


template<typename View,
#ifdef DTL_USE_NO_HASH_CONTAINER
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
         typename IdxContainer = DEFAULT_IDXCONTAINER
            <TYPENAME_IN_TEMPLATE_PARAMS View::DataObject *,
	         hash_functor<TYPENAME_IN_TEMPLATE_PARAMS View::DataObject>,
	         eq_functor<TYPENAME_IN_TEMPLATE_PARAMS View::DataObject> >,
#  endif
		 typename HashOrNoHash = HASH>
#endif
class IndexedDBView
{
  public:
	  typedef typename View::DataObject DataObj;
	  typedef typename View::ParamObject ParamObj;
	  typedef BCAWrap<DataObj> BCA;
	  typedef BPAWrap<ParamObj> BPA;
  protected:

	  typedef DBIndex<View, IdxContainer, HashOrNoHash> IndexType;

	  // function to set the underlying ParamObj
	  // typedef void (*SetParamsFn) (ParamObj &params);
	  typedef SetParamsWrap<ParamObj> SetParamsFn;

 	  // points to a DataObj *
	  typedef typename IndexType::iterator indexed_data_iterator;

	  // points to a DataObj
	  typedef typename STD_::list<DataObj>::iterator data_iterator;
	  typedef STD_::pair<indexed_data_iterator, indexed_data_iterator> indexed_data_pair;

	  // index map type
	  typedef STD_::map<tstring, IndexType> index_map;

	  // fetch using bulk or single mode?
	  FetchMode fetchMode;

	  // number of records to fetch in one call to bulk_copy() if in bulk mode
	  size_t numRecordsToFetch;

	  // should we generate bulk_copy() call ...
	  // only generate it for non-variant_row types
	  typedef typename generate_bulk_fetch<DataObj>::result should_gen_bulk_fetch;

  public:

// hash containers will crap out on iterator::operator--() because they only
// use forward iterators
// unfortunately, we can't support both forward iterators AND bidirectional iterators here
// without partial specialization
// so if hashed containers are supported, then we only support forward iterators in
// IndexedDBView, otherwise we support bidirectional iterators
// (know of any other fix????)

#ifndef DTL_USE_HASH_CONTAINER
	  // star iterator adapter for indexed_data_iterator
	  typedef typename dtl::star_bidirectional_iterator<typename IndexType::iterator,
	     DataObj, DataObj &, DataObj *> indexed_iterator;

	  typedef typename dtl::star_bidirectional_iterator<typename IndexType::const_iterator,
	     const DataObj, const DataObj &, const DataObj *> const_indexed_iterator;
#else
	  // star iterator adapter for indexed_data_iterator
	  typedef typename dtl::star_forward_iterator<typename IndexType::iterator,
	     DataObj, DataObj &, DataObj *> indexed_iterator;

	  typedef typename dtl::star_forward_iterator<typename IndexType::const_iterator,
	     const DataObj, const DataObj &, const DataObj *> const_indexed_iterator;
#endif

	  typedef STD_::pair<indexed_iterator, indexed_iterator> indexed_pair;

	  // standard STL typedefs!
	  typedef DataObj value_type;
	  typedef value_type* pointer;
	  typedef const value_type* const_pointer;
	  typedef value_type& reference;
	  typedef const value_type& const_reference;

	  // note that as DBIndex::iterator is a constant bidirectional iterator, so is
	  // IndexedDBView::iterator
	  typedef indexed_iterator iterator;

	  // typedef indexed_iterator const_iterator; // buggy as able to modify on operator*()

	  // buggy as implicit conversion of template args. does not occur
	  typedef const_indexed_iterator const_iterator;

// hashed containers will crap out on iterator::operator--()
#ifndef DTL_USE_HASH_CONTAINER
#  if defined(_MSC_VER) // && !defined(__SGI_STL_PORT)
    #if  !defined (__SGI_STL_PORT)
    // Microsoft non-standard in how it defines reverse_iterator
	  typedef STD_::reverse_bidirectional_iterator<iterator, DataObj> reverse_iterator;
	  typedef STD_::reverse_bidirectional_iterator<const_iterator, DataObj> const_reverse_iterator;

    #else
		#if __SGI_STL_PORT <= 0x452
			typedef STD_::reverse_bidirectional_iterator<iterator, value_type> reverse_iterator;
			typedef STD_::reverse_bidirectional_iterator<const_iterator, value_type> const_reverse_iterator;
		#else
			typedef STD_::reverse_iterator<iterator> reverse_iterator;
			typedef STD_::reverse_iterator<const_iterator> const_reverse_iterator;
		#endif
    #endif


#  elif defined(__BORLANDC__) || defined(__GNUC__)
		typedef STD_::reverse_iterator<iterator> reverse_iterator;
		typedef STD_::reverse_iterator<const_iterator> const_reverse_iterator;
#  elif defined (__SUNPRO_CC)
      typedef STD_::reverse_iterator
           	           <iterator,
		                STD_::bidirectional_iterator_tag,
					    DataObj>                     reverse_iterator;
	  typedef STD_::reverse_iterator
                       <const_iterator,
		                STD_::bidirectional_iterator_tag,
					    DataObj>                     const_reverse_iterator;
#  elif defined (__hpux)
  typedef STD_::reverse_iterator<iterator>              reverse_iterator;
  typedef STD_::reverse_iterator<const_iterator>        const_reverse_iterator;
#  else
		// definition of std::reverse_bidirectional_iterator slightly different between compilers
		// just use the common denominator here
		typedef STD_::reverse_bidirectional_iterator<iterator, DataObj, DataObj &, std::ptrdiff_t> reverse_iterator;
		typedef STD_::reverse_bidirectional_iterator<const_iterator, DataObj, const DataObj &, std::ptrdiff_t> const_reverse_iterator;
#  endif
#endif

	  typedef std::ptrdiff_t difference_type;
	  typedef size_t size_type;


	  typedef IdxContainer index_container_type;

  protected:
	  indexed_pair sp(indexed_data_pair dp) const {
		    indexed_iterator i(dp.first);
			indexed_iterator j(dp.second);
			indexed_pair s(i, j);
			return s;
	  }

	  indexed_pair sp(indexed_iterator it) const
	  {
			return indexed_pair(it, it);
	  }

	  View *pDBview;  // points to underlying view that the
			 		  // indexed view refers to

	  BCA bca;

	  // these members are mutable as several methods needed to make this class STL
	  // compatible must be declared const and have to modify these members
	  mutable STD_::list<DataObj> data;		  // actual data objects owned here
	  mutable STD_::map<tstring, IndexType > indexes;
										  // the indices are named and refer
										  // to the data objects stored in the list
	  mutable bool bFetched;	// flag indicating whether the data has been fetched from the DB

	  SetParamsFn SetParams; // function to set the underlying ParamObj

	  STD_::vector<tstring> orderOfKeys; // reflects on the order of the strings passed in at
								  // construction
								  // first key is the one we wish to apply first,
								  // 2nd key we wish to apply 2nd, etc.

	  STD_::vector<typename IndexType::Uniqueness> isUniqueKeys; // needed for copy constructor and assignment
									   // operator to properly build the object
									   // and preserve uniqueness from source Index
      STD_::vector<tstring> FieldNameCommaLists; // needed for copy constructor and
										  // assignment operator to
										  // properly build the object
										  // essentially is a STD_::vector of
										  // comma delimited field lists

	  BoundMode boundToDB;				  // are we BOUND to the DB or UNBOUND
										  // changes propagate through to DB if bound
	  KeyMode useWhichFields;			  // do we USE_PK_ONLY, USE_ALL_FIELDS,
										  // or USE_AUTO_KEY
										  // for writing changes to DB
										  // (note: USE_AUTO_KEY only valid for
										  //  DynamicDBView)

	  // used by copy constructor and assignment operator to build the indexes
	  // and populate them with DataObj *'s
	  void BuildIndexes()
	  {
			 // note that if the copy has fetched the data already, we don't need
			 // to fetch() again (if changes are made, we don't need to query the database
			 // for them as the data will be properly maintained in the IndexedDBView
			 // and we never change the underlying query or its parameters)
			 // thus, consistency of the IndexedDBView with the DB is not a necessary
			 // condition?)

			 // safe, but useless to copy a default constructed IndexedDBView
		     if (pDBview == NULL)
				return;

			 // temporary map of indexes used to provide exception-safety
			 // atomicity and consistency guarantees
			 STD_::map<tstring, IndexType> tmp;


			 STD_::vector<tstring>::iterator it = orderOfKeys.begin();
			 STD_::vector<tstring>::iterator fname_it = FieldNameCommaLists.begin();
			 typename STD_::vector<TYPENAME_IN_TEMPLATE_PARAMS IndexType::Uniqueness>::iterator
				 un_it = isUniqueKeys.begin();

			 // we must rebuild the indexes map because the DataObj's are copied
			 // in the copy constructor for data and thus have different addresses
			 while (it != orderOfKeys.end())
			 {
				tstring IndexName = *it;
			    tstring FieldNames = *fname_it;
				typename IndexType::Uniqueness isUnique = *un_it;

				IndexType idx(IndexName, FieldNames, *pDBview, isUnique);

				// create this index
			    tmp.insert(STD_::pair<const tstring, IndexType>(IndexName, idx));

				// for each DataObj in the list, insert a pointer it in the index's
				// data list, using the proper address of the newly made copy
				if (bFetched)
				{
					for (data_iterator data_it = data.begin(); data_it != data.end(); data_it++)
					{
						// get address of the list element as it's the pointer we need
						DataObj *pData = &(*data_it);

						// indexes[IndexName].insert(*pData);

						typename index_map::iterator map_it = tmp.find(IndexName);

						// IndexName should always be in the list
						if (map_it != tmp.end())
						{
							(*map_it).second.insert(*pData);
						}
					}
				}

				// march through matching vectors
				++it;
				++fname_it;
				++un_it;

			 }

			 indexes.swap(tmp);
	  }

	  // InsValidate() which validates the DataObj passed in
	  bool InsValidate(const DataObj &dataObj)
	  {
		 DataObj &dat = const_cast<DataObj &>(dataObj);
		 BoundIOs boundIOs;
		 boundIOs.BindAsBase(dat);

		 bca(boundIOs, dat);

		 return (pDBview->GetInsVal())(boundIOs, dat);
											  // forward the return value
											  // from underlying view's
											  // InsVal() method
	  }

  public:

	  class Args;
	  friend class Args;
	  class Args
	  {
	  private:
		  const View *pDBView;
		  tstring INF;
		  BoundMode bm ;
		  KeyMode km;
		  SetParamsFn sp;
		  FetchMode fm;
		  size_t fr;
	  public:
		  friend class IndexedDBView<View, IdxContainer, HashOrNoHash>;
		  friend class DynamicIndexedDBView<View, IdxContainer>;

		  // we get sneaky here by defaulting keyMode to USE_AUTO_KEY
		  // it will get coerced anyway to USE_ALL_FIELDS if the view is static
		  // (that is, it doesn't use DynamicRowBCA)
		  Args() : pDBView(NULL), INF(), bm(UNBOUND), km(USE_AUTO_KEY),
			  sp(DefaultSetParams<ParamObj>()), fm(DEFAULT_FETCH_MODE), fr(100)
		  {}

		  Args &view(const View &rview) {
			  pDBView = &rview;
			  return *this;
		  }

		  Args &indexes(const tstring &s) {
			  INF = s;
			  return *this;
		  }

		  Args &bound(BoundMode m) {
			  bm = m;
			  return *this;
		  }

		  Args &key_mode(KeyMode k) {
			  km = k;
			  return *this;
		  }

		  Args &params(SetParamsFn p) {
			  sp = p;
			  return *this;
		  }

		  Args &fetch_mode(FetchMode f)
		  {
			  fm = f;
			  return *this;
		  }

		  Args &fetch_records(size_t n)
		  {
			  fr = n;
			  return *this;
		  }
	  };


	  IndexedDBView(const Args &args) :
	  fetchMode(typeid(DataObj) != typeid(variant_row) ? args.fm : SINGLE_FETCH),
	  numRecordsToFetch(args.fr), pDBview(NULL), bca(args.pDBView->GetBCA()), data(), indexes(), bFetched(false), SetParams(args.sp),
	  orderOfKeys(), isUniqueKeys ( ), FieldNameCommaLists(), boundToDB(args.bm), useWhichFields(args.km)
	  {
		  init(*(args.pDBView), args.INF, args.km);
	  }

	  // IndexNamesAndFields takes the form
	  // _TEXT("IndexName1; IndexField1, IndexField2; IndexName2; IndexField3, IndexField4, IndexField5")};
	  // used to construct the map of indexes.
	  // first index in constructor list is used as primary index in default
	  // equal_range() method.
	  // The keyword UNIQUE may be place in front of any index name to indicate that
	  // that set of fields represents a unique constraint on the rows
	  // SetParams() function is used to set any of the condition parameters that are to be applied as
	  // part of the fetch.
	  // so, e. g.
	  // DBView view(_TEXT("DB_EXAMPLE"), BCA);
	  // IndexedDBView indexedView(view, _TEXT("Unique StringIndex; STRING_VALUE; AnotherIndex; INT_VALUE, EXAMPLE_LONG"));
	  IndexedDBView(const View &view, const tstring IndexNamesAndFields,
		  BoundMode bm = UNBOUND, KeyMode km = USE_ALL_FIELDS,
		  SetParamsFn SetPar = DefaultSetParams<ParamObj>(),
		  FetchMode fm = DEFAULT_FETCH_MODE, size_t fr = 100) :
			fetchMode(typeid(DataObj) != typeid(variant_row) ? fm : SINGLE_FETCH),
			numRecordsToFetch(fr), pDBview(NULL), bca(view.GetBCA()), data(), indexes(), bFetched(false), SetParams(SetPar),
			orderOfKeys(), isUniqueKeys ( ), FieldNameCommaLists(), boundToDB(bm), useWhichFields(km)
			{
				init(view, IndexNamesAndFields, km);
			}

	  private:
	  void init(const View &view, const tstring IndexNamesAndFields, KeyMode km)
		{
			// make *copy* of view here
			pDBview = new View(view);

			// coerce to USE_ALL_FIELDS if underlying DBView is static or
		    // doesn't use autokey itself
		    if (km == USE_AUTO_KEY && pDBview->GetKeyMode() != USE_AUTO_KEY)
			{
				useWhichFields = USE_ALL_FIELDS;
			}

			// construct autokey string if were in autokey mode
			// convention will name the constructed index for the autokey
			// as "AutoKey" and tack onto IndexNamesAndFields string

			// better behavior would be to allow syntax in IndexNamesAndFields to say
			// "KeyName; AutoKey" ... and the parsing routine
			// would expand the AutoKey keyword to the actual fields in the autokey
			// stored in the view
			if (useWhichFields == USE_AUTO_KEY)
			{
				tstring &INF_mutable =
					const_cast<tstring &>(IndexNamesAndFields);

				INF_mutable.reserve(INF_mutable.capacity() * 2);

				// figure out proper delimiter prefixing AutoKey
				// if string is empty or has only spaces, do nothing
				// else prefix with "; "
				tstring prefix;

				tstring tmp(INF_mutable);

				if (!EliminateSpaces(tmp).empty())
					prefix = _TEXT("; ");

				INF_mutable += prefix;
				INF_mutable += _TEXT("Unique AutoKey; ");
				INF_mutable += MakeDelimitedList(pDBview->GetKey());
			}

			// construct the map of indices based on the IndexNamesAndFields tstring

			TCHAR *c_str = new TCHAR[IndexNamesAndFields.length() + 1];

			std_tstrncpy(c_str, IndexNamesAndFields.c_str(), IndexNamesAndFields.length() + 1);


			// we first need to parse out this tstring for semicolons
			// to give us strings we can work with for IndexName;FieldList pairs
			STD_::vector<tstring> semiColonsParsedStrings;

			TCHAR *savept = NULL;
			TCHAR *semiColonToken = std_tstrtok(c_str, _TEXT(";"), &savept);

			while (semiColonToken != NULL)
			{
				// for each token, trim whitespace and then record it
				trim(semiColonToken);

				// don't allow empty tokens after whitespace eaten
				// DBIndex constructor will have to perform a similar check for
				// field names once they get parsed
				if (std_tstrlen(semiColonToken) == 0)
				{
					delete[] c_str;
					DTL_THROW DBException(_TEXT("IndexedDBView::IndexedDBView()"),
						_TEXT("Invalid IndexNamesAndFields tstring \"") +
						IndexNamesAndFields + _TEXT("\"!  Empty IndexName or FieldList detected!"),
						NULL, NULL);
				}
				semiColonsParsedStrings.push_back(semiColonToken);

				semiColonToken = std_tstrtok(NULL, _TEXT(";"), &savept);
			}

			delete[] semiColonToken;
			delete[] c_str;

			// now we've got to make sense out of the name/list pairs
			STD_::vector<tstring>::iterator tok_it = semiColonsParsedStrings.begin();

			bool AutoKeyFound = false; // have we found the autokey yet?
									   // in order for us to reject a user key
									   // named "AutoKey"

			// loop to STD_::pair up and validate strings
			while (tok_it != semiColonsParsedStrings.end())
			{
				tstring IndexName(*tok_it);

				typename IndexType::Uniqueness isUnique = IndexType::NONUNIQUE_INDEX;

				// first up should be an IndexName
				// commas mean we have an invalid name as user
				// erroneously entered a field list
				if (numOfOccurrences(',', IndexName) != 0)
					DTL_THROW DBException(_TEXT("IndexedDBView::IndexedDBView()"),
						_TEXT("Invalid IndexNamesAndFields tstring \"") +
						IndexNamesAndFields + _TEXT("\"!  Expected IndexName, instead found ")
						_TEXT("FieldList!"), NULL, NULL);

				// check for _TEXT("unique") keyword ... case insensitive
				MemPtr strTokMe((IndexName.length() + 1)*sizeof(TCHAR));

				std_tstrcpy(reinterpret_cast<TCHAR *>(strTokMe.get()), IndexName.c_str());

				// get tstring up to first space
				savept = NULL;
				TCHAR *nameToken = std_tstrtok(reinterpret_cast<TCHAR *>(strTokMe.get()), _TEXT(" "), &savept);

				if (std_tstricmp(nameToken, _TEXT("Unique")) == 0)
				{
					// index is unique ... so we expect an index name to follow
					// strtoking on ';' guarantees to grab the rest of the tstring
					// we're parsing for the index name
					nameToken = std_tstrtok(NULL, _TEXT(";"), &savept);

					// can't use unique as index name as it's a keyword
					if (std_tstricmp(nameToken, _TEXT("Unique")) == 0)
					{
						tstring errmsg;
						errmsg.reserve(512);
						errmsg += _TEXT("Invalid IndexNamesAndFields tstring \"");
						errmsg += IndexNamesAndFields;
						errmsg += _TEXT("\"!  Can't use keyword \"Unique\" as ");
						errmsg += _TEXT("an index name!");

						DTL_THROW DBException(_TEXT("IndexedDBView::IndexedDBView()"),
						   errmsg, NULL, NULL);
					}
					// must specify name of unique index
					else if (nameToken == NULL)
					{
						tstring errmsg;
						errmsg.reserve(512);
						errmsg += _TEXT("Invalid IndexNamesAndFields tstring \"");
						errmsg += IndexNamesAndFields;
						errmsg += _TEXT("\"!  Expected name of unique index!");
						DTL_THROW DBException(_TEXT("IndexedDBView::IndexedDBView()"),
							errmsg, NULL, NULL);
					}

					isUnique = IndexType::UNIQUE_INDEX;

					// IndexName is simply the part of the tstring after _TEXT("Unique")
					IndexName = nameToken;

				}
				else
				{
					// index is nonunique ... IndexName is the name of the index
					isUnique = IndexType::NONUNIQUE_INDEX;
				}

				tok_it++;

				// grab FieldNames list ... if no strings are left in the STD_::vector,
				// then we have an IndexName that doesn't have any fields associated with it
				if (tok_it == semiColonsParsedStrings.end())
				{
					tstring errmsg;
					errmsg.reserve(512);
					errmsg += _TEXT("Invalid IndexNamesAndFields tstring \"");
					errmsg += IndexNamesAndFields;
					errmsg += _TEXT("\"!  Expected FieldList, instead found ");
					errmsg += _TEXT(" premature end of list!");

					DTL_THROW DBException(_TEXT("IndexedDBView::IndexedDBView()"),
						errmsg, NULL, NULL);
				}

				const tstring FieldNames(*tok_it);

				// the DBIndex constructor will be responsible for making sure
				// that the fields in the field list are valid columns in the view
				// we must first make sure that the map already doesn't contain an index
				// with the given name
				// DBIndex constructor will throw an exception if there's
				// a problem with the FieldNames
				IndexName = EliminateSpaces(IndexName);

				// don't allow user to give an index name of "AutoKey"
				//
				if (std_tstricmp(IndexName.c_str(), _TEXT("AutoKey")) == 0)
				{
				   // accept use of "AutoKey" iff it's used exactly once
				   // in the IndexNamesAndFields string on an autokeyed view
				   if ((useWhichFields != USE_AUTO_KEY) || AutoKeyFound)
				   {
					DTL_THROW DBException(_TEXT("IndexedDBView::IndexedDBView()"),
						_TEXT("Can't use 'AutoKey' as a key name (reserved by system)!"),
						NULL, NULL);
				   }

				   AutoKeyFound = true;
				}

				IndexType idx(IndexName, FieldNames, *pDBview, isUnique);

				typedef typename STD_::map<tstring, IndexType >::iterator   map_tstring_iterator;
				STD_::pair<map_tstring_iterator, bool>  ins_pair(indexes.insert(STD_::pair<const tstring, IndexType>
						(IndexName, idx)));

				// if insertion failed, that means the user tried to specify a duplicate
				// key name
				if (!(ins_pair.second))
				{
					tstring errmsg;
					errmsg.reserve(512);
					errmsg += _TEXT("Invalid IndexNamesAndFields tstring \"");
					errmsg += IndexNamesAndFields;
					errmsg += _TEXT("\"!  Attempted to redefine IndexName \"");
					errmsg += IndexName;
					errmsg += _TEXT("\"!");
					DTL_THROW DBException(_TEXT("IndexedDBView::IndexedDBView()"),
						errmsg, NULL, NULL);
				}

				// record in key names list
				orderOfKeys.push_back(IndexName);

				// also record FieldNames comma delimited list and uniqueness ... need for

				// copy constructor and assignment operator to work properly
				FieldNameCommaLists.push_back(FieldNames);
				isUniqueKeys.push_back(isUnique);

				tok_it++;
			}

			// now have all information needed to build template rowbuf

			// BuildTemplateRowbuf();

		}

		public:
	   // requirements for Assignable!

	   // default constructor

	   // as any IndexedDBView must be associated with a DBView, you *must*
	   // assign another IndexedDBView to a default constructed object
	   IndexedDBView() : pDBview(NULL), bca(DefaultBCA<DataObj>()), data(), indexes(), bFetched(false),
		  SetParams(DefaultSetParams<ParamObj>()), orderOfKeys(), isUniqueKeys(), FieldNameCommaLists(),
		  boundToDB(UNBOUND), useWhichFields(USE_ALL_FIELDS)
	   { }

	   // copy constructor
	   IndexedDBView(const IndexedDBView<View, IdxContainer, HashOrNoHash> &idxview) : numRecordsToFetch(idxview.numRecordsToFetch),
	      pDBview(NULL),
		  bca(DefaultBCA<DataObj>()), data(idxview.data), indexes(), bFetched(idxview.bFetched),
		  SetParams(idxview.SetParams), orderOfKeys(idxview.orderOfKeys),
		  isUniqueKeys(idxview.isUniqueKeys), FieldNameCommaLists(idxview.FieldNameCommaLists),
		  boundToDB(idxview.boundToDB), useWhichFields(idxview.useWhichFields)
		  {
			// need *copy* of view
			if (idxview.pDBview != NULL)
			{
				pDBview = new View(*(idxview.pDBview));
				bca = pDBview->GetBCA();
			}

			BuildIndexes();
		  }

	   // exception-safe swap()
	   void swap(IndexedDBView<View, IdxContainer, HashOrNoHash> &other)
	   {
		   STD_::swap(pDBview, other.pDBview);
		   bca.swap(other.bca);
		   data.swap(other.data);
		   indexes.swap(other.indexes);
		   STD_::swap(bFetched, other.bFetched);
		   STD_::swap(SetParams, other.SetParams);
		   FieldNameCommaLists.swap(other.FieldNameCommaLists);
		   STD_::swap(isUniqueKeys, other.isUniqueKeys);
		   STD_::swap(boundToDB, other.boundToDB);
		   STD_::swap(useWhichFields, other.useWhichFields);
		   STD_::swap(orderOfKeys, other.orderOfKeys);
		   STD_::swap(fetchMode, other.fetchMode);
		   STD_::swap(numRecordsToFetch, other.numRecordsToFetch);
	   }

	   // exception-safe assignment
	   IndexedDBView<View, IdxContainer, HashOrNoHash> &
		   operator=(const IndexedDBView<View, IdxContainer, HashOrNoHash> &other)
	   {
		  if (this != &other)
		  {
			 IndexedDBView<View, IdxContainer, HashOrNoHash> temp(other);
			 swap(temp);
		  }

		  return *this;

	   }

	   virtual ~IndexedDBView()
	   {
		  delete pDBview;
		  pDBview = NULL;
	   }


		 // *** ALL ITERATORS BELOW MUST CALL fetch() FIRST ***

	   // WARNING!!!!  You must give the compiler enough of a hint in order for it
	   // to pass the proper types for DataFields for find() and equal_range()
	   // for example, if you are searching based on a DataField that's a tstring,
	   // either pass in a tstring variable or a tstring constant:
	   // examples:  tstring str;
	   //			 find(str);
	   //			 find(tstring(_TEXT("Foo")));
	   //			 // NOT find(_TEXT("Foo"));
	   // you also need to use similar conversions for numerics
	   // This caveat is because the compiler isn't performing
	   // implicit conversions between types due to the use of templates

	   // *** equal_range() functions ***

	   // the versions of equal_range() below will work for most keys
	   // as they are composed usually of no more than 3 fields
	   // for keys that are composed of more than 3 fields, you must use one of the above
	   // versions that take a DataObj representing the key

	   // return an iterator to objects that have the given value
	   // based on a key composed of a single field
	   // (that of the first key field listed in the orderOfFields for the DBIndex)
	   // (INDEXNM = df)


	   // same as the single DataField version, but based on two key fields
	   // (a two-field key)
	   // (INDEXNM.DF1 = df1 AND INDEXNM.DF2 = df2)
	   template<class DataField1, class DataField2>
		   indexed_pair
		   equal_range_AK(const tstring &IndexNm, const DataField1 &df1, const DataField2 &df2) const
	   {
		  IndexedDBView<View, IdxContainer, HashOrNoHash> *this_ptr = const_cast<IndexedDBView<View, IdxContainer, HashOrNoHash> *>(this);

		  this_ptr->fetch();

		  // return sp(indexes[IndexNm].equal_range(df1, df2));

		  typename index_map::iterator map_it = this_ptr->indexes.find(IndexNm);

		  // should always be the case that IndexNm exists
		  if (map_it != this_ptr->indexes.end())
		  {
			return sp((*map_it).second.equal_range(df1, df2));
		  }
		  else // if index not found, return PK's end() iterator to stick to conventions
		  {
			return sp(this_ptr->end());
		  }
	   }

	   // same as the single DataField version, but based on three key fields
	   // (a three-field key)
	   // (INDEXNM.DF1 = df1 AND INDEXNM.DF2 = df2 AND INDEXNM.DF3 = df3)
	   template<class DataField1, class DataField2, class DataField3>
		   indexed_pair
		   equal_range_AK(const tstring &IndexNm, const DataField1 &df1,
						  const DataField2 &df2, const DataField3 &df3) const
	   {
		  IndexedDBView<View, IdxContainer, HashOrNoHash> *this_ptr = const_cast<IndexedDBView<View, IdxContainer, HashOrNoHash> *>(this);

		  this_ptr->fetch();

		  // return sp(indexes[IndexNm].equal_range(df1, df2, df3));

		  typename index_map::iterator map_it = this_ptr->indexes.find(IndexNm);

		  // should always be the case that IndexNm exists
		  if (map_it != this_ptr->indexes.end())
		  {
			return sp((*map_it).second.equal_range(df1, df2, df3));
		  }
		  else // if index not found, return PK's end() iterator to stick to conventions
		  {
			return sp(this_ptr->end());
		  }
	   }

	   // same as the single DataField version, but based on four key fields
	   // (a four-field key)
	   // (INDEXNM.DF1 = df1 AND INDEXNM.DF2 = df2 AND INDEXNM.DF3 = df3 AND INDEXNM.DF4 = df4)
	   template<class DataField1, class DataField2, class DataField3, class DataField4>
		   indexed_pair
		   equal_range_AK(const tstring &IndexNm, const DataField1 &df1,
						  const DataField2 &df2, const DataField3 &df3,
						  const DataField4 &df4) const
	   {
		  IndexedDBView<View, IdxContainer, HashOrNoHash> *this_ptr = const_cast<IndexedDBView<View, IdxContainer, HashOrNoHash> *>(this);

		  this_ptr->fetch();

		  // return sp(indexes[IndexNm].equal_range(df1, df2, df3, df4));

		  typename index_map::iterator map_it = this_ptr->indexes.find(IndexNm);

		  // should always be the case that IndexNm exists
		  if (map_it != this_ptr->indexes.end())
		  {
			return sp((*map_it).second.equal_range(df1, df2, df3, df4));
		  }
		  else // if index not found, return PK's end() iterator to stick to conventions
		  {
			return sp(this_ptr->end());
		  }
	   }

       // templated versions of equal_range that use the first key that was passed in
	   // to the constructor to this indexed view
	   // (use key composed of single field)
	   // (FIRSTKEY = df)
	   template<class DataField> indexed_pair
		   equal_range(const DataField &df1) const
	   {
		  return equal_range_AK(orderOfKeys[0], df1);
	   }

	   // (uses key composed of two fields)
	   // (FIRSTKEY.DF1 = df1 AND FIRSTKEY.DF2 = df2)
	   template<class DataField1, class DataField2>
		   indexed_pair
		   equal_range(const DataField1 &df1, const DataField2 &df2) const
	   {
		   return equal_range_AK(orderOfKeys[0] , df1, df2);
	   }

   	   // (uses key composed of three fields)
	   // (FIRSTKEY.DF1 = df1 AND FIRSTKEY.DF2 = df2 AND FIRSTKEY.DF3 = df3)
	   template<class DataField1, class DataField2, class DataField3>
		   indexed_pair
		   equal_range(const DataField1 &df1, const DataField2 &df2,
					   const DataField3 &df3) const
	   {
		   return equal_range_AK(orderOfKeys[0] , df1, df2, df3);
	   }

	   // (uses key composed of four fields)
	   // (FIRSTKEY.DF1 = df1 AND FIRSTKEY.DF2 = df2 AND
	   //  FIRSTKEY.DF3 = df3 AND FIRSTKEY.DF4 = df4)
	   template<class DataField1, class DataField2, class DataField3, class DataField4>
		   indexed_pair
		   equal_range(const DataField1 &df1, const DataField2 &df2,
					   const DataField3 &df3, const DataField4 &df4) const
	   {
		   return equal_range_AK(orderOfKeys[0] , df1, df2, df3, df4);
	   }

	   // *** find() functions ***

	   // the versions find() below will work for most keys
	   // as they are composed usually of no more than 3 fields
	   // for keys that are composed of more than 3 fields, you must use one of the above
	   // versions that take a DataObj representing the key

	   // return an iterator to object that has the given value
	   // based on a key composed of a single field
	   // (that of the first key field listed in the orderOfFields for the DBIndex)
	   // (INDEXNM = df)

	   // same as the single DataField version, but based on two key fields
	   // (a two-field key)
	   // (INDEXNM.DF1 = df1 AND INDEXNM.DF2 = df2)
	   template<class DataField1, class DataField2>
		   indexed_iterator
		   find_AK(const tstring &IndexNm, const DataField1 &df1, const DataField2 &df2) const
	   {
		  IndexedDBView<View, IdxContainer, HashOrNoHash> *this_ptr = const_cast<IndexedDBView<View, IdxContainer, HashOrNoHash> *>(this);

		  this_ptr->fetch();

		  // return indexes[IndexNm].find(df1, df2);

		  typename index_map::iterator map_it = this_ptr->indexes.find(IndexNm);

		  // should always be the case that IndexNm exists
		  if (map_it != this_ptr->indexes.end())
		  {
			  return indexed_iterator((*map_it).second.find(df1, df2));
		  }
		  else // if index not found, return PK's end() iterator to stick to conventions
		  {
			  return this_ptr->end();
		  }
	   }

	   // same as the single DataField version, but based on three key fields
	   // (a three-field key)
	   // (INDEXNM.DF1 = df1 AND INDEXNM.DF2 = df2 AND INDEXNM.DF3 = df3)
	   template<class DataField1, class DataField2, class DataField3>
		   indexed_iterator
		   find_AK(const tstring &IndexNm, const DataField1 &df1,
				   const DataField2 &df2, const DataField3 &df3) const
	   {
		  IndexedDBView<View, IdxContainer, HashOrNoHash> *this_ptr = const_cast<IndexedDBView<View, IdxContainer, HashOrNoHash> *>(this);

		  this_ptr->fetch();

		  // return indexes[IndexNm].find(df1, df2, df3);

		  typename index_map::iterator map_it = this_ptr->indexes.find(IndexNm);

		  // should always be the case that IndexNm exists
		  if (map_it != this_ptr->indexes.end())
		  {
			  return indexed_iterator((*map_it).second.find(df1, df2, df3));
		  }
		  else // if index not found, return PK's end() iterator to stick to conventions
		  {
			  return this_ptr->end();
		  }
	   }

	   // same as the single DataField version, but based on four key fields
	   // (a four-field key)
	   // (INDEXNM.DF1 = df1 AND INDEXNM.DF2 = df2 AND INDEXNM.DF3 = df3
	   //  AND INDEXNM.DF4 = df4)
	   template<class DataField1, class DataField2, class DataField3, class DataField4>
		   indexed_iterator
		   find_AK(const tstring &IndexNm, const DataField1 &df1,
				   const DataField2 &df2, const DataField3 &df3,
				   const DataField4 &df4) const
	   {
		  IndexedDBView<View, IdxContainer, HashOrNoHash> *this_ptr = const_cast<IndexedDBView<View, IdxContainer, HashOrNoHash> *>(this);

		  this_ptr->fetch();

		  // return indexes[IndexNm].find(df1, df2, df3, df4);

		  typename index_map::iterator map_it = this_ptr->indexes.find(IndexNm);

		  // should always be the case that IndexNm exists
		  if (map_it != this_ptr->indexes.end())
		  {
			  return indexed_iterator((*map_it).second.find(df1, df2, df3, df4));
		  }
		  else // if index not found, return PK's end() iterator to stick to conventions
		  {
			  return this_ptr->end();
		  }
	   }
	   // templated versions of find that use the first key that was passed in
	   // to the constructor to this indexed view
	   // (use key composed of single field)
	   // (FIRSTKEY = df)
	   template<class DataField> indexed_iterator
		   find(const DataField &df1) const
	   {
			  return find_AK(orderOfKeys[0], df1);

	   }

	   // (uses key composed of two fields)
	   // (FIRSTKEY.DF1 = df1 AND FIRSTKEY.DF2 = df2)
	   template<class DataField1, class DataField2>
		   indexed_iterator
		   find(const DataField1 &df1, const DataField2 &df2) const
	   {
		   return find_AK(orderOfKeys[0], df1, df2);
	   }

	   // (uses key composed of three fields)
	   // (FIRSTKEY.DF1 = df1 AND FIRSTKEY.DF2 = df2 AND FIRSTKEY.DF3 = df3)
	   template<class DataField1, class DataField2, class DataField3>
		   indexed_iterator
		   find(const DataField1 &df1, const DataField2 &df2, const DataField3 &df3) const
	   {
		   return find_AK(orderOfKeys[0], df1, df2, df3);
	   }

	   // (uses key composed of four fields)
	   // (FIRSTKEY.DF1 = df1 AND FIRSTKEY.DF2 = df2 AND FIRSTKEY.DF3 = df3
	   //  AND FIRSTKEY.DF4 = df4)
	   template<class DataField1, class DataField2, class DataField3, class DataField4>
		   indexed_iterator
		   find(const DataField1 &df1, const DataField2 &df2, const DataField3 &df3,
				const DataField4 &df4) const
	   {
		   return find_AK(orderOfKeys[0], df1, df2, df3, df4);
	   }

//-- equal_range_AK ---------------------

     template<class DataField> indexed_pair
		   equal_range_AK(const tstring &IndexNm, const DataField &df) const
	   {
		  IndexedDBView<View, IdxContainer, HashOrNoHash> *this_ptr = const_cast<IndexedDBView<View, IdxContainer, HashOrNoHash> *>(this);

		  this_ptr->fetch();

		  // return sp(indexes[IndexNm].equal_range(df));

		  typename index_map::iterator map_it = this_ptr->indexes.find(IndexNm);

		  // should always be the case that IndexNm exists
		  if (map_it != this_ptr->indexes.end())
		  {
			return sp((*map_it).second.equal_range(df));
		  }
		  else // if index not found, return PK's end() iterator to stick to conventions
		  {
			return sp(this_ptr->end());
		  }
	   }

	   // return iterator to the DataObj * that has key IndexNm = key
	   // equal_range() needs to return a STD_::pair of iterators, one to march through the
	   // found elements and the other to return the iterator marking the end of the
	   // list for this particular index
	   indexed_pair
			equal_range_AK(const tstring &IndexNm, const DataObj &key) const
	   {
		    IndexedDBView<View, IdxContainer, HashOrNoHash> *this_ptr = const_cast<IndexedDBView<View, IdxContainer, HashOrNoHash> *>(this);

		    this_ptr->fetch();

			// return sp(indexes[IndexNm].equal_range(key));

		    typename index_map::iterator map_it = this_ptr->indexes.find(IndexNm);

		    // should always be the case that IndexNm exists
		    if (map_it != this_ptr->indexes.end())
			{
			  return sp((*map_it).second.equal_range(key));
			}
			else // if index not found, return PK's end() iterator to stick to conventions
			{
			  return sp(this_ptr->end());
			}
	   }


//-- find_AK ---------------------
	   template<class DataField> indexed_iterator
		   find_AK(const tstring &IndexNm, const DataField &df) const
	   {
		  IndexedDBView<View, IdxContainer, HashOrNoHash> *this_ptr = const_cast<IndexedDBView<View, IdxContainer, HashOrNoHash> *>(this);

		  this_ptr->fetch();

		  // return indexes[IndexNm].find(df);


		  typename index_map::iterator map_it = this_ptr->indexes.find(IndexNm);

		  // should always be the case that IndexNm exists
		  if (map_it != this_ptr->indexes.end())
		  {
			  return indexed_iterator((*map_it).second.find(df));
		  }
		  else // if index not found, return PK's end() iterator to stick to conventions
		  {
			  return this_ptr->end();
		  }
	   }

	   // return iterator to the DataObj * that has key IndexNm = key
	   // find() returns an iterator to an object meeting the filter criteria
	   indexed_iterator
			find_AK(const tstring &IndexNm, const DataObj &key) const
	   {
		  IndexedDBView<View, IdxContainer, HashOrNoHash> *this_ptr = const_cast<IndexedDBView<View, IdxContainer, HashOrNoHash> *>(this);

		  this_ptr->fetch();

		  // return indexes[IndexNm].find(key);

		  typename index_map::iterator map_it = this_ptr->indexes.find(IndexNm);

		  // should always be the case that IndexNm exists
		  if (map_it != this_ptr->indexes.end())
		  {
			  return indexed_iterator((*map_it).second.find(key));
		  }
		  else // if index not found, return PK's end() iterator to stick to conventions
		  {
			  return this_ptr->end();
		  }
	   }


	   // *** insert() and erase() methods ***

	   // complete with BOUND/UNBOUND, USE_PK_FIELDS_ONLY/USE_ALL_FIELDS semantics
	   // each of the base insert() and erase() methods does the following:
	   // 1. Sees if the operation is valid locally to the IndexedDBView structure itself
	   // 2. If in BOUND mode, tries to apply the operation to the DB
	   // 3. If #2 successful or in UNBOUND mode, applies the operation to the
	   //	 IndexedDBView structure.

	   // *** base insert() method ***
	   // attempts to insert dataObj into the IndexedDBView
	   // returns a STD_::pair:
	   // STD_::pair::first: iterator to the newly inserted object if successful
	   // if object already in the view locally, returns an iterator
	   // to the already existing element in the view
	   // STD_::pair::second: whether the insertion was successful or not

	   // to provide atomicity, actually written to do local_insert() first,
	   // then DB insert
	   STD_::pair<iterator, bool> insert(const DataObj &dataObj)
	   {
		   fetch();


		   	// we can update any field whether or not it is in the index
			BoundIOs bcabios;

    		DataObj rowbuf;
			bcabios.BindAsBase(rowbuf);
			bca(bcabios, rowbuf);

   		    // remove each auto key field from the insert for the view
		    // (assumes that each autokey field is *NOT* a regular field in the table)

		     try
		   {
			 STD_::vector<tstring> keyNames = GetAppendedFieldNames(bca);

		      for (STD_::vector<tstring>::iterator erase_it = keyNames.begin();
						erase_it != keyNames.end(); ++erase_it)
				{
				  bcabios.erase(*erase_it);
				}

			   bcabios.FixColNos();
		   }
		   catch (VariantException &)
		   {
				// do nothing for static views
		   }


		   // don't include AutoKey fields in the insert statement
		   if (useWhichFields == USE_AUTO_KEY)
		   {
			   typename index_map::iterator map_it =
				   indexes.find(_TEXT("AutoKey"));

			   if (map_it == indexes.end())
			   {
					DTL_THROW DBException(_TEXT("IndexedDBView::insert()"),
						_TEXT("Couldn't find AutoKey for the view!"),
						NULL, NULL);
			   }
		   }

		   // validating DataObj now will catch errors more quickly
		   // note that some errors will still get by this preliminary
		   // InsValidate check ... don't worry as the rest will still be
		   // caught by DBView::InsValidate()

		   DataObj finaldataObj(dataObj); // needed in case dataObj gets changed
										  // by InsValidate()
										  // otherwise, changes in dataObj there
										  // won't propagate properly to
										  // the object we insert in the view

		   if (!InsValidate(finaldataObj))
				DTL_THROW DBException(_TEXT("IndexedDBView::insert(const DataObj &dataObj)"),
					_TEXT("InsValidate() failed!"), NULL, NULL);

		   // STD_::pair<iterator, bool> find_pr = CheckUniqueness(dataObj);

		   // if element found, insertion failed
		   // just return the STD_::pair back up to caller
		   // if (!find_pr.second)
		   //  return find_pr;

		   // first do local_insert() ... if fails, we don't touch the object state at all
		   STD_::pair<iterator, bool> ins_pr = local_insert(finaldataObj);

		   // if insertion failed, return ins_pr
		   if (!ins_pr.second)
			   return ins_pr;

		   iterator it = ins_pr.first;

		   // if in BOUND mode, try to insert into DB
		   // insertion will throw if the remote insertion fails
		   // (for example, if uniqueness constraints are violated on the DB)
		   try
		   {
			   if (boundToDB == BOUND)
			   {
				  DBView<DataObj, ParamObj> view(MakeDelimitedList(pDBview->GetTableNames()), DynamicBCA<DataObj, BCA>(bcabios,  bca),
			         _TEXT(""),
				     EmptyBPA<ParamObj>(), pDBview->GetSelVal(),
					 NoInsValidate<DataObj>(),
					 pDBview->get_io_handler(),
					 pDBview->GetConnection(), MakeDelimitedList(pDBview->GetKey()),
					 useWhichFields);

				  // bPrepare = false as stmt. only executed once
				  typename DBView<DataObj, ParamObj>::insert_iterator ins_it(view, false );

				  *ins_it = finaldataObj;
			      ++ins_it;

				  // if iterator encountered a problem, that means insertion failed, so throw
				  if (ins_it.bad() || ins_it.fail())
					  DTL_THROW DBException(_TEXT("IndexedDBView::insert()"), _TEXT("Insertion into DB failed!"),
							NULL, NULL);
			   }
		   }

		   catch (...)
		   {
			   // must rollback local state, then rethrow the exception!
			   local_erase(it);
			   throw;
		   }

		   // if successful so far, insert into the IndexedDBView structure
		   // uniqueness constraints should be guaranteed here
		   return ins_pr;
	   }

	   // *** base erase() method ***
	   // attempts to remove the DataObj pointed to by the iterator passed in
	   // 1.  If in BOUND mode, tries to delete the object from the DB
	   // 2.  Remove the element from each of the indexes.
	   // 3.  Erase the element from the owned list<DataObj> ... data
	   // Note: This method assumes the iterator passed in is a valid
	   // iterator pointing into the container

	   // atomicity guaranteed: database erase succeeds or fails; steps 2 and 3
	   // are nothrow due to STL erase() guarantees
	   void erase(iterator it)
	   {
		   fetch();

		   // if in BOUND mode, try to delete object from DB
		   if (boundToDB == BOUND)
		   {
			   BoundIOs tmpbios;

			   // get a copy of the proper BoundIOs
			   switch (useWhichFields)
			   {
			     case USE_PK_FIELDS_ONLY:
					{
					   // get BoundIOs representing PK only
				       tmpbios = ExtractPK();
					   break;
					}
				 case USE_ALL_FIELDS:
					{
					   // get BoundIOs representing all fields
					   DataObj rowbuf;
					   tmpbios.BindAsBase(rowbuf);
					   bca(tmpbios, rowbuf);
					   break;
					}
				 case USE_AUTO_KEY:
					{
					   // get BoundIOs representing the autokey
					   tmpbios = ExtractAutoKey();
					   break;
					}
			   }

			   // note: as the below view does not have a postfix clause,
			   // extra records might get deleted then what the user might expect
			   // (except in autokey case where only the requested records should get
			   //  wiped from the face of the earth!)
			   DBView<DataObj, ParamObj> view(MakeDelimitedList(pDBview->GetTableNames()),
				   DynamicBCA<DataObj, BCA>(tmpbios, bca), _TEXT(""),
				   EmptyBPA<ParamObj>(), pDBview->GetSelVal(), pDBview->GetInsVal(),
				   pDBview->get_io_handler(),
				   pDBview->GetConnection());

			   // bPrepare = false as stmt. only executed once
			   typename DBView<DataObj, ParamObj>::delete_iterator del_it(view, false);

			   *del_it = *it;
   			   ++del_it;

			   // if iterator encountered a problem, that means deletion failed, so throw
			   if (del_it.bad() || del_it.fail() || del_it.GetLastCount() == 0)
					  DTL_THROW DBException(_TEXT("IndexedDBView::erase()"),
					  _TEXT("Delete from DB failed! Record may have been changed by another user."),
							NULL, NULL);
		   }

		  // steps 2 and 3 taken care of by local_erase()
		  local_erase(it);
	   }

       // *** base replace() method ***
	   // attempts to replace the object pointed to by the iterator it with
	   // a new set of data as specified by dataObj into the IndexedDBView
	   // returns a STD_::pair:
	   // STD_::pair::first: iterator to the newly replaced object if successful
	   // if object already in the view locally, returns an iterator
	   // to the already existing element in the view
	   // STD_::pair::second: whether the insertion was successful or not

       // to gain atomicity we will do the following:
	   // 1. Mark elements for deletion in the index lists
	   //    that match it based on comparing &(*it)
	   // 2. CheckUniqueness() - find() on new item (dataObj), if anything comes back
	   //    other than old item or not found, fails uniqueness check
	   // 3. local_insert(dataObj) in try/catch
	   // 4. DB Update in try/catch ... rollback local_insert() if failed
	   // 5. local_erase() on all marked elements from step 1
	   STD_::pair<iterator, bool> replace(iterator it, const DataObj &dataObj)
	   {
		  fetch();

		  // try the update in the DB

		  // TODO: two tricky things need to be done to make replace work efficiently
		  // 1.  BCA should be just the list of changed columns between *it and dataObj
		  //  so that only the minimum set of changes are send to the database.
		  //  The changed columns should be found by doing comparisons using BoundIOs.
		  //
		  // 2.  A where clause needs to be setup along with a BCA functor to specify
		  //  which rows in the database are to be replaced.  If the mode is
		  //  USE_PK_ONLY      --> WHERE db_field1 = key_field1 AND db_field2 = key_field2 AND...
		  //  USE_ALL_FIELDS   --> WHERE db_field1 = field1 AND ...(use all fields here)
		  //  USE_AUTO_KEY	   --> WHERE autokey_field1 = (?) and autokey_field2 = (?)

		  // use pImpl so we can do the work to create the view only if
		  // in bound mode
		  CountedPtr<DBView<DataObj, DataObj> > pView = NULL;
		  typename DBView<DataObj, DataObj>::update_iterator up_it;


		  // validating DataObj now will catch errors more quickly
		  // note that some errors will still get by this preliminary
		  // InsValidate check ... don't worry as the rest will still be
		  // caught by DBView::InsValidate()

		  DataObj finaldataObj(dataObj);  // needed in case dataObj gets changed
										  // by InsValidate()
										  // otherwise, changes in dataObj there
										  // won't propagate properly to
										  // the object we insert in the view

		  if (!InsValidate(finaldataObj))
			   DTL_THROW DBException(_TEXT("IndexedDBView::replace()"),
					_TEXT("InsValidate() failed!"), NULL, NULL);

		  if (boundToDB == BOUND)
		  {
			BoundIOs bpabios;  // used for BPA

			// get a copy of the proper BoundIOs
			switch (useWhichFields)
			{
			     case USE_PK_FIELDS_ONLY:
					 {
					   // get BoundIOs representing PK only
				       bpabios = ExtractPK();
					   break;
					 }
				 case USE_ALL_FIELDS: // get all BoundIOs
					 {
					   // get BoundIOs representing all fields
					   DataObj rowbuf;
					   bpabios.BindAsBase(rowbuf);
					   bca(bpabios, rowbuf);

					   break;
					 }
				 case USE_AUTO_KEY:
					 {
						// get BoundIOs representing autokey
						bpabios = ExtractAutoKey();
						break;
					 }
			}

			// we can update any field whether or not it is in the index
			BoundIOs bcabios;

    		DataObj rowbuf;
			bcabios.BindAsBase(rowbuf);
			bca(bcabios, rowbuf);

   			// bcabios.ComputeAbsFromRel(BoundIO::BIND_AS_COL);

         // TODO - this probably doesn't work with NULLs.
         // Extend test_nullable.cpp
			bcabios = bcabios.ChangedFields(*it, bcabios, dataObj);

			// if no fields tied to the BoundIOs changed,
			// then *it == dataObj as far as the replace() is concerned
			// so just return

			if (bcabios.empty())
			{
				return STD_::pair<iterator, bool>(it, true);
			}

			// remove each auto key field from the insert for the view
		    // (assumes that each autokey field is *NOT* a regular field in the table)

		     try
		   {
			 STD_::vector<tstring> keyNames = GetAppendedFieldNames(bca);

		      for (STD_::vector<tstring>::iterator erase_it = keyNames.begin();
						erase_it != keyNames.end(); ++erase_it)
				{
				  bcabios.erase(*erase_it);
				}

			   bcabios.FixColNos();
		   }
		   catch (VariantException &)
		   {
				// do nothing for static views
		   }

			STD_::pair<tstring, STD_::vector<tstring> > pr =
				MakePostfixForUpdate(bpabios, pDBview->GetConnection().GetQuoteChar(),
					(const DataObj *) NULL);

			tstring postfix = pr.first;
			STD_::vector<tstring> orderOfColumns = pr.second;


			// in order to set the IOHandler we have to convert the indexed view's
			// IOHandler<DataObj, ParamObj> to IOHandler<DataObj, DataObj>
			// monomorphize() does that for us

            const STD_::set<tstring> &tbls =   pDBview->GetTableNames();
            tstring test_me =   MakeDelimitedList(tbls);

			pView = new DBView<DataObj, DataObj>(MakeDelimitedList(pDBview->GetTableNames()),
					DynamicBCA<DataObj, BCA>(bcabios, bca), postfix,
					DynamicBCA<DataObj, BCA>(bpabios, bca, orderOfColumns), pDBview->GetSelVal(), NoInsValidate<DataObj>(),
					monomorphize(pDBview->get_io_handler()),
					pDBview->GetConnection(),
					MakeDelimitedList(pDBview->GetKey()),
					useWhichFields);

			// bPrepare = false as stmt. only executed once
			up_it = SPEC_CASE_TYPENAME_IN_TEMPLATE_PARAMS DBView<DataObj, DataObj>::update_iterator(*pView, false);

		    up_it.Params(*it);  // search parameters are the object to replace

		  }

		  // *** Preliminary check for a primary key ***
		  typename index_map::iterator map_it = indexes.find(orderOfKeys[0]);

		  // shouldn't happen
		  if (map_it == indexes.end())
		  {
			  DTL_THROW DBException(_TEXT("IndexedDBView::erase()"),
					  _TEXT("View must have a PK declared!"), NULL, NULL);
		  }

		  // *** mark elements in index lists for deletion ***
		  // each index list should contain &(*it) exactly once
		  // if not, we know our data structure is corrupted and we should throw
          // throwing is OK here as we're still atomic
		  // DataObj *addr = &(*it); // get address of DataObj referred to by it

		  STD_::list<iterator> condemned;// iterators to elements in index lists to erase
								   // the delete markers are stored in order of the indexes
								   // as we iterate over them
								   // *** You are hereby condemned to die at the hands ***
								   // *** of the Coding Gods, you mere pointers!!!!!   ***

		  // mark the element in each index list
		  for (map_it = indexes.begin(); map_it != indexes.end(); map_it++)
		  {
			 IndexType &index = (*map_it).second;

			 typename IndexType::iterator idx_it = index.find_by_addr(*it);

			 // if element not found, throw as we've detected that the indexed view
			 // is in a corrupted state ... still atomic here as no changes made to
			 // container or the objects it owns
			 if (idx_it == index.end())
				 DTL_THROW DBException(_TEXT("IndexedDBView::replace()"),
				     _TEXT("Indexed view is corrupted due to bad index list!"), NULL, NULL);

			 // record the iterator in the list of the damned
			 condemned.push_back(iterator(idx_it));
		  }

		  // *** Check uniqueness of new dataObj ***

		  // if anything comes back other than the old item or not found on any unique index,
		  // uniqueness check violated!


		  // Check replace before performing to make sure it does not
		  // violate uniqueness
		  // STD_::pair<iterator, bool> find_pr = CheckUniqueness(dataObj, it);

		  // if element found, check to see if dataObj != it
		  // if so, replace will violate uniqueness
		  // in this case return the STD_::pair back up to caller
		  // if (!find_pr.second)
		  //   return find_pr;

		  // *** local_insert() of dataObj ***

		  // atomicity maintained as local_insert() itself is atomic and
		  // this is our first mutating operation attempted on the indexed view
		  // no need for try/catch here as all the semantics for atomicity
		  // are in local_insert()
		  // do NOT use the built in checks for uniqueness in local_insert()
		  // bool value returned as ins_pr.second should always be true
		  STD_::pair<iterator, bool> ins_pr = local_insert(finaldataObj, it, false);


		  // if local_insert() call fails, so does replace()
		  if (!ins_pr.second)
			  return ins_pr;

		  // *** DB Update in try/catch block ***
		  // on failure we must rollback the local_insert() in order to maintain atomicity

		  // if in BOUND mode, try to insert into DB
		  // insertion will throw if the remote insertion fails
		  // (for example, if uniqueness constraints are violated on the DB)
		  if (boundToDB == BOUND)
		  {
			try
			{
#if 0
			   // debugging ... only use with types that support operator<<()
			   // using to debug DynamicIndexedViewExample()
			   tcout << _TEXT("In replace() ... updating: ") << STD_::endl;
			   tcout << *it << STD_::endl;
			   tcout << _TEXT("with: ") << STD_::endl;
			   tcout << finaldataObj << STD_::endl;
#endif
			   *up_it = finaldataObj;
			   ++up_it;

			   // if iterator encountered a problem, that means insertion failed, so throw
			   if (up_it.bad() || up_it.fail() || up_it.GetLastCount() == 0)
					  DTL_THROW DBException(_TEXT("IndexedDBView::replace()"),
					  _TEXT("Update in DB failed! Row may have been changed by another user."),
							NULL, NULL);
			}
			catch (...)
			{
			   // if update fails in DB, we must rollback the local_insert() with
			   // the appropriate call to local_erase()?  Then rethrow

		       // ins_pr.first is an iterator to the elements we inserted into the
			   // indexed view ... so atomicity is maintained by erasing those elements
			   local_erase(ins_pr.first);
			   throw;
			}

		  }

		  typename STD_::list<iterator>::iterator condemned_it;

		  // *** now must erase all marked (damned) elements ***

		  // guaranteed to succeed as erase() is nothrow ... atomicity holds
		  // remember each iterator to a condemned element is properly matched
		  // with its index because we built the list of the damned in the same order
		  for (map_it = indexes.begin(), condemned_it = condemned.begin();
		           map_it != indexes.end(); map_it++, condemned_it++)
		  {
			  IndexType &index = (*map_it).second;
			  index.erase(condemned_it->base());// God has spoken, condemned pointer!
		  }

		  // return the STD_::pair from the local_insert() as the result
		  return ins_pr;

	   }

	   // *** composite insert(), erase(), replace() methods ***

	   // insert with hint just ignores the hint
	   iterator insert(iterator pos, const DataObj &dataObj)
	   {
		  return insert(dataObj).first;
	   }

	   // range insert ... each insert guaranteed to be atomic, but not the whole range
	   template<class InputIterator> void insert(InputIterator first, InputIterator last)
	   {
		  while (first != last)
			 insert(*first++);
	   }

	   // erase all elements in a given range
	   void erase(iterator first, iterator last)
	   {

		  while (first != last)
			 erase(first++);
	   }

	   // erase all elements with the given value
	   void erase(const DataObj &dataObj)
	   {
		  STD_::pair<iterator, iterator> pr = equal_range(dataObj);
		  erase(pr.first, pr.second);
	   }

	   // applies to PK as the container itself is indexed on DataObj's
	   // based on the PK... returned iterator points to first DataObj in the list

	   // ************* begin() and end() iterators ********************

	   // applies to PK as the container itself is indexed on DataObj's
	   // based on the PK... returned iterator points to first DataObj in the list

	   const_iterator begin() const
	   {
			return begin_AK(orderOfKeys[0]);
	   }

	   const_iterator end() const
	   {
			return end_AK(orderOfKeys[0]);
	   }

	   iterator begin()
	   {
			return begin_AK(orderOfKeys[0]);
	   }

	   iterator end()
	   {
			return end_AK(orderOfKeys[0]);
	   }

	   // begin_AK() returns the begin() iterator for the DBIndex with the given name
	   iterator begin_AK(const tstring &IndexNm)
	   {
		  fetch();

		  typename index_map::iterator map_it = indexes.find(IndexNm);

		  iterator result;

		  // should always be the case that IndexNm exists
		  if (map_it != indexes.end())
		  {
			  result = iterator((*map_it).second.begin());
		  }
		  else // if index not found, throw
		  {
			  DTL_THROW DBException(_TEXT("IndexedDBView::begin_AK()"),
				  _TEXT("Index ") + IndexNm + _TEXT(" doesn't exist!"), NULL, NULL);
		  }

		  return result;

	   }

	   // const version of begin_AK()
	   const_iterator begin_AK(const tstring &IndexNm) const
	   {
		  IndexedDBView<View, IdxContainer, HashOrNoHash> *this_ptr = const_cast<IndexedDBView<View, IdxContainer, HashOrNoHash> *>(this);

		  this_ptr->fetch();

		  typename index_map::iterator map_it = this_ptr->indexes.find(IndexNm);

		  const_iterator result;

		  // should always be the case that IndexNm exists
		  if (map_it != this_ptr->indexes.end())
		  {
			  result = const_iterator((*map_it).second.begin());
		  }
		  else // if index not found, throw
		  {
			  DTL_THROW DBException(_TEXT("IndexedDBView::begin_AK()"),
				  _TEXT("Index ") + IndexNm + _TEXT(" doesn't exist!"), NULL, NULL);
		  }

		  return result;

	   }

	   // end_AK() returns the end() iterator for the DBIndex with the given name
	   iterator end_AK(const tstring &IndexNm)
	   {
		  fetch();

		  typename index_map::iterator map_it = indexes.find(IndexNm);

		  iterator result;

		  // should always be the case that IndexNm exists
		  if (map_it != indexes.end())
		  {
			  result = iterator((*map_it).second.end());
		  }
		  else // if index not found, throw
		  {
			  DTL_THROW DBException(_TEXT("IndexedDBView::end_AK()"),
				  _TEXT("Index ") + IndexNm + _TEXT(" doesn't exist!"), NULL, NULL);
		  }

		  return result;

	   }

	   // const version of end_AK()
	   const_iterator end_AK(const tstring &IndexNm) const
	   {
		  IndexedDBView<View, IdxContainer, HashOrNoHash> *this_ptr = const_cast<IndexedDBView<View, IdxContainer, HashOrNoHash> *>(this);

		  this_ptr->fetch();

		  typename index_map::iterator map_it = this_ptr->indexes.find(IndexNm);

		  const_iterator result;

		  // should always be the case that IndexNm exists
		  if (map_it != this_ptr->indexes.end())
		  {
			  result = const_iterator((*map_it).second.end());
		  }
		  else // if index not found, throw
		  {
			  DTL_THROW DBException(_TEXT("IndexedDBView::end_AK()"),
				  _TEXT("Index ") + IndexNm + _TEXT(" doesn't exist!"), NULL, NULL);
		  }

		  return result;
	   }

// hashed containers will crap out on iterator::operator--()
#ifndef DTL_USE_HASH_CONTAINER
	   // ************** rbegin() and rend() ***************
	   // reverse iterators ... applies to PK

	   reverse_iterator rbegin() { return reverse_iterator(end());   }
	   reverse_iterator rend()   { return reverse_iterator(begin()); }

	   const_reverse_iterator rbegin() const
	   {
			return const_reverse_iterator(end());
	   }

	   const_reverse_iterator rend() const
	   {
			return const_reverse_iterator(begin());
	   }

	   reverse_iterator rbegin_AK(const tstring &IndexNm)
	   {
			return reverse_iterator(end_AK(IndexNm));
	   }

	   reverse_iterator rend_AK(const tstring &IndexNm)
	   {
			return reverse_iterator(begin_AK(IndexNm));
	   }

	   const_reverse_iterator rbegin_AK(const tstring &IndexNm) const
	   {
			return const_reverse_iterator(end_AK(IndexNm));
	   }

	   const_reverse_iterator rend_AK(const tstring &IndexNm) const
	   {
			return const_reverse_iterator(begin_AK(IndexNm));
	   }
#endif

	   //

	   bool empty() const
	   {
			const_cast<IndexedDBView *>(this)->fetch();
			// return indexes[orderOfKeys[0]].empty();

			typename index_map::iterator map_it = indexes.find(orderOfKeys[0]);

		    // should always be the case that IndexNm exists
		    if (map_it != indexes.end())
			{
			  return (*map_it).second.empty();
			}
			else // PK should exist, so throw if it doesn't
			{
			  DTL_THROW DBException(_TEXT("IndexedDBView::empty()"), _TEXT("PK doesn't exist for this view!"),
				  NULL, NULL);
			}
	   }

	   size_type size() const
	   {
	    	const_cast<IndexedDBView *>(this)->fetch();
			// return indexes[orderOfKeys[0]].size();

			typename index_map::iterator map_it = indexes.find(orderOfKeys[0]);

		    // should always be the case that IndexNm exists
		    if (map_it != indexes.end())
			{
			  return (*map_it).second.size();
			}
			else // PK should exist, so throw if it doesn't
			{
			  DTL_THROW DBException(_TEXT("IndexedDBView::size()"), _TEXT("PK doesn't exist for this view!"),
				  NULL, NULL);

			  return 0;
			}
	   }

	   size_type max_size() const
	   {
			const_cast<IndexedDBView *>(this)->fetch();
			// return indexes[orderOfKeys[0]].max_size();

			typename index_map::iterator map_it = indexes.find(orderOfKeys[0]);

		    // should always be the case that IndexNm exists
		    if (map_it != indexes.end())
			{
			  return (*map_it).second.max_size();
			}
			else // PK should exist, so throw if it doesn't
			{
			  DTL_THROW DBException(_TEXT("IndexedDBView::max_size()"), _TEXT("PK doesn't exist for this view!"),
				  NULL, NULL);
			}
	   }

	   // returns the number of elements whose key is dataObj based on the Primary Key
	   size_t count(const DataObj &dataObj) const
	   {
			return count_AK(orderOfKeys[0], dataObj);
	   }

	   // same, but for an alternate key
	   size_t count_AK(const tstring &IndexNm, const DataObj &dataObj) const
	   {
			STD_::pair<iterator, iterator> toCount = equal_range_AK(IndexNm, dataObj);

			size_t result = 0;

			for (iterator it = toCount.first; it != toCount.second; ++it)
				++result;

			return result;

	   }

	   // erase all elements - probably don't want to support erase of
	   // individual elems as this will be too hairy & slow
	   void clear()
	   {
		   for (typename index_map::iterator erase_it = indexes.begin(); erase_it != indexes.end(); ++erase_it)
		   {
				erase_it->second.clear();
		   }
			indexes.clear();
		   data.clear();
			bFetched = false;
	   }

	   // lexicographical compare .. based on PK
	   // indexed views can't be const due to the potential need to fetch()
	   friend bool operator==
           (const IndexedDBView<View, IdxContainer, HashOrNoHash> &indexview1,
			const IndexedDBView<View, IdxContainer, HashOrNoHash> &indexview2)
	   {
		    IndexedDBView<View, IdxContainer, HashOrNoHash> &idxview1 =
				const_cast<IndexedDBView<View, IdxContainer, HashOrNoHash> &>(indexview1);
		    IndexedDBView<View, IdxContainer, HashOrNoHash> &idxview2 =
				const_cast<IndexedDBView<View, IdxContainer, HashOrNoHash> &>(indexview2);

		    idxview1.fetch();
		    idxview2.fetch();

			// return idxview1.indexes[orderOfKeys] == idxview2.indexes[orderOfKeys[0]];
#ifndef __BORLANDC__
		    typename index_map::iterator map_it1 = idxview1.indexes.find(idxview1.orderOfKeys[0]);
		    typename index_map::iterator map_it2 = idxview2.indexes.find(idxview2.orderOfKeys[0]);
#else
			index_map::iterator map_it1 = idxview1.indexes.find(idxview1.orderOfKeys[0]);
		    index_map::iterator map_it2 = idxview2.indexes.find(idxview2.orderOfKeys[0]);
#endif

		    // make sure PK's exist for both views
		    if (map_it1 == idxview1.indexes.end())
			{
			  DTL_THROW DBException(_TEXT("IndexedDBView::operator==()"),
				  _TEXT("PK doesn't exist for first view!"), NULL, NULL);
			}
			else if (map_it2 == idxview2.indexes.end())			{
			  DTL_THROW DBException(_TEXT("IndexedDBView::operator==()"),
				  _TEXT("PK doesn't exist for second view!"),
				  NULL, NULL);
			}
		    return (*map_it1).second == (*map_it2).second;
	   }

	   // though not part of the standard, operator!=() is added for convenience
	   friend bool operator!=
	       (const IndexedDBView<View, IdxContainer, HashOrNoHash> &indexview1,
			const IndexedDBView<View, IdxContainer, HashOrNoHash> &indexview2)
	   {
		 return !(indexview1 == indexview2);
	   }

#if 0
	   friend bool operator<(const IndexedDBView<View, IdxContainer, HashOrNoHash> &indexview1,
			const IndexedDBView<View, IdxContainer, HashOrNoHash> &indexview2)
	   {
		   IndexedDBView<View, IdxContainer, HashOrNoHash> &idxview1 =
				const_cast<IndexedDBView<View, IdxContainer, HashOrNoHash> &>(indexview1);
		   IndexedDBView<View, IdxContainer, HashOrNoHash> &idxview2 =
				const_cast<IndexedDBView<View, IdxContainer, HashOrNoHash> &>(indexview2);

		   idxview1.fetch();
		   idxview2.fetch();

		   // return idxview1.indexes[orderOfKeys[0]] < idxview2.indexes[orderOfKeys[0]];

		   index_map::iterator map_it1 = idxview1.indexes.find(idxview1.orderOfKeys[0]);
		   index_map::iterator map_it2 = idxview2.indexes.find(idxview2.orderOfKeys[0]);

		   // make sure PK's exist for both views
		   if (map_it1 == idxview1.indexes.end())
		   {
			  DTL_THROW DBException(_TEXT("IndexedDBView::operator<()"),
				  _TEXT("PK doesn't exist for first view!"), NULL, NULL);
		   }
		   else if (map_it2 == idxview2.indexes.end())			{
			  DTL_THROW DBException(_TEXT("IndexedDBView::operator<()"),
				  _TEXT("PK doesn't exist for second view!"),
				  NULL, NULL);
		   }
		   return (*map_it1).second < (*map_it2).second;
	   }

	   friend bool operator>(const IndexedDBView<View, IdxContainer, HashOrNoHash> &indexview1,
			const IndexedDBView<View, IdxContainer, HashOrNoHash> &indexview2)
	   {
		   IndexedDBView<View, IdxContainer, HashOrNoHash> &idxview1 =
				const_cast<IndexedDBView<View, IdxContainer, HashOrNoHash> &>(indexview1);
		   IndexedDBView<View, IdxContainer, HashOrNoHash> &idxview2 =
				const_cast<IndexedDBView<View, IdxContainer, HashOrNoHash> &>(indexview2);

		   idxview1.fetch();
		   idxview2.fetch();

		   // return idxview1.indexes[orderOfKeys[0]] > idxview2.indexes[orderOfKeys[0]];

		   index_map::iterator map_it1 = idxview1.indexes.find(idxview1.orderOfKeys[0]);
		   index_map::iterator map_it2 = idxview2.indexes.find(idxview2.orderOfKeys[0]);

     	   // make sure PK's exist for both views
		   if (map_it1 == idxview1.indexes.end())
		   {
			  DTL_THROW DBException(_TEXT("IndexedDBView::operator>()"),
				  _TEXT("PK doesn't exist for first view!"), NULL, NULL);
		   }
		   else if (map_it2 == idxview2.indexes.end())			{
			  DTL_THROW DBException(_TEXT("IndexedDBView::operator>()"),
				  _TEXT("PK doesn't exist for second view!"),
				  NULL, NULL);
		   }
		   return (*map_it1).second > (*map_it2).second;
	   }

	   friend bool operator<=(const IndexedDBView<View, IdxContainer, HashOrNoHash> &indexview1,
			const IndexedDBView<View, IdxContainer, HashOrNoHash> &indexview2)
	   {
		   IndexedDBView<View, IdxContainer, HashOrNoHash> &idxview1 =
				const_cast<IndexedDBView<View, IdxContainer, HashOrNoHash> &>(indexview1);
		   IndexedDBView<View, IdxContainer, HashOrNoHash> &idxview2 =
				const_cast<IndexedDBView<View, IdxContainer, HashOrNoHash> &>(indexview2);

		   idxview1.fetch();
		   idxview2.fetch();

		   // return idxview1.indexes[orderOfKeys[0]] <= idxview2.indexes[orderOfKeys[0]];

		   index_map::iterator map_it1 = idxview1.indexes.find(idxview1.orderOfKeys[0]);
		   index_map::iterator map_it2 = idxview2.indexes.find(idxview2.orderOfKeys[0]);

		   // make sure PK's exist for both views
		   if (map_it1 == idxview1.indexes.end())
		   {
			  DTL_THROW DBException(_TEXT("IndexedDBView::operator<=()"),
				  _TEXT("PK doesn't exist for first view!"), NULL, NULL);
		   }
		   else if (map_it2 == idxview2.indexes.end())			{
			  DTL_THROW DBException(_TEXT("IndexedDBView::operator<=()"),
				  _TEXT("PK doesn't exist for second view!"),
				  NULL, NULL);
		   }
		   return (*map_it1).second <= (*map_it2).second;
	   }

	   friend bool operator>=(const IndexedDBView<View, IdxContainer, HashOrNoHash> &indexview1,
			const IndexedDBView<View, IdxContainer, HashOrNoHash> &indexview2)
	   {
		   IndexedDBView<View, IdxContainer, HashOrNoHash> &idxview1 =

				const_cast<IndexedDBView<View, IdxContainer, HashOrNoHash> &>(indexview1);
		   IndexedDBView<View, IdxContainer, HashOrNoHash> &idxview2 =
				const_cast<IndexedDBView<View, IdxContainer, HashOrNoHash> &>(indexview2);

		   idxview1.fetch();
		   idxview2.fetch();

		   // return idxview1.indexes[orderOfKeys[0]] >= idxview2.indexes[orderOfKeys[0]];

		   index_map::iterator map_it1 = idxview1.indexes.find(idxview1.orderOfKeys[0]);
		   index_map::iterator map_it2 = idxview2.indexes.find(idxview2.orderOfKeys[0]);

		   // make sure PK's exist for both views
		   if (map_it1 == idxview1.indexes.end())
		   {
			  DTL_THROW DBException(_TEXT("IndexedDBView::operator>=()"),
				  _TEXT("PK doesn't exist for first view!"), NULL, NULL);

		   }
		   else if (map_it2 == idxview2.indexes.end())			{
			  DTL_THROW DBException(_TEXT("IndexedDBView::operator>=()"),
				  _TEXT("PK doesn't exist for second view!"),
				  NULL, NULL);
		   }
		   return (*map_it1).second >= (*map_it2).second;
	   }
#endif

#ifdef DATAOBJ_PRINTABLE
	   // debugging operator<<() .., DataObj must have an operator<<()
	   friend tostream &operator<<(tostream &o, const IndexedDBView<View, IdxContainer, HashOrNoHash> &view)
	   {
		    index_map::const_iterator map_it;

			for (map_it = view.indexes.begin(); map_it != view.indexes.end(); map_it++)
			{
			   o << _TEXT("indexes[") << (*map_it).first << _TEXT("] = ") << STD_::endl;
			   o << (*map_it).second << STD_::endl;
			}

			return o;
	   }
#endif

#if 0
	// debugging
	void show_sizes()
	{
		    index_map::const_iterator map_it;

			for (map_it = indexes.begin(); map_it != indexes.end(); map_it++)
			{
			   tcout << _TEXT("indexes[") << (*map_it).first << _TEXT("].size() = ") << STD_::endl;
			   tcout << (*map_it).second.size() << STD_::endl;
			}
	}
#endif

	// gets the IOHandler of the underlying view
	template<class UserHandler> const UserHandler &
		get_io_handler(const UserHandler *dummy) const
	{
		return pDBview->get_io_handler(dummy);
	}

	// returns raw IOHandler object
	IOHandler<DataObj, ParamObj> &get_io_handler() const
	{
		return pDBview->get_io_handler();
	}

	// returns a prototype DataObj with its structure built on the fly if necessary (for variant_row)
	DataObj GetDataObj() const
	{
		return pDBview->GetDataObj();
	}

	// returns a prototpye ParamObj with its structure built on the fly if necessary (for variant_row)
	ParamObj GetParamObj() const
	{
		return pDBview->GetParamObj();
	}

#if 0
	// friend comparison functions needed to compare const_iterator with iterator
  friend bool operator==(const iterator &ncit1, const iterator &ncit2)
  {
    return (const_iterator(ncit1) == const_iterator(ncit2));
  }

  friend bool operator!=(const iterator &ncit1, const iterator &ncit2)
  {
    return !(ncit1 == ncit2);
  }

	friend bool operator==(const const_iterator &cit, const iterator &ncit)
	{
		return (cit == const_iterator(ncit));
	}

	friend bool operator==(const iterator &ncit, const const_iterator &cit)
	{
		return (cit == const_iterator(ncit));
	}

	friend bool operator!=(const const_iterator &cit, const iterator &ncit)
	{
		return !(cit == const_iterator(ncit));
	}

	friend bool operator!=(const iterator &ncit, const const_iterator &cit)
	{
		return !(cit == const_iterator(ncit));
	}
#endif
   protected:
	   // as its name implies, fetch the data associated with this view from the database
	   // if we haven't done so already
	   virtual void fetch()
	   {
		  // because of database connection issues, fetch() cannot be part of constructor
		  // all functions that return an iterator must call fetch() first to initialize
		  // the list

		  if (bFetched)
			  return;

		  if (pDBview == NULL)
			  DTL_THROW DBException(_TEXT("IndexedDBView::fetch()"),
				_TEXT("Must assign to a default constructed IndexedDBView before first use!"),
				NULL, NULL);

		  // indexes destroyed by clear() ... need to rebuild indexes map
		  if (indexes.empty())
		  {
			 BuildIndexes();
		  }

		  typename View::select_iterator read_it = pDBview->begin();
		  typename View::select_iterator read_it_end = pDBview->end();

		  SetParams(read_it.Params());

  		  // must set flag here to avoid infinite recursion
		  bFetched = true;

		  // ********** BULK_FETCH mode *****************
		  if (fetchMode == BULK_FETCH)
		  {
			// vector guaranteed to be contiguous range
			const size_t buffer_size = numRecordsToFetch;

			// will hold running list of DataObj's for IndexedDBView
			STD_::list<DataObj> tmp_data;

			// data in indexes guaranteed to be clear at this point
			// so making a fresh copy will give us what we want
			index_map tmp_indexes(indexes);

			bulk_fetch_helper_aux(read_it, buffer_size, STD_::back_inserter(tmp_data),
				should_gen_bulk_fetch());

			// now we can insert the newly read DataObj's into the indexes
			for (typename STD_::list<DataObj>::iterator data_it = tmp_data.begin();
						data_it != tmp_data.end(); ++data_it)
			{
					for (typename index_map::iterator index_it = tmp_indexes.begin();
						index_it != tmp_indexes.end(); ++index_it)
					{
						index_it->second.insert(*data_it);
					}
			}

			// all done, so now swap list data and indexes into their permanent counterparts
			data.swap(tmp_data);
			indexes.swap(tmp_indexes);

			return;
		  }

		  // ********** SINGLE_FETCH mode ***************

		  // don't throw until we catch all uniqueness constraint violations
		  size_t totalViolations = 0;

     	  // don't check uniqueness on fetch() if we've got a vec_multiset as
		  // the underlying container
		  bool bCheckUniqueness = should_check_uniqueness_on_fetch(HashOrNoHash());

		  for ( ; read_it != read_it_end; ++read_it)
		  {
			STD_::pair<iterator, bool> ins_pr = local_insert(*read_it, bCheckUniqueness);

			// uniqueness constraints violated on existing data in DB
			if (!ins_pr.second)
			{
				totalViolations++;
			}
		  }

		  // uniqueness constraints violated!  Construct exception tstring
		  // and throw
		  if (totalViolations > 0)
		  {
				tostringstream errstr;

				errstr << totalViolations << _TEXT(" uniqueness constraint violations on existing data in DB!") << STD_::ends;

				bFetched = false;

				tstring errstring = errstr.str();
				DTL_THROW DBException(_TEXT("IndexedDBView::fetch()"), errstring, NULL, NULL);
		  }

		  bFetched = true;
	   }

   private:
	   // insert object locally ... (uniqueness checks?)
	   STD_::pair<iterator, bool> local_insert(const DataObj &dataObj,
		   bool bCheckUniqueness = true)
	   {
			 // ************* more efficient implementation ****************

			 // pr.first = returns cached insertion hints for efficient insertion
			 //            if the element to insert won't violate uniqueness constraints
			 //			   otherwise, returns a single iterator pointing to the first
			 //			   DataObj that would violate uniqueness constraints
			 // pr.second = does dataObj meet the uniqueness constraints?
			 //				(also returns true if we're not checking for uniqueness)
			 //				(as index is nonunique or bCheckUniqueness = true)
			 STD_::pair<STD_::vector<iterator>, bool> pr = CheckUniqueness(dataObj, bCheckUniqueness);

		     // if uniqueness constraints violated, return pair with iterator
			 // to offending element and false indicating failure
			 if (!pr.second)

				 return STD_::pair<iterator, bool>(*pr.first.begin(), pr.second);

			 // insert the elements into the indexed view ...
			 // we have rollback semantics to preserve exception safety

		     // insert object into list that owns DataObj's
			 // data.push_back(dataObj);

			 typename STD_::list<DataObj>::iterator ins_it = data.insert(data.end(), dataObj);


 			 // add pointer to DataObj to each of the key lists
			 // remember that each DBIndex contains pointers to all addresses
			 // of elements in the list
			 // (just in a different order based on the key)
			 typename index_map::iterator idx_it = indexes.begin();
			 typename index_map::iterator idx_it_end = indexes.end();

			 // iterator pointing to insertion hints
			 typename STD_::vector<iterator>::iterator hint_it = pr.first.begin();

			 iterator new_elem_it; // iterator pointing to inserted element in PK index

			 try
			 {
				// main insertion loop where we insert dataObj into each index
				for ( ; idx_it != idx_it_end; ++idx_it, ++hint_it)
				{
					// as the vector of hints is built in the order
					// of DBIndex objects, the insertion for *idx_it uses
					// the hint *hint_it

					// *ins_it references the last element in the indexed view's
					// owned object list

					iterator tmp_elem_it =
						iterator(idx_it->second.insert(hint_it->base(), *ins_it));
									// insert into index using hint

					// if we've inserted into primary key, the iterator
					// returned in the line above (tmp_elem_it) will be the first
					// element in the pair returned by local_insert() upon success
					if (idx_it->first == orderOfKeys[0])
						new_elem_it = tmp_elem_it;
				}
			 }
			 catch (STD_::exception &ex)
			 {
				tcout << ex.what() << STD_::endl;

				typename index_map::reverse_iterator rev_it =
					SPEC_CASE_TYPENAME_IN_TEMPLATE_PARAMS index_map::reverse_iterator(idx_it);

				// rollback local state on an exception, then rethrow
				for ( ; rev_it != indexes.rend(); rev_it++)
				{
					IndexType &index = (*rev_it).second;
				    index.erase(*ins_it);
				}

				// remove object from master list
				data.erase(ins_it);
				throw;
			 }

			 return STD_::pair<iterator, bool>(new_elem_it, true);
	   }

	   // insert object locally ... (uniqueness checks?)
	   // element pointed to it disregarded in uniqueness checks
	   STD_::pair<iterator, bool> local_insert(const DataObj &dataObj, iterator it,
		   bool bCheckUniqueness = true)
	   {
			 // ************* more efficient implementation ****************

			 // pr.first = returns cached insertion hints for efficient insertion
			 //            if the element to insert won't violate uniqueness constraints
			 //			   otherwise, returns a single iterator pointing to the first
			 //			   DataObj that would violate uniqueness constraints
			 // pr.second = does dataObj meet the uniqueness constraints?
			 //				(also returns true if we're not checking for uniqueness)
			 //				(as index is nonunique or bCheckUniqueness = true)
		   STD_::pair<STD_::vector<iterator>, bool> pr = CheckUniqueness(dataObj, it, bCheckUniqueness);

		     // if uniqueness constraints violated, return pair with iterator
			 // to offending element and false indicating failure
			 if (!pr.second)
				 return STD_::pair<iterator, bool>(*pr.first.begin(), pr.second);

			 // insert the elements into the indexed view ...
			 // we have rollback semantics to preserve exception safety

		     // insert object into list that owns DataObj's
			 // data.push_back(dataObj);

			 typename STD_::list<DataObj>::iterator ins_it = data.insert(data.end(), dataObj);


 			 // add pointer to DataObj to each of the key lists
			 // remember that each DBIndex contains pointers to all addresses
			 // of elements in the list
			 // (just in a different order based on the key)
			 typename index_map::iterator idx_it = indexes.begin();
			 typename index_map::iterator idx_it_end = indexes.end();


			 // iterator pointing to insertion hints
			 typename STD_::vector<iterator>::iterator hint_it = pr.first.begin();

 			 iterator new_elem_it; // iterator pointing to inserted element in PK index

			 try
			 {
				// main insertion loop where we insert dataObj into each index
				for ( ; idx_it != idx_it_end; ++idx_it, ++hint_it)
				{
					// as the vector of hints is built in the order
					// of DBIndex objects, the insertion for *idx_it uses
					// the hint *hint_it

					// *ins_it references the last element in the indexed view's
					// owned object list
					iterator tmp_elem_it =
						iterator(idx_it->second.insert(hint_it->base(), *ins_it));
								// insert into index using hint

					// if we've inserted into primary key, the iterator
					// returned in the line above (tmp_elem_it) will be the first
					// element in the pair returned by local_insert() upon success
					if (idx_it->first == orderOfKeys[0])
						new_elem_it = tmp_elem_it;
				}
			 }
			 catch (STD_::exception &ex)
			 {
				tcout << ex.what() << STD_::endl;
				typename index_map::reverse_iterator rev_it =
					SPEC_CASE_TYPENAME_IN_TEMPLATE_PARAMS index_map::reverse_iterator(idx_it);

				// rollback local state on an exception, then rethrow
				for ( ; rev_it != indexes.rend(); rev_it++)
				{
					IndexType &index = (*rev_it).second;
				    index.erase(*ins_it);
				}

				// remove object from master list
				data.erase(ins_it);
				throw;
			 }

			 // return iterator to newly inserted element and success
			 return STD_::pair<iterator, bool>(new_elem_it, true);
	   }

	   // remove the DataObj pointed to by the iterator from the IndexedDBView
	   // must remove from all index lists and then from the owned list
	   void local_erase(iterator it)
	   {
		    // grab address of the DataObj in the owned data list
			DataObj *pData = &(*it);

			// now remove all pointers to the DataObj from the index lists
			for (typename index_map::iterator map_it = indexes.begin(); map_it != indexes.end();
				map_it++)
			{
				IndexType &index = (*map_it).second;
				index.erase(*pData);
			}

			// remove item from owned data list
			for (data_iterator data_it = data.begin(); data_it != data.end();
				data_it++)
			{
				// remove the DataObj if the iterator is pointing to the same DataObj
			    // referred to by the iterator passed in
				if (pData == &(*data_it))
				{
					data.erase(data_it);
					break;
				}
			}

	   }

	   // extract the BoundIO's representing the columns that make up the primary key
	   BoundIOs ExtractPK() const
	   {

		   IndexedDBView<View, IdxContainer, HashOrNoHash> *this_ptr = const_cast<IndexedDBView<View, IdxContainer, HashOrNoHash> *>(this);

		   typename index_map::iterator map_it = this_ptr->indexes.find(orderOfKeys[0]);

		   if (map_it == this_ptr->indexes.end())
		   {
				DTL_THROW DBException(_TEXT("IndexedDBView::ExtractPK()"), _TEXT("PK does not exist for this view"),
					NULL, NULL);
		   }

		   IndexType &idx = (*map_it).second;

		   return idx.ExtractKey();
	   }

	   // extract the BoundIO's representing the columns that make up the primary key
	   BoundIOs ExtractAutoKey() const
	   {

		   IndexedDBView<View, IdxContainer, HashOrNoHash> *this_ptr = const_cast<IndexedDBView<View, IdxContainer, HashOrNoHash> *>(this);

		   typename index_map::iterator map_it = this_ptr->indexes.find(_TEXT("AutoKey"));

		   if (map_it == this_ptr->indexes.end())
		   {
				DTL_THROW DBException(_TEXT("IndexedDBView::ExtractAutoKey()"), _TEXT("PK does not exist for this view"),
					NULL, NULL);
		   }

		   IndexType &idx = (*map_it).second;

		   return idx.ExtractKey();
	   }

// see if we would be allowed to insert dataObj into this indexed view
// based on uniqueness constraints
// of returned pair pr:
// pr.first = on success, returns vector of insertion hints for use by local_insert(),
//            one for each index
//            on failure, what should we return???  Maybe a vector of size 1 with
//            an iterator to the offending element (assumed in implementation below)?
// pr.second = returns whether the element may be inserted into the indexed view
//             (uniqueness test succeeded or index is nonunique in the first place)

	   STD_::pair<STD_::vector<iterator>, bool> CheckUniqueness(const DataObj &dataObj,
		   bool bCheckUniqueness = true) const
	   {
		   // ********* new, more efficient implemenation ****************
		   IndexedDBView<View, IdxContainer, HashOrNoHash> *this_ptr = const_cast<IndexedDBView<View, IdxContainer, HashOrNoHash> *>(this);

		   STD_::vector<iterator> hints; // hints for insertion

		   // check all indexes to see if dataObj meets uniqueness constraints
		   // if so, return insertion hint and success
		   // otherwise, return vector with single iterator to offending element
		   // and indicate failure
		   for (typename index_map::iterator map_it = this_ptr->indexes.begin(); map_it != this_ptr->indexes.end(); ++map_it)
		   {
			   STD_::pair<indexed_data_iterator, bool> raw_pr =
					(*map_it).second.CheckUniqueness(dataObj, bCheckUniqueness);

			   STD_::pair<iterator, bool> pr(iterator(raw_pr.first), raw_pr.second);

			   if (pr.second)
			   {
					// if ok, we record the hint iterator returned by CheckUniqueness
					hints.push_back(pr.first);
			   }

			   else
			   {
					// if uniqueness test failed for the index, return the iterator
					// to the offending element and indicate failure
				   return STD_::pair<STD_::vector<iterator>, bool>(
					   STD_::vector<iterator>(1, pr.first), false
				   );
			   }
			}

			// success
		   return STD_::pair<STD_::vector<iterator>, bool>(hints, true);
	   }

	   // ****** need to rewrite this overload to reflect new local_insert(),
	   // CheckUniqueness() logic ******************************************

	   // returns iterator to existing element and false if not unique to index,
	   // end() and true otherwise
	   // the element pointed to by the iterator passed in is disregarded in the
	   // test for uniqueness performed by this method

	   // will rewrite to get proper behavior
	   STD_::pair<STD_::vector<iterator>, bool> CheckUniqueness(const DataObj &dataObj, iterator it,
		   bool bCheckUniqueness = true)
	   {
		   IndexedDBView<View, IdxContainer, HashOrNoHash> *this_ptr = const_cast<IndexedDBView<View, IdxContainer, HashOrNoHash> *>(this);

		   STD_::vector<iterator> hints;

		   // check unique indices for match
		   // if found it, return make_pair(iterator to found element, false)
		   for (typename index_map::iterator map_it = this_ptr->indexes.begin();
		              map_it != this_ptr->indexes.end(); map_it++)
		   {
			    IndexType &idx = (*map_it).second;

				STD_::pair<indexed_data_iterator, bool> raw_pr = idx.CheckUniqueness(dataObj, bCheckUniqueness);

				STD_::pair<iterator, bool> pr(iterator(raw_pr.first), raw_pr.second);

				// STD_::pair<iterator, bool>unique_pr(iterator(pr.first), pr.second);

				// if uniqueness check failed, return a pair containing a vector
				// with one element with the iterator to the offending element
				// and indication of failure
				if (!pr.second && pr.first != it)
					return STD_::pair<STD_::vector<iterator>, bool>(
					   STD_::vector<iterator>(1, pr.first), false
					);
				else
				{
					// if ok, we record the hint iterator returned by CheckUniqueness
					hints.push_back(pr.first);
				}
		   }

			// success
			return STD_::pair<STD_::vector<iterator>, bool>(hints, true);
	   }

	   // utility functions to determine whether we should check uniqueness on the initial fetch()

	   // only when container is not geared towards efficient inserts before lookups
	   // should we check uniqueness on the fetch
	   bool should_check_uniqueness_on_fetch(NO_HASH nh)
	   {
		   return typeid(IdxContainer) != typeid(vec_multiset<DataObj *,
			   TYPENAME_IN_TEMPLATE_PARAMS IdxContainer::key_compare,
			   TYPENAME_IN_TEMPLATE_PARAMS IdxContainer::allocator_type>);

	   }

	   // always check uniqueness for hashed containers on fetch
	   bool should_check_uniqueness_on_fetch(HASH DTL_DUMMY_PARAM_1)
	   {
		   return 1;
	   }

	   template<class OutputIterator>
		   void bulk_fetch_helper_aux(typename View::select_iterator &read_it,
		   size_t buffer_size, OutputIterator output, GEN_BULK_FETCH)
	   {
		   bulk_fetch_helper(read_it, buffer_size, output);
	   }

	   template<class OutputIterator>
		   void bulk_fetch_helper_aux(typename View::select_iterator & DTL_DUMMY_PARAM_1,
		   size_t DTL_DUMMY_PARAM_2, OutputIterator DTL_DUMMY_PARAM_3, NO_GEN_BULK_FETCH)
	   {

	   }
};

END_DTL_NAMESPACE

#endif
