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

using namespace std;

[[nodiscard]] TokenList Parser::parse(TokenList const& infixTokens) {
	TokenList outputQ;
	stack<Token::pointer_type> operationS;
	

	for (Token::pointer_type x : infixTokens) {
		if (is<Operand>(x)) // if the token is an operand
			outputQ.push_back(x);
		else if (is<Function>(x)) // if the token is a function
			operationS.push(x);
		else if (is<ArgumentSeparator>(x)) { // if the token is an ArgumentSeparator
			while (operationS.top() != nullptr && !is<LeftParenthesis>(operationS.top())) {
				outputQ.push_back(operationS.top());
				operationS.pop();
			}
		}
		else if (is<LeftParenthesis>(x)) // if the token is a LeftParenthesis
			operationS.push(x);
		else if (is<RightParenthesis>(x)) { // if the token is a RightParenthesis
			while (!operationS.empty() && !is<LeftParenthesis>(operationS.top())) {
				outputQ.push_back(operationS.top());
				operationS.pop();
			}
			if (operationS.empty())
				throw runtime_error("Right parenthesis, has no matching left parenthesis");
			operationS.pop();
			if (!operationS.empty() && is<Function>(operationS.top())) {
				outputQ.push_back(operationS.top());
				operationS.pop();
			}
		}
		else if (is<Operator>(x)) { // if the token is an Operator
			while (!operationS.empty()) {
				if (!is<Operator>(operationS.top()))
					break;
				if (is<NonAssociative>(x))
					break;
				//if the token is a left associative operator and has greater precedence than the top of the operation-stack
				if (is<LAssocOperator>(x) && convert<Operator>(x)->precedence() > convert<Operator>(operationS.top())->precedence())
					break;
				//if the token is a right associative operator and has greater or equal precedence than the top of the operation-stack
				if (is<RAssocOperator>(x) && convert<Operator>(x)->precedence() >= convert<Operator>(operationS.top())->precedence())
					break;
				outputQ.push_back(operationS.top());
				operationS.pop();
			}
			operationS.push(x);
		}
		else
			throw runtime_error("Unknown token");
	}

	//while the operation stack is not empty
	while (!operationS.empty()) {
		if (is<LeftParenthesis>(operationS.top()))
			throw runtime_error("Missing right - parenthesis");
		outputQ.push_back(operationS.top());
		operationS.pop();
	}

	return outputQ;
}

