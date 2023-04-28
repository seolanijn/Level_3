#pragma once
/*	gats/win32/ConsoleApp.hpp

	ConsoleApp class declaration.

=============================================================
Revision History
-------------------------------------------------------------
Version 2021.08.24
	C++ 17 Version
	Moved to namespace gats::win32

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


#include <gats/win32/ConsoleApp/ConsoleEnhanced.hpp>
#include <string>
#include <vector>


namespace gats::win32 {

	/*!	ConsoleApp is the base-class of the framework.
	*/
	class ConsoleApp {
		using Args = std::vector<std::wstring>;

		static ConsoleApp* thisApp_sm;
		Args								args_m;

		bool wrapupCalled_m = false;
		bool autoRestoreWindowStateOnExit_m = false;

	protected:
		gats::win32::ConsoleEnhanced::State	initialState_m;

		gats::win32::ConsoleEnhanced console;

		ConsoleApp(bool autoRestoreWindowStateOnExit = false);
		virtual ~ConsoleApp();

		virtual void setup() {}
		virtual int execute();
		virtual void wrapup() {}

		/*!	Access the command-line arguments container.
		*/
		Args const& get_args() const { return args_m; }
	public:
		static int wmain(int argc, wchar_t* argv[]);
	};


} // end-of-namespace gats::win32

#pragma endregion
