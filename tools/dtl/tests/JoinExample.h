// join example data class ... note the class declaration is now free of any of these DB
// abstractions
// Initial: 9/5/2000 - MG
// Edited: 12/20/2000 - MG - added namespace support - cleaned up class declaration

#ifndef _JOIN_EXAMPLE_H
#define _JOIN_EXAMPLE_H

#include "DTL.h"
#include "print_util.h"

using namespace dtl;
using namespace std;

const tstring joinOrderBy = 
	_TEXT(" ORDER BY INT_VALUE, STRING_VALUE, DOUBLE_VALUE, SAMPLE_LONG, EXTRA_FLOAT");

// assume the corresponding native data for this class is in the DB_EXAMPLE table in
// the  database except 
class JoinExample
{
  private:						// tablename.columnname:
	int exampleInt;	    		// DB_EXAMPLE.INT_VALUE
	tstring exampleStr;			// DB_EXAMPLE.STRING_VALUE
	double exampleDouble;		// DB_EXAMPLE.DOUBLE_VALUE
	unsigned long sampleLong;	// DB_SAMPLE.SAMPLE_LONG
	double extraDouble;			// DB_SAMPLE.EXTRA_FLOAT

  public:
	JoinExample() : exampleInt(0), exampleStr(_TEXT("")), exampleDouble(0.0),
		sampleLong(0), extraDouble(0.0) { }

	JoinExample(int exInt, tstring exStr, double exDouble, unsigned long samLong,
		double extra) : exampleInt(exInt), exampleStr(exStr), exampleDouble(exDouble),
		sampleLong(samLong), extraDouble(extra) { }

	friend class BCAJoinExampleObj;
	friend class BCAJoinFuncObj;

    friend class DefaultBCA<JoinExample>;

	friend class BCAJoinParamObj;
	
	friend class DefaultInsValidate<JoinExample>;

	friend class IntValuePosSelValidate;    

	friend tostream& operator<<(tostream &o, const JoinExample &je)
	{
      o << _TEXT("JoinExample(") << je.exampleInt << _TEXT(", \"") << je.exampleStr << _TEXT("\", ");
	  o << je.exampleDouble << _TEXT(", ") << je.sampleLong << _TEXT(", ") << je.extraDouble;
	  o << _TEXT(")");
	  return o;
	}
};

// BCA for JoinExample ... needed to store bindings between
// view  records and members in JoinExample objects
// as a reference is passed in for each rowbuf member, the member's address
// will be stored in the BoundColumn object for the corresponding DB column
// void BCAJoinExample(BoundIOs &boundIOs, JoinExample &rowbuf);

BEGIN_DTL_NAMESPACE
#if 1
// BCA for JoinExample ... needed to store bindings between
// view  records and members in JoinExample objects
// as a reference is passed in for each rowbuf member, the member's address
// will be stored in the BoundColumn object for the corresponding DB column
// void BCAJoinExample(BoundIOs &boundIOs, JoinExample &rowbuf);

template<> class DefaultBCA<JoinExample>
{
public:
	void operator()(BoundIOs &boundIOs, JoinExample &rowbuf)
	{
	  boundIOs[_TEXT("INT_VALUE")] 	 == rowbuf.exampleInt;
	  boundIOs[_TEXT("STRING_VALUE")]   == rowbuf.exampleStr;
	  boundIOs[_TEXT("DOUBLE_VALUE")]   == rowbuf.exampleDouble;
	  boundIOs[_TEXT("SAMPLE_LONG")]	 == rowbuf.sampleLong;
	  boundIOs[_TEXT("EXTRA_FLOAT")]    == rowbuf.extraDouble;
	}
};
#endif

// insertions should never be valid for JoinExample because it is a join on two tables
// so we specialize DefaultInsValidate to always return false for JoinExamples

template<> class DefaultInsValidate<JoinExample>
{
public:
   bool operator()(BoundIOs & DTL_DUMMY_PARAM_1, JoinExample & DTL_DUMMY_PARAM_2)
   {
	   return false; // insertions never allowed
   }
};
END_DTL_NAMESPACE

class BCAJoinFuncObj
{
public:
	void operator()(BoundIOs &boundIOs, JoinExample &rowbuf)
	{
	  boundIOs[_TEXT("INT_VALUE")] 	 == rowbuf.exampleInt;
	  boundIOs[_TEXT("STRING_VALUE")]   == rowbuf.exampleStr;
	  boundIOs[_TEXT("DOUBLE_VALUE")]   == rowbuf.exampleDouble;
	  boundIOs[_TEXT("SAMPLE_LONG")]	 == rowbuf.sampleLong;
	  boundIOs[_TEXT("EXTRA_FLOAT")]    == rowbuf.extraDouble;
	}
};

// here, we are happy with the default parameter object's behavior
// but let's define a custom parameter object
// a data class can be associated with any number of parameter object classes
class JoinParamObj
{
public:
	int intValue;
	tstring strValue;
	int sampleInt;
	tstring sampleStr;
};

class BPAJoinParamObj
{
public:
	void operator()(BoundIOs &boundIOs, JoinParamObj &paramObj)
	{
		boundIOs[0] == paramObj.intValue;
		boundIOs[1] == paramObj.strValue;
		boundIOs[2] == paramObj.sampleInt;
		boundIOs[3] == paramObj.sampleStr;
	}
};

// BCA for JoinExample ... needed to store bindings between
// view  records and members in JoinExample objects
// as a reference is passed in for each rowbuf member, the member's address
// will be stored in the BoundColumn object for the corresponding DB column
class BCAJoinExampleObj
{
public:
	void operator()(BoundIOs &boundIOs, JoinExample &rowbuf)
	{
	   boundIOs[_TEXT("INT_VALUE")] 	   == rowbuf.exampleInt;
	   boundIOs[_TEXT("STRING_VALUE")]    == rowbuf.exampleStr;
	   boundIOs[_TEXT("DOUBLE_VALUE")]    == rowbuf.exampleDouble;
	   boundIOs[_TEXT("SAMPLE_LONG")]	   == rowbuf.sampleLong;
	   boundIOs[_TEXT("EXTRA_FLOAT")]     == rowbuf.extraDouble;
	}
};

// we want to use the generic default behavior usually for SelValidate for JoinExamples
// but with the view we want, assume we only care that the intValue is positive
class IntValuePosSelValidate
{
public:
	bool operator()(BoundIOs &boundIOs, JoinExample &rowbuf)
	{
	   return (!boundIOs[_TEXT("INT_VALUE")].IsNull() && rowbuf.exampleInt > 0);
	}
};

// insertions should never be valid for JoinParamObj because it is a join on two tables
// so we specialize DefaultInsValidate to always return false for JoinExamples

// bool DefaultInsValidate(JoinExample &rowbuf);


#if 0
// insertions should never be valid for JoinParamObj because it is a join on two tables
// so we specialize DefaultInsValidate to always return false for JoinExamples
// bool JoinExampleInsValidate(BoundIOs &boundIOs, JoinExample &rowbuf);
#endif

// we want to use the generic default behavior usually for SelValidate for JoinExamples
// but with the view we want, assume we only care that the intValue is positive
// bool IntValuePosInsValidate(BoundIOs &boundIOs, JoinExample &rowbuf);

// one BPA for JoinParamObj ?initialize parameter values here
// on insert, column values get initialized by operator *()
// void BPAJoinParamObj(BoundIOs &boundIOs, JoinParamObj &paramObj);

// let's read some JoinExample objects from the database and return a vector of
// the results
vector<JoinExample> ReadJoinedData();

// typedef void (*BCAJoin) (BoundIOs &boundIOs, JoinExample &rowbuf);
// typedef void (*BPAJoin) (BoundIOs &boundIOs, JoinParamObj &paramObj);

// typedef DBView<JoinExample, JoinParamObj, BCAJoin, BPAJoinParamFuncObj> JoinExView;
// typedef IndexedDBView<JoinExample, JoinParamObj, BCAJoin, BPAJoinParamFuncObj>
//	JoinExIdxView;

// read JoinExamples from indexed view to make sure data gets in IndexedDBView's
// internal list<DataObj> properly
vector<JoinExample> ReadJoinedDataFromIndexedView();

// read JoinExamples from indexed view that have the given DataObj as (primary) key
vector<JoinExample> ReadJoinedFilteredDataPK(JoinExample &key);
#endif
