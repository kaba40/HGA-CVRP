/*
 * Customer.cpp
 *
 *  Created on: 23 nov. 2018
 *      Author: kkeita
 */

#include "Customer.hpp"
#include "DataAP.hpp"

Customer::Customer(string idx, int ind, int dem, DataAP* _data)
{
	id = idx;
	index = ind;
	demand = dem;
	data_Inst = _data;
}

Customer::~Customer() {
	// TODO Auto-generated destructor stub
}

string Customer::getId()
{
	return id;
}

int Customer::getIndex()
{
	return index;
}

int Customer::getDemand()
{
	return demand;
}

double Customer::getDistance(Customer *c)
{
	int index1 = this->index+1;
	int index2 = c->index+1 ;

	return data_Inst->getDistances(index1, index2);
}

double Customer::getDistanceDepot()
{
	int index = this->index+1;
	return data_Inst->getDistances(index, 0);
}
