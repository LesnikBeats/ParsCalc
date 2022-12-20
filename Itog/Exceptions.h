#pragma once
#include <exception>
#include <string>

class InvalidExpressionException : public std::exception {
public:
	std::string message;

	InvalidExpressionException() {
		message = "Invalid expression";
	}

	const char* what() const throw() {
		return message.c_str();
	}
};


class UnbalancedBracesException : public InvalidExpressionException {
public:
	UnbalancedBracesException() {
		message = "Unbalanced braces";
	}
};

class DecimalPointException : public InvalidExpressionException {
public:
	DecimalPointException() {
		message = "Invalid decimal point placement";
	}
};