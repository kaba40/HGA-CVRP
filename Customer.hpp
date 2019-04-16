/*
 * Customer.hpp
 *
 *  Created on: 23 nov. 2018
 *      Author: kkeita
 */

#ifndef CUSTOMER_HPP_
#define CUSTOMER_HPP_

#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include <string>

using namespace std;

class DataAP;
class Customer
{
private:
	int index;
	string id;
	int demand;
	DataAP *data_Inst;
public:
	Customer(string idx, int ind, int dem, DataAP* _data);
	virtual ~Customer();

	double getDistance(Customer *c);
	double getDistanceDepot();
	string getId();
	int getIndex();
	int getDemand();
};

#endif /* CUSTOMER_HPP_ */
