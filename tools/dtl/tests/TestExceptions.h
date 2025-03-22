// code in which we intentionally cause DTL to trip and throw exceptions
// in order to make sure they work
// Initial: 12/27/2000 - MG

#ifndef _TEST_EXCEPTIONS_H
#define _TEST_EXCEPTIONS_H

#include "Example.h"

// a BCA using an invalid column name
class BadBCAExampleObj
{
public:
	void operator()(BoundIOs &boundIOs, Example &rowbuf)
    {
	   boundIOs[_TEXT("INT_VALUE")] 	   == rowbuf.exampleInt;
	   boundIOs[_TEXT("STRING_VALUE")]    == rowbuf.exampleStr;
	   boundIOs[_TEXT("DOUBLE_VALUE")]    == rowbuf.exampleDouble;
	   boundIOs[_TEXT("EXAMPLE_FOO")]   == rowbuf.exampleLong; // doesn't exist, should throw on Execute
	   boundIOs[_TEXT("EXAMPLE_DATE")]   == rowbuf.exampleDate;
	}
};

// a BPA using a parameter that's out of bounds
class BadBPAExampleObj
{
public:
	void operator()(BoundIOs &boundIOs, DefaultParamObj<Example> &paramObj)
	{
	  boundIOs[0] == paramObj.lowIntValue;
	  boundIOs[1] == paramObj.highIntValue;
	  boundIOs[2] == paramObj.strValue;
	  boundIOs[4] == paramObj.dateValue; // param # out of bounds, should throw on Execute
	}

};

class BadDelBCAExampleObj
{
public:
	void operator()(BoundIOs &boundIOs, Example &rowbuf)
	{
      boundIOs[_TEXT("STRING_VALUE")]       == rowbuf.exampleStr;
	  boundIOs[_TEXT("EXAMPLE_DATE_BAD")]   == rowbuf.exampleDate; // throw
	}
};

class BadDelBPAExampleObj
{
public:
	void operator()(BoundIOs &boundIOs, DefaultParamObj<Example> &paramObj)
	{
	  boundIOs[0] == paramObj.strValue;
	  boundIOs[2] == paramObj.lowIntValue; // throw
	}
};

// SelValidate() which throws if INT_VALUE ends in a 3
class BadSelValidate
{
public:
	bool operator()(BoundIOs & DTL_DUMMY_PARAM_1, Example &rowbuf)
	{
      return (rowbuf.exampleInt % 10 != 1);
	}
};

// InsValidate() which throws if INT_VALUE equals 5
class BadInsValidate
{
public:
	bool operator()(BoundIOs & DTL_DUMMY_PARAM_1, Example &rowbuf)
	{
		return (rowbuf.exampleInt != 7);
	}
};

// SelValidate() which throws on every third element read
// bool BadSelValidate(BoundIOs &boundIOs, Example &rowbuf);

// master function to test different exception cases
void TestExceptions();

// a function for each different case
void TestBadBCASelect();
void TestBadBPASelect();

void TestBadSelValidate();

void TestBadBCAInsert();
void TestBadInsValidate();

void TestBadBCAUpdate();
void TestBadUpdateValidate();
void TestBadBCADelete();
void TestBadBPADelete();

#endif

