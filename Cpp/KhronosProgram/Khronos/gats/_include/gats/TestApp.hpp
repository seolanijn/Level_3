#pragma once
/*	TestApp.hpp

	TestApp declaration.

=============================================================
Revision History
-------------------------------------------------------------
Version 2021.09.23
	Fixed pass-count in GATS_CHECK_THROW
	Changed stream type from wide to standard.
	Added:
		char_type, ostream_type, ostringstream_type
		GATS_CHECK_MESSAGE
	Refactored internal macro names to 'DETAILED_ ...'
	Replaced some CHECK macros internals with methods
	Formatting: placed quotes around case name in output.
	Added filename to check failure report.

Version 2021.08.29
	Alpha release.

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


#include <gats/win32/ConsoleApp/debug.hpp>
#include <gats/win32/ConsoleApp.hpp>

#include <cstdint>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <vector>
#include <filesystem>
#include <type_traits>


namespace gats {

	class TestApp : public win32::ConsoleApp {
	// TYPES
	public:
		using char_type = char;
		using ostream_type = std::basic_ostream<char_type>;
		using ofstream_type = std::basic_ofstream<char_type>;
		using ostringstream_type = std::basic_ostringstream<char_type>;
		using string_type = std::basic_string<char_type>;

		class TestCase {
		public:
			const char_type*	name_m;
			std::uintmax_t		nPassed_m = 0;
			std::uintmax_t		nChecked_m = 0;
			double				weight_m = 1;

		// OPERATIONS
		public:
			TestCase(const char* name) : TestCase(name, 1) {}
			TestCase(const char* name, double weight);
			virtual void execute() {}
		protected:
			void _check_output_location(ostream_type& os, std::filesystem::path file, int line);
			void _check(bool condition, const char_type* const condStr, const char * const file, int line);
			void _check_message(bool condition, const string_type& message, const char* const file, int line);
			template <typename LHS, typename RHS>
			void _check_equal(const LHS& lhs, const RHS& rhs, const char_type* lhsStr, const char_type* rhsStr, const char* const file, int line);
			template <typename LHS, typename RHS, typename EPS>
			void _check_close_epsilon(const LHS& lhs, const RHS& rhs, const EPS& epsilon, const char_type* lhsStr, const char_type* rhsStr, const char_type* epsilonStr, const char* const file, int line);

			ostream_type& display() { return TestApp::display(); }
			ostream_type& log() { return logFile_m; }
		};

	// ATTRIBUTES
	private:
		using Container = std::vector<TestCase*>;
		static std::unique_ptr<Container> casesPtr_sm;
		static ofstream_type logFile_m;
		static ostream_type& display() { return std::cout; }

		static Container& cases();
		void setup() override;
		int execute() override;
	};


	template <typename LHS, typename RHS>
	void TestApp::TestCase::_check_equal(const LHS& lhs, const RHS& rhs, const TestApp::char_type* lhsStr, const TestApp::char_type* rhsStr, const char* const file, int line) {
		bool condition = lhs == rhs;
		++nChecked_m;
		if (condition==false) {
			ostringstream_type oss;
			_check_output_location(oss, file, line);
			oss << "\"" << lhsStr <<  "\" [" << lhs << "] != \"" << rhsStr << "\" [" << rhs << "]\n";
			display() << oss.str();
			//log() << oss.str();
		} else
			++nPassed_m;
	}

	template <typename LHS, typename RHS, typename EPS>
	void TestApp::TestCase::_check_close_epsilon(const LHS& lhs, const RHS& rhs, const EPS& epsilon, const char_type* lhsStr, const char_type* rhsStr, const char_type* epsilonStr, const char* const file, int line) {
		bool condition = abs((lhs) - (rhs)) <= abs(epsilon);
		++nChecked_m;
		if (condition==false) {
			ostringstream_type oss;
			_check_output_location(oss, file, line);
			oss << "difference(" << lhsStr << ", " << rhsStr << ") > " << epsilonStr << " ==> \t|" << lhs << " - " << rhs << "| > " << abs(epsilon) << "\n"; 
			display() << oss.str();
			//log() << oss.str();
		} else
			++nPassed_m;
	}


} // end-of-namespace gats



/*	Creates a test case with the identifier 'name'

	\param 'name' is the test cases identifier.

	The identifier is used to indicate execution order (lexicographical ordering) and both the class and global object name.
*/
#define GATS_TEST_CASE(name) \
	static class TestCase_ ## name : public gats::TestApp::TestCase {\
	public: TestCase_ ## name() : TestCase(#name) { }\
	public: virtual void execute() override;\
	} TestCase_ ## name ## _g;\
	void TestCase_ ## name :: execute()


/*	Creates a test case with the identifier 'name'

	\param 'name' is the test cases identifier.

	The identifier is used to indicate execution order (lexicographical ordering) and both the class and global object name.
*/
#define GATS_TEST_CASE_WEIGHTED(name,weight) \
	static class TestCase_ ## name : public gats::TestApp::TestCase {\
	public: TestCase_ ## name() : TestCase(#name,weight) { }\
	public: virtual void execute() override;\
	} TestCase_ ## name ## _g;\
	void TestCase_ ## name :: execute()


/*	Performs a check point for the specified condition.

	\param 'cond' is condition that must pass.
*/
#define GATS_CHECK(condition) _check((condition), #condition, __FILE__, __LINE__)


/*	Performs a check point for the specified condition.

	\param 'cond' is condition that must pass.
*/
#define DETAIL_GATS_CHECK_MESSAGE(condition,message,file,line) {\
	bool cond = (condition);\
	gats::TestApp::string_type strMessage;\
	if (cond==false) {\
		gats::TestApp::ostringstream_type oss;\
		oss << message;\
		strMessage = oss.str();\
	}\
	_check_message(cond, strMessage, file, line);\
}
#define GATS_CHECK_MESSAGE(condition, message) DETAIL_GATS_CHECK_MESSAGE(condition, message, __FILE__, __LINE__)



/*	Performs a check that the values are exact matches.

	\param 'testValue' is the value being checked.
	\param 'expectedValue' is the value that the test value should have.
*/
#define GATS_CHECK_EQUAL(testValue, expectedValue) _check_equal((testValue), (expectedValue), #testValue, #expectedValue, __FILE__, __LINE__)



/*	Performs a check that the values are close matches.

	\param 'testValue' is the value being checked.
	\param 'expectedValue' is the value that the test value should have.
	\param 'epsilon' is the maximum difference allowed.
*/
#define GATS_CHECK_CLOSE_EPSILON(testValue, expectedValue, epsilon) _check_close_epsilon((testValue), (expectedValue), (epsilon), #testValue, #expectedValue, #epsilon, __FILE__, __LINE__)



/*	Performs a check that the values are exact matches.

	\param 'testValue' is the value being checked.
	\param 'expectedValue' is the value that the test value should have.
*/
#define DETAIL_GATS_CHECK_THROW(operation, expectedException, file, line) {\
	++nChecked_m;\
	bool isGood = false;\
	try {\
		operation;\
	} catch(expectedException) {\
		++nPassed_m;\
		isGood = true;\
	} catch(...){\
		gats::TestApp::ostringstream_type oss;\
		_check_output_location(oss, file, line);\
		oss << "unknown exception \"" #expectedException "\" not thrown\n";\
		display() << oss.str();\
		isGood = true;\
	}\
	if (!isGood) {\
		gats::TestApp::ostringstream_type oss;\
		_check_output_location(oss, file, line);\
		oss << "no exception thrown, expecting \"" #expectedException "\"\n";\
		display() << oss.str();\
	}\
}
#define GATS_CHECK_THROW(operation, expectedException) DETAIL_GATS_CHECK_THROW(operation, expectedException, __FILE__, __LINE__)
