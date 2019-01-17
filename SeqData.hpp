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
//	Customer *client;
	Node *head;
	Node *tail;


public:
	SeqData();
	SeqData(Node *node);
	SeqData(SeqData *seq);
	virtual ~SeqData();

	// set methods
	void setDemand(int dde);
	void setDistance(double dist);
	void setHead(Node *hd);
	void setTail(Node *tl);

	// update methods
	void updateDemand(int dde);
	void updateDistance(double dist);

	// get methods
	int getDemand();
	double getDistance();
	Node* getHead();
	Node* getTail();

	// concatenation method
	SeqData* concatForWard(SeqData *seq);
};

#endif /* SEQDATA_HPP_ */
