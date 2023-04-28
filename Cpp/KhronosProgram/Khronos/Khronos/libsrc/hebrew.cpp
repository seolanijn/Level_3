/**	@file Hebrew.cpp
	@author Garth Santor
	@date 2013-10-20

	Hebrew calendar class implementation.
	*/

#include <khronos/hebrew_calendar.hpp>
#include <sstream>
using namespace khronos;

/**	Hebrew default constructor.  Initialize to the current local time. */
Hebrew::Hebrew() {
	time_t nowTime = time(NULL);
	struct tm tmNow;
	localtime_s(&tmNow, &nowTime);
	year_ = year_t(tmNow.tm_year) + 1900;
	month_ = month_t(tmNow.tm_mon) + 1;
	day_ = day_t(tmNow.tm_mday);
}

/** Print the Hebrew Date as a string. */
std::string Hebrew::to_string() const {
	std::ostringstream oss;
	oss << civil::day_name(civil::day_of_week(to_jd())) << ", ";
	oss << hebrew_month_name(month_) << ' ' << (unsigned)day_ << ' ';
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