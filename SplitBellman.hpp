/*
 * SplitBellman.hpp
 *
 *  Created on: 27 nov. 2018
 *      Author: kkeita
 */

#ifndef SPLITBELLMAN_HPP_
#define SPLITBELLMAN_HPP_

#include "DataAP.hpp"
#include "DLinkedList.hpp"


class SplitBellman
{
private:
	DataAP * tspData; //data instance
//	vector<Customer*> encod;
	DLinkedList* encod;
	vector<double> pathCost; //cost of the shortest path
	vector<int> pred; //index of the predecessor
    vector<Node*> predNode; //index of the predecessor
public:
	SplitBellman(DataAP* tspDat, DLinkedList* enc);
	virtual ~SplitBellman();
	int solve();
	vector<double>& getPathCost(void);
	vector<int>& getPredence(void);
    vector<Node*>& getPredenceNode(void);
};

#endif /* SPLITBELLMAN_HPP_ */
