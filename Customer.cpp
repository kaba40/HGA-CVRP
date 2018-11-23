/*
 * Customer.cpp
 *
 *  Created on: 23 nov. 2018
 *      Author: kkeita
 */

#include "Customer.hpp"
#include "DataAP.hpp"

Customer::Customer(int ind, int dem)
{
	index = ind;
	demand = dem;
}

Customer::~Customer() {
	// TODO Auto-generated destructor stub
}

DataAP *data_Inst;
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

	if(index1 == 0 || index2 == 0)
	{
		cerr << " index1 and index2 must be superior to 0" << endl;
		exit(-1);
	}

	if(index1 > data_Inst->getNumberNodes() || index2 > data_Inst->getNumberNodes())
	{
		cerr << " index1 and index2 must be inferior to " << data_Inst->getNumberNodes() << endl;
		exit(-1);
	}

	if( index1 == index2)
	{
		cerr << " index1 and index2 must not be equal " << endl;
		exit(-1);
	}

	if(index1 < index2)
	{
		return data_Inst->getDistances(index1, index2);
	}

	return data_Inst->getDistances(index1, index2);
}
