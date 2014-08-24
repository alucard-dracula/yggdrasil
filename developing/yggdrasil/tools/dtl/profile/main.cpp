#include "dtl.h"
#include <sys/timeb.h>
#include <time.h>
#include <vector>

using namespace std;
using namespace dtl;


// Define an object to hold our row data -- used by all single table examples
class Example
{
public:                       // tablename.columnname:
 int exampleInt;               // DB_EXAMPLE.INT_VALUE
 tcstring<200> exampleStr;            // DB_EXAMPLE.STRING_VALUE
 double exampleDouble;         // DB_EXAMPLE.DOUBLE_VALUE
 long exampleLong;             // DB_EXAMPLE.EXAMPLE_LONG
 jtime_c exampleDate; // DB_EXAMPLE.EXAMPLE_DATE

public: 

 Example() : exampleInt(0),  exampleDouble(0.0),
	 exampleLong(0), exampleDate() {exampleStr[0]=0;}

 Example(int exInt, char *exStr, double exDouble, 
  long exLong, const TIMESTAMP_STRUCT &exDate) :
    exampleInt(exInt),  exampleDouble(exDouble), 
	exampleLong(exLong), exampleDate(exDate){strcpy(exampleStr, exStr);}


 friend class dtl::DefaultBCA<Example>;

 friend class dtl::DefaultInsValidate<Example>;

 friend class ExampleBCA;
 friend class ExampleBPA;

 friend ostream& operator<<(ostream &o, const Example &ex)
 {
    o << "Example(" << ex.exampleInt << ", \"" << ex.exampleStr << "\", ";
	o << ex.exampleDouble << ", " << ex.exampleLong << ", ";
	o << ex.exampleDate << ")";

	return o;
 }

};

 
// Create an association between table columns and fields in our object
// Here, BoundIOs is a class that is passed in to store column names &
// mappings

BEGIN_DTL_NAMESPACE

template<> class DefaultBCA<Example>
{
public:
 void operator()(BoundIOs &cols, Example &row)
 {
  cols["INT_VALUE"]    == row.exampleInt;
  cols["STRING_VALUE"] == row.exampleStr;
  cols["DOUBLE_VALUE"] == row.exampleDouble;
  cols["EXAMPLE_LONG"] == row.exampleLong;
  cols["EXAMPLE_DATE"] == row.exampleDate;
 }
};

// Specialization of DefaultInsValidate for Example
// This defines a business rule we wish to enforce for all 
// Example objects before they are allowed to be inserted into the database
template<> class DefaultInsValidate<Example>
{
public:

	bool operator()(BoundIOs &cols, Example &rowbuf) {	
		return true;
	}
};

template<> class DefaultSelValidate<Example>
{
public:

	bool operator()(BoundIOs &cols, Example &rowbuf) {	
		// data is valid if rowbuf.exampleStr is nonempty and
		// rowbuf.exampleDouble is 
		// between 0 and 100 (like a percentage)
		return true;
	}
};
END_DTL_NAMESPACE

#if 1
// Read the contents of the DB_EXAMPLE table and return a vector of the
// resulting rows
// Use DTL
vector<Example> ReadData()
{
 vector<Example> results;
 DBView<Example> view("DB_EXAMPLE");

 DBView<Example>::select_iterator read_it = view.begin();
 DBView<Example>::select_iterator end_it = view.end();

 for ( ; read_it != end_it; ++read_it)
 {
  results.push_back(*read_it);
 }

 return results;
}
#else

#if 0
// Read the contents of the DB_EXAMPLE table and return a vector of the
// resulting rows
// Use DTL bulk fetch
vector<Example> ReadData()
{
 static vector<Example> buffer(128);
 static DBView<Example> view("DB_EXAMPLE");

 bulk_copy(view.begin(), &buffer[0], &buffer[127], false);
 
 return buffer;
}
#endif

// Read the contents of the DB_EXAMPLE table and return a vector of the
// resulting rows
// Use DTL bulk fetch helper (to validate records along with the read)
vector<Example> ReadData() {
	vector<Example> results;
	typedef DBView<Example> DBV;

	 DBV view(DBV::Args().tables("DB_EXAMPLE")
		 .handler(BulkFetchHandler<Example>())
	 );

	 DBV::select_iterator read_it = view.begin();

	 // bulk fetch *all* records into the results container 
	 // use a buffer *at least* 128 rows long as defined by buffer.capacity()
	 bulk_fetch_helper(read_it, 128, back_inserter(results));
	
	 return results;
}
#endif


// Read the contents of the DB_EXAMPLE table and return a vector of the
// resulting rows
// Use raw ODBC API
vector<Example> RawReadData()
{
	vector<Example> results;
	Example row;

	SQLHENV	     	V_OD_Env;			// Handle ODBC environment
	RETCODE		V_OD_erg;			// result of functions
	SQLHDBC		V_OD_hdbc;		// Handle connection
	SQLHSTMT	V_OD_hstmt;

	SQLINTEGER		V_OD_status[10];	// Status SQL, NULL or not
	TIMESTAMP_STRUCT    	timestamp_buffer;
	 
	V_OD_Env = DBConnection::GetDefaultConnection().GetHENV();
	V_OD_hdbc = DBConnection::GetDefaultConnection().GetHDBC();

	V_OD_erg=SQLAllocHandle(SQL_HANDLE_STMT, V_OD_hdbc, &V_OD_hstmt);
	if ((V_OD_erg != SQL_SUCCESS) && (V_OD_erg != SQL_SUCCESS_WITH_INFO))
	{
		cout << "Error. Unable to alloc handle" << endl;
		exit(0);
	}
	SQLBindCol(V_OD_hstmt,1,SQL_C_SSHORT, &row.exampleInt, 0, &V_OD_status[0]);
	SQLBindCol(V_OD_hstmt,2,SQL_C_CHAR, row.exampleStr, sizeof(row.exampleStr), &V_OD_status[1]);
	SQLBindCol(V_OD_hstmt,3,SQL_C_DOUBLE, &row.exampleDouble, 0, &V_OD_status[2]);
	SQLBindCol(V_OD_hstmt,4,SQL_C_SLONG, &row.exampleLong, 0, &V_OD_status[3]);
	SQLBindCol(V_OD_hstmt,5,SQL_C_TIMESTAMP, &timestamp_buffer, 0, &V_OD_status[4]);

	V_OD_erg=SQLExecDirect(V_OD_hstmt,(unsigned char *)"SELECT INT_VALUE, STRING_VALUE, DOUBLE_VALUE, EXAMPLE_LONG, "
	 "EXAMPLE_DATE FROM DB_EXAMPLE",SQL_NTS);   
	if ((V_OD_erg != SQL_SUCCESS) && (V_OD_erg != SQL_SUCCESS_WITH_INFO))
	{
	   cout << "Error. Unable to execute statement" << endl;
	   exit(0);
	}

	while(SQLFetch(V_OD_hstmt) != SQL_NO_DATA)
        {
	   row.exampleDate = timestamp_buffer;
           results.push_back(row);
    
        }  
    	SQLFreeHandle(SQL_HANDLE_STMT,V_OD_hstmt);

   	return results;
}

// Read the contents of the DB_EXAMPLE table and return a vector of the
// resulting rows
// Use raw ODBC API
vector<Example> RawReadDataDummy()
{
	vector<Example> results;
	Example row;

	SQLHENV	     	V_OD_Env;			// Handle ODBC environment
	RETCODE		V_OD_erg;			// result of functions
	SQLHDBC		V_OD_hdbc;		// Handle connection
	SQLHSTMT	V_OD_hstmt;

	SQLINTEGER		V_OD_status[10];	// Status SQL, NULL or not
	char                	string_buffer[200];
	TIMESTAMP_STRUCT    	timestamp_buffer;
	 
	V_OD_Env = DBConnection::GetDefaultConnection().GetHENV();
	V_OD_hdbc = DBConnection::GetDefaultConnection().GetHDBC();

	V_OD_erg=SQLAllocHandle(SQL_HANDLE_STMT, V_OD_hdbc, &V_OD_hstmt);
	if ((V_OD_erg != SQL_SUCCESS) && (V_OD_erg != SQL_SUCCESS_WITH_INFO))
	{
		cout << "Error. Unable to alloc handle" << endl;
		exit(0);
	}
	SQLBindCol(V_OD_hstmt,1,SQL_C_SSHORT, &row.exampleInt, 0, &V_OD_status[0]);
	SQLBindCol(V_OD_hstmt,2,SQL_C_CHAR, string_buffer, sizeof(string_buffer), &V_OD_status[1]);
	SQLBindCol(V_OD_hstmt,3,SQL_C_DOUBLE, &row.exampleDouble, 0, &V_OD_status[2]);
	SQLBindCol(V_OD_hstmt,4,SQL_C_SLONG, &row.exampleLong, 0, &V_OD_status[3]);
	SQLBindCol(V_OD_hstmt,5,SQL_C_TIMESTAMP, &timestamp_buffer, 0, &V_OD_status[4]);

	V_OD_erg=SQLExecDirect(V_OD_hstmt,(unsigned char *)"SELECT INT_VALUE, STRING_VALUE, DOUBLE_VALUE, EXAMPLE_LONG, "
	 "EXAMPLE_DATE FROM DB_EXAMPLE",SQL_NTS);   
	if ((V_OD_erg != SQL_SUCCESS) && (V_OD_erg != SQL_SUCCESS_WITH_INFO))
	{
	   cout << "Error. Unable to execute statement" << endl;
	   exit(0);
	}

	while(SQLFetch(V_OD_hstmt) != SQL_NO_DATA)
        {
	   row.exampleDate = timestamp_buffer;
           results.push_back(row);
     
        }  
    	SQLFreeHandle(SQL_HANDLE_STMT,V_OD_hstmt);

   	return results;
}


void test_read() {
	struct _timeb start, finish;
	vector<Example> examples;
    int i;

   // read initial data from table
   _ftime( &start );
   examples = RawReadDataDummy();  // RawReadDataDummy() is the same as RawReadData() -- see below for an explanation
  _ftime( &finish );
   cout << "Dummy: " << finish.time*1000+finish.millitm 
	   - start.time*1000 - start.millitm << endl;



   // read data from table using ODBC API
   cout << "Examples read from database using raw ODBC API:" << endl;
   _ftime( &start );
   for (i = 0; i < 100; i++)
     examples = RawReadData();
   _ftime( &finish );
   cout << "Raw: " << finish.time*1000+finish.millitm 
	   - start.time*1000 - start.millitm << endl;

 
   // read data from table using DBView
   cout << "Examples read from database using DBView:" << endl;
   _ftime( &start );
   for (i = 0; i < 100; i++)
     examples = ReadData();
   _ftime( &finish );
   cout << "Read Data: " << finish.time*1000+finish.millitm 
	   - start.time*1000 - start.millitm << endl;
}


int main(int argc, char **argv) {
	try {
	DBConnection::GetDefaultConnection().Connect(argv[1]);


	test_read();
	}
	catch(RootException &ex) {
		cout << ex.what() << endl;
	}
	return 0;

}

