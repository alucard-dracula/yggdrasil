// main code to drive test of DB Abstractions
// Initial: 9/5/2000 - MG
// Reviewed: 11/12/2000 - CJ
// Edited: 12/20/2000 - MG - added namespace support

#include "DTL.h"
#include "print_util.h"

using namespace dtl;
using namespace std;

// const tstring DSN_str = _TEXT("UID=zainet;PWD=z;DSN=zrdb");
// const tstring DSN_str = _TEXT("UID=example;PWD=example;DSN=example");
// const tstring DSN_str = _TEXT("UID=mg;PWD=mg;DSN=eve_old");
// const tstring DSN_str = _TEXT("UID=michaelg;PWD=michaelg;DSN=jeru2");

#define EXAMPLE
// #define JOIN_EXAMPLE

// ************* Uncomment if you wish to run Example.cpp ***********************
#ifdef EXAMPLE

#include "Example.h"
#include "TestExceptions.h"
#include "IterSemanticsTest.h"
// #include "UnicodeTest.h"

const TIMESTAMP_STRUCT today = {1999, 9, 29, 0, 0, 0, 0};
const TIMESTAMP_STRUCT y2k = {2000, 1, 1, 0, 0, 0, 0};

void ResetTables()
{
	DBStmt(_TEXT("DELETE FROM DB_LONGSTR")).Execute();
	DBStmt(_TEXT("DELETE FROM DB_EXAMPLE")).Execute();
	DBStmt(_TEXT("INSERT INTO DB_EXAMPLE SELECT * FROM DB_EXAMPLE_BACKUP")).Execute();
	DBConnection::GetDefaultConnection().CommitAll();
}



int main(int argc, char **argv)
{
	tcstring<50> before;
	tstring after(before);

  try
  {
    
	if (argc != 2)
	{
		tcout << _TEXT("Usage: tests.exe <DSN connect string>") << endl;
		return -1;
	}

	test_variant();

	const string DSN_argv(argv[1]);
	const tstring DSN_str = tstring_cast((tstring *) NULL, DSN_argv);
    
	DBConnection::GetDefaultConnection().Connect(DSN_str);

    // to auto commit, just uncomment the following line
	DBConnection::GetDefaultConnection().SetAutoCommit(true);

	tcout << _TEXT("Driver: ") << DBConnection::GetDefaultConnection().GetDBMSName() 
		<< _TEXT(" Version: ") << DBConnection::GetDefaultConnection().GetDBMSVersion()
		<< endl;

	ResetTables();

#if 1
	PrintHeader(tcout, _TEXT("Premature UpdateData()"));

	vector<Example> oh_my_god = SimpleRead();

//	SimpleUpdateData ( );
	UpdateData();

	vector<Example> oh_my_god_new = SimpleRead();

	TableDiff(tcout, oh_my_god, oh_my_god_new);

	PrintSeparator(tcout);
#endif

	ResetTables();
	// test insertion of a variant_row with an uninitialized date field
	PrintHeader(tcout, _TEXT("TestBadVariantInsert()"));

	TestBadVariantInsert();

	PrintSeparator(tcout);

	ResetTables();

	PrintHeader(tcout, _TEXT("TestInsertDynamicFromStatic()"));

	vector<Example> preinserted = SimpleRead();

	TestInsertDynamicFromStatic();

	vector<Example> postinserted = SimpleRead();

	TableDiff(tcout, preinserted, postinserted);
	
	PrintSeparator(tcout);

	// long string test
    ResetTables();

	PrintHeader(tcout, _TEXT("TestAVeryLongString()"));
	TestAVeryLongString();
	PrintSeparator(tcout);

	ResetTables();

	// TestRowID();

	PrintHeader(tcout, _TEXT("TestNoStringParams()"));

	vector<ExampleCharred> initial_examples  = SimpleCharredRead();

	tcout << _TEXT("Initial Examples ...") << endl;
	copy(initial_examples.begin(), initial_examples.end(),
		tostream_iterator<ExampleCharred>(tcout, _TEXT("\n")));

	TestNoStringParams();

	vector<ExampleCharred> no_param_examples = SimpleCharredRead();
	tcout << _TEXT("Examples after TestNoStringParams() ...") << endl;
	copy(no_param_examples.begin(), no_param_examples.end(),
		tostream_iterator<ExampleCharred>(tcout, _TEXT("\n")));

	TableDiff(tcout, initial_examples, no_param_examples);

	ResetTables();

	// TestRowID();

	PrintHeader(tcout, _TEXT("TestNoStringParams-IndexView()"));

	vector<ExampleCharred> initial_examples2  = SimpleCharredIdxRead();

	tcout << _TEXT("Initial Examples ...") << endl;
	copy(initial_examples2.begin(), initial_examples2.end(),
		tostream_iterator<ExampleCharred>(tcout, _TEXT("\n")));

	TestNoStringParams();

	vector<ExampleCharred> no_param_examples2 = SimpleCharredIdxRead();
	tcout << _TEXT("Examples after TestNoStringParams() ...") << endl;
	copy(no_param_examples2.begin(), no_param_examples2.end(),
		tostream_iterator<ExampleCharred>(tcout, _TEXT("\n")));

	TableDiff(tcout, initial_examples2, no_param_examples2);

	PrintSeparator(tcout);

    // use a indexed view with *static* DataObj with a *dynamic* ParamObj
    // as we are working with DB_EXAMPLE, TableDiff() can still
    // crunch on Example objects
	ResetTables();

    PrintHeader(tcout, _TEXT("IndexedViewExampleVariantParamObj()"));

    vector<Example> old_static_data = ReadData();
 
    IndexedViewExampleVariantParamObj();
	
    tcout << _TEXT("Changes after operating on dynamic indexed view:") << endl;
   
    vector<Example> new_static_data = ReadData();

    TableDiff(tcout, old_static_data, new_static_data);

    PrintSeparator(tcout);

    // use a indexed view for the dynamic case with a *dynamic* ParamObj
    // as we are working with DB_EXAMPLE, TableDiff() can still
    // crunch on Example objects
    ResetTables();

    PrintHeader(tcout, _TEXT("DynamicIndexedViewExampleVariantParamObj()"));

    vector<variant_row> old_dyn_data = ReadDynamicData();
 
    DynamicIndexedViewExampleVariantParamObj();

    tcout << _TEXT("Changes after operating on dynamic indexed view:") << endl;
   
    vector<variant_row> new_dyn_data = ReadDynamicData();

    TableDiff(tcout, old_dyn_data, new_dyn_data);

    PrintSeparator(tcout);

    ResetTables();

	// unicode tests
	//TestSimpleUnicodeRead();

	// TestUnicodeRead();
	
	// try printing out contents of initial view
	PrintExamplesFromView();

	SimpleDynamicRead();
	TestDynamicView(); 

	// dynamic IndexedDBView example
    DynamicIndexedViewExample();
	
	// get initial contents of Example table
	vector<Example> result_vec = ReadData();
	vector<Example> full_result_vec = SimpleRead();
    vector<Example> aliased_result_vec = SimpleAliasedRead();

	IndexedViewExample();

	// reads based on use of keys ... should give us the same results
	// as ReadData() though the data may be in a different order
    vector<Example> list_data = ReadDataFromIndexedView();

#ifdef __SGI_STL_PORT
	PrintHeader(tcout, _TEXT("ReadDataFromHashedIndexedView()"));
	vector<Example> hashed_data = ReadDataFromHashedIndexedView();
	copy(hashed_data.begin(), hashed_data.end(), tostream_iterator<Example, TCHAR>(tcout, _TEXT("\n")));
	PrintSeparator(tcout);
#endif

	// construct Example object to use for testing indexing
	Example index_test(3, _TEXT("Example"), 0, 0, y2k);

	// these first four tests should all yield the same results
	// (though maybe in various orders)
	vector<Example> data_by_name = ReadFilteredDataByName(index_test);
 	vector<Example> data_by_PK = ReadFilteredDataPK(index_test);
    vector<Example> data_by_DF = ReadDataIdxByStrUseDF(_TEXT("Example"));
	vector<Example> data_by_DF_PK = ReadDataIdxByStrUseDFAndPK(_TEXT("Example"));

	TestUnboundInsert();

	TestUnboundErase();

	TestBoundInsert();

	// TestBoundErasePK();

	TestBoundUpdate();

	// tests for two fields ... both using strValue and dateValue
	vector<Example> data_2_fields_by_name =
									ReadTwoFieldKeyByName(index_test);
	vector<Example> data_2_fields_by_PK =
									ReadTwoFieldKeyPK(index_test);
	vector<Example> data_2_fields_DF = ReadTwoFieldKeyByNameDF(_TEXT("Example"), y2k);
	vector<Example> data_2_fields_DF_PK = ReadTwoFieldKeyDFAndPK(_TEXT("Example"), y2k);

	vector<Example> ex_obj_vec;

	// construct some Example objects here to write
	ex_obj_vec.push_back(Example(5, _TEXT("Example"), 54.32, 45, today));
	ex_obj_vec.push_back(Example(34, _TEXT("New Record"), 333.333, 99, today));
	ex_obj_vec.push_back(Example(1111, _TEXT("Ascension"), 234.33, 12, today));

	// insert
	WriteData(ex_obj_vec);

    result_vec = ReadData();

	// update
	UpdateData();

	result_vec = ReadData();

	// delete
	DeleteData();

	result_vec = ReadData();

	SimpleDynamicWrite();

	TestIOStates();

	SanityCheck();

	TestExceptions();

	SanityCheck();

	TestVariantRowConstness();

	TestDistinctness();

	// TestConstRefs();

	// iterator semantics tests
	TestParamsBeforeOpen();

	TestIterEffectsOnOtherIter();

	TestIterCopy();

	TestIterCopyIncrDest();

	TestSelectIteratorInvariants();

	TestSelectIteratorAssignment();

	TestCopyOfIndexedView();

#ifndef DTL_USE_HASH_CONTAINER
	IndexedViewRevIters();
#endif

	PrintHeader(tcout, _TEXT("TestLocalBCAs()"));
	vector<Example> local_examples = TestLocalBCAs();
	copy(local_examples.begin(), local_examples.end(),
		tostream_iterator<Example>(tcout, _TEXT("\n")));
	PrintSeparator(tcout);

	PrintHeader(tcout, _TEXT("TestLocalBCASingleField()"));
	vector<tstring> local_strings = TestLocalBCASingleField();

	for (vector<tstring>::iterator str_it = local_strings.begin();
			str_it != local_strings.end(); ++str_it)
	{
		tcout << *str_it << endl;

	}
	
	PrintSeparator(tcout);

	PrintHeader(tcout, _TEXT("TestCharArrays()"));
	vector<StringsTest> local_strtests = TestCharArrays();
	copy(local_strtests.begin(), local_strtests.end(),
		tostream_iterator<StringsTest>(tcout, _TEXT("\n")));
	PrintSeparator(tcout);

	tcout << _TEXT("***** Resetting Tables! ***** ") << endl;

	ResetTables();

	PrintHeader(tcout, _TEXT("ReadAndUpdateData()"));
	vector<Example> examples_before_updating = SimpleRead();
	vector<Example> examples_after_updating = ReadAndUpdateData();
	copy(examples_after_updating.begin(), examples_after_updating.end(),
		tostream_iterator<Example>(tcout, _TEXT("\n")));
	TableDiff(tcout, examples_before_updating, examples_after_updating);
	PrintSeparator(tcout);

	tcout << _TEXT("***** Resetting Tables! ***** ") << endl;

	ResetTables();

	PrintHeader(tcout, _TEXT("DynamicReadAndUpdateData()"));
	vector<variant_row> dynamic_examples_before_updating = SimpleDynamicRead();
	vector<variant_row> dynamic_examples_after_updating = DynamicReadAndUpdateData();
	copy(dynamic_examples_after_updating.begin(), dynamic_examples_after_updating.end(),
		tostream_iterator<variant_row>(tcout, _TEXT("\n")));
	TableDiff(tcout, dynamic_examples_before_updating, dynamic_examples_after_updating);
	PrintSeparator(tcout);

	tcout << _TEXT("***** Resetting Tables! ***** ") << endl;

	ResetTables();

	PrintHeader(tcout, _TEXT("TestTransformSelectUpdate()"));
	tcout << _TEXT("<<< Original table data >>>") << endl;
	vector<Example> untransformed_examples = SimpleRead();
	copy(untransformed_examples.begin(), untransformed_examples.end(),
		tostream_iterator<Example>(tcout, _TEXT("\n")));
	TestTransformSelectUpdate();
	vector<Example> transformed_examples = SimpleRead();
	tcout << _TEXT("<<< Transformed table data ... exampleLong++ >>>") << endl;
	copy(transformed_examples.begin(), transformed_examples.end(),
		tostream_iterator<Example>(tcout, _TEXT("\n")));
	PrintSeparator(tcout);

	PrintHeader(tcout, _TEXT("TestPKs()"));
	TestPKs();
	PrintSeparator(tcout);

	ResetTables();
	
	PrintHeader(tcout, _TEXT("TestOuterInner()"));
	TestOuterInner();
	PrintSeparator(tcout);

    ResetTables();

    PrintHeader(tcout, _TEXT("IndexedViewExampleCharred()"));

    vector<Example> before_charred = SimpleRead();
    IndexedViewExampleCharred();
    vector<Example> after_charred = SimpleRead();

	TableDiff(tcout, before_charred, after_charred);
    PrintSeparator(tcout);

	PrintHeader(tcout, _TEXT("SimpleBlobTest()"));
	SimpleBlobTest ( );
    PrintSeparator(tcout);

	PrintHeader(tcout, _TEXT("Test_dtl_posix_time()"));
	Test_dtl_posix_time( );
    PrintSeparator(tcout);

	PrintHeader(tcout, _TEXT("Test_dtl_date()"));
	Test_dtl_date( );
    PrintSeparator(tcout);

	PrintHeader(tcout, _TEXT("Test_dtl_time()"));
	Test_dtl_time( );
    PrintSeparator(tcout);

    // These tests are in general not valid, as the DTL iterators
    // are Input Iterators, and will not pass this test.
	// PrintHeader(tcout, _TEXT("TestIterators()"));
	// TestIterators( );
    // PrintSeparator(tcout);

	PrintHeader(tcout, _TEXT("TestNullable()"));
	TestNullable( );
    PrintSeparator(tcout);

    PrintHeader(tcout,_TEXT("TestJoin()"));
    TestJoin();
    PrintSeparator(tcout);

	// PrintHeader(tcout, _TEXT("TestInsValidate()"));
   // TestInsValidate( );
   PrintSeparator(tcout);
  }

  catch (RootException &ex)
  {
	tcout << _TEXT("Exception thrown") << endl;
	tcout << ex << endl;
  }

  return 0;
}

#endif // ************************ End of Example Test ****************************

// ************** Uncomment if you wish to run JoinExample.cpp **********************
#ifdef JOIN_EXAMPLE

#include "JoinExample.h"


int main(int argc, char **argv)
{
  try
  {
	

	const string DSN_argv(argv[1]);
	const tstring DSN_str = tstring_cast((tstring *) NULL, DSN_argv);

	DBConnection::GetDefaultConnection().Connect(DSN_str);
	DBStmt(_TEXT("DELETE FROM DB_EXAMPLE")).Execute();
	DBStmt(_TEXT("INSERT INTO DB_EXAMPLE SELECT * FROM DB_EXAMPLE_BACKUP")).Execute();

	// get contents of joined Example and JoinExample tables
	vector<JoinExample> result_vec = ReadJoinedData();

	JoinExample index_test(0, _TEXT(""), 0.0, 2222, 0.0);
	vector<JoinExample> list_data = ReadJoinedDataFromIndexedView();
 	vector<JoinExample> data_by_PK = ReadJoinedFilteredDataPK(index_test);

  }

  catch (RootException &ex)
  {
	tcout << _TEXT("Exception thrown") << endl;
	tcout << ex << endl;
  }

  return 0;
}
#endif // ***************** End JoinExample Test ************************
