#pragma once
/*
author:		Seolan Jin
Date:		Dec. 10, 2021
Purpose:	It will simulate binary tree
*/

#include <string>
#include <vector>

class BinaryTree {
public: 
	struct Node {
		Node* left_child;
		Node* right_child;
		Node* parent_node;
		
		// Ex. (Uniformity of Cell Size <= 2)
		int compareInt;	// will store 2
		int compareIdx;	// will store the index number which has Cell Size

		// Node constructor
		Node(const int compareInt, const int compareIdx, Node* parent_ = nullptr) : left_child(nullptr), right_child(nullptr), parent_node(parent_)
		{
			this->compareInt = compareInt;
			this->compareIdx = compareIdx;
		}
	};

	Node* root_node = nullptr;

	/*
	\fn:		std::string diagnosis(Node* const node_);
	\brief:		This recursive function determines if it is Benign or Malignant
	\param:		Node* const node_
	\return:	std::string "Benign" or "Malignant"
	*/
	std::string diagnosis(Node* const node_);

	/*
	\fn:		std::string build_pruned_tree(std::vector<int> nodeData);
	\brief:		This function will build the binary tree each node will store if statement
	\param:		std::vector<int> nodeData
	\return:	std::string
	*/
	std::string build_pruned_tree(std::vector<int> nodeData);
private:
	std::vector<int> nodeData;
};

std::string BinaryTree::diagnosis(Node* const node_)
{
	if (node_->compareInt == -1) {
		if (node_->compareIdx == 22)
			return "Benign";
		else
			return "Malignant";
	}
	else {
		if (nodeData[node_->compareIdx] <= node_->compareInt)
			return diagnosis(node_->right_child);
		else
			return diagnosis(node_->left_child);
	}
}

std::string BinaryTree::build_pruned_tree(std::vector<int> nodeData)
{
	this->nodeData = nodeData;

	// diagnosis class node
	Node* Benign_node = new Node(-1, 22);
	Node* Malignant_node = new Node(-1, 44);

	// if statement node
	Node* cellSize_2_node = new Node(2, 2);
	Node* cellShape_2_node = new Node(2, 3, cellSize_2_node);
	Node* bare_3_node = new Node(3, 6, cellSize_2_node);
	Node* cellSize_4_node = new Node(4, 2, cellShape_2_node);
	Node* clump_5_node = new Node(5, 1, cellShape_2_node);
	Node* clump_3_node = new Node(3, 1, bare_3_node);
	Node* bare_2_node = new Node(2, 6, cellSize_4_node);
	Node* bland_2_node = new Node(2, 7, clump_3_node);
	Node* clump_6_node = new Node(6, 1, bare_2_node);
	Node* adhesion_3_node_L = new Node(3, 4, bare_2_node);
	Node* adhesion_3_node_R = new Node(3, 4, bland_2_node);
	Node* cellSize_3_node = new Node(3, 2, clump_6_node);
	Node* adhesion_5_node = new Node(5, 4, cellSize_3_node);

	// root
	root_node = cellSize_2_node;
	cellSize_2_node->left_child = cellShape_2_node;
	cellSize_2_node->right_child = bare_3_node;

	// left (cellSize_2_node)
	cellShape_2_node->left_child = cellSize_4_node;
	cellShape_2_node->right_child = clump_5_node;

	clump_5_node->left_child = Malignant_node;
	clump_5_node->right_child = Benign_node;

	cellSize_4_node->left_child = Malignant_node;
	cellSize_4_node->right_child = bare_2_node;

	bare_2_node->left_child = clump_6_node;
	bare_2_node->right_child = adhesion_3_node_L;

	adhesion_3_node_L->left_child = Malignant_node;
	adhesion_3_node_L->right_child = Benign_node;

	clump_6_node->left_child = Malignant_node;
	clump_6_node->right_child = cellSize_3_node;

	cellSize_3_node->left_child = adhesion_5_node;
	cellSize_3_node->right_child = Malignant_node;

	adhesion_5_node->left_child = Malignant_node;
	adhesion_5_node->right_child = Benign_node;

	// right (cellSize_2_node)
	bare_3_node->left_child = clump_3_node;
	bare_3_node->right_child = Benign_node;

	clump_3_node->left_child = bland_2_node;
	clump_3_node->right_child = Benign_node;

	bland_2_node->left_child = Malignant_node;
	bland_2_node->right_child = adhesion_3_node_R;

	adhesion_3_node_R->left_child = Benign_node;
	adhesion_3_node_R->right_child = Malignant_node;

	// call recursive function
	return diagnosis(cellSize_2_node);
}