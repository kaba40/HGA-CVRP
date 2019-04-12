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

	void setHead(Node *node);
	void setTail(Node *node);
	void addHead(Node *node);
	void addTail(Node *node);
	Node* getHead();
	Node* getTail();

	void push_front(Node *node);
	void push_back(Node *node);
	void insert(int position, Node *node);
	Node* find(int position);
	bool search(Node *node);
	Node* pop_front();
	Node* pop_back();
	Node* pop_position(int position);
	void removeNode(Node *node);
//	void removeDepot(Node *node);
	void delete_list();

	inline int getSize(){return size;}
	void show();
//	void showBack();

	string toString();

};

#endif /* DLINKEDLIST_HPP_ */
