/**	@file Jdn.cpp
	@author Garth Santor
	@date 2012-09-16

	Date and time library to_string method implementations.
	*/


#include <khronos/julian_day.hpp>
#include <khronos/gregorian_calendar.hpp>
#include <sstream>
#include <ctime>
using namespace std;

khronos::Jd::Jd() : jd_(0)
{
	time_t nowTime = time(NULL);
	struct tm tmNow;
	localtime_s(&tmNow, &nowTime);
	jd_ = khronos::gregorian_to_jd(
		year_t(tmNow.tm_year) + 1900,
		month_t(tmNow.tm_mon) + 1,
		day_t(tmNow.tm_mday),
		hour_t(tmNow.tm_hour),
		minute_t(tmNow.tm_min),
		second_t(tmNow.tm_sec)
	);
}

khronos::Jd::Jd(now_t now) : jd_(0)
{
	if (now == 0) {
		time_t nowTime = time(NULL);
		struct tm tmNow;
		localtime_s(&tmNow, &nowTime);
		jd_ = khronos::gregorian_to_jd(
			year_t(tmNow.tm_year) + 1900,
			month_t(tmNow.tm_mon) + 1,
			day_t(tmNow.tm_mday)
		);
	}
	else {
		time_t nowTime = time(NULL);
		struct tm tmNow;
		localtime_s(&tmNow, &nowTime);
		jd_ = khronos::gregorian_to_jd(
			year_t(tmNow.tm_year) + 1900,
			month_t(tmNow.tm_mon) + 1,
			day_t(tmNow.tm_mday),
			hour_t(tmNow.tm_hour),
			minute_t(tmNow.tm_min),
			second_t(tmNow.tm_sec)
		);
	}


}

std::string khronos::Jd::to_string() const
{
	ostringstream oss;
	oss << "JD " << jd_;
	return oss.str();
}