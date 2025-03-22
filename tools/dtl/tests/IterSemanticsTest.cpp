// test DB_iterator semantics

#include "IterSemanticsTest.h"
#include "select_iterator.h"
#include <algorithm>
#include <iostream>
#include <utility>
#include <iterator>
using namespace  std;

void VariantRowBPAExample(BoundIOs &boundIOs, variant_row &paramObj)
{
	boundIOs[0] == paramObj._int();
	boundIOs[1] == paramObj._int();
	boundIOs[2] == paramObj._tstring();
	boundIOs[3] == paramObj._timestamp();
	boundIOs.BindVariantRow(paramObj);
}

// tests the assigning of params to iterator before iterator is open
void TestParamsBeforeOpen()
{
	PrintHeader(tcout, _TEXT("TestParamsBeforeOpen()"));

	// construct view
	// DBView<Example> is actually DBView<Example, 
    // DefaultParamObj<Example> > thanks to the default argument to the DBView
	// template

	DBView<Example, variant_row>
		view(_TEXT("DB_EXAMPLE"), BCAExampleObj(),
		_TEXT("WHERE INT_VALUE BETWEEN (?) AND (?) AND ")
		_TEXT("STRING_VALUE = (?) OR EXAMPLE_DATE < (?)") + exampleOrderBy,
		cb_ptr_fun(VariantRowBPAExample));

	// loop through query results and add them to our vector
	// in this loop, read_it.GetLastCount() records read from DB

	DBView<Example, variant_row>::select_iterator read_it = view.begin();

	// set parameter values for the WHERE clause in our SQL query
	// note: read_it not opened until for-loop below
	variant_row &params = read_it.Params();
	
	params[0] = 2; // lowIntValue
	params[1] = 8; // highIntValue
	params[2] = tstring(_TEXT("Example")); // strValue

	TIMESTAMP_STRUCT paramDate = {2000, 1, 1, 0, 0, 0, 0};
	params[3] = paramDate; // dateValue 

	for ( ; read_it != view.end(); ++read_it)
	{
		tcout << *read_it << endl;
	}
	
	PrintSeparator(tcout);
}

// tests iterator assignment to make sure
// that changing of params on one iterator doesn't effect params of other
void TestIterEffectsOnOtherIter()
{
	PrintHeader(tcout, _TEXT("TestIterEffectsOnOtherIter()"));

	// construct view
	// DBView<Example> is actually DBView<Example, 
    // DefaultParamObj<Example> > thanks to the default argument to the DBView
	// template

	DBView<Example, variant_row>
		view(_TEXT("DB_EXAMPLE"), BCAExampleObj(),
		_TEXT("WHERE INT_VALUE BETWEEN (?) AND (?) AND ")
		_TEXT("STRING_VALUE = (?) OR EXAMPLE_DATE < (?)") + exampleOrderBy,
		cb_ptr_fun(VariantRowBPAExample));

	// loop through query results and add them to our vector
	// in this loop, read_it.GetLastCount() records read from DB

	DBView<Example, variant_row>::select_iterator read_it = view.begin();

	// set parameter values for the WHERE clause in our SQL query
	// note: read_it not opened until for-loop below
	variant_row &params = read_it.Params();
	
	params[0] = 2; // lowIntValue
	params[1] = 8; // highIntValue
	params[2] = tstring(_TEXT("Example")); // strValue

	TIMESTAMP_STRUCT paramDate = {2000, 1, 1, 0, 0, 0, 0};
	params[3] = paramDate; // dateValue 

	// make our copy
	DBView<Example, variant_row>::select_iterator read_it_copy = read_it;

	// invalidate the original read_it's params
	params = variant_row();

	for ( ; read_it_copy != view.end(); ++read_it_copy)
	{
		tcout << *read_it_copy << endl;
	}
	
	PrintSeparator(tcout);


}

// tests iterator copy ... std::copy() algorithm should work on copies of
// DB_iterators ... such as:
// DBView<Example>::select_iterator = view.begin();
// copy(it, view.end(), tostream_iterator<Example, TCHAR>(tcout, _TEXT("\n"));
void TestIterCopy()
{
	PrintHeader(tcout, _TEXT("TestIterCopy()"));

	// construct view
	// DBView<Example> is actually DBView<Example, 
    // DefaultParamObj<Example> > thanks to the default argument to the DBView
	// template

	DBView<Example, variant_row>
		view(_TEXT("DB_EXAMPLE"), BCAExampleObj(),
		_TEXT("WHERE INT_VALUE BETWEEN (?) AND (?) AND ")
		_TEXT("STRING_VALUE = (?) OR EXAMPLE_DATE < (?)") + exampleOrderBy,
		cb_ptr_fun(VariantRowBPAExample));

	// loop through query results and add them to our vector
	// in this loop, read_it.GetLastCount() records read from DB

	DBView<Example, variant_row>::select_iterator read_it_begin = view.begin();

	// set parameter values for the WHERE clause in our SQL query
	// note: read_it not opened until for-loop below
	variant_row &params = read_it_begin.Params();
	
	params[0] = 2; // lowIntValue
	params[1] = 8; // highIntValue
	params[2] = tstring(_TEXT("Example")); // strValue

	TIMESTAMP_STRUCT paramDate = {2000, 1, 1, 0, 0, 0, 0};
	params[3] = paramDate; // dateValue 

	DBView<Example, variant_row>::select_iterator copy_it(read_it_begin);

	tcout << read_it_begin.Params() << endl;
	tcout << copy_it.Params() << endl;

	// test iterator copy by passing read_it to std::copy()
	// if data prints out fine, test works

	copy(read_it_begin, view.end(), tostream_iterator<Example, TCHAR>(tcout, _TEXT("\n")));
	
	PrintSeparator(tcout);

}

// tests iterator copy ... copy iterator b to a, increment a, and make sure that
// dummy == dummy2
void TestIterCopyIncrDest()
{
	PrintHeader(tcout, _TEXT("TestIterCopyIncrDest()"));

	DBView<Example> view(_TEXT("DB_EXAMPLE"), DefaultBCA<Example>(), exampleOrderBy);

	DBView<Example>::select_iterator a = view.begin();

	Example dummy = *a; // force a to read first record

	tcout << _TEXT("dummy  = ") << dummy << endl;

	DBView<Example>::select_iterator b(a);

	a++; // what happens to b here?
	
	Example dummy2 = *b;  // will this give the same value as dummy?

	tcout << _TEXT("dummy2 = ") << dummy2 << endl;

	if (dummy == dummy2)
		tcout << _TEXT("Test Successful") << endl;
	else
		tcout << _TEXT("Test failed") << endl;

	PrintSeparator(tcout);
}

// test various cases for select iterators for the invariant
// i == j iff &*i == &*j
void TestSelectIteratorInvariants()
{
   PrintHeader(tcout, _TEXT("TestSelectIteratorInvariants()"));

   DBView<Example> view(_TEXT("DB_EXAMPLE"), DefaultBCA<Example>(), exampleOrderBy);

   DBView<Example>::select_iterator i = view.begin();
   DBView<Example>::select_iterator j = view.begin();

   tcout << _TEXT("i = view.begin(); j = view.begin(); ") << endl;

   tcout << _TEXT("i == j? ") << (i == j) << endl;
   tcout << _TEXT("&*i == &*j? ") << (&*i == &*j) << endl;
   
   ++i;
   j = i;

   tcout << _TEXT("++i; j = i;") << endl;

   tcout << _TEXT("i == j? ") << (i == j) << endl;
   tcout << _TEXT("&*i == &*j? ") << (&*i == &*j) << endl;
   
   ++j;
   ++i;

   tcout << _TEXT("++j; ++i") << endl;
   
   tcout << _TEXT("i == j? ") << (i == j) << endl;
   tcout << _TEXT("&*i == &*j? ") << (&*i == &*j) << endl;

   i = view.begin();
   j = view.begin();

   tcout << _TEXT("i = view.begin(); j = view.begin();") << endl;

   tcout << _TEXT("i == j? ") << (i == j) << endl;
   tcout << _TEXT("&*i == &*j? ") << (&*i == &*j) << endl;

   ++i;
   ++j;
   i = j;

   tcout << _TEXT("++i; ++j; i = j;") << endl;

   tcout << _TEXT("i == j? ") << (i == j) << endl;
   tcout << _TEXT("&*i == &*j? ") << (&*i == &*j) << endl;

   i = view.begin();
   j = i;

   tcout << _TEXT("i = view.begin(); j = i;") << endl;

   tcout << _TEXT("i == j? ") << (i == j) << endl;
   tcout << _TEXT("&*i == &*j? ") << (&*i == &*j) << endl;

   ++j;
   i = j;

   tcout << _TEXT("++j; i = j;") << endl;

   tcout << _TEXT("i == j? ") << (i == j) << endl;
   tcout << _TEXT("&*i == &*j? ") << (&*i == &*j) << endl;

   PrintSeparator(tcout);
}

// we get the proper 2 record difference back from TableDiff()
// on this test of select_iterator assignment
void TestSelectIteratorAssignment()
{
	PrintHeader(tcout, _TEXT("TestSelectIteratorAssignment()"));
	DBView<Example> view(_TEXT("DB_EXAMPLE"), DefaultBCA<Example>(), exampleOrderBy);

	tcout << _TEXT("Should see 2 records more in results_1 than results_2!") << endl;

	DBView<Example>::select_iterator read_it1 = view.begin();

    vector<Example> results_1;
	vector<Example> results_2;

	results_1.push_back(*read_it1);
	++read_it1;
	results_1.push_back(*read_it1);
	++read_it1;

	DBView<Example>::select_iterator read_it2 = read_it1;
	
	copy(read_it1, view.end(), back_insert_iterator<vector<Example> >(results_1));

	copy(read_it2, view.end(), back_insert_iterator<vector<Example> >(results_2));

	tcout << _TEXT("results_1.size() = ") << results_1.size() << endl;
	tcout << _TEXT("results_2.size() = ") << results_2.size() << endl;

	TableDiff(tcout, results_1, results_2);

	PrintSeparator(tcout);
}
