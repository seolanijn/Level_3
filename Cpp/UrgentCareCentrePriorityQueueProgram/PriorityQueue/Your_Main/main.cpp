/*
author:		Seolan Jin
Date:		Nov. 10, 2021
Purpose:	This program will simulate a patient emergency room triage system with a priority patient queue.
*/
#include "Patient.h"
#include "PriorityQueue.h"
#include "LinkedList.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

PriorityQueue<Patient> queue;
LinkedList<Patient> patients_history;

int main(int argc, char* argv[])
{

	std::cout << "****************************************************" << std::endl;
	std::cout << "* Welcome to Fanshawe College Medical Center F2021 *" << std::endl;
	std::cout << "* Seolan Jin                                       *" << std::endl;
	std::cout << "****************************************************\n\n" << std::endl;

	int userSelection;
	do {
		std::cout << "\nPlease Make A Selection:\n" << std::endl;
		std::cout << "\t1 = Add Patient" << std::endl;
		std::cout << "\t2 = Process Next Patient In Queue" << std::endl;
		std::cout << "\t3 = Display Queue" << std::endl;
		std::cout << "\t4 = View Processed Patients History" << std::endl;
		std::cout << "\t5 = Load Queue" << std::endl;
		std::cout << "\t6 = Save Queue" << std::endl;
		std::cout << "\t0 = Exit\n> ";
		std::cin >> userSelection;
		
		// if the user entered 0 to end this program
		if (userSelection == 0) {
			std::cout << "End Program" << std::endl;
		} // end if
		else if (userSelection == 1) { // if the user entered 1 to add a patient
			std::string patientName, ailment;
			int severity = 0, time_c = 0, contagiousness = 0;
			bool isEntered = true;

			// get patient's name
			std::cout << "Enter patient name: ";
			std::cin.ignore();
			std::getline(std::cin, patientName, '\n');

			//validate patient's name (no duplicate)
			bool isDulplicate = false;
			auto* node = queue.begin();
			for (int i = 0; i < queue.size(); i++) {
				if (node->data.get_name() == patientName) {
					std::cout << "<Error: There is the same name in the list. Restart the program.>\n" << std::endl;
					isDulplicate = true;
					break;
				} // end if
				node = node->next;
			}// end for
			if (isDulplicate)
				continue;
			delete node;
			Patient user_patient(patientName);
			LinkedList<Ailment> ailments_;

			// get ailment's name, severity, time criticality, contagiousness
			do {
				isEntered = false;
				std::cout << "\tEnter ailment (leave blank when done): ";
				std::getline(std::cin, ailment, '\n');


				if (ailment != "") {
					isEntered = true;
					std::cout << "\tEnter severity: ";
					std::cin >> severity;
					std::cout << "\tEnter time criticality: ";
					std::cin >> time_c;
					std::cout << "\tEnter contagiousness: ";
					std::cin >> contagiousness;
					std::cout << std::endl << std::endl;

					// add ailment to the linked list
					ailments_.push_back(Ailment(ailment, severity, time_c, contagiousness));
				} // end if
				std::cin.ignore();
				ailment = "";
			} while (isEntered);

			// add ailment linked list to a patient
			while (!ailments_.empty()) {
				user_patient.add_ailment(ailments_.pop_front());
			} // end while

			// add the patient to the queue
			queue.enqueue(user_patient);
		}// end else if
		else if (userSelection == 2) { // if the user entered 2 to process next patient
			// print a patient to process
			if (!queue.empty()) {
				patients_history.push_back(queue.begin()->data); // add a processed patient to display history
				std::cout << queue.dequeue().get_name() << " moved to patient room!" << std::endl;
			}
			else {
				std::cout << "There is no patient to process." << std::endl;
				continue;
			}

			// print the next patient
			if (queue.empty())
				std::cout << "There is no next patient to process." << std::endl;
			else
				std::cout << "Next in queue: " << queue.begin()->data.get_name() << " with priority score " << queue.begin()->data.get_score() << std::endl;
		} // end else if
		else if (userSelection == 3) { // if the user entered 3 to display queue
			if (queue.empty()) {
				std::cout << "There is no patient to display." << std::endl;
				continue;
			}
			std::cout << "Patients In Queue:\n" << std::endl;

		    auto* nodeQ = queue.begin();
			for (int i = 0; i < queue.size(); i++) {
				// print patient's name and score
				std::cout << i << " : " << nodeQ->data.get_name() << " - " << nodeQ->data.get_score() << " - ";
				LinkedList<Ailment> ailment_ = nodeQ->data.get_ailments();
				// print ailment
				auto* nodeL = ailment_.begin();
				for (int j = 0; j < ailment_.size(); j++) {
					std::cout << nodeL->data.get_name() << ", ";
					nodeL = nodeL->next;
				} // end inner for
				std::cout << std::endl;
				nodeQ = nodeQ->next;
			}// end outer for
		}// end else if
		else if (userSelection == 4) { // if the user entered 4 to view processed patient history
			if (patients_history.empty()) {
				std::cout << "There is no patient history to display." << std::endl;
				continue;
			}
			std::cout << "History:" << std::endl;

			// print history
			auto* node = patients_history.end();
			for (int i = 0; i < patients_history.size(); i++) {
				std::cout << node->data.get_name() << " - " << node->data.get_score() << " - ";
				LinkedList<Ailment> ailment_ = node->data.get_ailments();
				auto* nodeL = ailment_.begin();
				for (int j = 0; j < ailment_.size(); j++) {
					std::cout << nodeL->data.get_name() << ", ";
					nodeL = nodeL->next;
				} // end inner for
				std::cout << std::endl;
				node = node->previous;
			}// end outer for
		} // end else if
		else if (userSelection == 5) { // if the user entered 5 to load queue
			std::string file_name;
			std::cout << "Enter path to file:";
			std::cin >> file_name;
			std::ifstream patients_list_file(file_name);

			if (!patients_list_file)
			{
				//report and exits if file is bad and cannot be opened
				std::cerr << "<Error: could not open the file. Restart the program>\n";
				continue;
			}
			while (!patients_list_file.eof()) {
				//get a line from the file (one patient)
				std::stringstream ss;
				std::string line_data;
				std::getline(patients_list_file, line_data, '\n');
				ss << line_data;

				// get patient's name
				std::string patient_name;
				std::getline(ss, patient_name, ',');
				Patient patient_of_file = Patient(patient_name);

				// get number of ailments
				std::string num_ailments;
				std::getline(ss, num_ailments, ',');
				
				// get the rest of data
				for (int i = 0; i < std::stoi(num_ailments); i++) {
					// get ailment name
					std::string ailment_name;
					std::getline(ss, ailment_name, ',');

					// get severity
					std::string severity;
					std::getline(ss, severity, ',');
					
					// get time criticality
					std::string time_c;
					std::getline(ss, time_c, ',');

					// get contagiousness 
					std::string contagiousness;
					std::getline(ss, contagiousness, ',');

					// add ailment using the data
					patient_of_file.add_ailment(Ailment(ailment_name, std::stoi(severity), std::stoi(time_c), std::stoi(contagiousness)));
				} // end for

				// add patient to queue
				queue.enqueue(patient_of_file);
			} // end while

			patients_list_file.close();
		} // end else if
		else if (userSelection == 6) { // if the user entered 6 to save queue
			std::string file_name;
			std::cout << "Enter path to file to save:";
			std::cin >> file_name;
			std::ofstream patients_list_file(file_name);

			if (!patients_list_file)
			{
				//report and exits if file is bad and cannot be opened
				std::cerr << "<Error: could not open the file. Restart the program>\n";
				continue;
			}

			// pass the value to the file
			auto* nodeQ = queue.begin();
			for (int i = 0; i < queue.size(); i++) {
				patients_list_file << nodeQ->data.get_name();
				LinkedList<Ailment> ailment_ = nodeQ->data.get_ailments();
				auto* nodeL = ailment_.begin();
				patients_list_file << "," << ailment_.size();
				for (int j = 0; j < ailment_.size(); j++) {
					patients_list_file << "," << nodeL->data.get_name() << "," << nodeL->data.get_severity() << "," << nodeL->data.get_time_sensitivity() << "," << nodeL->data.get_contagiousness();
					nodeL = nodeL->next;
				} // end inner for
				if (i != queue.size() - 1)
					patients_list_file << std::endl;
				nodeQ = nodeQ->next;
			} // end outer for

			patients_list_file.close();
		} // end else if
		else { // if the user entered other input, prompt the user to re-enter
			std::cout << "<Error: Invalid input. Try Again>" << std::endl;
		}

	} while (userSelection != 0); // if user entered 0, end the program

} // end main
