/*
 * DLinkedList.hpp
 *
 *  Created on: 12 déc. 2018
 *      Author: kkeita
 */

#ifndef DLINKEDLIST_HPP_
#define DLINKEDLIST_HPP_

#include "Node.hpp"

class DLinkedList
{
private:
Node *head;
Node *tail;
int size;
public:
	DLinkedList();
	DLinkedList(Node *hd, Node *tl);
	virtual ~DLinkedList();

	void push_front(Node *node);
	void push_back(Node *node);
	void insert(int position, Node *node);
	Node* find(int position);
	Node* pop_front();
	Node* pop_back();
	void delete_list();

	inline int getSize(){return size;}
	void show();

};

#endif /* DLINKEDLIST_HPP_ */
