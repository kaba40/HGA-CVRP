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
	Node *head;
	Node *tail;


public:
	SeqData();
	SeqData(Node *node);
	SeqData(SeqData *seq);
	virtual ~SeqData();

	// setter methods
	void setDemand(int dde);
	void setDistance(double dist);
	void setHead(Node *hd);
	void setTail(Node *tl);

	// updater methods
	void updateDemand(int dde);
	void updateDistance(double dist);

	// getter methods
	int getDemand();
	double getDistance();
	Node* getHead();
	Node* getTail();

	// concatenation methods
	SeqData* concatBackWard(SeqData *seq); // add a single visit after a sub-sequence of visits
	SeqData* concatForWard(SeqData *seq); // add a single visit before a sub-sequence of visits
};

#endif /* SEQDATA_HPP_ */
