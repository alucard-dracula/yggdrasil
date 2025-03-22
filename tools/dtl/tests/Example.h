// Example client code for a single table
// Initial: 9/5/2000 - MG
// Edited: 12/20/2000 - MG - added namespace support

#ifndef _EXAMPLE_H
#define _EXAMPLE_H

#include "DBView.h"
#include "print_util.h"

using namespace dtl;
using namespace std;

// order by clause for DB_EXAMPLE to give us more consistent regression test results
const tstring exampleOrderBy = 
  _TEXT(" ORDER BY INT_VALUE, STRING_VALUE, DOUBLE_VALUE, EXAMPLE_LONG, EXAMPLE_DATE");

// Table difference function.
// Takes two containers and prints out the differences (via set difference) between the containers.
// container 1 = "original" values, container 2 = "new" values
// must have same value_type!
template<class Container1, class Container2> 
	void TableDiff(tostream &o, const Container1 &cont1, const Container2 &cont2)
{
   typedef typename Container1::value_type value_type;

   // copy container data into sets as set_symmetric_difference needs a sorted list to do its work
   multiset<value_type> set1;
   multiset<value_type> set2;

   // Slight workaround here, M$ compiler 6.0 STL library can only work with pointers not iterators
   // Therefore, cannot do this at set construction time as recommended by the standard
   copy(cont1.begin(), cont1.end(), inserter(set1, set1.begin()));
   copy(cont2.begin(), cont2.end(), inserter(set2, set2.begin()));

   // Show set1 - set2 = deleted / changed items
   o << _TEXT("deleted / changed items:") << endl;
   set_difference(set1.begin(), set1.end(), set2.begin(), set2.end(),
				  tostream_iterator<value_type, TCHAR>(o, _TEXT("\n")));
  
    // Show set2 - set1 = inserted / changed items
   o << _TEXT("inserted / changed items:") << endl;
   set_difference(set2.begin(), set2.end(), set1.begin(), set1.end(),
				  tostream_iterator<value_type, TCHAR>(o, _TEXT("\n")));

#if 0
   // Show all differences as single set
   set_symmetric_difference(set1.begin(), set1.end(), set2.begin(), set2.end(),
				  tostream_iterator<value_type, TCHAR>(o, _TEXT("\n")));
#endif
}

// single table example data class ... note the class declaration is now free of any of these // DB abstractions

// assume the corresponding data for this class is in the DB_EXAMPLE table in
// the database 

class Example
{
  public:							// tablename.columnname:
	int exampleInt;	    			// DB_EXAMPLE.INT_VALUE
	tstring exampleStr;				// DB_EXAMPLE.STRING_VALUE
	double exampleDouble;			// DB_EXAMPLE.DOUBLE_VALUE
	long exampleLong;				// DB_EXAMPLE.EXAMPLE_LONG
	TIMESTAMP_STRUCT exampleDate;	// DB_EXAMPLE.EXAMPLE_DATE

	Example() : exampleInt(0), exampleStr(_TEXT("")), exampleDouble(0.0), exampleLong(0)
	{ 
		TIMESTAMP_STRUCT exDate = {2000, 6, 6, 13, 13, 0, 0};
		exampleDate = exDate;
	}

	Example(int exInt, const tstring &exStr, double exDouble, long exLong,
		const TIMESTAMP_STRUCT &exDate) :
	   exampleInt(exInt), exampleStr(exStr), exampleDouble(exDouble), exampleLong(exLong),
	   exampleDate(exDate)
	{ }

	friend bool operator==(const Example &ex1, const Example &ex2)
	{
	  return (ex1.exampleInt == ex2.exampleInt) &&
			 (ex1.exampleStr == ex2.exampleStr) &&
			 (ex1.exampleDouble == ex2.exampleDouble) &&
			 (ex1.exampleLong == ex2.exampleLong) &&
			 (ex1.exampleDate == ex2.exampleDate);
	}

	friend bool operator<(const Example &ex1, const Example &ex2)
	{
		if (ex1.exampleInt < ex2.exampleInt)
			return true;
		if (ex1.exampleInt > ex2.exampleInt)
			return false;

		if (ex1.exampleStr < ex2.exampleStr)
			return true;
		if (ex1.exampleStr > ex2.exampleStr)
			return false;

		if (ex1.exampleDouble < ex2.exampleDouble)
			return true;
		if (ex1.exampleDouble > ex2.exampleDouble)
			return false;

		if (ex1.exampleLong < ex2.exampleLong)
			return true;
		if (ex1.exampleLong > ex2.exampleLong)
			return false;

		return (ex1.exampleDate < ex2.exampleDate);

	}


	friend tostream& operator<<(tostream &o, const Example &ex)
	{
		o << _TEXT("Example(") << ex.exampleInt << _TEXT(", \"") << ex.exampleStr << _TEXT("\", ");
		o << ex.exampleDouble << _TEXT(", ") << ex.exampleLong << _TEXT(", ");
		o << ex.exampleDate << _TEXT(")");
		return o;

	}
};


// specialization of DefaultParamObj for Example
// even though we're modifying the behavior of the default parameter object for
// Example, users can still create any number of parameter object classes for a single
// data class

// need to specialize within namespace
BEGIN_DTL_NAMESPACE
template<> class DefaultParamObj<Example>
{
    public:
       int lowIntValue;
	   int highIntValue;
	   tstring strValue;
	   TIMESTAMP_STRUCT dateValue;

	   DefaultParamObj() : lowIntValue(0), highIntValue(0) {}
};

// BCA for Example ... needed to store bindings between
// view  records and members in JoinExample objects
// as a reference is passed in for each rowbuf member, the member's address
// will be stored in the BoundColumn object for the corresponding DB column

// due to compiler bug (?), must explicitly qualify with dtl::
template<> class DefaultBCA<Example>
{
public:
	void operator()(BoundIOs &boundIOs, Example &rowbuf)
    {
	  boundIOs[_TEXT("INT_VALUE")] 	 == rowbuf.exampleInt;
	  boundIOs[_TEXT("STRING_VALUE")]   == rowbuf.exampleStr;
	  boundIOs[_TEXT("DOUBLE_VALUE")]   == rowbuf.exampleDouble;
	  boundIOs[_TEXT("EXAMPLE_LONG")]   == rowbuf.exampleLong;
	  boundIOs[_TEXT("EXAMPLE_DATE")]   == rowbuf.exampleDate;
	}
};

// specialization of DefaultInsValidate for Example
// can have any number of InsValidate functions for a single data class just as with
// ParamObj's ... business rule constraint we wish to enforce for all Examples
// bool DefaultInsValidate(Example &rowbuf);

// default behavior with instantiated DefaultSelValidate<Example> is the behavior we
// want here ... so no extra code needed here
// (in this example, assume data is always guaranteed to be valid after use of
// DefaultInsValidate<Example> or other InsValidate function
// unless some columns are null)

// BPA for Example using DefaultParamObj<Example> as the param. obj.
// we're binding
// initialize parameter values here

// due to compiler bug (?), must explicitly qualify with dtl::
template<> class DefaultBPA<DefaultParamObj<Example> >
{
public:
	void operator()(BoundIOs &boundIOs, DefaultParamObj<Example> &paramObj)
	{
	  boundIOs[0] == paramObj.lowIntValue;
	  boundIOs[1] == paramObj.highIntValue;
	  boundIOs[2] == paramObj.strValue;
	  boundIOs[3] == paramObj.dateValue;
	}
};

END_DTL_NAMESPACE

// must handle long strings special for Access because of
// SQL_VARCHAR fields being only 255 chars in length
// so set type to SQL_LONGVARCHAR and max strlen to 500 (max in access is up to 510)
class BCAExampleAccess
{
public:
	void operator()(BoundIOs &boundIOs, Example &rowbuf)
    {
	  boundIOs[_TEXT("INT_VALUE")] 	 == rowbuf.exampleInt;
	  boundIOs[_TEXT("STRING_VALUE")]   == rowbuf.exampleStr;
	  boundIOs[_TEXT("STRING_VALUE")].SetSQLType(SQL_LONGVARCHAR);
	  boundIOs[_TEXT("STRING_VALUE")].SetColumnSize(500);
	  boundIOs[_TEXT("DOUBLE_VALUE")]   == rowbuf.exampleDouble;
	  boundIOs[_TEXT("EXAMPLE_LONG")]   == rowbuf.exampleLong;
	  boundIOs[_TEXT("EXAMPLE_DATE")]   == rowbuf.exampleDate;
	}
};

// used for DISTINCT hack test
class DistinctBCAExample
{
public:
	void operator()(BoundIOs &boundIOs, Example &rowbuf)
	{
		boundIOs[_TEXT("DISTINCT STRING_VALUE")] == rowbuf.exampleStr;
	}
};

// used for aliased views
class AliasedBCAExample
{
public:
	void operator()(BoundIOs &boundIOs, Example &rowbuf)
    {
	  boundIOs[_TEXT("EXAMPLE.INT_VALUE")] 	 == rowbuf.exampleInt;
	  boundIOs[_TEXT("EXAMPLE.STRING_VALUE")]   == rowbuf.exampleStr;
	  boundIOs[_TEXT("EXAMPLE.DOUBLE_VALUE")]   == rowbuf.exampleDouble;
	  boundIOs[_TEXT("EXAMPLE.EXAMPLE_LONG")]   == rowbuf.exampleLong;
	  boundIOs[_TEXT("EXAMPLE.EXAMPLE_DATE")]   == rowbuf.exampleDate;
	}
};

// void DelBCAExample(BoundIOs &boundIOs, Example &rowbuf);
// BCA for Example ... needed to store bindings between
// view  records and members in JoinExample objects
// as a reference is passed in for each rowbuf member, the member's address
// will be stored in the BoundColumn object for the corresponding DB column
// void BCAExample(BoundIOs &boundIOs, Example &rowbuf);

#ifndef NDEBUG
STD_::ostream& operator<< (STD_::ostream&, const Example&);
#endif

class SimpleExampleBCA
{
public:
  void operator() (BoundIOs& boundIOs, Example& rowbuf)
  {
    boundIOs[_TEXT("STRING_VALUE")] == rowbuf.exampleStr;
#if 0
    boundIOs[_TEXT("INT_VALUE")] == rowbuf.exampleInt;
    boundIOs[_TEXT("DOUBLE_VALUE")] == rowbuf.exampleDouble;
    boundIOs[_TEXT("EXAMPLE_LONG")] == rowbuf.exampleLong;
    boundIOs[_TEXT("EXAMPLE_DATE")] == rowbuf.exampleDate;
#endif
  }
};

class SimpleExampleBPA
{
public:
  void operator() (BoundIOs & DTL_DUMMY_PARAM_1,
		   DefaultParamObj<Example>& DTL_DUMMY_PARAM_2)
  { }
};

class BCAExampleObj
{
public:
	void operator()(BoundIOs &boundIOs, Example &rowbuf)
    {
	   boundIOs[_TEXT("INT_VALUE")] 	   == rowbuf.exampleInt;
	   boundIOs[_TEXT("STRING_VALUE")]    == rowbuf.exampleStr;
	   boundIOs[_TEXT("DOUBLE_VALUE")]    == rowbuf.exampleDouble;
	   boundIOs[_TEXT("EXAMPLE_LONG")]   == rowbuf.exampleLong;
	   boundIOs[_TEXT("EXAMPLE_DATE")]   == rowbuf.exampleDate;
	}
};

class BPAExampleObj
{
public:
	void operator()(BoundIOs &boundIOs, DefaultParamObj<Example> &paramObj)
	{
	  boundIOs[0] == paramObj.lowIntValue;
	  boundIOs[1] == paramObj.highIntValue;
	  boundIOs[2] == paramObj.strValue;
	  boundIOs[3] == paramObj.dateValue;
	}

};

class DelBCAExampleObj
{
public:
	void operator()(BoundIOs &boundIOs, Example &rowbuf)
	{
      boundIOs[_TEXT("STRING_VALUE")]   == rowbuf.exampleStr;
	  boundIOs[_TEXT("EXAMPLE_DATE")]   == rowbuf.exampleDate;
	}
	
};

class DelBPAExampleObj
{
public:
	void operator()(BoundIOs &boundIOs, DefaultParamObj<Example> &paramObj)
	{
	  boundIOs[0] == paramObj.strValue;
	  boundIOs[1] == paramObj.lowIntValue;
	}
};

// set parameters function for Example ... used by IndexedDBView<Example>
void SetParamsExample(DefaultParamObj<Example> &params);

void SetParamsDynamicExample(variant_row &params);


// read some Example objects from the database and return a vector of
// the results
vector<Example> SimpleRead();
vector<Example> ReadData();

void SimpleBlobTest ( );

// simple read with an aliased view
vector<Example> SimpleAliasedRead();

// print out Example objects directly from a DBView
void PrintExamplesFromView();

// typedef DBView<Example, DefaultParamObj<Example>, BCAEx, BPAEx> BasicView;
// typedef IndexedDBView<Example, DefaultParamObj<Example>, BCAEx, BPAEx> BasicIdxView;

// write some Example objects to the database
void WriteData(const vector<Example> &examples);

// update Example object (with new values)  meeting a query in the database
void SimpleUpdateData();
void UpdateData();

// delete some Examples from the database meeting the query
void DeleteData();

// read Examples from indexed view to make sure data gets in IndexedDBView's
// internal list<DataObj> properly
vector<Example> ReadDataFromIndexedView();

#ifdef __SGI_STL_PORT
// read Examples from hashed indexed view to make sure data gets in IndexedDBView's
// internal list<DataObj> properly
vector<Example> ReadDataFromHashedIndexedView();
#endif

// read Examples from indexed view that have the given Example object as (primary) key
// with a single key known as IndexString
vector<Example> ReadFilteredDataPK(Example &key);

// read Example objects using the index IndexString by name
vector<Example> ReadFilteredDataByName(Example &key);

// read Example objects using the index IndexString by name, but using the
// datafield rather than the object to guide the search
vector<Example> ReadDataIdxByStrUseDF(const tstring &strVal);

// read Example objects using the tstring as a datafield value for the PK
// using IndexString as PK
vector<Example> ReadDataIdxByStrUseDFAndPK(const tstring &strVal);

// read Example objects using the passed in
// uses a two field key
vector<Example> ReadTwoFieldKeyByName(Example &key);

// read Example objects using the PK
// uses atwo field key
vector<Example> ReadTwoFieldKeyPK(Example &key);

// read Example objects using the tstring and date values as fields in a key
vector<Example> ReadTwoFieldKeyByNameDF(const tstring &strVal, const TIMESTAMP_STRUCT &dateVal);

// same as above, but as PK
vector<Example> ReadTwoFieldKeyDFAndPK(const tstring &strVal, const TIMESTAMP_STRUCT &dateVal);

// test the insertion of Example objects into IndexedDBView using *unbound* mode
void TestUnboundInsert();

// test the erasing of Example objects from the IndexedDBView using *unbound* mode
void TestUnboundErase();

// test the insertion of Example objects into IndexedDBView using *bound* mode
void TestBoundInsert();

// test the erasing of Example objects from the IndexedDBView using *unbound* mode
// using all fields
void TestBoundEraseUAF();

// test the erasing of Example objects from the IndexedDBView using *bound* mode
// using only the PK fields
void TestBoundErasePK();

void TestBoundUpdate();

void IndexedViewExample();

void TestDynamicView();
vector<variant_row> SimpleDynamicRead();
void SimpleDynamicWrite();

// dynamic IndexedDBView example
void DynamicIndexedViewExample();

void test_variant(void);

// CountedPtr derived test
class DerivedExample : public Example
{

};

void TestCountedPtr();

void TestIOStates();

// modified form of Boris Tursky's example with comments
struct ProcessRecord
{
    void operator()( const variant_row& row )
    {
	   // const_cast no longer needed due to new working version
	   // of 
       // variant_row& row = const_cast< variant_row& >( rrow );
	   
       // some field access
       tcout << row[ _TEXT("INT_VALUE")    ] << _TEXT(", ")
			<< row[ _TEXT("STRING_VALUE") ] << _TEXT(", ")
			<< row[ _TEXT("DOUBLE_VALUE") ] << _TEXT(", ")
            << row[ _TEXT("EXAMPLE_LONG") ] << _TEXT(", ")
			<< row[ _TEXT("EXAMPLE_DATE") ] << endl;

	   // test constness constraint again here
	   // should fail to compile

	   // due to conversion of preferred conversion
	   // of const variant_field to const variant_field &
	   
	   // row[_TEXT("INT_VALUE")] = 77;  // surely enough this line doesn't compile!
	   
	   // D:\dtl\tests\example.h(316) : error C2678: binary '=' : 
	   // no operator defined which takes a left-hand operand of 
	   // type 'const class dtl::variant_field' (or there is no acceptable conversion)
    }
};

void TestVariantRowConstness();

// test DISTINCT hack for a IndexedDBView
void TestDistinctness();

// test const ref for a DBView
// void TestConstRefs();

// BPA using a variant_row
inline void VariantBPAExample(BoundIOs &boundIOs, variant_row &paramObj)
{
	// make the bindings
	boundIOs[0] == paramObj._int();
	boundIOs[1] == paramObj._int();
	boundIOs[2] == paramObj._tstring();
	boundIOs[3] == paramObj._timestamp();

	boundIOs.BindVariantRow(paramObj);
}

// now we must have a VariantSetParamsExample
inline void VariantSetParamsExample(variant_row &params)
{
	// set parameter values
	params[0] = 2;
	params[1] = 8;
	params[2] = tstring(_TEXT("Example"));
	params[3] = tstring(_TEXT("01-Jan-2000"));
}


// Dynamic IndexedDBView example using a variant_row ParamObj
void DynamicIndexedViewExampleVariantParamObj();

// IndexedDBView using a variant_row ParamObj
void IndexedViewExampleVariantParamObj();

// Read the contents of a table and return the resulting rows
vector<variant_row> ReadDynamicData();

// test the copying of an indexed view
void TestCopyOfIndexedView();

#ifndef DTL_USE_HASH_CONTAINER
// reverse iterator test
void IndexedViewRevIters();
#endif

// test Local BCA's
vector<Example> TestLocalBCAs();

// test Local BCA's and BPA's for single fields
vector<tstring> TestLocalBCASingleField();

// stuff to test char arrays
class StringsTest
{
public:
	int testInt;
	TCHAR testStr[40];

	StringsTest() : testInt(0)
	{
		std_tstrcpy(testStr, _TEXT(""));
	}

	StringsTest(int i, TCHAR *str) : testInt(i)
	{
		std_tstrncpy(testStr, str, 39);
		testStr[39] = _TEXT('\0');
	}

	StringsTest(const StringsTest &other) : testInt(other.testInt)
	{

		std_tstrcpy(testStr, other.testStr);
	}

	StringsTest &operator=(const StringsTest &other)
	{
		if (this != &other)
		{
			testInt = other.testInt;
			std_tstrcpy(testStr, other.testStr);
		}
		
		return *this;
	}

	friend tostream &operator<<(tostream &o, const StringsTest &test)
	{
		o << _TEXT("StringsTest(") << test.testInt << _TEXT(", \"") << test.testStr << _TEXT("\")");
		return o;
	}
};

class SetParamsStringsTest : public unary_function<StringsTest &, void>
{
public:
	void operator()(StringsTest &params)
	{
		std_tstrcpy(params.testStr, _TEXT("Ascension"));
	}

};

// test character arrays
vector<StringsTest> TestCharArrays();

// test for select_update_iterator
vector<Example> ReadAndUpdateData();

// dynamic test for select_update_iterator
vector<variant_row> DynamicReadAndUpdateData();

// functor for TestTransformSelectUpdate()
class AddOneToExampleLong : public unary_function<Example, Example>
{
public:
	Example operator()(const Example &example)
	{
		Example result(example);
		result.exampleLong++;
		return result;
	}

};

// test the power of select_update_iterator ... add one to exampleLong to all objects
// in the view
void TestTransformSelectUpdate();

// sanity check
void SanityCheck();

// make sure we can grab names of primary key columns
void TestPKs();

class RowID
{
public:
	tstring rowID;
};

class BCARowID : public binary_function<BoundIOs &, RowID &, void>
{
public:
	void operator()(BoundIOs &boundIOs, RowID &rowbuf)
	{
		boundIOs[_TEXT("ROWID")] == rowbuf.rowID;
	}
};

void TestRowID();

void TestTypeIDCharArray();

// stuff to test nested structs
struct InnerStruct
{
	int inner_int;
	tstring inner_string;
	long inner_long;

	friend tostream &operator<<(tostream &o, const InnerStruct &inner)
	{
		tcout << _TEXT("InnerStruct(") << inner.inner_int << _TEXT(", ") 
			  << inner.inner_long;
		tcout << _TEXT(", \"") << inner.inner_string << _TEXT("\")");
		return o;
	}
};

struct OuterStruct
{
	double outer_double;
	InnerStruct outer_inner;
	jtime_c outer_jtime;

	friend tostream &operator<<(tostream &o, const OuterStruct &outer)
	{
		tcout << _TEXT("OuterStruct(") << outer.outer_double << _TEXT(", ");
		tcout << outer.outer_inner << _TEXT(", ") 
			  << outer.outer_jtime << _TEXT(")");
		return o;
	}
};

class BCAOuterInner : public binary_function<BoundIOs &, OuterStruct &, void>
{
public:
	void operator()(BoundIOs &boundIOs, OuterStruct &rowbuf)
	{
	  boundIOs[_TEXT("INT_VALUE")] 		== rowbuf.outer_inner.inner_int;
	  boundIOs[_TEXT("STRING_VALUE")]   == rowbuf.outer_inner.inner_string;
	  boundIOs[_TEXT("EXAMPLE_LONG")]   == rowbuf.outer_inner.inner_long;
	  boundIOs[_TEXT("DOUBLE_VALUE")]   == rowbuf.outer_double;
	  boundIOs[_TEXT("EXAMPLE_DATE")]   == rowbuf.outer_jtime;		
	}
};

// test nested structs
void TestOuterInner();

// single table example data class ... note the class declaration is now free of any of these // DB abstractions

// assume the corresponding data for this class is in the DB_EXAMPLE table in
// the database 

class ExampleCharred
{
  public:							// tablename.columnname:
	int exampleInt;	    			// DB_EXAMPLE.INT_VALUE
	TCHAR exampleStr[100];			// DB_EXAMPLE.STRING_VALUE
	double exampleDouble;			// DB_EXAMPLE.DOUBLE_VALUE
	long exampleLong;				// DB_EXAMPLE.EXAMPLE_LONG
	TIMESTAMP_STRUCT exampleDate;	// DB_EXAMPLE.EXAMPLE_DATE

	ExampleCharred() : exampleInt(0), exampleDouble(0.0), exampleLong(0)
	{ 
		exampleStr[0] = 0;
		TIMESTAMP_STRUCT exDate = {2000, 6, 6, 13, 13, 0, 0};
		exampleDate = exDate;
	}

	ExampleCharred(int exInt, const TCHAR *exStr, double exDouble, long exLong,
		const TIMESTAMP_STRUCT &exDate) :
	   exampleInt(exInt), exampleDouble(exDouble), exampleLong(exLong),
	   exampleDate(exDate)
	{ 
		std_tstrncpy(exampleStr, exStr, 49);   
	}

    ExampleCharred(const ExampleCharred &other) : 
		exampleInt(other.exampleInt), exampleDouble(other.exampleDouble),
		exampleLong(other.exampleLong), exampleDate(other.exampleDate)
	{
		std_tstrcpy(exampleStr, other.exampleStr);
	}
	
	ExampleCharred &operator=(const ExampleCharred &other)
	{
		if (this != &other)
		{
			exampleInt = other.exampleInt;
			exampleDouble = other.exampleDouble;
			exampleLong = other.exampleLong;
			exampleDate = other.exampleDate;
			std_tstrcpy(exampleStr, other.exampleStr);
		}

		return *this;

	}

	friend bool operator==(const ExampleCharred &ex1, const ExampleCharred &ex2)
	{
	  return (ex1.exampleInt == ex2.exampleInt) &&
			 (std_tstrcmp(ex1.exampleStr, ex2.exampleStr) == 0) &&
			 (ex1.exampleDouble == ex2.exampleDouble) &&
			 (ex1.exampleLong == ex2.exampleLong) &&
			 (ex1.exampleDate == ex2.exampleDate);
	}

	friend bool operator<(const ExampleCharred &ex1, const ExampleCharred &ex2)
	{
		if (ex1.exampleInt < ex2.exampleInt)
			return true;
		if (ex1.exampleInt > ex2.exampleInt)
			return false;

		int strResult = std_tstrcmp(ex1.exampleStr, ex2.exampleStr);

		if (strResult < 0)
			return true;
		if (strResult > 0)
			return false;

		if (ex1.exampleDouble < ex2.exampleDouble)
			return true;
		if (ex1.exampleDouble > ex2.exampleDouble)
			return false;

		if (ex1.exampleLong < ex2.exampleLong)
			return true;
		if (ex1.exampleLong > ex2.exampleLong)
			return false;

		return (ex1.exampleDate < ex2.exampleDate);

	}


	friend tostream& operator<<(tostream &o, const ExampleCharred &ex)
	{
		o << _TEXT("ExampleCharred(") << ex.exampleInt << _TEXT(", \"") << ex.exampleStr << _TEXT("\", ");
		o << ex.exampleDouble << _TEXT(", ") << ex.exampleLong << _TEXT(", ");
		o << ex.exampleDate << _TEXT(")");
		return o;

	}
};

void CharredBCA(BoundIOs &boundIOs, ExampleCharred &rowbuf);

BEGIN_DTL_NAMESPACE
template<> class DefaultBCA<ExampleCharred>
{
public:
	void operator()(BoundIOs &boundIOs, ExampleCharred &rowbuf)
    {
	  boundIOs[_TEXT("INT_VALUE")] 	 == rowbuf.exampleInt;
	  boundIOs[_TEXT("STRING_VALUE")]   == DTL_CHAR_FIELD(rowbuf.exampleStr);
	  boundIOs[_TEXT("DOUBLE_VALUE")]   == rowbuf.exampleDouble;
	  boundIOs[_TEXT("EXAMPLE_LONG")]   == rowbuf.exampleLong;
	  boundIOs[_TEXT("EXAMPLE_DATE")]   == rowbuf.exampleDate;
	}
};
END_DTL_NAMESPACE

class ParamObjExampleCharred
{
    public:
       int lowIntValue;
	   int highIntValue;
	   TCHAR strValue[100];
	   jtime_c dateValue;

	   ParamObjExampleCharred() : lowIntValue(0), highIntValue(0), dateValue()
       {
		 std_tstrcpy(strValue, _TEXT(""));
	   }

	   ParamObjExampleCharred(const ParamObjExampleCharred &other) :
		 lowIntValue(other.lowIntValue), highIntValue(other.highIntValue),
		 dateValue(other.dateValue)
		 {
		   std_tstrcpy(strValue, other.strValue);
         }

	   ParamObjExampleCharred &operator=(const ParamObjExampleCharred &other)
		 {
		   if (this != &other)
			 {
			   lowIntValue = other.lowIntValue;
               highIntValue = other.highIntValue;
               std_tstrcpy(strValue, other.strValue);
               dateValue = other.dateValue;
             }

           return *this;
		 }
};

class CharredBPA : 
     public binary_function<BoundIOs &, ParamObjExampleCharred &, void>
{
    public:
      void operator()(BoundIOs &boundIOs, ParamObjExampleCharred &paramObj)
	  {
		boundIOs[0] == paramObj.lowIntValue;
		boundIOs[1] == paramObj.highIntValue;
		boundIOs[2] == DTL_CHAR_FIELD(paramObj.strValue);
		boundIOs[3] == paramObj.dateValue;
      }
};

class SetParamsExampleCharred : public unary_function<ParamObjExampleCharred &, void>
{
public:
	void operator()(ParamObjExampleCharred &params)
	{
		const TIMESTAMP_STRUCT past = {1999, 1, 1, 0, 0, 0, 0};
		params.lowIntValue = 2;
		params.highIntValue = 8;
		// params.strValue = _TEXT("Example");
		std_tstrcpy(params.strValue, _TEXT("Example"));
		params.dateValue = past;
	}

};

// test params where none are strings
void TestNoStringParams();

vector<ExampleCharred> SimpleCharredRead();
vector<ExampleCharred> SimpleCharredIdxRead();
void IndexedViewExampleCharred();

// functor to generate a long string
class GenLongString
{
private:
	TCHAR currentChar;
	size_t numChars;
public:
	GenLongString(size_t numCh = 1000) : currentChar(_TEXT('A')), numChars(numCh) { }
	
	tstring operator()()
	{
		tostringstream result;
		
		// generate string by looping through alphabet ...
		// upper case, then lower
		for (size_t i = 0; i < numChars; ++i)
		{
		   result << currentChar;

		   // determine next character to output
		   switch (currentChar)
		   {
			   case _TEXT('Z') :  
				   currentChar = _TEXT('a');
				   break;
			   case _TEXT('z') :
				   currentChar = _TEXT('A');
				   break;
			   default:
				   ++currentChar;
				   break;
		   }
		}

		return result.str();
	}

};

// test insertion of string that will take >= 255 chars and thus
// more than one call to PutData()
void TestAVeryLongString();

// test copying from static structure into dynamic one and insert into DB
void TestInsertDynamicFromStatic();

//Now we call this function that puts 
//the data from struct on row in a weird way
template <class avariant, class aiterator>
bool MakeDynamicFromStatic(avariant &fields, aiterator i, const Example &a_struct)
{
   fields[(*i++).c_str()] = a_struct.exampleInt;
   fields[(*i++).c_str()] = a_struct.exampleStr;
   fields[(*i++).c_str()] = a_struct.exampleDouble;
   fields[(*i++).c_str()] = a_struct.exampleLong;
   fields[(*i++).c_str()] = a_struct.exampleDate;   

   return true;
};

// test a bad insertion of a variant_row ... should catch uninitialized timestamp
void TestBadVariantInsert();

// test the nullable<T> class
void TestNullable();

// test iterators
void TestIterators();

// test dtl_posix_time
void Test_dtl_posix_time();
void Test_dtl_date();
void Test_dtl_time();

// test ins_validate, to ensure it fires when it should.
void TestInsValidate();

// test ANSI Join syntax
void TestJoin();

#endif
