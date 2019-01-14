/*
 * SeqData.hpp
 *
 *  Created on: 14 janv. 2019
 *      Author: kkeita
 */

#ifndef SEQDATA_HPP_
#define SEQDATA_HPP_

#include "Customer.hpp"
#include "Node.hpp"
#include "DLinkedList.hpp"
class SeqData
{
private:
	int demand;
	double distance;
	Customer *client;
	Node *head;
	Node *tail;


public:
	SeqData(Customer *clt);
	virtual ~SeqData();

	DLinkedList* addForWard(Customer *clt, Node *hd, Node *tl);
};

#endif /* SEQDATA_HPP_ */
