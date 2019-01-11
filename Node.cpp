/*
 * Node.cpp
 *
 *  Created on: 12 déc. 2018
 *      Author: kkeita
 */

#include "Node.hpp"



Node::Node(Customer* clt)
{
	this->client = clt;
	this->next = NULL;
	this->previous = NULL;
}

Node::Node(Node *nxt, Node *prev, Customer *clt)
{
	this->next = nxt;
	this->previous = prev;
	this->client = clt;

}

Node::~Node()
{
	// TODO Auto-generated destructor stub
}

Node* Node::getNext()
{
	return next;
}

Node* Node::getPrevious()
{
	return previous;
}

void Node::setNext(Node *nxt)
{
	this->next = nxt;
}

void Node::setPrevious(Node *prev)
{
	this->previous = prev;
}

void Node::insertAfter(Node *node)
{
	//insert a node after current node
	node->setNext(this->next);
	this->next->setPrevious(node);
	this->next = node ; //moveNode->setNext(insertNode);
	node->setPrevious(this);
}

void Node::insertBefore(Node *node)
{
	//insert a node before current node
	node->setPrevious(this->previous);
	this->previous->setNext(node);
	this->previous = node ;
	node->setNext(this);
}

void Node::insertBetween(Node *nodePrev, Node *nodeNext)
{
	// insert current node after nodePrev and before nodeNext
	nodePrev->setNext(this);
	this->previous = nodePrev;
	this->next = nodeNext;
	nodeNext->setPrevious(this);
}

Customer* Node::getClient()
{
	return this->client;
}
