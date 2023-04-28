#pragma once
/*
khronos\gregorian_calendar.hpp
(c) Garth Santor
Created: 2015-09-22
Last Updated: 2015-09-22

Khronos library 'Gregorian calendar' declarations.
*/


#include <khronos/def.hpp>
#include <khronos/calendar.hpp>
#include <khronos/julian_day.hpp>
#include <string>


namespace khronos {
	
	// LITERALS
	/* UDL - converts a Gregorian year CE to an astronomical Gregorian year. */
	constexpr year_t operator ""_CE(unsigned long long gregorianYearCE) { return static_cast<year_t>(gregorianYearCE); }
	constexpr year_t operator ""_ce(unsigned long long gregorianYearCE) { return static_cast<year_t>(gregorianYearCE); }

	/* UDL - converts a Gregorian year BCE to an astronomical Gregorian year. */
	constexpr year_t operator ""_BCE(unsigned long long gregorianYearBCE) { return -static_cast<year_t>(gregorianYearBCE) + 1; }
	constexpr year_t operator ""_bce(unsigned long long gregorianYearBCE) { return -static_cast<year_t>(gregorianYearBCE) + 1; }

	// VALUES
	constexpr jd_t GREGORIAN_EPOCH = 1'721'425.5;

	// OPERATIONS

	/** Provide the number of days in the month of the Gregorian Calendar. */
	constexpr day_t gregorian_days_in_month(month_t month, bool isLeapYear) {
		return civil::days_in_month(month, isLeapYear);
	}


	/** Provide the name of the given month in the Gregorian calendar. */
	constexpr char const * gregorian_month_name(month_t month) {
		return civil::month_name_long(month);
	}


	/** Provide the short name of the given month in the Gregorian calendar. */
	constexpr char const * gregorian_short_month_name(month_t month) {
		return civil::month_name_short(month);
	}




	// CLASSES
	// --------------------------------------------------------------------------------------

	/**	Proleptic Gregorian Calendar Date class. */

	// FUNCTIONS
	/*!	Leap year test for Proleptic Gregorian Calendar.
	\return 'true' if leap year, 'false' if not.
	\param year [in] Astronomical year (1 CE = 1, 1 BCE = 0, 2 BCE = -1, etc.)
	*/
	constexpr bool is_gregorian_leapyear(year_t year) {
		return (year % 4 == 0) && (year % 100 != 0) || (year % 400 == 0);
	}

	jd_t gregorian_to_jd(year_t year, month_t month, day_t day);
	jd_t gregorian_to_jd(year_t year, month_t month, day_t day, hour_t hour, minute_t minute, second_t seconds);
	void jd_to_gregorian(jd_t jd, year_t& year, month_t& month, day_t& day);
	void jd_to_gregorian(jd_t jd, year_t& year, month_t& month, day_t& day, hour_t& hour, minute_t& minute, second_t& seconds);

	// OPERATORS
	// ====================


	class Gregorian {
		year_t year_ = 1;
		month_t month_ = 1;
		day_t day_ = 1;
		hour_t hour_ = 0;
		minute_t minute_ = 0;
		second_t second_ = 0;

		void from_jd(jd_t jd) { jd_to_gregorian(jd, year_, month_, day_, hour_, minute_, second_); }
		jd_t to_jd() const { return gregorian_to_jd(year_, month_, day_); }
	public:
		Gregorian();
		Gregorian(now_t now);

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

		/*! Construct a Gregorian calendar date from year,month,day,[hour,minute,second]
			\param year [in] Astronomical year.
			\param month [in] Month number [1..12]
			\param day [in] Day of month [1..31]
			\param hour [in] 24-hour of the day [0..23]
			\param minute [in] minute of the hour [0..59]
			\param second [in] second of the minute [0..59]
			*/
		constexpr Gregorian(year_t year, month_t month, day_t day) : year_(year), month_(month), day_(day) {}
		
		constexpr Gregorian(year_t year, month_t month, day_t day, hour_t hour, minute_t min, second_t sec) 
			: year_(year), month_(month), day_(day), hour_(hour), minute_(min), second_(sec) {}

		/** Construct a Gregorian date from Julian Day Number object.
			@param jd [in] Jd object.
			*/
		Gregorian(Jd const& jd) { from_jd(jd.jd()); }

		std::string to_string() const;

		/**	Implicit cast to Jd class. */
		operator Jd() const { return Jd(to_jd()); }


		/**	Assign and convert from Jd type to Gregorian type. */
		Gregorian& operator = (Jd const& jd) {
			from_jd(jd.jd());
			return *this;
		}


		// block some operators
	private:
		Gregorian operator + (detail::packaged_year_real const&);
		Gregorian operator - (detail::packaged_year_real const&);
		Gregorian operator + (detail::packaged_month_real const&);
		Gregorian operator - (detail::packaged_month_real const&);
	};

	constexpr year_t year(Gregorian g) { return g.year(); }
	constexpr month_t month(Gregorian g) { return g.month(); }
	constexpr day_t day(Gregorian g) { return g.day(); }
	constexpr hour_t hour(Gregorian g) { return g.hour(); }
	constexpr minute_t minute(Gregorian g) { return g.minute(); }
	constexpr second_t second(Gregorian g) { return g.second(); }


	/** Gregorian + (integer month) */
	Gregorian operator + (Gregorian const& dt, detail::packaged_month_integer const& month);

	/** Gregorian - (integer month) */
	inline Gregorian operator - (Gregorian const& dt, detail::packaged_month_integer const& month) { return dt + detail::packaged_month_integer(-month.nMonths_); }

	/**	Gregorian + (integer year) */
	Gregorian operator + (Gregorian const& dt, detail::packaged_year_integer const& year);

	/**	Gregorian - (integer year) */
	inline Gregorian operator - (Gregorian const& dt, detail::packaged_year_integer const& year) { return dt + detail::packaged_year_integer(-year.nYears_); }

	/** Stream insertion operator. */
	inline std::ostream& operator << (std::ostream& os, Gregorian const& g) { return os << g.to_string(); }

} // end-of-namespace khronos




