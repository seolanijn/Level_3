/*
Date:		Nov. 10, 2021
Purpose:	it has some useful functions to simulate Ailment class
*/
#pragma once

#include <string>

class Ailment
{
	std::string name;
	int severity;
	int time_sensitivity;
	int contagiousness;
public:
	Ailment(const std::string& name = "", int severity = 0, int time_sensitivity = 0, int contagiousness = 0);

	/*
	* fn:		bool operator==(const Ailment& rhs) const
	* brief:	determine if ailment's name is the same
	* param:	const Ailment& rhs
	* return:	bool
	*/
	bool operator==(const Ailment& rhs) const {
		return name == rhs.name;
	}
	/*
	* fn:		std::string get_name() const
	* brief:	get name
	* param:	no param
	* return:	string
	*/
	std::string get_name() const;
	/*
	* fn:		int get_severity() const
	* brief:	get severity
	* param:	no param
	* return:	int
	*/
	int get_severity() const;
	/*
	* fn:		int get_time_sensitivity() const
	* brief:	get time sensitivity
	* param:	no param
	* return: int
	*/
	int get_time_sensitivity() const;
	/*
	* fn:		int get_contagiousness() const
	* brief:	get contagiousness
	* param:	no param	
	* return:	int
	*/
	int get_contagiousness() const;
};