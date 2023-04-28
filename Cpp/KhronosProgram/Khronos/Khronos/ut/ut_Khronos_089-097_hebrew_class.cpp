/*	ut_Khronos_089-097_hebrew_class.cpp

	Khronos library Hebrew Calendar class unit test.

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
#include <ctime>
using namespace std;

constexpr double oneNinth = 1.0 / 9.0;


/** Default constructor initializes to current time. */
#if PHASE_89 
TEST_PHASE(ut089_test_Hebrew_ctor_default, oneNinth) {
	using namespace khronos;
	time_t utcTime = time(NULL);
	Hebrew const now;
	Hebrew const today(NOTIMEOFDAY);
	struct tm tmLocal;
	localtime_s(&tmLocal, &utcTime);

	Gregorian localDateTime(
		year_t(tmLocal.tm_year) + 1900,
		month_t(tmLocal.tm_mon) + 1,
		day_t(tmLocal.tm_mday),
		hour_t(tmLocal.tm_hour),
		minute_t(tmLocal.tm_min),
		second_t(tmLocal.tm_sec)
		);

	// Hebrew days start at noon
	Gregorian localDate(
		year_t(tmLocal.tm_year) + 1900,
		month_t(tmLocal.tm_mon) + 1,
		day_t(tmLocal.tm_mday)
		);

	GATS_CHECK(localDateTime == now);
	GATS_CHECK(localDate == today);
}
#else
EMPTY_PHASE(ut089_test_Hebrew_ctor_default,oneNinth)
#endif



#if PHASE_90
TEST_PHASE(ut090_test_Hebrew_ctor_literal, oneNinth) {
	using namespace khronos;
	Hebrew const	hebrewEpoch(1, Nisan, 1);
	GATS_CHECK(1 == hebrewEpoch.year());
	GATS_CHECK(Nisan == hebrewEpoch.month());
	GATS_CHECK(1 == hebrewEpoch.day());

	Hebrew const aDay(5774, Heshvan, 16, 12, 30, 45);
	GATS_CHECK(5774 == aDay.year());
	GATS_CHECK(Heshvan == aDay.month());
	GATS_CHECK(16 == aDay.day());
	GATS_CHECK(12 == aDay.hour());
	GATS_CHECK(30 == aDay.minute());
	GATS_CHECK(45 == aDay.second());
}
#else
EMPTY_PHASE(ut090_test_Hebrew_ctor_literal,oneNinth)
#endif



#if PHASE_91 
TEST_PHASE(ut091_test_Hebrew_ctor_from_Jd, oneNinth) {
	using namespace khronos;
	Hebrew i(Jd(0.0));
	GATS_CHECK_EQUAL(i.year(), -952);
	GATS_CHECK_EQUAL(i.month(), Teveth);
	GATS_CHECK_EQUAL(i.day(), 21);
	GATS_CHECK_EQUAL(i.hour(), 12);
	GATS_CHECK_EQUAL(i.minute(), 0);
	GATS_CHECK_EQUAL(i.second(), 0);
}
#else
EMPTY_PHASE(ut091_test_Hebrew_ctor_from_Jd,oneNinth)
#endif



#if PHASE_92
TEST_PHASE(ut092_test_Hebrew_cast_to_Jd, oneNinth) {
	using namespace khronos;
	Hebrew const	i(-952, Teveth, 21, 12, 0, 0);
	Jd je = i;
	GATS_CHECK_EQUAL(je.jd(), 0.0);
}
#else
EMPTY_PHASE(ut092_test_Hebrew_cast_to_Jd,oneNinth)
#endif



#if PHASE_93 
TEST_PHASE(ut093_test_Hebrew_assignment_from_Jd, oneNinth) {
	using namespace khronos;
	Hebrew i;
	i = Jd(0.0);
	GATS_CHECK_EQUAL(i.year(), -952);
	GATS_CHECK_EQUAL(i.month(), Teveth);
	GATS_CHECK_EQUAL(i.day(), 21);
	GATS_CHECK_EQUAL(i.hour(), 12);
	GATS_CHECK_EQUAL(i.minute(), 0);
	GATS_CHECK_EQUAL(i.second(), 0);
}
#else
EMPTY_PHASE(ut093_test_Hebrew_assignment_from_Jd,oneNinth)
#endif



#if PHASE_94
TEST_PHASE(ut094_test_Hebrew_add_days, oneNinth) {
	using namespace khronos;
	Hebrew date(5774, Heshvan, 16);
	Hebrew res = date + days(100);
	GATS_CHECK(res == Hebrew(5774, Shevat, 27));
	res = res - days(100);
	GATS_CHECK(res == date);
}
#else
EMPTY_PHASE(ut094_test_Hebrew_add_days,oneNinth)
#endif



#if PHASE_95
TEST_PHASE(ut095_test_Hebrew_add_weeks, oneNinth) {
	using namespace khronos;
	Hebrew date(5774, Heshvan, 16);
	Hebrew res = date + weeks(10);
	GATS_CHECK(date + days(70) == res);
	res = res - weeks(10);
	GATS_CHECK(date == res);
}
#else
EMPTY_PHASE(ut095_test_Hebrew_add_weeks,oneNinth)
#endif



#if PHASE_96
TEST_PHASE(ut096_test_Hebrew_relational, oneNinth) {
	using namespace khronos;
	Hebrew low(5774, Teveth, 1);
	Hebrew high(5774, Shevat, 29);
	Hebrew copyLow(low);

	GATS_CHECK(low == copyLow);
	GATS_CHECK(low < high);
	GATS_CHECK(low <= copyLow);
	GATS_CHECK(high >= low);
	GATS_CHECK(high > low);
	GATS_CHECK(high != low);
}
#else
EMPTY_PHASE(ut096_test_Hebrew_relational,oneNinth)
#endif



#if PHASE_97
TEST_PHASE(ut097_test_Hebrew_to_string, oneNinth) {
	using namespace khronos;
	GATS_CHECK_EQUAL(Hebrew(1000, Nisan, 1).to_string(), std::string("Nisan 1 1000, 12:00:00 am"));
	GATS_CHECK_EQUAL(Hebrew(2000, Shevat, 29, 12, 0, 0).to_string(), std::string("Shevat 29 2000, 12:00:00 pm"));
	GATS_CHECK_EQUAL(Hebrew(2010, Heshvan, 12, 23, 45, 8).to_string(), std::string("Heshvan 12 2010, 11:45:08 pm"));
	GATS_CHECK_EQUAL(Hebrew(-100, Nisan, 1, 6, 7, 8).to_string(), std::string("Nisan 1 -100, 6:07:08 am"));

	std::ostringstream oss;
	oss << Hebrew(2010, Heshvan, 12, 23, 45, 8);
	GATS_CHECK_EQUAL(oss.str(), std::string("Heshvan 12 2010, 11:45:08 pm"));
}
#else
EMPTY_PHASE(ut097_test_Hebrew_to_string,oneNinth)
#endif