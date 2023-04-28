/*	ConsoleApplication.cpp

	ConsoleApplication class implementation.

=============================================================
Revision History
-------------------------------------------------------------

Version 2021.08.24
	C++ 17 Version

Version 2019.01.05
	C++ 11 Version

Version 2017.02.25
	Alpha release.

=============================================================

Copyright 2017-21, Garth Santor & Trinh Han

The copyright to the computer program(s) herein
is the property of Garth Santor/Trinh Han, Canada.
The program(s) may be used and/or copied only with
the written permission of Garth Santor/Trinh Han
or in accordance with the terms and conditions
stipulated in the agreement/contract under which
the program(s) have been supplied.
=============================================================*/


#include <gats/win32/ConsoleApp.hpp>
#include <iostream>
#include <stdexcept>
using namespace std;



/*!	Process entry point.
* 
	Calls the ConsoleApp singleton's wmain and traps unhandled exceptions.
*/
int wmain(int argc, wchar_t* argv[]) try {
	// report unhandled exceptions
	std::set_terminate([]() { std::wcerr << "Error (wmain): Unhandled exception\n"; std::exit(EXIT_FAILURE); });

	// Enable CRT memory leak checking.
#ifdef _DEBUG
	int dbgFlags = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
	dbgFlags |= _CRTDBG_CHECK_ALWAYS_DF;
	dbgFlags |= _CRTDBG_DELAY_FREE_MEM_DF;
	dbgFlags |= _CRTDBG_LEAK_CHECK_DF;
	_CrtSetDbgFlag(dbgFlags);

	// redirect leak warnings to console as well as debug window
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG | _CRTDBG_MODE_FILE );
	_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);
#endif

	// transfer execution to the ConsoleApp object.
	return gats::win32::ConsoleApp::wmain(argc, argv);
}
catch (gats::win32::XError& xe) {
	wcerr << xe.message() << endl;
}
catch (char const* msg) {
	wcerr << L"Error (wmain) caught exception string: " << msg << endl;
	return EXIT_FAILURE;
}
catch (std::logic_error& e) {
	wcerr << L"Error (wmain) caught std::logic_error: " << e.what() << endl;
	return EXIT_FAILURE;
}
catch (std::runtime_error& e) {
	wcerr << L"Error (wmain) caught std::runtime_error: " << e.what() << endl;
	return EXIT_FAILURE;
}
catch (std::exception& e) {
	wcerr << L"Error (wmain) caught std::exception: " << e.what() << endl;
	return EXIT_FAILURE;
}



namespace gats::win32 {
	/*!	ConsoleApplication singleton instance pointer.
	*/
	ConsoleApp* ConsoleApp::thisApp_sm = nullptr;



	/*!	wmain runs the application sandwich (setup | execute | wrapup).
	*/
	int ConsoleApp::wmain(int argc, wchar_t* argv[]) {

		// check to see that a ConsoleApp derived object was created.
		if (thisApp_sm == nullptr)
			throw "no ConsoleApp derived class instantiated.";

		// Collect the command-line arguments.
		thisApp_sm->args_m.assign(argv, argv + argc);

		// Save the console state
		if (thisApp_sm->autoRestoreWindowStateOnExit_m)
			thisApp_sm->initialState_m = thisApp_sm->console.GetState();

		// Reconfigure the console
		thisApp_sm->setup();

		// Execute the application
		auto executionResult = thisApp_sm->execute();

		thisApp_sm->wrapup();
		thisApp_sm->wrapupCalled_m = true;

		return executionResult;
	}



	/*!	Singleton initialization and confirmation.
		Throws a logic_error if multiple instances are created.
	*/
	ConsoleApp::ConsoleApp(bool autoRestoreWindowStateOnExit)
		: autoRestoreWindowStateOnExit_m(autoRestoreWindowStateOnExit)
	{
		if (thisApp_sm)
			throw std::logic_error("Error: ConsoleApp already initialized!");
		thisApp_sm = this;
	}



	/*!	Singleton termination.  Restore the state of the console to its original.
	*/
	ConsoleApp::~ConsoleApp() {
		// Call the wrapup if it hasn't yet happened.
		if (!wrapupCalled_m)
			this->wrapup();

		// Restore the console state
		if (autoRestoreWindowStateOnExit_m)
			console.SetState(initialState_m);
	}




	/*!	execute the application.
		Override this method in the derived class.
	*/
	int ConsoleApp::execute() {
		wcout << "Console application framework: (c) 2017-2021, Garth Santor\n";
		return EXIT_SUCCESS;
	}


} // end-of-namespace gats::win32