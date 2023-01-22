/*
Date:		Nov. 10, 2021
Purpose:	it has some useful functions to simulate priority queue
*/
#pragma once

template <typename T>
class PriorityQueue
{
	LinkedList<T> patients_; 

public:

	PriorityQueue() = default;

	LinkedList<Patient>::Node* begin() { return patients_.begin(); }
	LinkedList<Patient>::Node* end() { return patients_.end(); }

	// return bool if it is empty, false if not
	bool empty() { return patients_.empty(); }
	// return the size of current patients linked list
	size_t size() { return patients_.size(); }

	void enqueue(const T& item) {
		patients_.push_back(item); // sort the queue
		queue_sort(patients_);
	}
	T dequeue() {
		return patients_.pop_front();
	}
private:
	/*
	* fn:		queue_sort(LinkedList<Patient>& priorityQueue_);
	* brief:	sorts queue
	* param:	LinkedList<Patient>& priorityQueue_
	* return:	void
	*/
	void queue_sort(LinkedList<Patient>& priorityQueue_) {
		const size_t list_size = priorityQueue_.size();

		// bubble sort
		for (int i = 0; i < list_size - 1; i++) {
			bool hasSwapped = false;

			auto* node = priorityQueue_.begin();
			for (int j = 0; j < list_size - i - 1; j++) {
				if (node->data.get_score() < node->next->data.get_score()) {
					hasSwapped = true;
					swap(node->data, node->next->data);
				}
				// move the node
				node = node->next;
			}
			if (!hasSwapped)
				break;
		}
	}
	/*
	* fn:		inline void swap(Patient& a, Patient& b)
	* brief:	swap the value
	* param:	Patient& a, Patient& b
	* return:	void
	*/
	inline void swap(Patient& a, Patient& b)
	{
		Patient temp = a;
		a = b;
		b = temp;
	}
};
