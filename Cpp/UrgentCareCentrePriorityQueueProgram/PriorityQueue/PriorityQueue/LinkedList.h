/*
Date:		Nov. 10, 2021
Purpose:	it has some useful functions to simulate linked list
*/
#pragma once

template <typename T>
class LinkedList
{
public:
	struct Node
	{
		Node* previous = nullptr;
		Node* next = nullptr;

		T data;
	};

protected:
	Node* begin_;
	Node* end_;

public:
	LinkedList() : begin_(nullptr), end_(nullptr) {}
	~LinkedList() = default;

	Node* begin() { return begin_; }
	Node* end() { return end_; }

	/*
	* fn:		void push_front(const T& item)
	* brief:	add data to the front
	* param:	const T& item
	* return:	void
	*/
	void push_front(const T& item)
	{
		Node* node = new Node();
		node->data = item;

		//check if there is a node to link in-front of
		if (begin_ != nullptr)
		{
			begin_->previous = node;
			node->next = begin_;
		}
		else
		{
			end_ = node;
		}

		begin_ = node;
	}
	/*
	* fn:		void push_back(const T& item)
	* brief:	add data to the end
	* param:	const T& item
	* return:	void
	*/
	void push_back(const T& item)
	{

		Node* node = new Node();
		node->data = item;
		//check if there is a node to link in-front of
		if (begin_ == nullptr)
		{
			node->previous = nullptr;
			begin_ = node;
		}
		else
		{
			end_->next = node;
			node->previous = end_;
		}

		end_ = node;
	}
	/*
	* fn:		T pop_front()
	* brief:	remove the first data
	* param:	no param
	* return:	removed data type of T
	*/
	T pop_front()
	{
		//check if there is no data to remove
		if (begin_ == nullptr)
		{
			//todo: throw
			throw "";
		}

		Node* node = begin_;
		const T value = node->data;
		begin_ = node->next;

		delete node;

		return value;
	}
	/*
	* fn:		T pop_back()
	* brief:	remove the last data
	* param:	no param
	* return:	removed data type of T
	*/
	T pop_back()
	{
		//check if there is no data to remove
		if (end_ == nullptr)
		{
			//todo: throw
			throw "";
		}

		Node* node = end_;
		const T value = node->data;
		end_ = node->previous;

		return value;
	}
	/*
	* fn:		bool empty() const
	* brief:	check if the linked list is empty
	* param:	no param
	* return:	bool
	*/
	bool empty() const
	{
		return begin_ == nullptr;
	}
	/*
	* fn:		size_t size() const
	* brief:	get the size of linked list
	* param:	no param
	* return:	size_t
	*/
	size_t size() const
	{
		size_t counter = 0;

		Node* node = begin_;
		while (node != nullptr)
		{
			++counter;
			node = node->next;
		}

		return counter;
	}
};
