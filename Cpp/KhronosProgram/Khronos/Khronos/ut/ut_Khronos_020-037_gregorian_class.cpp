/*	ut_Khronos_020-037_gregorian_class.cpp

	Khronos library Gregorian class unit test.

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

constexpr double oneThird = 1.0 / 3.0;

#if PHASE_20
TEST_PHASE(ut020_gregorian_differences, oneThird) {
	using namespace khronos;
	Gregorian past(1858, 11, 16);
	Gregorian future(2132, 8, 31);

	GATS_CHECK(future - past == 100'000);
	GATS_CHECK(past - future == -100'000);

	Gregorian start(2000, 1, 1);
	Gregorian end(2001, 1, 1);

	GATS_CHECK(end - start == 366);
	GATS_CHECK(start - end == -366);
}
#else 
EMPTY_PHASE(ut020_gregorian_differences, oneThird)
#endif





#if PHASE_21
/* Default constructor initializes to current time. */
TEST_PHASE(ut021_test_Gregorian_ctor_default, oneThird) {
	using namespace khronos;
	time_t utcTime = time(NULL);
	Gregorian const now;
	Gregorian const today(NOTIMEOFDAY);
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

	Gregorian localDate(
		year_t(tmLocal.tm_year) + 1900,
		month_t(tmLocal.tm_mon) + 1,
		day_t(tmLocal.tm_mday)
		);

	GATS_CHECK(localDateTime == now);
	GATS_CHECK(localDate == today);
}
#else 
EMPTY_PHASE(ut021_test_Gregorian_ctor_default, oneThird)
#endif



#if PHASE_22
TEST_PHASE(ut022_test_gregorian_ctor_literal, oneThird) {
	using namespace khronos;
	Gregorian const	julianEpoch(-4713, November, 24);
	GATS_CHECK(-4713 == julianEpoch.year());
	GATS_CHECK(November == julianEpoch.month());
	GATS_CHECK(24 == julianEpoch.day());

	Gregorian const greatWar(1918, NOV, 11, 11, 11, 11);
	GATS_CHECK(1918 == greatWar.year());
	GATS_CHECK(November == greatWar.month());
	GATS_CHECK(11 == greatWar.day());
	GATS_CHECK(11 == greatWar.hour());
	GATS_CHECK(11 == greatWar.minute());
	GATS_CHECK(11 == greatWar.second());
}
#else 
EMPTY_PHASE(ut022_test_gregorian_ctor_literal, oneThird)
#endif



#if PHASE_23
TEST_PHASE(ut023_test_Gregorian_ctor_from_Jd, oneThird) {
	using namespace khronos;
	Gregorian g(Jd(-0.5));
	GATS_CHECK_EQUAL(g.year(), -4713);
	GATS_CHECK_EQUAL(g.month(), November);
	GATS_CHECK_EQUAL(g.day(), 24);
	GATS_CHECK_EQUAL(g.hour(), 0);
	GATS_CHECK_EQUAL(g.minute(), 0);
	GATS_CHECK_EQUAL(g.second(), 0);
}
#else 
EMPTY_PHASE(ut023_test_Gregorian_ctor_from_Jd, oneThird)
#endif



#if PHASE_24
TEST_PHASE(ut024_test_Gregorian_cast_to_Jd, oneThird) {
	using namespace khronos;
	Gregorian const	julianEpoch(-4713, November, 24);
	Jd je = julianEpoch;
	GATS_CHECK_EQUAL(je.jd(), -0.5);
}
#else 
EMPTY_PHASE(ut024_test_Gregorian_cast_to_Jd, oneThird)
#endif



#if PHASE_25
TEST_PHASE(ut025_test_Gregorian_assignment_from_Jd, oneThird) {
	using namespace khronos;
	Gregorian g;
	g = Jd(-0.5);
	GATS_CHECK_EQUAL(g.year(), -4713);
	GATS_CHECK_EQUAL(g.month(), November);
	GATS_CHECK_EQUAL(g.day(), 24);
	GATS_CHECK_EQUAL(g.hour(), 0);
	GATS_CHECK_EQUAL(g.minute(), 0);
	GATS_CHECK_EQUAL(g.second(), 0);
}
#else 
EMPTY_PHASE(ut025_test_Gregorian_assignment_from_Jd, oneThird)
#endif



#if PHASE_26
TEST_PHASE(ut026_test_Gregorian_add_days, oneThird) {
	using namespace khronos;
	Gregorian date(2000, January, 1);
	Gregorian res = date + days(10);
	GATS_CHECK(res == Gregorian(2000, January, 11));
	res = res - days(10);
	GATS_CHECK(res == date);
}
#else 
EMPTY_PHASE(ut026_test_Gregorian_add_days, oneThird)
#endif



#if PHASE_27
TEST_PHASE(ut027_test_Gregorian_add_days_with_time, oneThird) {
	using namespace khronos;
	Gregorian date(2000, January, 1, 12, 13, 14);
	Gregorian res = date + days(10);
	GATS_CHECK(res == Gregorian(2000, January, 11, 12, 13, 14));
	res = res - days(10);
	GATS_CHECK(res == date);
}
#else 
EMPTY_PHASE(ut027_test_Gregorian_add_days_with_time, oneThird)
#endif



#if PHASE_28
TEST_PHASE(ut028_test_Gregorian_add_weeks, oneThird) {
	using namespace khronos;
	Gregorian date(2000, JAN, 1);
	Gregorian res = date + weeks(10);
	GATS_CHECK(date + days(70) == res);
	res = res - weeks(10);
	GATS_CHECK(date == res);
}
#else 
EMPTY_PHASE(ut028_test_Gregorian_add_weeks, oneThird)
#endif



#if PHASE_29
TEST_PHASE(ut029_test_Gregorian_add_weeks_with_time, oneThird) {
	using namespace khronos;
	Gregorian date(2000, JAN, 1, 6, 7, 8);
	Gregorian res = date + weeks(10);
	GATS_CHECK(date + days(70) == res);
	res = res - weeks(10);
	GATS_CHECK(date == res);
}
#else 
EMPTY_PHASE(ut029_test_Gregorian_add_weeks_with_time, oneThird)
#endif



#if PHASE_30
TEST_PHASE(ut030_test_Gregorian_add_months, oneThird) {
	using namespace khronos;
	{	Gregorian start(2000, JAN, 1);
	Gregorian end(2000, FEB, 1);
	Gregorian res = start + months(1);
	GATS_CHECK(end == res);
	res = end - months(1);
	GATS_CHECK(res == start);
	}
	{	Gregorian start(2000, JAN, 1, 12, 13, 14);
	Gregorian end(2001, FEB, 1, 12, 13, 14);
	Gregorian res = start + months(13);
	GATS_CHECK(end == res);
	res = end - months(13);
	GATS_CHECK(res == start);
	}

	// shouldn't compile
	// start + months(1.2);
}
#else 
EMPTY_PHASE(ut030_test_Gregorian_add_months, oneThird)
#endif



#if PHASE_31
TEST_PHASE(ut031_test_Gregorian_add_months_w_carry, oneThird) {
	using namespace khronos;

	{	Gregorian start(2000, OCT, 1);
	Gregorian end(2001, FEB, 1);
	Gregorian res = start + months(4);
	GATS_CHECK(end == res);
	res = end - months(4);
	GATS_CHECK(res == start);
	}
	{	Gregorian start(2000, OCT, 1, 6, 7, 8);
	Gregorian end(2003, FEB, 1, 6, 7, 8);
	Gregorian res = start + months(28);
	GATS_CHECK(end == res);
	res = end - months(28);
	GATS_CHECK(res == start);
	}
}
#else 
EMPTY_PHASE(ut031_test_Gregorian_add_months_w_carry, oneThird)
#endif



#if PHASE_32
TEST_PHASE(ut032_test_Gregorian_add_month_leap, oneThird) {
	using namespace khronos;

	{	Gregorian start(2000, February, 29, 12, 13, 14);
	Gregorian end(2000, March, 29, 12, 13, 14);
	Gregorian res = start + months(1);
	GATS_CHECK(end == res);
	res = end - months(1);
	GATS_CHECK(res == start);
	}
	{	Gregorian start(2000, January, 31, 6, 7, 8);
	Gregorian end(2000, February, 29, 6, 7, 8);
	Gregorian res = start + months(1);
	GATS_CHECK(end == res);
	res = end - months(1);
	GATS_CHECK(res == Gregorian(2000, January, 29, 6, 7, 8));
	}
}
#else 
EMPTY_PHASE(ut032_test_Gregorian_add_month_leap, oneThird)
#endif



#if PHASE_33
TEST_PHASE(ut033_test_Gregorian_add_years, oneThird) {
	using namespace khronos;
	Gregorian date(2000, January, 1, 6, 7, 8);
	Gregorian res = date + years(10);
	GATS_CHECK(Gregorian(2010, January, 1, 6, 7, 8) == res);
	res = res - years(10);
	GATS_CHECK(res == date);

	// The following won't compile (we made the operator private)
	//res = date + years(10.4);
}
#else 
EMPTY_PHASE(ut033_test_Gregorian_add_years, oneThird)
#endif



#if PHASE_34
TEST_PHASE(ut034_test_Gregorian_add_years_leapyear, oneThird) {
	using namespace khronos;
	Gregorian date(2000, February, 29, 12, 13, 14);
	Gregorian later = date + years(2);
	GATS_CHECK(Gregorian(2002, February, 28, 12, 13, 14) == later);
}
#else 
EMPTY_PHASE(ut034_test_Gregorian_add_years_leapyear, oneThird)
#endif



#if PHASE_35
TEST_PHASE(ut035_test_Gregorian_relational, oneThird) {
	using namespace khronos;
	Gregorian low(2009, January, 1);
	Gregorian high(2009, December, 31);
	Gregorian copyLow(low);

	GATS_CHECK(low == copyLow);
	GATS_CHECK(low < high);
	GATS_CHECK(low <= copyLow);
	GATS_CHECK(high >= low);
	GATS_CHECK(high > low);
	GATS_CHECK(high != low);
}
#else 
EMPTY_PHASE(ut035_test_Gregorian_relational, oneThird)
#endif



#if PHASE_36
TEST_PHASE(ut036_test_Gregorian_element_functions, oneThird) {
	using namespace khronos;
	Gregorian g(2010, September, 12, 23, 45, 8);
	GATS_CHECK_EQUAL(year(g), 2010);
	GATS_CHECK_EQUAL(month(g), September);
	GATS_CHECK_EQUAL(day(g), 12);
	GATS_CHECK_EQUAL(hour(g), 23);
	GATS_CHECK_EQUAL(minute(g), 45);
	GATS_CHECK_EQUAL(second(g), 8);
}
#else 
EMPTY_PHASE(ut036_test_Gregorian_element_functions, oneThird)
#endif




#if PHASE_37
TEST_PHASE(ut037_test_Gregorian_to_string, oneThird) {
	using namespace khronos;
	GATS_CHECK_EQUAL(Gregorian(1000, January, 1).to_string(), std::string("Wednesday, January 1 1000 CE, 12:00:00 am"));
	GATS_CHECK_EQUAL(Gregorian(2000, February, 29, 12, 30, 11).to_string(), std::string("Tuesday, February 29 2000 CE, 12:30:11 pm"));
	GATS_CHECK_EQUAL(Gregorian(2010, September, 12, 23, 45, 8).to_string(), std::string("Sunday, September 12 2010 CE, 11:45:08 pm"));
	GATS_CHECK_EQUAL(Gregorian(101_BCE, January, 1, 6, 7, 8).to_string(), std::string("Monday, January 1 101 BCE, 6:07:08 am"));

	std::ostringstream oss;
	oss << Gregorian(2010, September, 12, 23, 45, 8);
	GATS_CHECK_EQUAL(oss.str(), std::string("Sunday, September 12 2010 CE, 11:45:08 pm"));
}
#else 
EMPTY_PHASE(ut037_test_Gregorian_to_string, oneThird)
#endif