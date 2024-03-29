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
	Node(Customer *clt); // constructor
	Node(Node *node); // constructor copy

	virtual ~Node();

	bool isDepot();
	bool isLastDepot();
	bool isFirstDepot();
	Node* getNext();
	Node* getPrevious();
	void setNext(Node *nxt);
	void setPrevious(Node *prev);

	void insertAfter(Node *node);
	void insertBefore(Node *node);
	void insertBetween(Node *nodePrev, Node *nodeNext);
	void removeNode();

	Customer* getClient();
};

#endif /* NODE_HPP_ */
