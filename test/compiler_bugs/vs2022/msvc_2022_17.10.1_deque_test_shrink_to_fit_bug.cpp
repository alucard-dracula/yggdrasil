//safe_deque_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <deque>

//Microsoft Visual Studio Enterprise 2022
//Version 17.10.1
//VisualStudio.17.Release/17.10.1+34928.147
//Microsoft .NET Framework
//Version 4.8.09037
//
//Installed Version: Enterprise
//
//Visual C++ 2022   00482-10000-00261-AA639
//Microsoft Visual C++ 2022
//
//ASP.NET and Web Tools   17.10.338.1105
//ASP.NET and Web Tools
//
//Azure App Service Tools v3.0.0   17.10.338.1105
//Azure App Service Tools v3.0.0
//
//C# Tools   4.10.0-3.24270.2+e8f775c1d8a73dee7ad02408712d714251e708ea
//C# components used in the IDE. Depending on your project type and settings, a different version of the compiler may be used.
//
//GitHub Copilot   0.2.889.30432
//GitHub Copilot is an AI pair programmer that helps you write code faster and with less work.
//
//Intel® C++ Compiler   2024.0
//Intel® C++ Compiler 2024.0, Copyright © 2002-2023 Intel Corporation. All rights reserved.
//
//Intel® Libraries for oneAPI   2024.0
//Intel® Libraries for oneAPI 2024.0, Copyright © 2019-2023 Intel Corporation. All rights reserved.
//
//Intel® oneAPI DPC++ Compiler   2024.0
//Intel® oneAPI DPC++ Compiler 2024.0, Copyright © 2019-2023 Intel Corporation. All rights reserved.
//
//Intel® oneAPI Menu & Samples   10.10.392.9731
//Intel® oneAPI Visual Studio Menu & Samples Extension
//
//Microsoft JVM Debugger   1.0
//Provides support for connecting the Visual Studio debugger to JDWP compatible Java Virtual Machines
//
//NuGet Package Manager   6.10.0
//NuGet Package Manager in Visual Studio. For more information about NuGet, visit https://docs.nuget.org/
//
//Test Adapter for Boost.Test   1.0
//Enables Visual Studio's testing tools with unit tests written for Boost.Test.  The use terms and Third Party Notices are available in the extension installation directory.
//
//Test Adapter for Google Test   1.0
//Enables Visual Studio's testing tools with unit tests written for Google Test.  The use terms and Third Party Notices are available in the extension installation directory.
//
//TypeScript Tools   17.0.30327.2001
//TypeScript Tools for Microsoft Visual Studio
//
//Visual Basic Tools   4.10.0-3.24270.2+e8f775c1d8a73dee7ad02408712d714251e708ea
//Visual Basic components used in the IDE. Depending on your project type and settings, a different version of the compiler may be used.
//
//Visual Studio IntelliCode   2.2
//AI-assisted development for Visual Studio.


#define TEST_DEATH_LOOP() 1

int main(int argc, char* argv[])
{
	{
		typedef std::deque<int> now_cont_type;
		now_cont_type base_cont1;;
		base_cont1.emplace_back();
		base_cont1.emplace_front();
		base_cont1.erase(base_cont1.begin(), base_cont1.begin() + 1);

#if TEST_DEATH_LOOP()
		std::cout << "death loop start" << std::endl;
		base_cont1.shrink_to_fit(); //death loop 
		std::cout << "death loop end" << std::endl;
#endif // TEST_DEATH_LOOP()
	}

	{
		typedef std::deque<int> now_cont_type;
		now_cont_type base_cont2;
		base_cont2.insert(base_cont2.begin(), 1, 0);
		base_cont2.insert(base_cont2.begin(), 1, 0);
		base_cont2.erase(base_cont2.begin(), base_cont2.begin() + 1);

#if TEST_DEATH_LOOP()
		std::cout << "death loop start" << std::endl;
		base_cont1.shrink_to_fit(); //also death loop 
		std::cout << "death loop end" << std::endl;
#endif // TEST_DEATH_LOOP()
	}

	{
		typedef std::deque<int> now_cont_type;
		now_cont_type base_cont1;;
		base_cont1.push_back(0);
		base_cont1.push_front(0);
		base_cont1.erase(base_cont1.begin(), base_cont1.begin() + 1);

#if TEST_DEATH_LOOP()
		std::cout << "death loop start" << std::endl;
		base_cont1.shrink_to_fit(); //death loop 
		std::cout << "death loop end" << std::endl;
#endif // TEST_DEATH_LOOP()
	}

	wait_any_key(argc, argv);
	return 0;
}
