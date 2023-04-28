/**	@file Islamic.cpp
	@author Garth Santor
	@date 2013-09-20

	Islamic calendar class implementation.
	*/

#include <khronos/islamic_calendar.hpp>
#include <sstream>
using namespace khronos;

/**	Islamic default constructor.  Initialize to the current local time. */
Islamic::Islamic() {
	time_t nowTime = time(NULL);
	struct tm tmNow;
	localtime_s(&tmNow, &nowTime);
	year_ = year_t(tmNow.tm_year) + 1900;
	month_ = month_t(tmNow.tm_mon) + 1;
	day_ = day_t(tmNow.tm_mday);
	hour_ = hour_t(tmNow.tm_hour);
	minute_ = minute_t(tmNow.tm_min);
	second_ = second_t(tmNow.tm_sec);
}
Islamic::Islamic(now_t now) {
	if (now == WTIMEOFDAY) {
		time_t nowTime = time(NULL);
		struct tm tmNow;
		localtime_s(&tmNow, &nowTime);
		year_ = year_t(tmNow.tm_year) + 1900;
		month_ = month_t(tmNow.tm_mon) + 1;
		day_ = day_t(tmNow.tm_mday);
		hour_ = hour_t(tmNow.tm_hour);
		minute_ = minute_t(tmNow.tm_min);
		second_ = second_t(tmNow.tm_sec);
	}
	else {
		time_t nowTime = time(NULL);
		struct tm tmNow;
		localtime_s(&tmNow, &nowTime);
		year_ = year_t(tmNow.tm_year) + 1900;
		month_ = month_t(tmNow.tm_mon) + 1;
		day_ = day_t(tmNow.tm_mday);
	}
}

/** Print the Islamic Date as a string. */
std::string Islamic::to_string() const {
	std::ostringstream oss;
	oss << civil::day_name(civil::day_of_week(to_jd())) << ", ";
	oss << islamic_month_name(month_) << ' ' << (unsigned)day_ << ' ';
	if (year_ <= 0)
		oss << (-year_ + 1) << " BCE, ";
	else
		oss << year_ << " CE, ";

	if (hour_ > 12)
		oss << hour_ - 12 << ":";
	else if (hour_ == 0)
		oss << 12 << ":";
	else
		oss << hour_ << ":";

	if (minute_ < 10)
		oss << 0 << minute_ << ":";
	else
		oss << minute_ << ":";

	if (second_ < 10)
		oss << 0 << second_ << " ";
	else
		oss << second_ << " ";

	if (hour_ < 12)
		oss << "am";
	else
		oss << "pm";

	return oss.str();
}