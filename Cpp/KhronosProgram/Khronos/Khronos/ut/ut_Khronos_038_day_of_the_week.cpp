/*	ut_khronos_038_day_of_the_week.cpp

	Khronos library day-of-the-week unit test.

=============================================================
Revision History
-------------------------------------------------------------

Version 2021.09.25
	Replaced BOOST's test framework with gats_test framework.

Version 2016.09.30
	Added: Phase List

Version 2015.09.21
	Initial Release


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

#include <khronos.hpp>
#include "ut_Khronos.hpp"



#if PHASE_38
/** Day-of-week from JD. */
TEST_PHASE(ut038_day_of_week_base_func_test,3) {
	using namespace khronos;
	Gregorian g(2012_CE, September, 24);  // a monday
	Jd jd = g;

	for (double offset = 0.0; offset < 1.0; offset += 0.01)
		GATS_CHECK(day_of_week(jd.jd() + offset) == Monday);

	GATS_CHECK(day_of_week(jd.jd() + 1.0) == Tuesday);
}
#else 
EMPTY_PHASE(ut038_day_of_week_base_func_test,3)
#endif