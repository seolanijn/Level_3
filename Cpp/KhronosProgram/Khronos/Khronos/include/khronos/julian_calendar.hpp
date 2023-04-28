#pragma once

/**	@file khronos/julian_calendar.hpp
	@author Garth Santor
	@date 2013-09-19

	Julian calendar functions and class declaration.
	*/

#include <khronos.hpp>
#include <khronos/def.hpp>
#include <khronos/calendar.hpp>
#include <khronos/julian_day.hpp>
#include <string>

namespace khronos {

	// LITERALS
	/* UDL - converts a Julian year BC to an astronomical Julian year. */
	constexpr year_t operator ""_BC(unsigned long long julianYearCE) { return -static_cast<year_t>(julianYearCE) + 1; }
	constexpr year_t operator ""_bc(unsigned long long julianYearCE) { return -static_cast<year_t>(julianYearCE) + 1; }


	constexpr jd_t JULIAN_EPOCH = 1'721'423.5;

	jd_t julian_to_jd(year_t year, month_t month, day_t day);
	jd_t julian_to_jd(year_t year, month_t month, day_t day, hour_t hour, minute_t minute, second_t seconds);
	void jd_to_julian(jd_t jd, year_t& year, month_t& month, day_t& day);
	void jd_to_julian(jd_t jd, year_t& year, month_t& month, day_t& day, hour_t& hour, minute_t& minute, second_t& seconds);

	class Julian {
		year_t year_ = 1;
		month_t month_ = 1;
		day_t day_ = 1;
		hour_t hour_ = 0;
		minute_t minute_ = 0;
		second_t second_ = 0;

		void from_jd(jd_t jd) { jd_to_julian(jd, year_, month_, day_, hour_, minute_, second_); }
		jd_t to_jd() const { return julian_to_jd(year_, month_, day_); }
	public:
		Julian();

		/*! Get the year.
			\return Astronomical year. */
		constexpr year_t year() const { return year_; }

		/*! Get the month.
			\return Month number [1..12] */
		constexpr month_t month() const { return month_; }

		/*! Get the day of the month.
			\return Day of month number [1..31]. */
		constexpr day_t day() const { return day_; }

		constexpr hour_t hour() const { return hour_; }
		constexpr minute_t minute() const { return minute_; }
		constexpr second_t second() const { return second_; }

		/*! Construct a Julian calendar date from year,month,day,[hour,minute,second]
			\param year [in] Astronomical year.
			\param month [in] Month number [1..12]
			\param day [in] Day of month [1..31]
			\param hour [in] 24-hour of the day [0..23]
			\param minute [in] minute of the hour [0..59]
			\param second [in] second of the minute [0..59]
			*/
		constexpr Julian(year_t year, month_t month, day_t day) : year_(year), month_(month), day_(day) {}

		constexpr Julian(year_t year, month_t month, day_t day, hour_t hour, minute_t min, second_t sec)
			: year_(year), month_(month), day_(day), hour_(hour), minute_(min), second_(sec) {}

		/** Construct a Julian date from Julian Day Number object.
			@param jd [in] Jd object.
			*/
		Julian(Jd const& jd) { from_jd(jd.jd()); }

		std::string to_string() const;

		/**	Implicit cast to Jd class. */
		operator Jd() const { return Jd(to_jd()); }


		/**	Assign and convert from Jd type to Julian type. */
		Julian& operator = (Jd const& jd) {
			from_jd(jd.jd());
			return *this;
		}


		// block some operators
	private:
		Julian operator + (detail::packaged_year_real const&);
		Julian operator - (detail::packaged_year_real const&);
		Julian operator + (detail::packaged_month_real const&);
		Julian operator - (detail::packaged_month_real const&);
	};

	constexpr year_t year(Julian g) { return g.year(); }
	constexpr month_t month(Julian g) { return g.month(); }
	constexpr day_t day(Julian g) { return g.day(); }
	constexpr hour_t hour(Julian g) { return g.hour(); }
	constexpr minute_t minute(Julian g) { return g.minute(); }
	constexpr second_t second(Julian g) { return g.second(); }

	/** Julian + (integer month) */
	Julian operator + (Julian const& dt, detail::packaged_month_integer const& month);

	/** Julian - (integer month) */
	inline Julian operator - (Julian const& dt, detail::packaged_month_integer const& month) { return dt + detail::packaged_month_integer(-month.nMonths_); }

	/**	Julian + (integer year) */
	Julian operator + (Julian const& dt, detail::packaged_year_integer const& year);

	/**	Julian - (integer year) */
	inline Julian operator - (Julian const& dt, detail::packaged_year_integer const& year) { return dt + detail::packaged_year_integer(-year.nYears_); }

	/** Stream insertion operator. */
	inline std::ostream& operator << (std::ostream& os, Julian const& g) { return os << g.to_string(); }

}