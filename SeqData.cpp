/*
 * SeqData.cpp
 *
 *  Created on: 14 janv. 2019
 *      Author: kkeita
 */

#include "SeqData.hpp"

SeqData::SeqData()
{
	this->demand = 0;
	this->distance = 0;
//	this->client = NULL;
	this->head = NULL;
	this->tail = NULL;
}

SeqData::SeqData(Node *node)
{
	this->demand = node->getClient()->getDemand();
	this->distance = 0;
//	this->client = clt;
	this->head = node;
	this->tail = node;

}

SeqData::SeqData(SeqData *seq)
{
	this->demand = seq->getDemand();
	this->distance = seq->getDistance();
//	this->client = clt;
	this->head = seq->getHead();
	this->tail = seq->getTail();

}

SeqData::~SeqData() {
	// TODO Auto-generated destructor stub
}

// set demand of a sub-sequence
void SeqData::setDemand(int dde)
{
	this->demand = dde;
}

// set distance of a sub-sequence
void SeqData::setDistance(double dist)
{
	this->distance = dist;
}

// update demand of a sub-sequence
void SeqData::updateDemand(int dde)
{
	this->demand += dde;
}

// update distance of a sub-sequence
void SeqData::updateDistance(double dist)
{
	this->distance += dist;
}
// set head of a sub-sequence
void SeqData::setHead(Node *hd)
{
	this->head = hd;
}

// set tail of a sub-sequence
void SeqData::setTail(Node *tl)
{
	this->tail = tl;
}

// get demand of a sub-sequence
int SeqData::getDemand()
{
	return this->demand;
}

// get distance of a sub-sequence
double SeqData::getDistance()
{
	return this->distance;
}

// get head of a sub-sequence
Node* SeqData::getHead()
{
	return this->head;
}

// get tail of a sub-sequence
Node* SeqData::getTail()
{
	return this->tail;
}

// concatenate two sub-sequences using forward concatenation
SeqData* SeqData::concatForWard(SeqData *seq)
{

	// get distance between the tail of the first sub-sequence and the head of the second sub-sequence
	double distAdded;

#ifdef DEBUG_SeqData
	cout << "thisId = " << this->tail->getClient()->getId() << " " << "seqId = " << seq->getHead()->getClient()->getId()  << " " ;
	cout << "thisDemand = " << this->tail->getClient()->getDemand() << " " << "seqDemand = " << seq->getHead()->getClient()->getDemand() << endl;
#endif

	if(this->tail->getClient()->getDemand() == 0 && seq->getHead()->getClient()->getDemand() != 0)
	{
		// if the tail of the first sub-sequence is the depot
		distAdded = seq->getHead()->getClient()->getDistanceDepot();
	}
	else if(this->tail->getClient()->getDemand() != 0 && seq->getHead()->getClient()->getDemand() == 0)
	{
		// if the head of the second sub-sequence is the depot
		distAdded = this->tail->getClient()->getDistanceDepot();
	}
	else if(this->tail->getClient()->getDemand() == 0 && seq->getHead()->getClient()->getDemand() == 0)
	{
			// if the two sub-sequences are depot
			distAdded = 0;
	}
	else
	{
		// neither the tail of the first sub-sequence nor the head of the second sub-sequence is the depot
		distAdded = seq->getHead()->getClient()->getDistance(this->tail->getClient());
	}

	// copy current object in return (ret) object
	SeqData* ret = new SeqData(this);

	// update demand data
	ret->updateDemand(seq->getDemand());

	// update distance data
	ret->updateDistance(distAdded);

	// move the tail of the new sub-sequence to the tail of the added sub-sequence
	ret->setTail(seq->tail);

	return ret;
}

// concatenate two sub-sequences using backward concatenation
SeqData* SeqData::concatBackWard(SeqData *seq)
{
	// get distance between the head of the first sub-sequence and the tail of the second sub-sequence
	double distAdded;

#ifdef DEBUG_SeqData
	cout << "thisId = " << this->head->getClient()->getId() << " " << "seqId = " << seq->getTail()->getClient()->getId()  << " " ;
	cout << "thisDemand = " << this->head->getClient()->getDemand() << " " << "seqDemand = " << seq->getTail()->getClient()->getDemand() << endl;
#endif

	if(this->head->getClient()->getDemand() == 0 && seq->getTail()->getClient()->getDemand() != 0)
	{
		// if the head of the first sub-sequence is the depot then
		distAdded = seq->getTail()->getClient()->getDistanceDepot();
	}
	else if(this->head->getClient()->getDemand() != 0 && seq->getTail()->getClient()->getDemand() == 0)
	{
		// if the tail of the second sub-sequence is the depot then
		distAdded = this->head->getClient()->getDistanceDepot();
	}
	else if(this->head->getClient()->getDemand() == 0 && seq->getTail()->getClient()->getDemand() == 0)
	{
			// if the two sub-sequences are depot
			distAdded = 0;
	}
	else
	{
		// neither the head of the first sub-sequence nor the tail of the second sub-sequence is the depot
		distAdded = seq->getTail()->getClient()->getDistance(this->head->getClient());
	}

	// copy current object in return (ret) object
	SeqData* ret = new SeqData(this);

	// update demand data
	ret->updateDemand(seq->getDemand());

	// update distance data
	ret->updateDistance(distAdded);

	// move the head of the new sub-sequence to the head of the added sub-sequence
	ret->setHead(seq->head);

	return ret;
}
