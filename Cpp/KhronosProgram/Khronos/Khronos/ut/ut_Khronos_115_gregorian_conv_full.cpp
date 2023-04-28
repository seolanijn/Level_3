/*	ut_khronos_115_gregorian_conv_full.cpp

	Khronos library jd to Gregorian functions unit test.

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
using namespace std;



#if defined(FULL_TEST) && PHASE_115
/**	Check all conversions from the lowest Proleptic Gregorian date, until a day well in the future. */
TEST_PHASE(ut115_test_gregorian_full,1) {
	using namespace khronos;

	cout << "Starting Gregorian FULL_TEST: ";
	auto low = gregorian_to_jd(4800_BCE, January, 1);
	auto high = gregorian_to_jd(4800_CE, January, 1);
	for (auto jd = low; jd <= high; jd += 0.125) {
		year_t year;
		month_t month;
		day_t day;
		jd_to_gregorian(jd, year, month, day);
		auto result = gregorian_to_jd(year, month, day);
		GATS_CHECK_EQUAL(jd_to_jdn(jd)-0.5, result);
		hour_t hours;
		minute_t minutes;
		second_t seconds;
		jd_to_gregorian(jd, year, month, day, hours, minutes, seconds);
		result = gregorian_to_jd(year, month, day, hours, minutes, seconds);
		GATS_CHECK_CLOSE_EPSILON(jd, result,0.000'01);
	}
	cout << "... completed\n";
}
#else 
EMPTY_PHASE(ut115_test_gregorian_full,1)
#endif
