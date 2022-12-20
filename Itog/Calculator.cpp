#include <iostream>
#include <string>
#include <stdlib.h>
#include "Stack.h"
#include "Buffer.h"
#include "Calculator.h"

using namespace std;

int main()
{
	string expression;
	cout << "----------------";
	cout << " Calculator_By_IvanKalesnik ";
	cout << "----------------" << endl;
	cout << "Type your expression to evaluate. Type 'exit' to close the program\n" << endl;
	do {
		cout << "Expression: ";
		getline(cin, expression);

		if (expression == "exit") {
			return 1;
		}

		try {
			Calculator calc = Calculator(expression); // Create the Calculator object and pass the expression as a parameter (automatic evaluation)
			cout << "RESULT: " << calc.result();
		}
		catch (InvalidExpressionException e) {
			cout << "ERROR: " << e.what() << endl;
		}
		catch (...) {
			cout << "Unhandled exception. Program will now exit";
			return -1;
		}

		cout << "\n" << endl;

	} while (true);

}