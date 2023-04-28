/*	ut_Khronos_098-100_vulcan_concepts.cpp

	Khronos library Vulcan Calendar concepts unit test.

=============================================================
Revision History
-------------------------------------------------------------

Version 2021.09.25
	Replaced BOOST's test framework with gats_test framework.

Version 2016.09.30
	Added: Phase List

Version 2015.09.21
	Initial Release


=============================================================

Copyright Garth Santor/Trinh Han

The copyright to the computer program(s) herein
is the property of Garth Santor/Trinh Han, Canada.
The program(s) may be used and/or copied only with
the written permission of Garth Santor/Trinh Han
or in accordance with the terms and conditions
stipulated in the agreement/contract under which
the program(s) have been supplied.
=============================================================*/

#include <khronos.hpp>
#include "ut_Khronos.hpp"
#include "ut_utilities.hpp"
#include <array>

constexpr double oneThird = 1.0 / 3.0;


#if PHASE_98 
TEST_PHASE(ut098_test_vulcan_days_of_month, oneThird) {
	using namespace khronos;
	GATS_CHECK_EQUAL(vulcan_days_in_month(), 21);
}
#else
EMPTY_PHASE(ut098_test_vulcan_days_of_month,oneThird)
#endif



#if PHASE_99 
TEST_PHASE(ut099_test_Vulcan_month_name, oneThird) {
	using namespace khronos;
	static std::array<char const *, 13> const names = {
		"",
		"Z'at",
		"D'ruh",
		"K'riBrax",
		"re'T'Khutai",
		"T'keKhuti",
		"Khuti",
		"Ta'Krat",
		"K'ri'lior",
		"et'khior",
		"T'lakht",
		"T'ke'Tas",
		"Tasmeen",
	};

	for (month_t i = 1; i <= 12; ++i)
		GATS_CHECK(vulcan_month_name(i) == names[i]);
}
#else
EMPTY_PHASE(ut099_test_Vulcan_month_name,oneThird)
#endif



#if PHASE_100
/** Test Vulcan month names. */
TEST_PHASE(ut100_test_vulcan_month_names, oneThird) {
	using namespace khronos;
	using namespace std;
	GATS_CHECK_EQUAL("Z'at"s, vulcan_month_name(1));
	GATS_CHECK_EQUAL("D'ruh"s, vulcan_month_name(2));
	GATS_CHECK_EQUAL("K'riBrax"s, vulcan_month_name(3));
	GATS_CHECK_EQUAL("re'T'Khutai"s, vulcan_month_name(4));
	GATS_CHECK_EQUAL("T'keKhuti"s, vulcan_month_name(5));
	GATS_CHECK_EQUAL("Khuti"s, vulcan_month_name(6));
	GATS_CHECK_EQUAL("Ta'Krat"s, vulcan_month_name(7));
	GATS_CHECK_EQUAL("K'ri'lior"s, vulcan_month_name(8));
	GATS_CHECK_EQUAL("et'khior"s, vulcan_month_name(9));
	GATS_CHECK_EQUAL("T'lakht"s, vulcan_month_name(10));
	GATS_CHECK_EQUAL("T'ke'Tas"s, vulcan_month_name(11));
	GATS_CHECK_EQUAL("Tasmeen"s, vulcan_month_name(12));
}
#else
EMPTY_PHASE(ut100_test_vulcan_month_names,oneThird)
#endif