/*
 * Customer.cpp
 *
 *  Created on: 23 nov. 2018
 *      Author: kkeita
 */

#include "Customer.hpp"
#include "DataAP.hpp"

Customer::Customer(int ind, int dem, DataAP* _data)
{
	index = ind;
	demand = dem;
//	data_Inst = new DataAP();
	data_Inst = _data;
}

Customer::~Customer() {
	// TODO Auto-generated destructor stub
}

//DataAP *data_Inst;
//data_Inst = new DataAP();
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
	int index1 = this->index;
	int index2 = c->index;

	return data_Inst->getDistances(index1, index2);
}
