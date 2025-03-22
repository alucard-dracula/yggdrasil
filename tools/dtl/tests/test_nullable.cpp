#include "std_warn_off.h"
#include <iostream>
#include <DTL.h>
#include <nullable.h>
#include "std_warn_on.h"

using namespace dtl;
using namespace std;

struct Test
{
	nullable<int> int_val;
	nullable<double> dbl_val;
};

bool operator==(Test const& a, Test const& b)
{
   return a.int_val == b.int_val && a.dbl_val == b.dbl_val;
}

tostream &operator<<(tostream &o, const Test &t)
{
	o << t.int_val << _TEXT(", ") << t.dbl_val;
	return o;
}

namespace dtl {
   template <>
   struct DefaultBCA<Test>
   {
     void operator()(BoundIOs & boundIOs, Test & rowbuf)
     {
        boundIOs[_TEXT("INT_VALUE")] == rowbuf.int_val;
        boundIOs[_TEXT("DOUBLE_VALUE")] == rowbuf.dbl_val;
     }
   };

}

void TestNullable()
{
   {
      DBStmt(_TEXT("delete from test_nullable")).Execute();
      DBStmt(_TEXT("insert into test_nullable (int_value, double_value) values(NULL, 23)")).Execute();
      tcout << _TEXT("Testing by adjusting the double, with int as null") << endl;

      DBView<Test> view(_TEXT("test_nullable"));
      DBView<Test>::select_update_iterator it = view;
      // it.SetKey(_TEXT("INT_VALUE"));
      Test row = *it;
      tcout << _TEXT("Initial value: ") << row << endl;
      row.dbl_val = nullable<double>();
      *it = row;

      it = view;
      // it.SetKey(_TEXT("INT_VALUE"));
      assert(row == *it);
      tcout << _TEXT("Value after first update (to NULL): ") << row << endl;
      
      row.dbl_val = nullable<double>(5);
      *it = row;

      it = view;
      // it.SetKey(_TEXT("INT_VALUE"));
      assert(row == *it);
      tcout << _TEXT("Value after second update (to 5): ") << row << endl;
      
      row.dbl_val = nullable<double>(82);  // override value, set to NULL
      *it = row;

      it = view;
      // it.SetKey(_TEXT("INT_VALUE"));
      assert(row == *it);
      tcout << _TEXT("Value after third update (to 82): ") << row << endl;
      
      row.dbl_val = nullable<double>();  // override value, set to NULL
      *it = row;

      it = view;
      // it.SetKey(_TEXT("INT_VALUE"));
      assert(row == *it);
      tcout << _TEXT("Value after fourth update (to NULL): ") << row << endl;
   }

   tcout << endl;

   {
      DBStmt(_TEXT("delete from test_nullable")).Execute();
      DBStmt(_TEXT("insert into test_nullable (int_value, double_value) values(NULL, NULL)")).Execute();

      tcout << _TEXT("Testing by adjusting the int, with double as null") << endl;

      DBView<Test> view(_TEXT("test_nullable"));
      DBView<Test>::select_update_iterator it = view;
      // it.SetKey(_TEXT("INT_VALUE"));
      Test row = *it;
      tcout << _TEXT("Initial value: ") << row << endl;
      row.int_val = nullable<int>(5);
      *it = row;

      it = view;
      // it.SetKey(_TEXT("INT_VALUE"));
      assert(row == *it);
      tcout << _TEXT("Value after first update: ") << row << endl;
      
      row.int_val = nullable<int>();  // override value, set to NULL
      *it = row;

      it = view;
      // it.SetKey(_TEXT("INT_VALUE"));
      assert(row == *it);
      tcout << _TEXT("Value after second update: ") << row << endl;
   }

   tcout << endl;

   {
      DBStmt(_TEXT("delete from test_nullable")).Execute();
      DBStmt(_TEXT("insert into test_nullable (int_value, double_value) values(15, NULL)")).Execute();

      tcout << _TEXT("Testing by adjusting the int, with double as null, this time use reset() so the content doesn't change, just the NULL state.") << endl;

      DBView<Test> view(_TEXT("test_nullable"));
      DBView<Test>::select_update_iterator it = view;
      Test row = *it;
      tcout << _TEXT("Initial value: ") << row << endl;
      row.int_val.reset();
      *it = row;

      it = view;
      assert(row == *it);
      tcout << _TEXT("Value after first update: ") << row << endl;
   }

   tcout << endl;

   {
      DBStmt(_TEXT("delete from test_nullable")).Execute();
      DBStmt(_TEXT("insert into test_nullable (int_value, double_value) values(NULL, NULL)")).Execute();
      tcout << _TEXT("Testing SetKey() - Testing by adjusting the double, with int as null") << endl;

      DBView<Test> view(_TEXT("test_nullable"));
      DBView<Test>::select_update_iterator it = view;
      it.SetKey(_TEXT("INT_VALUE"));
      Test row = *it;
      tcout << _TEXT("Initial value: ") << row << endl;
      row.dbl_val = nullable<double>(5);
      *it = row;

      it = view;
      it.SetKey(_TEXT("INT_VALUE"));
      assert(row == *it);
      tcout << _TEXT("Value after first update: ") << row << endl;
      
      row.dbl_val = nullable<double>();  // override value, set to NULL
      *it = row;

      it = view;
      it.SetKey(_TEXT("INT_VALUE"));
      assert(row == *it);
      tcout << _TEXT("Value after second update: ") << row << endl;
   }

   tcout << endl;

#ifdef FIX_NULL_DELETES
   {
      DBStmt(_TEXT("delete from test_nullable")).Execute();
      DBStmt(_TEXT("insert into test_nullable (int_value, double_value) values(NULL, NULL)")).Execute();
      DBStmt(_TEXT("insert into test_nullable (int_value, double_value) values(23, NULL)")).Execute();
      DBStmt(_TEXT("insert into test_nullable (int_value, double_value) values(NULL, 48)")).Execute();
      DBStmt(_TEXT("insert into test_nullable (int_value, double_value) values(23, 48)")).Execute();
      tcout << _TEXT("Testing delete_iterator") << endl;

      unsigned int rowbuf; // for the benefit of BCA
      DBView<unsigned int> check("test_nullable", BCA(rowbuf, COLS["COUNT(*)"] >> rowbuf));
      DBView<unsigned int>::select_iterator checkit = check.begin();
      assert(checkit != check.end() && *checkit == 4);
      
      DBView<Test> view(_TEXT("test_nullable"));
      
      DBView<Test>::delete_iterator it = view;
      Test row;
      unsigned int rows = 4;
      tcout << _TEXT("Deleting the 23,48") << endl;
      row.int_val = 23;
      row.dbl_val = 48;
      *it = row;
      checkit = check.begin();   // reread row count
      assert(checkit != check.end() && *checkit == --rows);
      tcout << _TEXT("Deleting the null,null") << endl;
      row.int_val.reset();
      row.dbl_val.reset();
      *it = row;
      checkit = check.begin();   // reread row count
      assert(checkit != check.end() && *checkit == --rows);
      tcout << _TEXT("Deleting the 23,null") << endl;
      row.int_val = 23;
      *it = row;
      checkit = check.begin();   // reread row count
      assert(checkit != check.end() && *checkit == --rows);
      tcout << _TEXT("Deleting the null,48") << endl;
      row.int_val.reset();
      row.dbl_val = 48;
      *it = row;
      checkit = check.begin();   // reread row count
      assert(checkit != check.end() && *checkit == --rows);
      exit(1);
   }
#endif

   DBConnection::GetDefaultConnection().CommitAll();
}

