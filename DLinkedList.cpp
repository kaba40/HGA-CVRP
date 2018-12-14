/*
 * DLinkedList.cpp
 *
 *  Created on: 12 déc. 2018
 *      Author: kkeita
 */

#include "DLinkedList.hpp"

DLinkedList::DLinkedList()
{
	this->head = NULL;
	this->tail = NULL;
	size = 0;
}

DLinkedList::DLinkedList(Node *hd, Node *tl)
{
	this->head = hd;
	this->tail = tl;
	size = 0;

}

DLinkedList::~DLinkedList() {
	// TODO Auto-generated destructor stub
}

void DLinkedList::push_front(Node *node) // add an element to the front
{
	if(!node)
		throw invalid_argument(" Non-empty list pointer can't be null");

	if(!head) // if head is null
	{
		head = node;
		size = 1;
		// why tail = node; ?
	}

	node->setNext(head); //  why this instruction "node->getNext() = head;" lvalue required as left operand of assignment error ?
	head->setPrevious(node);
	node = head; // node become head
	node->setPrevious(nullptr);
	size++;
}

void DLinkedList::push_back(Node *node) // add an element to the end
{
	if(!node)
		throw invalid_argument(" Non-empty list pointer can't be null");

	if(!tail) // if list is empty
	{
		// why head = node;
		tail = node;
		size = 1;
	}

	node->setPrevious(tail);
	tail->setNext(node);
	node = tail ; // node become tail
	node->setNext(nullptr);
	size++;
}

void DLinkedList::insert(int position, Node *node)
{
	if(!node)
		throw invalid_argument("Non-empty list pointer can't be null");

	if(position <= 0)
		push_front(node);
	else if(position >= size)
		push_back(node);
	else
	{
		if(!node)
			throw invalid_argument("Non-empty list pointer can't be null");

		Node *tmp = head;
		for(int i = 0; i < position-1; i++)
		{
			tmp = tmp->getNext();
			node->setNext(tmp->getNext());
			tmp->setNext(node);
			size++;
		}
	}
}

Node* DLinkedList::pop_front()
{
	if(!head)
		throw out_of_range("Can't delete from empty list");

	Node *tmp = nullptr;
	tmp = head;
	head = nullptr;
	delete head;

	size--;

	return tmp;

}

Node* DLinkedList::pop_back()
{
	if(!tail)
		throw string("Cannot delete from empty queue");

	Node *tmp = nullptr;
	tmp = tail;
	tail = nullptr;
	delete tail;

	size--;

	return tmp;
}

void DLinkedList::delete_list()
{
	Node *tmp = head;
	while(tmp)
	{
		Node *current = tmp;
		tmp =tmp->getNext();

		current = nullptr;
		delete current;
	}

	head = nullptr;
	tail = nullptr;
}
