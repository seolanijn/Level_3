/**	@file Hebrew_to_jd.cpp
	@author Garth Santor
	@date 2012-09-14

	Hebrew calendar conversion implementations.
	*/

#include <khronos/hebrew_calendar.hpp>
using namespace khronos;


jd_t khronos::hebrew_to_jd(year_t year, month_t month, day_t day) {
	assert(year > -4800 && "Minimum year is 4800 BCE");
	assert(month >= 1 && "Minimum month is January = 1");
	assert(month <= 12 && "Maximum month is December = 12");
	assert(day >= 1 && "Minimum day of the month is 1");
	assert(day <= 31 && "Maximum day of the month is 31");

	long long a = (14 - month) / 12;
	long long y = year + 4800 - a;
	long long m = month + 12 * a - 3;
	return day + (153 * m + 2) / 5 + 365 * y + y / 4 - y / 100 + y / 400 - 32045 - 0.5;
}

jd_t khronos::hebrew_to_jd(year_t year, month_t month, day_t day, hour_t hour, minute_t minute, second_t seconds) {
	assert(year > -4800 && "Minimum year is 4800 BCE");
	assert(month >= 1 && "Minimum month is January = 1");
	assert(month <= 12 && "Maximum month is December = 12");
	assert(day >= 1 && "Minimum day of the month is 1");
	assert(day <= 31 && "Maximum day of the month is 31");

	jd_t jdn = hebrew_to_jd(year, month, day);
	tod_t tod_ = tod(hour, minute, seconds);
	return jdn + tod_;
}

void khronos::jd_to_hebrew(jd_t jd, year_t& year, month_t& month, day_t& day) {
	assert(jd >= -31738.5 && "Minimum convertable date not provided.");

	jd = floor(jd - 0.5) + 0.5;
	long long a = static_cast<long long>(jd + 0.5) + 32044;
	long long b = (4 * a + 3) / 146097;
	long long c = a - b * 146097 / 4;
	long long d = (4 * c + 3) / 1461;
	long long e = c - 1461 * d / 4;
	long long m = (5 * e + 2) / 153;
	day = static_cast<day_t>(e - (153 * m + 2) / 5 + 1);
	month = static_cast<month_t>(m + 3 - 12 * (m / 10));
	year = b * 100 + d - 4800 + m / 10;
}

void khronos::jd_to_hebrew(jd_t jd, year_t& year, month_t& month, day_t& day, hour_t& hour, minute_t& minute, second_t& seconds) {
	jd_to_hebrew(jd, year, month, day);

	jd_t frac = jd + 0.5 - floor(jd + 0.5);

	hour = (hour_t)frac * 24;
	minute = hour / 60;
	seconds = floor(frac * 24 / 60 / 60);
}


/** Hebrew + (integer year) operator. */
Hebrew khronos::operator + (Hebrew const& dt, detail::packaged_year_integer const& year) {
	year_t y = dt.year() + year.nYears_;
	month_t m = dt.month();
	day_t d = dt.day();

	//if (m == FEB && d == 29 && !is_hebrew_leapyear(y))
	//	d = 28;


	return Hebrew(y, m, d);
}



/** Hebrew + (integer month) operator. */
Hebrew khronos::operator + (Hebrew const& dt, detail::packaged_month_integer const& month) {
	year_t yearsToAdd = month.nMonths_ / 12;
	month_t monthsToAdd = month.nMonths_ % 12;
	year_t y = dt.year() + yearsToAdd;
	month_t m = dt.month() + monthsToAdd;

	int adjustment = (m - 1) / 12 + (m - 12) / 12;
	y += adjustment;
	m -= month_t(adjustment * 12);

	day_t d = 0;// std::min(dt.day(), hebrew_days_in_month(m, is_hebrew_leapyear(y)));

	return Hebrew(y, m, d);
}