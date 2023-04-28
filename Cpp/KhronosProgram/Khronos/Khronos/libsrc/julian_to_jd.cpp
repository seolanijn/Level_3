/**	@file Julian_to_jd.cpp
	@author Garth Santor
	@date 2012-09-14

	Date and time library Julian conversion implementations.
	*/

#include <khronos/julian_calendar.hpp>
using namespace khronos;

jd_t khronos::julian_to_jd(year_t year, month_t month, day_t day)
{
	int a = (14 - month) / 12;
	year_t y = year + 4800 - a;
	month_t m = month + 12 * a - 3;
	return day + (153 * m + 2) / 5 + 365 * y + y / 4 - 32083.5;
}

jd_t khronos::julian_to_jd(year_t year, month_t month, day_t day, hour_t hour, minute_t minute, second_t seconds)
{
	jd_t jdn = julian_to_jd(year, month, day);
	tod_t tod_ = tod(hour, minute, seconds);
	return jdn + tod_;
}

void khronos::jd_to_julian(jd_t jd, year_t& year, month_t& month, day_t& day)
{
	long long a = (long long)floor(jd + 0.5);
	long long b = a + 1524;
	long long c = (long long)floor((b - 122.1) / 365.25);
	long long d = (long long)floor(365.25 * c);
	long long e = (long long)floor((b - d) / 30.6001);
	
	if (e < 14) month = (month_t)e - 1;
	else month = (month_t)e - 13;

	if (month > 2) year = c - 4716;
	else year = c - 4715;

	day = (day_t)b - (day_t)d - (day_t)floor(30.6001 * e);
}
void khronos::jd_to_julian(jd_t jd, year_t& year, month_t& month, day_t& day, hour_t& hour, minute_t& minute, second_t& seconds)
{
	jd_to_julian(jd, year, month, day);

	jd_t frac = jd + 0.5 - floor(jd + 0.5);

	hour = (hour_t)frac * 24;
	minute = hour / 60;
	seconds = floor(frac * 24 / 60 / 60);
}
/** Julian + (integer year) operator. */
Julian khronos::operator + (Julian const& dt, detail::packaged_year_integer const& year) {
	year_t y = dt.year() + year.nYears_;
	month_t m = dt.month();
	day_t d = dt.day();

	if (m == FEB && d == 29 && !is_julian_leapyear(y))
		d = 28;


	return Julian(y, m, d);
}



/** Julian + (integer month) operator. */
Julian khronos::operator + (Julian const& dt, detail::packaged_month_integer const& month) {
	year_t yearsToAdd = month.nMonths_ / 12;
	month_t monthsToAdd = month.nMonths_ % 12;
	year_t y = dt.year() + yearsToAdd;
	month_t m = dt.month() + monthsToAdd;

	int adjustment = (m - 1) / 12 + (m - 12) / 12;
	y += adjustment;
	m -= month_t(adjustment * 12);

	day_t d = std::min(dt.day(), julian_days_in_month(m, is_julian_leapyear(y)));

	return Julian(y, m, d);
}