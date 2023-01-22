/*
Date:		Nov. 10, 2021
Purpose:	it has some useful functions to simulate Patient class
*/

#include "Patient.h"

// get name
std::string Patient::get_name() const {
	return name_; 
}
//add ailment to current patient
void Patient::add_ailment(const Ailment& ailment) {
	ailments_.push_back(ailment);
}
//get current patient's ailment linked list
LinkedList<Ailment> Patient::get_ailments() const {
	return ailments_;
}
//get score of current patient
int Patient::get_score() const {
	const auto* ptr = get_ailments().begin();

	int score = 0;
	while (ptr != nullptr)
	{
		// calculate the score
		score += static_cast<int>(ptr->data.get_severity() * (ptr->data.get_time_sensitivity()) + (ptr->data.get_contagiousness()));
		ptr = ptr->next;
	}
	return score;
}