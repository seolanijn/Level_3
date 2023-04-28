#pragma once

/**	@file khronos/hebrew_calendar.hpp
	@author Garth Santor
	@date 2013-09-26

	Hebrew calendar class.
	*/

#include <khronos/def.hpp>
#include <khronos/calendar.hpp>
#include <khronos/julian_day.hpp>
#include <string>


namespace khronos {
	// VALUES
	constexpr jd_t HEBREW_EPOCH = 347'995.5;

	// OPERATIONS

	/** Provide the number of days in the month of the Hebrew Calendar. */
	constexpr day_t hebrew_days_in_month(month_t month, bool isLeapYear) {
		return civil::days_in_month(month, isLeapYear);
	}


	/** Provide the name of the given month in the Hebrew calendar. */
	constexpr char const* hebrew_month_name(month_t month) {
		return civil::month_name_long(month);
	}


	/** Provide the short name of the given month in the Hebrew calendar. */
	constexpr char const* hebrew_short_month_name(month_t month) {
		return civil::month_name_short(month);
	}




	// CLASSES
	// --------------------------------------------------------------------------------------

	/**	Proleptic Hebrew Calendar Date class. */

	// FUNCTIONS
	/*!	Leap year test for Proleptic Hebrew Calendar.
	\return 'true' if leap year, 'false' if not.
	\param year [in] Astronomical year (1 CE = 1, 1 BCE = 0, 2 BCE = -1, etc.)
	*/
	// bool is_hebrew_leapyear(year_t year) {
	//	return utility::mod(7 * year + 1, 19) < 7;
	//}

	jd_t hebrew_to_jd(year_t year, month_t month, day_t day);
	jd_t hebrew_to_jd(year_t year, month_t month, day_t day, hour_t hour, minute_t minute, second_t seconds);
	void jd_to_hebrew(jd_t jd, year_t& year, month_t& month, day_t& day);
	void jd_to_hebrew(jd_t jd, year_t& year, month_t& month, day_t& day, hour_t& hour, minute_t& minute, second_t& seconds);

	

	class Hebrew {
		year_t year_ = 1;
		month_t month_ = 1;
		day_t day_ = 1;
		hour_t hour_ = 0;
		minute_t minute_ = 0;
		second_t second_ = 0;

		void from_jd(jd_t jd) { jd_to_hebrew(jd, year_, month_, day_, hour_, minute_, second_); }
		jd_t to_jd() const { return hebrew_to_jd(year_, month_, day_); }
	public:
		Hebrew();

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

		/*! Construct a Hebrew calendar date from year,month,day,[hour,minute,second]
			\param year [in] Astronomical year.
			\param month [in] Month number [1..12]
			\param day [in] Day of month [1..31]
			\param hour [in] 24-hour of the day [0..23]
			\param minute [in] minute of the hour [0..59]
			\param second [in] second of the minute [0..59]
			*/
		constexpr Hebrew(year_t year, month_t month, day_t day) : year_(year), month_(month), day_(day) {}

		constexpr Hebrew(year_t year, month_t month, day_t day, hour_t hour, minute_t min, second_t sec)
			: year_(year), month_(month), day_(day), hour_(hour), minute_(min), second_(sec) {}

		/** Construct a Hebrew date from Julian Day Number object.
			@param jd [in] Jd object.
			*/
		Hebrew(Jd const& jd) { from_jd(jd.jd()); }

		std::string to_string() const;

		/**	Implicit cast to Jd class. */
		operator Jd() const { return Jd(to_jd()); }


		/**	Assign and convert from Jd type to Hebrew type. */
		Hebrew& operator = (Jd const& jd) {
			from_jd(jd.jd());
			return *this;
		}


		// block some operators
	private:
		Hebrew operator + (detail::packaged_year_real const&);
		Hebrew operator - (detail::packaged_year_real const&);
		Hebrew operator + (detail::packaged_month_real const&);
		Hebrew operator - (detail::packaged_month_real const&);
	};

	constexpr year_t year(Hebrew g) { return g.year(); }
	constexpr month_t month(Hebrew g) { return g.month(); }
	constexpr day_t day(Hebrew g) { return g.day(); }
	constexpr hour_t hour(Hebrew g) { return g.hour(); }
	constexpr minute_t minute(Hebrew g) { return g.minute(); }
	constexpr second_t second(Hebrew g) { return g.second(); }


	/** Hebrew + (integer month) */
	Hebrew operator + (Hebrew const& dt, detail::packaged_month_integer const& month);

	/** Hebrew - (integer month) */
	inline Hebrew operator - (Hebrew const& dt, detail::packaged_month_integer const& month) { return dt + detail::packaged_month_integer(-month.nMonths_); }

	/**	Hebrew + (integer year) */
	Hebrew operator + (Hebrew const& dt, detail::packaged_year_integer const& year);

	/**	Hebrew - (integer year) */
	inline Hebrew operator - (Hebrew const& dt, detail::packaged_year_integer const& year) { return dt + detail::packaged_year_integer(-year.nYears_); }

	/** Stream insertion operator. */
	inline std::ostream& operator << (std::ostream& os, Hebrew const& g) { return os << g.to_string(); }

} // end-of-namespace khronos
