/*	TestConsoleApp.cpp

	TestConsoleApp implementation.

=============================================================
Revision History
-------------------------------------------------------------

Version 2021.09.23
	Fixed pass-count in GATS_CHECK_THROW
	Changed stream type from wide to standard.
	Added:
		char_type, ostream_type, ostringstream_type
		GATS_CHECK_MESSAGE
		filename to check failure report.
		GATS_TEST_CASE_WEIGHTED
		Weighted score
	Refactored internal macro names to 'DETAILED_ ...'
	Replaced some CHECK macros internals with methods
	Formatting: placed quotes around case name in output.

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


#include <gats/TestApp.hpp>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <functional>
#include <filesystem>
#include <string>


namespace gats {
// TestApp::TestCase
// ----------------------------------------------------------------------------

	/*	TestCase constructor registers the test case with the TestApp
	*/
	TestApp::TestCase::TestCase(const char* name, double weight) : name_m(name), weight_m(weight) {
		TestApp::cases().push_back(this);
	}


	void TestApp::TestCase::_check_output_location(ostream_type& os, std::filesystem::path file, int line) {
		os << file.filename().string() << " (" << line << "): error in \"" << name_m << "\": ";
	}
	void TestApp::TestCase::_check(bool condition, const char_type* const condStr, const char* file, int line) {
		++nChecked_m;
		if (condition == false) {
			ostringstream_type oss;
			_check_output_location(oss, file, line);
			oss << condStr << "\n";
			display() << oss.str();
			//log() << oss.str();
		} else
			++nPassed_m;
	}
	void TestApp::TestCase::_check_message(bool condition, const string_type& message, const char* const file, int line) {
		++nChecked_m;
		if (condition == false) {
			ostringstream_type oss;
			_check_output_location(oss, file, line);
			oss << message << "\n";
			display() << oss.str();
			//log() << oss.str();
		} else
			++nPassed_m;
	}



// TestApp
// ----------------------------------------------------------------------------

	// Classifier instances for TestApp
	std::unique_ptr<TestApp::Container> TestApp::casesPtr_sm;
	std::ofstream TestApp::logFile_m;

	// Global TestConsoleApp instance <<singleton>>
	TestApp testConsoleApp_g;


	/*	Get a reference to the cases container. */
	TestApp::Container& TestApp::cases() {
		if (!casesPtr_sm) 			casesPtr_sm.reset(new Container());
		return *casesPtr_sm;
	}



	void TestApp::setup() {
		using namespace std;
		std::filesystem::path filename = "gats-test-log-file.txt";
		logFile_m.open(filename);
		if (!logFile_m) {
			throw std::runtime_error("Could not open: "s + filename.string());
		}
	}

	/*	Perform all test and log/report the results.
	*/
	int TestApp::execute() {
		using namespace gats::win32;

		std::size_t nCasesPassed = 0, nChecked_m = 0, nChecksPassed = 0;
		double score = 0, maxScore = 0;

		auto setcolor = [=](auto passed, auto checked) {
			if (passed == checked)
				ConsoleCore::SetTextAttribute(green);
			else if (passed * 10 / checked >= 9)
				ConsoleCore::SetTextAttribute(yellow);
			else
				ConsoleCore::SetTextAttribute(bright(red));
		};

		ScopedAttributeState sas;

		// sort the cases
		std::sort(cases().begin(), cases().end(), [](TestApp::TestCase* pLHS, TestApp::TestCase* pRHS)->bool { return strcmp(pLHS->name_m, pRHS->name_m) < 0; });

		// Run the cases
		for (auto& testCase : cases()) {
			std::wcout << bright(yellow);
			testCase->execute();
			std::wcout << white;
		}



		for (auto& testCase : cases()) {
			// Check for empty case

			bool passed = testCase->nPassed_m == testCase->nChecked_m && testCase->nChecked_m > 0;

			// report pass/fail
			double ratio = double(testCase->nPassed_m) / testCase->nChecked_m;
			if (testCase->nChecked_m == 0) ratio = 0.0;
			double percentage = ratio * 100.0;

			auto setColorLevel = [=]() {
				if (passed)
					std::wcout << bright(green);
				else if (ratio >= 0.8)
					std::wcout << bright(yellow);
				else if (ratio >= 0.5)
					std::wcout << yellow;
				else
					std::wcout << bright(red);
			};

			setColorLevel();
			std::wcout << (passed ? "Passed " : "Failed ");
			std::wcout	<< std::setw(5) << std::setprecision(1) << std::fixed << percentage << "% ";
			std::wcout << cyan << testCase->name_m;
			std::wcout << white << std::endl;

			// log file record
			logFile_m << std::setw(5) << std::setprecision(1) << std::fixed << ratio * testCase->weight_m;
			logFile_m << "\t" << testCase->weight_m;
			logFile_m << "\t" << testCase->name_m;
			logFile_m << std::endl;

			// enumerate cases
			nCasesPassed += passed;
			nChecked_m += testCase->nChecked_m;
			nChecksPassed += testCase->nPassed_m;
			score += ratio * testCase->weight_m;
			maxScore += testCase->weight_m;
		}



		// report summary
		double checkPercentage = 100.0 * nChecksPassed / nChecked_m;

		setcolor(nChecksPassed, nChecked_m);
		std::ostringstream oss;
		oss << "\n" <<
			std::setprecision(1) << std::fixed <<
			nChecksPassed << "/" << nChecked_m << " checks passed (" << (nChecked_m ? checkPercentage : 0.0) << "%)\n";
		std::cout << oss.str();

		setcolor(nCasesPassed, cases().size());
		oss.str(""); oss.clear();
		oss <<
			nCasesPassed << "/" << cases().size() << " cases (" << 
			std::setprecision(1) << std::fixed << 
			(cases().size() ? 100.0 * nCasesPassed / cases().size() : 0.0) << "%)\n";
		std::cout << oss.str();

		oss.str("");  oss.clear();
		
		setcolor(score, maxScore);
		oss << "\nScore = " << score << " of " << maxScore << " (" << 
			std::setprecision(1) << std::fixed << 
			(maxScore > 0.0 ? score*100/maxScore : 0.0) << 
			"%)";
		std::cout << oss.str() << std::endl;
		logFile_m << oss.str() << std::endl;

		return EXIT_SUCCESS;
	}

} // end-of-namespace gats