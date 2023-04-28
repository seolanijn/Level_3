/*	ut_khronos_039-041_julian_concepts.cpp
* 
	Khronos library Julian Calendar concepts unit test.

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
#include <array>




#if PHASE_39
TEST_PHASE(ut039_test_julian_leapyear,3) {
	using namespace khronos;
	year_t y = 2000;
	while (y > -4800) {
		GATS_CHECK_EQUAL(is_julian_leapyear(y--), true);
		GATS_CHECK_EQUAL(is_julian_leapyear(y--), false);
		GATS_CHECK_EQUAL(is_julian_leapyear(y--), false);
		GATS_CHECK_EQUAL(is_julian_leapyear(y--), false);
	}
}
#else 
EMPTY_PHASE(ut039_test_julian_leapyear,3)
#endif



#if PHASE_40
TEST_PHASE(ut040_test_Julian_days_of_month,3) {
	using namespace khronos;
	GATS_CHECK_EQUAL(julian_days_in_month(JAN, false), 31);
	GATS_CHECK_EQUAL(julian_days_in_month(FEB, false), 28);
	GATS_CHECK_EQUAL(julian_days_in_month(MAR, false), 31);
	GATS_CHECK_EQUAL(julian_days_in_month(APR, false), 30);
	GATS_CHECK_EQUAL(julian_days_in_month(MAY, false), 31);
	GATS_CHECK_EQUAL(julian_days_in_month(JUN, false), 30);
	GATS_CHECK_EQUAL(julian_days_in_month(JUL, false), 31);
	GATS_CHECK_EQUAL(julian_days_in_month(AUG, false), 31);
	GATS_CHECK_EQUAL(julian_days_in_month(SEP, false), 30);
	GATS_CHECK_EQUAL(julian_days_in_month(OCT, false), 31);
	GATS_CHECK_EQUAL(julian_days_in_month(NOV, false), 30);
	GATS_CHECK_EQUAL(julian_days_in_month(DEC, false), 31);

	GATS_CHECK_EQUAL(julian_days_in_month(JAN, true), 31);
	GATS_CHECK_EQUAL(julian_days_in_month(FEB, true), 29);
	GATS_CHECK_EQUAL(julian_days_in_month(MAR, true), 31);
	GATS_CHECK_EQUAL(julian_days_in_month(APR, true), 30);
	GATS_CHECK_EQUAL(julian_days_in_month(MAY, true), 31);
	GATS_CHECK_EQUAL(julian_days_in_month(JUN, true), 30);
	GATS_CHECK_EQUAL(julian_days_in_month(JUL, true), 31);
	GATS_CHECK_EQUAL(julian_days_in_month(AUG, true), 31);
	GATS_CHECK_EQUAL(julian_days_in_month(SEP, true), 30);
	GATS_CHECK_EQUAL(julian_days_in_month(OCT, true), 31);
	GATS_CHECK_EQUAL(julian_days_in_month(NOV, true), 30);
	GATS_CHECK_EQUAL(julian_days_in_month(DEC, true), 31);
}
#else 
EMPTY_PHASE(ut040_test_Julian_days_of_month,3)
#endif



#if PHASE_41
TEST_PHASE(ut041_test_Julian_month_name,3) {
	using namespace khronos;
	static std::array<std::string const, 13> const names = { "", "January", "February", "March",
		"April", "May", "June", "July", "August", "September", "October",
		"November", "December" };

	for (month_t i = 1; i <= 12; ++i)
		GATS_CHECK(julian_month_name(i) == names[i]);

	for (month_t i = 1; i <= 12; ++i) {
		auto s = names[i].substr(0, 3);
		s[1] = (char)toupper(s[1]);
		s[2] = (char)toupper(s[2]);
		GATS_CHECK(julian_short_month_name(i) == s);
	}
}
#else 
EMPTY_PHASE(ut041_test_Julian_month_name,3)
#endif