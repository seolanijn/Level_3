/*
Date:		Nov. 10, 2021
Purpose:	it has some useful functions to simulate Ailment class
*/

#include "Ailment.h"

Ailment::Ailment(const std::string& name, int severity, int time_sensitivity, int contagiousness) {
	this->name = name;
	this->severity = severity;
	this->time_sensitivity = time_sensitivity;
	this->contagiousness = contagiousness;
}

//get name
std::string Ailment::get_name() const {
	return name;
}
//get severity
int Ailment::get_severity() const {
	return severity;
}
//get time sensitivity
int Ailment::get_time_sensitivity() const {
	return time_sensitivity;
}
//get contagiousness
int Ailment::get_contagiousness() const {
	return contagiousness;
}