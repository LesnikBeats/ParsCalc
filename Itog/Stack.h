#pragma once
#include <cstddef>

template <typename T>
struct Node
{
	T data;
	Node<T>* next; // next = below
};

template <typename T>
class Stack {
public:
	Node<T>* top = NULL;

	bool isEmpty() {
		if (top == NULL) {
			return true;
		}
		return false;
	}

	bool hasSingleElement() {
		if (!isEmpty() && top->next == NULL) {
			return true;
		}
		return false;
	}

	void push(Node<T>* newNode) {
		if (isEmpty()) {
			newNode->next = NULL;
			top = newNode;
		}
		else {
			newNode->next = top;
			top = newNode;
		}
	}

	void push(T newData) {
		Node<T>* intoTop = new Node<T>;
		intoTop->data = newData;
		push(intoTop);
	}

	T peek() {
		return top->data;
	}

	T pop() {
		Node<T>* prevTop = top;
		T topData = top->data;
		top = top->next;
		delete prevTop;
		return topData;
	}

	void destroy() {
		Node<T>* prevTop;
		while (!isEmpty()) {
			prevTop = top;
			top = top->next;
			delete prevTop;
		}
	}
};
