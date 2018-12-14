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
	Customer *client;
public:
	Node(Node *nxt, Node *prev, Customer *clt);
	virtual ~Node();

	Node* getNext();
	Node* getPrevious();
	void setNext(Node *nxt);
	void setPrevious(Node *prev);
};

#endif /* NODE_HPP_ */
