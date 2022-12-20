#pragma once
#include "Stack.h"

class Buffer : public Stack<double> {
public:
	void add() {
		double upper = pop(), lower = pop();
		push(upper + lower);
	}

	void subtract() {
		double upper = pop(), lower = pop();
		push(lower - upper);
	}

	void multiply() {
		double upper = pop(), lower = pop();
		push(upper * lower);
	}

	void divide() {
		double upper = pop(), lower = pop();
		push(lower / upper);
	}

	void exponentiate() {
		double upper = pop(), lower = pop();
		push(pow(lower, upper));
	}
};