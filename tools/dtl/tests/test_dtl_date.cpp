#include "std_warn_off.h"
#include <iostream>
#include <DTL.h>
#include <dtl_date.h>
#include <nullable.h>
#include "std_warn_on.h"

using namespace dtl;
using namespace std;


#ifndef DTL_HAVE_BOOST

void Test_dtl_date()
{
   tcout << "Not supported without DTL_HAVE_BOOST" << endl;
}

#else

typedef nullable<dtl_date> nulldate;

namespace dtl {
   template <>
   struct DefaultBCA<dtl_date>
   {
     void operator()(BoundIOs & boundIOs, dtl_date & rowbuf)
     {
        boundIOs[_TEXT("EXAMPLE_DATE")] == rowbuf;
     }
   };

   template <>
   struct DefaultBCA<nulldate>
   {
     void operator()(BoundIOs & boundIOs, nulldate & rowbuf)
     {
        boundIOs[_TEXT("EXAMPLE_DATE")] == rowbuf;
     }
   };
}



void Test_dtl_date()
{
   try {
		DBStmt(_TEXT("CREATE TABLE test_date (   EXAMPLE_DATE  DATE )")).Execute();
   }
   catch (...) {};

	DBStmt(_TEXT("delete from test_date")).Execute();
	DBStmt(_TEXT("insert into test_date (example_date) values ('2004-08-01')")).Execute();
	DBStmt(_TEXT("insert into test_date (example_date) values ('2004-08-02')")).Execute();
	DBStmt(_TEXT("insert into test_date (example_date) values ('2004-08-03')")).Execute();
	DBStmt(_TEXT("insert into test_date (example_date) values ('2004-08-04')")).Execute();
	DBStmt(_TEXT("insert into test_date (example_date) values ('2004-08-05')")).Execute();

   DBView<dtl_date> plain_view(_TEXT("test_date"));
   RandomDBView<dtl_date> view(plain_view);

   // first, a straight copy into a vector from plain_view
   tcout << "Test full read via DBView<>" << endl;
   vector<dtl_date> copy1(plain_view.begin(),plain_view.end());
   tcout << "output from DBView\n";
   vector<dtl_date>::iterator it;
   for (it = copy1.begin(); it != copy1.end(); ++it) {
	   cout << (int) it->year() << endl;
   }
   // now via random
   // TODO theres a problem with user validation here
   tcout << "Test full read via RandomDBView<>" << endl;
   vector<dtl_date> copy2(view.begin(),view.end());

   tcout << "output from RandomDBView\n";
   for (it = copy2.begin(); it != copy2.end(); ++it) {
	   cout << (int) it->year() << endl;
   }
   tcout << "Testing 2004-01-01 date" << endl;

   {
      DBStmt(_TEXT("delete from test_date")).Execute();
      DBStmt(_TEXT("insert into test_date (example_date) values "
               "('2004-01-01')")).Execute();
      DBView<dtl_date> plain_view(_TEXT("test_date"));
      dtl_date t = *plain_view.begin();
      assert(t == dtl_date(2004,1,1));
   }

   tcout << "Testing 2004-12-31 date" << endl;

   {
      DBStmt(_TEXT("delete from test_date")).Execute();
      DBStmt(_TEXT("insert into test_date (example_date) values "
               "('2004-12-31')")).Execute();
      DBView<dtl_date> plain_view(_TEXT("test_date"));
      dtl_date t = *plain_view.begin();
      assert(t == dtl_date(2004,12,31));
   }

   tcout << "Testing 2004 date" << endl;

   {
      DBStmt(_TEXT("delete from test_date")).Execute();
      DBStmt(_TEXT("insert into test_date (example_date) values "
               "('2004-08-05')")).Execute();
      DBView<dtl_date> plain_view(_TEXT("test_date"));
      dtl_date t = *plain_view.begin();
      assert(t == dtl_date(2004,8,5));
   }

   tcout << "Testing pre-1970 (1929) date" << endl;

   {
      DBStmt(_TEXT("delete from test_date")).Execute();
      DBStmt(_TEXT("insert into test_date (example_date) values "
               "('1929-08-05')")).Execute();
      DBView<dtl_date> plain_view(_TEXT("test_date"));
      dtl_date t = *plain_view.begin();
      assert(t == dtl_date(1929,8,5));
   }

   tcout << "Testing NULL date with null data" << endl;

   {
      DBStmt(_TEXT("delete from test_date")).Execute();
      DBStmt(_TEXT("insert into test_date (example_date) values "
               "(NULL)")).Execute();
      DBView<nulldate> plain_view(_TEXT("test_date"));
      nulldate t = *plain_view.begin();
      assert(t.empty());
   }

   tcout << "Testing NULL date with valid data" << endl;

   {
      DBStmt(_TEXT("delete from test_date")).Execute();
      DBStmt(_TEXT("insert into test_date (example_date) values "
               "('1929-08-05')")).Execute();
      DBView<nulldate> plain_view(_TEXT("test_date"));
      nulldate t = *plain_view.begin();
      assert(!t.empty() && *t == dtl_date(1929,8,5));
   }

   tcout << "Testing writing valid date into NULL column" << endl;

   {
      DBStmt(_TEXT("delete from test_date")).Execute();
      DBView<nulldate> plain_view(_TEXT("test_date"));
      nulldate n = dtl_date(1929,8,5);
      DBView<nulldate>::insert_iterator insert(plain_view);
      *insert = n;
      ++insert;
      nulldate t = *plain_view.begin();
      assert(!t.empty() && *t == dtl_date(1929,8,5));
   }

   tcout << "Testing NULL date with null data via RandomDBView" << endl;

   {
      DBStmt(_TEXT("delete from test_date")).Execute();
      DBStmt(_TEXT("insert into test_date (example_date) values "
               "(NULL)")).Execute();
      DBView<nulldate> plain_view(_TEXT("test_date"));
      RandomDBView<nulldate> view(plain_view);
      nulldate t = *view.begin();
      assert(t.empty());
   }

   tcout << "Testing NULL date with valid data via RandomDBView" << endl;

   {
      DBStmt(_TEXT("delete from test_date")).Execute();
      DBStmt(_TEXT("insert into test_date (example_date) values "
               "('1929-08-05')")).Execute();
      DBView<nulldate> plain_view(_TEXT("test_date"));
      RandomDBView<nulldate> view(plain_view);
      nulldate t = *view.begin();
      assert(!t.empty() && *t == dtl_date(1929,8,5));
   }

   tcout << "Iteration tests" << endl;

   {
      DBStmt(_TEXT("delete from test_date")).Execute();
      DBStmt(_TEXT("insert into test_date (example_date) values "
               "('2004-08-01'),"
               "('2004-08-02'),"
               "('2004-08-03'),"
               "('2004-08-04'),"
               "('2004-08-05')")).Execute();

      DBView<dtl_date> plain_view(_TEXT("test_date"));
      RandomDBView<dtl_date> view(plain_view);

      // first, a straight copy into a vector from plain_view
      tcout << "Test full read via DBView<>" << endl;
      vector<dtl_date> copy1(plain_view.begin(),plain_view.end());
      // now via random
      // TODO theres a problem with user validation here
      tcout << "Test full read via RandomDBView<>" << endl;
      vector<dtl_date> copy2(view.begin(),view.end());
   }
}

#endif
