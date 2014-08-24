// kill spurious VC 6 warnings
#ifdef  _MSC_VER
#pragma	warning(disable: 4786)
#pragma	warning(disable: 4503)  // Kill decorated names too long
#pragma	warning(disable: 4100)  // Kill unreferenced formal parameter
#pragma	warning(disable: 4511)  // Kill unable to create default copy constructor
#pragma	warning(disable: 4512)  // Kill unable to create default assingment operator
#pragma warning(disable: 4355)  // Kill 'this': used in base initializer list
#endif


#include <iostream>
#include <DTL.h>

using namespace dtl;
using namespace std;

void TestJoin()
{
   // first, ensure long long table names throw an exception
   bool test1 = false;
   try {
      DynamicDBView<> dv(_TEXT("DB_EXAMPLE")
            _TEXT("                                                 ")
            _TEXT("                                                 ")
            _TEXT("                                                 ")
            _TEXT("                                                 ")
            _TEXT("                                                 ")
            _TEXT("                                                 ")
            _TEXT("                                                 "),
            _TEXT("*"));
   }
	catch (ETIException&) {
      // good!
      test1 = true;
	}

   if (!test1)
   {
		tcout << _TEXT("EXCEPTION was not thrown!") << endl;
      exit(1);
   }

   // check the OnlyOneTable() function
   assert( OnlyOneTable(_TEXT("HELLO_THERE")) );
   assert( !OnlyOneTable(_TEXT("HELLO,THERE")) );
   tstring tables = _TEXT("DB_EXAMPLE LEFT JOIN DB_SAMPLE ON (SAMPLE_INT = INT_VALUE)");
   // check it correctly detects the JOIN
   assert( ! OnlyOneTable(tables) );

	try {
      // try a LEFT JOIN
      DynamicDBView<> view(tables,_TEXT("*"));

#if !defined (__BORLANDC__) // borland 5 seg faults here

      // select iterator should fail... TODO!!
      tcout << "With select_iterator: ";
      for (DynamicDBView<>::select_iterator i = view.begin();
            i != view.end(); ++i )
         tcout << (*i)[0] << " ";
      tcout << endl;
#endif

      // sql_iterator should be ok...
      tcout << _TEXT("With sql_iterator: ");
      for (DynamicDBView<>::select_iterator j = view.begin();
            j != view.end(); ++j )
         tcout << (*j)[0] << _TEXT(" ");
      tcout << endl;
	}
	catch (std::exception &ex) 	{
		std::cout << ex.what() << std::endl;
	}
}

