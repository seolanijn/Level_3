/*!	\file	parser.cpp
	\brief	Parser class implementation.
	\author	Garth Santor
	\date	2021-10-29
	\copyright	Garth Santor, Trinh Han
=============================================================
Revision History
-------------------------------------------------------------

Version 2021.11.01
	C++ 20 validated
	Changed to GATS_TEST

Version 2014.10.31
	Visual C++ 2013

Version 2012.11.13
	C++ 11 cleanup

Version 2009.12.02
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

#include <ee/parser.hpp>
#include <ee/function.hpp>
#include <ee/operand.hpp>
#include <ee/operator.hpp>
#include <ee/pseudo_operation.hpp>
#include <stack>

#include <queue>
#include <ee/operation.hpp>
//#include <chrono>

using namespace std;
//using namespace std::chrono;

[[nodiscard]] TokenList Parser::parse(TokenList const& infixTokens) {
	TokenList outputQ;
	stack<Token::pointer_type> operationS;
	

	for (Token::pointer_type x : infixTokens) {
		if (is<Operand>(x))
			outputQ.push_back(x);
		else if (is<Function>(x))
			operationS.push(x);
		else if (is<ArgumentSeparator>(x)) {
			while (operationS.top() != nullptr && !is<LeftParenthesis>(operationS.top())) {
				outputQ.push_back(operationS.top());
				operationS.pop();
			}
		}
		else if (is<LeftParenthesis>(x))
			operationS.push(x);
		else if (is<RightParenthesis>(x)) {
			while (!operationS.empty() && !is<LeftParenthesis>(operationS.top())) {
				outputQ.push_back(operationS.top());
				operationS.pop();
			}
			if (operationS.empty())
				throw "Right parenthesis, has no matching left parenthesis";
			operationS.pop();
			if (!operationS.empty() && is<Function>(operationS.top())) {
				outputQ.push_back(operationS.top());
				operationS.pop();
			}
		}
		else if (is<Operator>(x)) {
			while (!operationS.empty()) {
				if (!is<Operator>(operationS.top()))
					break;
				if (is<NonAssociative>(x))
					break;
				if (is<LAssocOperator>(x) && comparisonAssociative(x, operationS.top()) == 1)
					break;
				if (is<RAssocOperator>(x) && (comparisonAssociative(x, operationS.top()) == 1 || comparisonAssociative(x, operationS.top()) == 0))
					break;
				outputQ.push_back(operationS.top());
				operationS.pop();
			}
			operationS.push(x);
		}
		else
			throw "Unknown token";
	}
	while (!operationS.empty()) {
		if (is<LeftParenthesis>(operationS.top()))
			throw "Missing right-parenthesis";
		outputQ.push_back(operationS.top());
		operationS.pop();
	}

	//TokenList output;
	//while (!outputQ.empty()) {
	//	output.push_back(outputQ.front());
	//	outputQ.push_back();
	//}

	// The following line is just a placeholder until you have completed the parser.
	return outputQ;
}

// return 0 if token == top
// return 1 if token > top
// return -1 if token < top
[[nodiscard]] int Parser::comparisonAssociative(Token::pointer_type const& tkn, Token::pointer_type const& top) {
	
	if (find_index(tkn) == find_index(top)) return 0;
	else if (find_index(tkn) > find_index(top)) return 1;
	else return -1;
	                
}
[[nodiscard]] int Parser::find_index(Token::pointer_type const& tkn) {
	vector<string> precedence = { "<Assignment>",
									"<Nor>", "<Or>",
									"<Xor>", "<Xnor>",
									"<Nand>", "<And>",
									"<Equality>", "<Inequality>",
									"<Greater>", "<GreaterEqual>", "<Less>", "<LessEqual>",
									"<Addition>", "<Subtraction>",
									"<Multiplication>", "<Division>", "<Modulus>",
									"<Power>" };

	int idx = -1;

	for (string x : precedence) {
		++idx;
		if (x == tkn->str()) break;
	}

	if (idx == 2) idx = 1;
	else if (idx == 4) idx = 3;
	else if (idx == 6) idx = 5;
	else if (idx == 8) idx = 7;
	else if (idx == 10 || idx == 11 || idx == 12) idx = 9;
	else if (idx == 14) idx = 13;
	else if (idx == 16 || idx == 17) idx = 15;

	return idx;
}

