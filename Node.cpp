/*
 * Node.cpp
 *
 *  Created on: 12 déc. 2018
 *      Author: kkeita
 */

#include "Node.hpp"

Node::Node(int d)
{
	this->data = d;
	this->next = NULL;
	this->previous = NULL;
}

Node::Node(Node *nxt, Node *prev, int d)
{
	this->next = nxt;
	this->previous = prev;
	this->data = d;
}

/*
Node::Node(Customer* clt)
{
	this->client = clt;
	this->next = nullptr;
	this->previous = nullptr;
}

Node::Node(Node *nxt, Node *prev, Customer *clt)
{
	this->next = nxt;
	this->previous = prev;
	this->client = clt;

}

// */
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

int Node::getData()
{
	return data;
}
