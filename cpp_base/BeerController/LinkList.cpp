/*
 * LinkList.cpp
 *
 *  Created on: Apr 14, 2015
 *      Author: dhedley
 */

#include <iostream>
#include <iomanip>
using namespace std;

template <class T>
class LinkList {
private:
	struct ListNode{
		T value;
		struct ListNode *next;
	};
	ListNode *head;
	int count;
	void sortArray(T*, int);
public:
	LinkList();
	void appendNode(T);
	void insertNode(T);
	void deleteNode(T);
	void displayList() const;
	void findMaxValues(T*,int);
	int findMaxValue();
	virtual ~LinkList();
};


template <class T>
LinkList<T>::LinkList() {
	head = nullptr;
	count = 0;
}


template <class T>
LinkList<T>::~LinkList() {
	ListNode *nodePtr;
	ListNode *nextNode;

	nodePtr = head;

	while (nodePtr != nullptr) {
		nextNode = nodePtr->next;
		delete nodePtr;
		nodePtr = nextNode;
	}

}

template <class T>
void LinkList<T>::appendNode(T num) {
	ListNode *newNode;
	ListNode *nodePtr;

	newNode = new ListNode;
	newNode->value = num;
	newNode->next = nullptr;

	if (!head) {
		head = newNode;
	} else {
		nodePtr = head;
		while (nodePtr->next) {
			nodePtr = nodePtr->next;
		}
		nodePtr->next = newNode;
	}
	count++;
}
template <class T>
void LinkList<T>::deleteNode(T num) {
	ListNode *nodePtr;
	ListNode *previousNode;

	if (!head)
		return;

	if (head->value == num) {
		nodePtr = head->next;
		delete head;
		head = nodePtr;
	} else {
		nodePtr = head;
		while (nodePtr != nullptr && nodePtr->value != num) {
			previousNode = nodePtr;
			nodePtr = nodePtr->next;
		}

		if (nodePtr) {
			previousNode->next = nodePtr->next;
			delete nodePtr;
		}
	}
	count--;
}

template <class T>
void LinkList<T>::insertNode(T num) {
	ListNode *nodePtr;
	ListNode *previousNode;
	ListNode *newNode;

	previousNode = nullptr;

	newNode = new ListNode;
	newNode->value = num;
	newNode->next = nullptr;

	nodePtr = head;
	if (!head) {
		head = newNode;
	} else {
		nodePtr = head;
		while (nodePtr != nullptr && nodePtr->value < num) {
			previousNode = nodePtr;
			nodePtr = nodePtr->next;
		}
		if (previousNode == nullptr) {
			head = newNode;
			newNode->next = nodePtr;
		} else {
			previousNode->next = newNode;
			newNode->next = nodePtr;
		}
	}
	count++;
}

template <class T>
void LinkList<T>::displayList() const {

	ListNode *nodePtr;
	nodePtr = head;
	while (nodePtr) {
		cout << nodePtr->value << endl;
		nodePtr = nodePtr->next;
	}

}

template <class T>
void LinkList<T>::findMaxValues(T *array, int size) {
	ListNode *nodePtr;
	nodePtr = head;

	T* tempArray = new T[count];

	//put all nodes into an array to sort
	for (int i = 0; i < count; i++) {
		tempArray[i] = nodePtr->value;
		nodePtr = nodePtr->next;
	}

	sortArray(tempArray, count); //sort ascending

	//grab only the last items from the list needed for highest values
	//since the array is sorted ascending. And add to the incoming array.
	int sizeIndex = 0;
	for (int i = count - 1; i >= (count - size); i--) {
		array[sizeIndex] = tempArray[i];
		sizeIndex++;
	}

	delete[] tempArray;
}

template <class T>
void LinkList<T>::sortArray(T *array, int size) {

	int startScan, minIndex, minValue;

	for (startScan = 0; startScan < (size - 1); startScan++) {
		minIndex = startScan;
		minValue = array[startScan];
		for (int index = startScan + 1; index < size; index++) {
			if (array[index] < minValue) {
				minValue = array[index];
				minIndex = index;
			}
		}
		array[minIndex] = array[startScan];
		array[startScan] = minValue;
	}

}

template <class T>
int LinkList<T>::findMaxValue() {
	ListNode *nodePtr;
	nodePtr = head;

	int maxNum = nodePtr->value;
	nodePtr = nodePtr->next;

	while (nodePtr) {
		if (nodePtr->value > maxNum) {
			maxNum = nodePtr->value;
		}
		nodePtr = nodePtr->next;
	}

	return (maxNum);

}


