/*
author:		Seolan Jin
Date:		Dec. 10, 2021
Purpose:	This program will detect the type of Breast Cancer tumors using Binary Decision Tree
*/

#include <iostream>
#include <string>
#include <fstream>
#include <vector>

#include "BinaryTree.h"

using namespace std;

int main() {

	// create file with file name
	ifstream unformattedFile("unformatted_data_v1.0.0.csv");
	ofstream resultFile("results.csv");

	// check if there is an error
	if (!unformattedFile) {
		cerr << "<Error: could not open the file. End the program>";
		return EXIT_FAILURE;
	} if (!resultFile) {
		cerr << "<Error: could not create the file. End the program>";
		return EXIT_FAILURE;
	}
		
	int countBenign = 0;
	int countMalignant = 0;
	int countPatients = 0;
	int countUnknownPatients = 0;

	// calculate and insert the data into result file
	while (!unformattedFile.eof()) {
				
		string tempBareNuclei;
		char comma_;
		vector<int> nodeData(11);

		// get data from the unformattedFile
		unformattedFile >> nodeData.at(0) >> comma_ >> nodeData.at(1) >> comma_ >> nodeData.at(2) >> comma_ >> nodeData.at(3) >> comma_ >> nodeData[4] >> comma_ >>
			nodeData.at(5) >> comma_;
		getline(unformattedFile, tempBareNuclei, ',');
		unformattedFile >> nodeData.at(7) >> comma_ >> nodeData.at(8) >> comma_ >> nodeData.at(9) >> comma_ >> nodeData.at(10);

		// if there is a '?' mark in the data, ignore it
		if (tempBareNuclei == "?") {
			countUnknownPatients++;
			continue;
		}
		// if there is no data anymore, break the loop
		if (tempBareNuclei == "") break;
			
		// convert string to int
		nodeData.at(6) = stoi(tempBareNuclei);

		if (comma_ != ',') {
			cerr << "<Error: data should be written in order. End the program>";
			return EXIT_FAILURE;
		}
		
		// validate the data
		if (nodeData[1] < 0 || nodeData[2] < 0 || nodeData[3] < 0 || nodeData[4] < 0 ||
			nodeData[5] < 0 || nodeData[6] < 0 || nodeData[7] < 0 || nodeData[8] < 0 || nodeData[9] < 0) {
			cerr << "<Error: could not create the file. End the program>";
			return EXIT_FAILURE;
		}
		
		BinaryTree bt;
		int nodeDataSize = 11;

		// insert data into the resultFile
		for (int i = 0; i < nodeDataSize - 1; i++)
			resultFile << nodeData.at(i) << ",";
		
		// diagnosis
		std::string diagnosis_name = bt.build_pruned_tree(nodeData);

		// count the result
		if (diagnosis_name == "Benign") {
			countBenign++;
			resultFile << 2 << "\n";
		}
		else if (diagnosis_name == "Malignant") {
			countMalignant++;
			resultFile << 4 << "\n";
		}

		// insert diagnosis name
		
		
		// count processed patients 
		countPatients++;

		
	}

	// print out a summary 
	// * Total Patients Processed
	// * Total Benign
	// * Total Malignant
	std::cout << " ___________[Summary]___________" << endl;
	std::cout << "|                               |" << endl;
	std::cout << "|Total Patients Processed: " << countPatients << "  |" << endl;
	std::cout << "|            Total Benign: " << countBenign << "  |" << endl;
	std::cout << "|         Total Malignant: " << countMalignant << "  |" << endl;
	std::cout << "|_______________________________|" << endl;
	//std::cout << "Total Unknown Patients: " << countUnknownPatients << endl;

	unformattedFile.close();
	resultFile.close();

	

	
}
