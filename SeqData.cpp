/*
 * SeqData.cpp
 *
 *  Created on: 14 janv. 2019
 *      Author: kkeita
 */

#include "SeqData.hpp"

SeqData::SeqData(Customer *clt)
{
	this->demand = clt->getDemand();
	this->distance = 0;
	this->client = clt;
	this->head = new Node(clt);
	this->tail = new Node(clt);

}

SeqData::~SeqData() {
	// TODO Auto-generated destructor stub
}


// add a node after a sequence
DLinkedList* SeqData::addForWard(Customer *clt, Node *hd, Node *tl)
{
	// create a node
	Node *newNode = new Node(clt);

	// push back newNode to the sub-sequence

}
