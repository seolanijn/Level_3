/*
Date:		Nov. 10, 2021
Purpose:	it has some useful functions to simulate Patient class
*/
#pragma once
#include "Ailment.h"
#include "LinkedList.h"
#include <string>
#include <iostream>
#include <utility>

class Patient
{
	std::string name_;
	LinkedList<Ailment> ailments_;

public:
	Patient(std::string name = ""):name_(std::move(name)) {}

	/*
	* fn:		std::string get_name() const
	* brief:	get name
	* param:	no param
	* return:	string type of patient's name
	*/
	std::string get_name() const;

	/*
	* fn:		void add_ailment(const Ailment& ailment)
	* brief:	add ailment to current patient
	* param:	const Ailment& ailment
	* return:	void
	*/
	void add_ailment(const Ailment& ailment);
	/*
	* fn:		LinkedList<Ailment> get_ailments() const
	* brief:	get current patient's ailment linked list
	* param:	no param
	* return:	LinkedList<Ailment>
	*/
	LinkedList<Ailment> get_ailments() const;
	/*
	* fn:		int get_score() const
	* brief:	get score of current patient
	* param:	no param
	* return:	int
	*/
	int get_score() const;
	/*
	* fn:		bool operator==(const Patient& rhs) const
	* brief:	detetmines if the patients name is the same
	* param:	const Patient& rhs
	* return:	bool
	*/
	bool operator==(const Patient& rhs) const {
		return name_ == rhs.name_;
	}
};

