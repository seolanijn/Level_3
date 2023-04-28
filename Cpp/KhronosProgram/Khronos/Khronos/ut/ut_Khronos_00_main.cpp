/*	ut_khronos_00_main.cpp

	ut_khronos_build_info testcase

=============================================================
Revision History
-------------------------------------------------------------

Version 2021.09.25
	Replaced BOOST's test framework with gats_test framework.

Version 2016.09.30
	Added: Phase List

Version 2015.09.21
	Inital Release


=============================================================

Copyright Garth Santor/Trinh Han

The copyright to the computer program(s) herein
is the property of Garth Santor/Trinh Han, Canada.
The program(s) may be used and/or copied only with
the written permission of Garth Santor/Trinh Han
or in accordance with the terms and conditions
stipulated in the agreement/contract under which
the program(s) have been supplied.
=============================================================*/

#include "ut_Khronos.hpp"
#include <iostream>
#include <iomanip>
using namespace std;



/** Unit test build info. */
GATS_TEST_CASE_WEIGHTED(ut00_build_info,0) {
	double mscVersion = _MSC_VER / 100.0;
	cout << "ut-khronos version " UT_VERSION "\n"
		"MSC Version: " << fixed << setprecision(2) << mscVersion << "\n"
		"Last Build: " __DATE__ " - " __TIME__ "\n" << endl;
}
