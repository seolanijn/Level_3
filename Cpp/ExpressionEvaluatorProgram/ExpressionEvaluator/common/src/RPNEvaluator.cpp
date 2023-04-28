/*!	\file	RPNEvaluator.cpp
	\brief	RPN Evaluator class implementation.
	\author	Garth Santor
	\date	2021-10-29
	\copyright	Garth Santor, Trinh Han

=============================================================
Revision History
-------------------------------------------------------------

Version 2021.11.01
	C++ 20 validated
	Changed to GATS_TEST

Version 2012.11.13
	C++ 11 cleanup

Version 2009.12.10
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

#include <ee/RPNEvaluator.hpp>

#include <ee/integer.hpp>
#include <ee/real.hpp>
#include <ee/boolean.hpp>
#include <ee/variable.hpp>

#include <ee/operation.hpp>
#include <cassert>
#include <stack>


#include <boost/multiprecision/cpp_int.hpp>
#include <boost/multiprecision/cpp_dec_float.hpp>
#include <boost/math/special_functions.hpp>
#include <boost/multiprecision/detail/default_ops.hpp>
#include <boost/multiprecision/debug_adaptor.hpp>
#include <boost/math/constants/constants.hpp>
using real_type = boost::multiprecision::number<boost::multiprecision::cpp_dec_float<1000, int32_t, void>>;
using int_type = boost::multiprecision::cpp_int;
using namespace std;


[[nodiscard]] Operand::pointer_type RPNEvaluator::evaluate(TokenList const& rpnExpression) {

	//if the expression is empty 
	if (rpnExpression.empty()) throw runtime_error("Error: insufficient operands");

	stack<Token::pointer_type> operandS;

	for (Token::pointer_type tkn : rpnExpression) {
		Token::pointer_type temp1, temp2, tempResult;
		
		if (is<Operand>(tkn))// if the token is operand push the operand to the operand stack
			operandS.push(tkn);
		else { // if not it is operator
			Operation::pointer_type tempTkn = convert<Operation>(tkn);
			if (tempTkn->number_of_args() > operandS.size())
				throw runtime_error("Error: insufficient operands");
			if (tempTkn->number_of_args() == 2) {
				temp1 = operandS.top(); operandS.pop();
				temp2 = operandS.top(); operandS.pop();
			}
			else if (tempTkn->number_of_args() == 1) {
				temp1 = operandS.top(); operandS.pop();
			}

			// Big If

			// <Operator>
			//RAssocOperator
			if (tkn->str() == "<Power>") {
				if (is<Integer>(temp1)) {
					if (is<Real>(temp2)) {
						if (value_of<Integer>(temp1) < 0)
							tempResult = make<Real>(1 / boost::multiprecision::pow(value_of<Real>(temp2), value_of<Integer>(temp1).convert_to<real_type>() * -1));
						else
							tempResult = make<Real>(boost::multiprecision::pow(value_of<Real>(temp2), value_of<Integer>(temp1).convert_to<real_type>()));
					}
					else if (is<Integer>(temp2)) {
						if (value_of<Integer>(temp1) < 0)
							tempResult = make<Real>(1 / boost::multiprecision::pow(value_of<Integer>(temp2).convert_to<real_type>(), (value_of<Integer>(temp1) * -1).convert_to<real_type>()));
						else
							tempResult = make<Integer>(boost::multiprecision::pow(value_of<Integer>(temp2).convert_to<real_type>(), value_of<Integer>(temp1).convert_to<real_type>()).convert_to<int_type>());
					}
					else
						throw runtime_error("Error: Invalid Operand Type");
				}
				else if (is<Real>(temp1)) {
					if (is<Real>(temp2)) {
						if (value_of<Real>(temp1) < 0)
							tempResult = make<Real>(1 / boost::multiprecision::pow(value_of<Real>(temp2), value_of<Real>(temp1) * -1));
						else
							tempResult = make<Real>(boost::multiprecision::pow(value_of<Real>(temp2), value_of<Real>(temp1)));
					}
					else if (is<Integer>(temp2)) {
						if (value_of<Real>(temp1) < 0)
							tempResult = make<Real>(1 / boost::multiprecision::pow(value_of<Integer>(temp2).convert_to<real_type>(), value_of<Real>(temp1) * -1));
						else
							tempResult = make<Real>(boost::multiprecision::pow(value_of<Integer>(temp2).convert_to<real_type>(), value_of<Real>(temp1)));
					}
					else
						throw runtime_error("Error: Invalid Operand Type");
				}
				else if (is<Variable>(temp1)) {
					Operand::pointer_type tempVar = convert<Variable>(temp1)->value();
					if (tempVar == nullptr)
						throw runtime_error("Error: variable not initialized");
					if (is<Integer>(tempVar)) {
						if (is<Real>(temp2)) {
							if (value_of<Integer>(tempVar) < 0)
								tempResult = make<Real>(1 / boost::multiprecision::pow(value_of<Real>(temp2), value_of<Integer>(tempVar).convert_to<real_type>() * -1));
							else
								tempResult = make<Real>(boost::multiprecision::pow(value_of<Real>(temp2), value_of<Integer>(tempVar).convert_to<real_type>()));
						}
						else if (is<Integer>(temp2)) {
							if (value_of<Integer>(tempVar) < 0)
								tempResult = make<Real>(1 / boost::multiprecision::pow(value_of<Integer>(temp2).convert_to<real_type>(), (value_of<Integer>(tempVar) * -1).convert_to<real_type>()));
							else
								tempResult = make<Integer>(boost::multiprecision::pow(value_of<Integer>(temp2).convert_to<real_type>(), value_of<Integer>(tempVar).convert_to<real_type>()).convert_to<int_type>());
						}
						else
							throw runtime_error("Error: Invalid Operand Type");
					}
					else if (is<Real>(tempVar)) {
						if (is<Real>(temp2)) {
							if (value_of<Real>(tempVar) < 0)
								tempResult = make<Real>(1 / boost::multiprecision::pow(value_of<Real>(temp2), value_of<Real>(tempVar) * -1));
							else
								tempResult = make<Real>(boost::multiprecision::pow(value_of<Real>(temp2), value_of<Real>(tempVar)));
						}
						else if (is<Integer>(temp2)) {
							if (value_of<Real>(tempVar) < 0)
								tempResult = make<Real>(1 / boost::multiprecision::pow(value_of<Integer>(temp2).convert_to<real_type>(), value_of<Real>(tempVar) * -1));
							else
								tempResult = make<Real>(boost::multiprecision::pow(value_of<Integer>(temp2).convert_to<real_type>(), value_of<Real>(tempVar)));
						}
						else
							throw runtime_error("Error: Invalid Operand Type");
					}
					else
						throw runtime_error("Error: Invalid Operand Type");
				}
				else
					throw runtime_error("Error: Invalid Operand Type");
			}
			else if (tkn->str() == "<Assignment>") {
				if (is<Variable>(temp2)) {
					if (is<Integer>(temp1))
						convert<Variable>(temp2)->set(convert<Integer>(temp1));
					else if (is<Real>(temp1))
						convert<Variable>(temp2)->set(convert<Real>(temp1));
					else if (is<Boolean>(temp1))
						convert<Variable>(temp2)->set(convert<Boolean>(temp1));
					else
						throw runtime_error("Error: Invalid Operand Type");
				}
				else
					throw runtime_error("Error: assignment to a non-variable.");
				tempResult = convert<Variable>(temp2);
			}
			//LAssocOperator
			else if (tkn->str() == "<Addition>") {
				if (is<Integer>(temp1)) {
					if (is<Real>(temp2)) {
						tempResult = make<Real>(value_of<Integer>(temp1).convert_to<real_type>() + value_of<Real>(temp2));
					}
					else if (is<Integer>(temp2)) {
						tempResult = make<Integer>(value_of<Integer>(temp1) + value_of<Integer>(temp2));
					}
					else if (is<Variable>(temp2)) {
						if (is<Integer>(value_of<Variable>(temp2)))
							tempResult = make<Integer>(value_of<Integer>(value_of<Variable>(temp2)) + value_of<Integer>(temp1));
						else if (is<Real>(value_of<Variable>(temp2)))
							tempResult = make<Real>(value_of<Real>(value_of<Variable>(temp2)) + value_of<Integer>(temp1).convert_to<real_type>());
					}
					else
						throw runtime_error("Error: Invalid Operand Type");
				}
				else if (is<Real>(temp1)) {
					if (is<Real>(temp2)) {
						tempResult = make<Real>(value_of<Real>(temp1) + value_of<Real>(temp2));
					}
					else if (is<Integer>(temp2)) {
						tempResult = make<Real>(value_of<Real>(temp1) + value_of<Integer>(temp2).convert_to<real_type>());
					}
					else
						throw runtime_error("Error: Invalid Operand Type");
				}
				else if (is<Variable>(temp1)) {
					Operand::pointer_type tempVar = convert<Variable>(temp1)->value();
					if (tempVar == nullptr)
						throw runtime_error("Error: variable not initialized");
					if (is<Integer>(tempVar)) {
						if (is<Real>(temp2)) {
							tempResult = make<Real>(value_of<Integer>(tempVar).convert_to<real_type>() + value_of<Real>(temp2));
						}
						else if (is<Integer>(temp2)) {
							tempResult = make<Integer>(value_of<Integer>(tempVar) + value_of<Integer>(temp2));
						}
						else
							throw runtime_error("Error: Invalid Operand Type");
					}
					else if (is<Real>(tempVar)) {
						if (is<Real>(temp2)) {
							tempResult = make<Real>(value_of<Real>(tempVar) + value_of<Real>(temp2));
						}
						else if (is<Integer>(temp2)) {
							tempResult = make<Real>(value_of<Real>(tempVar) + value_of<Integer>(temp2).convert_to<real_type>());
						}
						else
							throw runtime_error("Error: Invalid Operand Type");
					}
					else
						throw runtime_error("Error: Invalid Operand Type");
				}
				else
					throw runtime_error("Error: Invalid Operand Type");
			}
			else if (tkn->str() == "<And>") {
				if (is<Boolean>(temp1) && is<Boolean>(temp2)) {
					tempResult = make<Boolean>(value_of<Boolean>(temp1) && value_of<Boolean>(temp2));
				}
				else if (is<Boolean>(temp1) && is<Variable>(temp2)) {
					tempResult = make<Boolean>(value_of<Boolean>(temp1) && value_of<Boolean>(value_of<Variable>(temp2)));
				}
				else if (is<Variable>(temp1)) {
					Operand::pointer_type tempVar = convert<Variable>(temp1)->value();
					if (tempVar == nullptr)
						throw runtime_error("Error: variable not initialized");
					if (is<Boolean>(tempVar) && is<Boolean>(temp2))
						tempResult = make<Boolean>(value_of<Boolean>(tempVar) && value_of<Boolean>(temp2));
					else
						throw runtime_error("Error: Invalid Operand Type");
				}
				else
					throw runtime_error("Error: Invalid Operand Type");
			}
			else if (tkn->str() == "<Division>") {
				if (is<Integer>(temp1)) {
					if (is<Real>(temp2)) {
						tempResult = make<Real>(value_of<Real>(temp2) / value_of<Integer>(temp1).convert_to<real_type>());
					}
					else if (is<Integer>(temp2)) {
						tempResult = make<Integer>(value_of<Integer>(temp2) / value_of<Integer>(temp1));
					}
					else if (is<Variable>(temp2)) {
						if (is<Integer>(value_of<Variable>(temp2)))
							tempResult = make<Integer>(value_of<Integer>(value_of<Variable>(temp2)) / value_of<Integer>(temp1));
						else if (is<Real>(value_of<Variable>(temp2)))
							tempResult = make<Real>(value_of<Real>(value_of<Variable>(temp2)) / value_of<Integer>(temp1).convert_to<real_type>());
					}
					else
						throw runtime_error("Error: Invalid Operand Type");
				}
				else if (is<Real>(temp1)) {
					if (is<Real>(temp2)) {
						tempResult = make<Real>(value_of<Real>(temp2) / value_of<Real>(temp1));
					}
					else if (is<Integer>(temp2)) {
						tempResult = make<Real>(value_of<Integer>(temp2).convert_to<real_type>() / value_of<Real>(temp1));
					}
					else
						throw runtime_error("Error: Invalid Operand Type");
				}
				else if (is<Variable>(temp1)) {
					Operand::pointer_type tempVar = convert<Variable>(temp1)->value();
					if (tempVar == nullptr)
						throw runtime_error("Error: variable not initialized");
					if (is<Integer>(tempVar)) {
						if (is<Real>(temp2)) {
							tempResult = make<Real>(value_of<Real>(temp2) / value_of<Integer>(tempVar).convert_to<real_type>());
						}
						else if (is<Integer>(temp2)) {
							tempResult = make<Integer>(value_of<Integer>(temp2) / value_of<Integer>(tempVar));
						}
						else
							throw runtime_error("Error: Invalid Operand Type");
					}
					else if (is<Real>(tempVar)) {
						if (is<Real>(temp2)) {
							tempResult = make<Real>(value_of<Real>(temp2) / value_of<Real>(tempVar));
						}
						else if (is<Integer>(temp2)) {
							tempResult = make<Real>(value_of<Integer>(temp2).convert_to<real_type>() / value_of<Real>(tempVar));
						}
						else
							throw runtime_error("Error: Invalid Operand Type");
					}
					else
						throw runtime_error("Error: Invalid Operand Type");
				}
				else
					throw runtime_error("Error: Invalid Operand Type");
			}
			else if (tkn->str() == "<Equality>")  {
				if (is<Integer>(temp1)) {
					if (is<Real>(temp2)) {
						tempResult = make<Boolean>(value_of<Integer>(temp1).convert_to<real_type>() == value_of<Real>(temp2));
					}
					else if (is<Integer>(temp2)) {
						tempResult = make<Boolean>(value_of<Integer>(temp1) == value_of<Integer>(temp2));
					}
					else
						throw runtime_error("Error: Invalid Operand Type");
				}
				else if (is<Boolean>(temp1) && is<Boolean>(temp2)) {
					tempResult = make<Boolean>(value_of<Boolean>(temp1) == value_of<Boolean>(temp2));
				}
				else if (is<Boolean>(temp1) && is<Variable>(temp2)) {
					tempResult = make<Boolean>(value_of<Boolean>(temp1) == value_of<Boolean>(value_of<Variable>(temp2)));
				}
				else if (is<Real>(temp1)) {
					if (is<Real>(temp2)) {
						tempResult = make<Boolean>(value_of<Real>(temp1) == value_of<Real>(temp2));
					}
					else if (is<Integer>(temp2)) {
						tempResult = make<Boolean>(value_of<Real>(temp1) == value_of<Integer>(temp2).convert_to<real_type>());
					}
					else
						throw runtime_error("Error: Invalid Operand Type");
				}
				else if (is<Variable>(temp1)) {
					Operand::pointer_type tempVar = convert<Variable>(temp1)->value();
					if (tempVar == nullptr)
						throw runtime_error("Error: variable not initialized");
					if (is<Integer>(tempVar)) {
						if (is<Real>(temp2)) {
							tempResult = make<Boolean>(value_of<Integer>(tempVar).convert_to<real_type>() == value_of<Real>(temp2));
						}
						else if (is<Integer>(temp2)) {
							tempResult = make<Boolean>(value_of<Integer>(tempVar) == value_of<Integer>(temp2));
						}
						else
							throw runtime_error("Error: Invalid Operand Type");
					}
					else if (is<Boolean>(tempVar) && is<Boolean>(temp2)) {
						tempResult = make<Boolean>(value_of<Boolean>(tempVar) == value_of<Boolean>(temp2));
					}
					else if (is<Real>(tempVar)) {
						if (is<Real>(temp2)) {
							tempResult = make<Boolean>(value_of<Real>(tempVar) == value_of<Real>(temp2));
						}
						else if (is<Integer>(temp2)) {
							tempResult = make<Boolean>(value_of<Real>(tempVar) == value_of<Integer>(temp2).convert_to<real_type>());
						}
						else
							throw runtime_error("Error: Invalid Operand Type");
					}
					else
						throw runtime_error("Error: Invalid Operand Type");
				}
				else
					throw runtime_error("Error: Invalid Operand Type");

			}
			else if (tkn->str() == "<Greater>") {
				if (is<Integer>(temp1)) {
					if (is<Real>(temp2)) {
						tempResult = make<Boolean>(value_of<Integer>(temp1).convert_to<real_type>() < value_of<Real>(temp2));
					}
					else if (is<Integer>(temp2)) {
						tempResult = make<Boolean>(value_of<Integer>(temp1) < value_of<Integer>(temp2));
					}
					else
						throw runtime_error("Error: Invalid Operand Type");
				}
				else if (is<Boolean>(temp1) && is<Boolean>(temp2)) {
					tempResult = make<Boolean>(value_of<Boolean>(temp1) < value_of<Boolean>(temp2));
				}
				else if (is<Real>(temp1)) {
					if (is<Real>(temp2)) {
						tempResult = make<Boolean>(value_of<Real>(temp1) < value_of<Real>(temp2));
					}
					else if (is<Integer>(temp2)) {
						tempResult = make<Boolean>(value_of<Real>(temp1) < value_of<Integer>(temp2).convert_to<real_type>());
					}
					else
						throw runtime_error("Error: Invalid Operand Type");
				}
				else if (is<Variable>(temp1)) {
					Operand::pointer_type tempVar = convert<Variable>(temp1)->value();
					if (tempVar == nullptr)
						throw runtime_error("Error: variable not initialized");
					if (is<Integer>(tempVar)) {
						if (is<Real>(temp2)) {
							tempResult = make<Boolean>(value_of<Integer>(tempVar).convert_to<real_type>() < value_of<Real>(temp2));
						}
						else if (is<Integer>(temp2)) {
							tempResult = make<Boolean>(value_of<Integer>(tempVar) < value_of<Integer>(temp2));
						}
						else
							throw runtime_error("Error: Invalid Operand Type");
					}
					else if (is<Boolean>(tempVar) && is<Boolean>(temp2)) {
						tempResult = make<Boolean>(value_of<Boolean>(tempVar) < value_of<Boolean>(temp2));
					}
					else if (is<Real>(tempVar)) {
						if (is<Real>(temp2)) {
							tempResult = make<Boolean>(value_of<Real>(tempVar) < value_of<Real>(temp2));
						}
						else if (is<Integer>(temp2)) {
							tempResult = make<Boolean>(value_of<Real>(tempVar) < value_of<Integer>(temp2).convert_to<real_type>());
						}
						else
							throw runtime_error("Error: Invalid Operand Type");
					}
					else
						throw runtime_error("Error: Invalid Operand Type");
				}
				else
					throw runtime_error("Error: Invalid Operand Type");
			}
			else if (tkn->str() == "<GreaterEqual>") {
				if (is<Integer>(temp1)) {
					if (is<Real>(temp2)) {
						tempResult = make<Boolean>(value_of<Integer>(temp1).convert_to<real_type>() <= value_of<Real>(temp2));
					}
					else if (is<Integer>(temp2)) {
						tempResult = make<Boolean>(value_of<Integer>(temp1) <= value_of<Integer>(temp2));
					}
					else
						throw runtime_error("Error: Invalid Operand Type");
				}
				else if (is<Boolean>(temp1) && is<Boolean>(temp2)) {
					tempResult = make<Boolean>(value_of<Boolean>(temp1) <= value_of<Boolean>(temp2));
				}
				else if (is<Real>(temp1)) {
					if (is<Real>(temp2)) {
						tempResult = make<Boolean>(value_of<Real>(temp1) <= value_of<Real>(temp2));
					}
					else if (is<Integer>(temp2)) {
						tempResult = make<Boolean>(value_of<Real>(temp1) <= value_of<Integer>(temp2).convert_to<real_type>());
					}
					else
						throw runtime_error("Error: Invalid Operand Type");
				}
				else if (is<Variable>(temp1)) {
					Operand::pointer_type tempVar = convert<Variable>(temp1)->value();
					if (tempVar == nullptr)
						throw runtime_error("Error: variable not initialized");
					if (is<Integer>(tempVar)) {
						if (is<Real>(temp2)) {
							tempResult = make<Boolean>(value_of<Integer>(tempVar).convert_to<real_type>() <= value_of<Real>(temp2));
						}
						else if (is<Integer>(temp2)) {
							tempResult = make<Boolean>(value_of<Integer>(tempVar) <= value_of<Integer>(temp2));
						}
						else
							throw runtime_error("Error: Invalid Operand Type");
					}
					else if (is<Boolean>(tempVar) && is<Boolean>(temp2)) {
						tempResult = make<Boolean>(value_of<Boolean>(tempVar) <= value_of<Boolean>(temp2));
					}
					else if (is<Real>(tempVar)) {
						if (is<Real>(temp2)) {
							tempResult = make<Boolean>(value_of<Real>(tempVar) <= value_of<Real>(temp2));
						}
						else if (is<Integer>(temp2)) {
							tempResult = make<Boolean>(value_of<Real>(tempVar) <= value_of<Integer>(temp2).convert_to<real_type>());
						}
						else throw runtime_error("Error: Invalid Operand Type");
					}
					else throw runtime_error("Error: Invalid Operand Type");
				}
				else throw runtime_error("Error: Invalid Operand Type");
			}
			else if (tkn->str() == "<Inequality>") {
				if (is<Integer>(temp1)) {
					if (is<Real>(temp2)) {
						tempResult = make<Boolean>(value_of<Integer>(temp1).convert_to<real_type>() != value_of<Real>(temp2));
					}
					else if (is<Integer>(temp2)) {
						tempResult = make<Boolean>(value_of<Integer>(temp1) != value_of<Integer>(temp2));
					}
					else throw runtime_error("Error: Invalid Operand Type");
				}
				else if (is<Boolean>(temp1) && is<Boolean>(temp2)) {
					tempResult = make<Boolean>(value_of<Boolean>(temp1) != value_of<Boolean>(temp2));
				}
				else if (is<Real>(temp1)) {
					if (is<Real>(temp2)) {
						tempResult = make<Boolean>(value_of<Real>(temp1) != value_of<Real>(temp2));
					}
					else if (is<Integer>(temp2)) {
						tempResult = make<Boolean>(value_of<Real>(temp1) != value_of<Integer>(temp2).convert_to<real_type>());
					}
					else throw runtime_error("Error: Invalid Operand Type");
				}
				else if (is<Variable>(temp1)) {
					Operand::pointer_type tempVar = convert<Variable>(temp1)->value();
					if (tempVar == nullptr)
						throw runtime_error("Error: variable not initialized");
					if (is<Integer>(tempVar)) {
						if (is<Real>(temp2)) {
							tempResult = make<Boolean>(value_of<Integer>(tempVar).convert_to<real_type>() != value_of<Real>(temp2));
						}
						else if (is<Integer>(temp2)) {
							tempResult = make<Boolean>(value_of<Integer>(tempVar) != value_of<Integer>(temp2));
						}
						else throw runtime_error("Error: Invalid Operand Type");
					}
					else if (is<Boolean>(tempVar) && is<Boolean>(temp2)) {
						tempResult = make<Boolean>(value_of<Boolean>(tempVar) != value_of<Boolean>(temp2));
					}
					else if (is<Real>(tempVar)) {
						if (is<Real>(temp2)) {
							tempResult = make<Boolean>(value_of<Real>(tempVar) != value_of<Real>(temp2));
						}
						else if (is<Integer>(temp2)) {
							tempResult = make<Boolean>(value_of<Real>(tempVar) != value_of<Integer>(temp2).convert_to<real_type>());
						}
						else throw runtime_error("Error: Invalid Operand Type");
					}
					else throw runtime_error("Error: Invalid Operand Type");
				}
				else throw runtime_error("Error: Invalid Operand Type");
			}
			else if (tkn->str() == "<Less>") {
				if (is<Integer>(temp1)) {
					if (is<Real>(temp2)) {
						tempResult = make<Boolean>(value_of<Integer>(temp1).convert_to<real_type>() > value_of<Real>(temp2));
					}
					else if (is<Integer>(temp2)) {
						tempResult = make<Boolean>(value_of<Integer>(temp1) > value_of<Integer>(temp2));
					}
					else throw runtime_error("Error: Invalid Operand Type");
				}
				else if (is<Boolean>(temp1) && is<Boolean>(temp2)) {
					tempResult = make<Boolean>(value_of<Boolean>(temp1) > value_of<Boolean>(temp2));
				}
				else if (is<Real>(temp1)) {
					if (is<Real>(temp2)) {
						tempResult = make<Boolean>(value_of<Real>(temp1) > value_of<Real>(temp2));
					}
					else if (is<Integer>(temp2)) {
						tempResult = make<Boolean>(value_of<Real>(temp1) > value_of<Integer>(temp2).convert_to<real_type>());
					}
					else throw runtime_error("Error: Invalid Operand Type");
				}
				else if (is<Variable>(temp1)) {
					Operand::pointer_type tempVar = convert<Variable>(temp1)->value();
					if (tempVar == nullptr)
						throw runtime_error("Error: variable not initialized");
					if (is<Integer>(tempVar)) {
						if (is<Real>(temp2))
							tempResult = make<Boolean>(value_of<Integer>(tempVar).convert_to<real_type>() > value_of<Real>(temp2));
						else if (is<Integer>(temp2))
							tempResult = make<Boolean>(value_of<Integer>(tempVar) > value_of<Integer>(temp2));
						else throw runtime_error("Error: Invalid Operand Type");
					}
					else if (is<Boolean>(tempVar) && is<Boolean>(temp2))
						tempResult = make<Boolean>(value_of<Boolean>(tempVar) > value_of<Boolean>(temp2));
					else if (is<Real>(tempVar)) {
						if (is<Real>(temp2))
							tempResult = make<Boolean>(value_of<Real>(tempVar) > value_of<Real>(temp2));
						else if (is<Integer>(temp2))
							tempResult = make<Boolean>(value_of<Real>(tempVar) > value_of<Integer>(temp2).convert_to<real_type>());
						else throw runtime_error("Error: Invalid Operand Type");
					}
					else throw runtime_error("Error: Invalid Operand Type");
				}
				else throw runtime_error("Error: Invalid Operand Type");
			}
			else if (tkn->str() == "<LessEqual>") {
				if (is<Integer>(temp1)) {
					if (is<Real>(temp2)) {
						tempResult = make<Boolean>(value_of<Integer>(temp1).convert_to<real_type>() >= value_of<Real>(temp2));
					}
					else if (is<Integer>(temp2)) {
						tempResult = make<Boolean>(value_of<Integer>(temp1) >= value_of<Integer>(temp2));
					}
					else throw runtime_error("Error: Invalid Operand Type");
				}
				else if (is<Boolean>(temp1) && is<Boolean>(temp2)) {
					tempResult = make<Boolean>(value_of<Boolean>(temp1) >= value_of<Boolean>(temp2));
				}
				else if (is<Real>(temp1)) {
					if (is<Real>(temp2)) {
						tempResult = make<Boolean>(value_of<Real>(temp1) >= value_of<Real>(temp2));
					}
					else if (is<Integer>(temp2)) {
						tempResult = make<Boolean>(value_of<Real>(temp1) >= value_of<Integer>(temp2).convert_to<real_type>());
					}
					else throw runtime_error("Error: Invalid Operand Type");
				}
				else if (is<Variable>(temp1)) {
					Operand::pointer_type tempVar = convert<Variable>(temp1)->value();
					if (tempVar == nullptr)
						throw runtime_error("Error: variable not initialized");
					if (is<Integer>(tempVar)) {
						if (is<Real>(temp2))
							tempResult = make<Boolean>(value_of<Integer>(tempVar).convert_to<real_type>() >= value_of<Real>(temp2));
						else if (is<Integer>(temp2))
							tempResult = make<Boolean>(value_of<Integer>(tempVar) >= value_of<Integer>(temp2));
						else throw runtime_error("Error: Invalid Operand Type");
					}
					else if (is<Boolean>(tempVar) && is<Boolean>(temp2))
						tempResult = make<Boolean>(value_of<Boolean>(tempVar) >= value_of<Boolean>(temp2));
					else if (is<Real>(tempVar)) {
						if (is<Real>(temp2))
							tempResult = make<Boolean>(value_of<Real>(tempVar) >= value_of<Real>(temp2));
						else if (is<Integer>(temp2))
							tempResult = make<Boolean>(value_of<Real>(tempVar) >= value_of<Integer>(temp2).convert_to<real_type>());
						else throw runtime_error("Error: Invalid Operand Type");
					}
					else throw runtime_error("Error: Invalid Operand Type");
				}
				else throw runtime_error("Error: Invalid Operand Type");
			}
			else if (tkn->str() == "<Multiplication>") {
				if (is<Integer>(temp1)) {
					if (is<Real>(temp2)) {
						tempResult = make<Real>(value_of<Integer>(temp1).convert_to<real_type>() * value_of<Real>(temp2));
					}
					else if (is<Integer>(temp2)) {
						tempResult = make<Integer>(value_of<Integer>(temp1) * value_of<Integer>(temp2));
					}
					else if (is<Variable>(temp2)) {
						if (is<Integer>(value_of<Variable>(temp2)))
							tempResult = make<Integer>(value_of<Integer>(value_of<Variable>(temp2))*value_of<Integer>(temp1));
						else if (is<Real>(value_of<Variable>(temp2)))
							tempResult = make<Real>(value_of<Real>(value_of<Variable>(temp2))* value_of<Integer>(temp1).convert_to<real_type>());
					}
					else throw runtime_error("Error: Invalid Operand Type");
				}
				else if (is<Real>(temp1)) {
					if (is<Real>(temp2)) {
						tempResult = make<Real>(value_of<Real>(temp1) * value_of<Real>(temp2));
					}
					else if (is<Integer>(temp2)) {
						tempResult = make<Real>(value_of<Real>(temp1) * value_of<Integer>(temp2).convert_to<real_type>());
					}
					else throw runtime_error("Error: Invalid Operand Type");
				}
				else if (is<Variable>(temp1)) {
					Operand::pointer_type tempVar = convert<Variable>(temp1)->value();
					if (tempVar == nullptr)
						throw runtime_error("Error: variable not initialized");
					if (is<Integer>(tempVar)) {
						if (is<Real>(temp2)) {
							tempResult = make<Real>(value_of<Integer>(tempVar).convert_to<real_type>() * value_of<Real>(temp2));
						}
						else if (is<Integer>(temp2)) {
							tempResult = make<Integer>(value_of<Integer>(tempVar) * value_of<Integer>(temp2));
						}
						else throw runtime_error("Error: Invalid Operand Type");
					}
					else if (is<Real>(tempVar)) {
						if (is<Real>(temp2)) {
							tempResult = make<Real>(value_of<Real>(tempVar) * value_of<Real>(temp2));
						}
						else if (is<Integer>(temp2)) {
							tempResult = make<Real>(value_of<Real>(tempVar) * value_of<Integer>(temp2).convert_to<real_type>());
						}
						else throw runtime_error("Error: Invalid Operand Type");
					}
					else throw runtime_error("Error: Invalid Operand Type");
				}
				else throw runtime_error("Error: Invalid Operand Type");
			}
			else if (tkn->str() == "<Modulus>") {
				if (is<Integer>(temp1)) {
					if (is<Integer>(temp2)) {
						tempResult = make<Integer>(value_of<Integer>(temp2) % value_of<Integer>(temp1));
					}
					else throw runtime_error("Error: Invalid Operand Type");
				}
				else if (is<Variable>(temp1)) {
					Operand::pointer_type tempVar = convert<Variable>(temp1)->value();
					if (tempVar == nullptr)
						throw runtime_error("Error: variable not initialized");
					if (is<Integer>(tempVar)) {
						if (is<Integer>(temp2)) {
							tempResult = make<Integer>(value_of<Integer>(temp2) % value_of<Integer>(tempVar));
						}
						else throw runtime_error("Error: Invalid Operand Type");
					}
					else throw runtime_error("Error: Invalid Operand Type");
				}
				else throw runtime_error("Error: Invalid Operand Type");
			}
			else if (tkn->str() == "<Nand>") {
				if (is<Boolean>(temp1) && is<Boolean>(temp2)) {
					tempResult = make<Boolean>(!(value_of<Boolean>(temp1) && value_of<Boolean>(temp2)));
				}
				else if (is<Variable>(temp1)) {
					Operand::pointer_type tempVar = convert<Variable>(temp1)->value();
					if (tempVar == nullptr)
						throw runtime_error("Error: variable not initialized");
					if (is<Boolean>(tempVar) && is<Boolean>(temp2))
						tempResult = make<Boolean>(!(value_of<Boolean>(tempVar) && value_of<Boolean>(temp2)));
					else throw runtime_error("Error: Invalid Operand Type");
				}
				else throw runtime_error("Error: Invalid Operand Type");
			}
			else if (tkn->str() == "<Nor>") {
				if (is<Boolean>(temp1) && is<Boolean>(temp2)) {
					tempResult = make<Boolean>(!(value_of<Boolean>(temp1) || value_of<Boolean>(temp2)));
				}
				else if (is<Variable>(temp1)) {
					Operand::pointer_type tempVar = convert<Variable>(temp1)->value();
					if (tempVar == nullptr)
						throw runtime_error("Error: variable not initialized");
					if (is<Boolean>(tempVar) && is<Boolean>(temp2))
						tempResult = make<Boolean>(!(value_of<Boolean>(tempVar) || value_of<Boolean>(temp2)));
					else throw runtime_error("Error: Invalid Operand Type");
				}
				else throw runtime_error("Error: Invalid Operand Type");
			}
			else if (tkn->str() == "<Or>") {
				if (is<Boolean>(temp1) && is<Boolean>(temp2)) {
					tempResult = make<Boolean>(value_of<Boolean>(temp1) || value_of<Boolean>(temp2));
				}
				else if (is<Variable>(temp1)) {
					Operand::pointer_type tempVar = convert<Variable>(temp1)->value();
					if (tempVar == nullptr)
						throw runtime_error("Error: variable not initialized");
					if (is<Boolean>(tempVar) && is<Boolean>(temp2))
						tempResult = make<Boolean>(value_of<Boolean>(tempVar) || value_of<Boolean>(temp2));
					else throw runtime_error("Error: Invalid Operand Type");
				}
				else throw runtime_error("Error: Invalid Operand Type");
			}
			else if (tkn->str() == "<Subtraction>") {
				if (is<Integer>(temp1)) {
					if (is<Real>(temp2)) {
						tempResult = make<Real>(value_of<Real>(temp2) - value_of<Integer>(temp1).convert_to<real_type>());
					}
					else if (is<Integer>(temp2)) {
						tempResult = make<Integer>(value_of<Integer>(temp2) - value_of<Integer>(temp1));
					}
					else if (is<Variable>(temp2)) {
						if (is<Integer>(value_of<Variable>(temp2)))
							tempResult = make<Integer>(value_of<Integer>(value_of<Variable>(temp2)) - value_of<Integer>(temp1));
						else if (is<Real>(value_of<Variable>(temp2)))
							tempResult = make<Real>(value_of<Real>(value_of<Variable>(temp2)) - value_of<Integer>(temp1).convert_to<real_type>());
					}
					else throw runtime_error("Error: Invalid Operand Type");
				}
				else if (is<Real>(temp1)) {
					if (is<Real>(temp2)) {
						tempResult = make<Real>(value_of<Real>(temp2) - value_of<Real>(temp1));
					}
					else if (is<Integer>(temp2)) {
						tempResult = make<Real>(value_of<Integer>(temp2).convert_to<real_type>() - value_of<Real>(temp1));
					}
					else throw runtime_error("Error: Invalid Operand Type");
				}
				else if (is<Variable>(temp1)) {
					Operand::pointer_type tempVar = convert<Variable>(temp1)->value();
					if (tempVar == nullptr)
						throw runtime_error("Error: variable not initialized");
					if (is<Integer>(tempVar)) {
						if (is<Real>(temp2)) {
							tempResult = make<Real>(value_of<Real>(temp2) - value_of<Integer>(tempVar).convert_to<real_type>());
						}
						else if (is<Integer>(temp2)) {
							tempResult = make<Integer>(value_of<Integer>(temp2) - value_of<Integer>(tempVar));
						}
						else throw runtime_error("Error: Invalid Operand Type");
					}
					else if (is<Real>(tempVar)) {
						if (is<Real>(temp2)) {
							tempResult = make<Real>(value_of<Real>(temp2) - value_of<Real>(tempVar));
						}
						else if (is<Integer>(temp2)) {
							tempResult = make<Real>(value_of<Integer>(temp2).convert_to<real_type>() - value_of<Real>(tempVar));
						}
						else throw runtime_error("Error: Invalid Operand Type");
					}
					else throw runtime_error("Error: Invalid Operand Type");
				}
				else throw runtime_error("Error: Invalid Operand Type");
			}
			else if (tkn->str() == "<Xor>") {
				if (is<Boolean>(temp1) && is<Boolean>(temp2)) {
					tempResult = make<Boolean>(value_of<Boolean>(temp1) ^ value_of<Boolean>(temp2));
				}
				else if (is<Variable>(temp1)) {
					Operand::pointer_type tempVar = convert<Variable>(temp1)->value();
					if (tempVar == nullptr)
						throw runtime_error("Error: variable not initialized");
					if (is<Boolean>(tempVar) && is<Boolean>(temp2))
						tempResult = make<Boolean>(value_of<Boolean>(tempVar) ^ value_of<Boolean>(temp2));
					else throw runtime_error("Error: Invalid Operand Type");
				}
				else throw runtime_error("Error: Invalid Operand Type");
			}
			else if (tkn->str() == "<Xnor>") {
				if (is<Boolean>(temp1) && is<Boolean>(temp2)) {
					tempResult = make<Boolean>(!(value_of<Boolean>(temp1) ^ value_of<Boolean>(temp2)));
				}
				else if (is<Variable>(temp1)) {
					Operand::pointer_type tempVar = convert<Variable>(temp1)->value();
					if (tempVar == nullptr)
						throw runtime_error("Error: variable not initialized");
					if (is<Boolean>(tempVar) && is<Boolean>(temp2))
						tempResult = make<Boolean>(!(value_of<Boolean>(tempVar) ^ value_of<Boolean>(temp2)));
					else throw runtime_error("Error: Invalid Operand Type");
				}
				else throw runtime_error("Error: Invalid Operand Type");
			}
			//NonAssociative
			else if (tkn->str() == "<Identity>") {
				if (is<Integer>(temp1))
					tempResult = make<Integer>(value_of<Integer>(temp1).convert_to<int_type>());
				else if (is<Real>(temp1))
					tempResult = make<Real>(value_of<Real>(temp1).convert_to<real_type>());
				else throw runtime_error("Error: Invalid Operand Type");
			}
			else if (tkn->str() == "<Negation>") {
				if (is<Integer>(temp1))
					tempResult = make<Integer>(value_of<Integer>(temp1) * -1);
				else if (is<Real>(temp1))
					tempResult = make<Real>(value_of<Real>(temp1) * -1);
				else if (is<Variable>(temp1)) {
					Operand::pointer_type tempVar = convert<Variable>(temp1)->value();
					if (tempVar == nullptr)
						throw runtime_error("Error: variable not initialized");
					if (is<Integer>(tempVar))
						tempResult = make<Integer>(convert<Integer>(tempVar)->value() * -1);
					else if (is<Real>(tempVar))
						tempResult = make<Real>(convert<Real>(tempVar)->value() * -1);
					else throw runtime_error("Error: Invalid Operand Type");
				}
				else throw runtime_error("Error: Invalid Operand Type");
			}
			else if (tkn->str() == "<Not>") {
				if (is<Boolean>(temp1))
					tempResult = make<Boolean>(!value_of<Boolean>(temp1));
				else if (is<Variable>(temp1)) {
					Operand::pointer_type tempVar = convert<Variable>(temp1)->value();
					if (tempVar == nullptr)
						throw runtime_error("Error: variable not initialized");
					if (is<Boolean>(tempVar))
						tempResult = make<Boolean>(!convert<Boolean>(tempVar)->value());
					else throw runtime_error("Error: Invalid Operand Type");
				}
				else throw runtime_error("Error: Invalid Operand Type");
			}
			else if (tkn->str() == "<Factorial>") {
				if (is<Integer>(temp1))
					tempResult = make<Integer>(boost::math::unchecked_factorial<int_type>(value_of<Integer>(temp1).convert_to<unsigned>()));
				else if (is<Variable>(temp1)) {
					Operand::pointer_type tempVar = convert<Variable>(temp1)->value();
					if (tempVar == nullptr)
						throw runtime_error("Error: variable not initialized");
					if (is<Integer>(tempVar))
						tempResult = make<Integer>(boost::math::unchecked_factorial<int_type>(convert<Integer>(tempVar)->value().convert_to<unsigned>()));
					else throw runtime_error("Error: Invalid Operand Type");
				}
				else throw runtime_error("Error: Invalid Operand Type");
			}
			//<Function>
			//OneArgFunction
			else if (tkn->str() == "<Abs>") {
				if (is<Integer>(temp1))
					tempResult = make<Integer>(boost::multiprecision::abs(value_of<Integer>(temp1)));
				else if (is<Real>(temp1))
					tempResult = make<Real>(boost::multiprecision::abs(value_of<Real>(temp1)));
				else if (is<Variable>(temp1)) {
					Operand::pointer_type tempVar = convert<Variable>(temp1)->value();
					if (tempVar == nullptr)
						throw runtime_error("Error: variable not initialized");
					if (is<Integer>(tempVar))
						tempResult = make<Integer>(boost::multiprecision::abs(convert<Integer>(tempVar)->value()));
					else if (is<Real>(tempVar))
						tempResult = make<Real>(boost::multiprecision::abs(convert<Real>(tempVar)->value()));
					else throw runtime_error("Error: Invalid Operand Type");
				}
				else throw runtime_error("Error: Invalid Operand Type");
			}
			else if (tkn->str() == "<Arccos>") {
				if (is<Real>(temp1))
					tempResult = make<Real>(boost::multiprecision::acos(value_of<Real>(temp1)));
				else if (is<Variable>(temp1)) {
					Operand::pointer_type tempVar = convert<Variable>(temp1)->value();
					if (tempVar == nullptr)
						throw runtime_error("Error: variable not initialized");
					if (is<Real>(tempVar))
						tempResult = make<Real>(boost::multiprecision::acos(convert<Real>(tempVar)->value()));
					else throw runtime_error("Error: Invalid Operand Type");
				}
				else throw runtime_error("Error: Invalid Operand Type");
			}
			else if (tkn->str() == "<Arcsin>") {
				if (is<Real>(temp1))
					tempResult = make<Real>(boost::multiprecision::asin(value_of<Real>(temp1)));
				else if (is<Variable>(temp1)) {
					Operand::pointer_type tempVar = convert<Variable>(temp1)->value();
					if (tempVar == nullptr)
						throw runtime_error("Error: variable not initialized");
					if (is<Real>(tempVar))
						tempResult = make<Real>(boost::multiprecision::asin(value_of<Real>(tempVar)));
					else throw runtime_error("Error: Invalid Operand Type");
				}
				else throw runtime_error("Error: Invalid Operand Type");
			}
			else if (tkn->str() == "<Arctan>") {
				if (is<Real>(temp1))
					tempResult = make<Real>(boost::multiprecision::atan(value_of<Real>(temp1)));
				else if (is<Variable>(temp1)) {
					Operand::pointer_type tempVar = convert<Variable>(temp1)->value();
					if (tempVar == nullptr)
						throw runtime_error("Error: variable not initialized");
					if (is<Real>(tempVar))
						tempResult = make<Real>(boost::multiprecision::atan(value_of<Real>(tempVar)));
					else throw runtime_error("Error: Invalid Operand Type");
				}
				else throw runtime_error("Error: Invalid Operand Type");
			}
			else if (tkn->str() == "<Ceil>") {
				if (is<Real>(temp1))
					tempResult = make<Real>(boost::multiprecision::ceil(value_of<Real>(temp1)));
				else if (is<Integer>(temp1))
					tempResult = make<Integer>(value_of<Integer>(temp1));
				else if (is<Variable>(temp1)) {
					Operand::pointer_type tempVar = convert<Variable>(temp1)->value();
					if (tempVar == nullptr)
						throw runtime_error("Error: variable not initialized");
					if (is<Real>(tempVar))
						tempResult = make<Real>(boost::multiprecision::ceil(value_of<Real>(tempVar)));
					else throw runtime_error("Error: Invalid Operand Type");
				}
				else throw runtime_error("Error: Invalid Operand Type");
			}
			else if (tkn->str() == "<Cos>") {
				if (is<Real>(temp1))
					tempResult = make<Real>(boost::multiprecision::cos(value_of<Real>(temp1)));
				else if (is<Integer>(temp1))
					tempResult = make<Integer>(boost::multiprecision::cos(value_of<Integer>(temp1).convert_to<real_type>()).convert_to<int_type>());
				else if (is<Variable>(temp1)) {
					Operand::pointer_type tempVar = convert<Variable>(temp1)->value();
					if (tempVar == nullptr)
						throw runtime_error("Error: variable not initialized");
					if (is<Real>(tempVar))
						tempResult = make<Real>(boost::multiprecision::cos(value_of<Real>(tempVar)));
					else throw runtime_error("Error: Invalid Operand Type");
				}
				else throw runtime_error("Error: Invalid Operand Type");
			}
			else if (tkn->str() == "<Exp>") {
				if (is<Real>(temp1))
					tempResult = make<Real>(boost::multiprecision::exp(value_of<Real>(temp1)));
				else if (is<Integer>(temp1))
					tempResult = make<Real>(boost::multiprecision::exp(value_of<Integer>(temp1).convert_to<real_type>()));
				else if (is<Variable>(temp1)) {
					Operand::pointer_type tempVar = convert<Variable>(temp1)->value();
					if (tempVar == nullptr)
						throw runtime_error("Error: variable not initialized");
					if (is<Real>(tempVar))
						tempResult = make<Real>(boost::multiprecision::exp(value_of<Real>(tempVar)));
					else throw runtime_error("Error: Invalid Operand Type");
				}
				else throw runtime_error("Error: Invalid Operand Type");
			}
			else if (tkn->str() == "<Floor>") {
				if (is<Real>(temp1))
					tempResult = make<Real>(boost::multiprecision::floor(value_of<Real>(temp1)));
				else if (is<Integer>(temp1))
					tempResult = make<Integer>(value_of<Integer>(temp1));
				else if (is<Variable>(temp1)) {
					Operand::pointer_type tempVar = convert<Variable>(temp1)->value();
					if (tempVar == nullptr)
						throw runtime_error("Error: variable not initialized");
					if (is<Real>(tempVar))
						tempResult = make<Real>(boost::multiprecision::floor(value_of<Real>(tempVar)));
					else throw runtime_error("Error: Invalid Operand Type");
				}
				else throw runtime_error("Error: Invalid Operand Type");
			}
			else if (tkn->str() == "<Lb>") {
				if (is<Real>(temp1))
					tempResult = make<Real>(boost::multiprecision::log2(value_of<Real>(temp1)));
				else if (is<Integer>(temp1))
					tempResult = make<Real>(boost::multiprecision::log2(value_of<Integer>(temp1).convert_to<real_type>()));
				else if (is<Variable>(temp1)) {
					Operand::pointer_type tempVar = convert<Variable>(temp1)->value();
					if (tempVar == nullptr)
						throw runtime_error("Error: variable not initialized");
					if (is<Real>(tempVar))
						tempResult = make<Real>(boost::multiprecision::log2(value_of<Real>(tempVar)));
					else throw runtime_error("Error: Invalid Operand Type");
				}
				else throw runtime_error("Error: Invalid Operand Type");
			}
			else if (tkn->str() == "<Ln>") {
				if (is<Real>(temp1))
					tempResult = make<Real>(boost::multiprecision::log10(value_of<Real>(temp1)));
				else if (is<Integer>(temp1))
					tempResult = make<Real>(boost::multiprecision::log10(value_of<Integer>(temp1).convert_to<real_type>()));
				else if (is<Variable>(temp1)) {
					Operand::pointer_type tempVar = convert<Variable>(temp1)->value();
					if (tempVar == nullptr)
						throw runtime_error("Error: variable not initialized");
					if (is<Real>(tempVar))
						tempResult = make<Real>(boost::multiprecision::log10(value_of<Real>(tempVar)));
					else throw runtime_error("Error: Invalid Operand Type");
				}
				else throw runtime_error("Error: Invalid Operand Type");
			}
			else if (tkn->str() == "<Log>") {
				if (is<Real>(temp1))
					tempResult = make<Real>(boost::multiprecision::log(value_of<Real>(temp1)));
				else if (is<Integer>(temp1))
					tempResult = make<Real>(boost::multiprecision::log(value_of<Integer>(temp1).convert_to<real_type>()));
				else if (is<Variable>(temp1)) {
					Operand::pointer_type tempVar = convert<Variable>(temp1)->value();
					if (tempVar == nullptr)
						throw runtime_error("Error: variable not initialized");
					if (is<Real>(tempVar))
						tempResult = make<Real>(boost::multiprecision::log(value_of<Real>(tempVar)));
					else throw runtime_error("Error: Invalid Operand Type");
				}
				else throw runtime_error("Error: Invalid Operand Type");
			}
			else if (tkn->str() == "<Result>");
			else if (tkn->str() == "<Sin>") {
				if (is<Real>(temp1))
					tempResult = make<Real>(boost::multiprecision::sin(value_of<Real>(temp1)));
				else if (is<Integer>(temp1))
					tempResult = make<Real>(boost::multiprecision::sin(value_of<Integer>(temp1).convert_to<real_type>()));
				else if (is<Variable>(temp1)) {
					Operand::pointer_type tempVar = convert<Variable>(temp1)->value();
					if (tempVar == nullptr)
						throw runtime_error("Error: variable not initialized");
					if (is<Real>(tempVar))
						tempResult = make<Real>(boost::multiprecision::sin(value_of<Real>(tempVar)));
					else throw runtime_error("Error: Invalid Operand Type");
				}
				else throw runtime_error("Error: Invalid Operand Type");
			}
			else if (tkn->str() == "<Sqrt>") {
				if (is<Real>(temp1))
					tempResult = make<Real>(boost::multiprecision::sqrt(value_of<Real>(temp1)));
				else if (is<Integer>(temp1))
					tempResult = make<Real>(boost::multiprecision::sqrt(value_of<Integer>(temp1).convert_to<real_type>()));
				else if (is<Variable>(temp1)) {
					Operand::pointer_type tempVar = convert<Variable>(temp1)->value();
					if (tempVar == nullptr)
						throw runtime_error("Error: variable not initialized");
					if (is<Real>(tempVar))
						tempResult = make<Real>(boost::multiprecision::sqrt(value_of<Real>(temp1)));
					else throw runtime_error("Error: Invalid Operand Type");
				}
				else throw runtime_error("Error: Invalid Operand Type");
			}
			else if (tkn->str() == "<Tan>") {
				if (is<Real>(temp1))
					tempResult = make<Real>(boost::multiprecision::tan(value_of<Real>(temp1)));
				else if (is<Integer>(temp1))
					tempResult = make<Real>(boost::multiprecision::tan(value_of<Integer>(temp1).convert_to<real_type>()));
				else if (is<Variable>(temp1)) {
					Operand::pointer_type tempVar = convert<Variable>(temp1)->value();
					if (tempVar == nullptr)
						throw runtime_error("Error: variable not initialized");
					if (is<Real>(tempVar))
						tempResult = make<Real>(boost::multiprecision::tan(value_of<Real>(tempVar)));
					else throw runtime_error("Error: Invalid Operand Type");
				}
				else throw runtime_error("Error: Invalid Operand Type");
			}
			//TwoArgFunction
			else if (tkn->str() == "<Arctan2>") {
				if (is<Real>(temp1)) {
					if (is<Real>(temp2)) {
						tempResult = make<Real>(boost::multiprecision::atan2(value_of<Real>(temp2), value_of<Real>(temp1)));
					}
					else if (is<Integer>(temp2)) {
						tempResult = make<Real>(boost::multiprecision::atan2(value_of<Integer>(temp2).convert_to<real_type>(), value_of<Real>(temp1)));
					}
					else throw runtime_error("Error: Invalid Operand Type");
				}
				else if (is<Integer>(temp1)) {
					if (is<Real>(temp2)) {
						tempResult = make<Real>(boost::multiprecision::atan2(value_of<Integer>(temp2).convert_to<real_type>(), value_of<Real>(temp1)));
					}
					else throw runtime_error("Error: Invalid Operand Type");
				}
				else if (is<Variable>(temp1)) {
					Operand::pointer_type tempVar = convert<Variable>(temp1)->value();
					if (tempVar == nullptr)
						throw runtime_error("Error: variable not initialized");
					if (is<Real>(tempVar)) {
						if (is<Real>(temp2)) {
							tempResult = make<Real>(boost::multiprecision::atan2(value_of<Real>(temp2), value_of<Real>(tempVar)));
						}
						else if (is<Integer>(temp2)) {
							tempResult = make<Real>(boost::multiprecision::atan2(value_of<Integer>(temp2).convert_to<real_type>(), value_of<Real>(tempVar)));
						}
						else throw runtime_error("Error: Invalid Operand Type");
					}
					else throw runtime_error("Error: Invalid Operand Type");
				}
				else throw runtime_error("Error: Invalid Operand Type");
			}
			else if (tkn->str() == "<Max>") {
				if (is<Integer>(temp1)) {
					if (is<Real>(temp2)) {
						tempResult = make<Real>(boost::multiprecision::max(value_of<Integer>(temp1).convert_to<real_type>(), value_of<Real>(temp2)));
					}
					else if (is<Integer>(temp2)) {
						tempResult = make<Integer>(boost::multiprecision::max(value_of<Integer>(temp1), value_of<Integer>(temp2)));
					}
					else throw runtime_error("Error: Invalid Operand Type");
				}
				else if (is<Real>(temp1)) {
					if (is<Real>(temp2)) {
						tempResult = make<Real>(boost::multiprecision::max(value_of<Real>(temp1), value_of<Real>(temp2)));
					}
					else if (is<Integer>(temp2)) {
						tempResult = make<Real>(boost::multiprecision::max(value_of<Real>(temp1), value_of<Integer>(temp2).convert_to<real_type>()));
					}
					else throw runtime_error("Error: Invalid Operand Type");
				}
				else if (is<Variable>(temp1)) {
					Operand::pointer_type tempVar = convert<Variable>(temp1)->value();
					if (tempVar == nullptr)
						throw runtime_error("Error: variable not initialized");
					if (is<Integer>(tempVar)) {
						if (is<Real>(temp2)) {
							tempResult = make<Real>(boost::multiprecision::max(value_of<Integer>(tempVar).convert_to<real_type>(), value_of<Real>(temp2)));
						}
						else if (is<Integer>(temp2)) {
							tempResult = make<Integer>(boost::multiprecision::max(value_of<Integer>(tempVar), value_of<Integer>(temp2)));
						}
						else throw runtime_error("Error: Invalid Operand Type");
					}
					else if (is<Real>(tempVar)) {
						if (is<Real>(temp2)) {
							tempResult = make<Real>(boost::multiprecision::max(value_of<Real>(tempVar), value_of<Real>(temp2)));
						}
						else if (is<Integer>(temp2)) {
							tempResult = make<Real>(boost::multiprecision::max(value_of<Real>(tempVar), value_of<Integer>(temp2).convert_to<real_type>()));
						}
						else throw runtime_error("Error: Invalid Operand Type");
					}
					else throw runtime_error("Error: Invalid Operand Type");
				}
				else throw runtime_error("Error: Invalid Operand Type");
			}
			else if (tkn->str() == "<Min>") {
				if (is<Integer>(temp1)) {
					if (is<Real>(temp2)) {
						tempResult = make<Real>(boost::multiprecision::min(value_of<Integer>(temp1).convert_to<real_type>(), value_of<Real>(temp2)));
					}
					else if (is<Integer>(temp2)) {
						tempResult = make<Integer>(boost::multiprecision::min(value_of<Integer>(temp1), value_of<Integer>(temp2)));
					}
					else throw runtime_error("Error: Invalid Operand Type");
				}
				else if (is<Real>(temp1)) {
					if (is<Real>(temp2)) {
						tempResult = make<Real>(boost::multiprecision::min(value_of<Real>(temp1), value_of<Real>(temp2)));
					}
					else if (is<Integer>(temp2)) {
						tempResult = make<Real>(boost::multiprecision::min(value_of<Real>(temp1), value_of<Integer>(temp2).convert_to<real_type>()));
					}
					else throw runtime_error("Error: Invalid Operand Type");
				}
				else if (is<Variable>(temp1)) {
					Operand::pointer_type tempVar = convert<Variable>(temp1)->value();
					if (tempVar == nullptr)
						throw runtime_error("Error: variable not initialized");
					if (is<Integer>(tempVar)) {
						if (is<Real>(temp2)) {
							tempResult = make<Real>(boost::multiprecision::min(value_of<Integer>(tempVar).convert_to<real_type>(), value_of<Real>(temp2)));
						}
						else if (is<Integer>(temp2)) {
							tempResult = make<Integer>(boost::multiprecision::min(value_of<Integer>(tempVar), value_of<Integer>(temp2)));
						}
						else throw runtime_error("Error: Invalid Operand Type");
					}
					else if (is<Real>(tempVar)) {
						if (is<Real>(temp2)) {
							tempResult = make<Real>(boost::multiprecision::min(value_of<Real>(tempVar), value_of<Real>(temp2)));
						}
						else if (is<Integer>(temp2)) {
							tempResult = make<Real>(boost::multiprecision::min(value_of<Real>(tempVar), value_of<Integer>(temp2).convert_to<real_type>()));
						}
						else throw runtime_error("Error: Invalid Operand Type");
					}
					else throw runtime_error("Error: Invalid Operand Type");
				}
				else throw runtime_error("Error: Invalid Operand Type");
			}
			else if (tkn->str() == "<Pow>") {
				if (is<Integer>(temp1)) {
					if (is<Real>(temp2)) {
						tempResult = make<Real>(boost::multiprecision::pow(value_of<Real>(temp2), value_of<Integer>(temp1).convert_to<real_type>()));
					}
					else if (is<Integer>(temp2)) {
						tempResult = make<Integer>(boost::multiprecision::pow(value_of<Integer>(temp2).convert_to<real_type>(), value_of<Integer>(temp1).convert_to<real_type>()).convert_to<int_type>());
					}
					else throw runtime_error("Error: Invalid Operand Type");
				}
				else if (is<Real>(temp1)) {
					if (is<Real>(temp2)) {
						tempResult = make<Real>(boost::multiprecision::pow(value_of<Real>(temp2), value_of<Real>(temp1)));
					}
					else if (is<Integer>(temp2)) {
						tempResult = make<Real>(boost::multiprecision::pow(value_of<Integer>(temp2).convert_to<real_type>(), value_of<Real>(temp1)));
					}
					else throw runtime_error("Error: Invalid Operand Type");
				}
				else if (is<Variable>(temp1)) {
					Operand::pointer_type tempVar = convert<Variable>(temp1)->value();
					if (tempVar == nullptr)
						throw runtime_error("Error: variable not initialized");
					if (is<Integer>(tempVar)) {
						if (is<Real>(temp2)) {
							tempResult = make<Real>(boost::multiprecision::pow(value_of<Real>(temp2), value_of<Integer>(tempVar).convert_to<real_type>()));
						}
						else if (is<Integer>(temp2)) {
							tempResult = make<Integer>(boost::multiprecision::pow(value_of<Integer>(temp2).convert_to<real_type>(), value_of<Integer>(tempVar).convert_to<real_type>()).convert_to<int_type>());
						}
						else throw runtime_error("Error: Invalid Operand Type");
					}
					else if (is<Real>(tempVar)) {
						if (is<Real>(temp2)) {
							tempResult = make<Real>(boost::multiprecision::pow(value_of<Real>(temp2), value_of<Real>(tempVar)));
						}
						else if (is<Integer>(temp2)) {
							tempResult = make<Real>(boost::multiprecision::pow(value_of<Integer>(temp2).convert_to<real_type>(), value_of<Real>(tempVar)));
						}
						else throw runtime_error("Error: Invalid Operand Type");
					}
					else throw runtime_error("Error: Invalid Operand Type");
				}
				else throw runtime_error("Error: Invalid Operand Type");
			}
			//unknown
			else throw runtime_error("Error: Unknown Token");
			operandS.push(tempResult);// push the result into the operand stack
		}
	}

	// handle the problems which can crash the program
	if (operandS.size() > 1) throw runtime_error("Error: too many operands");
	else if (operandS.empty()) throw runtime_error("Too many operations for the number of operands");

	// convert the result type
	Operand::pointer_type result;
	result = convert<Operand>(operandS.top());
	operandS.pop();
	return result;
}


