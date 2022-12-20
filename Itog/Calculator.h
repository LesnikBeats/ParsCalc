#pragma once
#include <algorithm>
#include <string>
#include <stdlib.h>
#include "Stack.h"
#include "Buffer.h"
#include "Exceptions.h"

using namespace std;

typedef Stack<char> OperationStack;

class Calculator {
private:
	static const int NUMBER_OF_OPS = 5;
	char precedenceTable[NUMBER_OF_OPS][2] = { {'^', '4'}, {'/', '3'}, {'*', '2'}, {'+', '1'}, {'-', '0'} };

public:
	string expression;
	OperationStack operations;
	Buffer operands;

	Calculator(string __expression) : expression(__expression) { parseAndEvaluate(); }
	
	void parseAndEvaluate() {
		string numberString = "";      // Reads each digit in the expression. Flushed to "currentNum" if a whole number has been read
		double currentNum;             // Reads the entire number from "numberString" after its end has been recognized
		char prevChar = ' ';           // Holds the last character read from the expression (for validation purposes)
		bool decimalAllowed = true;    // Switch for determining where decimal points can and can not go
		bool negativesAllowed = true;  // Switch for determining where negative numbers can and can not go


		// Expression validity checks. Exceptions must be handled when calling "parseAndEvaluate()"
		removeWhiteSpace();
		if (!bracesBalanced()) { throw UnbalancedBracesException(); }
		if (!legalStart() || !legalEnding()) { throw InvalidExpressionException(); }


		int expressionLength = expression.length(); // Holds the length of the expression so the length() function doesn't have to be called every time
													// Assigned after "removeWhiteSpace()" because white space can affect length


		for (int i = 0; i < expressionLength; i++) {
			// Handle negative number detection. Detects if '-' means it's a negative number or a subtraction operation
			if (expression[i] == '-' && negativesAllowed) {
				if ((prevChar == ' ' || isOperation(prevChar) || isOpenBrace(prevChar)) && numberString == "") {
					numberString += expression[i];
					negativesAllowed = false;
					continue;
				}
			}

			// Digit detection
			if (isNumeric(expression[i])) {
				numberString += expression[i];
			}
			// Decimal point detection
			else if (isDecimalPoint(expression[i])) {
				if (!decimalAllowed) {
					throw DecimalPointException();
				}
				numberString += expression[i];
				decimalAllowed = false; // Prevents errors that arise from multiple decimal points
			}
			else {
				decimalAllowed = true;
				// End of digit detection (converts "numberString" to double and assigns it to "currentNum", then sets it to an empty string) 
				if (isNumeric(prevChar) || isDecimalPoint(prevChar) || (isOperation(prevChar) && numberString != "")) {
					currentNum = atof(numberString.c_str());
					operands.push(currentNum);
					numberString = "";
				}

				// Open brace detection (push '(' to the operation stack)
				if (isOpenBrace(expression[i])) {
					negativesAllowed = true;
					if (isNumeric(prevChar) || isDecimalPoint(prevChar)) {
						pushOperation('*'); // A number before a brace is considered a multiplication operation
					}
					operations.push('(');
				}
				// Close brace detection (Evaluates the operations inside the braces)
				else if (isCloseBrace(expression[i])) {
					while (!operations.isEmpty() && !isOpenBrace(operations.peek())) { // Evaluate all operations within the braces
						if (isOpenBrace(operations.peek())) { // Removes the open brace once all operations inside it have been evaluated
							operations.pop();
							break;
						}
						eval(operations.pop());
					}
				}
				// Pushes operations to the operation stack based on precedence (pushOperation() handles precedence comparison with help from getPrecedence(<operation>)
				else if (isOperation(expression[i])) {
					if (isOperation(prevChar) && expression[i] != '-') {
						throw InvalidExpressionException();
					}

					negativesAllowed = true;

					if (numberString != "") {
						currentNum = atof(numberString.c_str());
						operands.push(currentNum);
						numberString = "";
					}

					pushOperation(expression[i]);
				}
			}
			prevChar = expression[i]; // Sets the current character to "prevChar". Helps A LOT with number/decimal/brace detection
		}

		// If "numberString" is not empty after the loop ends, pushes the contents (the last number) to the operand buffer
		if (numberString != "") {
			currentNum = atof(numberString.c_str());
			operands.push(currentNum);
			numberString = "";
		}

		// Evaluates every operand on the operand buffer with every operation on the operator stack
		while (!operations.isEmpty()) {
			eval(operations.pop());
		}
	}

	// Removes white space from the expression
	void removeWhiteSpace() {
		expression.erase(remove_if(expression.begin(), expression.end(), isspace), expression.end()); // Code to remove whitespace from stackoverflow
	}

	// Checks if there are any orphaned braces (opened without closing or closed without opening)
	bool bracesBalanced() {
		int openingBraces = 0, closingBraces = 0, statementLength = expression.length();
		for (int i = 0; i < statementLength; i++) {
			if (expression[i] == '(') {
				openingBraces++;
			}
			else if (expression[i] == ')') {
				if (openingBraces == 0) {
					return false;
				}
				closingBraces++;
			}
		}
		return openingBraces == closingBraces;
	}

	// Checks if the beginning of the expression is valid
	bool legalStart() {
		if (isNumeric(expression[0]) || isOpenBrace(expression[0]) || isDecimalPoint(expression[0]) || expression[0] == '-')
			return true;
	}

	// Checks if the end of the expression is valid
	bool legalEnding() {
		int lastElementIndex = expression.length() - 1;
		if (isNumeric(expression[lastElementIndex]) || isCloseBrace(expression[lastElementIndex])) {
			return true;
		}
		return false;
	}

	// Returns true if the character is an operation
	bool isOperation(char c) {
		if (c == '+' || c == '-' || c == '*' || c == '/' || c == '^') {
			return true;
		}
		else {
			return false;
		}
	}

	// Returns true if the character is a decimal point
	bool isDecimalPoint(char c) {
		return c == '.';
	}

	// Returns true if the character is a number
	bool isNumeric(char c) {
		return c >= '0' && c <= '9';
	}

	// Returns true if the character is an opening brace '('
	bool isOpenBrace(char c) {
		return c == '(';
	}

	// Returns true if the character is an closing brace ')'
	bool isCloseBrace(char c) {
		return c == ')';
	}

	// Gets a number value representing the precedence of the operation based on those given in the "precedenceTable" array (higher values => higher precedence)
	int getPrecedence(char c) {
		for (int i = 0; i < NUMBER_OF_OPS; i++) {
			if (c == precedenceTable[i][0]) {
				return precedenceTable[i][1] - '0';
			}
		}
		return -1;
	}

	// Push the operation into the operations stack with respect to its precedence (2 possibilities)
	//		- "operation" has equal or higher precedence than the operation at the top of the stack => push to stack
	//		- "operation" has lower precedence => evaluate the operations in the stack until an operation of equal or lower precedence is found
	void pushOperation(char operation) {
		while (!operations.isEmpty() && (getPrecedence(operations.peek()) > getPrecedence(operation))) {
			eval(operations.pop());
		}
		operations.push(operation);
	}

	// Perform the operation on the buffer (call functions based on the character)
	void eval(char operation) {
		if (operation == '+') {
			operands.add();
		}
		else if (operation == '-') {
			operands.subtract();
		}
		else if (operation == '*') {
			operands.multiply();
		}
		else if (operation == '/') {
			operands.divide();
		}
		else if (operation == '^') {
			operands.exponentiate();
		}
	}

	// Returns the result after all evaluations are done (returns top of the buffer which is where the result should be)
	double result() {
		return operands.peek();
	}
};
