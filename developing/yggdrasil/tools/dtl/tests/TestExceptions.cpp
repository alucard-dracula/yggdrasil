// code in which we intentionally cause DTL to trip and throw exceptions
// in order to make sure they work
// Initial: 12/27/2000 - MG

#include "TestExceptions.h"

#include "select_iterator.h"
#include "insert_iterator.h"
#include "update_iterator.h"
#include "delete_iterator.h"

#include <vector>

using namespace std;

// master function to test different exception cases
void TestExceptions()
{
   // test cases ... reset all to use LoggingHandler!
   SanityCheck();
   DBConnection::GetDefaultConnection().CommitAll(); // commit due to problem on SQLServer
													 // with uncontrollable rollbacks
													 // on failure
   TestBadBCASelect();
   SanityCheck();
   TestBadBPASelect();
   SanityCheck();
   TestBadSelValidate();
   TestBadBCAInsert();
   TestBadInsValidate();
   TestBadBCAUpdate();
   TestBadUpdateValidate();
   TestBadBCADelete();
   TestBadBPADelete();
}

// test out a bad BCA on a select
void TestBadBCASelect()
{
    PrintHeader(tcout, _TEXT("TestBadBCASelect()"));

	vector<Example> results;

	// construct view
	// DBView<Example> is actually DBView<Example, 
    // DefaultParamObj<Example> > thanks to the default argument to the DBView
	// template

	// use our bad BCA which references a nonexistent column name in DB_EXAMPLE
	DBView<Example>
		view(_TEXT("DB_EXAMPLE"), BadBCAExampleObj(),
		_TEXT("WHERE INT_VALUE BETWEEN (?) AND (?) AND ")
		_TEXT("STRING_VALUE = (?) OR EXAMPLE_DATE < (?)") + exampleOrderBy,
		BPAExampleObj());

	view.set_io_handler(LoggingHandler<Example>());

	// loop through query results and add them to our vector
	// in this loop, read_it.GetCount() records read from DB

	DBView<Example>::select_iterator read_it = view.begin();

	// set parameter values for the WHERE clause in our SQL query
	read_it.Params().lowIntValue = 2;
	read_it.Params().highIntValue = 8;
	read_it.Params().strValue = _TEXT("Example");
	
	TIMESTAMP_STRUCT paramDate = {2000, 1, 1, 0, 0, 0, 0};
	read_it.Params().dateValue = paramDate;

	for ( ; read_it != view.end();  )
	{
        try
		{
		  // note that the read_iterator::GetCount()  is incremented in operator++()
		  // remember that the record is fetched and thus the count incremented
		  // before operator*() is applied to the read_iterator

		  tcout << _TEXT("Reading element #") << read_it.GetCount() << endl;
		  tcout << _TEXT("read_it->exampleInt = ") << read_it->exampleInt << endl;
		  tcout << _TEXT("read_it->exampleStr = ") << read_it->exampleStr << endl;
		  results.push_back(*read_it);

		  read_it++;
		}
		catch (RootException &ex)
		{
          tcout << _TEXT("Caught Exception!!!!") << endl;
		  tcout << ex.twhat() << endl;
		}
	}

	LoggingHandler<Example> handler = 
		read_it.get_io_handler((LoggingHandler<Example> *) NULL);

	typedef LoggingHandler<Example>::LoggedTriple LoggedTriple;

	vector<LoggedTriple> errors = handler.GetLog();

	for (vector<LoggedTriple>::iterator log_it = errors.begin(); log_it != errors.end();
			log_it++)
	{
		LoggedTriple error = *log_it;

		tcout << _TEXT("Error msg = ") << error.errmsg << endl;
		tcout << _TEXT("Example = ") << error.dataObj << endl;
	}

	PrintSeparator(tcout);
}

// test out a bad BPA on a select
void TestBadBPASelect()
{
    PrintHeader(tcout, _TEXT("TestBadBPASelect()"));

	vector<Example> results;

	// construct view
	// DBView<Example> is actually DBView<Example, 
    // DefaultParamObj<Example> > thanks to the default argument to the DBView
	// template

	// use our bad BCA which references a nonexistent column name in DB_EXAMPLE
	DBView<Example>
		view(_TEXT("DB_EXAMPLE"), BCAExampleObj(),
		_TEXT("WHERE INT_VALUE BETWEEN (?) AND (?) AND ")
		_TEXT("STRING_VALUE = (?) OR EXAMPLE_DATE < (?)") + exampleOrderBy,
		BadBPAExampleObj());

	view.set_io_handler(LoggingHandler<Example>());

	// loop through query results and add them to our vector
	// in this loop, read_it.GetLastCount() records read from DB

	DBView<Example>::select_iterator read_it = view.begin();

	// set parameter values for the WHERE clause in our SQL query
	read_it.Params().lowIntValue = 2;
	read_it.Params().highIntValue = 8;
	read_it.Params().strValue = _TEXT("Example");
	
	TIMESTAMP_STRUCT paramDate = {2000, 1, 1, 0, 0, 0, 0};
	read_it.Params().dateValue = paramDate;

	for ( ; read_it != view.end();  )
	{
        try
		{
		  // note that the read_iterator::GetCount()  is incremented in operator++()
		  // remember that the record is fetched and thus the count incremented
		  // before operator*() is applied to the read_iterator

		  tcout << _TEXT("Reading element #") << read_it.GetCount() << endl;

		  // results.push_back(*read_it);
		  tcout << _TEXT("read_it->exampleInt = ") << read_it->exampleInt << endl;
		  tcout << _TEXT("read_it->exampleStr = ") << read_it->exampleStr << endl;
		  results.push_back(*read_it);

		  read_it++;
		}
		catch (RootException &ex)
		{
          tcout << _TEXT("Caught Exception!!!!") << endl;
		  tcout << ex.twhat() << endl;
		}
	}

	LoggingHandler<Example> handler = 
		read_it.get_io_handler((LoggingHandler<Example> *) NULL);

	typedef LoggingHandler<Example>::LoggedTriple LoggedTriple;

	vector<LoggedTriple> errors = handler.GetLog();

	for (vector<LoggedTriple>::iterator log_it = errors.begin(); log_it != errors.end();
			log_it++)
	{
		LoggedTriple error = *log_it;

		tcout << _TEXT("Error msg = ") << error.errmsg << endl;
		tcout << _TEXT("Example = ") << error.dataObj << endl;
	}

	PrintSeparator(tcout);
}

// test of failed SelValidate() when reading data
void TestBadSelValidate()
{
    PrintHeader(tcout, _TEXT("TestBadSelValidate()"));

	vector<Example> results;


	// construct view
	// DBView<Example> is actually DBView<Example, 
    // DefaultParamObj<Example> > thanks to the default argument to the DBView
	// template

	// use our bad BCA which references a nonexistent column name in DB_EXAMPLE
	DBView<Example>
		view(_TEXT("DB_EXAMPLE"), BCAExampleObj(),
		_TEXT("WHERE INT_VALUE BETWEEN (?) AND (?) AND ")
		_TEXT("STRING_VALUE = (?) OR EXAMPLE_DATE < (?)") + exampleOrderBy,
		BPAExampleObj(), BadSelValidate());


	view.set_io_handler(LoggingHandler<Example>());

	// loop through query results and add them to our vector
	// in this loop, read_it.GetCount() records read from DB

	DBView<Example>::select_iterator read_it = view.begin(),
	  read_x = view.end();

	// set parameter values for the WHERE clause in our SQL query
	read_it.Params().lowIntValue = 2;
	read_it.Params().highIntValue = 8;
	read_it.Params().strValue = _TEXT("Example");
	
	TIMESTAMP_STRUCT paramDate = {2000, 1, 1, 0, 0, 0, 0};
	read_it.Params().dateValue = paramDate;

	for ( ; read_it != read_x; read_it++)
	{
        try
		{
		  // note that the read_iterator::GetCount()  is incremented in operator++()
		  // remember that the record is fetched and thus the count incremented
		  // before operator*() is applied to the read_iterator

		  tcout << _TEXT("Reading element #") << read_it.GetCount() << endl;
		  
		  tcout << _TEXT("read_it->exampleInt = ") << read_it->exampleInt << endl;
		  tcout << _TEXT("read_it->exampleStr = ") << read_it->exampleStr << endl;
		  
		  results.push_back(*read_it);
		}
		catch (RootException &ex)
		{
          tcout << _TEXT("Caught Exception!!!!") << endl;
		  tcout << ex.twhat() << endl;
		}
	}

	LoggingHandler<Example> handler = 
		read_it.get_io_handler((LoggingHandler<Example> *) NULL);

	typedef LoggingHandler<Example>::LoggedTriple LoggedTriple;

	vector<LoggedTriple> errors = handler.GetLog();

	for (vector<LoggedTriple>::iterator log_it = errors.begin(); log_it != errors.end();
			log_it++)
	{
		LoggedTriple error = *log_it;

		tcout << _TEXT("Error msg = ") << error.errmsg << endl;
		tcout << _TEXT("Example = ") << error.dataObj << endl;
	}

	PrintSeparator(tcout);
}

void TestBadBCAInsert()
{
	PrintHeader(tcout, _TEXT("TestBadBCAInsert()"));

	const TIMESTAMP_STRUCT indep = {1999, 7, 4, 0, 0, 0, 0};
    const TIMESTAMP_STRUCT y2k = {2000, 1, 1, 0, 0, 0, 0};
	const TIMESTAMP_STRUCT xmas = {2000, 12, 25, 0, 0, 0, 0};

	vector<Example> examples;

	examples.push_back(Example(7, _TEXT("Seven"), 7.777, 77, indep));
	examples.push_back(Example(6, _TEXT("Six"), 6.666, 66, y2k));
	examples.push_back(Example(5, _TEXT("Five"), 5.555, 55, xmas));

	// construct view ... note: no BPA as there is no postfix clause
	DBView<Example>
			view(_TEXT("DB_EXAMPLE"), BadBCAExampleObj());

	view.set_io_handler(LoggingHandler<Example>());

	// loop through vector and write Example objects  to DB
	// write_it.GetLastCount() records written in loop

	DBView<Example>::insert_iterator write_it = view;

	// note initialization of query parameters done in BPA for BoundParams
	// and in operator*() for BoundCols
	for (vector<Example>::const_iterator ex_it = examples.begin(); ex_it != examples.end(); ex_it++)
	{
		try
		{
		  *write_it = *ex_it;
		
	      // note that write_iterator::GetCount() is the # of records actually written 
	      // so far ... which is not registered until after operator++()
	      // hence the need to add 1 to the result of write_it.GetLastCount()
	      tcout << _TEXT("Writing element #") << write_it.GetCount() + 1 << endl;

		  write_it++;
		}
		catch (RootException &ex)
		{
		  tcout << _TEXT("Exception caught!") << endl;
		  tcout << ex.twhat() << endl;
		}
	}

	LoggingHandler<Example> handler = 
		write_it.get_io_handler((LoggingHandler<Example> *) NULL);

	typedef LoggingHandler<Example>::LoggedTriple LoggedTriple;

	vector<LoggedTriple> errors = handler.GetLog();

	for (vector<LoggedTriple>::iterator log_it = errors.begin(); log_it != errors.end();
			log_it++)
	{
		LoggedTriple error = *log_it;

		tcout << _TEXT("Error msg = ") << error.errmsg << endl;
		tcout << _TEXT("Example = ") << error.dataObj << endl;
	}

	PrintSeparator(tcout);
}

void TestBadInsValidate()
{
	PrintHeader(tcout, _TEXT("TestBadInsValidate()"));

	const TIMESTAMP_STRUCT indep = {1999, 7, 4, 0, 0, 0, 0};
    const TIMESTAMP_STRUCT y2k = {2000, 1, 1, 0, 0, 0, 0};
	const TIMESTAMP_STRUCT xmas = {2000, 12, 25, 0, 0, 0, 0};

	vector<Example> examples;

	examples.push_back(Example(7, _TEXT("Seven"), 7.777, 77, indep));
	examples.push_back(Example(6, _TEXT("Six"), 6.666, 66, y2k));
	examples.push_back(Example(5, _TEXT("Five"), 5.555, 55, xmas));

	// construct view ... note: no BPA as there is no postfix clause
	DBView<Example> view(_TEXT("DB_EXAMPLE"), DefaultBCA<Example>(), 
		    _TEXT(""), DefaultBPA<DefaultParamObj<Example> >(), 
			DefaultSelValidate<Example>(), BadInsValidate());

	view.set_io_handler(LoggingHandler<Example>());

	// loop through vector and write Example objects  to DB
	// write_it.GetLastCount() records written in loop

	DBView<Example>::insert_iterator write_it = view;

	// note initialization of query parameters done in BPA for BoundParams
	// and in operator*() for BoundCols
	for (vector<Example>::const_iterator ex_it = examples.begin(); ex_it != examples.end(); ex_it++, write_it++)
	{
		try
		{
		  *write_it = *ex_it;
		  
	      // note that write_iterator::GetCount() is the # of records actually written 
	      // so far ... which is not registered until after operator++()
	      // hence the need to add 1 to the result of write_it.GetLastCount()
	      
		  tcout << _TEXT("Writing element #") << write_it.GetCount() + 1 << endl;
		}
		catch (RootException &ex)
		{
		  tcout << _TEXT("Exception caught!") << endl;
		  tcout << ex.twhat() << endl;
		}
	}

	LoggingHandler<Example> handler = 
		write_it.get_io_handler((LoggingHandler<Example> *) NULL);

	typedef LoggingHandler<Example>::LoggedTriple LoggedTriple;

	vector<LoggedTriple> errors = handler.GetLog();

	for (vector<LoggedTriple>::iterator log_it = errors.begin(); log_it != errors.end();
			log_it++)
	{
		LoggedTriple error = *log_it;

		tcout << _TEXT("Error msg = ") << error.errmsg << endl;
		tcout << _TEXT("Example = ") << error.dataObj << endl;
	}

	PrintSeparator(tcout);
}

void TestBadBCAUpdate()
{
	// construct view
	DBView<Example> view(_TEXT("DB_EXAMPLE"), 
		BadBCAExampleObj(), _TEXT("WHERE INT_VALUE BETWEEN (?) AND (?) AND ")
		_TEXT("STRING_VALUE = (?) OR EXAMPLE_DATE = (?)"), BPAExampleObj());
	
	view.set_io_handler(LoggingHandler<Example>());

	// build an updater for the view
	DBView<Example>::update_iterator 
		exampleUpdater;
	
	exampleUpdater = view;

	// set data fields we want to update to their desired values
	// exampleStr to _TEXT("Updated") and sampleLong to 0
	Example updateMe;

	try
	{   
		// now set the parameters indicating which rows we want the update applied
		exampleUpdater.Params().lowIntValue = 5;
		exampleUpdater.Params().highIntValue = 13;
		exampleUpdater.Params().strValue = _TEXT("Find Me");

		TIMESTAMP_STRUCT paramDate = {1999, 11, 11, 0, 0, 0, 0};
		exampleUpdater.Params().dateValue = paramDate;

		updateMe.exampleStr = _TEXT("Updated");
	    updateMe.exampleLong = 25;

	    TIMESTAMP_STRUCT today = {2000, 9, 29, 0, 0, 0, 0};

	    updateMe = Example(2121, _TEXT("Dereferenced"), 99.99, 25, today);

		*exampleUpdater = updateMe;

		// execute the update
		exampleUpdater++;

		tcout << exampleUpdater.GetLastCount() << _TEXT(" rows updated!") << endl;
	}
	catch (RootException &ex)
	{
		tcout << _TEXT("Exception caught!") << endl;
		tcout << ex.twhat() << endl;
	}

	try
	{
		// now can perform other updates using the same updater object
		// make sure to put in your new values for both the data and parameter values
		// for the update
		// set data fields we want to update to their desired values
		// exampleStr to _TEXT("Second Update") and exampleLong to 66

		// now set the parameters indicating which rows we want the update applied
		exampleUpdater.Params().lowIntValue = 21;
		exampleUpdater.Params().highIntValue = 30;
		exampleUpdater.Params().strValue = _TEXT("To find");

		TIMESTAMP_STRUCT otherParamDate = {2001, 10, 31, 0, 0, 0, 0};
		exampleUpdater.Params().dateValue = otherParamDate;

		TIMESTAMP_STRUCT tomorrow = {2000, 9, 30, 0, 0, 0, 0};
		updateMe = Example(2222, _TEXT("Second Update"), 0.11111, 66, tomorrow);
		updateMe.exampleLong = 66;

		*exampleUpdater = updateMe;

		// execute the update
		exampleUpdater++;

		tcout << exampleUpdater.GetLastCount() << _TEXT(" rows updated!") << endl;
	}
	catch (RootException &ex)
	{
		tcout << _TEXT("Exception caught!") << endl;
		tcout << ex.twhat() << endl;
	}

	LoggingHandler<Example> handler = 
		exampleUpdater.get_io_handler((LoggingHandler<Example> *) NULL);

	typedef LoggingHandler<Example>::LoggedTriple LoggedTriple;

	vector<LoggedTriple> errors = handler.GetLog();

	for (vector<LoggedTriple>::iterator log_it = errors.begin(); log_it != errors.end();
			log_it++)
	{
		LoggedTriple error = *log_it;

		tcout << _TEXT("Error msg = ") << error.errmsg << endl;
		tcout << _TEXT("Example = ") << error.dataObj << endl;
	}

	PrintSeparator(tcout);
}

void TestBadUpdateValidate()
{
	PrintHeader(tcout, _TEXT("TestBadUpdateValidate()"));

	// construct view
	DBView<Example> view(_TEXT("DB_EXAMPLE"), 
		BCAExampleObj(), _TEXT("WHERE INT_VALUE BETWEEN (?) AND (?) AND ")
		_TEXT("STRING_VALUE = (?) OR EXAMPLE_DATE = (?)"), BPAExampleObj(), 
		DefaultSelValidate<Example>(), BadInsValidate());
	
	view.set_io_handler(LoggingHandler<Example>());

	// build an updater for the view
	DBView<Example>::update_iterator 
		exampleUpdater;
	
	exampleUpdater = view;

	// now set the parameters indicating which rows we want the update applied
	exampleUpdater.Params().lowIntValue = 5;
	exampleUpdater.Params().highIntValue = 33;
	exampleUpdater.Params().strValue = _TEXT("Example");

	TIMESTAMP_STRUCT paramDate = {1999, 11, 11, 0, 0, 0, 0};
	exampleUpdater.Params().dateValue = paramDate;

	// set data fields we want to update to their desired values

	Example updateMe;

	TIMESTAMP_STRUCT today = {2000, 9, 29, 0, 0, 0, 0};

	// should be rejected ... operator++() will do nothing
	updateMe = Example(7, _TEXT("Rejected"), 99.99, 25, today);

	try
	{
	  *exampleUpdater = updateMe;
    }
	catch (RootException &ex)
	{
	  tcout << _TEXT("Exception caught!") << endl;
	  tcout << ex.twhat() << endl;
	}

	// execute the update
	exampleUpdater++;

	tcout << exampleUpdater.GetLastCount() << _TEXT(" rows updated!") << endl;

	// now can perform other updates using the same updater object
	// make sure to put in your new values for both the data and parameter values
	// for the update
	// set data fields we want to update to their desired values
	// exampleStr to _TEXT("Second Update") and exampleLong to 66

	// now set the parameters indicating which rows we want the update applied
	exampleUpdater.Params().lowIntValue = 31;
	exampleUpdater.Params().highIntValue = 34;
	exampleUpdater.Params().strValue = _TEXT("Example");

	TIMESTAMP_STRUCT otherParamDate = {1995, 7, 31, 0, 0, 0, 0};
    exampleUpdater.Params().dateValue = otherParamDate;

	
	TIMESTAMP_STRUCT tomorrow = {2000, 9, 30, 0, 0, 0, 0};
	updateMe = Example(2222, _TEXT("Accepted"), 0.11111, 66, tomorrow);

	try
	{
		*exampleUpdater = updateMe;
    }
	catch (RootException &ex)
	{
	  tcout << _TEXT("Exception caught!") << endl;
	  tcout << ex.twhat() << endl;
	}

	// execute the update
	exampleUpdater++;

	tcout << exampleUpdater.GetLastCount() << _TEXT(" rows updated!") << endl;

	LoggingHandler<Example> handler = 
		exampleUpdater.get_io_handler((LoggingHandler<Example> *) NULL);

	typedef LoggingHandler<Example>::LoggedTriple LoggedTriple;

	vector<LoggedTriple> errors = handler.GetLog();

	for (vector<LoggedTriple>::iterator log_it = errors.begin(); log_it != errors.end();
			log_it++)
	{
		LoggedTriple error = *log_it;

		tcout << _TEXT("Error msg = ") << error.errmsg << endl;
		tcout << _TEXT("Example = ") << error.dataObj << endl;
	}

	PrintSeparator(tcout);
}

void TestBadBCADelete()
{

	PrintHeader(tcout, _TEXT("TestBadBCADelete()"));

	// construct view
	DBView<Example>
		view(_TEXT("DB_EXAMPLE"), 
		BadDelBCAExampleObj(), _TEXT("OR STRING_VALUE = (?) OR INT_VALUE = (?)"), 
		DelBPAExampleObj());

	view.set_io_handler(AlwaysThrowsHandler<Example>());

	// build a deleter for the view
	DBView<Example>::delete_iterator exampleDeleter;
	
	try
	{
		exampleDeleter = view;

	    // now set the parameters indicating which rows we want to delete
	    exampleDeleter.Params().strValue = _TEXT("Find Me");
	    exampleDeleter.Params().lowIntValue = 18;

	    TIMESTAMP_STRUCT y2k = {2000, 1, 1, 0, 0, 0, 0};
	
	    *exampleDeleter = Example(0, _TEXT("Example"), 0.0, 0, y2k);

	    // execute the delete
	    exampleDeleter++;

	    tcout << exampleDeleter.GetLastCount() << _TEXT(" rows deleted!") << endl;
	}
	catch (RootException &ex)
	{
		tcout << _TEXT("Exception caught!") << endl;
		tcout << ex.twhat() << endl;
	}

	try
	{
	// now can perform other deletes using the same deleter object
	// make sure to put in your new values for the parameters
	// for the delete
	
	// now set the parameters indicating which rows we want to delete

	TIMESTAMP_STRUCT today = {1999, 9, 29, 0, 0, 0, 0};
	*exampleDeleter = Example(3, _TEXT("operator->() works"), 0.0, 0, today);

	// execute the delete
	exampleDeleter++;

	tcout << exampleDeleter.GetLastCount() << _TEXT(" rows deleted!") << endl;
	}

	catch (RootException &ex)
	{
		tcout << _TEXT("Exception caught!") << endl;
		tcout << ex.twhat() << endl;
	}

	PrintSeparator(tcout);
}

void TestBadBPADelete()
{

	PrintHeader(tcout, _TEXT("TestBadBPADelete()"));

	// construct view
	DBView<Example>
		view(_TEXT("DB_EXAMPLE"), 
		DelBCAExampleObj(), _TEXT("OR STRING_VALUE = (?) OR INT_VALUE = (?)"), 
		BadDelBPAExampleObj());

	// view.set_io_handler(AlwaysThrowsHandler<Example>());

	view.set_io_handler(LoggingHandler<Example>());

	// build a deleter for the view
	DBView<Example>::delete_iterator exampleDeleter;

	try
	{
		exampleDeleter = view;

	    // now set the parameters indicating which rows we want to delete
	    exampleDeleter.Params().strValue = _TEXT("Find Me");
	    exampleDeleter.Params().lowIntValue = 18;

	    TIMESTAMP_STRUCT y2k = {2000, 1, 1, 0, 0, 0, 0};

	    *exampleDeleter = Example(0, _TEXT("Example"), 0.0, 0, y2k);

	    // execute the delete
	    exampleDeleter++;

	    tcout << exampleDeleter.GetLastCount() << _TEXT(" rows deleted!") << endl;
	}
	catch (RootException &ex)
	{
		tcout << _TEXT("Exception caught!") << endl;
		tcout << ex.twhat() << endl;
	}

	try
	{
	// now can perform other deletes using the same deleter object
	// make sure to put in your new values for the parameters
	// for the delete
	
	// now set the parameters indicating which rows we want to delete

	TIMESTAMP_STRUCT today = {1999, 9, 29, 0, 0, 0, 0};
	*exampleDeleter = Example(3, _TEXT("operator->() works"), 0.0, 0, today);

	// execute the delete
	exampleDeleter++;

	tcout << exampleDeleter.GetLastCount() << _TEXT(" rows deleted!") << endl;
	}

	catch (RootException &ex)
	{
		tcout << _TEXT("Exception caught!") << endl;
		tcout << ex.twhat() << endl;
	}

	LoggingHandler<Example> handler = 
		exampleDeleter.get_io_handler((LoggingHandler<Example> *) NULL);

	typedef LoggingHandler<Example>::LoggedTriple LoggedTriple;

	vector<LoggedTriple> errors = handler.GetLog();

	for (vector<LoggedTriple>::iterator log_it = errors.begin(); log_it != errors.end();
			log_it++)
	{
		LoggedTriple error = *log_it;

		tcout << _TEXT("Error msg = ") << error.errmsg << endl;
		tcout << _TEXT("Example = ") << error.dataObj << endl;
	}

	PrintSeparator(tcout);
}
