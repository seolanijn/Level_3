/**	@file Islamic_to_jd.cpp
	@author Garth Santor
	@date 2013-09-19

	Islamic calendar conversion implementations.
	*/

#include <khronos/islamic_calendar.hpp>
#include <cmath>

using namespace khronos;


jd_t khronos::islamic_to_jd(year_t year, month_t month, day_t day) {
	return day + ceil(29.5 * (month - 1)) + (year - 1) * 354 + floor((3 + 11 * year) / 30) + ISLAMIC_EPOCH - 1;
}

jd_t khronos::islamic_to_jd(year_t year, month_t month, day_t day, hour_t hour, minute_t minute, second_t seconds) {
	jd_t jdn = islamic_to_jd(year, month, day);
	tod_t tod_ = tod(hour, minute, seconds);
	if (tod_ >= 0.5) tod_ = tod_ - 1;
	return jdn + tod_;
}

void khronos::jd_to_islamic(jd_t jd, year_t& year, month_t& month, day_t& day) {

	jd = floor(jd) + 0.5;
	year = (year_t)floor((30 * (jd - ISLAMIC_EPOCH) + 10646) / 10631);
	if (12 < ceil((jd - (29 + islamic_to_jd(year, 1, 1))) / 29.5) + 1) month = 12;
	else month = (month_t)ceil((jd - (29 + islamic_to_jd(year, 1, 1))) / 29.5) + 1;
	day = (day_t)(jd - islamic_to_jd(year, month, 1) + 1);
}

void khronos::jd_to_islamic(jd_t jd, year_t& year, month_t& month, day_t& day, hour_t& hour, minute_t& minute, second_t& seconds) {
	jd_to_islamic(jd, year, month, day);

	jd_t frac = jd + 0.5 - floor(jd + 0.5);

	hour = (hour_t)frac * 24;
	minute = hour / 60;
	seconds = floor(frac * 24 / 60 / 60);
}


/** Islamic + (integer year) operator. */
Islamic khronos::operator + (Islamic const& dt, detail::packaged_year_integer const& year) {
	year_t y = dt.year() + year.nYears_;
	month_t m = dt.month();
	day_t d = dt.day();

	if (m == DEC && d == 30 && !is_islamic_leapyear(y))
		d = 29;


	return Islamic(y, m, d);
}



/** Islamic + (integer month) operator. */
Islamic khronos::operator + (Islamic const& dt, detail::packaged_month_integer const& month) {
	year_t yearsToAdd = month.nMonths_ / 12;
	month_t monthsToAdd = month.nMonths_ % 12;
	year_t y = dt.year() + yearsToAdd;
	month_t m = dt.month() + monthsToAdd;

	int adjustment = (m - 1) / 12 + (m - 12) / 12;
	y += adjustment;
	m -= month_t(adjustment * 12);

	day_t d = std::min(dt.day(), islamic_days_in_month(m, is_islamic_leapyear(y)));

	return Islamic(y, m, d);
}