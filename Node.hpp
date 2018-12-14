/*
 * Node.hpp
 *
 *  Created on: 12 déc. 2018
 *      Author: kkeita
 */

#ifndef NODE_HPP_
#define NODE_HPP_

#include "Customer.hpp"

class Node
{
private:
	Node *next;
	Node *previous;
//	Customer *client;
	int data;
public:
	Node(int d); //Node(Customer *clt);
	Node(Node *nxt, Node *prev, int d); //Node(Node *nxt, Node *prev, Customer *clt);
	virtual ~Node();

	Node* getNext();
	Node* getPrevious();
	void setNext(Node *nxt);
	void setPrevious(Node *prev);

	int getData();
};

#endif /* NODE_HPP_ */
