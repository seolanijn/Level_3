#pragma once

/*
khronos\timeofday.hpp
(c) Garth Santor
Created: 2015-09-22
Last Updated: 2015-09-22

Khronos library 'time-of-day' declarations.
*/

namespace khronos {

	constexpr second_t SECONDS_PER_DAY = 24.0 * 60.0 * 60.0;

	constexpr hour_t operator ""_am(unsigned long long am) {
		switch (am) {
		case 12: return 0;
		default: return static_cast<hour_t>(am);
		}
	}
	constexpr hour_t operator ""_pm(unsigned long long pm) {
		switch (pm) {
		case 12: return static_cast<hour_t>(pm);
		default: return static_cast<hour_t>(pm) + 12;
		}
	}

	constexpr tod_t tod(hour_t hour, minute_t min, second_t sec) {
		return (sec + 60.0 * (min + 60.0 * hour)) / SECONDS_PER_DAY;
	}
}


